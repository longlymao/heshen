#pragma once

#include "name_pipe_base.h"

namespace rolling::ipc::pipe_win
{
    class NamePipeClient : public NamePipeBase
    {
        public:
            NamePipeClient(const std::string& pipe_name);

        private:
            bool CreatePipe();

            void WorkThreadMain() override;

        private:
            std::string pipe_name_;
    };
}