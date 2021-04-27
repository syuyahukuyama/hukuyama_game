//===================================================================
//
// ゲーム画面の処理　[game.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "game.h"
#include "main.h"
#include "player.h"
#include "bg.h"
#include "input.h"
#include "pause.h"
#include "block.h"
#include "fade.h"
#include "result.h"
#include "effect.h"
#include "sound.h"
#include "coin.h"

//===========================================================================================================
//	マクロ定義
//===========================================================================================================
#define MAX_LINE (2)																					//敵の横数
#define MAX_POS_Y  (SCREEN_WIDTH / 3)																	//敵のY座標の位置
#define MAX_POS_X  (280.0f)																				//敵のX座標の位置

#define SCREEN_WIDTH_HALF (120.0f)																		//X軸の位置
#define SCREEN_HEIGHT_Y (700.0f)																		//Y軸の位置

#define VERTEX_Y (54)																					//中心から端までのYの距離

//===========================================================================================================
//	グローバル変数
//===========================================================================================================
bool g_bPause = false;																					//ポーズ中かどうか
GAMEJUB g_pResult = GAMECLAER_RESULT;																	//ゲームリザルト
int g_nCount;																							//ステージカウント

//===========================================================================================================
//	ゲームの初期化処理
//===========================================================================================================
HRESULT InitGame(void)
{
	//背景の初期化
	InitBg();

	//プレイヤーの初期化処理
	InitPlayer();

	//ブロックの初期化処理
	InitBlock();

	//爆発の初期化処理
	InitExplosion();

	//ポーズの初期化処理
	//InitPause();

	//音源
	PlaySound(SOUND_LABEL_BGM011);

	//通常ブロック
	SetBlock(D3DXVECTOR3(700, 485, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(660, 485, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(620, 485, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(580, 445, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(540, 445, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(500, 445, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(460, 445, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(180, 405, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(460, 485, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(460, 405, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(360, 765, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(320, 765, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(300, 765, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(100, 765, 0.0f), 120, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 3.0f, 1.0f);
	SetBlock(D3DXVECTOR3(60, 525, 0.0f), 40, 280, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 7.0f);
	SetBlock(D3DXVECTOR3(100, 525, 0.0f), 360, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 9.0f, 1.0f);

	SetBlock(D3DXVECTOR3(840, 735, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(820, 735, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(960, 615, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1000, 575, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1040, 535, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(180, 180, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(220, 180, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(440, 220, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(480, 220, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(520, 220, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(900, 295, 0.0f), 100, 15, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 2.0f, 1.0f);

	SetBlock(D3DXVECTOR3(20, 285, 0.0f), 100, 15, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 2.0f, 1.0f);

	//隠しブロック
	SetBlock(D3DXVECTOR3(420, 640, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(820, 580, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	//当たり判定なしブロック
	SetBlock(D3DXVECTOR3(550, 765, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(590, 765, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(630, 765, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(880, 695, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(920, 655, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(1080, 500, 0.0f), 200, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 5.0f, 1.0f);

	SetBlock(D3DXVECTOR3(220, 485, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(220, 445, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	//下だけの当たり判定ブロック
	SetBlock(D3DXVECTOR3(0, 405, 0.0f), 480, 40, BLOCK_UNDERJUDGMENT, BLOCKTEX_STONE, GAMESTAGE_ONE, 12.0f, 1.0f);

	//ゴールブロック
	SetBlock(D3DXVECTOR3(1160, 255, 0.0f), 40, 40, BLOCK_GOAL, BLOCKTEX_GOAL, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1200, 255, 0.0f), 40, 40, BLOCK_GOAL, BLOCKTEX_GOAL, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1240, 255, 0.0f), 40, 40, BLOCK_GOAL, BLOCKTEX_GOAL, GAMESTAGE_ONE, 1.0f, 1.0f);

	//ジャンプできず壊れるブロック
	SetBlock(D3DXVECTOR3(660, 285, 0.0f), 130, 15, BLOCK_NOJAMPBREAK, BLOCKTEX_STONE, GAMESTAGE_ONE, 3.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1080, 215, 0.0f), 80, 40, BLOCK_NOJAMPBREAK, BLOCKTEX_STONE, GAMESTAGE_ONE, 2.0f, 1.0f);

	//注意矢印
	SetBlock(D3DXVECTOR3(1040, 540, 0.0f), 200, 150, BLOCK_ARROW, BLOCKTEX_ARROW, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1080, 540, 0.0f), 200, 150, BLOCK_ARROW, BLOCKTEX_ARROW, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1120, 540, 0.0f), 200, 150, BLOCK_ARROW, BLOCKTEX_ARROW, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(-60, 540, 0.0f), 200, 150, BLOCK_ARROW, BLOCKTEX_ARROW, GAMESTAGE_ONE, 1.0f, 1.0f);

	return S_OK;
}

//===========================================================================================================
//	ゲームの終了処理
//===========================================================================================================
void UninitGame(void)
{
	//サウンドを一時停止
	StopSound();

	//ブロックの終了処理
	UninitBlock();

	//プレイヤーの終了処理
	UninitPlayer();

	//爆発の終了処理
	UninitExplosion();

	//背景の終了処理
	UninitBg();

	//ポーズの終了処理
	//UninitPause();
}

//===========================================================================================================
//	ゲームの更新処理
//===========================================================================================================
void UpdateGame(void)
{
	//変数宣言
	PLAYER *pPlayer;

	pPlayer = GetPlayer();
	g_nCount++;

	//ポーズメニューの更新処理
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == true)
	{//ポーズの更新処理
		//UpdatePause();
	}
	else
	{
		//背景の更新
		UpdateBg();

		//プレイヤーの更新処理
		UpdatePlayer();

		//ブロックの終了処理
		UpdateBlock();

		//爆発の更新処理
		UpdateExplosion();
	}

	if (GetKeyboardTrigger(DIK_S))
	{
		SetFade(FADE_OUT, MODE_SELECT);
	}

	if (pPlayer->pos.y > SCREEN_HEIGHT - VERTEX_Y + 100)
	{
		pPlayer->bReset = true;
		pPlayer->pos = D3DXVECTOR3(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_Y, 0.0f);

		SetFade(FADE_OUT, MODE_GAME);
		PlaySound(SOUND_LABEL_SE_AHIRU);
	}

}

//===========================================================================================================
//	ゲームの描画処理
//===========================================================================================================
void DrawGame(void)
{
	//背景の描画処理
	DrawBg();

	//プレイヤーの描画処理
	DrawPlayer();

	//ブロックの終了処理
	DrawBlock();

	//爆発の描画処理
	DrawExplosion();

	//敵の描画処理
	//DrawEnemy();

	if (g_bPause == true)
	{
		//ポーズの描画処理
		//DrawPause();
	}
}

//===========================================================================================================
//	リザルト
//===========================================================================================================
GAMEJUB GetGameResult(void)
{
	return g_pResult;
}

//===========================================================================================================
//	ポーズ処理
//===========================================================================================================
void SetPause(bool bPause)
{
	//ポーズ中かどうか
	g_bPause = bPause;
}