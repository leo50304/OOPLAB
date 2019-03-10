#ifndef BLOCK_H
#define BLOCK_H

#include "stdafx.h"
//#include "CHero.h"
#include <stdio.h>

namespace game_framework {
	class MapObject
	{
	public:
		virtual bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos) {return false; }; //return true if can move
		virtual void LoadBitMap() {};
		void PutBlock(int px, int py)
		{
			block.SetTopLeft(px, py);
			block.ShowBitmap();
		}
	protected:
		CMovingBitmap block;
	};

	class Blank : public MapObject
	{
	public:
		void LoadBitMap()
		{
			block.LoadBitmap(GM_00);
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos)
		{
			if (isOnLadder && y % 32 < 4)
			{
				y -= y % 32;
				isOnLadder = false;
				return true;
			}
			if (pos == "Left" || pos == "Right")
			{
				return !isOnLadder;
			}
			else if (pos == "Up" || pos == "Down")
			{
				return isOnLadder;
			}
			throw "Blank error";
		}
	};

	class Block : public MapObject
	{
	public:
		void LoadBitMap()
		{
			block.LoadBitmap(GM_01);
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos)
		{
			if (pos == "Down") 
			{
				isOnLadder = false;
			}
			return false;
		}
	};

	class LadderBlock : public MapObject
	{
	public:
		void LoadBitMap()
		{
			block.LoadBitmap(GM_03);
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos)
		{

			if (pos == "Left")
			{
				atLadder = x % 32 < 16;
				return !isOnLadder;
			}
			else if (pos == "Right")
			{
				atLadder = x % 32 > 16;
				return !isOnLadder;
			}
			else if (pos == "Up") 
			{
				return true;
			}
			else if (pos == "Down")
			{
				if (isOnLadder)
				{
					return true;
				}
				else if (atLadder)
				{
					if (x % 32 > 16)
					{
						x = x + (32 - x % 32);
					}
					else
					{
						x -= x % 32;
					}
					atLadder = false;
					isOnLadder = true;
					return true;
				}
				else
				{
					return false;
				}
			}
			throw "Ladder error";
		}
	private:
		bool atLadder = false;
	};

	class Ladder : public MapObject
	{
	public:
		void LoadBitMap()
		{
			block.LoadBitmap(GM_02);
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos)
		{
			if (pos == "Left") 
			{
				atLadder = x % 32 < 16;
				return !isOnLadder;
			}
			else if (pos == "Right") 
			{
				atLadder = x % 32 > 16;
				return !isOnLadder;
			}
			else if(pos == "Up" || pos == "Down")
			{
				if (isOnLadder)
				{
					return true;
				}
				else if (atLadder)
				{
					if (x % 32 > 16)
					{
						x += (32 - (x % 32));
					}
					else
					{
						x -= x % 32;
					}
					atLadder = false;
					isOnLadder = true;
					return true;
				}
				else
				{
					return false;
				}
			}
			throw "Ladder error";
		}
	private:
		bool atLadder = false;
	};

}

#endif