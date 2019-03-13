#ifndef MAP_H
#define MAP_H

#include "stdafx.h"
#include "Block.h"
#include <vector>

namespace game_framework {

	class MapBrown
	{
	public:
		MapBrown();
		void LoadBitMap();
		void OnShow();
		int GetBlock(int i, int j);
		MapObject* getMapObject(int i);
		void UpdateMap(char nextPos);
	protected:
		int map[14][19];
		int mapOfMap[1][2];
		MapObject* mapObjects[6];
		std::vector<int**> mapList;
		const int X, Y;
		const int MV, MH;
		int next;
		int mapPosX, mapPosY;
	};
}

#endif
