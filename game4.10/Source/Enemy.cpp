#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>       /* sqrt */
#include "audio.h"
#include "gamelib.h"
#include "Enemy.h"
#include "Block.h"
#include "map.h"

namespace game_framework {
	Enemy::Enemy(int x, int y, int d)
	{
		Initialize();
		hp = 1;
		SetXY(x, y);
		location = d;
		onAttack = false;
		weaponX = -100;
		weaponY = -100;
		directX = 0;
		directY = 0;
		damage = 1;
		weaponDamage = 1;
	}

	void Enemy::ShowWeapon() {};

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
		isVanished = false;
		isMovingRight = isMovingUp = isMovingDown = false;
		isMovingLeft = true;
		speed = 1;
		countVanish = 15;
		thundered = false;
	}

	int Enemy::getDamage() 
	{
		return damage;
	}

	int Enemy::getWeaponDamage()
	{
		return weaponDamage;
	}

	void Enemy::isThundered(bool flag)
	{
		thundered = flag;
	}

	bool Enemy::isThundered()
	{
		return thundered;
	}

	void Enemy::showVanish()
	{
		if (countVanish <= 0) 
		{
			isVanished = true;
		}
		if (isDistroyed && !isVanished) 
		{
			countVanish--;
			vanish.SetTopLeft(GetX1(), GetY1());
			vanish.ShowBitmap();
		}
	}

	void Enemy::hit(int d)
	{
		CAudio::Instance()->Play(HIT_MONSTER, false);		// 撥放 WAVE
		hp -= d;
	}

	int Enemy::getLoot1()
	{
		if (!lootable)
		{
			return 0;
		}
		return rand() % 100 < 40;
	}
	int Enemy::getLoot2()
	{
		if (!lootable)
		{
			return -1;
		}
		if (rand() % 100 > 20)
		{
			return -1;
		}
		else
		{
			return rand() % 5;
		}
	}

	int Enemy::getExp()
	{
		if (lootable)
		{
			lootable = false;
			return exp;
		}
		return 0;
	}

	void Enemy::Distroy()
	{
		if (hp <= 0)
		{
			lootable = true;
			isDistroyed = true;
			CAudio::Instance()->Play(MONSTER_DEAD, false);		// 撥放 WAVE
		}
	}

	int Enemy::GetWeaponX1()
	{
		return weaponX;
	}

	bool Enemy::InWeaponHitBox(int x, int y)
	{
		return false;
	}

	bool Enemy::InAttackRange(int x, int y)
	{
		return false;
	}

	bool Enemy::InHitBox(int x, int y)
	{
		return !isDistroyed && x < GetX1() + 32 && x + 32 > GetX1() && y < GetY1() + 32 && y + 32 > GetY1();
	}

	bool Enemy::IsDistroyed()
	{
		return isDistroyed;
	}

	int Enemy::getMapLocation() 
	{
		return location;
	}

	void Enemy::OnAttack(int x, int y)
	{
	}

	void Enemy::MoveWeapon(Map* map)
	{
	}

	void Enemy::OnMove(Map* map)
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

		if (!map->isBlockSolid(x / 32, (y + 32) / 32) || !map->isBlockSolid((x + 22) / 32, (y + 32) / 32) || map->isBlockSolid(x / 32, y / 32) || map->isBlockSolid((x + 22) / 32, y / 32) || x < 0 || x>32 * 17) //碰到邊緣或邊界
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
			if (onAttackAnime)
			{
				AtkR.SetTopLeft(x, y);
				AtkR.ShowBitmap();
			}
			else
			{
				moveRAnimation.SetTopLeft(x, y);
				moveRAnimation.OnShow();
			}
		}
		else if (isMovingLeft)
		{
			if (onAttackAnime)
			{
				AtkL.SetTopLeft(x, y);
				AtkL.ShowBitmap();
			}
			else
			{
				moveLAnimation.SetTopLeft(x, y);
				moveLAnimation.OnShow();
			}
		}
	}

	Frog::Frog(int x, int y, int d) :Enemy(x, y, d)
	{
		LoadBitmap();
		damage = 1;
	}

	void Frog::LoadBitmap()
	{
		moveLAnimation.AddBitmap(E_L0, RGB(255, 255, 255));
		moveLAnimation.AddBitmap(E_L1, RGB(255, 255, 255));
		moveRAnimation.AddBitmap(E_R0, RGB(255, 255, 255));
		moveRAnimation.AddBitmap(E_R1, RGB(255, 255, 255));
		vanish.LoadBitmap(VANISH, RGB(128, 0, 128));
	}

	Slime::Slime(int x, int y, int d) : Enemy(x, y, d)
	{
		srand((unsigned int)time(NULL));
		damage = 1;
		weaponDamage = 2;
		LoadBitmap();
	}

	void Slime::LoadBitmap()
	{
		moveLAnimation.AddBitmap(SLIME_0, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(SLIME_1, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(SLIME_2, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(SLIME_0, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(SLIME_1, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(SLIME_2, RGB(128, 0, 128));
		vanish.LoadBitmap(VANISH, RGB(128, 0, 128));

		weapon.AddBitmap(SLIME_W0, RGB(128, 0, 128));

		weapon2.AddBitmap(SLIME_W1, RGB(128, 0, 128));
		weapon2.AddBitmap(SLIME_W2, RGB(128, 0, 128));
		weapon2.AddBitmap(SLIME_W3, RGB(128, 0, 128));
		weapon2.AddBitmap(SLIME_W3, RGB(128, 0, 128));

	}

	void Slime::OnMove(Map* map)
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

		if (!map->isBlockSolid(x / 32, (y - 1) / 32) || !map->isBlockSolid((x + 22) / 32, (y - 1) / 32) || map->isBlockSolid(x / 32, y / 32) || map->isBlockSolid((x + 22) / 32, y / 32) || x < 0 || x>32 * 17) //碰到邊緣或邊界
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

	void Slime::OnAttack(int x, int y)
	{
		onAttack = true;
		weaponState = 0;
		weapon.Reset();
		weaponX = GetX1();
		weaponY = GetY1();
		directX = 0;
		directY = 1;
	}

	bool Slime::InWeaponHitBox(int x, int y)
	{
		return onAttack && x < weaponX + 32 && x + 32 > weaponX && y < weaponY + 32 && y + 32 > weaponY;
	}

	void Slime::ShowWeapon()
	{
		if (weaponState == 1)
		{
			weapon2.SetTopLeft(weaponX, weaponY);
			weapon2.OnShow();
			if (weapon2.IsFinalBitmap())
			{
				weaponState = 0;
			}
		}
		if (!onAttack)
		{
			return;
		}
		if (weaponState == 0)
		{
			weapon.SetTopLeft(weaponX, weaponY);
			weapon.OnShow();
		}
	};

	void Slime::MoveWeapon(Map* map)
	{
		if (weaponState == 1)
		{
			weapon2.OnMove();
		}
		if (!onAttack)
		{
			return;
		}
		weapon.OnMove();
		weaponX += directX * 3;
		weaponY += directY * 3;
		if (map->isBlockSolid(weaponX / 32, (weaponY + 31) / 32))
		{
			weaponY = (weaponY / 32) * 32;
			weaponState = 1;
			onAttack = false;
			weapon2.Reset();
		}
	};

	bool Slime::InAttackRange(int x, int y)
	{
		int randAttack = rand() % 200;
		return !isDistroyed && !onAttack && weaponState != 1 && randAttack < 4;
	}

	Bat::Bat(int x, int y, int d) :Enemy(x, y, d)
	{
		LoadBitmap();
		directX = 1;
		directY = 1;
		countY = 0;
		hp = 2;
		damage = 1;
	}

	void Bat::LoadBitmap()
	{
		moveLAnimation.AddBitmap(BAT_0, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(BAT_1, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(BAT_0, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(BAT_1, RGB(128, 0, 128));
		vanish.LoadBitmap(VANISH, RGB(128, 0, 128));

	}

	void Bat::OnMove(Map* map)
	{
		moveLAnimation.OnMove();
		x += directX * 1;
		y += directY * 1;
		countY += 1;
		if (map->isBlockSolid((x) / 32, (y + 5) / 32) || map->isBlockSolid((x + 32) / 32, (y + 5) / 32) || map->isBlockSolid(x / 32, (y + 32 - 8) / 32) || map->isBlockSolid((x + 32) / 32, (y + 32 - 8) / 32))  //撞到頭、腳
		{
			y -= directY * 1;
		}
		if (countY >= 48)
		{
			directY *= -1;
			countY = 0;
		}
		if (map->isBlockSolid(x / 32, (y + 5) / 32) || map->isBlockSolid(x / 32, (y + 32 - 8) / 32) || map->isBlockSolid((x + 32) / 32, (y + 5) / 32) || map->isBlockSolid((x + 32) / 32, (y + 32 - 8) / 32) || x < 0 || x > 32 * 17)
		{
			directX *= -1;
		}
	}

	bool Bat::InHitBox(int x, int y)
	{
		return !isDistroyed && x < GetX1() + 32 && x + 32 > GetX1() && y < GetY1() + 32 - 8 && y + 32 > GetY1() + 5;
	}

	BowHead::BowHead(int x, int y, int d) :Enemy(x, y, d)
	{
		LoadBitmap();
		hp = 4;
		countRolling = 0;
		weaponState = 0;
		damage = 3;
		weaponDamage = 3;
		heroX = x;
		heroY = y;
		attackSide = false;
	}

	void BowHead::LoadBitmap()
	{
		moveLAnimation.AddBitmap(BH_L0, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(BH_L1, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(BH_L2, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(BH_R0, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(BH_R1, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(BH_R2, RGB(128, 0, 128));
		AtkL.LoadBitmap(BH_A0, RGB(128, 0, 128));
		AtkR.LoadBitmap(BH_A1, RGB(128, 0, 128));

		weapon.AddBitmap(BH_W0, RGB(128, 0, 128));
		weapon.AddBitmap(BH_W1, RGB(128, 0, 128));
		weapon.AddBitmap(BH_W2, RGB(128, 0, 128));
		weapon.AddBitmap(BH_W3, RGB(128, 0, 128));
		weapon.AddBitmap(BH_W4, RGB(128, 0, 128));
		weapon.AddBitmap(BH_W5, RGB(128, 0, 128));
		weapon.AddBitmap(BH_W6, RGB(128, 0, 128));
		weapon.AddBitmap(BH_W7, RGB(128, 0, 128));

		weapon2.AddBitmap(BH_W4, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W3, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W1, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W2, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W0, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W7, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W6, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W5, RGB(128, 0, 128));
		vanish.LoadBitmap(VANISH, RGB(128, 0, 128));


	}

	void BowHead::OnMove(Map* map)
	{

		if (onAttack)
		{
			return;
		}
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

		if (!map->isBlockSolid(x / 32, (y + 32) / 32) || !map->isBlockSolid((x + 22) / 32, (y + 32) / 32) || map->isBlockSolid(x / 32, y / 32) || map->isBlockSolid((x + 22) / 32, y / 32) || x < 0 || x > 32 * 17) //碰到邊緣或邊界
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

	void BowHead::OnAttack(int x, int y)
	{
		if (onAttack)
		{
			return;
		}
		weapon.Reset();
		weapon2.Reset();
		weapon.SetDelayCount(10);
		weapon2.SetDelayCount(10);
		onAttackAnime = true;
		onAttack = true;
		weaponX = GetX1();
		weaponY = GetY1();
		heroX = x;
		heroY = y;
		if (weaponX >= heroX)
		{
			attackSide = false;
		}
		else
		{
			attackSide = true;
		}
		directX = (x - GetX1()) / 2;

		directY = (-1) * GetY1();
		unit = sqrt(directX * directX + directY * directY);
		countRolling = 0;
		weaponState = 0;
		weaponMoveX = 0;
		weaponMoveY = 0;
	}

	void BowHead::ShowWeapon()
	{
		if (!onAttack)
		{
			return;
		}
		if (attackSide == false)
		{
			weapon.SetTopLeft((int)(weaponX + weaponMoveX), (int)(weaponY + weaponMoveY));
			weapon.OnShow();
		}
		else
		{
			weapon2.SetTopLeft((int)(weaponX + weaponMoveX), (int)(weaponY + weaponMoveY));
			weapon2.OnShow();
		}
	}

	void BowHead::MoveWeapon(Map* map)
	{
		if (!onAttack)
		{
			return;
		}
		weapon.OnMove();
		weapon2.OnMove();
		int weaponPositionX = (int)(weaponX + weaponMoveX);
		int weaponPositionY = (int)(weaponY + weaponMoveY);

		if (weaponState == 0)
		{
			weaponMoveX += (directX / unit) * 3;
			weaponMoveY += (directY / unit) * 3;
			if (weaponPositionY <= 0)
			{
				weaponMoveY = -1 * weaponY;
				weapon.SetDelayCount(3);
				weapon2.SetDelayCount(3);
				weaponState = 1;
			}
		}
		else if (weaponState == 1)
		{
			if (weaponPositionX >= heroX)
			{
				attackSide = false;
			}
			else
			{
				attackSide = true;
			}
			if (countRolling >= 90)
			{
				countRolling = 0;
				onAttackAnime = false;
				weaponState = 2;
				directX = heroX - weaponPositionX;
				directY = heroY - weaponPositionY;
				unit = sqrt(directX*directX + directY * directY);
			}
			else
			{
				countRolling++;
			}
		}
		else if (weaponState == 2)
		{
			weaponMoveX += (directX / unit) * 16;
			weaponMoveY += (directY / unit) * 16;

			if (weaponPositionX < 0 || weaponPositionX > 18 * 32 || weaponPositionY < 0 || weaponPositionY > 32 * 13)
			{
				onAttack = false;
				onAttackAnime = false;
				weaponMoveX = -100;
				weaponMoveY = -100;
				weaponX = -100;
				weaponY = -100;
			}
		}
	}

	void BowHead::saveHeroPos(int x, int y)
	{
		heroX = x;
		heroY = y;
	}

	bool BowHead::InAttackRange(int x, int y)
	{
		int randAttack = rand() % 250;
		return !isDistroyed && !onAttack && randAttack < 4;
	}

	bool BowHead::InWeaponHitBox(int x, int y)
	{
		int weaponPositionX = (int)(weaponX + weaponMoveX);
		int weaponPositionY = (int)(weaponY + weaponMoveY);
		return x < weaponPositionX + 32 && x + 32 > weaponPositionX && y < weaponPositionY + 32 && y + 32 > weaponPositionY;
	}

	Snake::Snake(int x, int y, int d) :Enemy(x, y, d)
	{
		LoadBitmap();
		onJump = false;
		speed = 1;
		hp = 3;
		damage = 2;
	}

	void Snake::LoadBitmap()
	{
		moveLAnimation.AddBitmap(SNAKE_0, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(SNAKE_1, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(SNAKE_2, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(SNAKE_3, RGB(128, 0, 128));
		vanish.LoadBitmap(VANISH, RGB(128, 0, 128));
	}

	bool Snake::HitGround(Map* map)
	{
		return (map->isBlockSolid(x / 32, (y + 32) / 32) && !map->isBlockSolid(x / 32, (y + 1) / 32)) || (map->isBlockSolid((x + 31) / 32, (y + 32) / 32) && !map->isBlockSolid((x + 31) / 32, (y + 1) / 32));
	}

	bool Snake::HitTop(Map* map)
	{
		return (map->isBlockSolid(x / 32, (y - 1) / 32) && !map->isBlockSolid(x / 32, (y + 16) / 32)) || (map->isBlockSolid((x + 31) / 32, (y - 1) / 32) && !map->isBlockSolid((x + 31) / 32, (y + 16) / 32));
	}

	void Snake::OnMove(Map* map)
	{
		moveRAnimation.OnMove();
		moveLAnimation.OnMove();
		if (!onJump)
		{
			int randJump = rand() % 250;
			if (randJump < 3)
			{
				onJump = true;
				speed = 3;
				jumpTop = y - 32;
				jumpSpeed = 5;
				onDrop = false;
			}
		}

		double ACCELERATE = (0.892*0.3);

		if (onJump)
		{
			if (onJump)
			{
				if (!onDrop) //上升
				{
					jumpSpeed = (double)jumpSpeed - ACCELERATE;
					y -= (int)jumpSpeed;
				}
				else //下降
				{
					jumpSpeed = (double)jumpSpeed + ACCELERATE;
					if (jumpSpeed > 16)
					{
						jumpSpeed = 16;
					}
					y += (int)jumpSpeed;
				}
				if (HitGround(map)) //落地
				{
					y = (y / 32) * 32;
					onJump = false;
					onDrop = false;
					speed = 1;
				}
				if (HitTop(map)) //撞到頭
				{
					jumpSpeed = 1;
					onDrop = true;
					y = ((y - 1) / 32) * 32 + 32;
				}
				if (y <= jumpTop && !onDrop)
				{
					jumpSpeed = 0;
					y = jumpTop;
					onDrop = true;
				}
			}
		}
		if (isMovingLeft)
		{
			x -= speed;
		}
		else if (isMovingRight)
		{
			x += speed;
		}

		bool hitSide = false;

		if (map->isBlockSolid(x / 32, y / 32) || map->isBlockSolid((x + 31) / 32, y / 32) || x < 0 || x>32 * 17) //牆壁/邊界
		{
			if (onJump)
			{
				onDrop = true;
			}
			else
			{
				hitSide = true;
			}
			if (isMovingLeft)
			{
				x += speed;
			}
			else
			{
				x -= speed;
			}
		}
		if (!onJump && (!map->isBlockSolid(x / 32, (y + 32) / 32) || !map->isBlockSolid((x + 31) / 32, (y + 32) / 32))) //踩空
		{
			if (isMovingLeft)
			{
				x += speed;
			}
			else
			{
				x -= speed;
			}
			hitSide = true;
		}
		if (hitSide)
		{
			isMovingLeft = !isMovingLeft;
			isMovingRight = !isMovingRight;
		}
	}

	Skull::Skull(int x, int y, int d) : Enemy(x, y, d)
	{
		LoadBitmap();
		hp = 3;
		damage = 1;
		weapon = 2;
		weaponState = 0;
		attackSide = false;
	}

	void Skull::LoadBitmap()
	{
		moveLAnimation.AddBitmap(SKULL_0, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(SKULL_1, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(SKULL_2, RGB(128, 0, 128));

		moveRAnimation.AddBitmap(SKULL_3, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(SKULL_4, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(SKULL_5, RGB(128, 0, 128));
		AtkL.LoadBitmap(SKULL_6, RGB(128, 0, 128));
		AtkR.LoadBitmap(SKULL_7, RGB(128, 0, 128));

		weapon.AddBitmap(SKULL_W0, RGB(128, 0, 128));
		weapon.AddBitmap(SKULL_W1, RGB(128, 0, 128));
		weapon.AddBitmap(SKULL_W2, RGB(128, 0, 128));
		weapon.AddBitmap(SKULL_W3, RGB(128, 0, 128));
		weapon.AddBitmap(SKULL_W4, RGB(128, 0, 128));
		weapon.AddBitmap(SKULL_W5, RGB(128, 0, 128));
		weapon.AddBitmap(SKULL_W6, RGB(128, 0, 128));
		weapon.AddBitmap(SKULL_W7, RGB(128, 0, 128));

		weapon2.AddBitmap(SKULL_W4, RGB(128, 0, 128));
		weapon2.AddBitmap(SKULL_W3, RGB(128, 0, 128));
		weapon2.AddBitmap(SKULL_W2, RGB(128, 0, 128));
		weapon2.AddBitmap(SKULL_W1, RGB(128, 0, 128));
		weapon2.AddBitmap(SKULL_W0, RGB(128, 0, 128));
		weapon2.AddBitmap(SKULL_W7, RGB(128, 0, 128));
		weapon2.AddBitmap(SKULL_W6, RGB(128, 0, 128));
		weapon2.AddBitmap(SKULL_W5, RGB(128, 0, 128));
		vanish.LoadBitmap(VANISH, RGB(128, 0, 128));
	}

	void Skull::OnMove(Map* map)
	{
		if (onAttackAnime)
		{
			return;
		}
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

		if (!map->isBlockSolid(x / 32, (y + 32) / 32) || !map->isBlockSolid((x + 22) / 32, (y + 32) / 32) || map->isBlockSolid(x / 32, y / 32) || map->isBlockSolid((x + 22) / 32, y / 32) || x < 0 || x > 32 * 17) //碰到邊緣或邊界
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

	void Skull::OnAttack(int x, int y)
	{
		if (onAttack)
		{
			return;
		}
		weapon.Reset();
		weapon2.Reset();
		weapon.SetDelayCount(10);
		weapon2.SetDelayCount(10);
		onAttackAnime = true;
		onAttack = true;
		weaponX = GetX1();
		weaponY = GetY1();
		if (weaponX >= x)
		{
			attackSide = false;
		}
		else
		{
			attackSide = true;
		}
		directX = x - GetX1();
		directY = y - GetY1();
		weaponState = 0;
		weaponMoveX = 0;
		weaponMoveY = 0;
		count = 0;
	}

	void Skull::ShowWeapon()
	{
		if (!onAttack)
		{
			return;
		}
		if (count > 40)
		{
			if (attackSide == false)
			{
				weapon.SetTopLeft((int)(weaponX + weaponMoveX), (int)(weaponY + weaponMoveY));
				weapon.OnShow();
			}
			else
			{
				weapon2.SetTopLeft((int)(weaponX + weaponMoveX), (int)(weaponY + weaponMoveY));
				weapon2.OnShow();
			}
		}
	}

	void Skull::MoveWeapon(Map* Map)
	{
		if (!onAttack)
		{
			return;
		}
		if (count > 40)
		{
			onAttackAnime = false;
			weapon.OnMove();
			weapon2.OnMove();
			int weaponPositionX = (int)(weaponX + weaponMoveX);
			int weaponPositionY = (int)(weaponY + weaponMoveY);
			unit = sqrt(directX*directX + directY * directY);

			if (weaponState == 0)
			{
				weaponMoveX += (directX / unit) * 3;
				weaponMoveY += (directY / unit) * 3;

				if (weaponPositionX < 0 || weaponPositionX > 18 * 32 || weaponPositionY < 0 || weaponPositionY > 32 * 13)
				{
					onAttack = false;
					onAttackAnime = false;
					weaponMoveX = -100;
					weaponMoveY = -100;
					weaponX = -100;
					weaponY = -100;
				}
			}
		}
		else
		{
			count++;
		}
	}

	bool Skull::InAttackRange(int x, int y)
	{
		int randAttack = rand() % 250;
		return !isDistroyed && !onAttack && randAttack < 4;
	}

	bool Skull::InWeaponHitBox(int x, int y)
	{
		int weaponPositionX = (int)(weaponX + weaponMoveX);
		int weaponPositionY = (int)(weaponY + weaponMoveY);
		return x < weaponPositionX + 32 && x + 32 > weaponPositionX && y < weaponPositionY + 32 && y + 32 > weaponPositionY;
	}

	Eye::Eye(int x, int y, int d) :Enemy(x, y, d)
	{
		LoadBitmap();
		hp = 4;
		damage = 1;
		weaponDamage = 3;
	}

	void Eye::LoadBitmap()
	{
		moveLAnimation.AddBitmap(EYE_0, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(EYE_1, RGB(128, 0, 128));

		moveRAnimation.AddBitmap(EYE_2, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(EYE_3, RGB(128, 0, 128));

		AtkL.LoadBitmap(EYE_4, RGB(128, 0, 128));
		AtkR.LoadBitmap(EYE_5, RGB(128, 0, 128));
		vanish.LoadBitmap(VANISH, RGB(128, 0, 128));

		weapon.AddBitmap(EYE_W1, RGB(128, 0, 128));
		weapon.AddBitmap(EYE_W2, RGB(128, 0, 128));
		weapon.AddBitmap(EYE_W3, RGB(128, 0, 128));
	}

	void Eye::OnMove(Map* map)
	{
		if (onAttack)
		{
			return;
		}

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

		if (!map->isBlockSolid(x / 32, (y + 32) / 32) || !map->isBlockSolid((x + 22) / 32, (y + 32) / 32) || map->isBlockSolid(x / 32, y / 32) || map->isBlockSolid((x + 22) / 32, y / 32) || x < 0 || x>32 * 17) //碰到邊緣或邊界
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

	void Eye::OnAttack(int x, int y)
	{
		countAttack = rand() % 150;
		onAttack = true;
		onAttackAnime = true;
		weapon.Reset();
		weaponX = GetX1();
		weaponY = GetY1();
		if (x <= GetX1())
		{
			directX = -1;
			weaponX -= 32;
			attackSide = false;
		}
		else
		{
			directX = 1;
			weaponX += 32;
			attackSide = true;
		}
	}

	void Eye::MoveWeapon(Map* map)
	{
		countAttack--;
		if (countAttack > 0)
		{
			return;
		}

		if (countAttack < -10)
		{
			onAttackAnime = false;
		}

		weapon.OnMove();

		weaponX += directX * 10;

		if (weaponX < 0 || weaponX > 18 * 32)
		{
			onAttack = false;
			weaponX = -100;
			weaponY = -100;
		}
	}

	bool Eye::InAttackRange(int x, int y)
	{
		if (onAttack)
		{
			return false;
		}
		if ((isMovingLeft && x < GetX1()) || (isMovingRight && x > GetX1()))
		{
			return (y + 31) > GetY1() && y < GetY1() + 32;
		}
		return false;
	}

	void Eye::ShowWeapon()
	{
		if (!onAttack || countAttack > 0)
		{
			return;
		}
		weapon.SetTopLeft(weaponX, weaponY);
		weapon.OnShow();
	}

	bool Eye::InWeaponHitBox(int x, int y)
	{
		return x < weaponX + 32 && x + 32 > weaponX && weaponY + 16 > y && weaponY + 16 < y + 32;
	}

	Orc::Orc(int x, int y, int d) :Enemy(x, y, d)
	{
		LoadBitmap();
		countSprint = 0;
		sprintSpeed = 0;
		speed = 0;
		hp = 3;
		damage = 2;
		onAttackAnime = true;//true代表不衝刺
	}

	void Orc::LoadBitmap()
	{
		moveLAnimation.AddBitmap(ORC_0, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(ORC_1, RGB(128, 0, 128));
		AtkL.LoadBitmap(ORC_2, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(ORC_3, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(ORC_4, RGB(128, 0, 128));
		AtkR.LoadBitmap(ORC_5, RGB(128, 0, 128));
		vanish.LoadBitmap(VANISH, RGB(128, 0, 128));
	}

	void  Orc::OnMove(Map* map)
	{
		if (countSprint <= 0)
		{
			onAttackAnime = true;
			sprintSpeed = 0;
		}
		if (!onAttackAnime)
		{
			moveRAnimation.OnMove();
			moveLAnimation.OnMove();
			countSprint--;
		}
		else
		{
			int randSprint = rand() % 100;
			if (randSprint < 6)
			{
				onAttackAnime = false;
				countSprint = 40;
				sprintSpeed = 3;
			}
		}

		if (isMovingLeft)
		{
			x -= speed + sprintSpeed;
		}
		else if (isMovingRight)
		{
			x += speed + sprintSpeed;
		}

		if (!map->isBlockSolid(x / 32, (y + 32) / 32) || !map->isBlockSolid((x + 22) / 32, (y + 32) / 32) || map->isBlockSolid(x / 32, y / 32) || map->isBlockSolid((x + 22) / 32, y / 32) || x < 0 || x>32 * 17) //碰到邊緣或邊界
		{
			if (isMovingLeft)
			{
				x += speed + sprintSpeed;
			}
			else
			{
				x -= speed + sprintSpeed;
			}
			isMovingLeft = !isMovingLeft;
			isMovingRight = !isMovingRight;
		}
	}

	bool Orc::InAttackRange(int x, int y)
	{
		return true;
	}

	MBall::MBall(int x, int y, int d) : Enemy(x, y, d)
	{
		initX = x;
		initY = y;
		LoadBitmap();
		onJump = false;
		onDrop = false;
		counter = 0;
		damage = 1;
		hp = 200;
	}

	void  MBall::LoadBitmap()
	{
		moveLAnimation.AddBitmap(MBALL_0, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(MBALL_1, RGB(128, 0, 128));
		vanish.LoadBitmap(VANISH, RGB(128, 0, 128));
	}

	void  MBall::OnMove(Map* map)
	{
		if (counter > 0)
		{
			counter--;
		}
		if (onJump == false && counter <= 0)
		{
			onJump = true;
			onDrop = false;
			speed = 1;
			jumpSpeed = 16;
			jumpTop = 0;
			x = initX;
			y = initY;
			counter = 0;
		}
		moveLAnimation.OnMove();

		double ACCELERATE = (0.892*0.33);

		if (onJump)
		{
			if (!onDrop) //上升
			{
				jumpSpeed = (double)jumpSpeed - ACCELERATE;
				y -= (int)jumpSpeed;
			}
			else //下降
			{
				jumpSpeed = (double)jumpSpeed + ACCELERATE;
				if (jumpSpeed > 16)
				{
					jumpSpeed = 16;
				}
				y += (int)jumpSpeed;
			}
			if (y > 13 * 32) //落地
			{
				y = 100 * 32;
				onJump = false;
				onDrop = false;
				counter = 108;
			}

			if (y <= jumpTop && !onDrop)
			{
				jumpSpeed = 0;
				y = jumpTop;
				onDrop = true;
			}
		}

		if (isMovingLeft)
		{
			x -= speed;
		}
		else if (isMovingRight)
		{
			x += speed;
		}
	}

	bool  MBall::InAttackRange(int x, int y)
	{
		return false;
	}
	Boss::Boss(int x, int y, int d) : Enemy(x, y, d)
	{
		directX = 3;
		directY = 1;
		countY = 0;
		counthit = 0;
		LoadBitmap();
		weaponState = 0;
		onAttack = false;
		attackSide = false;
		hit = false;
		hp = 120;
		damage = 3;
		weaponDamage = 5;
		weapon1X = -100;
		weapon1Y = -100;
		weapon2X = -100;
		weapon2Y = -100;
		weapon3X = -100;
		weapon3Y = -100;
	}

	void Boss::Initialize()
	{
		isMovingRight = isMovingUp = isMovingDown = false;
		isMovingLeft = true;
		speed = 1;
		mode = 0;
	}

	void Boss::LoadBitmap()
	{
		moveLAnimation.AddBitmap(boss_1, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(boss_2, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(boss_3, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(boss_1, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(boss_2, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(boss_3, RGB(128, 0, 128));
		weapon1.AddBitmap(boss_attack_1, RGB(128, 0, 128));
		weapon1.AddBitmap(boss_attack_2, RGB(128, 0, 128));
		weapon1.AddBitmap(boss_attack_3, RGB(128, 0, 128));
		weapon2.AddBitmap(boss_attack_2, RGB(128, 0, 128));
		weapon2.AddBitmap(boss_attack_3, RGB(128, 0, 128));
		weapon2.AddBitmap(boss_attack_1, RGB(128, 0, 128));
		weapon3.AddBitmap(boss_attack_3, RGB(128, 0, 128));
		weapon3.AddBitmap(boss_attack_1, RGB(128, 0, 128));
		weapon3.AddBitmap(boss_attack_2, RGB(128, 0, 128));
		vanish.LoadBitmap(VANISH, RGB(128, 0, 128));
	}
	void Boss::OnMove(Map* map)
	{
		moveLAnimation.OnMove();
		x += (int)(directX);
		y += (int)(directY);

		countY += 1;
		if (countY >= 40)
		{
			if (!onAttack)
			{
				directY *= -1;
				countY = 0;
			}
		}
		if (mode == 0)
		{
			if ((y >= 32 * 13 || y <= -8) && hit)
			{
				// boss and item image order
				// to do find hero position and move to hero position.
				//撞完、重置boss位置
				if (counthit >= 5)
				{
					if (y >= 32 * 15 || y <= -64)
					{
						x = -200;
						y = 30;
						directX = 3;
						directY = 1;
						hit = false;
						onAttack = false;
						counthit = 0;
						mode = 1;
					}
				}
				else
				{
					directY *= -1;
					counthit++;
				}
			}

			if ((x >= 32 * 17 || x <= -8) && hit)
			{
				if (counthit >= 5)
				{
					if (x >= 32 * 19 || x <= -64)
					{
						x = -200;
						y = 30;
						directX = 3;
						directY = 1;
						hit = false;
						onAttack = false;
						counthit = 0;
						mode = 1;

					}
				}
				else
				{
					directX *= -1;
					counthit++;
				}
			}
		}
		else if (mode == 1)
		{
			directX = 3;
			directY = 0;
			if (x > 32 * 8)
			{
				directX = 0;
				directY = 0;
				weapon1X = x + 32;
				weapon1Y = y + 32;
			}
		}
	}

	int Boss::getLoot2() 
	{
		if (!lootable)
		{
			return -1;
		}
		else
		{
			return 5;
		}
	}

	bool Boss::InAttackRange(int x, int y)
	{
		if (GetX1() >= 32 * 16)
		{
			mode = 0;
			return true;
		}
		if (GetX1() >= 32 * 8 && mode == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void Boss::showVanish() 
	{
		return;
	}

	void Boss::OnAttack(int x, int y) //判定要不要攻擊
	{
		if (onAttack)
		{
			return;
		}
		onAttack = true;
		hit = true;
		dx = x - GetX1() - 32;
		dy = y - GetY1() - 32;
		dxl = dx + 96;
		dxr = dx - 96;
		unit = sqrt(dx * dx + dy * dy);
		unitl = sqrt(dxl * dxl + dy * dy);
		unitr = sqrt(dxr * dxr + dy * dy);
		directX = (dx / unit) * 8;
		directY = (dy / unit) * 8;
		direct1X = directX;
		direct1Y = directY;
		direct2X = (dxl / unitl) * 8;
		direct2Y = (dy / unitl) * 8;
		direct3X = (dxr / unitr) * 8;
		direct3Y = (dy / unitr) * 8;
		weapon1.Reset();// direct attack hero
		weapon2.Reset();// attack hero like BowHead
		weapon3.Reset();// attack hero by a circle
		weapon1X = GetX1();
		weapon1Y = GetY1();
		weapon2X = GetX1() - 96;
		weapon2Y = GetY1();
		weapon3X = GetX1() + 96;
		weapon3Y = GetY1();
		weaponState = 0;
		weaponMoveX = 0;
		weaponMoveY = 0;
		weaponMove2X = 0;
		weaponMove2Y = 0;
		weaponMove3X = 0;
		weaponMove3Y = 0;
		count = 0;
	}
	// bug weapon1 and weapon2 move same 
	void Boss::MoveWeapon(Map* map)
	{
		if (!onAttack)
		{
			return;
		}
		if (mode == 1 && (x >= 32 * 8))
		{
			// weapon 1
			weapon1.OnMove();
			weaponPositionX = (int)(weapon1X + weaponMoveX);
			weaponPositionY = (int)(weapon1Y + weaponMoveY);
			//unit1 = sqrt(direct1X * direct1X + direct1Y * direct1Y);
			//weapon 2
			weapon2.OnMove();
			weaponPosition2X = (int)(weapon2X + weaponMove2X) + 64;
			weaponPosition2Y = (int)(weapon2Y + weaponMove2Y);
			// weapon 3
			weapon3.OnMove();
			weaponPosition3X = (int)(weapon3X + weaponMove3X);
			weaponPosition3Y = (int)(weapon3Y + weaponMove2Y);
			//unit2 = sqrt((direct2X) * (direct2X) + direct2Y * direct2Y);
			if (weaponState == 0)
			{
				weaponMove3X += (direct3X);
				weaponMove3Y += (direct3Y);
				weaponMove2X += (direct2X);
				weaponMove2Y += (direct2Y);
				weaponMoveX += (direct1X);
				weaponMoveY += (direct1Y);
				if (weaponPositionX < 0 || weaponPositionX > 20 * 32 || weaponPositionY < 0 || weaponPositionY > 32 * 18)
				{
					weaponPositionX = -100;
					weaponPositionY = -100;
					directX = 2;
					directY = 0;
					hit = false;
					onAttack = false;
					mode = 0;
				}
				if (weaponPosition2X < 0 || weaponPosition2X > 20 * 32 || weaponPosition2Y < 0 || weaponPosition2Y > 32 * 18)
				{
					weaponPosition2X = -100;
					weaponPosition2Y = -100;
				}
				if (weaponPosition3X < 0 || weaponPosition3X > 20 * 32 || weaponPosition3Y < 0 || weaponPosition3Y > 32 * 18)
				{
					weaponPosition3X = -100;
					weaponPosition3Y = -100;
				}
			}
			else
			{
				count++;
			}
		}
	}
	void Boss::ShowWeapon()
	{
		if (!onAttack)
		{
			return;
		}
		weapon1.SetTopLeft((int)(weaponPositionX), (int)(weaponPositionY));
		weapon1.OnShow();
		weapon2.SetTopLeft((int)(weaponPosition2X), (int)(weaponPositionY));
		weapon2.OnShow();
		weapon3.SetTopLeft((int)(weaponPosition3X), (int)(weaponPositionY));
		weapon3.OnShow();
	}

	bool Boss::InHitBox(int x, int y)
	{
		return !isDistroyed && x < GetX1() + 96 && x + 32 > GetX1() && y < GetY1() + 96 && y + 32 > GetY1();
	}

	bool Boss::InWeaponHitBox(int x, int y)
	{
		bool flag1 = x < weaponPositionX + 32 && x + 32 > weaponPositionX && y < weaponPositionY + 32 && y + 32 > weaponPositionY;
		bool flag2 = x < weaponPosition2X + 32 && x + 32 > weaponPosition2X && y < weaponPosition2Y + 32 && y + 32 > weaponPosition2Y;
		bool flag3 = x < weaponPosition3X + 32 && x + 32 > weaponPosition3X && y < weaponPosition3Y + 32 && y + 32 > weaponPosition3Y;
		return flag1 || flag2 || flag3;
	}
}