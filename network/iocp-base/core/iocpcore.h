#pragma once

#include "iocpcontext.h"
#include <unordered_map>
#include <mswsock.h>

class IocpWorker;

class IocpCore
{
public:
    IocpCore(IocpWorker& worker) : worker_(worker) {}

    virtual ~IocpCore() = default;

    virtual void OnReadReady() = 0;
    virtual void OnWriteReady() = 0;
    virtual void OnConnectReady() = 0;
    virtual void OnNewConnection(SOCKET client_socket) = 0;
    virtual void OnClose() = 0;
    virtual void OnSuccess() = 0;
    virtual void OnFailed() = 0;

protected:
    static LPFN_ACCEPTEX GetAcceptEx();
    static LPFN_CONNECTEX GetConnectEx();
    static LPFN_GETACCEPTEXSOCKADDRS GetGetAcceptExSockaddrs();

protected:
    IocpWorker& worker_;

    std::unordered_map<OVERLAPPED*, IocpContext*> context_map_;
};