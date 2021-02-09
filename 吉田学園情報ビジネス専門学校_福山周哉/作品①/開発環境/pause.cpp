//=============================================================================
//
// ポーズメニューの処理 [pouse.cpp]
// Author : fukuyama syuya
//
//=============================================================================
#include "pause.h"
#include "input.h"
#include "fade.h"

//===========================================================================================================
//グローバル変数
//===========================================================================================================
LPDIRECT3DTEXTURE9 g_pPolygon = NULL;																	//ポーズのポリゴン
LPDIRECT3DVERTEXBUFFER9 g_pVtxPolygon = NULL;															//頂点ポリゴン

LPDIRECT3DTEXTURE9 g_pContinuePoly = NULL;																//コンテニューポリゴン
LPDIRECT3DVERTEXBUFFER9 g_pVtxContinuePoly = NULL;														//頂点コンテポリゴン

LPDIRECT3DTEXTURE9 g_pRetryPoly = NULL;																	//リトライポリゴン
LPDIRECT3DVERTEXBUFFER9 g_pVtxRetryPoly = NULL;															//頂点リトライポリゴン

LPDIRECT3DTEXTURE9 g_pQuitPoly = NULL;																	//終了ポリゴン
LPDIRECT3DVERTEXBUFFER9 g_pVtxQuitPoly = NULL;															//頂点終了ポリゴン

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;															//暗転

//===========================================================================================================
//初期化処理
//===========================================================================================================
HRESULT InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	//rhmの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, 120);
	pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, 120);
	pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, 120);
	pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 120);

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause100.png", &g_pPolygon);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxPolygon, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック
	g_pVtxPolygon->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(400, 800, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(400, 200, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(800, 800, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(800, 200, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxPolygon->Unlock();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause000.png", &g_pContinuePoly);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxContinuePoly, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック
	g_pVtxContinuePoly->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(450, 350, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(450, 230, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(750, 350, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(750, 230, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxContinuePoly->Unlock();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause001.png", &g_pRetryPoly);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxRetryPoly, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック
	g_pVtxRetryPoly->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(370, 500, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(370, 400, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(820, 500, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(820, 400, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxRetryPoly->Unlock();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause002.png", &g_pQuitPoly);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxQuitPoly, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック
	g_pVtxQuitPoly->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(370, 650, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(370, 550, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(820, 650, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(820, 550, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxQuitPoly->Unlock();

	return S_OK;
}

//===========================================================================================================
//終了処理
//===========================================================================================================
void UninitPause(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxPolygon != NULL)
	{
		g_pVtxPolygon->Release();
		g_pVtxPolygon = NULL;
	}

	//テクスチャの開放
	if (g_pVtxPolygon != NULL)
	{
		g_pVtxPolygon->Release();
		g_pVtxPolygon = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxContinuePoly != NULL)
	{
		g_pVtxContinuePoly->Release();
		g_pVtxContinuePoly = NULL;
	}

	//テクスチャの開放
	if (g_pVtxContinuePoly != NULL)
	{
		g_pVtxContinuePoly->Release();
		g_pVtxContinuePoly = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxRetryPoly != NULL)
	{
		g_pVtxRetryPoly->Release();
		g_pVtxRetryPoly = NULL;
	}

	//テクスチャの開放
	if (g_pVtxRetryPoly != NULL)
	{
		g_pVtxRetryPoly->Release();
		g_pVtxRetryPoly = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxQuitPoly != NULL)
	{
		g_pVtxQuitPoly->Release();
		g_pVtxQuitPoly = NULL;
	}

	//テクスチャの開放
	if (g_pVtxQuitPoly != NULL)
	{
		g_pVtxQuitPoly->Release();
		g_pVtxQuitPoly = NULL;
	}

}

//=================================================================================================
//更新処理
//=================================================================================================
void UpdatePause(void)
{
	
}

//=================================================================================================
//描画処理
//=================================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0,NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxPolygon, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pPolygon);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxContinuePoly, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pContinuePoly);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxRetryPoly, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pRetryPoly);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxQuitPoly, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pQuitPoly);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}