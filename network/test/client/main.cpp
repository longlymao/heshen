#include <iostream>

#include "client/iocpclient.h"
#include "worker/iocpworker.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    IocpWorker worker;
    IocpClient client(worker);

    client.ConnectToServer("127.0.0.1", 8080);

    system("Pause");

    Sleep(2000);
    return 0;
}