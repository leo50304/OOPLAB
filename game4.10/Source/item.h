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
		virtual bool InHitBox(int x, int y);
		void ShowRemainingUse();
		void setXY(int x, int y);
		int getMap();
		void resetOffset();
		CAnimation icon;
	protected:
		int RemainingUse = -1;
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
		bool InHitBox(int x, int y);
	};

}

#endif