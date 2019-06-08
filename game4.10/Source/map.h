#ifndef MAP_H
#define MAP_H

#include "stdafx.h"
#include "Block.h"
#include <vector>

namespace game_framework {

	class MapBrown
	{
	public:
		~MapBrown();
		MapBrown();
		void LoadBitMap();
		void initMap();
		void OnShow();
		int GetBlock(int i, int j);
		MapObject* getMapObject(int i);
		void UpdateMap(char nextPos);
		bool isBlockSolid(int i, int j);
		int getNext();

	protected:
		int map[13][18];
		int mapOfMap[6][5];
		//MapObject* mapObjects[12];
		vector<MapObject*> mapObjects;
		std::vector<vector<vector<int>>> mapList;
		const int X, Y;
		const int MV, MH;
		int next;
		int mapPosX, mapPosY;
	};
}

#endif
