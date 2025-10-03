/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */
#pragma once

#include "iocpcontext.h"
#include "core/iocpcore.h"
#include <string>

class IocpClient : public IocpCore {
public:
	IocpClient(IocpWorker& worker);
	~IocpClient();

	void SetRequest(const std::string& request);
	std::string GetResponse();

	void OnFailed();
	void OnSuccess();

	void OnConnectSuccess();
	void OnConnectFailed();

	void AppendResponse(const std::string& response);

	IocpContext& GetContext() { return context; }
	std::string GetHost() { return host; }
	uint16_t GetPort() { return port; }

	void ConnectToServer(const std::string& host, uint16_t port);

	void PostConnect(IocpContext* context);
	void Connect(IocpContext* context);

private:
	IocpContext context;
	std::string host;
	uint16_t port;

	std::string request;
	std::string response;
	
	IocpContext connect_context;
	IocpContext read_context;
	IocpContext write_context;
};