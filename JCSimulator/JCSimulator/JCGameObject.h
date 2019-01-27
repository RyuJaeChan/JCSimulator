#pragma once

class JCGameObject
{
private:
	int _x;
	int _y;
	int _width;
	int _height;

	int _type;
	//rendering ...

public:
	JCGameObject() = default;
	JCGameObject(int x, int y, int w, int h, int t);
	virtual ~JCGameObject();

	int Get_X()			const;
	int Get_Y()			const;
	int Get_Width()		const;
	int Get_Height()	const;
	int Get_Type()		const;

	const int& Get_XRef();
	const int& Get_YRef();
	const int& Get_WidthRef();
	const int& Get_HeightRef();
	const int& Get_TypeRef();
};

