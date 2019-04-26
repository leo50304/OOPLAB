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
		void LoadBitmap();				// ���J�ϧ�
		virtual void OnMove(MapBrown* map);     // ����
		virtual void MoveWeapon(MapBrown* map);
		virtual void OnAttack();
		virtual void ShowWeapon();
		//bool HitHeroAction(int type, string pos);
		void OnShow();      			// �N���l�ϧζK��e��
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void SetXY(int nx, int ny);		// �]�w���l���W���y��
		void Distroy();
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
		CAnimation moveRAnimation;
		CAnimation moveLAnimation;
		CAnimation weapon;
		int x, y;					// ���l���W���y��
		bool onDrop = false;
		bool isDistroyed = false;
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
	};

	class Frog : public Enemy
	{
	public:
		Frog(int x, int y, int d);
	};

	class Slime : public Enemy
	{
	public:
		Slime(int x, int y, int d);
		void LoadBitmap();
		void OnAttack();
		void OnMove(MapBrown* map);     // ����
		void ShowWeapon();
		void MoveWeapon(MapBrown* map);

	};
	

}
#endif // !Enemy_H
