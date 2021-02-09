//===================================================================
//
// ゲーム画面の処理　[game.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "gameselect.h"
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

//===========================================================================================================
//	グローバル変数
//===========================================================================================================
//bool g_bPause = false;																					//ポーズ中かどうか
//GAMEJUB g_pResult = GAMECLAER_RESULT;																	//ゲームリザルト
//int g_nCount;																							//ステージカウント

//===========================================================================================================
//	ゲームの初期化処理
//===========================================================================================================
HRESULT InitSelectGame(void)
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
	//PlaySound(SOUND_LABEL_BGM009);

	//ブロックの配置
	SetBlock(D3DXVECTOR3(0,740, 0), 1320, 80, BLOCK_SOIL, BLOCKTEX_SOIL, GAMESTAGE_SELECT, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(420, 700, 0), 400, 40, BLOCK_SOIL, BLOCKTEX_SOIL, GAMESTAGE_SELECT, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(460, 660, 0), 320, 40, BLOCK_SOIL, BLOCKTEX_SOIL, GAMESTAGE_SELECT, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(500, 580, 0), 240, 80, BLOCK_SOIL, BLOCKTEX_SOIL, GAMESTAGE_SELECT, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(100, 500, 0), 160, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_SELECT, 4.0f, 1.0f);

	SetBlock(D3DXVECTOR3(260, 540, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_SELECT, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(300, 580, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_SELECT, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(1000, 500, 0), 160, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_SELECT, 4.0f, 1.0f);
	SetBlock(D3DXVECTOR3(960, 540, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_SELECT, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(920, 580, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_SELECT, 1.0f, 1.0f);

	//バネブロック
	SetBlock(D3DXVECTOR3(140, 440, 0), 60, 60, BLOCK_SPRING, BLOCKTEX_SPRING, GAMESTAGE_SELECT, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(1040, 440, 0), 60, 60, BLOCK_TSPRING, BLOCKTEX_SPRING, GAMESTAGE_SELECT, 1.0f, 1.0f);

	//土管
	SetBlock(D3DXVECTOR3(580, 510, 0), 70, 70, BLOCK_PIPE, BLOCKTEX_PIPE, GAMESTAGE_SELECT, 1.0f, 1.0f);

	return S_OK;
}

//===========================================================================================================
//	ゲームの終了処理
//===========================================================================================================
void UninitSelectGame(void)
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
void UpdateSelectGame(void)
{
	//変数宣言
	PLAYER *pPlayer;
	BLOCK *pBlock;

	pPlayer = GetPlayer();
	pBlock = GetBlock();

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
		switch (pBlock->type)
		{
		case BLOCK_SPRING:
			SetFade(FADE_OUT, MODE_GAME);
			break;
		}


}

//===========================================================================================================
//	ゲームの描画処理
//===========================================================================================================
void DrawSelectGame(void)
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

	/*if (g_bPause == true)
	{
		ポーズの描画処理
		DrawPause();
	}*/
}