#ifndef ITEM_H
#define ITEM_H

#include "stdafx.h"
#include "Resource.h"
#include "map.h"

namespace game_framework {

	class Item 
	{
	public:
		Item(int x, int y, int d);
		Item();
		~Item() 
		{
			int a = 0;
		}
		virtual void use(CHero* hero) = 0;
		virtual void LoadBitMap() = 0;
		void ShowIcon();
		virtual void OnSelect(CHero* hero);
		virtual void RemoveSelect(CHero* hero);
		void MoveIcon();
		virtual bool pickUp(CHero* hero); //可以撿起
		virtual bool InHitBox(int x, int y);
		virtual void OnMove(MapBrown* map);     // 移動
		void ShowRemainingUse(); //顯示剩餘次數
		void setXY(int x, int y); //設定位置
		int getMap(); //取得所在地圖
		void resetOffset(); //碰撞修正
		CAnimation icon;
	protected:
		int onGround = false;
		double speedY = 0;
		int aY;
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

	class Gold : public Item
	{
	public:
		Gold(int x, int y, int d);
		Gold();
		void use(CHero* hero);
		void LoadBitMap();
		bool InHitBox(int x, int y);
		bool pickUp(CHero* hero);
	private:
		int amount = 1;
	};

	class BigBlood : public Item
	{
	public:
		BigBlood(int x, int y, int d);
		BigBlood();
		void use(CHero* hero);
		void LoadBitMap();
		bool InHitBox(int x, int y);
	private:
		int amount = 1;
	};

	class Ring : public Item
	{
	public:
		Ring(int x, int y, int d);
		Ring();
		void use(CHero* hero);
		void LoadBitMap();
		bool InHitBox(int x, int y);
		void OnSelect(CHero* hero);
		void RemoveSelect(CHero* hero);
	private:
		bool addhp;
		int amount = 1;
	};


	class LegendStone : public Item
	{
	public:
		LegendStone(int x, int y, int d);
		LegendStone();
		void use(CHero* hero);
		void LoadBitMap();
		bool InHitBox(int x, int y);
		void OnSelect(CHero* hero);
		void RemoveSelect(CHero* hero);
	private:
		int amount = 1;
	};

}

#endif