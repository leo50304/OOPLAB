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
		isMovingRight = isMovingUp = isMovingDown = false;
		isMovingLeft = true;
		speed = 1;
	}

	void Enemy::hit(int d)
	{
		hp -= d;
	}

	void Enemy::Distroy()
	{
		if (hp == 0)
		{
			isDistroyed = true;
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

	void Enemy::LoadBitmap()
	{

	}

	void Enemy::OnAttack(int x, int y)
	{
	}

	void Enemy::MoveWeapon(MapBrown* map)
	{
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

		if (!map->isBlockSolid(x / 32, (y + 32) / 32) || !map->isBlockSolid((x + 22) / 32, (y + 32) / 32) || map->isBlockSolid(x / 32, y / 32) || map->isBlockSolid((x + 22) / 32, y / 32) || x < 0 || x>32 * 17) //�I����t�����
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
	}

	void Frog::LoadBitmap()
	{
		moveLAnimation.AddBitmap(E_L0, RGB(255, 255, 255));
		moveLAnimation.AddBitmap(E_L1, RGB(255, 255, 255));
		moveRAnimation.AddBitmap(E_R0, RGB(255, 255, 255));
		moveRAnimation.AddBitmap(E_R1, RGB(255, 255, 255));
	}

	Slime::Slime(int x, int y, int d) : Enemy(x, y, d)
	{
		srand((unsigned int)time(NULL));
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

		weapon.AddBitmap(SLIME_W0, RGB(128, 0, 128));

		weapon2.AddBitmap(SLIME_W1, RGB(128, 0, 128));
		weapon2.AddBitmap(SLIME_W2, RGB(128, 0, 128));
		weapon2.AddBitmap(SLIME_W3, RGB(128, 0, 128));
		weapon2.AddBitmap(SLIME_W3, RGB(128, 0, 128));

	}

	void Slime::OnMove(MapBrown* map)
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

		if (!map->isBlockSolid(x / 32, (y - 1) / 32) || !map->isBlockSolid((x + 22) / 32, (y - 1) / 32) || map->isBlockSolid(x / 32, y / 32) || map->isBlockSolid((x + 22) / 32, y / 32) || x < 0 || x>32 * 17) //�I����t�����
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

	void Slime::MoveWeapon(MapBrown* map)
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
	}

	void Bat::LoadBitmap()
	{
		moveLAnimation.AddBitmap(BAT_0, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(BAT_1, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(BAT_0, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(BAT_1, RGB(128, 0, 128));
	}

	void Bat::OnMove(MapBrown* map)
	{
		moveLAnimation.OnMove();
		x += directX * 1;
		y += directY * 1;
		countY += 1;
		if (map->isBlockSolid((x) / 32, (y + 5) / 32) || map->isBlockSolid((x + 32) / 32, (y + 5) / 32) || map->isBlockSolid(x / 32, (y + 32 - 8) / 32) || map->isBlockSolid((x + 32) / 32, (y + 32 - 8) / 32))  //�����Y�B�}
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
		hp = 2;
		countRolling = 0;
		weaponState = 0;
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
		weapon2.AddBitmap(BH_W2, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W1, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W0, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W7, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W6, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W5, RGB(128, 0, 128));

	}

	void BowHead::OnMove(MapBrown* map)
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

		if (!map->isBlockSolid(x / 32, (y + 32) / 32) || !map->isBlockSolid((x + 22) / 32, (y + 32) / 32) || map->isBlockSolid(x / 32, y / 32) || map->isBlockSolid((x + 22) / 32, y / 32) || x < 0 || x > 32 * 17) //�I����t�����
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

	void BowHead::MoveWeapon(MapBrown* map)
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
	}

	void Snake::LoadBitmap()
	{
		moveLAnimation.AddBitmap(SNAKE_0, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(SNAKE_1, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(SNAKE_2, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(SNAKE_3, RGB(128, 0, 128));
	}

	bool Snake::HitGround(MapBrown* map)
	{
		return (map->isBlockSolid(x / 32, (y + 32) / 32) && !map->isBlockSolid(x / 32, (y + 1) / 32)) || (map->isBlockSolid((x + 31) / 32, (y + 32) / 32) && !map->isBlockSolid((x + 31) / 32, (y + 1) / 32));
	}

	bool Snake::HitTop(MapBrown* map)
	{
		return (map->isBlockSolid(x / 32, (y - 1) / 32) && !map->isBlockSolid(x / 32, (y + 16) / 32)) || (map->isBlockSolid((x + 31) / 32, (y - 1) / 32) && !map->isBlockSolid((x + 31) / 32, (y + 16) / 32));
	}

	void Snake::OnMove(MapBrown* map)
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
				if (!onDrop) //�W��
				{
					jumpSpeed = (double)jumpSpeed - ACCELERATE;
					y -= (int)jumpSpeed;
				}
				else //�U��
				{
					jumpSpeed = (double)jumpSpeed + ACCELERATE;
					if (jumpSpeed > 16)
					{
						jumpSpeed = 16;
					}
					y += (int)jumpSpeed;
				}
				if (HitGround(map)) //���a
				{
					y = (y / 32) * 32;
					onJump = false;
					onDrop = false;
					speed = 1;
				}
				if (HitTop(map)) //�����Y
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

		if (map->isBlockSolid(x / 32, y / 32) || map->isBlockSolid((x + 31) / 32, y / 32) || x < 0 || x>32 * 17) //���/���
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
		if (!onJump && (!map->isBlockSolid(x / 32, (y + 32) / 32) || !map->isBlockSolid((x + 31) / 32, (y + 32) / 32))) //���
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
		weaponState = 0;
		attackSide = false;
	}
	void Skull::LoadBitmap()
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
		weapon2.AddBitmap(BH_W2, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W1, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W0, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W7, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W6, RGB(128, 0, 128));
		weapon2.AddBitmap(BH_W5, RGB(128, 0, 128));
	}
	void Skull::OnMove(MapBrown* map)
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

		if (!map->isBlockSolid(x / 32, (y + 32) / 32) || !map->isBlockSolid((x + 22) / 32, (y + 32) / 32) || map->isBlockSolid(x / 32, y / 32) || map->isBlockSolid((x + 22) / 32, y / 32) || x < 0 || x > 32 * 17) //�I����t�����
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
		if (count > 50)
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
	void Skull::MoveWeapon(MapBrown* Map)
	{
		if (!onAttack)
		{
			return;
		}
		if (count > 50)
		{
			weapon.OnMove();
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
	}

	void Eye::LoadBitmap()
	{
		moveLAnimation.AddBitmap(EYE_0, RGB(128, 0, 128));
		moveLAnimation.AddBitmap(EYE_1, RGB(128, 0, 128));

		moveRAnimation.AddBitmap(EYE_2, RGB(128, 0, 128));
		moveRAnimation.AddBitmap(EYE_3, RGB(128, 0, 128));

		AtkL.LoadBitmap(EYE_4, RGB(128, 0, 128));
		AtkR.LoadBitmap(EYE_5, RGB(128, 0, 128));

		weapon.AddBitmap(EYE_W1, RGB(128, 0, 128));
		weapon.AddBitmap(EYE_W2, RGB(128, 0, 128));
		weapon.AddBitmap(EYE_W3, RGB(128, 0, 128));
	}

	void Eye::OnMove(MapBrown* map)
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

		if (!map->isBlockSolid(x / 32, (y + 32) / 32) || !map->isBlockSolid((x + 22) / 32, (y + 32) / 32) || map->isBlockSolid(x / 32, y / 32) || map->isBlockSolid((x + 22) / 32, y / 32) || x < 0 || x>32 * 17) //�I����t�����
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
		countAttack = rand() % 100;
		onAttack = true;
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
	void Eye::MoveWeapon(MapBrown* map)
	{
		countAttack--;
		if (countAttack > 0) 
		{
			return;
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
		return false;
	}
}