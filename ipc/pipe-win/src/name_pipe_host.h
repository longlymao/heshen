#pragma once

#include <Windows.h>
#include <thread>
#include <mutex>
#include <queue>
#include <string>

namespace  rolling::ipc::pipe_win {

class NamePipeHost {
public:
    NamePipeHost();
    ~NamePipeHost();

    bool IsConnected() const;
    void Send(const std::string& message);
    
private:
    static void WorkThreadProc(NamePipeHost* host);
    void WorkThreadMain();

private:
    bool InitEvents();
    bool CreatePipe();
    void Cleanup();

    size_t PickSendData(char* buffer, size_t buffer_size);

    bool WriteData(const char* data, size_t size);
    bool ReadData(const char* buffer, size_t size);

    bool OnReadComplete();

private:
    HANDLE pipe_handle_ = INVALID_HANDLE_VALUE;
    HANDLE close_event_ = INVALID_HANDLE_VALUE;
    HANDLE send_event_ = INVALID_HANDLE_VALUE;
    HANDLE accept_complete_event_ = INVALID_HANDLE_VALUE;
    HANDLE read_complete_event_ = INVALID_HANDLE_VALUE;
    HANDLE write_complete_event_ = INVALID_HANDLE_VALUE;

    OVERLAPPED accept_overlapped_ = { 0 };
    OVERLAPPED read_overlapped_ = { 0 };
    OVERLAPPED write_overlapped_ = { 0 };

    std::atomic_bool is_connected_{ false };

    std::thread work_thread_;

    using SendItem = std::pair<size_t, std::string>;
    std::queue<SendItem> send_queue_;
    std::mutex send_queue_mutex_;
};

} // namespace rolling::ipc::pipe_win