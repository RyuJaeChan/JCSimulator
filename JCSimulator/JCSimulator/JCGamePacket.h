#pragma once

#include "JCPacket.h"
#include "JCGameObject.h"

#include <cstring>		//memecpy()

struct JCPacketHeader
{
	enum Type
	{
		Connect,
		Movement,
		Action
	};
	Type messageType;
};


class JCGamePacket : public JCPacket
{
private:
	JCPacketHeader header;
	JCGameObject gameObject;
public:
	JCGamePacket()
	{

	}

	JCGamePacket(JCGameObject obj) : gameObject(obj)
	{

	}

	JCGamePacket(const char* buffer)
	{
		memcpy(this, buffer, sizeof(JCGamePacket));
	}

	~JCGamePacket()
	{

	}

	JCPacketHeader GetPacketHeader()	const
	{
		return header;
	}

	JCGameObject GetGameObject()	const
	{
		return gameObject;
	}

	virtual void Write(OutputStream& ostrm);
	virtual void Read(InputStream& istrm);
};
