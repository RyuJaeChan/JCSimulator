#include "JCGamePacket.h"

void JCGamePacket::Write(OutputStream& ostrm)
{
	ostrm.Write(header.messageType);

	ostrm.Write(gameObject.Get_X());
	ostrm.Write(gameObject.Get_Y());
	ostrm.Write(gameObject.Get_Width());
	ostrm.Write(gameObject.Get_Height());
	ostrm.Write(gameObject.Get_Type());
}

void JCGamePacket::Read(InputStream& istrm)
{
	istrm.Read(header.messageType);

	istrm.Read(gameObject.Get_XRef());
	istrm.Read(gameObject.Get_YRef());
	istrm.Read(gameObject.Get_WidthRef());
	istrm.Read(gameObject.Get_HeightRef());
	istrm.Read(gameObject.Get_TypeRef());
}
