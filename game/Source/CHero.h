#ifndef CHERO_H
#define CHERO_H

#include "stdafx.h"
#include "map.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H����L�ηƹ�������l
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////

	class CHero
	{
	public:
		CHero();
		int  GetX1();					// �W x �y��
		int  GetY1();					// �W y �y��
		void Initialize();				// �]�w���l����l��
		void LoadBitmap();				// ���J�ϧ�
		void OnMove(MapBrown* map);     // ����
		//bool HitHeroAction(int type, string pos);
		void OnShow();      			// �N���l�ϧζK��e��
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void SetXY(int nx, int ny);		// �]�w���l���W���y��
		bool IsOnLadder();
		void Drop();
		void SetOnLadder(bool flag);
		void SetAttack(bool flag);
		void SetHoldUp(bool flag);
	protected:
		CMovingBitmap standL;		// ���l���ʵe
		CMovingBitmap standR;		// ���l���ʵe
		CMovingBitmap stayOnLadder;
		CMovingBitmap jumpL;
		CMovingBitmap jumpR;
		CMovingBitmap swordL;
		CMovingBitmap swordR;
		CMovingBitmap attackL;
		CMovingBitmap attackR;

		double speed;
		bool onAttack;
		CAnimation moveRAnimation;
		CAnimation moveLAnimation;
		CAnimation moveUAnimation;
		CAnimation moveDAnimation;
		

		int attackFrameCount = 0;
		int faceSide = 1;
		int x, y;					// ���l���W���y��
		bool onDrop = false;
		bool onJump = false;
		bool onHold = false;
		int TopLimit;
		int jumpTop;
		int previousBlock;
		bool isOnLadder;
		bool isOnLadderSide;
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
	};
}
#endif // !CHERO_H
