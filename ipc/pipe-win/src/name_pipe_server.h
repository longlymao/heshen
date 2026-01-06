#pragma once

#include "name_pipe_base.h"

namespace rolling::ipc::pipe_win
{
    class NamePipeServer : public NamePipeBase
    {
        public:
            NamePipeServer(const std::string& pipe_name);

        private:
            void Init() override;
            void Cleanup() override;

            void InitEvents();
            void ClearEvents();

            bool CreatePipe();
            bool AcceptClient();

            void WorkThreadMain() override;

            bool OnClientAccepted();

            private:
            HANDLE accept_complete_event_ = INVALID_HANDLE_VALUE;
            OVERLAPPED accept_overlapped_ = { 0 };

            std::string pipe_name_;
    };
}