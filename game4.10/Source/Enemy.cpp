#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Enemy.h"
#include "Block.h"
#include "map.h"

namespace game_framework {

	Enemy::Enemy()
	{
		Initialize();
	}

	int Enemy::GetX1()
	{
		return x;
	}

	int Enemy::GetY1()
	{
		return y;
	}

	void Enemy::Initialize()
	{
		isMovingRight = isMovingUp = isMovingDown = false;
		isMovingLeft = true;
		speed = 2;
	}

	void Enemy::Distroy()
	{
		isDistroyed = true;
	}

	bool Enemy::IsDistroyed()
	{
		return isDistroyed;
	}

	void Enemy::LoadBitmap()
	{

		moveLAnimation.AddBitmap(E_L0, RGB(255, 255, 255));
		moveLAnimation.AddBitmap(E_L1, RGB(255, 255, 255));
		moveRAnimation.AddBitmap(E_R0, RGB(255, 255, 255));
		moveRAnimation.AddBitmap(E_R1, RGB(255, 255, 255));
	}


	void Enemy::OnMove(MapBrown* map)
	{
		moveRAnimation.OnMove();
		moveLAnimation.OnMove();

		if (isMovingLeft)
		{
			x -= speed;
		}
		else if (isMovingRight)
		{
			x += speed;
		}

		if (!map->isBlockSolid(x / 32, (y + 32) / 32) || !map->isBlockSolid((x + 22) / 32, (y + 32) / 32) || x<0 || x>32*17) //碰到邊緣或邊界
		{
			if (isMovingLeft) 
			{
				x += speed;
			}
			else 
			{
				x -= speed;
			}
			isMovingLeft = !isMovingLeft;
			isMovingRight = !isMovingRight;
		}

	}

	void Enemy::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void Enemy::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void Enemy::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void Enemy::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void Enemy::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void Enemy::OnShow()
	{
		if (isMovingRight)
		{
		moveRAnimation.SetTopLeft(x, y);
		moveRAnimation.OnShow();
		}
		else if (isMovingLeft)
		{
		moveLAnimation.SetTopLeft(x, y);
		moveLAnimation.OnShow();
		}
	}
}