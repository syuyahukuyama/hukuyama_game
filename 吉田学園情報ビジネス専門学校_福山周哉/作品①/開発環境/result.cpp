//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : fukuyama syuya
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "game.h"

//=================================================================================================
//マクロ定義
//=================================================================================================
#define MAX_POLYGON (2)																		//リザルト数

//=================================================================================================
//グローバル変数
//=================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_POLYGON] = {};											//テクスチャのリザルト
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;												//バッファのリザルト

//=================================================================================================
//初期化処理
//=================================================================================================
HRESULT InitResult(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	GAMEJUB Result = GetGameResult();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bg002.jpg", &g_pTextureResult[0]);
	switch (Result)
	{
	case GAMECLAER_RESULT:
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gameclear_logo.png", &g_pTextureResult[1]);
		break;

	case GAMEOVER_RESULT:
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gameover_logo.png", &g_pTextureResult[1]);
		break;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL)))
	{
		return E_FAIL;
	}

	for (int nCntResult = 0; nCntResult < MAX_POLYGON; nCntResult++)
	{
		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		if (nCntResult == 1)
		{
			pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
		}

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

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
		pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
		pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
		pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

		pVtx + 4;
	}

	//音源
	PlaySound(SOUND_LABEL_BGM006);

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
}

//=================================================================================================
//終了処理
//=================================================================================================
void UninitResult(void)
{
	//サウンドを一時停止
	StopSound();

	//頂点バッファの開放
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	for (int nCntResult = 0; nCntResult < MAX_POLYGON; nCntResult++)
	{
		//テクスチャの開放
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}
}

//=================================================================================================
//更新処理
//=================================================================================================
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
}

//=================================================================================================
//描画処理
//=================================================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntResult = 0; nCntResult < MAX_POLYGON; nCntResult++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[nCntResult]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 * 4, 2);
	}
}
