//===================================================================
//
// 背景の処理　[bg.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "Bg.h"

//===========================================================================================================
//マクロ定義
//===========================================================================================================
#define MAX_BG (3)																					//背景テクスチャの最大数
//#define MAX_SPEED (0.0f)																				//背景の移動

//===========================================================================================================
//グローバル変数
//===========================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureBg = NULL;																//背景のテクスチャ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;														//背景
float g_fBg;																						//背景のスピード

//===========================================================================================================
//初期化処理
//===========================================================================================================
HRESULT InitBg(void)
{
	//初期化
	g_fBg = 0.0f;

	//変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	MODE mode = GetMode();

	//テクスチャの読み込み
	switch (mode)
	{
	case MODE_SELECT:
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky002.jpg", &g_pTextureBg);
		break;

	case MODE_GAME:
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.jpg", &g_pTextureBg);
		break;

	case MODE_GAMESECOND:
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bg002.jpg", &g_pTextureBg);
		break;

	case MODE_GAMETHIRD:
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.jpg", &g_pTextureBg);
		break;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBg, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);


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
	g_pVtxBuffBg->Unlock();

	return S_OK;
}

//===========================================================================================================
//終了処理
//===========================================================================================================
void UninitBg(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}

	//テクスチャの開放
	if (g_pTextureBg != NULL)
	{
		g_pTextureBg->Release();
		g_pTextureBg = NULL;
	}
	
}

//===========================================================================================================
//更新処理
//===========================================================================================================
void UpdateBg(void)
{
	//変数宣言
	VERTEX_2D *pVtx;


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	//g_fBg += MAX_SPEED;


	//頂点情報の設定
	/*pVtx[0].tex = D3DXVECTOR2(0.0 + g_fBg, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0 + g_fBg, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0 + g_fBg, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0 + g_fBg, 0.0);*/

	//頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}

//===========================================================================================================
//描画処理
//===========================================================================================================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBg);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);
}
