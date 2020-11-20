//===================================================================
//
// スコアの処理　[score.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "score.h"

//=================================================================================================
//マクロ定義
//=================================================================================================
#define TEXTURE_X (10)		//テクスチャのX座標
#define TEXTURE_Y (1)		//テクスチャのY座標

#define MAX_SCORE (8)		//スコアの種類

//=================================================================================================
//グローバル変数
//=================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureScore[MAX_SCORE] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;
D3DXVECTOR3 g_posScore;								//スコアの位置
D3DXVECTOR3 g_rotScore;								//スコアの回転角
float g_flenghtScore;								//スコアの対角線の長さ
float g_fAngleScore;								//スコアの対角線の角度
int g_nScore;										//スコア

//=================================================================================================
//初期化処理
//=================================================================================================
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();
	g_nScore = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureScore[0]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffScore, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//スコアの位置
		pVtx[0].pos = D3DXVECTOR3(850.0f + 50.0f * nCntScore, 1000.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(850.0f + 50.0f * nCntScore, 960.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(900.0f + 50.0f * nCntScore, 1000.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(900.0f + 50.0f * nCntScore, 960.0f, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f , 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f , 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / 10, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / 10, 0.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
	return S_OK;
}

//=================================================================================================
//終了処理
//=================================================================================================
void UninitScore(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	//テクスチャの開放
	for (int nCntEnekyType = 0; nCntEnekyType < 3; nCntEnekyType++)
	{
		if (g_pTextureScore[nCntEnekyType] != NULL)
		{
			g_pTextureScore[nCntEnekyType]->Release();
			g_pTextureScore[nCntEnekyType] = NULL;
		}
	}
}

//=================================================================================================
//更新処理
//=================================================================================================
void UpdateScore(void)
{
}

//=================================================================================================
//描画処理
//=================================================================================================
void DrawScore(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore[0]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//=================================================================================================
//スコアの加算
//=================================================================================================
void AddScore(int nValue)
{
	//変数宣言
	int aNumber[8];			//スコアを収納
	g_nScore += nValue;

	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//スコアを収納
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//桁を一つずつ収納
		aNumber[nCntScore] = (g_nScore / powf(10, MAX_SCORE - (nCntScore + 1)));

		//テクスチャの更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 0.0f);

		//スコアの情報を次に移す
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}