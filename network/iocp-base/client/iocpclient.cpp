/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */
#include "iocpclient.h"
#include "worker/iocpworker.h"
#include <mswsock.h>
#include <mutex>
#include <processthreadsapi.h>
#include <synchapi.h>
#include <ws2tcpip.h>
#include "spdlog/spdlog.h"

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
    spdlog::info("Connected to server successfully");

    int optVal = 0;
    int optLen = sizeof(optVal);
    if (setsockopt(connect_context.socket,
                   SOL_SOCKET,
                   SO_UPDATE_CONNECT_CONTEXT,
                   (char *)&optVal,
                   optLen) == SOCKET_ERROR) {
        spdlog::error("Failed to update connect context, error code: {}",
                      WSAGetLastError());
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

    spdlog::info("Success to connect to server local: {}:{} -> Server: {}:{}",
                  localIp, localPort, serverIp, serverPort);

    socket_state = SOCKET_STATE::CONNECTED;

    read_context.socket = connect_context.socket;
    PostRead(&read_context);
    write_context.socket = connect_context.socket;
    CheckPostWrite();
}

void IocpClient::OnConnectFailed() {
    spdlog::error("Failed to connect to server {}:{}", GetHost(), GetPort());

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
        spdlog::error("WSASocket failed with error: {}", WSAGetLastError());
        return;
    }

    struct sockaddr_in addr;
    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = 0;
    if (bind(connect_context.socket, (SOCKADDR *)&addr, sizeof(addr)) != 0) {
        spdlog::error("bind failed with error: {}", WSAGetLastError());
        return;
    }

    worker_.AssociateSocket(connect_context.socket,
                            reinterpret_cast<ULONG_PTR>(this));
    PostConnect(&connect_context);
}

void IocpClient::PostConnect(IocpContext *context) {
    spdlog::info("PostConnect called, threadid: {}", GetCurrentThreadId());

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
            spdlog::error("ConnectEx failed with error: {}", WSAGetLastError());
            OnConnectFailed();
        }
    }
}

void IocpClient::PostRead(IocpContext *context) {
    spdlog::info("PostRead called, threadid: {}", GetCurrentThreadId());
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
    spdlog::info("Read called, threadid: {}", GetCurrentThreadId());
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
            spdlog::error("WSARecv failed with error: {}", WSAGetLastError());
        } else {
            spdlog::info("WSARecv pending, waiting for completion...");
        }
    }
}

void IocpClient::OnRecv(IocpContext *context, DWORD bytesTransferred) {
    if (bytesTransferred > 0) {
        std::string data(context->wsaBuf.buf, bytesTransferred);
        spdlog::info("Received {} bytes: {}", bytesTransferred, data);
    } else {
        spdlog::info("Connection closed by server.");
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
    spdlog::info("PostWrite called, threadid: {}", GetCurrentThreadId());
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
    spdlog::info("Write called, threadid: {}", GetCurrentThreadId());
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
            spdlog::error("WSASend failed with error: {}", WSAGetLastError());
            {
                std::lock_guard<std::mutex> lock(write_mutex);
                write_state = WRITE_STATE::IDLE;
            }
        } else {
            spdlog::info("WSASend pending, waiting for completion...");
            {
                std::lock_guard<std::mutex> lock(write_mutex);
                write_state = WRITE_STATE::WRITING;
            }
        }
    }
}

void IocpClient::OnWriteComplete(IocpContext *context, DWORD bytesTransferred) {
    std::string data(context->wsaBuf.buf, bytesTransferred);
    spdlog::info("OnWriteComplete called, threadid: {}, sent data: {}", GetCurrentThreadId(), data);

    context->bytesTransferred += bytesTransferred;
    if (context->bytesTransferred < context->buffer.size()) {
        // Not all data sent, continue sending
        Write(context);
    } else {
        spdlog::info("Sent {} bytes successfully.", context->bytesTransferred);
        {
            std::lock_guard<std::mutex> lock(write_mutex);
            write_state = WRITE_STATE::IDLE;
        }
        CheckPostWrite();
    }
}

void IocpClient::OnWriteFailed() {
    spdlog::error("Write operation failed.");
}