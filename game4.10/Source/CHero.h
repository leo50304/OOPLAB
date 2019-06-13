#ifndef CHERO_H
#define CHERO_H

#include "stdafx.h"
#include "map.h"
#include <vector>

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供可以用鍵盤或滑鼠控制的擦子
	// 看懂就可以改寫成自己的程式了
	/////////////////////////////////////////////////////////////////////////////

	class Item;

	class CHero
	{
	public:
		CHero();
		~CHero();
		int  GetX1(); //left x of hero
		int  GetY1(); //top y of hero
		void Initialize(); //init hero states when begin game
		void LoadBitmap();
		void OnMove(Map* map);// hero move action of every frame
		void OnShow(); //select and show bitmaps
		void SetMovingDown(bool flag); //set moving down
		void SetMovingLeft(bool flag); // set moveing left
		void SetMovingRight(bool flag); // set moving right
		void SetMovingUp(bool flag); // set moving up
		void SetXY(int nx, int ny); // set hero bit pos
		bool IsOnLadder(); //is hero on ladder or not
		void Drop();// toggle hero dropping
		void addHp(int num); // add hp to hero, no more than maxhp
		void addMaxHp(int num); // increase hero's maxhp
		void SetOnLadder(bool flag);// set hero on ladder or not
		void ToggleGodStatus();// toggle god mode status
		bool isGodMode();//return true if is god mode
		void ToggleInfiniteGold();// toggle infinite gold
		void SetAttack(bool flag);// set hero is attack or not
		void InitThunder(bool flag);// set the thunder should be initialized or not
		bool InitThunder();//return true if thunder should be init
		bool InAttackRange(int x, int y);// true if pos x, y is in hero's attack range
		bool InBossRange(int eX, int eY);//boss is in hit range
		bool InFireRange(int x, int y);// true if pos x, y is in fire ball's attack range
		void SetHoldUp(bool flag);// up is on pressed, for big jump
		bool isInvincible();// true if hero invinceble
		bool isOnAttack();//true if hero is on attack
		void BeatBack(bool flag, int direct);// beat back hero
		bool BeatBack();// true if hero is on beatback
		void setHitValid(bool flag);// set hero's attack is valid or not
		int getDamage();// return hero's damage
		int getHP();// return hero's hp
		int getMaxHP();// return hero's maxhp
		int getMaxExp();// return hero's max exp
		int getExp();// return hero's exp
		void addLevel();// hero add level
		int getLevel();// return hero's level
		void SetFire(bool flag);// set true if hero is shooting fireball
		bool isOnFire();// return true if hero is shooting fireball
		bool isOnBook();// return true if hero is holding book
		void addExp(int n);// add n exp to hero
		bool isOnThunder();// return true if hero is on thunder attack
		void SetThunder(bool flag); // set true to attack thunder
		void addItem(Item* item);// add item to hero
		bool ItemMax();// return true if hero holds max num of items
		void useItem();// use current selected item 
		int getGold();// return hero's gold
		void changeGold(int i);// add i to hero's gold
		int getCurrentItem();// get pos of current selected item
		void showItemList();// show icon of item list
		void moveCurrentItem(int i);// move current selected item
		void setWin(bool flag);// set true if hero can win the game
		bool FinishGame();//return true if hero can finish game
		bool buyItem(Item* item, int price);// buy item and add it to hero
		void moveMap();//actions when moveing map

	protected:
		vector<Item*> items;
		vector<Item*> usedItem;
		int currentItem;
		CMovingBitmap standL;
		CMovingBitmap standR;
		CMovingBitmap stayOnLadder;
		CMovingBitmap jumpL;
		CMovingBitmap jumpR;
		CMovingBitmap swordL;
		CMovingBitmap swordR;
		CMovingBitmap attackL;
		CMovingBitmap attackR;
		CMovingBitmap FBookL;
		CMovingBitmap FBookR;
		CAnimation moveRAnimation;
		CAnimation moveLAnimation;
		CAnimation moveUAnimation;
		CAnimation moveDAnimation;
		CAnimation FireL;
		CAnimation FireR;
		bool onCTRL = false;
		bool hitValid = false;
		double speed;
		bool onAttack;
		bool onFire;
		bool onBook;
		bool onThunder = false;
		int FireX;
		int FireY;
		bool FireSide;
		bool initThunder;
		bool beatBack = false;
		bool canWin = false;
		bool onFinishGame = false;
		double direction;
		double beatBackAy;
		double beatBackAx;
		double beatBackXSpeed;
		double beatBackYSpeed;
		int attackFrameCount = 0;
		int fireFrameCount = 0;
		int bookFrameCount = 0;
		int invincibleFrameCount = 0;
		int tempGold;
		int tempLevel;
		int tempHp;
		int tempMaxHP;
		int tempMaxExp;
		int tempDamage;
		bool godMode;
		int damage = 1;
		int gold = 3;
		int level = 1;
		int hp;
		int maxHP;
		int exp = 0;
		int maxExp = 10;
		int faceSide = 1;
		int x, y;					// 左上角座標
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
		bool HitGround(Map* map);
		bool HitTop(Map* map);
		bool GroundNotSolid(Map* map);
	};
}
#endif // !CHERO_H
