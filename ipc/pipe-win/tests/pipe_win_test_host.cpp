#include "name_pipe_host.h"
#include <synchapi.h>

int main()
{
    rolling::ipc::pipe_win::NamePipeHost host;

    int wait_cnt = 0;
    int send_cnt = 0;

    while(true){
        if(host.IsConnected()){
            host.Send("Hello from host " + std::to_string(send_cnt));
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