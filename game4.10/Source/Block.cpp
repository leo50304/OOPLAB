#include "stdafx.h"
//#include "Block.h"
//#include "Resource.h"
//#include <string>
//#include <mmsystem.h>
//#include <ddraw.h>
//#include "audio.h"
//#include "gamelib.h"
//
//
//namespace game_framework {
//
//
//	void MapObject::PutBlock(int px, int py)
//	{
//		block.SetTopLeft(px, py);
//		block.ShowBitmap();
//	}
//
//	void Blank::LoadBitMap()
//	{
//		block.LoadBitmap(GM_00);
//	}
//	bool Blank::IsSolid()
//	{
//		return false;
//	}
//	bool Blank::HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
//	{
//		if (pos == "Up" && isOnLadder && y % 32 < 4)
//		{
//			y -= y % 32;
//			isOnLadder = false;
//			return true;
//		}
//		if (pos == "Left" || pos == "Right")
//		{
//			return !isOnLadder;
//		}
//		else if (pos == "Up" || pos == "Down")
//		{
//			return isOnLadder;
//		}
//		throw "Blank error";
//	}
//
//	void Block::LoadBitMap()
//	{
//		block.LoadBitmap(GM_01);
//	}
//	bool Block::IsSolid()
//	{
//		return true;
//	}
//	bool Block::HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
//	{
//		if (pos == "Down")
//		{
//			isOnLadder = false;
//		}
//		return false;
//	}
//
//
//
//	void LadderBlock::LoadBitMap()
//	{
//		block.LoadBitmap(GM_03);
//	}
//	bool LadderBlock::IsSolid()
//	{
//		return true;
//	}
//	bool LadderBlock::HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
//	{
//		if (pos == "Left")
//		{
//			return !isOnLadder;
//		}
//		else if (pos == "Right")
//		{
//			return !isOnLadder;
//		}
//		else if (pos == "Up")
//		{
//			return true;
//		}
//		else if (pos == "Down")
//		{
//			if (isOnLadder)
//			{
//				return true;
//			}
//			else if (px - x <= 8 && px - x >= -8)
//			{
//				x = px;
//				isOnLadder = true;
//				return true;
//			}
//			else
//			{
//				return false;
//			}
//		}
//		throw "Ladder error";
//	}
//
//	void Ladder::LoadBitMap()
//	{
//		block.LoadBitmap(GM_02);
//	}
//	bool Ladder::IsSolid()
//	{
//		return false;
//	}
//	bool Ladder::HitHeroAction(int &x, int &y, bool &isOnLadder, string pos, int px)
//	{
//		if (pos == "Left")
//		{
//			return !isOnLadder;
//		}
//		else if (pos == "Right")
//		{
//			return !isOnLadder;
//		}
//		else if (pos == "Up" || pos == "Down")
//		{
//			if (isOnLadder)
//			{
//				return true;
//			}
//			else if (px - x <= 8 && px - x >= -8)
//			{
//				x = px;
//				isOnLadder = true;
//				return true;
//			}
//			else
//			{
//				return false;
//			}
//		}
//		throw "Ladder error";
//	}
//}