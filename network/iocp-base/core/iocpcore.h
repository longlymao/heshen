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

protected:
    static LPFN_ACCEPTEX GetAcceptEx();
    static LPFN_CONNECTEX GetConnectEx();
    static LPFN_GETACCEPTEXSOCKADDRS GetGetAcceptExSockaddrs();

protected:
    IocpWorker& worker_;

    std::unordered_map<OVERLAPPED*, IocpContext*> context_map_;
};