//===================================================================
//
// 敵の処理　[enemy.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "enemy.h"
#include "sound.h"
#include "explosion.h"
#include "fade.h"
#include "score.h"
#include "bullet.h"
#include "player.h"
#include "effect.h"
#include "input.h"

//=================================================================================================
//マクロ定義
//=================================================================================================
#define MAX_TYPE_ENEMY (1)																				//敵の種類

#define VERTEX_CHAR_X (200)																				//敵の位置座標X
#define VERTEX_CHAR_Y (200)																				//敵の位置座標Y

#define VERTEX_X (500)																					//中心から端までのXの距離
#define VERTEX_Y (500)																					//中心から端までのYの距離

#define SCREEN_WIDTH_HALF (SCREEN_WIDTH / 2)															//X軸の位置

//=================================================================================================
//グローバル変数
//=================================================================================================
LPDIRECT3DTEXTURE9 g_apTextureEnemy = NULL;																//エネミーのテクスチャ
LPDIRECT3DVERTEXBUFFER9 g_aVtxBuffEnemy = NULL;															//エネミーのバッファー
ENEMY g_aEnemy[MAX_ENEMY];																				//エネミーの最大数
D3DXVECTOR3 g_moveEnemy;																				//敵の移動量
int g_nCntEnemy;
int nCounter;																							//敵の弾の発射カウンター

//=================================================================================================
//敵の初期化処理
//=================================================================================================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\boss003.png", &g_apTextureEnemy);

	//敵の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nMove = 0;
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_aVtxBuffEnemy, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック
	g_aVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//エネミーの位置
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - VERTEX_CHAR_X, g_aEnemy[nCntEnemy].pos.y + VERTEX_CHAR_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - VERTEX_CHAR_X, g_aEnemy[nCntEnemy].pos.y - VERTEX_CHAR_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + VERTEX_CHAR_X, g_aEnemy[nCntEnemy].pos.y + VERTEX_CHAR_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + VERTEX_CHAR_X, g_aEnemy[nCntEnemy].pos.y - VERTEX_CHAR_Y, 0.0f);

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
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_aVtxBuffEnemy->Unlock();
	return S_OK;
}

//=================================================================================================
//終了処理
//=================================================================================================
void UninitEnemy(void)
{
	//頂点バッファの開放
	if (g_aVtxBuffEnemy != NULL)
	{
		g_aVtxBuffEnemy->Release();
		g_aVtxBuffEnemy = NULL;
	}
	
	//テクスチャの開放
	for (int nCntEnekyType = 0; nCntEnekyType < MAX_TYPE_ENEMY; nCntEnekyType++)
	{
		if (g_apTextureEnemy != NULL)
		{
			g_apTextureEnemy->Release();
			g_apTextureEnemy = NULL;
		}
	}
}

//=================================================================================================
//更新処理
//=================================================================================================
void UpdateEnemy(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	ENEMY  *pEnemy;
	PLAYER *pPlayer;

	pEnemy = &g_aEnemy[0];
	g_nCntEnemy = 0;
	pPlayer = GetPlayer();
	nCounter++;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_aVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		//敵の更新
		if (pEnemy->bUse == true)			//敵が使われているか
		{
			g_nCntEnemy++;

			if (pEnemy->pos.x > SCREEN_WIDTH)
			{
				pEnemy->nMove += 1;
			}

			//位置の更新
			pEnemy->pos.x += g_moveEnemy.x;
			pEnemy->pos.y += g_moveEnemy.y;

			//敵の弾の発射

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(0.1f, 1.8f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(0.2f, 2.0f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			
			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(0.4f, 2.15f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(0.6f, 2.25f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(0.8f, 2.3f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.0f, 2.25f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.2f, 2.15f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.4f, 2.0f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.6f, 1.8f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.8f, 0.0f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.0f, 0.2f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.15f, 0.4f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.25f, 0.6f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.3f, 0.8f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.25f, 1.0f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.15f, 1.2f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.0f, 1.4f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.8f, 1.6f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.85f, 1.8f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			//逆回転

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-0.1f, 2.5f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(0.0f, 1.6f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-0.2f, 2.3f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-0.4f, 2.45f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-0.6f, 2.55f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-0.8f, 2.6f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-1.0f, 2.55f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-1.2f, 2.45f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-1.4f, 2.3f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-1.6f, 2.1f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			
			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.1f, 0.0f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.3f, 0.2f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.45f, 0.4f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.55f, 0.6f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.6f, 0.8f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.55f, 1.0f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.45f, 1.2f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.3f, 1.4f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.1f, 1.6f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-1.85f, 1.8f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			switch (pEnemy->state)
			{
			case ENEMYSTATE_DAMAGE:
				pEnemy->nCounterstate--;
				if (pEnemy->nCounterstate <= 0)
				{
					pEnemy->state = ENEMYSTATE_NORMAL;
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				break;
			}
		}
		pVtx += 4;
	}
}

//=================================================================================================
//描画処理
//=================================================================================================
void DrawEnemy(void)
{
	//変数宣言
	ENEMY *pEnemy;
	LPDIRECT3DDEVICE9 pDevice;

	pEnemy = &g_aEnemy[0];
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_aVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureEnemy);

		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=================================================================================================
//敵の設定
//=================================================================================================
void SetEnemy(D3DXVECTOR3 pos,int nLife, int nType,int nMove)
{
	ENEMY *pEnemy;
	VERTEX_2D *pVtx;

	pEnemy = &g_aEnemy[0];

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_aVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			//位置の設定
			pEnemy->pos = pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - VERTEX_CHAR_X, pEnemy->pos.y + VERTEX_CHAR_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x - VERTEX_CHAR_X, pEnemy->pos.y - VERTEX_CHAR_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x + VERTEX_CHAR_X, pEnemy->pos.y + VERTEX_CHAR_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + VERTEX_CHAR_X, pEnemy->pos.y - VERTEX_CHAR_Y, 0.0f);

			//敵の体力
			pEnemy->nLife = nLife;

			//敵の移動
			pEnemy->nMove = nMove;

			//敵のタイプ
			pEnemy->nType = nType;
			pEnemy->bUse = true;
			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_aVtxBuffEnemy->Unlock();
}

//=================================================================================================
//敵の取得
//=================================================================================================
ENEMY *GetENEMY(void)
{
	return &g_aEnemy[0];
}

//敵との当たり判定
bool HitEnemy(int nIdx, int nDamege)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_aVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//敵の体力が0になった
	g_aEnemy[nIdx].nLife -= nDamege;

	pVtx += nIdx * 4;

	if (g_aEnemy[nIdx].nLife <= 0)
	{
		g_aEnemy[nIdx].bUse = false;
		AddScore(ENEMY_POINT);
		SetExplosion(g_aEnemy[nIdx].pos);
		return true;
	}
	else
	{
		g_aEnemy[nIdx].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nIdx].nCounterstate = 5;

		//敵の色を変更
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		return false;
	}
	

	//頂点バッファをアンロック
	g_aVtxBuffEnemy->Unlock();
}

//使われている敵の数の取得
int GetCntEnemy(void)
{
	return g_nCntEnemy;
}