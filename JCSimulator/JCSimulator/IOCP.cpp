#include "IOCP.h"


SocketInfo::SocketInfo(TCPSocket* sock) : sock(std::make_shared<TCPSocket>(sock->GetSocket(), sock->GetSocketAddress()))
{
}

SocketInfo::SocketInfo(TCPSocketPtr sock) : sock(sock)
{
}


IoData::IoData()
{
}

IoData::IoData(IoDataMode mode) :OVERLAPPED({ 0 }), wsaBuf({ BUF_SIZE, buff }), mode(mode)
{
}

IoData::~IoData()
{
}


IOCP::IOCP() : tcpSocket(std::make_shared<TCPSocket>())
{
}

IOCP::~IOCP()
{
	acceptWorking = false;
	ioCompletionWorking = false;


	tcpSocket->CleanUp();
}

int IOCP::AcceptWork()
{
	acceptWorking = true;
	while(acceptWorking)
	{
		TCPSocket* clientSocket = tcpSocket->Accept();
		if (clientSocket == nullptr)
		{
			SocketUtil::GetLastSocketError();
			continue;
		}
		fprintf(stdout, "client connected...\n");
		SocketInfo* socketInfo = new SocketInfo(clientSocket);

		OnAccept(socketInfo);

		HANDLE port = CreateIoCompletionPort(
			(HANDLE)clientSocket->GetSocket(),
			comPort,
			(DWORD)socketInfo,
			0);

		if (port != comPort)
		{
			fprintf(stderr, "Create Client IoCompletionPort Error...\n");
			continue;
		}

		IoData* ioData = new IoData(RECV);
	
		DWORD recvBytes, flags = 0;
		WSARecv(
			clientSocket->GetSocket(),
			&ioData->wsaBuf,
			1,
			&recvBytes,
			&flags,
			ioData,
			NULL);
	}

	return 0;
}

int IOCP::IoCompletionWork()
{
	ioCompletionWorking = true;
	while (ioCompletionWorking)
	{
		DWORD bytesTrans;
		SocketInfo* socketInfo;
		IoData* ioData;

		GetQueuedCompletionStatus(
			comPort,
			&bytesTrans,
			(PULONG_PTR)&socketInfo,
			(LPOVERLAPPED*)&ioData,
			INFINITE);

		fprintf(stdout, "after evt recv : %s\n", ioData->wsaBuf.buf);
		TCPSocketPtr clientSocket = socketInfo->sock;

		if (bytesTrans == 0)    //close socket
		{
			//closesocket(clientSocket->GetSocket());
			delete socketInfo;
			delete ioData;

			OnCloseSocket();
			continue;
		}

		if (ioData->mode == SEND)
		{
			//Onsend
			delete ioData;
			continue;
		}
		else if (ioData->mode == RECV)
		{
			OnRecvPacket(socketInfo, ioData->wsaBuf.buf);

			DWORD flags = 0;
			WSARecv(clientSocket->GetSocket(),
				&ioData->wsaBuf,
				1,
				NULL,
				&flags,
				ioData,
				NULL);
		}
	}

	return 0;
}

void IOCP::InitIocp()
{
	comPort = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL,
		0,
		0);

	//set work thread, num of thread is (process cnt * 2)
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	for (int i = 0; i < sysInfo.dwNumberOfProcessors; i++)
	{
		//ioCompFut = std::async(&IOCP::IoCompletionWork, this);
		std::thread workThread(&IOCP::IoCompletionWork, this);
		workThread.detach();
	}
}

bool IOCP::RunServer(std::string host, UINT16 portNum)
{
	printf("RunServer Called\n");

	if (!tcpSocket->Init())
	{
		return false;
	}

	InitIocp();

	tcpSocket->SetSocket(WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED));

	if (!tcpSocket->Bind(host, portNum))
	{
		return false;
	}

	if (!tcpSocket->Listen(30))
	{
		return false;
	}


	//acceptFut = std::async(&IOCP::AcceptWork, this);
	std::thread acpt(&IOCP::AcceptWork, this);
	acpt.detach();

	return true;
}

bool IOCP::Connect(std::string host, uint16_t portNum)
{
	DWORD recvBytes, flags = 0;

	if (!tcpSocket->Init())
	{
		return false;
	}

	comPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (!comPort)
	{
		return false;
	}

	InitIocp();

	tcpSocket->SetSocket(WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED));
	if (tcpSocket->GetSocket() == INVALID_SOCKET)
	{
		return false;
	}

	SocketInfo* socketInfo = new SocketInfo(tcpSocket);
	CreateIoCompletionPort((HANDLE)tcpSocket->GetSocket(), comPort, (ULONG_PTR)socketInfo, 0);

	SocketAddress sockAddr = SocketAddress::CreateTCPSocket(host, portNum);
	WSAConnect(tcpSocket->GetSocket(), (SOCKADDR*)&sockAddr, sockAddr.addrLen, NULL, NULL, NULL, NULL);

	IoData* ioData = new IoData(RECV);
	WSARecv(
		tcpSocket->GetSocket(),
		&ioData->wsaBuf,
		1,
		&recvBytes,
		&flags,
		ioData,
		NULL);

	fprintf(stdout, "connected...\n");
	return true;
}

void IOCP::Send(const char* packet)
{
	IoData* ioData = new IoData(SEND);
	memcpy(ioData->buff, packet, BUF_SIZE);

	WSASend(tcpSocket->GetSocket(),
		&ioData->wsaBuf,
		1,
		NULL,
		0,
		ioData,
		NULL);
}

void IOCP::Send(SOCKET destSock, char* packet)
{
	printf("Send Called : %s\n", packet);
	IoData* ioData = new IoData(SEND);
	memcpy(ioData->buff, packet, BUF_SIZE);

	WSASend(destSock,
		&ioData->wsaBuf,
		1,
		NULL,
		0,
		ioData,
		NULL);
}

void IOCP::Send(SOCKET destSock, JCPacket& packet)
{
	OutputStream ostrm;
	packet.Write(ostrm);
	
	IoData* ioData = new IoData(SEND);
	memcpy(ioData->buff, ostrm.GetBufferPtr(), ostrm.GetLength());
	WSASend(destSock,
		&ioData->wsaBuf,
		1,
		NULL,
		0,
		ioData,
		NULL);
}

