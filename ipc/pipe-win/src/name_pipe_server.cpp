#include "name_pipe_server.h"
#include "spdlog/spdlog.h"
#include <handleapi.h>
#include <ioapiset.h>
#include <minwindef.h>
#include <synchapi.h>
#include <winsock.h>


namespace rolling::ipc::pipe_win {

    NamePipeServer::NamePipeServer(const std::string &pipe_name)
        : pipe_name_(pipe_name) {
        InitCustom();
    }

    NamePipeServer::~NamePipeServer() {
        Stop();
        CleanupCustom();
    }

    void NamePipeServer::InitCustom() {
        accept_complete_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        accept_overlapped_.hEvent = accept_complete_event_;
    }

    void NamePipeServer::CleanupCustom() {
        if (accept_complete_event_ != nullptr) {
            CloseHandle(accept_complete_event_);
            accept_complete_event_ = nullptr;
        }
    }

    bool NamePipeServer::CreatePipe() {
        pipe_handle_ =
            CreateNamedPipe(pipe_name_.c_str(),
                            PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                            1,
                            kBufSize,
                            kBufSize,
                            0,
                            nullptr);

        if (pipe_handle_ == INVALID_HANDLE_VALUE) {
            SPDLOG_INFO("{}: Failed to create named pipe, lasterror: {}",
                        __FUNCTION__,
                        WSAGetLastError());
            return false;
        }
        return true;
    }

    void NamePipeServer::CleanupPipe() {
        CleanupPipeHandle();
    }

    bool NamePipeServer::AcceptClient() {
        SPDLOG_INFO("{}: Waiting for client to connect...", __FUNCTION__);
        BOOL result = ConnectNamedPipe(pipe_handle_, &accept_overlapped_);

        if (!result) {
            if (GetLastError() != ERROR_IO_PENDING) {
                SPDLOG_ERROR("{}: ConnectNamedPipe failed, lasterror: {}",
                             __FUNCTION__,
                             WSAGetLastError());
                return false;
            }
        }

        op_status_map_[eOpType::kConnect] = eOpStatus::kPending;

        HANDLE events[] = {close_event_, accept_complete_event_};

        while (true) {
            DWORD wait_result = WaitForMultipleObjects(
                sizeof(events) / sizeof(events[0]), events, FALSE, INFINITE);

            if (wait_result == WAIT_OBJECT_0) {
                SPDLOG_INFO("{}: Close event signaled", __FUNCTION__);
                break;
            } else if (wait_result == WAIT_OBJECT_0 + 1) {
                SPDLOG_INFO("{}: Accept event signaled", __FUNCTION__);
                if (!OnClientAccepted()) {
                    return false;
                } else {
                    is_connected_ = true;
                    SPDLOG_INFO("{}: Client connected", __FUNCTION__);
                    break;
                }
            } else if (wait_result == WAIT_FAILED) {
                SPDLOG_ERROR("{}: WaitForMultipleObjects failed, lasterror: {}",
                             __FUNCTION__,
                             WSAGetLastError());
                return false;
            }
        }

        return true;
    }

    void NamePipeServer::CancelAccept() {
        SPDLOG_INFO("{}: Cancelling accept operations", __FUNCTION__);
        if (op_status_map_[eOpType::kConnect] == eOpStatus::kPending) {
            SPDLOG_INFO("{}: There are pending accept operations, cancelling...",
                        __FUNCTION__);
            CancelIoEx(pipe_handle_, &accept_overlapped_);
        }

        while(op_status_map_[eOpType::kConnect] == eOpStatus::kPending) {
            DWORD wait_result = WaitForSingleObject(
                accept_complete_event_,
                INFINITE);

            SPDLOG_INFO("{}: Waiting for accept cancellation, result: {}",
                        __FUNCTION__,
                        wait_result);

            if (wait_result == WAIT_OBJECT_0) {
                SPDLOG_INFO("{}: Accept complete event signaled after cancel",
                            __FUNCTION__);
                OnClientAccepted();
            }
        }
    }

    void NamePipeServer::WorkThreadMain() {
        if (!CreatePipe()) {
            return;
        }
        if (!AcceptClient()) {
            CancelAccept();
            return;
        }
        ReadWriteLoop();
        CleanupPipe();

        SPDLOG_INFO("{}: Work thread exiting", __FUNCTION__);
    }

    bool NamePipeServer::OnClientAccepted() {
        op_status_map_[eOpType::kConnect] = eOpStatus::kIdle;

        DWORD bytes_transferred = 0;
        BOOL result = GetOverlappedResult(
            pipe_handle_, &accept_overlapped_, &bytes_transferred, FALSE);
        if (!result) {
            DWORD err = WSAGetLastError();
            if (err == ERROR_PIPE_CONNECTED) {
                SPDLOG_INFO("{}: Client already connected", __FUNCTION__);
                return true;
            }
            SPDLOG_ERROR("{}: GetOverlappedResult failed, lasterror: {}",
                         __FUNCTION__,
                         WSAGetLastError());
            return false;
        }
        return true;
    }

} // namespace rolling::ipc::pipe_win