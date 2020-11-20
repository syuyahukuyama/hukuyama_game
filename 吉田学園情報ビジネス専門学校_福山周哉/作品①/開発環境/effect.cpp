//===================================================================
//
// ウインドウの処理　[explosion.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "effect.h"
#include "enemy.h"
#include "sound.h"

//=================================================================================================
//マクロ定義
//=================================================================================================
#define VERTEX_CENTER_X	(30)																	//中心座標のX座標
#define VERTEX_CENTER_Y	(30)																	//中心座標のY座標

#define MAX_EFFECT (10000)																		//爆発の数

#define MAX_PATTERN (8)																			//アニメーションパターンNO.の最大数
#define TEXTURE_X	(8)																			//アニメーションパターンの横の最大数
#define TEXTURE_Y	(1)																			//アニメーションパターンの縦の最大数

#define PLAY_ANIM	((1.0f / TEXTURE_X) * pEffect->nPatternAnim)								//テクスチャの動作
#define UV_U		((1.0f / TEXTURE_X) + PLAY_ANIM)											//Uの値
#define UV_V		(1.0f / TEXTURE_Y)															//Vの値

//=================================================================================================
//爆発の構造体
//=================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;																			//位置
	int nLife;																					//寿命
	bool bUse;																					//使用しているかどうか
}EFFECT;

//=================================================================================================
//グローバル変数
//=================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;													//爆発のテクスチャ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;											//バッファの
EFFECT g_aEffect[MAX_EFFECT];																//爆発の情報

//=================================================================================================
//初期化処理
//=================================================================================================
HRESULT InitEffect(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\effect000.jpg", &g_pTextureEffect);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		SetVertexEffect(nCntEffect);

		//rhmの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);

		//敵の位置
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
	return S_OK;
}

//=================================================================================================
//終了処理
//=================================================================================================
void UninitEffect(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

	//テクスチャの開放
	for (int nCntEnekyType = 0; nCntEnekyType < 3; nCntEnekyType++)
	{
		if (g_pTextureEffect != NULL)
		{
			g_pTextureEffect->Release();
			g_pTextureEffect = NULL;
		}
	}
}

//=================================================================================================
//更新処理
//=================================================================================================
void UpdateEffect(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	EFFECT *pEffect;

	pEffect = &g_aEffect[0];

	//頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	//
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		//エフェクトの更新
		if (pEffect->bUse == true)			//使われているかどうか
		{
			//寿命チェック
			pEffect->nLife--;
			
			if (pEffect->nLife <= 0)
			{
				pEffect->bUse = false;
			}
		}
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//=================================================================================================
//描画処理
//=================================================================================================
void DrawEffect(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//加算構成
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);

	//ポリゴンの描画
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	//元に戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


//=================================================================================================
//爆発の設定
//=================================================================================================
void SetEffect(D3DXVECTOR3 pos)
{
	//変数宣言
	EFFECT *pEffect;

	pEffect = &g_aEffect[0];

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == false)
		{
			//位置の設定
			pEffect->pos = pos;

			//頂点座標の設定
			SetVertexEffect(nCntEffect);
			pEffect->nLife = 30;

			//敵のタイプ
			pEffect->bUse = true;
			break;
		}
	}
}

//=================================================================================================
//爆発の頂点座標
//=================================================================================================
void SetVertexEffect(int nIdx)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	//弾の情報を移す
	pVtx += nIdx * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - VERTEX_CENTER_X, g_aEffect[nIdx].pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - VERTEX_CENTER_X, g_aEffect[nIdx].pos.y - VERTEX_CENTER_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + VERTEX_CENTER_X, g_aEffect[nIdx].pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + VERTEX_CENTER_X, g_aEffect[nIdx].pos.y - VERTEX_CENTER_Y, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}
