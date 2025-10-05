/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */
#include "iocpworker.h"

#include <MSWSock.h>
#include <WS2tcpip.h>
#include <iostream>

#include "client/iocpclient.h"
#include "iocpcontext.h"
#include "server/iocpserver.h"


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

        if (!result) {
            DWORD errorCode = WSAGetLastError();

            if (errorCode == ERROR_OPERATION_ABORTED) {
                std::cout << "operation has been canceled" << std::endl;
                break;
            }

            char errorMsg[256] = {0};
            FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
                           NULL,
                           errorCode,
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           errorMsg,
                           sizeof(errorMsg),
                           NULL);

            std::cerr << "GetQueuedCompletionStatus failed with error: "
                      << errorCode << " - " << errorMsg << std::endl;
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

        IocpContext *pContext =
            CONTAINING_RECORD(overlapped, IocpContext, overlapped);

        std::cout << "bytesTransferred: " << bytesTransferred
                  << " operation: " << (int)pContext->operation
                  << " threadid: " << GetCurrentThreadId() << std::endl;

        switch (pContext->operation) {
        case IocpOperation::TO_ACCEPT:
            dynamic_cast<IocpServer *>(core)->Accept(pContext);
            break;
        case IocpOperation::ACCEPT:
            dynamic_cast<IocpServer *>(core)->OnNewConnection(pContext->socket);
            dynamic_cast<IocpServer *>(core)->PostAccept(pContext);
            break;
        case IocpOperation::TO_CONNECT:
            dynamic_cast<IocpClient *>(core)->Connect(pContext);
            break;
        case IocpOperation::CONNECT:
            if (result) {
                dynamic_cast<IocpClient *>(core)->OnConnectSuccess();
            } else {
                dynamic_cast<IocpClient *>(core)->OnConnectFailed();
            }
            break;
        case IocpOperation::TO_READ:
            dynamic_cast<IocpClient *>(core)->Read(pContext);
            break;
        case IocpOperation::READ:
            if (result) {
                dynamic_cast<IocpClient *>(core)->OnRecv(pContext,
                                                         bytesTransferred);
                dynamic_cast<IocpClient *>(core)->PostRead(pContext);
            } else {
                std::cerr << "Read operation failed" << std::endl;
            }
            break;
        case IocpOperation::TO_WRITE:
            dynamic_cast<IocpClient *>(core)->Write(pContext);
            break;
        case IocpOperation::WRITE:
            if (result) {
                dynamic_cast<IocpClient *>(core)->OnWriteComplete(pContext, bytesTransferred);
            } else {
                dynamic_cast<IocpClient *>(core)->OnWriteFailed();
            }
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