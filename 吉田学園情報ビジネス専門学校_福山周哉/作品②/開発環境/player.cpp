//===================================================================
//
// プレイヤーの処理　[player.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "player.h"
#include "input.h"
#include "block.h"
#include "result.h"
#include "fade.h"
#include "effect.h"
#include "sound.h"

//==========================================================================================================
//マクロ定義
//==========================================================================================================
#define MOVE_DATA (10)
#define MOVE_Y (9.8f)																					//Y軸のジャンプ

#define TEXTURE_X (4)																					//プレイヤーの横数
#define TEXTURE_Y (2)																					//プレイヤーの縦数

#define MAX_TEXTUER (4)																					//テクスチャの数
#define MAX_BLOCK (256)																					//ブロックの最大数

#define SCREEN_WIDTH_HALF (120.0f)																		//X軸の位置
#define SCREEN_HEIGHT_Y (700.0f)																		//Y軸の位置

#define VERTEX_X (18)																					//中心から端までのXの距離
#define VERTEX_Y (54)																					//中心から端までのYの距離
#define MAX_ZAN (1)																						//最大残機

#define VERTEX_CHAR_X (40)																				//敵の位置座標X
#define VERTEX_CHAR_Y (40)																				//敵の位置座標Y

#define PLAY_ANIM	((1.0f / TEXTURE_X) * pPlayer->nPatternAnim)										//テクスチャの動作
#define UV_U		((1.0f / TEXTURE_X) + PLAY_ANIM)													//Uの値
#define UV_V		(1.0f / TEXTURE_Y)																	//Vの値

#define GRAVITY (0.35f)																					//重力
//==========================================================================================================
//グローバル変数
//==========================================================================================================
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;																//テクスチャのポリゴン
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;														//バッファポリゴン
int g_nCountAnim;																						//アニメーションカウンター
int g_nPatternAnim;																						//アニメーションパターンNO
int g_nCountAnimSe;																						//アニメーションカウンターセカンド
int g_nPatternAnimSe;																					//アニメーションパターンNOセカンド
D3DXVECTOR3 g_posPlayer;																				//プレイヤーの位置
D3DXVECTOR3 g_rotPlayer;																				//プレイヤーの回転角
float g_flenghtPlayer;																					//プレイヤーの対角線の長さ
float g_fAnglePlayer;																					//プレイヤーの対角線の角度
D3DXVECTOR3 g_movePlayer;																				//プレイヤーの移動量
PLAYER g_aPlayer;																						//プレイヤーの情報
PLAYERSTATE g_aPlayerState;																				//プレイヤーの状態情報
int nCounter;																							//ブロックの点滅

//===========================================================================================================
//初期化処理
//===========================================================================================================
HRESULT InitPlayer(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	PLAYER *pPlayer;

	pPlayer = GetPlayer();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\player001.png", &g_pTexturePlayer);

	//プレイヤー情報の初期化
	g_aPlayer.pos = D3DXVECTOR3(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_Y, 0.0f);
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.posold = g_aPlayer.pos;
	g_aPlayer.nLife = 0;
	g_aPlayer.State = PLAYERSTATE_APPEAR;
	g_aPlayer.nCounterDisp = 0;
	g_aPlayer.nCounterState = 0;
	g_aPlayer.nZan = MAX_ZAN;
	g_aPlayer.bUse = true;
	g_aPlayer.fHeight = VERTEX_Y;
	g_aPlayer.fWidth = VERTEX_X;

	g_nCountAnim = 0;
	g_nPatternAnim = 0;

	g_nCountAnimSe = 0;
	g_nPatternAnimSe = 0;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y , 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//ポリゴンの初期位置
	pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V * (g_nPatternAnimSe + 1.0f));
	pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, UV_V * g_nPatternAnimSe);
	pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V * (g_nPatternAnimSe + 1.0f));
	pVtx[3].tex = D3DXVECTOR2(UV_U, UV_V * g_nPatternAnimSe);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}


//===========================================================================================================
//終了処理
//===========================================================================================================
void UninitPlayer(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	//テクスチャの開放
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//===========================================================================================================
//更新処理
//===========================================================================================================
void UpdatePlayer(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;
	BLOCK *pBlock;

	pPlayer = GetPlayer();
	pBlock = GetBlock();
	nCounter++;

	pPlayer->posold = pPlayer->pos;

	//プレイヤーの登場位置
	if (pPlayer->bReset == true)
	{
		pPlayer->bReset = false;
		pPlayer->pos = D3DXVECTOR3(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_Y, 0.0f);
	}

	//更新処理
	//g_nCountAnim++;			//アニメーションのカウンター更新
	//if ((g_nCountAnim % 2) == 0)
	//{
	//	g_nPatternAnim = (g_nPatternAnim + 1) % 8;
	//}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (pPlayer->bUse == true)
	{
		//アニメーションパターンの更新
		pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V * (g_nPatternAnimSe + 1.0f));
		pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, UV_V * g_nPatternAnimSe);
		pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V * (g_nPatternAnimSe + 1.0f));
		pVtx[3].tex = D3DXVECTOR2(UV_U, UV_V * g_nPatternAnimSe);
	}

	//ポリゴンの左右
	if (GetKeyboardPress(DIK_LEFT) == true)
	{
		g_movePlayer.x += sinf(D3DX_PI / -2) * 1.2f;
		g_movePlayer.y += cosf(D3DX_PI / -2) * 1.2f;

		g_nPatternAnimSe = 1;

		//カウンターアップ
		pPlayer->nCountAnim += 1;

		if ((pPlayer->nCountAnim % 3) == 0)
		{
			//アニメーションパターンNOを更新
			pPlayer->nPatternAnim = (pPlayer->nPatternAnim + 1) % MAX_TEXTUER;
		}
	}

	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		g_movePlayer.x += sinf(D3DX_PI / 2) * 1.2f;
		g_movePlayer.y += cosf(D3DX_PI / 2) * 1.2f;

		g_nPatternAnimSe = 0;

		//カウンターアップ
		pPlayer->nCountAnim += 1;

		if ((pPlayer->nCountAnim % 3) == 0)
		{
			//アニメーションパターンNOを更新
			pPlayer->nPatternAnim = (pPlayer->nPatternAnim + 1) % MAX_TEXTUER;
		}
	}

	//プレイヤージャンプ
	if (g_aPlayer.bJump == false)
	{
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			g_aPlayer.bJump = true;
			g_movePlayer.y -= MOVE_Y;
		}
	}
	//重力の設定
	g_movePlayer.y += GRAVITY;

	//位置の更新
	g_aPlayer.pos.x += g_movePlayer.x;
	g_aPlayer.pos.y += g_movePlayer.y;

	//移動量の減衰
	g_movePlayer.x += (0.0f - g_movePlayer.x) * 0.20f;

	//
	if (g_aPlayer.pos.x < 0,0)
	{
		g_aPlayer.pos.x = (0 + (MOVE_DATA / 2));
	}

	//画面制限
	if (g_aPlayer.pos.x - VERTEX_X < 0)
	{
		g_aPlayer.pos.x = VERTEX_X;
	}
	if (g_aPlayer.pos.x + VERTEX_X > SCREEN_WIDTH)
	{
		g_aPlayer.pos.x = SCREEN_WIDTH - VERTEX_X;
	}
	/*if (g_aPlayer.pos.y - VERTEX_Y < 0)
	{
		g_aPlayer.pos.y = VERTEX_Y;
	}*/

	if (pPlayer->bReset == false)
	{
		//ブロックの当たり判定
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
		{
			switch (pBlock->type)
			{
			case BLOCK_NORMAL:						//通常ブロック
				if (pBlock->bUse == true)			//ブロックが使われているか
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_HIDDEN:						//隠しブロック
				if (pBlock->bUse == true)			//ブロックが使われているか
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						/*if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
							}
						}*/
						if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
								pBlock->type = BLOCK_NORMAL;
								PlaySound(SOUND_LABEL_SE_HIDDEN);
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						/*if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}*/
					}
				}
				break;

			case BLOCK_UNDERJUDGMENT:				//下だけ当たり判定のあるブロック
				if (pBlock->bUse == true)			//ブロックが使われているか
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
								PlaySound(SOUND_LABEL_SE_BLOCK);
							}
						}
					}
				}
				break;

			case BLOCK_UPJUDGMENT:					//上だけ当たり判定のあるブロック
				if (pBlock->bUse == true)			//ブロックが使われているか
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_HIDDENUNDER:					//見えない下だけブロック
				if (pBlock->bUse == true)			//ブロックが使われているか
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
								PlaySound(SOUND_LABEL_SE_BLOCK);
								pBlock->type = BLOCK_NORMAL;
							}
						}
					}
				}
				break;

			case BLOCK_BREAK:						//壊れるブロック
				if (pBlock->bUse == true)			//ブロックが使われているか
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
								pBlock->bUse = false;
								SetExplosion(pBlock->pos);
								PlaySound(SOUND_LABEL_SE_BLOCK);
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
								pBlock->bUse = false;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_NOJAMPBREAK:					//ジャンプできず壊れるブロック
				if (pBlock->bUse == true)			//ブロックが使われているか
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pBlock->bUse = false;
								SetExplosion(pBlock->pos);
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_GOAL:						//ゴールブロック
				if (pBlock->bUse == true)			//ブロックが使われているか
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
								SetFade(FADE_OUT, MODE_RESULT);
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_TSPRING:					//Ⅲバネブロック
				if (pBlock->bUse == true)			//ブロックが使われているか
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
								g_movePlayer.y -= 100.0f;
								SetFade(FADE_OUT, MODE_GAMETHIRD);
								PlaySound(SOUND_LABEL_SE_SPPLING);
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_FALL:						//落ちるブロック
				if (pBlock->bUse == true)			//ブロックが使われているか
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_PIPE:						//土管
				if (pBlock->bUse == true)			//ブロックが使われているか
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
								if (GetKeyboardTrigger(DIK_DOWN) == true)
								{
									pPlayer->bUse = false;
									SetFade(FADE_OUT, MODE_GAMESECOND);
									PlaySound(SOUND_LABEL_SE_ENTER);
								}
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_SPRING:
				if (pBlock->bUse == true)			//ブロックが使われているか
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
								g_movePlayer.y -= 100.0f;
								SetFade(FADE_OUT, MODE_GAME);
								PlaySound(SOUND_LABEL_SE_SPPLING);
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_SOIL:
				if (pBlock->bUse == true)			//ブロックが使われているか
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;
			}
		}
	}

	pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y , 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

}

//===========================================================================================================
//描画処理
//===========================================================================================================
void DrawPlayer(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定;
	pDevice->SetTexture(0, g_pTexturePlayer);

	//ポリゴンの描画
	if (g_aPlayer.bUse == true)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//===========================================================================================================
//プレイヤーの取得
//===========================================================================================================
PLAYER *GetPlayer(void)
{
	return &g_aPlayer;
}

//===========================================================================================================
//プレイヤーの状態の取得
//===========================================================================================================
PLAYERSTATE *GetPlayerState(void)
{
	return &g_aPlayerState;
}

//===========================================================================================================
//敵との当たり判定
//===========================================================================================================
bool HitPlayer(int nDamege)
{
	//変数宣言
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;

	pPlayer = &g_aPlayer;

	pPlayer->State = PLAYERSTATE_DAMAGE;

	pPlayer->nLife -= nDamege;

	//頂点バッファをロック
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//敵の体力が0になった
	if (pPlayer->nLife <= 0)
	{
		/*pPlayer->nZan -= 1;
		pPlayer->bDisp = false;*/

		if (pPlayer->nZan >= 0)
		{
			pPlayer->State = PLAYERSTATE_WAIT;
		}
		else
		{
			pPlayer->State = PLAYERSTATE_DEATH;
		}

		return true;
	}
	else
	{
		pPlayer->State = PLAYERSTATE_DAMAGE;
		pPlayer->nCounterState = 3;

		//敵の色を変更
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		return false;
	}
	pVtx += 4;

	//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}