#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "map.h"
#include "CHero.h"
#include "item.h"

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
		if (RemainingUse == -1)
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
		pDC->TextOut(X1+36, Y1+10, Levelbuffer);
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	void Item::ShowIcon()
	{
		icon.SetTopLeft(X1+ offSetX, Y1+ offSetY);
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
}