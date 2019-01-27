#include <iostream>

#include "JCMap.h"

int main()
{
	JCMap map(5, 5);

	JCGameObject obj1(1, 2, 3, 3, 1);
	JCGameObject obj2(1, 2, 3, 3, 2);


	map.DrawObject(obj1);
	map.PrintMap();

	map.DeleteObject(obj1);

	map.DrawObject(obj2);

	map.PrintMap();

	return 0;
}