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
		void LoadBitmap();				// 載入圖形
		virtual void OnMove(MapBrown* map);     // 移動
		virtual void MoveWeapon(MapBrown* map);
		virtual void OnAttack(int x, int y);
		virtual void ShowWeapon();
		//bool HitHeroAction(int type, string pos);
		void OnShow();      			// 將擦子圖形貼到畫面
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		void SetXY(int nx, int ny);		// 設定擦子左上角座標
		void Distroy();
		void hit(int d);
		virtual bool InAttackRange(int x, int y);
		virtual bool InWeaponHitBox(int x, int y);
		int GetWeaponX1();
		virtual bool InHitBox(int x, int y);
		bool IsDistroyed();
		int getMapLocation()
		{
			return location;
		}

	protected:
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
		void OnMove(MapBrown* map);     // 移動
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
		void OnMove(MapBrown* map);     // 移動
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
}
#endif
