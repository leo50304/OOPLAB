#ifndef CHERO_H
#define CHERO_H

#include "stdafx.h"
#include "map.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供可以用鍵盤或滑鼠控制的擦子
	// 看懂就可以改寫成自己的程式了
	/////////////////////////////////////////////////////////////////////////////

	class CHero
	{
	public:
		CHero();
		int  GetX1();
		int  GetY1();
		void Initialize();
		void LoadBitmap();
		void OnMove(MapBrown* map);
		void OnShow(); 
		void SetMovingDown(bool flag);
		void SetMovingLeft(bool flag);
		void SetMovingRight(bool flag);
		void SetMovingUp(bool flag);
		void SetXY(int nx, int ny);
		bool IsOnLadder();
		void Drop();
		void SetOnLadder(bool flag);
		void SetAttack(bool flag);
		bool InAttackRange(int x, int y);
		void SetHoldUp(bool flag);
		bool isOnAttack();
		void BeatBack(bool flag, int direct);
		bool BeatBack();
		void setHitValid(bool flag);
		int getDamage();

	protected:
		CMovingBitmap standL;
		CMovingBitmap standR;
		CMovingBitmap stayOnLadder;
		CMovingBitmap jumpL;
		CMovingBitmap jumpR;
		CMovingBitmap swordL;
		CMovingBitmap swordR;
		CMovingBitmap attackL;
		CMovingBitmap attackR;

		bool hitValid = false;
		double speed;
		bool onAttack;
		CAnimation moveRAnimation;
		CAnimation moveLAnimation;
		CAnimation moveUAnimation;
		CAnimation moveDAnimation;
		bool beatBack = false;
		double direction;
		double beatBackAy;
		double beatBackAx;
		double beatBackXSpeed;
		double beatBackYSpeed;
		int attackFrameCount = 0;
		int invincibleFrameCount = 0;
		
		int damage = 1;

		int faceSide = 1;
		int x, y;					// 擦子左上角座標
		bool onDrop = false;
		bool onJump = false;
		bool onHold = false;
		int TopLimit;
		int jumpTop;
		int previousBlock;
		bool isOnLadder;
		bool isOnLadderSide;
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動

		bool HitGround(MapBrown* map);
		bool HitTop(MapBrown* map);
		bool GroundNotSolid(MapBrown* map);
	};
}
#endif // !CHERO_H
