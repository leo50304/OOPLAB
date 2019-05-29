#ifndef BLOCK_H
#define BLOCK_H

#include "stdafx.h"
//#include "CHero.h"
#include <stdio.h>
#include <vector>


namespace game_framework {
	class MapObject
	{
	public:
		~MapObject()
		{
			for (unsigned int i = 0; i < block.size(); ++i)
			{
				delete block[i];
			}
		}
		virtual bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px) { return false; }; //return true if can move
		virtual void LoadBitMap() {};
		virtual bool IsSolid() { return true; };
		virtual void PutBlock(int px, int py, int type) {};
	protected:
		vector<CMovingBitmap*> block;
	};

	class Blank : public MapObject
	{
	public:
		void LoadBitMap()
		{
			for (int i = 0; i < 6; ++i)
			{
				block.push_back(new CMovingBitmap());
			}
			block[0]->LoadBitmap(M_000);
			block[1]->LoadBitmap(M_001);
			block[2]->LoadBitmap(M_002);
			block[3]->LoadBitmap(M_003);
			block[4]->LoadBitmap(M_004);
			block[5]->LoadBitmap(M_005);
		}
		void PutBlock(int px, int py, int type)
		{
			if (type % 100 >= 6)
				type = 0;
			block[type % 100]->SetTopLeft(px, py);
			block[type % 100]->ShowBitmap();
		}
		bool IsSolid()
		{
			return false;
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
		{
			if (pos == "Up" && isOnLadder && y % 32 < 4)
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
			for (int i = 0; i < 6; ++i)
			{
				block.push_back(new CMovingBitmap());
			}
			block[0]->LoadBitmap(M_500);
			block[1]->LoadBitmap(M_501);
			block[2]->LoadBitmap(M_502);
			block[3]->LoadBitmap(M_503);
			block[4]->LoadBitmap(M_504);
			block[5]->LoadBitmap(M_505);
		}
		void PutBlock(int px, int py, int type)
		{
			block[type % 100]->SetTopLeft(px, py);
			block[type % 100]->ShowBitmap();
		}
		bool IsSolid()
		{
			return true;
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
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
			block.push_back(new CMovingBitmap());
			block[0]->LoadBitmap(GM_03);
		}
		void PutBlock(int px, int py, int type)
		{
			block[0]->SetTopLeft(px, py);
			block[0]->ShowBitmap();
		}
		bool IsSolid()
		{
			return true;
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
		{
			if (pos == "Left")
			{
				return !isOnLadder;
			}
			else if (pos == "Right")
			{
				return !isOnLadder;
			}
			else if (pos == "Up")
			{
				isOnLadder = true;
				x = px;
				return true;
			}
			else if (pos == "Down")
			{
				if (isOnLadder)
				{
					return true;
				}
				else if (px - x <= 8 && px - x >= -8)
				{
					x = px;
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
	};

	class Ladder : public MapObject
	{
	public:
		void LoadBitMap()
		{
			block.push_back(new CMovingBitmap());
			block[0]->LoadBitmap(GM_02);
		}
		void PutBlock(int px, int py, int type)
		{
			block[0]->SetTopLeft(px, py);
			block[0]->ShowBitmap();
		}
		bool IsSolid()
		{
			return false;
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
		{
			if (pos == "Left")
			{
				return !isOnLadder;
			}
			else if (pos == "Right")
			{
				return !isOnLadder;
			}
			else if (pos == "Up" || pos == "Down")
			{
				if (isOnLadder)
				{
					return true;
				}
				else if (px - x <= 8 && px - x >= -8)
				{
					x = px;
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
	};

	class Grass : public MapObject
	{
	public:
		void LoadBitMap()
		{
			for (int i = 0; i < 2; ++i)
			{
				block.push_back(new CMovingBitmap());
			}
			block[0]->LoadBitmap(M_100);
			block[1]->LoadBitmap(M_101);
		}
		void PutBlock(int px, int py, int type)
		{
			block[type % 100]->SetTopLeft(px, py);
			block[type % 100]->ShowBitmap();
		}
		bool IsSolid()
		{
			return false;
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
		{
			if (pos == "Up" && isOnLadder && y % 32 < 4)
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

	class Bone : public MapObject
	{
	public:
		void LoadBitMap()
		{
			for (int i = 0; i < 2; ++i)
			{
				block.push_back(new CMovingBitmap());
			}
			block[0]->LoadBitmap(M_200);
			block[1]->LoadBitmap(M_201);
		}
		void PutBlock(int px, int py, int type)
		{
			block[type % 100]->SetTopLeft(px, py);
			block[type % 100]->ShowBitmap();
		}
		bool IsSolid()
		{
			return false;
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
		{
			if (pos == "Up" && isOnLadder && y % 32 < 4)
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

	class Tomb : public MapObject
	{
	public:
		void LoadBitMap()
		{
			for (int i = 0; i < 6; ++i)
			{
				block.push_back(new CMovingBitmap());
			}
			block[0]->LoadBitmap(GM_00);
			block[1]->LoadBitmap(GM_00);
			block[2]->LoadBitmap(GM_00);
			block[3]->LoadBitmap(GM_00);
			block[4]->LoadBitmap(GM_00);
			block[5]->LoadBitmap(GM_00);
		}
		void PutBlock(int px, int py, int type)
		{
			block[0]->SetTopLeft(px, py);
			block[0]->ShowBitmap();
		}
		bool IsSolid()
		{
			return false;
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
		{
			if (pos == "Up" && isOnLadder && y % 32 < 4)
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

	class Shop : public MapObject
	{
	public:
		void LoadBitMap()
		{
			for (int i = 0; i < 6; ++i)
			{
				block.push_back(new CMovingBitmap());
			}
			block[0]->LoadBitmap(GM_00);
			block[1]->LoadBitmap(GM_00);
			block[2]->LoadBitmap(GM_00);
			block[3]->LoadBitmap(GM_00);
			block[4]->LoadBitmap(GM_00);
			block[5]->LoadBitmap(GM_00);
		}
		void PutBlock(int px, int py, int type)
		{
			block[0]->SetTopLeft(px, py);
			block[0]->ShowBitmap();
		}
		bool IsSolid()
		{
			return false;
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
		{
			if (pos == "Up" && isOnLadder && y % 32 < 4)
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

	class WeakBlock : public MapObject
	{
	public:
		void LoadBitMap()
		{
			for (int i = 0; i < 2; ++i)
			{
				block.push_back(new CMovingBitmap());
			}
			block[0]->LoadBitmap(M_600);
			block[1]->LoadBitmap(M_601);
		}
		void PutBlock(int px, int py, int type)
		{
			block[type % 100]->SetTopLeft(px, py);
			block[type % 100]->ShowBitmap();
		}
		bool IsSolid()
		{
			return true;
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
		{
			if (pos == "Down")
			{
				isOnLadder = false;
			}
			return false;
		}
	};

	class Door : public MapObject
	{
	public:
		void LoadBitMap()
		{
			for (int i = 0; i < 6; ++i)
			{
				block.push_back(new CMovingBitmap());
			}
			block[0]->LoadBitmap(GM_00);
			block[1]->LoadBitmap(GM_00);
			block[2]->LoadBitmap(GM_00);
			block[3]->LoadBitmap(GM_00);
			block[4]->LoadBitmap(GM_00);
			block[5]->LoadBitmap(GM_00);
		}
		void PutBlock(int px, int py, int type)
		{
			block[0]->SetTopLeft(px, py);
			block[0]->ShowBitmap();
		}
		bool IsSolid()
		{
			return false;
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
		{
			if (pos == "Up" && isOnLadder && y % 32 < 4)
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

	class Spike : public MapObject
	{
	public:
		void LoadBitMap()
		{
			for (int i = 0; i < 6; ++i)
			{
				block.push_back(new CMovingBitmap());
			}
			block[0]->LoadBitmap(GM_00);
			block[1]->LoadBitmap(GM_00);
			block[2]->LoadBitmap(GM_00);
			block[3]->LoadBitmap(GM_00);
			block[4]->LoadBitmap(GM_00);
			block[5]->LoadBitmap(GM_00);
		}
		void PutBlock(int px, int py, int type)
		{
			block[0]->SetTopLeft(px, py);
			block[0]->ShowBitmap();
		}
		bool IsSolid()
		{
			return false;
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
		{
			if (pos == "Up" && isOnLadder && y % 32 < 4)
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

	class Lever : public MapObject
	{
	public:
		void LoadBitMap()
		{
			for (int i = 0; i < 6; ++i)
			{
				block.push_back(new CMovingBitmap());
			}
			block[0]->LoadBitmap(GM_00);
			block[1]->LoadBitmap(GM_00);
			block[2]->LoadBitmap(GM_00);
			block[3]->LoadBitmap(GM_00);
			block[4]->LoadBitmap(GM_00);
			block[5]->LoadBitmap(GM_00);
		}
		void PutBlock(int px, int py, int type)
		{
			block[0]->SetTopLeft(px, py);
			block[0]->ShowBitmap();
		}
		bool IsSolid()
		{
			return false;
		}
		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
		{
			if (pos == "Up" && isOnLadder && y % 32 < 4)
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
}

#endif


//
//namespace game_framework {
//	class MapObject
//	{
//	public:
//		virtual bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px) = 0; //return true if can move
//		virtual void LoadBitMap() = 0;
//		virtual bool IsSolid() = 0;
//		void PutBlock(int px, int py);
//	protected:
//		CMovingBitmap block;
//	};
//
//	class Blank : public MapObject
//	{
//	public:
//		void LoadBitMap();
//		bool IsSolid();
//		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px);
//	};
//
//	class Block : public MapObject
//	{
//	public:
//		void LoadBitMap();
//		bool IsSolid();
//		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px);
//	};
//
//	class LadderBlock : public MapObject
//	{
//	public:
//		void LoadBitMap();
//		bool IsSolid();
//		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px);
//	};
//
//	class Ladder : public MapObject
//	{
//	public:
//		void LoadBitMap();
//		bool IsSolid();
//		bool HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px);
//	};
//
//}
//
//#endif
