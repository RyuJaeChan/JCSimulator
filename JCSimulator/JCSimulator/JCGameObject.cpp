#include "JCGameObject.h"



JCGameObject::JCGameObject(int x, int y, int w, int h, int t) : _x(x), _y(y), _width(w), _height(h), _type(t)
{

}

JCGameObject::~JCGameObject()
{
}

int JCGameObject::Get_X() const
{
	return _x;
}

int JCGameObject::Get_Y() const
{
	return _y;
}

int JCGameObject::Get_Width() const
{
	return _width;
}

int JCGameObject::Get_Height() const
{
	return _height;
}

int JCGameObject::Get_Type() const
{
	return _type;
}

const int& JCGameObject::Get_XRef()
{
	return _x;
}

const int& JCGameObject::Get_YRef()
{
	return _y;
}

const int& JCGameObject::Get_WidthRef()
{
	return _width;
}

const int& JCGameObject::Get_HeightRef()
{
	return _height;
}

const int& JCGameObject::Get_TypeRef()
{
	return _type;
}
