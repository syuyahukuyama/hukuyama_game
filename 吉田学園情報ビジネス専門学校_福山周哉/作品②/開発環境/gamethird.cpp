//===================================================================
//
// ゲーム画面の処理　[game.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "gamethird.h"
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
//bool g_bPause = false;																					//ポーズ中かどうか
//GAMEJUB g_pResult = GAMECLAER_RESULT;																	//ゲームリザルト
//int g_nCount;																							//ステージカウント

//===========================================================================================================
//	ゲームの初期化処理
//===========================================================================================================
HRESULT InitThirdGame(void)
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
	PlaySound(SOUND_LABEL_BGM009);

	//通常ブロック
	SetBlock(D3DXVECTOR3(100, 765, 0.0f), 80, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 2.0f, 1.0f);
	SetBlock(D3DXVECTOR3(140, 45, 0.0f), 40, 720, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 18.0f);
	SetBlock(D3DXVECTOR3(100, 405, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(140, 45, 0.0f), 1000, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 25.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1100, 45, 0.0f), 40, 640, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 16.0f);
	SetBlock(D3DXVECTOR3(280, 765, 0.0f), 860, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 21.0f, 1.0f);
	SetBlock(D3DXVECTOR3(280, 155, 0.0f), 40, 520, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 13.0f);
	SetBlock(D3DXVECTOR3(1100, 45, 0.0f), 40, 640, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 16.0f);
	SetBlock(D3DXVECTOR3(280, 645, 0.0f), 460, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 12.0f, 1.0f);
	SetBlock(D3DXVECTOR3(820, 645, 0.0f), 120, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 3.0f, 1.0f);
	SetBlock(D3DXVECTOR3(980, 155, 0.0f), 40, 400, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 10.0f);
	SetBlock(D3DXVECTOR3(280, 155, 0.0f), 720, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 18.0f, 1.0f);
	SetBlock(D3DXVECTOR3(740, 645, 0.0f), 80, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 2.0f, 1.0f);

	SetBlock(D3DXVECTOR3(420, 270, 0.0f), 40, 240, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 6.0f);
	SetBlock(D3DXVECTOR3(860, 270, 0.0f), 40, 240, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 6.0f);
	SetBlock(D3DXVECTOR3(530, 270, 0.0f), 40, 240, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 6.0f);
	SetBlock(D3DXVECTOR3(640, 270, 0.0f), 40, 240, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 6.0f);
	SetBlock(D3DXVECTOR3(750, 270, 0.0f), 40, 240, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 6.0f);

	//隠しブロック
	SetBlock(D3DXVECTOR3(60, 640, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(20, 510, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(100, 285, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(10, 165, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(180, 280, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(240, 410, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(150, 540, 0.0f), 80, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(260, 670, 0.0f), 40, 20, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(940, 645, 0.0f), 160, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 4.0f, 1.0f);

	SetBlock(D3DXVECTOR3(320, 520, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(380, 390, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	//当たり判定なしブロック
	SetBlock(D3DXVECTOR3(1100, 685, 0.0f), 40, 80, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 2.0f);

	//上だけ判定ブロック
	SetBlock(D3DXVECTOR3(460, 430, 0.0f), 200, 40, BLOCK_UPJUDGMENT, BLOCKTEX_STONE, GAMESTAGE_ONE, 5.0f, 1.0f);
	SetBlock(D3DXVECTOR3(750, 430, 0.0f), 120, 40, BLOCK_UPJUDGMENT, BLOCKTEX_STONE, GAMESTAGE_ONE, 3.0f, 1.0f);

	//ゴールブロック
	SetBlock(D3DXVECTOR3(460, 470, 0.0f), 400, 40, BLOCK_GOAL, BLOCKTEX_GOAL, GAMESTAGE_ONE, 10.0f, 1.0f);

	return S_OK;
}

//===========================================================================================================
//	ゲームの終了処理
//===========================================================================================================
void UninitThirdGame(void)
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
void UpdateThirdGame(void)
{
	//変数宣言
	PLAYER *pPlayer;

	pPlayer = GetPlayer();

	//ポーズメニューの更新処理
	/*if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
	}*/

	//if (g_bPause == true)
	//{//ポーズの更新処理
	// //UpdatePause();
	//}
		//背景の更新
		UpdateBg();

		//プレイヤーの更新処理
		UpdatePlayer();

		//ブロックの終了処理
		UpdateBlock();

		//爆発の更新処理
		UpdateExplosion();

	//クリアリザルト画面へ飛ぶ処理
	//int nCntEnemy = GetCntEnemy();

	/*if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
	g_pResult = GAMECLAER_RESULT;
	SetFade(FADE_OUT, MODE_RESULT);
	}*/

	////ゲームオーバーリザルト画面の飛ぶ処理
	//if (pPlayer->nZan == 0)
	//{
	//	g_pResult = GAMEOVER_RESULT;
	//	SetFade(FADE_OUT, MODE_RESULT);
	//}
	if (GetKeyboardTrigger(DIK_L))
	{
		SetFade(FADE_OUT, MODE_GAMETHIRD);
	}

	if (pPlayer->pos.y > SCREEN_HEIGHT - VERTEX_Y + 100)
	{
		pPlayer->bReset = true;
		pPlayer->pos = D3DXVECTOR3(60, SCREEN_HEIGHT_Y, 0.0f);

		SetFade(FADE_OUT, MODE_GAMETHIRD);
		PlaySound(SOUND_LABEL_SE_AHIRU);
	}

	if (GetKeyboardTrigger(DIK_S))
	{
		SetFade(FADE_OUT, MODE_SELECT);
	}

}

//===========================================================================================================
//	ゲームの描画処理
//===========================================================================================================
void DrawThirdGame(void)
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
}