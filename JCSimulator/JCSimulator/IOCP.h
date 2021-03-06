#pragma once

#include <memory>		//std::shared_ptr, std::make_shared()
#include <thread>
#include <future>		//std::future<T>, std::async()
#include <vector>

#include "TCPSocket.h"
#include "JCPacket.h"

using TCPSocketPtr	= std::shared_ptr<TCPSocket>;

struct SocketInfo
{
public:
	SocketInfo() = default;
	~SocketInfo() = default;
	SocketInfo(TCPSocket* sock);
	SocketInfo(TCPSocketPtr sock);

	TCPSocketPtr sock;
};

enum IoDataMode
{
	SEND,
	RECV
};

#define BUF_SIZE 128
class IoData : public OVERLAPPED
{
public:
	IoData();
	IoData(IoDataMode mode);
	~IoData();

	WSABUF wsaBuf;
	IoDataMode mode;
	char buff[BUF_SIZE];
	//JCPacket packet;
};

class IOCP
{
private:
	TCPSocketPtr tcpSocket;
	HANDLE comPort;

	bool acceptWorking = false;
	int AcceptWork();
	std::future<int> acceptFut;

	bool ioCompletionWorking = false;
	int IoCompletionWork();
	std::future<int> ioCompFut;

	void InitIocp();
public:
	IOCP();
	virtual ~IOCP();

	bool RunServer(std::string host, UINT16 portNum);
	bool Connect(std::string host, uint16_t portNum);

	void Send(const char* packet);
	void Send(SOCKET destSock, char* packet);
	void Send(SOCKET destSock, JCPacket& packet);

	virtual int OnAccept(SocketInfo* socketInfo) = 0;
	virtual int OnRecvPacket(SocketInfo* socketInfo, char* packet) = 0;
	virtual int OnCloseSocket() = 0;
};

