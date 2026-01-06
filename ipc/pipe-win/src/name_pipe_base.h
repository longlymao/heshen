#pragma once

#include <Windows.h>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

namespace rolling::ipc::pipe_win {
    using RecvCallback = void (*)(const char *data, size_t size);

    class NamePipeBase {
      public:
        NamePipeBase();
        ~NamePipeBase();

        bool IsConnected() const;
        void SendData(const std::string &message);
        void SetRecvCallback(RecvCallback callback);

      protected:
        void Start();
        void Stop();

        virtual void Init();
        virtual void Cleanup();

        void InitBaseEvents();
        void ClearBaseEvents();

        static void WorkThreadProc(NamePipeBase *host);
        virtual void WorkThreadMain() = 0;

        void ReadWriteLoop();

      private:
        bool OnSendArrivedInternal();
        bool OnWriteCompleteInternal();
        bool OnReadCompleteInternal();

        size_t PickSendData(char *buffer, size_t buffer_size);
        bool WriteData(const char *data, size_t size);
        bool ReadData(const char *buffer, size_t size);
        void NotifyRecvCallback(const char *data, size_t size);

      public:
        constexpr static int kBufSize = 64 * 1024;

      protected:
        HANDLE pipe_handle_ = INVALID_HANDLE_VALUE;
        HANDLE close_event_ = INVALID_HANDLE_VALUE;

        std::atomic_bool is_connected_{false};

      private:
        using SendItem = std::pair<size_t, std::string>;
        std::queue<SendItem> send_queue_;
        std::mutex send_queue_mutex_;

        RecvCallback recv_callback_ = nullptr;
        std::mutex recv_callback_mutex_;

        HANDLE send_event_ = INVALID_HANDLE_VALUE;
        HANDLE read_complete_event_ = INVALID_HANDLE_VALUE;
        HANDLE write_complete_event_ = INVALID_HANDLE_VALUE;

        OVERLAPPED read_overlapped_ = {0};
        OVERLAPPED write_overlapped_ = {0};

        std::atomic_bool is_inited_{false};

        std::thread work_thread_;

        char read_buffer_[kBufSize] = {0};
        char write_buffer_[kBufSize] = {0};

        bool is_writing_ = false;
    };
} // namespace rolling::ipc::pipe_win