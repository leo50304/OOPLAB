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
#include "thunder.h"

namespace game_framework {

	Thunder::Thunder()
	{
		srand((unsigned int)time(NULL));
	}

	void Thunder::LoadBitmap() 
	{
		for (int i = 0; i < 5; ++i)
		{
			thunder[i] = CMovingBitmap();
		}
		thunder[0].LoadBitmap(attack_thunder_H_0, RGB(128, 0, 128));
		thunder[1].LoadBitmap(attack_thunder_H_1, RGB(128, 0, 128));
		thunder[2].LoadBitmap(attack_thunder_H_2, RGB(128, 0, 128));
		thunder[3].LoadBitmap(attack_thunder_H_3, RGB(128, 0, 128));
		thunder[4].LoadBitmap(attack_thunder_H_5, RGB(128, 0, 128));

		vThunderHead.LoadBitmap(attack_thunder_V_0, RGB(128, 0, 128));
		vThunder.AddBitmap(attack_thunder_V_1, RGB(128, 0, 128));
		vThunder.AddBitmap(attack_thunder_V_2, RGB(128, 0, 128));
		vThunder.AddBitmap(attack_thunder_V_3, RGB(128, 0, 128));

		onEnemyThunder.AddBitmap(attack_thunder_hit_1, RGB(128, 0, 128));
		onEnemyThunder.AddBitmap(attack_thunder_hit_2, RGB(128, 0, 128));
	}

	void Thunder::initialize()
	{
		initState();
	}

	void Thunder::stop() 
	{
		finish = true;
	}

	void Thunder::initState() 
	{
		thunders.clear();
		vThunders.clear();
		onPrepare = true;
		onAttack = false;
		onDone = false;
		finish = false;
		attackCount = 0;
		prepareCount = 0;
		thunder_s t;
		t.x = rand() % 13;
		t.length = 1;
		t.growth = true;
		frameCount = 0;
	}

	void Thunder::OnShow()
	{
		if (finish)
		{
			return;
		}
		for (unsigned int i = 0; i < thunders.size(); ++i)
		{
			if (thunders[i].length <= 0)
			{
				continue;
			}
			int posx = thunders[i].x * 32;
			thunder[0].SetTopLeft(posx, 0);
			thunder[0].ShowBitmap();
			for (int j = 1; j < thunders[i].length && j < 4; ++j)
			{
				thunder[thunders[i].type[j]].SetTopLeft(posx + j * 32, 0);
				thunder[thunders[i].type[j]].ShowBitmap();
			}
			if (thunders[i].length >= 5)
			{
				thunder[4].SetTopLeft(posx + 32 * 4, 0);
				thunder[4].ShowBitmap();
			}
		}

		if (onDrop && !onDone)
		{
			vThunder.OnMove();
			for (unsigned int i = 0; i < vThunders.size(); ++i)
			{
				for (int j = 1; j < vThunders[i].length; ++j)
				{
					vThunder.SetTopLeft(vThunders[i].x, j * 32);
					vThunder.OnShow();
				}
				onEnemyThunder.SetTopLeft(vThunders[i].x, vThunders[i].length * 32);
				onEnemyThunder.OnShow();
			}
		}
	}

	void Thunder::OnPrepare()
	{
		onPrepare = true;
	}

	void Thunder::OnAttack()
	{
		onAttack = true;
	}

	bool Thunder::InHitBox()
	{
		return false;
	}

	void Thunder::OnMove(vector<Enemy*> &enemies, int d)
	{
		if (finish)
		{
			return;
		}
		if (attackCount > 4)
		{
			onDone = true;
			onDrop = false;
			prepareCount = 0;
			for (unsigned int i = 0; i < enemies.size(); ++i)
			{
				enemies[i]->isThundered(false);
			}
		}
		if (prepareCount > 150 && !onDrop)
		{
			onDrop = true;
			attackCount = 0;
		}
		if (true)
		{
			if (!onDone)
			{
				prepareCount++;
			}
			if (frameCount < 9)
			{
				frameCount++;
				return;
			}
			else
			{
				frameCount = 0;
			}
			if (!onDone) //增加閃電
			{
				int randAdd = rand() % 3;
				if (randAdd > 0)
				{
					thunder_s t;
					t.x = rand() % 13;
					t.length = 1;
					t.growth = true;
					thunders.push_back(t);
				}
				if (randAdd == 2)
				{
					thunder_s t;
					t.x = rand() % 13;
					t.length = 1;
					t.growth = true;
					thunders.push_back(t);
				}
			}
			else
			{
				if (thunders.size() == 0)
				{
					finish = true;
				}
				else
				{
					int DecPos = rand() % thunders.size();
					thunders.erase(thunders.begin() + DecPos);
				}
			}

			for (unsigned int i = 0; i < thunders.size(); ++i)
			{
				for (int j = 1; j < 4; ++j)
				{
					thunders[i].type[j] = rand() % 3 + 1;
				}

				if (thunders[i].length >= 8)
				{
					thunders[i].length = 5;
					thunders[i].growth = false;
				}
				if (thunders[i].growth)
				{
					thunders[i].length += 1;
				}
				else
				{
					thunders[i].length -= 1;
				}
			}
		}
		if (onDrop && !onDone)
		{
			onEnemyThunder.OnMove();
			attackCount++;
			bool hasDrop = false;
			for (unsigned int i = 0; i < enemies.size(); ++i)
			{
				if (d == enemies[i]->getMapLocation() && enemies[i]->GetY1() < 32 * 13 && !enemies[i]->IsDistroyed())
				{
					if (enemies[i]->isThundered()) 
					{
						continue;
					}
					hasDrop = true;
					thunder_s t;
					t.x = enemies[i]->GetX1();
					t.length = enemies[i]->GetY1() / 32;
					t.growth = false;
					vThunders.push_back(t);
					enemies[i]->hit(damage);
					enemies[i]->Distroy();
					enemies[i]->isThundered(true);
				}
			}
			if (hasDrop) 
			{
				CAudio::Instance()->Play(THUNDER, false);		// 撥放 WAVE
			}
		}
	}
}