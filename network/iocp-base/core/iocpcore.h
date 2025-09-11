#pragma once

#include "iocpcontext.h"
#include <unordered_map>

class IocpCore
{
public:
    virtual ~IocpCore() = default;

    virtual void OnReadReady() = 0;
    virtual void OnWriteReady() = 0;
    virtual void OnConnectReady() = 0;
    virtual void OnNewConnection() = 0;
    virtual void OnClose() = 0;
    virtual void OnSuccess() = 0;
    virtual void OnFailed() = 0;

private:
    std::unordered_map<OVERLAPPED*, IocpContext*> context_map_;
};