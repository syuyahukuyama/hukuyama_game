//===================================================================
//
// ブロックの処理　[block.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "block.h"
#include "sound.h"

//===========================================================================================================
//マクロ定義
//===========================================================================================================
#define MAX_BLOCK (512)																					//ブロックの最大数
#define MAX_LINE (3)																					//ブロックの横の数

#define MAX_TYPE_BLOCK (1)																				//ブロックの種類

#define VERTEX_CHAR_X (5)																				//ブロックの位置座標X
#define VERTEX_CHAR_Y (5)																				//ブロックの位置座標Y

#define VERTEX_X (5)																					//中心から端までのXの距離
#define VERTEX_Y (5)																					//中心から端までのYの距離

#define SCREEN_WIDTH_HALF (SCREEN_WIDTH / 2)															//X軸の位置
#define SCREEN_HEIGHT_Y ((SCREEN_HEIGHT / 4) * 3.8)														//Y軸の位置

#define BLOCK_WIDTH (700.0f)																			//X軸のブロックの位置

#define GRAVITY (0.35f)																					//重力

//=================================================================================================
//グローバル変数
//=================================================================================================
LPDIRECT3DTEXTURE9 g_aTextureBlock[6] = {};																//ブロックのテクスチャ
LPDIRECT3DVERTEXBUFFER9 g_aVtxBuffBlock = NULL;															//ブロックのバッファー
LPDIRECT3DTEXTURE9 g_aTextureBlockGoal = NULL;															//ゴールブロックのテクスチャ
LPDIRECT3DVERTEXBUFFER9 g_aVtxBuffBlockGoal = NULL;														//ゴールブロックのバッファー
BLOCK g_aBlock[MAX_BLOCK];																				//ブロックの最大数
D3DXVECTOR3 g_moveBlock;																				//ブロックの移動量

D3DXVECTOR3 g_movedPlayer;																				//プレイヤーの移動量

//=================================================================================================
//ブロックの初期化処理
//=================================================================================================
HRESULT InitBlock()
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	BLOCK *pBlock;
	PLAYER *pPlayer;
	STAGEINFO stage;

	pDevice = GetDevice();
	pBlock = GetBlock();
	pPlayer = GetPlayer();
	stage = GetStageInfo();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block001.jpg", &g_aTextureBlock[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block001.png", &g_aTextureBlock[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pipe000.png", &g_aTextureBlock[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\arrow.png", &g_aTextureBlock[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\spring.png", &g_aTextureBlock[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block006.png", &g_aTextureBlock[5]);

	//ブロックの初期化
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].bUse = false;
		g_aBlock[nCntBlock].fHeight = 0;
		g_aBlock[nCntBlock].fWidth = 0;
		g_moveBlock = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_aVtxBuffBlock, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック
	g_aVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//ブロックの位置
		pVtx[0].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y + pBlock->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y, 0.0f);

		//rhmの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//ブロックの位置
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}
	
	//頂点バッファをアンロック
	g_aVtxBuffBlock->Unlock();

	//ブロックの配置
	PlacementBlock(stage);

	return S_OK;
}

//=================================================================================================
//終了処理
//=================================================================================================
void UninitBlock(void)
{
	//ブロックの頂点バッファの開放
	if (g_aVtxBuffBlock != NULL)
	{
		g_aVtxBuffBlock->Release();
		g_aVtxBuffBlock = NULL;
	}

	//ブロックのテクスチャの開放
	for (int nCntEnekyType = 0; nCntEnekyType < MAX_TYPE_BLOCK; nCntEnekyType++)
	{
		if (g_aTextureBlock[nCntEnekyType] != NULL)
		{
			g_aTextureBlock[nCntEnekyType]->Release();
			g_aTextureBlock[nCntEnekyType] = NULL;
		}
	}
}

//=================================================================================================
//更新処理
//=================================================================================================
void UpdateBlock(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	BLOCK *pBlock;
	PLAYER *pPlayer;

	pBlock = &g_aBlock[0];
	pPlayer = GetPlayer();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_aVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			switch (pBlock->type)
			{
			case BLOCK_NORMAL:						//通常ブロック
				pBlock->type = BLOCK_NORMAL;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				break;

			case BLOCK_HIDDEN:						//隠しブロック
				pBlock->type = BLOCK_HIDDEN;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				break;

			case BLOCK_UNDERJUDGMENT:				//下だけ当たり判定のあるブロック
				pBlock->type = BLOCK_UNDERJUDGMENT;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				break;

			case BLOCK_UPJUDGMENT:				//上だけ当たり判定のあるブロック
				pBlock->type = BLOCK_UPJUDGMENT;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				break;

			case BLOCK_BREAK:						//壊れるブロック
				pBlock->type = BLOCK_BREAK;
				pVtx[0].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				break;

			case BLOCK_NOJAMPBREAK:						//ジャンプできず壊れるブロック
				pBlock->type = BLOCK_BREAK;
				pVtx[0].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				break;

			case BLOCK_NOTCOLLISION:				//当たり判定なしブロック
				pBlock->type = BLOCK_NOTCOLLISION;
				pVtx[0].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				break;

			case BLOCK_FALL:						//落ちるブロック
				pBlock->type = BLOCK_FALL;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pBlock->move.y = GRAVITY / 4;
				pBlock->pos.y += pBlock->move.y;
				break;
			}

			//ブロックの座標
			pVtx[0].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y + pBlock->fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y, 0.0f);

			//ブロックの位置
			pVtx[0].tex = D3DXVECTOR2(0.0f, pBlock->fTexV);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(pBlock->fTexU, pBlock->fTexV);
			pVtx[3].tex = D3DXVECTOR2(pBlock->fTexU, 0.0f);
		}
		pVtx += 4;
	}
}

//=================================================================================================
//描画処理
//=================================================================================================
void DrawBlock(void)
{
	//変数宣言
	BLOCK *pBlock;
	LPDIRECT3DDEVICE9 pDevice;

	pBlock = &g_aBlock[0];
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_aVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_aTextureBlock[pBlock->tex]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
		}
	}
}

//=================================================================================================
//ブロックの設定
//=================================================================================================
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight,BLOCKTYPE type,BLOCKTEX tex,GAMESTAGE stage,float fTexU,float fTexV)
{
	BLOCK *pBlock;
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;

	pBlock = &g_aBlock[0];
	pPlayer = GetPlayer();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_aVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == false)
		{
			//位置の設定
			pBlock->pos = pos;

			pBlock->fTexV = fTexV;

			pBlock->fTexU = fTexU;

			pBlock->fHeight = fHeight;

			pBlock->fWidth = fWidth;

			pBlock->type = type;

			pBlock->tex = tex;

			pBlock->stage = stage;

			switch (pBlock->type)
			{
			case BLOCK_NORMAL:					//通常ブロック
				pBlock->type = BLOCK_NORMAL;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				break;

			case BLOCK_HIDDEN:					//隠しブロック
				pBlock->type = BLOCK_HIDDEN;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				//サウンド
				//PlaySound(SOUND_LABEL_SE_HIT);
				break;

			case BLOCK_UNDERJUDGMENT:					//下だけ当たり判定のあるブロック
				pBlock->type = BLOCK_UNDERJUDGMENT;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				break;

			case BLOCK_BREAK:					//壊れるブロック
				pBlock->type = BLOCK_BREAK;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				break;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pBlock->pos.x , pBlock->pos.y + fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pBlock->pos.x , pBlock->pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pBlock->pos.x + fWidth, pBlock->pos.y + fHeight , 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pBlock->pos.x + fWidth, pBlock->pos.y, 0.0f);

			//敵のタイプ
			//pEnemy->nType = nType;
			pBlock->bUse = true;
			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_aVtxBuffBlock->Unlock();
}

//=================================================================================================
//ブロックの取得
//=================================================================================================
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}

//=================================================================================================
//ブロックの配置
//=================================================================================================
void PlacementBlock(STAGEINFO stage)
{
	BLOCK *pBlock;
	PLAYER *pPlayer;

	pBlock = GetBlock();
	pPlayer = GetPlayer();
}