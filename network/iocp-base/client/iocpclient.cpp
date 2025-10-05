/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */
#include "iocpclient.h"
#include "worker/iocpworker.h"
#include <iostream>
#include <mswsock.h>
#include <mutex>
#include <processthreadsapi.h>
#include <synchapi.h>
#include <ws2tcpip.h>

IocpClient::IocpClient(IocpWorker &worker) : IocpCore(worker) {
}

IocpClient::~IocpClient() {
    if (connect_context.socket != INVALID_SOCKET) {
        CancelIoEx((HANDLE)connect_context.socket, NULL);
        closesocket(connect_context.socket);
        connect_context.socket = INVALID_SOCKET;
        read_context.socket = INVALID_SOCKET;
        write_context.socket = INVALID_SOCKET;
    }
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

    std::cout << "Success to connect to server local: " << localIp << ":"
              << localPort << " -> Server: " << serverIp << ":" << serverPort
              << std::endl;

    socket_state = SOCKET_STATE::CONNECTED;

    read_context.socket = connect_context.socket;
    PostRead(&read_context);
    write_context.socket = connect_context.socket;
    CheckPostWrite();
}

void IocpClient::OnConnectFailed() {
    std::cerr << "Failed to connect to server " << GetHost() << ":" << GetPort()
              << std::endl;

    socket_state = SOCKET_STATE::CLOSED;
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
    std::cout << "PostConnect called, threadid: " << GetCurrentThreadId() << std::endl;

    socket_state = SOCKET_STATE::CONNECTING;

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

void IocpClient::PostRead(IocpContext *context) {
    std::cout << "PostRead called, threadid: " << GetCurrentThreadId()
              << std::endl;
    context->operation = IocpOperation::TO_READ;
    context->bytesTransferred = 0;
    context->overlapped = {};

    PostQueuedCompletionStatus(
        worker_.GetIoCompletionPort(),
        0,
        reinterpret_cast<ULONG_PTR>(dynamic_cast<IocpCore *>(this)),
        &context->overlapped);
}

void IocpClient::Read(IocpContext *context) {
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

    if (rc == 0) {
        // sorry i found that even if synchronous complete, it still will post
        // to iocp queue OnRecv(context, bytesReceived); PostRead(context);
    } else {
        if (WSAGetLastError() != ERROR_IO_PENDING) {
            std::cerr << "WSARecv failed with error: " << WSAGetLastError()
                      << std::endl;
        } else {
            std::cout << "WSARecv pending, waiting for completion..."
                      << std::endl;
        }
    }
}

void IocpClient::OnRecv(IocpContext *context, DWORD bytesTransferred) {
    if (bytesTransferred > 0) {
        std::string data(context->wsaBuf.buf, bytesTransferred);
        std::cout << "Received " << bytesTransferred << " bytes: " << data
                  << std::endl;
    } else {
        std::cout << "Connection closed by server." << std::endl;
    }
}

void IocpClient::Send(const std::string &data) {
    {
        std::lock_guard<std::mutex> lock(write_mutex);
        write_queue.push(data);
    }
    CheckPostWrite();
}

void IocpClient::CheckPostWrite() {
    std::lock_guard<std::mutex> lock(write_mutex);
    if (socket_state == SOCKET_STATE::CONNECTED && write_state == WRITE_STATE::IDLE && !write_queue.empty()) {
        write_context.buffer = write_queue.front();
        write_queue.pop();
        write_state = WRITE_STATE::POSTING;
        PostWrite(&write_context);
    }
}

void IocpClient::PostWrite(IocpContext *context) {
    std::cout << "PostWrite called, threadid: " << GetCurrentThreadId()
              << std::endl;
    context->operation = IocpOperation::TO_WRITE;
    context->bytesTransferred = 0;
    context->overlapped = {};

    PostQueuedCompletionStatus(
        worker_.GetIoCompletionPort(),
        0,
        reinterpret_cast<ULONG_PTR>(dynamic_cast<IocpCore *>(this)),
        &context->overlapped);
}

void IocpClient::Write(IocpContext *context) {
    std::cout << "Write called, threadid: " << GetCurrentThreadId()
              << std::endl;
    context->operation = IocpOperation::WRITE;
    context->wsaBuf.buf = const_cast<char *>(context->buffer.data()) + context->bytesTransferred;
    context->wsaBuf.len = static_cast<ULONG>(context->buffer.size() - context->bytesTransferred);

    DWORD bytesSent = 0;
    int rc = WSASend(context->socket,
                     &context->wsaBuf,
                     1,
                     &bytesSent,
                     0,
                     &context->overlapped,
                     nullptr);

    if (rc == 0) {
        // sorry i found that even if synchronous complete, it still will post
        // to iocp queue OnWrite(context, bytesSent); CheckPostWrite();
    } else {
        if (WSAGetLastError() != ERROR_IO_PENDING) {
            std::cerr << "WSASend failed with error: " << WSAGetLastError()
                      << std::endl;
            {
                std::lock_guard<std::mutex> lock(write_mutex);
                write_state = WRITE_STATE::IDLE;
            }
        } else {
            std::cout << "WSASend pending, waiting for completion..."
                      << std::endl;
            {
                std::lock_guard<std::mutex> lock(write_mutex);
                write_state = WRITE_STATE::WRITING;
            }
        }
    }
}

void IocpClient::OnWriteComplete(IocpContext *context, DWORD bytesTransferred) {
    std::string data(context->wsaBuf.buf, bytesTransferred);
    std::cout << "OnWriteComplete called, threadid: " << GetCurrentThreadId()
              << ", sent data: " << data << std::endl;

    context->bytesTransferred += bytesTransferred;
    if (context->bytesTransferred < context->buffer.size()) {
        // Not all data sent, continue sending
        Write(context);
    } else {
        std::cout << "Sent " << context->bytesTransferred << " bytes successfully." << std::endl;
        {
            std::lock_guard<std::mutex> lock(write_mutex);
            write_state = WRITE_STATE::IDLE;
        }
        CheckPostWrite();
    }
}

void IocpClient::OnWriteFailed() {
    std::cerr << "Write operation failed." << std::endl;
}