#include "JCMap.h"


JCMap::JCMap(int width, int height) : _width(width), _height(height)
{
	map = static_cast<int*>(calloc(1, sizeof(int) * width * height));
}

JCMap::~JCMap()
{
	free(map);
}

int JCMap::DrawObject(const JCGameObject obj)
{
	if (!IsAvailable(obj))
	{
		return 0;
	}

	return 1;
}

void JCMap::DeleteObject(const JCGameObject obj)
{
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			map[i * _width + j] = EMPTY_BLOCK;
		}
	}
}

bool JCMap::IsAvailable(const JCGameObject obj)
{
	int x = obj.Get_X();
	int y = obj.Get_Y();
	int w = obj.Get_Width();
	int h = obj.Get_Height();
	int t = obj.Get_Type();

	int left = x - (w / 2);
	int top = y - (h / 2);

	//recover flag
	bool flag = true;
	int cnt = 0;

	for (int i = top; i < top + h; i++)
	{
		for (int j = left; j < left + w; j++)
		{
			if (i < 0 || i >= _height || j < 0 || j >= _width)
			{
				flag = false;
				break;
			}

			if (map[_width * i + j] != 0)
			{
				flag = false;
				break;
			}
			else
			{
				map[_width * i + j] ^= t;
			}
			cnt++;
		}

		if (!flag) break;
	}

	//recover
	if (!flag)
	{
		int rCnt = 0;
		for (int i = top; i < top + h; i++)
		{
			for (int j = left; j < left + w; j++)
			{
				if (rCnt++ >= cnt)
				{
					//recover end
					return flag;
				}

				map[_width * i + j] ^= t;
			}
		}
	}



	return flag;
}
