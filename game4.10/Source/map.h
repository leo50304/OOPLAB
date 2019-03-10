#ifndef MAP_H
#define MAP_H

#include "stdafx.h"
#include "Block.h"

namespace game_framework {

	class MapBrown
	{
	public:
		MapBrown();
		void LoadBitMap();
		void OnShow();
		int GetBlock(int i, int j);
		MapObject* getMapObject(int i);
	protected:
		int map[14][19];
		MapObject* mapObjects[6];
		const int X, Y;
		const int MV, MH;
	};
}

#endif
