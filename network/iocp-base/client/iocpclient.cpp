/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */
#include "iocpclient.h"
#include "worker/iocpworker.h"
#include <iostream>
#include <mswsock.h>
#include <synchapi.h>
#include <ws2tcpip.h>

IocpClient::IocpClient(IocpWorker &worker) : IocpCore(worker) {
}

IocpClient::~IocpClient() {
	if(connect_context.socket != INVALID_SOCKET) {
		closesocket(connect_context.socket);
		connect_context.socket = INVALID_SOCKET;
	}
}

void IocpClient::SetRequest(const std::string &request) {
    this->request = request;
    context.buffer = request;
}

std::string IocpClient::GetResponse() {
    return response;
}

void IocpClient::OnFailed() {
    std::cerr << "Request failed" << std::endl;
}

void IocpClient::OnSuccess() {
    std::cout << "Request succeeded" << std::endl;
}

void IocpClient::OnConnectSuccess() {
    std::cout << "Connected to server successfully" << std::endl;

    int optVal = 0;
    int optLen = sizeof(optVal);
    if (setsockopt(connect_context.socket,
                   SOL_SOCKET,
                   SO_UPDATE_CONNECT_CONTEXT,
                   (char *)&optVal,
                   optLen) == SOCKET_ERROR) {
        std::cerr << "Failed to update connect context, error code: "
                  << WSAGetLastError() << std::endl;
    }

    sockaddr_in localAddr;
    int localAddrLen = sizeof(localAddr);
    getsockname(connect_context.socket, (sockaddr *)&localAddr, &localAddrLen);
    char localIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &localAddr.sin_addr, localIp, INET_ADDRSTRLEN);
    u_short localPort = ntohs(localAddr.sin_port);

    sockaddr_in serverAddr;
    int serverAddrLen = sizeof(serverAddr);
    getpeername(
        connect_context.socket, (sockaddr *)&serverAddr, &serverAddrLen);
    char serverIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &serverAddr.sin_addr, serverIp, INET_ADDRSTRLEN);
    u_short serverPort = ntohs(serverAddr.sin_port);

    std::cout << "Success to connect to server local: " << localIp << ":" << localPort
              << " -> Server: " << serverIp << ":" << serverPort << std::endl;

    read_context.socket = connect_context.socket;
    PostRead(&read_context);
}

void IocpClient::OnConnectFailed() {
    std::cerr << "Failed to connect to server " << GetHost() << ":" << GetPort() << std::endl;
}

void IocpClient::AppendResponse(const std::string &response) {
    this->response += response;
}

void IocpClient::ConnectToServer(const std::string &host, uint16_t port) {
    this->host = host;
    this->port = port;

    connect_context.buffer.resize(4096);
    connect_context.wsaBuf.buf = connect_context.buffer.data();
    connect_context.wsaBuf.len =
        static_cast<ULONG>(connect_context.buffer.size());

    connect_context.socket = WSASocketW(
        AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (connect_context.socket == INVALID_SOCKET) {
        std::cerr << "WSASocket failed with error: " << WSAGetLastError()
                  << std::endl;
        return;
    }

    struct sockaddr_in addr;
    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = 0;
    if (bind(connect_context.socket, (SOCKADDR *)&addr, sizeof(addr)) != 0) {
        std::cerr << "bind failed with error: " << WSAGetLastError()
                  << std::endl;
        return;
    }

    worker_.AssociateSocket(connect_context.socket,
                            reinterpret_cast<ULONG_PTR>(this));
    PostConnect(&connect_context);
}

void IocpClient::PostConnect(IocpContext *context) {
    context->operation = IocpOperation::TO_CONNECT;
    context->bytesTransferred = 0;
    context->overlapped = {};

    PostQueuedCompletionStatus(
        worker_.GetIoCompletionPort(),
        0,
        reinterpret_cast<ULONG_PTR>(dynamic_cast<IocpCore *>(this)),
        &context->overlapped);
}

void IocpClient::Connect(IocpContext *context) {
    LPFN_CONNECTEX connectEx = GetConnectEx();

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(GetPort());
    inet_pton(AF_INET, GetHost().c_str(), &serverAddr.sin_addr);

    context->operation = IocpOperation::CONNECT;

    int rc = connectEx(context->socket,
                       reinterpret_cast<SOCKADDR *>(&serverAddr),
                       sizeof(serverAddr),
                       nullptr,
                       0,
                       nullptr,
                       &context->overlapped);

    if (rc == TRUE) {
        OnConnectSuccess();
    } else {
        if (WSAGetLastError() != ERROR_IO_PENDING) {
            std::cerr << "ConnectEx failed with error: " << WSAGetLastError()
                      << std::endl;
            OnConnectFailed();
        }
    }
}

void IocpClient::PostRead(IocpContext* context) {
    std::cout << "PostRead called, threadid: " << GetCurrentThreadId() << std::endl;
    context->operation = IocpOperation::TO_READ;
    context->bytesTransferred = 0;
    context->overlapped = {};

    PostQueuedCompletionStatus(
        worker_.GetIoCompletionPort(),
        0,
        reinterpret_cast<ULONG_PTR>(dynamic_cast<IocpCore *>(this)),
        &context->overlapped);
}

void IocpClient::Read(IocpContext* context) {
    std::cout << "Read called, threadid: " << GetCurrentThreadId() << std::endl;
    context->buffer.resize(1024);
    context->wsaBuf.buf = context->buffer.data();
    context->wsaBuf.len = static_cast<ULONG>(context->buffer.size());
    context->operation = IocpOperation::READ;

    DWORD flags = 0;
    DWORD bytesReceived = 0;
    int rc = WSARecv(context->socket,
                     &context->wsaBuf,
                     1,
                     &bytesReceived,
                     &flags,
                     &context->overlapped,
                     nullptr);

    if(rc == 0) {
        // sorry i found that even if synchronous complete, it still will post to iocp queue
        // OnRecv(context, bytesReceived);
        // PostRead(context);
    }
    else {
        if (WSAGetLastError() != ERROR_IO_PENDING) {
            std::cerr << "WSARecv failed with error: " << WSAGetLastError()
                      << std::endl;
        }
        else{
            std::cout << "WSARecv pending, waiting for completion..." << std::endl;
        }
    }
}

void IocpClient::OnRecv(IocpContext* context, DWORD bytesTransferred) {
    if (bytesTransferred > 0) {
        std::string data(context->wsaBuf.buf, bytesTransferred);
        std::cout << "Received " << bytesTransferred << " bytes: " << data << std::endl;
    } else {
        std::cout << "Connection closed by server." << std::endl;
    }
}