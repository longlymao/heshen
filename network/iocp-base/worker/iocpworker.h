/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */
#pragma once

#include <WinSock2.h>
#include <vector>
#include <thread>
#include <winsock2.h>
#include <windows.h>

class IocpServer;

class IocpWorker {
    constexpr static int THREAD_COUNT_DEFAULT = 2;

public:
    explicit IocpWorker(int threadCount = THREAD_COUNT_DEFAULT);
    ~IocpWorker();

private:
    void CreateIocp();
    void CreateWorkThreads();

    void ClearIocp();
    void ClearWorkThread();

    void WorkThreadProc();

public:
    void AssociateSocket(SOCKET socket, ULONG_PTR completionKey);
    HANDLE GetIoCompletionPort() const;

private:
    HANDLE iocp = nullptr;
    std::vector<std::thread> threads;
    int threadCount;
};