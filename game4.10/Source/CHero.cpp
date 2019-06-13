#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CHero.h"
#include "Block.h"
#include "map.h"
#include "mygame.h"


namespace game_framework {

	CHero::CHero()
	{
		Initialize();
	}

	CHero::~CHero()
	{
		for (unsigned int i = 0; i < items.size(); ++i)
		{
			delete items[i];
		}

		for (unsigned int i = 0; i < usedItem.size(); ++i)
		{
			delete usedItem[i];
		}
	}

	int CHero::GetX1()
	{
		return x;
	}

	int CHero::GetY1()
	{
		return y;
	}

	void CHero::useItem()
	{
		if (items.size() == 0)
		{
			return;
		}
		usedItem.push_back(items[currentItem]);
		items[currentItem]->use(this);
		items.erase(items.begin() + currentItem);
		if (currentItem > (signed)(items.size()) - 1)
		{
			currentItem = (signed)(items.size()) - 1;
		}
	}

	int CHero::getGold() 
	{
		return gold;
	}
	void CHero::changeGold(int i) 
	{
		if (gold == 999) 
		{
			return;
		}
		gold += i;
		if (gold > 999) 
		{
			gold = 999;
		}
	}

	void CHero::addHp(int num)
	{
		hp += num;
		if (hp > maxHP) 
		{
			hp = maxHP;
		}
	}

	void CHero::addMaxHp(int num)
	{
		maxHP += num;
		if (maxHP <1)
		{
			maxHP = 1;
		}
		if (hp > maxHP) 
		{
			hp = maxHP;
		}
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

	void CHero::showItemList()
	{
		for (unsigned int i = 0; i < items.size(); ++i)
		{
			items[i]->setXY(591 - 8, 1 + 32 * i);
			items[i]->ShowIcon();
		}
	}

	int CHero::getCurrentItem()
	{
		if (items.size() == 0)
		{
			return 100;
		}
		return currentItem;
	}

	void CHero::addItem(Item* item)
	{
		if (items.size() < 13)
		{
			item->resetOffset();
			items.push_back(item);
		}
	}

	bool CHero::BeatBack()
	{
		return beatBack;
	}

	bool CHero::ItemMax()
	{
		return items.size() >= 13;
	}

	bool  CHero::buyItem(Item* item, int price) 
	{
		if (gold >= price) 
		{
			addItem(item);
			if (gold < 999) 
			{
				gold -= price;
			}
			return true;
		}
		return false;
	}


	void CHero::Initialize()
	{
		const int X_POS = 32 * 2;
		const int Y_POS = 32 * 11;
		x = X_POS;
		y = Y_POS;
		godMode = false;
		isOnLadder = false;
		onDrop = false;
		initThunder = false;
		onAttack = false;
		onFire = false;
		onBook = false;
		hp = 8;
		maxHP = 8;
		previousBlock = 0;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;


		onCTRL = false;
		hitValid = false;
		onThunder = false;
		beatBack = false;
		canWin = false;
		onFinishGame = false;
		attackFrameCount = 0;
		fireFrameCount = 0;
		bookFrameCount = 0;
		invincibleFrameCount = 0;
		damage = 1;
		gold = 3;
		level = 1;
		exp = 0;
		maxExp = 10;
		faceSide = 1;
		onDrop = false;
		onJump = false;
		onHold = false;
		for (unsigned int i = 0; i < items.size(); ++i)
		{
			delete items[i];
		}

		for (unsigned int i = 0; i < usedItem.size(); ++i)
		{
			delete usedItem[i];
		}
		items.clear();
		usedItem.clear();

	}

	void CHero::setHitValid(bool flag)
	{
		hitValid = flag;
	}

	int CHero::getDamage()
	{
		return damage;
	}

	void CHero::InitThunder(bool flag) 
	{
		initThunder = flag;
	}

	bool CHero::InitThunder()
	{
		return initThunder;
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

	bool CHero::isGodMode() 
	{
		return godMode;
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

	int CHero::getHP()
	{
		return hp;
	}

	void CHero::addExp(int n)
	{
		exp += n;
		if (exp >= maxExp)
		{
			int dif = exp - maxExp;
			exp = 0;
			addLevel();
			addExp(dif);
		}
	}

	void CHero::addLevel() 
	{
		level++;
		maxExp = int(maxExp * 1.1 + 0.5);
		maxHP += 5;
		hp += 5;
		damage += 1;
	}

	int CHero::getLevel()
	{
		return level;
	}

	int CHero::getMaxHP()
	{
		return maxHP;
	}

	int CHero::getMaxExp()
	{
		return maxExp;
	}

	int CHero::getExp()
	{
		return exp;
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
		FBookL.LoadBitmap(fire_right, RGB(128, 0, 128));
		FBookR.LoadBitmap(fire_left, RGB(128, 0, 128));
		FireL.AddBitmap(attack_fire_left_1, RGB(128, 0, 128));
		FireL.AddBitmap(attack_fire_left_2, RGB(128, 0, 128));
		FireL.AddBitmap(attack_fire_left_3, RGB(128, 0, 128));
		FireR.AddBitmap(attack_fire_right_1, RGB(128, 0, 128));
		FireR.AddBitmap(attack_fire_right_2, RGB(128, 0, 128));
		FireR.AddBitmap(attack_fire_right_3, RGB(128, 0, 128));
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
		if (flag)
		{
			CAudio::Instance()->Play(ATK_EF, false);		// 撥放 WAVE
		}
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
		if (onFire)
		{
			CAudio::Instance()->Play(FIRE_ATK_EF, false);		// 撥放 WAVE
		}
	}


	bool CHero::HitGround(Map* map)
	{
		if (y % 32 == 0) 
		{
			return (map->isBlockSolid((x + 4) / 32, (y + 32) / 32) && !map->isBlockSolid((x + 4) / 32, (y + 1) / 32)) || (map->isBlockSolid((x + 27) / 32, (y + 32) / 32) && !map->isBlockSolid((x + 27) / 32, (y + 1) / 32));
		}
		//return map->isBlockSolid(x / 32, (y + 32) / 32) || map->isBlockSolid((x + 31) / 32, (y + 32) / 32);
		return (map->isBlockSolid((x) / 32, (y + 32) / 32) && !map->isBlockSolid((x + 4) / 32, (y + 1) / 32)) || (map->isBlockSolid((x + 31) / 32, (y + 32) / 32) && !map->isBlockSolid((x + 31) / 32, (y + 1) / 32));
	}

	bool CHero::HitTop(Map* map)
	{
		//return  map->isBlockSolid(x / 32, (y - 1) / 32) || map->isBlockSolid((x + 31) / 32, (y - 1) / 32);
		return (map->isBlockSolid(x / 32, (y - 1) / 32) && !map->isBlockSolid(x / 32, (y + 16) / 32)) || (map->isBlockSolid((x + 31) / 32, (y - 1) / 32) && !map->isBlockSolid((x + 31) / 32, (y + 16) / 32));
	}

	bool CHero::GroundNotSolid(Map* map)
	{
		return !map->isBlockSolid((x+4) / 32, (y + 32) / 32) && !map->isBlockSolid((x + 31) / 32, (y + 32) / 32);
	}

	bool CHero::isInvincible()
	{
		return invincibleFrameCount > 0;
	}

	void CHero::OnMove(Map* map)
	{

		if (map->InStore()) 
		{
			return;
		}

 	for (unsigned int i = 0; i < items.size(); ++i)
		{
			items[i]->MoveIcon();
		}

		if (invincibleFrameCount > 0)
		{
			invincibleFrameCount--;
		}

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
				CAudio::Instance()->Play(DROP_EF, false);		// 撥放 WAVE
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

			if (map->GetBlock(px, py + 1) == 4)
			{
				y += STEP_SIZE;
				map->enterStore(true);
				x = px * 32;
			}
			else if (!map->getMapObject(map->GetBlock(px, py))->HitHeroAction(x, y, isOnLadder, "Up", px * 32))
			{
				y += STEP_SIZE;

				if (!onJump && !onHold)
				{
					CAudio::Instance()->Play(JUMP_EF, false);		// 撥放 WAVE
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

		bool changeFlag = true;
		if (x < 0)//往左過圖
		{
			map->UpdateMap('L');
			x = 32 * 18 - 32;
			if (map->getNext() == 7)
			{
				x = 32 * 6;
				y = 32 * 1;
			}
		} 
		else if (x > 32 * 18 - 32)//往右過圖
		{
			map->UpdateMap('R');
			x = 0;
			if (map->getNext() == 7)
			{
				x = 32 * 6;
				y = 32 * 1;
			}
		}
		else if (y < 0)//往上過圖
		{
			map->UpdateMap('U');
			y = 32 * 13 - 32;
			if (map->getNext() == 7)
			{
				x = 32 * 6;
				y = 32 * 1;
			}
		}
		else if (y > 32 * 13 - 32)//往下過圖
		{
			map->UpdateMap('D');
			y = 0;
			if (map->getNext() == 7)
			{
				x = 32 * 9;
				y = 32 * 1;
			}
		}
		else 
		{
			changeFlag = false;
		}
		if (changeFlag) 
		{
			SetThunder(false);
		}

		if (map->getNext() == 7 && canWin)
		{
			onFinishGame = true;
		}
		if (onFire)
		{
			if (FireSide == 0)
			{
				FireX -= 8;
				FireL.OnMove();
			}
			else if (FireSide == 1)
			{
				FireX += 8;
				FireR.OnMove();
			}
			if (FireX >= 18 * 32 - 24 || FireX <= 0 - 8)
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

	void CHero::setWin(bool flag) 
	{
		canWin = flag;
	}

	void CHero::moveMap() 
	{
		SetThunder(false);
		isOnLadder = false;
		isOnLadderSide = false;
		onDrop = false;
		onJump = false;
	}

	bool CHero::FinishGame() 
	{
		return onFinishGame;
	}

	void CHero::ToggleInfiniteGold() 
	{
		gold = 999;
	}

	void CHero::ToggleGodStatus() 
	{
		if (godMode == false)
		{
			tempLevel = level;
			tempHp = hp;
			tempMaxHP = maxHP;
			tempMaxExp = maxExp;
			tempDamage = damage;
			level = 999;
			hp = 9999;
			maxHP = 9999;
			maxExp = 9999;
			damage = 9999;
			exp = 0;
			godMode = true;
		}
		else 
		{
			level = tempLevel;
			hp = tempHp;
			maxHP = tempMaxHP;
			maxExp = tempMaxExp;
			exp = 0;
			damage = tempDamage;
			godMode = false;
		}
	}

	void CHero::moveCurrentItem(int i)
	{
		int preItem = currentItem;
		currentItem += i;
		if (currentItem > signed(items.size()) - 1)
		{
			currentItem = 0;
		}
		else if (currentItem < 0)
		{
			currentItem = items.size() - 1;
		}
		if (preItem != currentItem)
		{
			items[preItem]->RemoveSelect(this);
			items[currentItem]->OnSelect(this);
		}
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