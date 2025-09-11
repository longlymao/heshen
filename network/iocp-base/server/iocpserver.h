#pragma once

#include "core/iocpcore.h"

class IocpServer : public IocpCore 
{
public:
    void listen(int port);

    void OnReadReady() override;
    void OnWriteReady() override;
    void OnNewConnection() override;
};