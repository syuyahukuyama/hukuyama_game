//===================================================================
//
// ウインドウの処理　[explosion.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "boss.h"
#include "enemy.h"
#include "sound.h"

//=================================================================================================
//マクロ定義
//=================================================================================================
#define VERTEX_CENTER_X	(30)																	//中心座標のX座標
#define VERTEX_CENTER_Y	(30)																	//中心座標のY座標

#define MAX_BOSSENEMY (1)																		//ボスの数

#define MAX_PATTERN (3)																			//アニメーションパターンNO.の最大数
#define TEXTURE_X	(8)																			//アニメーションパターンの横の最大数
#define TEXTURE_Y	(1)																			//アニメーションパターンの縦の最大数

#define PLAY_ANIM	((1.0f / TEXTURE_X) * pBossEnemy->nPatternAnim)								//テクスチャの動作
#define UV_U		((1.0f / TEXTURE_X) + PLAY_ANIM)											//Uの値
#define UV_V		(1.0f / TEXTURE_Y)															//Vの値

//=================================================================================================
//ボスの構造体
//=================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;																			//位置
	int nCountAnim;																				//アニメーション
	int nPatternAnim;																			//アニメーションパターンNO
	bool bUse;																					//使用しているかどうか
}BOSSENEMY;

//=================================================================================================
//グローバル変数
//=================================================================================================
LPDIRECT3DTEXTURE9 g_apTextureBossEnemy = NULL;													//爆発のテクスチャ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBossEnemy = NULL;												//バッファの
BOSSENEMY g_aBossEnemy;																			//ボスの情報

//=================================================================================================
//初期化処理
//=================================================================================================
HRESULT InitBossEnemy(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\boss001.png", &g_apTextureBossEnemy);

	//ボスの初期化
	for (int nCntBossEnemy = 0; nCntBossEnemy < MAX_BOSSENEMY; nCntBossEnemy++)
	{
		g_aBossEnemy.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBossEnemy.nCountAnim = 0;
		g_aBossEnemy.nPatternAnim = 0;
		g_aBossEnemy.bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BOSSENEMY, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBossEnemy, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック
	g_pVtxBuffBossEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBossEnemy = 0; nCntBossEnemy < MAX_BOSSENEMY; nCntBossEnemy++)
	{
		//頂点座標の設定
		SetVertexBossEnemy(nCntBossEnemy);

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

		//敵の位置
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 0.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffBossEnemy->Unlock();
	return S_OK;
}

//=================================================================================================
//終了処理
//=================================================================================================
void UninitBossEnemy(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffBossEnemy != NULL)
	{
		g_pVtxBuffBossEnemy->Release();
		g_pVtxBuffBossEnemy = NULL;
	}

	//テクスチャの開放
	for (int nCntEnekyType = 0; nCntEnekyType < 1; nCntEnekyType++)
	{
		if (g_pVtxBuffBossEnemy != NULL)
		{
			g_pVtxBuffBossEnemy->Release();
			g_pVtxBuffBossEnemy = NULL;
		}
	}
}

//=================================================================================================
//更新処理
//=================================================================================================
void UpdateBossEnemy(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	BOSSENEMY *pBossEnemy;

	pBossEnemy = &g_aBossEnemy;

	//頂点バッファをロック
	g_pVtxBuffBossEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//
	for (int nCntBossEnemy = 0; nCntBossEnemy < MAX_BOSSENEMY; nCntBossEnemy++, pBossEnemy++)
	{
		//爆発の更新
		if (pBossEnemy->bUse == true)			//使われているかどうか
		{
			//カウンターアップ
			pBossEnemy->nCountAnim += 1;

			if ((pBossEnemy->nCountAnim % 3) == 0)
			{
				//アニメーションパターンNOを更新
				pBossEnemy->nPatternAnim = (pBossEnemy->nPatternAnim + 1) % MAX_PATTERN;
			}

			//アニメーションパターンの更新
			pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V);
			pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V);
			pVtx[3].tex = D3DXVECTOR2(UV_U, 0.0f);
		}
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffBossEnemy->Unlock();
}

//=================================================================================================
//描画処理
//=================================================================================================
void DrawBossEnemy(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBossEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureBossEnemy);

	//ポリゴンの描画
	for (int nCntBossEnemy = 0; nCntBossEnemy < MAX_BOSSENEMY; nCntBossEnemy++)
	{
		if (g_aBossEnemy.bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBossEnemy * 4, 2);
		}
	}
}

//=================================================================================================
//爆発の設定
//=================================================================================================
void SetBossEnemy(D3DXVECTOR3 pos)
{
	//変数宣言
	BOSSENEMY *pBossEnemy;

	pBossEnemy = &g_aBossEnemy;

	for (int nCntBossEnemy = 0; nCntBossEnemy < MAX_BOSSENEMY; nCntBossEnemy++, pBossEnemy++)
	{
		if (pBossEnemy->bUse == false)
		{
			//位置の設定
			pBossEnemy->pos = pos;

			//頂点座標の設定
			SetVertexBossEnemy(nCntBossEnemy);

			//敵のタイプ
			pBossEnemy->bUse = true;
			break;
		}
	}
}

//=================================================================================================
//爆発の頂点座標
//=================================================================================================
void SetVertexBossEnemy(int nIdx)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBossEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//弾の情報を移す
	pVtx += nIdx * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aBossEnemy.pos.x - VERTEX_CENTER_X, g_aBossEnemy.pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBossEnemy.pos.x - VERTEX_CENTER_X, g_aBossEnemy.pos.y - VERTEX_CENTER_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBossEnemy.pos.x + VERTEX_CENTER_X, g_aBossEnemy.pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBossEnemy.pos.x + VERTEX_CENTER_X, g_aBossEnemy.pos.y - VERTEX_CENTER_Y, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBossEnemy->Unlock();
}
