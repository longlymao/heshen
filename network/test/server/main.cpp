#include <iostream>

#include "server/iocpserver.h"
#include "worker/iocpworker.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    IocpWorker worker;
    IocpServer server(worker);

    bool rst = server.listen(8080);
    if(!rst) {
        std::cerr << "Failed to start server" << std::endl;
        return -1;
    }

    system("Pause");

    Sleep(2000);
    return 0;
}