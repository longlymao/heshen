#include "client/iocpclient.h"
#include "worker/iocpworker.h"

int main() {
    IocpWorker worker;
    IocpClient client(worker);

    client.ConnectToServer("127.0.0.1", 8080);

    for(int i = 0; i < 5; i++) {
        std::string message = "Hello from client, message " + std::to_string(i) + "\n";
        client.Send(message);
        Sleep(1000);
    }

    system("Pause");

    Sleep(2000);
    return 0;
}