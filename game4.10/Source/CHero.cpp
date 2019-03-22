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

	void CHero::SetOnLadder(bool flag)
	{
		isOnLadder = flag;
	}

	void CHero::Initialize()
	{
		const int X_POS = 32 * 2;
		const int Y_POS = 32 * 12;
		x = X_POS;
		y = Y_POS;
		isOnLadder = false;
		onDrop = false;
		previousBlock = 0;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
	}

	void CHero::LoadBitmap()
	{
		standL.LoadBitmap(WWS_L, RGB(255, 255, 255));
		standR.LoadBitmap(WWS_R, RGB(255, 255, 255));
		jumpL.LoadBitmap(WWJ_L, RGB(255, 255, 255));
		jumpR.LoadBitmap(WWJ_R, RGB(255, 255, 255));

		stayOnLadder.LoadBitmap(WWL_2, RGB(255, 255, 255));

		moveRAnimation.AddBitmap(WWR_R1, RGB(255, 255, 255));
		moveRAnimation.AddBitmap(WWS_R, RGB(255, 255, 255));
		moveRAnimation.AddBitmap(WWR_R2, RGB(255, 255, 255));
		moveRAnimation.AddBitmap(WWS_R, RGB(255, 255, 255));

		moveLAnimation.AddBitmap(WWR_L1, RGB(255, 255, 255));
		moveLAnimation.AddBitmap(WWS_L, RGB(255, 255, 255));
		moveLAnimation.AddBitmap(WWR_L2, RGB(255, 255, 255));
		moveLAnimation.AddBitmap(WWS_L, RGB(255, 255, 255));

		moveUAnimation.AddBitmap(WWL_1, RGB(255, 255, 255));
		moveUAnimation.AddBitmap(WWL_2, RGB(255, 255, 255));
		moveUAnimation.AddBitmap(WWL_3, RGB(255, 255, 255));


		moveDAnimation.AddBitmap(WWL_1, RGB(255, 255, 255));
		moveDAnimation.AddBitmap(WWL_2, RGB(255, 255, 255));
		moveDAnimation.AddBitmap(WWL_3, RGB(255, 255, 255));

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
		double ACCELERATE = 0.89;

		const int STEP_SIZE = 4;

		moveRAnimation.OnMove();
		moveDAnimation.OnMove();
		moveUAnimation.OnMove();
		moveLAnimation.OnMove();


		if (onJump)
		{
			if (!onDrop) //�W��
			{
				speed = (double)speed - ACCELERATE;
				if (speed < 1) 
				{
					speed = 1;
				}
				y -= (int)speed;
			}
			else //�U��
			{
				speed = (double)speed+ ACCELERATE;
				y += (int)speed;
			}
			if (map->isBlockSolid(x / 32, (y + 31) / 32) || map->isBlockSolid((x+22) / 32, (y + 31) / 32)) //���a
			{
				y = (y / 32) * 32;
				onJump = false;
				onDrop = false;
			}
			if (map->isBlockSolid(x / 32, (y - 1) / 32) || map->isBlockSolid((x + 22) / 32, (y - 1) / 32)) //�����Y
			{
				onDrop = true;
				y = ((y - 1) / 32) * 32 + 32;
			}
			if (y <= jumpTop)
			{
				y = jumpTop;
				onDrop = true;
			}
		}
		else if(!isOnLadder && !map->isBlockSolid(x / 32, (y + 32) / 32) && !map->isBlockSolid((x+22) / 32, (y + 32) / 32)) //���
		{
			speed = 1;
			onJump = true;
			onDrop = true;
		}

		//if (onDrop) 
		//	y += STEP_SIZE;
		if (isMovingUp && !onJump)
		{
			y -= STEP_SIZE;
			//isMovingDown = false;
			int px = (x + 16) / 32;
			int py = y / 32;
			if (!map->getMapObject(map->GetBlock(px, py))->HitHeroAction(x, y, isOnLadder, "Up", px * 32))
			{
				y += STEP_SIZE;

				if (!onJump)
				{
					speed = 14;
					onJump = true;
					onDrop = false;
					jumpTop = y - 32; //���D�W��
					TopLimit = y - 32 * 3;//�j���W��
				}
			}

			if (isOnLadder)
			{
				isOnLadderSide = false;
			}
		}
		if (isMovingUp && onJump && !onDrop) 
		{
			jumpTop -= 10;
			if (jumpTop < TopLimit) 
			{
				jumpTop = TopLimit;		
			}
		}
		if (isMovingDown)
		{
			y += STEP_SIZE;
			int px = (x + 16) / 32;
			int py = (y + 31) / 32;
			if (!map->getMapObject(map->GetBlock(px, py))->HitHeroAction(x, y, isOnLadder, "Down", px * 32))
			{
				//isMovingDown = false;
				y -= STEP_SIZE;
			}
			if (isOnLadder)
			{
				isOnLadderSide = false;
			}
		}
		if (isMovingLeft)
		{
			faceSide = 0;
			x -= STEP_SIZE;
			int px = x / 32;
			int py = y / 32;
			if (!map->getMapObject(map->GetBlock(px, py))->HitHeroAction(x, y, isOnLadder, "Left", px * 32))
			{
				x += STEP_SIZE;
			}
			if (isOnLadder)
			{
				isOnLadderSide = true;
			}
		}
		if (isMovingRight)
		{
			faceSide = 1;
			x += STEP_SIZE;
			int px = (x + 31) / 32;
			int py = y / 32;
			if (!map->getMapObject(map->GetBlock(px, py))->HitHeroAction(x, y, isOnLadder, "Right", px * 32))
			{
				x -= STEP_SIZE;
			}
			if (isOnLadder)
			{
				isOnLadderSide = true;
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
		if (onJump) 
		{	
			if (faceSide == 0)
			{
				jumpL.SetTopLeft(x, y);
				jumpL.ShowBitmap();
			}
			else if (faceSide == 1)
			{
				jumpR.SetTopLeft(x, y);
				jumpR.ShowBitmap();
			}
		}
		else if (IsOnLadder())
		{
			if (isMovingUp)
			{
				moveUAnimation.SetTopLeft(x, y);
				moveUAnimation.OnShow();
			}
			else if (isMovingDown)
			{
				moveDAnimation.SetTopLeft(x, y);
				moveDAnimation.OnShow();
			}
			else if (isOnLadderSide)
			{
				if (faceSide == 0)
				{
					standL.SetTopLeft(x, y);
					standL.ShowBitmap();
				}
				else if (faceSide == 1)
				{
					standR.SetTopLeft(x, y);
					standR.ShowBitmap();
				}
			}
			else
			{
				stayOnLadder.SetTopLeft(x, y);
				stayOnLadder.ShowBitmap();
			}
		}
		else if (isMovingUp)
		{
			if (faceSide == 0)
			{
				standL.SetTopLeft(x, y);
				standL.ShowBitmap();
			}
			else if (faceSide == 1)
			{
				standR.SetTopLeft(x, y);
				standR.ShowBitmap();
			}
		}
		else if (isMovingDown)
		{
			if (faceSide == 0)
			{
				standL.SetTopLeft(x, y);
				standL.ShowBitmap();
			}
			else if (faceSide == 1)
			{
				standR.SetTopLeft(x, y);
				standR.ShowBitmap();
			}
		}
		else if (isMovingRight)
		{
			moveRAnimation.SetTopLeft(x, y);
			moveRAnimation.OnShow();
		}
		else if (isMovingLeft)
		{
			moveLAnimation.SetTopLeft(x, y);
			moveLAnimation.OnShow();
		}
		else
		{
			if (faceSide == 0)
			{
				standL.SetTopLeft(x, y);
				standL.ShowBitmap();
			}
			else if (faceSide == 1)
			{
				standR.SetTopLeft(x, y);
				standR.ShowBitmap();
			}
		}
	}
}