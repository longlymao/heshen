#pragma once

#include "core/iocpcore.h"
#include "iocpcontext.h"

class IocpServer : public IocpCore 
{
public:
    explicit IocpServer(IocpWorker& worker);
    ~IocpServer();

    bool listen(int port);

    void OnNewConnection(SOCKET client_socket);
    void OnClose();
    void OnSuccess();
    void OnFailed();

public:
    void PostAccept(IocpContext* context);
    void Accept(IocpContext* context);

private:
    SOCKET listen_socket = INVALID_SOCKET;
    IocpContext accept_context;
};