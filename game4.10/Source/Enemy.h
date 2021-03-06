#ifndef ENEMY_H
#define ENEMY_H

#include "stdafx.h"
#include "map.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供可以用鍵盤或滑鼠控制的擦子
	// 看懂就可以改寫成自己的程式了
	/////////////////////////////////////////////////////////////////////////////

	class Enemy
	{
	public:
		Enemy(int x, int y, int d);
		int  GetX1();					// 上 x 座標
		int  GetY1();					// 上 y 座標
		void Initialize();				// 設定擦子為初始值
		virtual void showVanish();
		virtual void LoadBitmap() = 0;				// 載入圖形
		virtual void OnMove(Map* map);     // 移動
		virtual void MoveWeapon(Map* map);
		virtual void OnAttack(int x, int y);
		virtual void ShowWeapon();
		bool isThundered();
		void isThundered(bool flag);
		//bool HitHeroAction(int type, string pos);
		void OnShow();      			// 將擦子圖形貼到畫面
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		void SetXY(int nx, int ny);		// 設定擦子左上角座標
		void Distroy();
		virtual int getDamage();
		virtual int getWeaponDamage();
		virtual int getExp();
		void hit(int d);
		virtual bool InAttackRange(int x, int y);
		virtual bool InWeaponHitBox(int x, int y);
		int GetWeaponX1();
		int getLoot1();
		virtual int getLoot2();
		virtual bool InHitBox(int x, int y);
		bool IsDistroyed();
		int getMapLocation();

	protected:
		bool thundered;
		bool isVanished;
		int countVanish;
		bool lootable = false;
		int exp = 2;
		int loot1;
		int loot2;
		int location;
		int speed;
		bool onAttack;
		int directX;
		int directY;
		int hp, damage, weaponDamage;
		CAnimation moveRAnimation;
		CAnimation moveLAnimation;
		CMovingBitmap AtkL;
		CMovingBitmap AtkR;
		CAnimation weapon;
		CAnimation weapon2;
		CAnimation weapon1;
		CAnimation weapon3;
		CMovingBitmap vanish;
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
		void OnMove(Map* map);     // 移動
		void ShowWeapon();
		void MoveWeapon(Map* map);
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
		void OnMove(Map* map);     // 移動
		bool InHitBox(int x, int y);

	private:
		int countY;
	};

	class BowHead : public Enemy
	{
	public:
		BowHead(int x, int y, int d);
		void LoadBitmap();
		void OnMove(Map* map);
		void OnAttack(int x, int y);
		void ShowWeapon();
		void MoveWeapon(Map* map);
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
		void OnMove(Map* map);
		bool HitTop(Map* map);
		bool HitGround(Map* map);

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
		void OnMove(Map* map);
		void OnAttack(int x, int y);
		void ShowWeapon();
		void MoveWeapon(Map* map);
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
		void OnMove(Map* map);
		void OnAttack(int x, int y);
		void MoveWeapon(Map* map);
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
		void OnMove(Map* map);
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
		void OnMove(Map* map);
		bool InAttackRange(int x, int y);

	private:
		int initX, initY;
		int counter;
		bool onJump, onDrop;
		int jumpTop;
		double jumpSpeed;
	};


	class Boss : public Enemy
	{
	public:
		Boss(int x, int y, int d);
		void LoadBitmap();
		void OnMove(Map* map);
		void OnAttack(int x, int y);
		void ShowWeapon();
		bool InHitBox(int x, int y);
		void showVanish();
		int getLoot2();
		void MoveWeapon(Map* map);
		bool InAttackRange(int x, int y);
		void Initialize();
		bool InWeaponHitBox(int x, int y);
	private:
		double unit;
		double unitl, unitr;
		double weaponMoveX, weaponMoveY, unit1;
		double weaponMove2X, weaponMove2Y, unit2;
		double weaponMove3X, weaponMove3Y, unit3;
		int weaponPositionX, weaponPositionY;
		int weaponPosition2X, weaponPosition2Y;
		int weaponPosition3X, weaponPosition3Y;

		double directX, directY;
		double dx, dy;
		double dxl, dxr;
		double direct1X, direct1Y;
		double direct2X, direct2Y;
		double direct3X, direct3Y;
		bool onAttack2, onAttack3;
		double speedx;
		double speedy;
		double acc;
		int counthit;
		int weaponState;
		int countRolling;
		int heroX, heroY;
		int countY, countX;
		bool attackSide, hit;
		int count;
		int weapon1X, weapon2X, weapon3X;
		int weapon1Y, weapon2Y, weapon3Y;
		int mode;
	};
}
#endif


