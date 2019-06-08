#ifndef ENEMY_H
#define ENEMY_H

#include "stdafx.h"
#include "map.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H����L�ηƹ�������l
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////

	class Enemy
	{
	public:
		Enemy(int x, int y, int d);
		int  GetX1();					// �W x �y��
		int  GetY1();					// �W y �y��
		void Initialize();				// �]�w���l����l��
		virtual void LoadBitmap() = 0;				// ���J�ϧ�
		virtual void OnMove(MapBrown* map);     // ����
		virtual void MoveWeapon(MapBrown* map);
		virtual void OnAttack(int x, int y);
		virtual void ShowWeapon();
		//bool HitHeroAction(int type, string pos);
		void OnShow();      			// �N���l�ϧζK��e��
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void SetXY(int nx, int ny);		// �]�w���l���W���y��
		void Distroy();
		virtual int getExp();
		void hit(int d);
		virtual bool InAttackRange(int x, int y);
		virtual bool InWeaponHitBox(int x, int y);
		int GetWeaponX1();
		int getLoot1();
		int getLoot2();
		virtual bool InHitBox(int x, int y);
		bool IsDistroyed();
		int getMapLocation()
		{
			return location;
		}

	protected:
		bool lootable = false;
		int exp = 2;
		int loot1;
		int loot2;
		int location;
		int speed;
		bool onAttack;
		int directX;
		int directY;
		int hp;
		CAnimation moveRAnimation;
		CAnimation moveLAnimation;
		CMovingBitmap AtkL;
		CMovingBitmap AtkR;
		CAnimation weapon;
		CAnimation weapon2;

		int x, y;
		int weaponX;
		int weaponY;
		bool onDrop = false;
		bool isDistroyed = false;
		bool isMovingDown;
		bool isMovingLeft;
		bool isMovingRight;
		bool isMovingUp;
		bool onAttackAnime = false;
	};

	class Frog : public Enemy
	{
	public:
		Frog(int x, int y, int d);
		void LoadBitmap();
	};

	class Slime : public Enemy
	{
	public:
		Slime(int x, int y, int d);
		void LoadBitmap();
		void OnAttack(int x, int y);
		void OnMove(MapBrown* map);     // ����
		void ShowWeapon();
		void MoveWeapon(MapBrown* map);
		bool InAttackRange(int x, int y);
		bool InWeaponHitBox(int x, int y);
	private:
		int weaponState;
	};

	class Bat : public Enemy
	{
	public:
		Bat(int x, int y, int d);
		void LoadBitmap();
		void OnMove(MapBrown* map);     // ����
		bool InHitBox(int x, int y);

	private:
		int countY;
	};

	class BowHead : public Enemy
	{
	public:
		BowHead(int x, int y, int d);
		void LoadBitmap();
		void OnMove(MapBrown* map);
		void OnAttack(int x, int y);
		void ShowWeapon();
		void MoveWeapon(MapBrown* map);
		bool InAttackRange(int x, int y);
		void saveHeroPos(int x, int y);
		bool InWeaponHitBox(int x, int y);
	private:
		double weaponMoveX, weaponMoveY, unit;
		int weaponState;
		int countRolling;
		int heroX, heroY;
		bool attackSide;
	};

	class Snake : public Enemy 
	{
	public:
		Snake(int x, int y, int d);
		void LoadBitmap();
		void OnMove(MapBrown* map);
		bool HitTop(MapBrown* map);
		bool HitGround(MapBrown* map);

	private:
		bool onJump;
		bool onDrop;
		int jumpTop;
		double jumpSpeed;
	};

	class Skull : public Enemy
	{
	public:
		Skull(int x, int y, int d);
		void LoadBitmap();
		void OnMove(MapBrown* map);
		void OnAttack(int x, int y);
		void ShowWeapon();
		void MoveWeapon(MapBrown* map);
		bool InAttackRange(int x, int y);
		bool InWeaponHitBox(int x, int y);
	private:
		double weaponMoveX, weaponMoveY, unit;
		int weaponState;
		bool attackSide;
		int count;
	};

	class Eye : public Enemy 
	{
	public:
		Eye(int x, int y, int d);
		void LoadBitmap();
		void OnMove(MapBrown* map);
		void OnAttack(int x, int y);
		void MoveWeapon(MapBrown* map);
		bool InAttackRange(int x, int y);
		void ShowWeapon();
		bool InWeaponHitBox(int x, int y);

	private:
		int countAttack;
		bool attackSide;
	};

	class Orc : public Enemy
	{
	public:
		Orc(int x, int y, int d);
		void LoadBitmap();
		void OnMove(MapBrown* map);
		bool InAttackRange(int x, int y);

	private:
		bool attackSide;
		int sprintSpeed;
		int countSprint;
	};

	class MBall : public Enemy
	{
	public:
		MBall(int x, int y, int d);
		void LoadBitmap();
		void OnMove(MapBrown* map);
		bool InAttackRange(int x, int y);

	private:
		int initX, initY;
		int counter;
		bool onJump, onDrop;
		int jumpTop;
		double jumpSpeed;
	};
}
#endif
