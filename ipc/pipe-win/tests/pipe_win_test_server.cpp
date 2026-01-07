#include "name_pipe_server.h"
#include <synchapi.h>
#include "spdlog/spdlog.h"

constexpr std::string_view kPipeName = R"(\\.\pipe\rolling_ipc_pipe_test)";

int main()
{
    auto OnRecv = [](const char* data, size_t len){
        std::string msg(data, len);
        SPDLOG_INFO("Host received message: {}", msg);
    };

    rolling::ipc::pipe_win::NamePipeServer server(kPipeName.data());

    server.SetRecvCallback(OnRecv);
    server.Start();

    int wait_cnt = 0;
    int send_cnt = 0;

    while(true){
        if(server.IsConnected()){
            server.SendData("Hello from server " + std::to_string(send_cnt));
            send_cnt++;

            if(send_cnt > 5){
                break;
            }
        }
        else{
            wait_cnt++;
            if(wait_cnt > 10){
                break;
            }
        }
        Sleep(1000);
    }

    Sleep(1000);

    return 0;
}