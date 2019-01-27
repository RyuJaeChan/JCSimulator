#include "JCGameServer.h"

JCGameServer::JCGameServer() : map(JCMap(MAP_WIDTH, MAP_HEIGHT))
{

}

JCGameServer::~JCGameServer()
{

}

int JCGameServer::BroadcastMessage(JCGamePacket& packet)
{
	for (const auto& ele : userList)
	{
		Send(ele.first.sock, packet);
	}
}

int JCGameServer::OnAccept(SocketInfo* socketInfo)
{
	return 0;
}

int JCGameServer::OnRecvPacket(SocketInfo* socketInfo, char* packet)
{
	InputStream istrm(packet, sizeof(JCGamePacket));

	JCGamePacket gamePacket;
	gamePacket.Read(istrm);

	switch (gamePacket.GetPacketHeader().messageType)
	{
		case JCPacketHeader::Type::Connect:
		{
			auto res = userList.find(socketInfo->sock->GetSocket());
			if (res != userList.end())
			{
				//user already exist
				break;
			}

			JCGameObject obj = gamePacket.GetGameObject();

			userList.insert(std::make_pair(
				socketInfo->sock->GetSocket(),
				std::make_shared<JCGameObject>(obj)
			));

			//update map
			map.DrawObject(obj);



			break;
		}
		case JCPacketHeader::Type::Movement:
		{

			break;
		}
		case JCPacketHeader::Type::Action:
		{

			break;
		}
	}

	return 0;
}

int JCGameServer::OnCloseSocket()
{
	return 0;
}
