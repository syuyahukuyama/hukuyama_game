//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : fukuyama syuya
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
//#include "sound.h"

//=================================================================================================
//マクロ定義
//=================================================================================================
#define MAX_TITLE_POLYGON (2)																//描画するポリゴンの最大数

#define POLYGON_WIDTH (1100)																//ポリゴンの長さ
#define POLYGON_HEIGHT (350)																//ポリゴンの高さ

//=================================================================================================
//グローバル変数
//=================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL;														//テクスチャのタイトル
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;													//バッファのタイトル

LPDIRECT3DTEXTURE9 g_pTexturePolygon[MAX_TITLE_POLYGON] = {};									//ポリゴンのテクスチャ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;												//バッファのポリゴン
D3DXVECTOR3 g_posPolygon;

bool nCntShape;
int nCount;

//=================================================================================================
//初期化処理
//=================================================================================================
HRESULT InitTitle(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	nCntShape = true;
	int nCount = 30;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bg1.png", &g_pTextureTitle);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title01.png", &g_pTexturePolygon[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\press_enter.png", &g_pTexturePolygon[1]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	//音源
	//PlaySound(SOUND_LABEL_BGM007);

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygon, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nPolygon = 0; nPolygon < MAX_TITLE_POLYGON; nPolygon++)
	{
		switch (nPolygon)
		{
		case 0:
			pVtx[0].pos = D3DXVECTOR3(200, POLYGON_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(200, 100, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(POLYGON_WIDTH, POLYGON_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(POLYGON_WIDTH, 100, 0.0f);
			break;

		case 1:
			pVtx[0].pos = D3DXVECTOR3(200, 650, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(200, 550, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(1100, 650, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1100, 550, 0.0f);
			break;
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

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();

	return S_OK;
}

//=================================================================================================
//終了処理
//=================================================================================================
void UninitTitle(void)
{
	//サウンドを一時停止
	//StopSound();

	//頂点バッファの開放
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	//テクスチャの開放
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}

	//ポリゴン
	//頂点バッファの開放
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//=================================================================================================
//更新処理
//=================================================================================================
void UpdateTitle(void)
{
	//変数の更新
	nCount++;

	//ポリゴンの点滅
	if (nCount % 30 == 0)
	{
		nCntShape = nCntShape ? false : true;
	}

	//ENTERキーを押した
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(FADE_OUT, MODE_TUTORIAL);
	}

}

//=================================================================================================
//描画処理
//=================================================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitle);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//ポリゴン
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nPolygon = 0; nPolygon < MAX_TITLE_POLYGON; nPolygon++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygon[nPolygon]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygon * 4, 2);
	}
}