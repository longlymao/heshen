/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */
#include "iocpworker.h"

#include <MSWSock.h>
#include <WS2tcpip.h>
#include <iostream>


#include "iocpcontext.h"
#include "server/iocpserver.h"

static LPFN_CONNECTEX LoadConnectEx() {
    GUID guid = WSAID_CONNECTEX;
    LPFN_CONNECTEX connectEx = nullptr;
    DWORD bytes = 0;
    SOCKET socket = WSASocketW(
        AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
    WSAIoctl(socket,
             SIO_GET_EXTENSION_FUNCTION_POINTER,
             &guid,
             sizeof(guid),
             &connectEx,
             sizeof(connectEx),
             &bytes,
             nullptr,
             nullptr);
    closesocket(socket);
    return connectEx;
}

static LPFN_CONNECTEX GetConnectEx() {
    static LPFN_CONNECTEX connectEx = LoadConnectEx();
    return connectEx;
}

IocpWorker::IocpWorker(int threadCount) : threadCount(threadCount) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    CreateIocp();
    CreateWorkThreads();
}

IocpWorker::~IocpWorker() {
    ClearWorkThread();
    ClearIocp();
}

void IocpWorker::CreateIocp() {
    iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
    if (iocp == nullptr) {
        std::cerr << "CreateIoCompletionPort failed with error: "
                  << GetLastError() << std::endl;
    }
}

void IocpWorker::CreateWorkThreads() {
    for (int i = 0; i < threadCount; i++) {
        threads.emplace_back(&IocpWorker::WorkThreadProc, this);
    }
}

void IocpWorker::ClearIocp() {
    if (iocp != nullptr) {
        CloseHandle(iocp);
        iocp = nullptr;
    }
}

void IocpWorker::ClearWorkThread() {
    for (int i = 0; i < threadCount; i++) {
        PostQueuedCompletionStatus(iocp, 0, 0, nullptr);
    }
    for (auto &thread : threads) {
        thread.join();
    }
}

void IocpWorker::WorkThreadProc() {
    DWORD bytesTransferred = 0;
    ULONG_PTR completionKey = 0;
    LPOVERLAPPED overlapped = nullptr;

    while (true) {
        BOOL result = GetQueuedCompletionStatus(
            iocp, &bytesTransferred, &completionKey, &overlapped, INFINITE);

        if (result && bytesTransferred == 0 && completionKey == 0 &&
            overlapped == nullptr) {
            std::cout
                << "GetQueuedCompletionStatus returned true with all zeros"
                << std::endl;
            break;
        }

        if (!result && overlapped == nullptr) {
            std::cerr << "GetQueuedCompletionStatus failed: " << GetLastError()
                      << std::endl;
            break;
        }

        IocpCore *core = reinterpret_cast<IocpCore *>(completionKey);
        if (core == nullptr) {
            std::cerr << "GetQueuedCompletionStatus returned nullptr core"
                      << std::endl;
            break;
        }

		std::cout << "bytesTransferred: " << bytesTransferred << std::endl;

        IocpContext *pContext =
            CONTAINING_RECORD(overlapped, IocpContext, overlapped);
        switch (pContext->operation) {
        case IocpOperation::TO_ACCEPT:
            dynamic_cast<IocpServer*>(core)->Accept(pContext);
            break;
        case IocpOperation::ACCEPT:
            dynamic_cast<IocpServer*>(core)->OnNewConnection(pContext->socket);
            dynamic_cast<IocpServer*>(core)->PostAccept(pContext);
            break;
        case IocpOperation::CONNECT:
            core->OnConnectReady();
            break;
        case IocpOperation::READ:
            core->OnReadReady();
            break;
        case IocpOperation::WRITE:
            core->OnWriteReady();
            break;
        default:
            std::cerr << "Unknown operation" << std::endl;
            break;
        }
    }
}

void IocpWorker::AssociateSocket(SOCKET socket, ULONG_PTR completionKey) {
    HANDLE result = CreateIoCompletionPort(
        reinterpret_cast<HANDLE>(socket), iocp, completionKey, 0);
    if (result == nullptr) {
        std::cerr << "CreateIoCompletionPort failed with error: "
                  << GetLastError() << std::endl;
    }
}

HANDLE IocpWorker::GetIoCompletionPort() const {
    return iocp;
}