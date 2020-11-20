//===================================================================
//
// ウインドウの処理　[bullet.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "effect.h"
#include "player.h"
#include "input.h"

//=================================================================================================
//マクロ定義
//=================================================================================================
#define MAX_BULLET (2048)																		//弾の最大数
#define BULLET_SIZE (15)																		//弾の大きさ

#define VERTEX_CHAR_X (180)																		//敵の位置座標X
#define VERTEX_CHAR_Y (180)																		//敵の位置座標Y

#define VERTEX_HRFF_X (10)																		//自機の当たり判定X
#define VERTEX_HRFF_Y (10)																		//自機の当たり判定Y

//=================================================================================================
//弾の構造体
//=================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;																			//位置
	D3DXVECTOR3 move;																			//移動量
	D3DXCOLOR cor;																				//弾の色
	int nLife;																					//寿命
	int nDamage;																				//弾のダメージ量
	bool bUse;																					//使用しているかどうか
	BULLETTYPE type;																			//種類
}BULLET;

//=================================================================================================
//グローバル変数
//=================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;														//弾のテクスチャ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;												//バッファの														
BULLET g_aBullet[MAX_BULLET];																	//弾の情報												

//=================================================================================================
//プロトタイプ宣言
//=================================================================================================
void SetVertexBullet(int nIdy);

//=================================================================================================
//初期化処理
//=================================================================================================
HRESULT InitBullet(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bullet001.png", &g_pTextureBullet);

	//弾の設定
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].nDamage = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		SetVertexBullet(nCntBullet);

		//rhmの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 100, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 100, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 100, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 100, 255);

		//頂点テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
	return S_OK;
}

//=================================================================================================
//終了処理
//=================================================================================================
void UninitBullet(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

	//テクスチャの開放
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
}

//=================================================================================================
//更新処理
//=================================================================================================
void UpdateBullet(void)
{
	//変数宣言
	BULLET *pBullet;
	PLAYER *pPlayer;
	PLAYERSTATE *pPlayerState;
	
	VERTEX_2D *pVtx;

	pBullet = &g_aBullet[0];
	pPlayer = GetPlayer();
	pPlayerState = GetPlayerState();
	

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pBullet++)
	{
		//弾の更新
		if (pBullet->bUse == true)			//弾が使われているか
		{
			//位置の更新
			pBullet->pos += pBullet->move;

			//頂点座標の設定
			SetVertexBullet(nCntBullet);

			//エフェクトの設定
			//SetEffect(pBullet->pos);

			ENEMY *pEnemy;
			pEnemy = GetENEMY();

			if (pBullet->type == BULLETTYPE_PLAYER)
			{

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{

					if (pEnemy->bUse == true)
					{

						if (pEnemy->pos.y - VERTEX_CHAR_Y <= pBullet->pos.y &&
							pEnemy->pos.y + VERTEX_CHAR_Y >= pBullet->pos.y &&
							pEnemy->pos.x - VERTEX_CHAR_X <= pBullet->pos.x &&
							pEnemy->pos.x + VERTEX_CHAR_X >= pBullet->pos.x)
						{
							pBullet->bUse = false;
							HitEnemy(nCntEnemy, pBullet->nDamage);

							//音源
							PlaySound(SOUND_LABEL_SE_EXPLOSION);
						}
					}
				}

			}
			else if (pBullet->type == BULLETTYPE_ENEMY)
			{

				if (pPlayer->State == PLAYERSTATE_NORMAL)
				{
					if (pPlayer->pos.y - VERTEX_HRFF_Y <= pBullet->pos.y &&
						pPlayer->pos.y + VERTEX_HRFF_Y >= pBullet->pos.y &&
						pPlayer->pos.x - VERTEX_HRFF_X <= pBullet->pos.x &&
						pPlayer->pos.x + VERTEX_HRFF_X >= pBullet->pos.x)
					{
						HitPlayer(pPlayer->nLife);
					}
				}
			}

			//寿命チェック
			pBullet->nLife--;

			if (pBullet->nLife <= 0)
			{
 				pBullet->bUse = false;
			}
			
		}
	}
	

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=================================================================================================
//描画処理
//=================================================================================================
void DrawBullet(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	//ポリゴンの描画
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}		
	}
}

//=================================================================================================
//弾の頂点座標の設定
//=================================================================================================
void SetVertexBullet(int nIdy)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdy * 4;

	//弾の頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdy].pos.x - BULLET_SIZE, g_aBullet[nIdy].pos.y + BULLET_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdy].pos.x - BULLET_SIZE, g_aBullet[nIdy].pos.y - BULLET_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdy].pos.x + BULLET_SIZE, g_aBullet[nIdy].pos.y + BULLET_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdy].pos.x + BULLET_SIZE, g_aBullet[nIdy].pos.y - BULLET_SIZE, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=================================================================================================
//弾の設定
//=================================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nDamage, int nLife,BULLETTYPE type,D3DXCOLOR cor)
{
	//変数宣言
	BULLET *pBullet;
	VERTEX_2D *pVtx;

	pBullet = &g_aBullet[0];

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			//位置の設定
			pBullet->pos = pos;
			
			//移動量を設定
			pBullet->move = move;

			//頂点座標の設定
			SetVertexBullet(nCntBullet);

			//弾の攻撃力
			pBullet->nDamage = nDamage;

			//寿命を設定
			pBullet->nLife = nLife;
			
			//タイプ
			pBullet->type = type;

			pBullet->bUse = true;

			//弾の色
			pBullet->cor = cor;

			break;

		}
		//pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}