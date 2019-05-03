/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include <fstream>
#include <sstream>
#include <vector>

namespace game_framework {

	const int MAPS_NUM = 17;
	const int MAPS_X = 5;
	const int MAPS_Y = 6;
	const int MAP_SIZE_X = 18;
	const int MAP_SIZE_Y = 13;


	MapBrown::MapBrown() :X(0), Y(0), MV(32), MH(32)
	{

		mapPosX = 0;
		mapPosY = 4;
		mapObjects.push_back(new Blank());
		mapObjects.push_back(new Grass());
		mapObjects.push_back(new Bone());
		mapObjects.push_back(new Tomb());
		mapObjects.push_back(new Shop());
		mapObjects.push_back(new Block());
		mapObjects.push_back(new WeakBlock());
		mapObjects.push_back(new LadderBlock());
		mapObjects.push_back(new Ladder());
		mapObjects.push_back(new Door());
		mapObjects.push_back(new Spike());
		mapObjects.push_back(new Lever());

		/*fstream mapsfile;
		mapsfile.open("./data/map/maps.txt");
		for (int i = 0; i < mapsY; i++) {
			for (int j = 0; j < mapsX; j++) {
				mapsfile >> mapOfMap[i][j];
			}
		}
		mapsfile.close();*/
		int maps[MAPS_Y][MAPS_X] = {
			{99,99,99,13,99},
			{99,99,99,11,12},
			{99,8,9,10,99},
			{4,5,6,99,99},
			{0,1,2,3,99},
			{7,16,15,14,99},
		};

		for (int i = 0; i < MAPS_Y; i++) {
			for (int j = 0; j < MAPS_X; j++) {
				mapOfMap[i][j] = maps[i][j];
			}
		}

		fstream file;

		for (int num = 0; num < MAPS_NUM; num++)
		{
			std::stringstream s;
			s << num;
			string fileName = "./data/map/map_" + s.str() + ".txt";
			file.open(fileName, ios::in);
			if (!file)
			{
				throw "file not exist";
			}
			vector<vector<int>> mapAry;
			for (int i = 0; i < MAP_SIZE_Y; i++)
			{
				vector<int> mapColumn;
				for (int j = 0; j < MAP_SIZE_X; j++)
				{
					int tempBlock;
					file >> tempBlock;
					mapColumn.push_back(tempBlock);
				}
				mapAry.push_back(mapColumn);
				mapColumn.clear();
			}

			mapList.push_back(mapAry);
			mapAry.clear();

			file.close();

		}

		next = mapOfMap[mapPosY][mapPosX];

		for (int i = 0; i < MAP_SIZE_Y; i++)
		{
			for (int j = 0; j < MAP_SIZE_X; j++)
			{
				map[i][j] = mapList[next][i][j];
			}
		}
	}

	int MapBrown::getNext()
	{
		return next;
	}

	MapBrown ::~MapBrown()
	{
		for (unsigned int i = 0; i < mapObjects.size(); ++i)
		{
			delete mapObjects[i];
		}
	}

	void MapBrown::UpdateMap(char nextPos)
	{
		if (nextPos == 'L')
		{
			mapPosX -= 1;
		}
		else if (nextPos == 'R')
		{
			mapPosX += 1;
		}
		else if (nextPos == 'U')
		{
			mapPosY -= 1;
		}
		else if (nextPos == 'D')
		{
			mapPosY += 1;
		}
		else
		{
			throw "update map error";
		}
		if (mapPosY >= MAPS_Y || mapPosX >= MAPS_X)
		{
			next = 7;
		}
		else
		{
			next = mapOfMap[mapPosY][mapPosX];
		}
		if (next == 99)
		{
			next = 7;
		}
		for (int i = 0; i < 13; i++)
		{
			for (int j = 0; j < 18; j++)
			{
				map[i][j] = mapList[next][i][j];
			}
		}
	}

	int MapBrown::GetBlock(int x, int y)
	{
		if (x >= 18 || y >= 13)
		{
			return 0;
		}

		int block = map[y][x];
		return (block / 100);
	}

	bool MapBrown::isBlockSolid(int i, int j)
	{
		if (i >= 18 || j >= 13)
		{
			return 0;
		}

		return mapObjects[GetBlock(i, j)]->IsSolid();
	}

	MapObject* MapBrown::getMapObject(int i)
	{
		return mapObjects[i];
	}

	void MapBrown::LoadBitMap()
	{
		for (unsigned int i = 0; i < mapObjects.size(); ++i)
		{
			mapObjects[i]->LoadBitMap();
		}
	}

	void MapBrown::OnShow()
	{
		for (int i = 0; i < 13; ++i) {
			for (int j = 0; j < 18; ++j) {
				mapObjects[GetBlock(j, i)]->PutBlock(X + (MV*j), Y + (MH*i), map[i][j]);
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	/////////////////////////////////////////////////////////////////////////////

	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateInit::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(0);	// 一開始的loading進度為0%
		//
		// 開始載入資料
		//
		logo.LoadBitmap(TITLE_INIT);
		//Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateInit::OnBeginState()
	{
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		if (nChar == KEY_SPACE)
			GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
		else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		//GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}

	void CGameStateInit::OnShow()
	{
		//
		// 貼上logo
		//
		logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
		logo.ShowBitmap();
		//
		// Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
		//
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		pDC->TextOut(185, 230, "Please press SPACE to begin.");
		pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");
		if (ENABLE_GAME_PAUSE)
			pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");
		pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	/////////////////////////////////////////////////////////////////////////////

	CGameStateOver::CGameStateOver(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateOver::OnMove()
	{
		counter--;
		if (counter < 0)
			GotoGameState(GAME_STATE_INIT);
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
	}

	void CGameStateOver::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
		//
		// 開始載入資料
		//
		//Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 最終進度為100%
		//
		ShowInitProgress(100);
	}

	void CGameStateOver::OnShow()
	{
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char str[80];								// Demo 數字對字串的轉換
		sprintf(str, "Game Over ! (%d)", counter / 30);
		pDC->TextOut(240, 210, str);
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	/////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame *g)
		: CGameState(g)
	{
		//ball = new CBall [NUMBALLS];
	}

	CGameStateRun::~CGameStateRun()
	{
		//delete [] ball;
	}

	void CGameStateRun::OnBeginState()
	{
		const int BALL_GAP = 90;
		const int BALL_XY_OFFSET = 45;
		const int BALL_PER_ROW = 7;
		const int HITS_LEFT = 10;
		const int HITS_LEFT_X = 590;
		const int HITS_LEFT_Y = 0;
		const int BACKGROUND_X = 60;
		const int ANIMATION_SPEED = 15;

		hero.Initialize();
		enemies.push_back(new Skull(13 * 32, 7 * 32, 0));

		//enemies.push_back(new Snake(1 * 32, 6 * 32, 0));
		//enemies.push_back(new Snake(13 * 32, 7 * 32, 0));

		//enemies.push_back(new Frog(12 * 32, 7 * 32, 0));
		//enemies.push_back(new Slime(12 * 32, 9 * 32, 0));
		//enemies.push_back(new Bat(0 * 32, 1 * 32, 0));
		//enemies.push_back(new Bat(12 * 32, 7 * 32, 0));
		//enemies.push_back(new BowHead(10 * 32, 7 * 32, 0));

		//enemies.push_back(new BowHead(2 * 32, 2 * 32, 7));
		//enemies.push_back(new BowHead(2 * 32, 2 * 32, 7));
		//enemies.push_back(new BowHead(2 * 32, 2 * 32, 7));
		//enemies.push_back(new BowHead(2 * 32, 2 * 32, 7));

		//enemies.push_back(new BowHead(16 * 32, 2 * 32, 7));
		//enemies.push_back(new BowHead(16 * 32, 2 * 32, 7));
		//enemies.push_back(new BowHead(16 * 32, 2 * 32, 7));
		//enemies.push_back(new BowHead(16 * 32, 2 * 32, 7));

		CAudio::Instance()->Play(AUDIO_BGM, false);		// 撥放 WAVE

	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		//
		// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
		//
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		//
		// 移動背景圖的座標
		//
		/*if (background.Top() > SIZE_Y)
			background.SetTopLeft(60 ,-background.Height());
		background.SetTopLeft(background.Left(),background.Top()+1);*/

		hero.OnMove(&mapBrown);


		if (hero.isOnAttack())
		{
			for (unsigned int i = 0; i < enemies.size(); ++i)
			{
				if (mapBrown.getNext() == enemies[i]->getMapLocation() && !enemies[i]->IsDistroyed())
				{
					if (hero.InAttackRange(enemies[i]->GetX1(), enemies[i]->GetY1()))
					{
						hero.setHitValid(false);
						enemies[i]->hit(hero.getDamage());
						enemies[i]->Distroy();
					}
				}
			}
		}

		for (unsigned int i = 0; i < enemies.size(); ++i)
		{
			if (mapBrown.getNext() == enemies[i]->getMapLocation() && !enemies[i]->IsDistroyed()) 
			{
				enemies[i]->OnMove(&mapBrown);
				BowHead* bowHead = dynamic_cast<BowHead*>(enemies[i]);
				if (bowHead != nullptr)
				{
					bowHead->saveHeroPos(hero.GetX1(), hero.GetY1());
				}
				enemies[i]->MoveWeapon(&mapBrown);
			}
		}


		for (unsigned int i = 0; i < enemies.size(); ++i)
		{
			if (mapBrown.getNext() == enemies[i]->getMapLocation() && !enemies[i]->IsDistroyed())
			{
				if (!hero.BeatBack() && enemies[i]->InHitBox(hero.GetX1(), hero.GetY1()) && mapBrown.getNext() == enemies[i]->getMapLocation())
				{
					if (hero.GetX1() < enemies[i]->GetX1())
					{
						hero.BeatBack(true, -1);
					}
					else
					{
						hero.BeatBack(true, 1);
					}
				}
			}
		}

		for (unsigned int i = 0; i < enemies.size(); ++i)
		{
			if (mapBrown.getNext() == enemies[i]->getMapLocation() && !enemies[i]->IsDistroyed())
			{
				if (enemies[i]->InAttackRange(hero.GetX1(), hero.GetY1()) && mapBrown.getNext() == enemies[i]->getMapLocation())
				{
					enemies[i]->OnAttack(hero.GetX1(), hero.GetY1());
				}
			}
		}

		for (unsigned int i = 0; i < enemies.size(); ++i)
		{
			if (mapBrown.getNext() == enemies[i]->getMapLocation() && !enemies[i]->IsDistroyed())
			{
				if (!hero.BeatBack() && enemies[i]->InWeaponHitBox(hero.GetX1(), hero.GetY1()) && mapBrown.getNext() == enemies[i]->getMapLocation())
				{
					if (hero.GetX1() < enemies[i]->GetWeaponX1())
					{
						hero.BeatBack(true, -1);
					}
					else
					{
						hero.BeatBack(true, 1);
					}
				}
			}
		}

	}

	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		//ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
		//
		// 開始載入資料
		//

		hero.LoadBitmap();
		mapBrown.LoadBitMap();
		//
		// 完成部分Loading動作，提高進度
		//
		//ShowInitProgress(50);
		Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 繼續載入其他資料
		//

		CAudio::Instance()->Load(AUDIO_BGM, "Sounds\\KYOUDANB.MID");	// 載入編號1的聲音lake.mp3
		//
		// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char KEY_SPACE = 0x20;
		if (nChar == KEY_LEFT) {
			hero.SetMovingLeft(true);
		}
		if (nChar == KEY_RIGHT) {
			hero.SetMovingRight(true);
		}
		if (nChar == KEY_UP) {
			hero.SetMovingUp(true);
		}
		if (nChar == KEY_DOWN) {
			hero.SetMovingDown(true);
		}
		if (nChar == KEY_SPACE) {
			hero.SetAttack(true);
		}
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		if (nChar == KEY_LEFT) {
			hero.SetMovingLeft(false);
		}
		if (nChar == KEY_RIGHT) {
			hero.SetMovingRight(false);
		}
		if (nChar == KEY_UP) {
			hero.SetHoldUp(false);
			hero.SetMovingUp(false);
		}
		if (nChar == KEY_DOWN) {
			hero.SetMovingDown(false);
		}
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		//eraser.SetMovingLeft(true);
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		//eraser.SetMovingLeft(false);
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		//eraser.SetMovingRight(true);
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		//eraser.SetMovingRight(false);
	}

	void CGameStateRun::OnShow()
	{
		//
		//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
		//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
		//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
		//
		//
		//  貼上背景圖、撞擊數、球、擦子、彈跳的球
		//
		mapBrown.OnShow();

		for (unsigned int i = 0; i < enemies.size(); ++i)
		{
			if (mapBrown.getNext() == enemies[i]->getMapLocation() && !enemies[i]->IsDistroyed())
			{
				enemies[i]->OnShow();
				enemies[i]->ShowWeapon();
			}
		}
		hero.OnShow();
	}
}