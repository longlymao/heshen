/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */
#pragma once

#include "iocpcontext.h"
#include "core/iocpcore.h"
#include <WinSock2.h>
#include <mutex>
#include <string>
#include <winnt.h>
#include <queue>


enum class SOCKET_STATE {
	INIT,
	CONNECTING,
	CONNECTED,
	CLOSED,
};

enum class WRITE_STATE {
	IDLE,
	POSTING,
	WRITING,
};

class IocpClient : public IocpCore {
public:
	IocpClient(IocpWorker& worker);
	~IocpClient();

	void OnConnectSuccess();
	void OnConnectFailed();

	std::string GetHost() { return host; }
	uint16_t GetPort() { return port; }

	void ConnectToServer(const std::string& host, uint16_t port);

	void PostConnect(IocpContext* context);
	void Connect(IocpContext* context);

	void PostRead(IocpContext* context);
	void Read(IocpContext* context);

	void OnRecv(IocpContext* context, DWORD bytesTransferred);

	void Send(const std::string& data);
	void CheckPostWrite();
	void PostWrite(IocpContext* context);
	void Write(IocpContext* context);
	void OnWriteComplete(IocpContext* context, DWORD bytesTransferred);
	void OnWriteFailed();

private:
	std::string host;
	uint16_t port;
	
	IocpContext connect_context;
	IocpContext read_context;
	IocpContext write_context;

	SOCKET_STATE socket_state = SOCKET_STATE::INIT;

	WRITE_STATE write_state = WRITE_STATE::IDLE;
	std::queue<std::string> write_queue;
	std::mutex write_mutex;
};