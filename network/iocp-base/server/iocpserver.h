#pragma once

#include "core/iocpcore.h"
#include "iocpcontext.h"

class IocpServer : public IocpCore 
{
public:
    explicit IocpServer(IocpWorker& worker);

    bool listen(int port);

    void OnReadReady() override;
    void OnWriteReady() override;
    void OnConnectReady() override;
    void OnNewConnection(SOCKET client_socket) override;
    void OnClose() override;
    void OnSuccess() override;
    void OnFailed() override;

public:
    void PostAccept(IocpContext* context);
    void Accept(IocpContext* context);

private:
    SOCKET listen_socket = INVALID_SOCKET;
    IocpContext accept_context;
};