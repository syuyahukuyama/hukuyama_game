//===================================================================
//
// ゲーム画面の処理　[game.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "gamesecond.h"
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
//bool g_bPause = false;																				//ポーズ中かどうか
//GAMEJUB g_pResult = GAMECLAER_RESULT;																	//ゲームリザルト
//int g_nCount;																							//ステージカウント

//===========================================================================================================
//	セカンドゲームの初期化処理
//===========================================================================================================
HRESULT InitSecondGame(void)
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
	PlaySound(SOUND_LABEL_BGM010);

	//通常ブロック
	SetBlock(D3DXVECTOR3(300, 770, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(380, 510, 0), 40, 200, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 5.0f);

	SetBlock(D3DXVECTOR3(540, 510, 0), 40, 200, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 5.0f);

	SetBlock(D3DXVECTOR3(740, 530, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(780, 250, 0), 40, 440, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 11.0f);

	SetBlock(D3DXVECTOR3(1240, 30, 0), 40, 800, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 20.0f);

	SetBlock(D3DXVECTOR3(160, 30, 0), 1000, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 25.0f, 1.0f);

	SetBlock(D3DXVECTOR3(780, 770, 0), 480, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 12.0f, 1.0f);

	SetBlock(D3DXVECTOR3(800, 640, 0), 360, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 9.0f, 1.0f);

	SetBlock(D3DXVECTOR3(880, 510, 0), 360, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 9.0f, 1.0f);

	SetBlock(D3DXVECTOR3(800, 380, 0), 360, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 9.0f, 1.0f);

	SetBlock(D3DXVECTOR3(880, 250, 0), 360, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 9.0f, 1.0f);

	SetBlock(D3DXVECTOR3(500, 270, 0), 200, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 4.0f, 1.0f);

	//隠しブロック
	SetBlock(D3DXVECTOR3(340, 640, 0), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1200, 140, 0), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(430, 410, 0), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);

	//壊れるブロック
	SetBlock(D3DXVECTOR3(100, 770, 0), 40, 40, BLOCK_BREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(140, 770, 0), 40, 40, BLOCK_BREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(180, 770, 0), 40, 40, BLOCK_BREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);

	//当たり判定なしブロック
	SetBlock(D3DXVECTOR3(220, 770, 0), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(260, 770, 0), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(420, 510, 0), 120, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_TWO, 3.0f, 1.0f);

	SetBlock(D3DXVECTOR3(250, 200, 0), 120, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_TWO, 3.0f, 1.0f);

	//ジャンプできず壊れるブロック
	SetBlock(D3DXVECTOR3(580, 510, 0), 40, 40, BLOCK_NOJAMPBREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(620, 515, 0), 40, 40, BLOCK_NOJAMPBREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(660, 520, 0), 40, 40, BLOCK_NOJAMPBREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(700, 525, 0), 40, 40, BLOCK_NOJAMPBREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);

	//注意矢印
	SetBlock(D3DXVECTOR3(390, 560, 0.0f), 200, 150, BLOCK_ARROW, BLOCKTEX_ARROW, GAMESTAGE_ONE, 1.0f, 1.0f);

	//ゴールブロック
	SetBlock(D3DXVECTOR3(0, 190, 0), 120, 40, BLOCK_GOAL, BLOCKTEX_GOAL, GAMESTAGE_TWO, 3.0f, 1.0f);

	return S_OK;
}

//===========================================================================================================
//	セカンドゲームの終了処理
//===========================================================================================================
void UninitSecondGame(void)
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
//	セカンドゲームの更新処理
//===========================================================================================================
void UpdateSecondGame(void)
{
	//変数宣言
	PLAYER *pPlayer;

	pPlayer = GetPlayer();
	
	//ポーズメニューの更新処理
	/*if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == true)*/
	{//ポーズの更新処理
	 //UpdatePause();
	}
		//背景の更新
		UpdateBg();

		//プレイヤーの更新処理
		UpdatePlayer();

		//ブロックの終了処理
		UpdateBlock();

		//爆発の更新処理
		UpdateExplosion();

	if (pPlayer->pos.y > SCREEN_HEIGHT - VERTEX_Y + 100)
	{
		pPlayer->bReset = true;
		pPlayer->pos = D3DXVECTOR3(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_Y, 0.0f);

		SetFade(FADE_OUT, MODE_GAMESECOND);
		PlaySound(SOUND_LABEL_SE_AHIRU);
	}

	if (GetKeyboardTrigger(DIK_S))
	{
		SetFade(FADE_OUT, MODE_SELECT);
	}
}

//===========================================================================================================
//	セカンドゲームの描画処理
//===========================================================================================================
void DrawSecondGame(void)
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

	//if (g_bPause == true)
	//{
	//	//ポーズの描画処理
	//	//DrawPause();
	//}
}