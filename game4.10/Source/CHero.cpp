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
			int px = x / 32;
			int py = y / 32;
			if (!map->getMapObject(map->GetBlock(px, py))->HitHeroAction(x, y, isOnLadder, "Left", px * 32))
			{
				x += STEP_SIZE;
			}
		}
		if (isMovingRight)
		{
			x += STEP_SIZE;
			int px = (x + 31) / 32;
			int py = y / 32;
			if (!map->getMapObject(map->GetBlock(px, py))->HitHeroAction(x, y, isOnLadder, "Right", px * 32))
			{
				x -= STEP_SIZE;
			}
		}
		if (isMovingUp)
		{
			y -= STEP_SIZE;
			int px = (x + 16) / 32;
			int py = y / 32;
			if (!map->getMapObject(map->GetBlock(px, py))->HitHeroAction(x, y, isOnLadder, "Up", px * 32))
			{
				y += STEP_SIZE;
			}
		}
		if (isMovingDown)
		{
			y += STEP_SIZE;
			int px = (x + 16) / 32;
			int py = (y + 31) / 32;
			if (!map->getMapObject(map->GetBlock(px, py))->HitHeroAction(x, y, isOnLadder, "Down", px * 32))
			{
				y -= STEP_SIZE;
			}
		}

		if (x < 0)
		{
			map->UpdateMap('L');
			x = 580;
		}
		else if (x > 600)
		{
			map->UpdateMap('R');
			x = 0;
		}
		else if (y < 0)
		{
			map->UpdateMap('U');
			y = 480;
		}
		else if (y > 460) 
		{
			map->UpdateMap('D');
			y = 0;
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