#include "iocpserver.h"
#include "worker/iocpworker.h"
#include <WinSock2.h>
#include <ioapiset.h>
#include <iostream>
#include <mswsock.h>
#include <ws2tcpip.h>

IocpServer::IocpServer(IocpWorker &worker) : IocpCore(worker) {
    accept_context.buffer.resize((sizeof(sockaddr_in) + 16) * 2); 
}

bool IocpServer::listen(int port) {
    listen_socket = WSASocketW(
        AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (listen_socket == INVALID_SOCKET) {
        std::cerr << "Failed to create listen socket: " << WSAGetLastError()
                  << std::endl;
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    for(char * c = reinterpret_cast<char*>(&serverAddr); c < reinterpret_cast<char*>(&serverAddr) + sizeof(serverAddr); c++) {
        std::cout << std::hex << (int)(unsigned char)(*c) << " ";
    }
    std::cout << std::dec << std::endl;

    if (bind(listen_socket, (SOCKADDR *)&serverAddr, sizeof(serverAddr)) ==
        SOCKET_ERROR) {
        std::cerr << "Failed to bind listen socket: " << WSAGetLastError()
                  << std::endl;
        closesocket(listen_socket);
        return false;
    }

    if (::listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen on socket: " << WSAGetLastError()
                  << std::endl;
        closesocket(listen_socket);
        return false;
    }

    worker_.AssociateSocket(listen_socket, reinterpret_cast<ULONG_PTR>(this));
    PostAccept(&accept_context);
    return true;
}

void IocpServer::OnReadReady() {
    // Handle read events
}

void IocpServer::OnWriteReady() {
    // Handle write events
}

void IocpServer::OnConnectReady() {
    // Handle connect events
}

void IocpServer::OnNewConnection(SOCKET client_socket) {
    LPFN_GETACCEPTEXSOCKADDRS getAcceptExSockaddrs = GetGetAcceptExSockaddrs();

    LPSOCKADDR local = nullptr;
    LPSOCKADDR remote = nullptr;
    int localLen = 0;
    int remoteLen = 0;
    getAcceptExSockaddrs(accept_context.buffer.data(), 0,
                         sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
                         &local, &localLen, &remote, &remoteLen);


    char localIp[INET_ADDRSTRLEN] = {0};
    char remoteIp[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &((sockaddr_in*)local)->sin_addr, localIp, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &((sockaddr_in*)remote)->sin_addr, remoteIp, INET_ADDRSTRLEN);   
    int localPort = ntohs(((sockaddr_in*)local)->sin_port);
    int remotePort = ntohs(((sockaddr_in*)remote)->sin_port);
    std::cout << "Local Address: " << localIp << ":" << localPort << std::endl;
    std::cout << "Remote Address: " << remoteIp << ":" << remotePort << std::endl;
}

void IocpServer::OnClose() {
    // Handle close events
}

void IocpServer::OnSuccess() {
    std::cout << "Operation succeeded" << std::endl;
}

void IocpServer::OnFailed() {
    std::cerr << "Operation failed" << std::endl;
}

void IocpServer::PostAccept(IocpContext *context) {
    context->operation = IocpOperation::TO_ACCEPT;
    context->wsaBuf.len = 0;
    context->wsaBuf.buf = nullptr;
    context->bytesTransferred = 0;

    PostQueuedCompletionStatus(
        worker_.GetIoCompletionPort(),
        0,
        reinterpret_cast<ULONG_PTR>(dynamic_cast<IocpCore *>(this)),
        &context->overlapped);
}

void IocpServer::Accept(IocpContext *context) {
    LPFN_ACCEPTEX acceptEx = GetAcceptEx();

    SOCKET accept_socket = WSASocketW(
        AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (accept_socket == INVALID_SOCKET) {
        std::cerr << "WSASocket failed with error: " << WSAGetLastError()
                  << std::endl;
        return;
    }

    DWORD bytesReceived = 0;
    context->socket = accept_socket;
    context->operation = IocpOperation::ACCEPT;
    context->wsaBuf.buf = context->buffer.data();
    context->wsaBuf.len = context->buffer.size();
    context->bytesTransferred = 0;

    int rc = acceptEx(listen_socket,
                      accept_socket,
                      context->wsaBuf.buf,
                      0,
                      sizeof(sockaddr_in) + 16,
                      sizeof(sockaddr_in) + 16,
                      &bytesReceived,
                      &context->overlapped);

    if (rc == TRUE) {
        OnNewConnection(accept_socket);
        PostAccept(context);
    } else {
        if (WSAGetLastError() != ERROR_IO_PENDING) {
            std::cerr << "AcceptEx Failed: " << WSAGetLastError() << std::endl;
            closesocket(accept_socket);
        }
    }
}