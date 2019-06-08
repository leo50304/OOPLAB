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
	bool isWin;
	bool onStart;
	clock_t t1, t2;

	MapBrown::MapBrown() :X(0), Y(0), MV(32), MH(32)
	{
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
		initMap();

	}

	void MapBrown::initMap() 
	{

		mapPosX = 0;
		mapPosY = 4;
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
		CAudio::Instance()->Load(TITLE_BGM, "Sounds\\TSUKINOS.MID");	// 載入編號1的聲音lake.mp3
		CAudio::Instance()->Load(JUMP_EF, "data\\se\\sb_00.wav");	// 載入編號1的聲音lake.mp3
		CAudio::Instance()->Load(DROP_EF, "data\\se\\sb_01.wav");	// 載入編號1的聲音lake.mp3
		CAudio::Instance()->Load(ATK_EF, "data\\se\\sb_01.wav");	// 載入編號1的聲音lake.mp3
		CAudio::Instance()->Load(FIRE_ATK_EF, "data\\se\\sb_01.wav");	// 載入編號1的聲音lake.mp3
		CAudio::Instance()->Load(SLIME_ATK_EF, "data\\se\\sb_01.wav");	// 載入編號1的聲音lake.mp3
		CAudio::Instance()->Load(HIT_HERO, "data\\se\\sb_01.wav");	// 載入編號1的聲音lake.mp3

		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(0);	// 一開始的loading進度為0%
		//
		// 開始載入資料
		//
		currentSelect = 0;
		titleOffset = 32;
		menuState = 0;
		countFlash = 60;
		titleFrame.LoadBitmap(TITLE_FRAME, RGB(128, 0, 128));
		titleNew.LoadBitmap(TITLE_NEW, RGB(128, 0, 128));
		titleHelp.LoadBitmap(TITLE_HELP, RGB(128, 0, 128));
		titleLeaderboard.LoadBitmap(TITLE_LEADERBOARD, RGB(128, 0, 128));
		titleSetting.LoadBitmap(TITLE_SETTING, RGB(128, 0, 128));
		titleExit.LoadBitmap(TITLE_EXIT, RGB(128, 0, 128));
		logo.LoadBitmap(MENU_0, RGB(128, 0, 128));
		logoBackground.LoadBitmap(MENU_1, RGB(128, 0, 128));
		CAudio::Instance()->Play(TITLE_BGM, false);		// 撥放 WAVE

		//Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
		//

	}

	void CGameStateInit::OnBeginState()
	{
		onStart = true;
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_DOWN = 0x28; // keyboard上箭頭

		if (menuState == 1 && nChar == KEY_SPACE)
		{
			menuState = 0;
			return;
		}

		if (nChar == KEY_SPACE)
		{
			if (currentSelect == 0)
			{
				CAudio::Instance()->Stop(TITLE_BGM);
				t1 = clock();
				onStart = false;
				GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
			}
			else if (currentSelect == 1)
			{
				menuState = 1;
			}
			else if (currentSelect == 2)
			{
				isWin = true;
				GotoGameState(GAME_STATE_OVER);	
			}
			else if (currentSelect == 4)
			{
				CAudio::Instance()->Stop(TITLE_BGM);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		else if (nChar == KEY_UP)
		{
			currentSelect--;
			if (currentSelect < 0)
			{
				currentSelect = 4;
			}
		}
		else if (nChar == KEY_DOWN)
		{
			currentSelect++;
			if (currentSelect > 4)
			{
				currentSelect = 0;
			}
		}
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

		logoBackground.SetTopLeft(320, 240);
		logoBackground.ShowBitmap();
		logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
		logo.ShowBitmap();

		if (menuState == 0)
		{
			titleFrame.SetTopLeft(270 - 32, 245 + titleOffset * currentSelect);
			titleFrame.ShowBitmap();
			titleNew.SetTopLeft(270, 240);
			titleNew.ShowBitmap();
			titleHelp.SetTopLeft(270, 240 + titleOffset * 1);
			titleHelp.ShowBitmap();
			titleLeaderboard.SetTopLeft(270, 240 + titleOffset * 2);
			titleLeaderboard.ShowBitmap();
			titleSetting.SetTopLeft(270, 240 + titleOffset * 3);
			titleSetting.ShowBitmap();
			titleExit.SetTopLeft(270, 240 + titleOffset * 4);
			titleExit.ShowBitmap();
		}
		else if (menuState == 1)
		{
			countFlash--;
			if (countFlash <= 0)
			{
				countFlash = 60;
			}
			CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
			CFont f, *fp;
			f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
			fp = pDC->SelectObject(&f);					// 選用 font f
			pDC->SetBkColor(RGB(0, 0, 0));
			pDC->SetTextColor(RGB(255, 200, 69));
			pDC->TextOut(155, 230, "打倒魔王取得傳說之石，逃脫洞窟");
			pDC->TextOut(125, 20+255, "E:           使用道具          CTRL+上下: 選擇道具");
			pDC->TextOut(125, 20+280, "SPACE: 攻擊                   上: 跳躍 / 爬梯子");
			pDC->TextOut(125, 20+305, "F:           火球                   下: 爬梯子");
			pDC->TextOut(125, 20+330, "T:           雷怒九天           左右: 移動");
			if (countFlash > 30) 
			{
				pDC->TextOut(237, 420, "空白鍵返回目錄");
			}


			pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
			CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
		}
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
		countFlash--;
		if (countFlash <= 0)
		{
			countFlash = 60;
		}
		counter--;
		if (counter < 0)
			GotoGameState(GAME_STATE_INIT);
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 600;
		t2 = clock();
		if (isWin)
		{
			fstream file;
			string fileName = "./data/leaderboard.txt";
			file.open(fileName, ios::in);
			if (!file)
			{
				throw "file not exist";
			}
			for (int i = 0; i < 6; ++i)
			{
				int time;
				file >> time;
				scores.push_back(time);
			}
			file.close();

			state = 1;
			if (!onStart) 
			{
				int t = (int)((t2 - t1) / (double)(CLOCKS_PER_SEC));
				scorePos = -1;
				for (int i = 0; i < 6; ++i)
				{
					if (t < scores[i])
					{
						scores.insert(scores.begin() + i, t);
						scorePos = i;
						countFlash = 60;
						break;
					}
				}
				scores.erase(scores.begin() + 6, scores.end());
				ofstream myfile("./data/leaderboard.txt");
				if (myfile.is_open())
				{
					for (int i = 0; i < 6; ++i)
					{
						myfile << scores[i] << '\n';
					}
					myfile.close();
				}
			}
		}
		else
		{
			state = 0;
		}
	}

	void CGameStateOver::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		//ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
		//
		// 開始載入資料
		//
		//Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 最終進度為100%
		//
		//ShowInitProgress(100);
		highScore.LoadBitmap(HIGH_SCORE, RGB(106, 3, 12));
		highScoreFrame.LoadBitmap(HIGH_SCORE_FRAME, RGB(130, 0, 128));
		leaderboardBg.LoadBitmap(LEADER_BOARD_BG);
	}

	void CGameStateOver::OnShow()
	{
		if (state == 1)
		{
			leaderboardBg.SetTopLeft(0, 0);
			leaderboardBg.ShowBitmap();
			highScoreFrame.SetTopLeft(0, 0);
			highScoreFrame.ShowBitmap();

			highScore.SetTopLeft(147, 32);
			highScore.ShowBitmap();

			CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
			CFont f, *fp;
			f.CreatePointFont(280, "Times New Roman");	// 產生 font f; 160表示16 point的字
			fp = pDC->SelectObject(&f);					// 選用 font f
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255, 200, 69));

			if (!(scorePos == 0 && countFlash < 30))
			{
				pDC->TextOut(32 * 3, 5 + 32 * 4 + 0, "ＴＯＰ");
			}
			if (!(scorePos == 1 && countFlash < 30))
			{
				pDC->TextOut(32 * 3, 5 + 32 * 5 + 22, "２ｎｄ");
			}
			if (!(scorePos == 2 && countFlash < 30))
			{
				pDC->TextOut(32 * 3, 5 + 32 * 6 + 44, "３ｒｄ");
			}
			if (!(scorePos == 3 && countFlash < 30))
			{
				pDC->TextOut(32 * 3, 5 + 32 * 7 + 66, "４ｔｈ");
			}
			if (!(scorePos == 4 && countFlash < 30))
			{
				pDC->TextOut(32 * 3, 5 + 32 * 8 + 88, "５ｔｈ");
			}
			if (!(scorePos == 5 && countFlash < 30))
			{
				pDC->TextOut(32 * 3, 5 + 32 * 9 + 110, "６ｔｈ");
			}

			for (int i = 0; i < 6; ++i)
			{
				char timeStr[80];
				int t = scores[i];
				sprintf(timeStr, "%d : %d", int(t / 60), t % 60);
				if (!(scorePos == i && countFlash < 30))
				{
					pDC->TextOut(32 * 8, 5 + 32 * (4 + i) + 22 * i, timeStr);
				}
			}

			pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
			CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC

			return;
		}
		else if (state == 0)
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
		for (unsigned int i = 0; i < enemies.size(); ++i)
		{
			delete enemies[i];
		}
		for (unsigned int i = 0; i < items.size(); ++i)
		{
			delete items[i];
		}
	}

	void CGameStateRun::SetCTRL(bool flag)
	{
		onCTRL = flag;
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

		mapBrown.initMap();
		hero.Initialize();
		thunder.initialize();

		for (unsigned int i = 0; i < items.size(); ++i)
		{
			delete items[i];
		}
		items.clear();
		items.push_back(new SmallBlood(32 * 4, 32 * 11, 0));
		items.push_back(new Gold(32 * 5, 32 * 11, 0));

		hero.addItem(new SmallBlood());

		for (unsigned int i = 0; i < enemies.size(); ++i)
		{
			delete enemies[i];
		}
		enemies.clear();
		enemies.push_back(new Slime(12 * 32, 9 * 32, 0));
		enemies.push_back(new Slime(1 * 32, 1 * 32, 0));
		enemies.push_back(new Slime(2 * 32, 8 * 32, 0));
		enemies.push_back(new BowHead(10 * 32, 7 * 32, 0));
		enemies.push_back(new Snake(6 * 32, 11 * 32, 0));

		enemies.push_back(new MBall(12 * 32, 13 * 32, 1));
		enemies.push_back(new MBall(9 * 32, 13 * 32, 1));
		enemies.push_back(new MBall(6 * 32, 12 * 32, 1));

		enemies.push_back(new Orc(13 * 32, 11 * 32, 2));
		enemies.push_back(new Orc(11 * 32, 6 * 32, 2));
		enemies.push_back(new Orc(7 * 32, 2 * 32, 2));

		enemies.push_back(new Skull(13 * 32, 8 * 32, 3));
		enemies.push_back(new Skull(14 * 32, 2 * 32, 3));

		enemies.push_back(new Eye(13 * 32, 8 * 32, 14));
		enemies.push_back(new Eye(13 * 32, 11 * 32, 14));

		enemies.push_back(new Frog(14 * 32, 11 * 32, 15));
		enemies.push_back(new Frog(2 * 32, 3 * 32, 15));
		enemies.push_back(new Frog(6 * 32, 7 * 32, 15));

		enemies.push_back(new Frog(13 * 32, 10 * 32, 16));
		enemies.push_back(new Frog(2 * 32, 5 * 32, 16));
		enemies.push_back(new Frog(2 * 32, 11 * 32, 16));

		enemies.push_back(new Bat(0 * 32, 1 * 32, 5));
		enemies.push_back(new Bat(0 * 32, 6 * 32, 5));
		enemies.push_back(new Bat(15 * 32, 10 * 32, 5));

		enemies.push_back(new Bat(12 * 32, 7 * 32, 4));
		enemies.push_back(new BowHead(3 * 32, 11 * 32, 4));
		enemies.push_back(new BowHead(6 * 32, 11 * 32, 4));

		enemies.push_back(new BowHead(6 * 32, 11 * 32, 6));
		enemies.push_back(new BowHead(6 * 32, 4 * 32, 6));

		enemies.push_back(new BowHead(2 * 32, 2 * 32, 7));
		enemies.push_back(new BowHead(2 * 32, 2 * 32, 7));
		enemies.push_back(new BowHead(2 * 32, 2 * 32, 7));
		enemies.push_back(new BowHead(2 * 32, 2 * 32, 7));
		enemies.push_back(new BowHead(16 * 32, 2 * 32, 7));
		enemies.push_back(new BowHead(16 * 32, 2 * 32, 7));
		enemies.push_back(new BowHead(16 * 32, 2 * 32, 7));
		enemies.push_back(new BowHead(16 * 32, 2 * 32, 7));

		CAudio::Instance()->Play(AUDIO_BGM, false);		// 撥放 WAVE

	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		//
		// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
		//
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));

		hero.OnMove(&mapBrown);

		for (unsigned int i = 0; i < items.size(); ++i)
		{

			items[i]->OnMove(&mapBrown);
		}

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
		if (hero.isOnFire())
		{
			for (unsigned int i = 0; i < enemies.size(); ++i)
			{
				if (mapBrown.getNext() == enemies[i]->getMapLocation() && !enemies[i]->IsDistroyed())
				{
					if (hero.InFireRange(enemies[i]->GetX1(), enemies[i]->GetY1()))
					{
						hero.setHitValid(false);
						enemies[i]->hit(hero.getDamage());
						enemies[i]->Distroy();
					}
				}
			}
		}

		if (hero.isOnThunder())
		{
			thunder.OnMove(enemies, mapBrown.getNext());
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
				if (!hero.isInvincible() && enemies[i]->InHitBox(hero.GetX1(), hero.GetY1()) && mapBrown.getNext() == enemies[i]->getMapLocation())
				{
					CAudio::Instance()->Play(HIT_HERO, false);
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

		for (unsigned int i = 0; i < enemies.size(); ++i)
		{
			if (enemies[i]->getLoot1())
			{
				items.push_back(new Gold(enemies[i]->GetX1(), enemies[i]->GetY1(), mapBrown.getNext()));
			}

			int item2X = enemies[i]->GetX1() + 16;
			switch (enemies[i]->getLoot2())
			{
			case 0:
				items.push_back(new Gold(item2X, enemies[i]->GetY1(), mapBrown.getNext()));
				break;
			case 1:
				items.push_back(new SmallBlood(item2X, enemies[i]->GetY1(), mapBrown.getNext()));
				break;
			case 2:
				items.push_back(new BigBlood(item2X, enemies[i]->GetY1(), mapBrown.getNext()));
				break;
			case 3:
				items.push_back(new Ring(item2X, enemies[i]->GetY1(), mapBrown.getNext()));
				break;
			default:
				break;
			}
			hero.addExp(enemies[i]->getExp());
		}

		for (unsigned int i = 0; i < items.size(); ++i)
		{
			if (items[i]->getMap() == mapBrown.getNext())
			{
				items[i]->MoveIcon();
			}
		}

		for (unsigned int i = 0; i < items.size(); ++i)
		{
			if (items[i]->InHitBox(hero.GetX1(), hero.GetY1()) && mapBrown.getNext() == items[i]->getMap())
			{
				if (hero.ItemMax())
				{
					break;
				}
				if (items[i]->pickUp(&hero)) //可以收到道具欄
				{
					hero.addItem(items[i]);
				}
				else
				{
					delete items[i];
				}
				items.erase(items.begin() + i);

				break;
			}
		}

		if (hero.FinishGame())
		{
			isWin = true;
			CAudio::Instance()->Stop(TITLE_BGM);
			GotoGameState(GAME_STATE_OVER);
		}

		if (hero.getHP() <= 0)
		{
			isWin = false;
			CAudio::Instance()->Stop(TITLE_BGM);
			GotoGameState(GAME_STATE_OVER);
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
		if (!loaded) 
		{
			thunder.LoadBitmap();
			hero.LoadBitmap();
			mapBrown.LoadBitMap();
			itemFrame.LoadBitmap(ITEM_FRAME, RGB(128, 0, 128));
			selectedItemFrame.LoadBitmap(SELECTED_ITEM_FRAME, RGB(128, 0, 128));
			eventFrame.LoadBitmap(EVENT_FRAME, RGB(128, 0, 128));
			statusBlocker.LoadBitmap(STATUS_BLOCKER, RGB(128, 0, 128));
			states[0].LoadBitmap(STATE_NAME, RGB(128, 0, 128));
			states[1].LoadBitmap(STATE_LV, RGB(128, 0, 128));
			states[2].LoadBitmap(STATE_LIFE, RGB(128, 0, 128));
			states[3].LoadBitmap(STATE_EXP, RGB(128, 0, 128));
			states[4].LoadBitmap(STATE_GOLD, RGB(128, 0, 128));
			itemLogo.LoadBitmap(ITEM_LOGO, RGB(40, 40, 40));
			statusFrame.LoadBitmap(STATUS_FRAME, RGB(0, 0, 0));
			hpBar.LoadBitmap(BAR_HP, RGB(128, 0, 128));
			expBar.LoadBitmap(BAR_EXP, RGB(128, 0, 128));
			CAudio::Instance()->Load(AUDIO_BGM, "Sounds\\KYOUDANB.MID");	// 載入編號1的聲音lake.mp3
			loaded = true;
		}

		//
		// 完成部分Loading動作，提高進度
		//
		//ShowInitProgress(50);
		//
		// 繼續載入其他資料
		//

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
		const char KEY_CTRL = 0x11;
		const char KEY_E = 69;
		const char KEY_F = 70;
		const char KEY_T = 84;
		if (nChar == KEY_CTRL)
		{
			this->SetCTRL(true);
		}
		if (nChar == KEY_LEFT) {
			hero.SetMovingLeft(true);
		}
		if (nChar == KEY_RIGHT) {
			hero.SetMovingRight(true);
		}
		if (nChar == KEY_UP) {
			if (this->onCTRL)
			{
				hero.moveCurrentItem(-1);
			}
			else
			{
				hero.SetMovingUp(true);
			}
		}
		if (nChar == KEY_DOWN) {
			if (this->onCTRL)
			{
				hero.moveCurrentItem(1);
			}
			else
			{
				hero.SetMovingDown(true);
			}
		}
		if (nChar == KEY_SPACE) {
			hero.SetAttack(true);
		}
		if (nChar == KEY_E) {
			hero.useItem();
		}
		if (nChar == KEY_F) {
			hero.SetFire(true);
		}
		if (nChar == KEY_T)
		{
			isWin = true;
			CAudio::Instance()->Stop(TITLE_BGM);
			GotoGameState(GAME_STATE_OVER);
			//hero.SetThunder(true);
			//thunder.initState();
			//thunder.OnPrepare();
		}
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char KEY_CTRL = 0x11;

		if (nChar == KEY_CTRL)
		{
			this->SetCTRL(false);
		}
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
		thunder.OnShow();

		selectedItemFrame.SetTopLeft(1 + 32 * 18, 1 + hero.getCurrentItem() * 32);
		selectedItemFrame.ShowBitmap();

		eventFrame.SetTopLeft(0, 416 - 25);
		eventFrame.ShowBitmap();
		itemLogo.SetTopLeft(32 * 18 + 3, 417);
		itemLogo.ShowBitmap();

		itemFrame.SetTopLeft(32 * 18, 0);
		itemFrame.ShowBitmap();
		//
		//exp
		states[3].SetTopLeft(279, 446);
		states[3].ShowBitmap();
		float expRatio = (float)hero.getExp() / (float)hero.getMaxExp();
		expBar.SetTopLeft(279 - 104 + 2 + (int)(104 * expRatio + 0.5), 446 + 18);
		expBar.ShowBitmap();
		statusFrame.SetTopLeft(279, 446 + 18);
		statusFrame.ShowBitmap();
		statusBlocker.SetTopLeft(279 - 104, 446 + 18);
		statusBlocker.ShowBitmap();
		//
		//hp
		states[2].SetTopLeft(170, 446);
		states[2].ShowBitmap();
		float hpRatio = (float)hero.getHP() / (float)hero.getMaxHP();
		hpBar.SetTopLeft(170 - 104 + 2 + +(int)(104 * hpRatio + 0.5), 446 + 18);
		hpBar.ShowBitmap();
		statusFrame.SetTopLeft(170, 446 + 18);
		statusFrame.ShowBitmap();
		statusBlocker.SetTopLeft(170 - 104, 446 + 18);
		statusBlocker.ShowBitmap();
		//
		//name
		states[0].SetTopLeft(8, 446);
		states[0].ShowBitmap();
		//
		//lv
		states[1].SetTopLeft(125, 446);
		states[1].ShowBitmap();
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, *fp;
		f.CreatePointFont(120, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char Levelbuffer[4];
		snprintf(Levelbuffer, 10, "%d", hero.getLevel());
		pDC->TextOut(127, 446 + 14, Levelbuffer);
		char goldBuffer[4];
		snprintf(goldBuffer, 10, "%d", hero.getGold());
		pDC->TextOut(392 + 18, 446 + 14, goldBuffer);

		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
		//
		//gold
		states[4].SetTopLeft(392, 446);
		states[4].ShowBitmap();

		hero.showItemList();

		for (unsigned int i = 0; i < items.size(); ++i)
		{
			if (items[i]->getMap() == mapBrown.getNext())
			{
				items[i]->ShowIcon();
			}
		}
	}
}