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
		int  GetX1();					// 上 x 座標
		int  GetY1();					// 上 y 座標
		int  GetX2();					// 下 x 座標
		int  GetY2();					// 下 y 座標
		void Initialize();				// 設定擦子為初始值
		void LoadBitmap();				// 載入圖形
		void OnMove(MapBrown* map);     // 移動
		//bool HitHeroAction(int type, string pos);
		void OnShow();      			// 將擦子圖形貼到畫面
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		void SetXY(int nx, int ny);		// 設定擦子左上角座標
		bool IsOnLadder();
		void Drop();
		void SetOnLadder(bool flag);
	protected:
		CAnimation animation;		// 擦子的動畫
		int x, y;					// 擦子左上角座標
		bool onDrop;
		int previousBlock;
		bool isOnLadder;
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
	};
}
#endif // !CHERO_H
