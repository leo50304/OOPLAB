#ifndef ITEM_H
#define ITEM_H

#include "stdafx.h"
#include "Resource.h"

namespace game_framework {

	class Item 
	{
	public:
		Item(int x, int y, int d);
		Item();
		virtual void use(CHero* hero) = 0;
		virtual void LoadBitMap() = 0;
		void ShowIcon();
		void MoveIcon();
		void ShowRemainingTime();
		void setXY(int x, int y);
		int getMap();
		CAnimation icon;
	protected:
		int RemainingTime = -1;
		int map = -1;
		int X1, Y1;
		int offSetX, offSetY;
	};

	class SmallBlood : public Item 
	{
	public:
		SmallBlood(int x, int y, int d);
		SmallBlood();
		void use(CHero* hero);
		void LoadBitMap();
	};

}

#endif