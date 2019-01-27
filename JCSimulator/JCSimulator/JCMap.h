#pragma once

#include <cstdlib>
#include <cstdio>	//TEST

#include "JCGameObject.h"

class JCMap
{
private:
	int* map;
	int _width;
	int _height;

	const static int EMPTY_BLOCK = 0;
public:
	JCMap(int width, int height);
	~JCMap();

	//test code
	void PrintMap()
	{
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				printf("%d ", map[i * _width + j]);
			}
			printf("\n");
		}
		printf("\n");
	}

	int DrawObject(const JCGameObject obj);
	void DeleteObject(const JCGameObject obj);
	bool IsAvailable(const JCGameObject obj);
};