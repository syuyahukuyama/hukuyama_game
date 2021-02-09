//===================================================================
//
// コインの処理　[coin.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "coin.h"

//===========================================================================================================
//マクロ定義
//===========================================================================================================
#define MAX_COIN (1)																					//テクスチャの最大数

//===========================================================================================================
//グローバル変数
//===========================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureCoin = NULL;																//コインのテクスチャ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCoin = NULL;															//コインのバッファ
COIN g_Coin[MAX_COIN];

//===========================================================================================================
//初期化処理
//===========================================================================================================
HRESULT InitCoin(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\coin000.png", &g_pTextureCoin);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffCoin, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

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

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	//頂点バッファをアンロックする
	g_pVtxBuffCoin->Unlock();

	return S_OK;
}

//===========================================================================================================
//終了処理
//===========================================================================================================
void UninitCoin(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffCoin != NULL)
	{
		g_pVtxBuffCoin->Release();
		g_pVtxBuffCoin = NULL;
	}

	//テクスチャの開放
	if (g_pTextureCoin != NULL)
	{
		g_pTextureCoin->Release();
		g_pTextureCoin = NULL;
	}
}

//===========================================================================================================
//更新処理
//===========================================================================================================
void UpdateCoin(void)
{
}

//===========================================================================================================
//描画処理
//===========================================================================================================
void DrawCoin(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCoin, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureCoin);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//===========================================================================================================
//コインの設定
//===========================================================================================================
void SetCoin(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	COIN *pCoin;
	VERTEX_2D *pVtx;

	pCoin = &g_Coin[0];

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++, pCoin++)
	{
		//位置の設定
		pCoin->pos;

		pCoin->fHeight;

		pCoin->fWidth;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pCoin->pos.x, pCoin->pos.y + fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pCoin->pos.x, pCoin->pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pCoin->pos.x + fWidth, pCoin->pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pCoin->pos.x + fWidth, pCoin->pos.y, 0.0f);
	}
	pVtx += 4;

	//頂点バッファをアンロックする
	g_pVtxBuffCoin->Unlock();
}

//===========================================================================================================
//コインの取得
//===========================================================================================================
COIN *GetCoin(void)
{
	return &g_Coin[0];
}