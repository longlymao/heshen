#include "name_pipe_base.h"
#include "spdlog/spdlog.h"
#include <minwindef.h>
#include <mutex>
#include <synchapi.h>
#include <winbase.h>

namespace rolling::ipc::pipe_win {
    NamePipeBase::NamePipeBase() {
        is_inited_ = true;
        InitBase();
    }

    NamePipeBase::~NamePipeBase() {
        Stop();
        CleanupBase();
    }

    bool NamePipeBase::IsConnected() const {
        return is_connected_.load();
    }

    void NamePipeBase::SendData(const std::string &message) {
        SPDLOG_INFO("{}: {}", __FUNCTION__, message);
        std::lock_guard<std::mutex> lock(send_queue_mutex_);
        send_queue_.push({0, message});
        SetEvent(send_event_);
    }

    void NamePipeBase::SetRecvCallback(RecvCallback callback) {
        SPDLOG_INFO("{}: Setting receive callback", __FUNCTION__);
        std::lock_guard<std::mutex> lock(recv_callback_mutex_);
        recv_callback_ = callback;
    }

    void NamePipeBase::Start() {
        work_thread_ = std::thread(WorkThreadProc, this);
    }

    void NamePipeBase::Stop() {
        SetEvent(close_event_);
        if (work_thread_.joinable()) {
            work_thread_.join();
        }
    }

    void NamePipeBase::InitBase() {
        close_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        send_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        read_complete_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        write_complete_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);

        read_overlapped_.hEvent = read_complete_event_;
        write_overlapped_.hEvent = write_complete_event_;

        is_inited_ =
            is_inited_ && (close_event_ != nullptr && send_event_ != nullptr &&
                           read_complete_event_ != nullptr &&
                           write_complete_event_ != nullptr);
    }

    void NamePipeBase::CleanupBase() {
        if (close_event_ != nullptr) {
            CloseHandle(close_event_);
            close_event_ = nullptr;
        }
        if (send_event_ != nullptr) {
            CloseHandle(send_event_);
            send_event_ = nullptr;
        }
        if (read_complete_event_ != nullptr) {
            CloseHandle(read_complete_event_);
            read_complete_event_ = nullptr;
        }
        if (write_complete_event_ != nullptr) {
            CloseHandle(write_complete_event_);
            write_complete_event_ = nullptr;
        }
    }

    void NamePipeBase::WorkThreadProc(NamePipeBase *host) {
        host->WorkThreadMain();
    }

    void NamePipeBase::ReadWriteLoop() {
        if (!is_inited_) {
            SPDLOG_ERROR("{}: Not initialized", __FUNCTION__);
            return;
        }
        if (!is_connected_) {
            SPDLOG_ERROR("{}: Not connected", __FUNCTION__);
            return;
        }
        if (!pipe_handle_) {
            SPDLOG_ERROR("{}: Invalid pipe handle", __FUNCTION__);
            return;
        }

        HANDLE events[] = {close_event_,
                           send_event_,
                           read_complete_event_,
                           write_complete_event_};

        if (!ReadData(read_buffer_, sizeof(read_buffer_))) {
            SPDLOG_INFO("{}: Initial ReadData failed", __FUNCTION__);
            return;
        }

        while (true) {
            DWORD wait_result = WaitForMultipleObjects(
                sizeof(events) / sizeof(events[0]), events, FALSE, INFINITE);

            if (wait_result == WAIT_OBJECT_0) {
                SPDLOG_INFO("{}: Close event signaled", __FUNCTION__);
                break;
            } else if (wait_result == WAIT_OBJECT_0 + 1) {
                SPDLOG_INFO("{}: Send event signaled", __FUNCTION__);
                if (!OnSendArrivedInternal()) {
                    break;
                }
            } else if (wait_result == WAIT_OBJECT_0 + 2) {
                SPDLOG_INFO("{}: Read complete event signaled", __FUNCTION__);
                if (!OnReadCompleteInternal()) {
                    break;
                }
            } else if (wait_result == WAIT_OBJECT_0 + 3) {
                SPDLOG_INFO("{}: Write complete event signaled", __FUNCTION__);
                if (!OnWriteCompleteInternal()) {
                    break;
                }
            }
        }
    }

    bool NamePipeBase::OnSendArrivedInternal() {
        ResetEvent(send_event_);
        if (!is_writing_) {
            size_t len = PickSendData(write_buffer_, sizeof(write_buffer_));
            if (len > 0) {
                if (!WriteData(write_buffer_, len)) {
                    return false;
                } else {
                    is_writing_ = true;
                }
            }
        }
        return true;
    }

    bool NamePipeBase::OnWriteCompleteInternal() {
        ResetEvent(write_complete_event_);
        is_writing_ = false;

        DWORD bytes_written = 0;
        BOOL result = GetOverlappedResult(
            pipe_handle_, &write_overlapped_, &bytes_written, FALSE);

        SPDLOG_INFO("{}: Wrote {} bytes", __FUNCTION__, bytes_written);
        if (!result) {
            return false;
        }
        return OnSendArrivedInternal();
    }

    bool NamePipeBase::OnReadCompleteInternal() {
        ResetEvent(read_complete_event_);
        DWORD bytes_read = 0;
        BOOL result = GetOverlappedResult(
            pipe_handle_, &read_overlapped_, &bytes_read, FALSE);

        SPDLOG_INFO("{}: Read {} bytes", __FUNCTION__, bytes_read);
        if (!result) {
            return false;
        }
        NotifyRecvCallback(read_buffer_, bytes_read);
        if (!ReadData(read_buffer_, sizeof(read_buffer_))) {
            return false;
        }
        return true;
    }

    size_t NamePipeBase::PickSendData(char *buffer, size_t buffer_size) {
        std::lock_guard<std::mutex> lock(send_queue_mutex_);
        if (send_queue_.empty()) {
            return 0;
        }

        auto &item = send_queue_.front();
        size_t bytes_to_copy =
            (std::min)(buffer_size, item.second.size() - item.first);
        memcpy(buffer, item.second.data() + item.first, bytes_to_copy);
        item.first += bytes_to_copy;

        if (item.first >= item.second.size()) {
            send_queue_.pop();
        }

        SPDLOG_INFO("{}: Picked {} bytes to send", __FUNCTION__, bytes_to_copy);

        return bytes_to_copy;
    }

    bool NamePipeBase::WriteData(const char *data, size_t size) {
        SPDLOG_INFO("{}: Writing {} bytes", __FUNCTION__, size);

        ResetEvent(write_complete_event_);

        DWORD bytes_written = 0;
        BOOL result = WriteFile(pipe_handle_,
                                data,
                                static_cast<DWORD>(size),
                                &bytes_written,
                                &write_overlapped_);

        if (!result) {
            if (GetLastError() != ERROR_IO_PENDING) {
                SPDLOG_INFO("{}: WriteFile failed, lasterror: {}",
                            __FUNCTION__,
                            GetLastError());
                return false;
            }
        }

        return true;
    }

    bool NamePipeBase::ReadData(const char *buffer, size_t size) {
        SPDLOG_INFO("{}", __FUNCTION__);

        ResetEvent(read_complete_event_);

        DWORD bytes_read = 0;
        BOOL result = ReadFile(pipe_handle_,
                               (LPVOID)buffer,
                               static_cast<DWORD>(size),
                               &bytes_read,
                               &read_overlapped_);

        if (!result) {
            if (GetLastError() != ERROR_IO_PENDING) {
                SPDLOG_INFO("{}: ReadFile failed, lasterror: {}",
                            __FUNCTION__,
                            GetLastError());
                return false;
            }
        }

        return true;
    }

    void NamePipeBase::NotifyRecvCallback(const char *data, size_t size){
        std::lock_guard<std::mutex> lock(recv_callback_mutex_);
        if (recv_callback_) {
            recv_callback_(data, size);
        }
    }

} // namespace rolling::ipc::pipe_win