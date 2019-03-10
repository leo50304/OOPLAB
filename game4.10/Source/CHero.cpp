#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CHero.h"
#include "Block.h"
#include "map.h"

namespace game_framework {

	CHero::CHero()
	{
		Initialize();
	}

	int CHero::GetX1()
	{
		return x;
	}

	int CHero::GetY1()
	{
		return y;
	}

	int CHero::GetX2()
	{
		return x + animation.Width();
	}

	int CHero::GetY2()
	{
		return y + animation.Height();
	}

	void CHero::SetOnLadder(bool flag) 
	{
		isOnLadder = flag;
	}

	void CHero::Initialize()
	{
		const int X_POS = 32*2;
		const int Y_POS = 32*12;
		x = X_POS;
		y = Y_POS;
		isOnLadder = false;
		onDrop = false;
		previousBlock = 0;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
	}

	void CHero::LoadBitmap()
	{
		animation.AddBitmap(GWR_01, RGB(255, 255, 255));
		animation.AddBitmap(GWR_02, RGB(255, 255, 255));
	}

	bool CHero::IsOnLadder() 
	{
		return isOnLadder;
	}

	void CHero::Drop() 
	{
		onDrop = true;
	}

	void CHero::OnMove(MapBrown* map)
	{
		const int STEP_SIZE = 4;
		animation.OnMove();

		//if (onDrop) 
		//	y += STEP_SIZE;
		if (isMovingLeft) 
		{
			x -= STEP_SIZE;
			if (!map->getMapObject(map->GetBlock(x / 32, y / 32))->HitHeroAction(x, y, isOnLadder, "Left"))
			{
				x += STEP_SIZE;
			}
		}
		if (isMovingRight) 
		{
			x += STEP_SIZE;

			if (!map->getMapObject(map->GetBlock((x+31) / 32, y / 32))->HitHeroAction(x, y, isOnLadder, "Right"))
			{
				x -= STEP_SIZE;
			}
		}
		if (isMovingUp)
		{
			y -= STEP_SIZE;
			if (!map->getMapObject(map->GetBlock(x / 32, y / 32))->HitHeroAction(x, y, isOnLadder, "Up"))
			{
				y += STEP_SIZE;
			}
		}
		if (isMovingDown) 
		{
			y += STEP_SIZE;
			if (!map->getMapObject(map->GetBlock(x / 32, (y+31) / 32))->HitHeroAction(x, y, isOnLadder, "Down"))
			{
				y -= STEP_SIZE;
			}
		}
	}

	void CHero::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void CHero::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void CHero::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void CHero::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void CHero::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void CHero::OnShow()
	{
		animation.SetTopLeft(x, y);
		animation.OnShow();
	}
}