#include "name_pipe_host.h"
#include <handleapi.h>
#include <mutex>
#include <winbase.h>

#include "spdlog/spdlog.h"

namespace rolling::ipc::pipe_win {

    constexpr int kBufSize = 1024;

NamePipeHost::NamePipeHost() {
    work_thread_ = std::thread(WorkThreadProc, this);
}

NamePipeHost::~NamePipeHost() {
    SetEvent(close_event_);
    if (work_thread_.joinable()) {
        work_thread_.join();
    }
    Cleanup();
}

bool NamePipeHost::IsConnected() const {
    return is_connected_.load();
}

void NamePipeHost::Send(const std::string& message) {
    SPDLOG_INFO("{}: {}", __FUNCTION__, message);

    std::lock_guard<std::mutex>  lock(send_queue_mutex_);
    send_queue_.push({0, message});
    SetEvent(send_event_);
}

void NamePipeHost::WorkThreadProc(NamePipeHost* host) {
    host->WorkThreadMain();
}

void NamePipeHost::WorkThreadMain() {
    if(!CreatePipe()){
        return;
    }

    if(!InitEvents()){
        return;
    }

    bool is_writing = false;

    char buffer_to_send[kBufSize] = {0};
    char buffer_to_read[kBufSize] = {0};

    HANDLE events[] = {
        close_event_,
        send_event_,
        accept_complete_event_,
        read_complete_event_,
        write_complete_event_
    };

    while (true) {
        DWORD wait_result = WaitForMultipleObjects(
            sizeof(events) / sizeof(events[0]),
            events,
            FALSE,
            INFINITE
        );

        if (wait_result == WAIT_OBJECT_0) {
            // Close event signaled
            break;
        }
        else if(wait_result == WAIT_OBJECT_0 + 1){
            // Send event signaled
            if(!is_writing){
                size_t len = PickSendData(buffer_to_send, sizeof(buffer_to_send));
                if(!WriteData(buffer_to_send, len)){
                    break;
                }
                else{
                    is_writing = true;
                }
            }

            if(!ReadData(buffer_to_read, sizeof(buffer_to_read))){
                break;
            }
        }
        else if(wait_result == WAIT_OBJECT_0 + 2){
            // Accept event signaled
            is_connected_.store(true);
            size_t len = PickSendData(buffer_to_send, sizeof(buffer_to_send));
            if(!WriteData(buffer_to_send, len)){
                break;
            }
            else{
                is_writing = true;
            }
        }
        else if(wait_result == WAIT_OBJECT_0 + 3){
            // Read complete event signaled
            if(!OnReadComplete()){
                break;
            }
            if(!ReadData(buffer_to_read, sizeof(buffer_to_read))){
                break;
            }
        }
        else if(wait_result == WAIT_OBJECT_0 + 4){
            // Write complete event signaled
            is_writing = false;
            size_t len = PickSendData(buffer_to_send, sizeof(buffer_to_send));
            if(!WriteData(buffer_to_send, len)){
                break;
            }
            else{
                is_writing = true;
            }
        }

        // Handle other events...
    }
}

bool NamePipeHost::InitEvents()
{
    close_event_ = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    send_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    accept_complete_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    read_complete_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    write_complete_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    if (close_event_ == nullptr || send_event_ == nullptr ||
        accept_complete_event_ == nullptr || read_complete_event_ == nullptr || write_complete_event_ == nullptr) {
        Cleanup();
        return false;
    }

    accept_overlapped_.hEvent = accept_complete_event_;
    read_overlapped_.hEvent = read_complete_event_;
    write_overlapped_.hEvent = write_complete_event_;

    return true;
}

bool NamePipeHost::CreatePipe()
{
    pipe_handle_ = CreateNamedPipe(
        R"(\\.\pipe\MyNamedPipe)",
        PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        kBufSize,
        kBufSize,
        0,
        nullptr
    );

    return pipe_handle_ != INVALID_HANDLE_VALUE;
}

void NamePipeHost::Cleanup()
{
    if(pipe_handle_){
        CloseHandle(pipe_handle_);
        pipe_handle_ = INVALID_HANDLE_VALUE;
    }
}

size_t NamePipeHost::PickSendData(char* buffer, size_t buffer_size)
{
    std::lock_guard<std::mutex>  lock(send_queue_mutex_);
    if(send_queue_.empty()){
        return 0;
    }

    auto& item = send_queue_.front();
    size_t bytes_to_copy = (std::min)(buffer_size, item.second.size() - item.first);
    memcpy(buffer, item.second.data() + item.first, bytes_to_copy);
    item.first += bytes_to_copy;

    if(item.first >= item.second.size()){
        send_queue_.pop();
    }

    SPDLOG_INFO("{}: Picked {} bytes to send", __FUNCTION__, bytes_to_copy);

    return bytes_to_copy;
}

bool NamePipeHost::WriteData(const char* data, size_t size)
{
    SPDLOG_INFO("{}: Writing {} bytes", __FUNCTION__, size);

    ResetEvent(write_complete_event_);

    DWORD bytes_written = 0;
    BOOL result = WriteFile(
        pipe_handle_,
        data,
        static_cast<DWORD>(size),
        &bytes_written,
        &write_overlapped_
    );

    if (!result) {
        if (GetLastError() != ERROR_IO_PENDING) {
            // Handle error
            return false;
        }
    }

    return true;
}

bool NamePipeHost::ReadData(const char* buffer, size_t size)
{
    SPDLOG_INFO("{}", __FUNCTION__);

    ResetEvent(read_complete_event_);

    DWORD bytes_read = 0;
    BOOL result = ReadFile(
        pipe_handle_,
        (LPVOID)buffer,
        static_cast<DWORD>(size),
        &bytes_read,
        &read_overlapped_
    );

    if (!result) {
        if (GetLastError() != ERROR_IO_PENDING) {
            // Handle error
            return false;
        }
    }

    return true;
}

bool NamePipeHost::OnReadComplete()
{
    DWORD bytes_read = 0;
    BOOL result = GetOverlappedResult(
        pipe_handle_,
        &read_overlapped_,
        &bytes_read,
        FALSE
    );

    SPDLOG_INFO("{}: Read {} bytes", __FUNCTION__, bytes_read);

    if (!result) {
        // Handle error
        return false;
    }
    return true;
}


} // namespace rolling::ipc::pipe_win