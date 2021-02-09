//===================================================================
//
// フェードの処理　[fade.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "fade.h"

//=================================================================================================
//グローバル変数
//=================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;
FADE g_fade;																					//フェードの状態
MODE g_modeNext;																				//次のモード(画面)
D3DXCOLOR g_colorFade;																			//フェード色

//=================================================================================================
//初期化処理
//=================================================================================================
HRESULT InitFade(MODE modeNext)
{
	//変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffFade, NULL)))
	{
		return E_FAIL;
	}

	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
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
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	return S_OK;
}

//=================================================================================================
//終了処理
//=================================================================================================
void UninitFade(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=================================================================================================
//更新処理
//=================================================================================================
void UpdateFade(void)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	if (g_fade == FADE_IN)
	{
		g_colorFade.a -= 0.05f;
		if (g_colorFade.a <= 0.0f)
		{
			g_fade = FADE_NONE;
		}
	}
	else if (g_fade == FADE_OUT)
	{
		g_colorFade.a += 0.05f;
		if (g_colorFade.a >= 1.0f)
		{
			g_fade = FADE_IN;
			SetMode(g_modeNext);

		}
	}
	//頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();
}

//=================================================================================================
//描画処理
//=================================================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの読み込み
	pDevice->SetTexture(0, g_pTextureFade);

	//フェードの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//フェードの設定
void SetFade(FADE fade, MODE modeNext)
{
	g_fade = fade;
	g_modeNext = modeNext;
}

//=================================================================================================
//フェードの取得
//=================================================================================================
FADE GetFade(void)
{
	return g_fade;
}