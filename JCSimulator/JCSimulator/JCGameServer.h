#pragma once

#include <vector>		//std::vector
#include <map>			//std::map
#include <memory>		//std::shared_ptr

#include "IOCP.h"
#include "JCMap.h"
#include "JCGameObject.h"
#include "JCGamePacket.h"


using JCGameObjectPtr = std::shared_ptr<JCGameObject>;

struct UserInfo
{
	int id;
	SOCKET sock;
};

class JCGameServer : public IOCP
{
private:
	const static int MAP_WIDTH	= 100;
	const static int MAP_HEIGHT	= 100;

	JCMap map;

	std::map<UserInfo, JCGameObjectPtr> userList;
public:
	JCGameServer();
	~JCGameServer();

	int BroadcastMessage(JCGamePacket& packet);
	int BroadcastUserLocation();

	virtual int OnAccept(SocketInfo* socketInfo);
	virtual int OnRecvPacket(SocketInfo* socketInfo, char* packet);
	virtual int OnCloseSocket();
};