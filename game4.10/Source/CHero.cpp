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

	void CHero::BeatBack(bool flag, int direct)
	{
		if (invincibleFrameCount == 0)
		{
			invincibleFrameCount = 100;
			direction = direct;
			beatBackAy = -3;
			beatBackAx = -0.4;
			beatBackYSpeed = -10;
			beatBackXSpeed = -8;
			beatBackXSpeed *= direction;
			beatBackAx *= direction;
			isOnLadder = false;
			isOnLadderSide = false;
			onJump = false;
			onDrop = false;
			beatBack = flag;
		}
	}

	bool CHero::BeatBack()
	{
		return beatBack;
	}

	void CHero::Initialize()
	{
		const int X_POS = 32 * 2;
		const int Y_POS = 32 * 11;
		x = X_POS;
		y = Y_POS;
		isOnLadder = false;
		onDrop = false;
		onAttack = false;
		onFire = false;
		onBook = false;
		previousBlock = 0;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
	}

	void CHero::setHitValid(bool flag)
	{
		hitValid = flag;
	}

	int CHero::getDamage()
	{
		return damage;
	}

	bool CHero::InAttackRange(int eX, int eY)
	{
		if (!hitValid)
		{
			return false;
		}
		if (eY - y <16 && eY - y > -16) 
		{
			if (faceSide == 0)
			{
				return x - eX < 64 && eX < x - 32;
			}
			else if (faceSide == 1)
			{
				return eX - x < 64 && x + 32 < eX;
			}
		}
		return false;
	}
	bool CHero::InFireRange(int eX, int eY)
	{
		return eX < FireX + 32 && eX + 32 > FireX && eY < FireY + 32 && eY + 32 > FireY;
	}

	bool CHero::isOnAttack()
	{
		return onAttack;
	}
	bool CHero::isOnFire()
	{
		return onFire;
	}
	bool CHero::isOnBook()
	{
		return onBook;
	}
	bool CHero::isOnThunder()
	{
		return onThunder;
	}

	void CHero::SetThunder(bool flag)
	{
		onThunder = flag;
	}

	void CHero::LoadBitmap()
	{
		standL.LoadBitmap(WWS_L, RGB(255, 255, 255));
		standR.LoadBitmap(WWS_R, RGB(255, 255, 255));
		jumpL.LoadBitmap(WWJ_L, RGB(255, 255, 255));
		jumpR.LoadBitmap(WWJ_R, RGB(255, 255, 255));

		swordL.LoadBitmap(MWW_L, RGB(255, 255, 255));
		swordR.LoadBitmap(MWW_R, RGB(255, 255, 255));
		attackL.LoadBitmap(WWA_L, RGB(255, 255, 255));
		attackR.LoadBitmap(WWA_R, RGB(255, 255, 255));
		FBookL.LoadBitmap(IDB_BITMAP43, RGB(128, 0, 128));
		FBookR.LoadBitmap(IDB_BITMAP44, RGB(128, 0, 128));
		FireL.AddBitmap(IDB_BITMAP15, RGB(128, 0, 128));
		FireL.AddBitmap(IDB_BITMAP16, RGB(128, 0, 128));
		FireL.AddBitmap(IDB_BITMAP17, RGB(128, 0, 128));
		FireR.AddBitmap(IDB_BITMAP18, RGB(128, 0, 128));
		FireR.AddBitmap(IDB_BITMAP19, RGB(128, 0, 128));
		FireR.AddBitmap(IDB_BITMAP20, RGB(128, 0, 128));
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

	void CHero::SetHoldUp(bool flag)
	{
		onHold = flag;
	}

	void CHero::SetAttack(bool flag)
	{
		if (flag == true && onAttack)
		{
			return;
		}
		onAttack = flag;
		hitValid = flag;
		attackFrameCount = 0;
	}

	void CHero::SetFire(bool flag)
	{
		if (flag == true && onFire)
		{
			return;
		}
		FireX = x;
		FireY = y;
		onFire = flag;
		onBook = flag;
		FireSide = faceSide;
		fireFrameCount = 0;
		bookFrameCount = 0;
	}


	bool CHero::HitGround(MapBrown* map)
	{
		//return map->isBlockSolid(x / 32, (y + 32) / 32) || map->isBlockSolid((x + 31) / 32, (y + 32) / 32);
		return (map->isBlockSolid(x / 32, (y + 32) / 32) && !map->isBlockSolid(x / 32, (y + 1) / 32)) || (map->isBlockSolid((x + 31) / 32, (y + 32) / 32) && !map->isBlockSolid((x + 31) / 32, (y + 1) / 32));
	}

	bool CHero::HitTop(MapBrown* map)
	{
		//return  map->isBlockSolid(x / 32, (y - 1) / 32) || map->isBlockSolid((x + 31) / 32, (y - 1) / 32);
		return (map->isBlockSolid(x / 32, (y - 1) / 32) && !map->isBlockSolid(x / 32, (y + 16) / 32)) || (map->isBlockSolid((x + 31) / 32, (y - 1) / 32) && !map->isBlockSolid((x + 31) / 32, (y + 16) / 32));
	}

	bool CHero::GroundNotSolid(MapBrown* map)
	{
		return !map->isBlockSolid(x / 32, (y + 32) / 32) && !map->isBlockSolid((x + 31) / 32, (y + 32) / 32);
	}

	void CHero::OnMove(MapBrown* map)
	{

		invincibleFrameCount = invincibleFrameCount <= 0 ? invincibleFrameCount : invincibleFrameCount - 1;

		if (beatBack)
		{
			y = (int)(y + beatBackYSpeed);
			x = (int)(x - beatBackXSpeed);

			isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
			if (direction == -1)
			{
				isMovingLeft = true;
			}
			else if (direction == 1)
			{
				isMovingRight = true;
			}

			if ((direction == -1 && beatBackXSpeed < 0) || (direction == 1 && beatBackXSpeed > 0))
			{
				beatBackXSpeed = 0;
			}
			beatBackYSpeed = (beatBackYSpeed - beatBackAy);
			beatBackXSpeed = (beatBackXSpeed - beatBackAx);
			if (HitGround(map)) //落地
			{
				y = (y / 32) * 32;
				onJump = false;
				onDrop = false;
				beatBack = false;
				isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
			}
			if (HitTop(map)) //撞到頭
			{
				speed = 1;
				onJump = true;
				onDrop = true;
				beatBack = false;
				isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
				y = ((y - 1) / 32) * 32 + 32;
			}
		}
		if (isOnLadder && !isOnLadderSide)
		{
			onAttack = false;
			onFire = false;
		}
		if (attackFrameCount == 16)
		{
			onAttack = false;
			attackFrameCount = 0;
		}
		if (bookFrameCount == 16)
		{
			onBook = false;
			bookFrameCount = 0;
		}
		if (fireFrameCount == 50)
		{
			onFire = false;
			fireFrameCount = 0;
		}
		double ACCELERATE = (0.892*0.5);
		attackFrameCount++;
		fireFrameCount++;
		bookFrameCount++;
		const int STEP_SIZE = 4;

		moveRAnimation.OnMove();
		moveDAnimation.OnMove();
		moveUAnimation.OnMove();
		moveLAnimation.OnMove();

		if (onJump)
		{
			if (!onDrop) //上升
			{
				speed = (double)speed - ACCELERATE;
				y -= (int)speed;
			}
			else //下降
			{
				speed = (double)speed + ACCELERATE;
				if (speed > 16)
				{
					speed = 16;
				}
				y += (int)speed;
			}
			if (HitGround(map)) //落地
			{
				y = (y / 32) * 32;
				onJump = false;
				onDrop = false;
			}
			if (HitTop(map)) //撞到頭
			{
				speed = 1;
				onDrop = true;
				y = ((y - 1) / 32) * 32 + 32;
			}
			if (y <= jumpTop && !onDrop && onJump)
			{
				speed = 0;
				y = jumpTop;
				onDrop = true;
			}
		}
		else if (!isOnLadder && GroundNotSolid(map) && !beatBack) //踩空
		{
			speed = 1;
			onJump = true;
			onDrop = true;
		}

		if (isMovingUp && !onJump && !beatBack)
		{
			y -= STEP_SIZE;
			//isMovingDown = false;
			int px = (x + 16) / 32;
			int py = y / 32;
			if (!map->getMapObject(map->GetBlock(px, py))->HitHeroAction(x, y, isOnLadder, "Up", px * 32))
			{
				y += STEP_SIZE;

				if (!onJump && !onHold)
				{
					speed = 10;
					onJump = true;
					onDrop = false;
					jumpTop = y - 32; //跳躍上限
					TopLimit = y - 32 * 3;//大跳上限
				}
			}

			if (isOnLadder)
			{
				isOnLadderSide = false;
			}
			onHold = true;
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
			if (beatBack == true)
			{
				x += STEP_SIZE;
			}
			int px = x / 32;
			int py = y / 32;
			int py2 = (y + 31) / 32;
			if (!map->getMapObject(map->GetBlock(px, py))->HitHeroAction(x, y, isOnLadder, "Left", px * 32) || !map->getMapObject(map->GetBlock(px, py2))->HitHeroAction(x, y, isOnLadder, "Left", px * 32))
			{
				//x += STEP_SIZE;
				x = ((x + 31) / 32) * 32;
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
			if (beatBack == true)
			{
				x -= STEP_SIZE;
			}
			int px = (x + 31) / 32;
			int py = y / 32;
			int py2 = (y + 31) / 32;
			if (!map->getMapObject(map->GetBlock(px, py))->HitHeroAction(x, y, isOnLadder, "Right", px * 32) || !map->getMapObject(map->GetBlock(px, py2))->HitHeroAction(x, y, isOnLadder, "Right", px * 32))
			{
				//x -= STEP_SIZE;
				x = (x / 32) * 32;
			}
			if (isOnLadder)
			{
				isOnLadderSide = true;
			}
		}

		bool changeFlag = false;
		if (x < 0)
		{
			map->UpdateMap('L');
			x = 32 * 18 - 32;
			if (map->getNext() == 7)
			{
				x = 32 * 6;
				y = 32 * 1;
			}
		}
		else if (x > 32 * 18 - 32)
		{
			map->UpdateMap('R');
			x = 0;
			if (map->getNext() == 7)
			{
				x = 32 * 6;
				y = 32 * 1;
			}
		}
		else if (y < 0)
		{
			map->UpdateMap('U');
			y = 32 * 13 - 32;
			if (map->getNext() == 7)
			{
				x = 32 * 6;
				y = 32 * 1;
			}
		}
		else if (y > 32 * 13 - 32)
		{
			map->UpdateMap('D');
			y = 0;
			if (map->getNext() == 7)
			{
				x = 32 * 9;
				y = 32 * 1;
			}
		}
		if (onFire)
		{
			if (FireSide == 0)
			{
				FireX -= 8;
			}
			else if (FireSide == 1)
				{
				FireX += 8;
				}
			if (FireX >= 18 * 32 - 24 || FireX <= 0 - 8 )
			{
				FireX = -100;
				onFire = false;
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
		if (invincibleFrameCount % 10 >= 8)
		{
			return;
		}

		if (onFire)
		{
			if (FireSide == 0)
			{
				FireL.SetTopLeft(FireX, FireY);
				FireL.OnShow();
			}
			else if (FireSide == 1)
			{
				FireR.SetTopLeft(FireX, FireY);
				FireR.OnShow();
			}
		}
		if (onBook)
		{
			if (faceSide == 0)
			{
				FBookL.SetTopLeft(x - 16, y + 2);
				FBookL.ShowBitmap();
				attackL.SetTopLeft(x, y);
				attackL.ShowBitmap();
			}
			if (faceSide == 1)
			{
				FBookR.SetTopLeft(x + 16, y + 2);
				FBookR.ShowBitmap();
				attackR.SetTopLeft(x, y);
				attackR.ShowBitmap();
			}
		}

		else if (onAttack)
		{
			if (faceSide == 0)
			{
				attackL.SetTopLeft(x, y);
				attackL.ShowBitmap();
				swordL.SetTopLeft(x - 31, y + 2);
				swordL.ShowBitmap();
			}
			else if (faceSide == 1)
			{
				attackR.SetTopLeft(x, y);
				attackR.ShowBitmap();
				swordR.SetTopLeft(x + 31, y + 2);
				swordR.ShowBitmap();
			}
		}

		else if (onJump)
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