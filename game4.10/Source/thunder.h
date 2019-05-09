#ifndef THUNDER_H
#define THUNDER_H

#include "stdafx.h"
#include "Enemy.h"
#include <vector>

namespace game_framework {

	class Thunder
	{
	public:
		Thunder();
		void initialize();
		void initState();
		void OnShow();
		bool InHitBox();
		void OnMove(vector<Enemy*> &enemies, int d);
		void OnPrepare();
		void OnAttack();
		struct thunder_s {
			int type[5];
			bool growth;
			int length;
			int x;
		};

	private:
		vector<thunder_s> thunders;
		CMovingBitmap thunder[5];

		vector<thunder_s> vThunders;
		CMovingBitmap vThunderHead;
		CAnimation vThunder;
		CAnimation onEnemyThunder;

		int finish;
		int attackCount = 0;
		int frameCount;
		int prepareCount;
		int randAttack;
		bool onPrepare = false;
		bool onDrop = false;
		bool onDone = false;
		bool onAttack;
		bool change = true;
	};
}

#endif