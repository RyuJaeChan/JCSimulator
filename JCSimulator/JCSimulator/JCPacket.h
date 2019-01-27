#pragma once

#include <cstdint>
#include <memory>		//std::memcpy()

#include "SerializeStream.h"

class JCPacket
{
public:
	virtual void Write(OutputStream& ostrm)	= 0;
	virtual void Read(InputStream& istrm)	= 0;
	JCPacket()			= default;
	virtual ~JCPacket()	= default;
};