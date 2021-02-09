//===================================================================
//
// ウインドウの処理　[explosion.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "effect.h"
#include "block.h"
#include "sound.h"

//=================================================================================================
//マクロ定義
//=================================================================================================
#define VERTEX_CENTER_X	(120)																	//中心座標のX座標
#define VERTEX_CENTER_Y	(60)																	//中心座標のY座標

#define MAX_EXPLOSION (256)																		//爆発の数

#define MAX_PATTERN (4)																			//アニメーションパターンNO.の最大数
#define TEXTURE_X	(4)																			//アニメーションパターンの横の最大数
#define TEXTURE_Y	(1)																			//アニメーションパターンの縦の最大数

#define PLAY_ANIM	((1.0f / TEXTURE_X) * pExplosion->nPatternAnim)								//テクスチャの動作
#define UV_U		((1.0f / TEXTURE_X) + PLAY_ANIM)											//Uの値
#define UV_V		(1.0f / TEXTURE_Y)															//Vの値

//=================================================================================================
//爆発の構造体
//=================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;																			//位置
	int nCountAnim;																				//アニメーション
	int nPatternAnim;																			//アニメーションパターンNO
	bool bUse;																					//使用しているかどうか
}EXPLOSION;

//=================================================================================================
//グローバル変数
//=================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;													//爆発のテクスチャ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;												//バッファの
EXPLOSION g_aExplosion[MAX_EXPLOSION];															//爆発の情報

//=================================================================================================
//初期化処理
//=================================================================================================
HRESULT InitExplosion(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\effect000.png", &g_pTextureExplosion);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].nCountAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点座標の設定
		SetVertexExplosion(nCntExplosion);

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

		//爆発の位置
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.25f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f, 0.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
	return S_OK;
}

//=================================================================================================
//終了処理
//=================================================================================================
void UninitExplosion(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

	//テクスチャの開放
	for (int nCntEnekyType = 0; nCntEnekyType < 3; nCntEnekyType++)
	{
		if (g_pTextureExplosion != NULL)
		{
			g_pTextureExplosion->Release();
			g_pTextureExplosion = NULL;
		}
	}
}

//=================================================================================================
//更新処理
//=================================================================================================
void UpdateExplosion(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	EXPLOSION *pExplosion;

	pExplosion = &g_aExplosion[0];

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	//
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		//爆発の更新
		if (pExplosion->bUse == true)			//使われているかどうか
		{
			//カウンターアップ
			pExplosion->nCountAnim += 1;

			if ((pExplosion->nCountAnim % 5) == 0)
			{
				//アニメーションパターンNOを更新
				pExplosion->nPatternAnim = (pExplosion->nPatternAnim + 1) % MAX_PATTERN;
			}

			//アニメーションパターンの更新
			pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V);
			pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V);
			pVtx[3].tex = D3DXVECTOR2(UV_U, 0.0f);

			//頂点座標の設定
			SetVertexExplosion(nCntExplosion);

			//爆発エフェクトの削除
			if (pExplosion->nPatternAnim == 3)
			{
				pExplosion->nPatternAnim = 0;
				pExplosion->bUse = false;
			}
		}
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//=================================================================================================
//描画処理
//=================================================================================================
void DrawExplosion(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	//ポリゴンの描画
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

//=================================================================================================
//爆発の設定
//=================================================================================================
void SetExplosion(D3DXVECTOR3 pos)
{
	//変数宣言
	EXPLOSION *pExplosion;

	pExplosion = &g_aExplosion[0];

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == false)
		{
			//位置の設定
			pExplosion->pos = pos;

			//頂点座標の設定
			SetVertexExplosion(nCntExplosion);

			//敵のタイプ
			pExplosion->bUse = true;
			break;
		}
	}
}

//=================================================================================================
//爆発の頂点座標
//=================================================================================================
void SetVertexExplosion(int nIdx)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	//弾の情報を移す
	pVtx += nIdx * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x - VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x - VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y - VERTEX_CENTER_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x + VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x + VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y - VERTEX_CENTER_Y, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}