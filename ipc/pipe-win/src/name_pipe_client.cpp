#include "name_pipe_client.h"
#include <handleapi.h>
#include <ioapiset.h>
#include <minwindef.h>
#include <winsock.h>
#include "spdlog/spdlog.h"

namespace rolling::ipc::pipe_win {

    NamePipeClient::NamePipeClient(const std::string& pipe_name)
        : pipe_name_(pipe_name) {
    }  

    bool NamePipeClient::CreatePipe() {
        pipe_handle_ = CreateFileA(
            pipe_name_.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED,
            nullptr
        );

        if(pipe_handle_ == INVALID_HANDLE_VALUE){
            SPDLOG_INFO("{}: Failed to create named pipe, lasterror: {}", __FUNCTION__, WSAGetLastError());
            return false;
        }
        return true;
    }

    void NamePipeClient::WorkThreadMain() {
        if(!CreatePipe()){
            return;
        }
        else{
            is_connected_ = true;
        }
        ReadWriteLoop();
    }
} // namespace rolling::ipc::pipe_win