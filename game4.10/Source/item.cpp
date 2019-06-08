#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "map.h"
#include "CHero.h"
#include "item.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>       /* sqrt */

namespace game_framework {

	Item::Item(int x, int y, int d)
	{
		X1 = x;
		Y1 = y;
		map = d;
	}

	Item::Item()
	{
		X1 = -100;
		Y1 = -100;
		map = -1;
		resetOffset();
	}

	void Item::OnMove(MapBrown* map)
	{
		if (!onGround)
		{
			int x = X1;
			int y = Y1;
			speedY += 0.892*0.5;
			if (speedY > 16)
			{
				speedY = 16;
			}
			Y1 += (int)speedY;

			if ((map->isBlockSolid(x / 32, (y-offSetY + 32) / 32) && !map->isBlockSolid(x / 32, (y - offSetY + 1) / 32)) || (map->isBlockSolid((x + 31) / 32, (y - offSetY + 32) / 32) && !map->isBlockSolid((x + 31) / 32, (y - offSetY + 1) / 32)))
			{
				Y1 = (Y1 / 32) * 32;
				onGround = true;
			}
		}	
	}


	bool Item::pickUp(CHero* hero)
	{
		return true;
	}


	void Item::OnSelect(CHero* hero)
	{
		return;
	}

	void Item::RemoveSelect(CHero* hero)
	{
		return;
	}

	bool Item::InHitBox(int x, int y)
	{
		return  x < X1 + 22 && x + 22 > X1 && y < Y1 + 28 && y + 28 > Y1;
	}

	int Item::getMap()
	{
		return map;
	}

	void Item::MoveIcon()
	{
		icon.OnMove();
	}

	void Item::resetOffset()
	{
		offSetX = 0;
		offSetY = 0;
	}

	void Item::setXY(int x, int y)
	{
		X1 = x;
		Y1 = y;
	}

	void Item::ShowRemainingUse()
	{
		if (RemainingUse <= -1)
		{
			return;
		}
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, *fp;
		f.CreatePointFont(120, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char Levelbuffer[4];
		snprintf(Levelbuffer, 10, "%d", RemainingUse);
		pDC->TextOut(X1 + 36, Y1 + 10, Levelbuffer);
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	void Item::ShowIcon()
	{
		icon.SetTopLeft(X1 + offSetX, Y1 + offSetY);
		icon.OnShow();
		ShowRemainingUse();
	}

	SmallBlood::SmallBlood(int x, int y, int d) :Item(x, y, d)
	{
		LoadBitMap();
		offSetX = 0;
		offSetY = 7;

	}

	SmallBlood::SmallBlood() : Item()
	{
		LoadBitMap();
	}

	void SmallBlood::use(CHero* hero)
	{
		hero->addHp(25);
	}

	void SmallBlood::LoadBitMap()
	{
		icon.AddBitmap(SMALL_BLOOD_1, RGB(128, 0, 128));
		icon.AddBitmap(SMALL_BLOOD_2, RGB(128, 0, 128));
		icon.AddBitmap(SMALL_BLOOD_3, RGB(128, 0, 128));
	}

	bool SmallBlood::InHitBox(int x, int y)
	{
		return  x < X1 + 22 && x + 22 > X1 && y < Y1 + 28 && y + 28 > Y1;
	}

	BigBlood::BigBlood(int x, int y, int d) :Item(x, y, d)
	{
		LoadBitMap();
		offSetX = 0;
		offSetY = 7;

	}

	BigBlood::BigBlood() : Item()
	{
		LoadBitMap();
	}

	void BigBlood::use(CHero* hero)
	{
		hero->addHp(500);
	}

	void BigBlood::LoadBitMap()
	{
		icon.AddBitmap(BIG_BLOOD_0, RGB(128, 0, 128));
		icon.AddBitmap(BIG_BLOOD_1, RGB(128, 0, 128));
		icon.AddBitmap(BIG_BLOOD_2, RGB(128, 0, 128));
	}

	bool BigBlood::InHitBox(int x, int y)
	{
		return  x < X1 + 22 && x + 22 > X1 && y < Y1 + 28 && y + 28 > Y1;
	}

	Gold::Gold(int x, int y, int d) :Item(x, y, d)
	{
		srand((unsigned int)time(NULL));
		LoadBitMap();
		offSetX = 0;
		offSetY = 7;
	}

	Gold::Gold() : Item()
	{
		srand((unsigned int)time(NULL));
		LoadBitMap();
	}

	void Gold::LoadBitMap()
	{
		icon.AddBitmap(GOLD_0, RGB(128, 0, 128));
		icon.AddBitmap(GOLD_1, RGB(128, 0, 128));
		icon.AddBitmap(GOLD_2, RGB(128, 0, 128));
		icon.AddBitmap(GOLD_3, RGB(128, 0, 128));
	}

	bool Gold::InHitBox(int x, int y)
	{
		return  x < X1 + 25 && x + 25 > X1 && y < Y1 + 28 && y + 28 > Y1;
	}

	void Gold::use(CHero* hero)
	{
		amount = rand() % 10 + 1;
		hero->changeGold(amount);
	}

	bool Gold::pickUp(CHero* hero)
	{
		use(hero);
		return false;
	}


	Ring::Ring(int x, int y, int d) :Item(x, y, d)
	{
		LoadBitMap();
		offSetX = 8;
		offSetY = 8;
	}

	Ring::Ring() : Item()
	{
		LoadBitMap();
	}

	void Ring::LoadBitMap()
	{
		icon.AddBitmap(RING, RGB(128, 0, 128));
	}

	bool Ring::InHitBox(int x, int y)
	{
		return  x < X1 + 25 && x + 25 > X1 && y < Y1 + 28 && y + 28 > Y1;
	}

	void Ring::use(CHero* hero)
	{
		return;
	}

	void Ring::OnSelect(CHero * hero) 
	{
		hero->addMaxHp(10);
	}

	void Ring::RemoveSelect(CHero * hero)
	{
		hero->addMaxHp(-10);
	}

	LegendStone::LegendStone(int x, int y, int d) :Item(x, y, d)
	{
		LoadBitMap();
		offSetX = 8;
		offSetY = 8;
	}

	LegendStone::LegendStone() : Item()
	{
		LoadBitMap();
	}

	void LegendStone::LoadBitMap()
	{
		icon.AddBitmap(RING, RGB(128, 0, 128));
	}

	bool LegendStone::InHitBox(int x, int y)
	{
		return  x < X1 + 25 && x + 25 > X1 && y < Y1 + 28 && y + 28 > Y1;
	}

	void LegendStone::use(CHero* hero)
	{
		return;
	}

	void LegendStone::OnSelect(CHero * hero)
	{
		hero->setWin(true);
	}

	void LegendStone::RemoveSelect(CHero * hero)
	{
		hero->setWin(false);
	}

}