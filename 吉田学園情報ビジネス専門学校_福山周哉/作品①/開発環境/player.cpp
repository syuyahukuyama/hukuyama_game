//===================================================================
//
// ウインドウの処理　[player.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "effect.h"
#include "fade.h"
#include "sound.h"

//==========================================================================================================
//マクロ定義
//==========================================================================================================
#define MOVE_DATA (10)

#define SCREEN_WIDTH_HALF (SCREEN_WIDTH / 2)															//X軸の位置
#define SCREEN_HEIGHT_Y ((SCREEN_HEIGHT / 4) * 3)														//Y軸の位置

#define VERTEX_X (25)																					//中心から端までのXの距離
#define VERTEX_Y (25)																					//中心から端までのYの距離
#define MAX_ZAN (1)																						//最大残機

#define VERTEX_CHAR_X (40)																				//敵の位置座標X
#define VERTEX_CHAR_Y (40)																				//敵の位置座標Y

//==========================================================================================================
//グローバル変数
//==========================================================================================================
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;																//テクスチャのポリゴン
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;														//バッファポリゴン
int g_nCountAnim;																						//アニメーションカウンター
int g_nPatternAnim;																						//アニメーションパターンNO
D3DXVECTOR3 g_posPlayer;																				//プレイヤーの位置
D3DXVECTOR3 g_rotPlayer;																				//プレイヤーの回転角
float g_flenghtPlayer;																					//プレイヤーの対角線の長さ
float g_fAnglePlayer;																					//プレイヤーの対角線の角度
D3DXVECTOR3 g_movePlayer;																				//プレイヤーの移動量
PLAYER g_aPlayer;																						//プレイヤーの情報
PLAYERSTATE g_aPlayerState;																				//プレイヤーの状態情報

//===========================================================================================================
//初期化処理
//===========================================================================================================
HRESULT InitPlayer(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\player000.png", &g_pTexturePlayer);

	//プレイヤー情報の初期化
	g_aPlayer.pos = D3DXVECTOR3(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_Y, 0.0f);
	g_aPlayer.nLife = 0;
	g_aPlayer.State = PLAYERSTATE_APPEAR;
	g_aPlayer.nCounterDisp = 0;
	g_aPlayer.nCounterState = 0;
	g_aPlayer.nZan = MAX_ZAN;
	g_aPlayer.bDisp = true;

	g_nCountAnim = 0;
	g_nPatternAnim = 0;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y + VERTEX_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y + VERTEX_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);

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

	//ポリゴンの初期位置
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}


//===========================================================================================================
//終了処理
//===========================================================================================================
void UninitPlayer(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	//テクスチャの開放
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//===========================================================================================================
//更新処理
//===========================================================================================================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;
	ENEMY *pEnemy;

	pEnemy = GetENEMY();
	pPlayer = GetPlayer();

	//更新処理
	//g_nCountAnim++;			//アニメーションのカウンター更新
	//if ((g_nCountAnim % 2) == 0)
	//{
	//	g_nPatternAnim = (g_nPatternAnim + 1) % 8;
	//}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//ポリゴンの上下左右or低速移動
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{
			g_movePlayer.x += sinf(D3DX_PI) / 2;
			g_movePlayer.y += cosf(D3DX_PI) / 2;
		}
		else
		{
			g_movePlayer.x += sinf(D3DX_PI) * 3;
			g_movePlayer.y += cosf(D3DX_PI) * 3;
		}
	}

	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{
			g_movePlayer.x -= sinf(-D3DX_PI) / 2;
			g_movePlayer.y -= cosf(-D3DX_PI) / 2;
		}
		else
		{
			g_movePlayer.x -= sinf(-D3DX_PI) * 3;
			g_movePlayer.y -= cosf(-D3DX_PI) * 3;
		}
	}

	else if (GetKeyboardPress(DIK_A) == true)
	{
		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{
			g_movePlayer.x += sinf(D3DX_PI / -2) / 2;
			g_movePlayer.y += cosf(D3DX_PI / -2) / 2;
		}
		else
		{
			g_movePlayer.x += sinf(D3DX_PI / -2) * 3;
			g_movePlayer.y += cosf(D3DX_PI / -2) * 3;
		}
	}

	else if (GetKeyboardPress(DIK_D) == true)
	{
		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{
			g_movePlayer.x += sinf(D3DX_PI / 2) / 2;
			g_movePlayer.y += cosf(D3DX_PI / 2) / 2;
		}
		else
		{
			g_movePlayer.x += sinf(D3DX_PI / 2) * 3;
			g_movePlayer.y += cosf(D3DX_PI / 2) * 3;
		}
	}

	//弾を発射
	if (GetKeyboardPress(DIK_SPACE) == true)
	{
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(1.2f, -25.0f, 0.0f), 2, 100, BULLETTYPE_PLAYER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(-1.2f, -25.0f, 0.0f), 2, 100, BULLETTYPE_PLAYER,D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//音源
		PlaySound(SOUND_LABEL_SE_SHOT);
	}

	//敵の弾に当たった
	if (pPlayer->State == PLAYERSTATE_NORMAL)
	{
		if (pPlayer->pos.y > pEnemy->pos.y - VERTEX_X &&
			pPlayer->pos.y < pEnemy->pos.y + VERTEX_X &&
			pPlayer->pos.x > pEnemy->pos.x - VERTEX_X &&
			pPlayer->pos.x < pEnemy->pos.x + VERTEX_X)
		{
			HitPlayer(pPlayer->nZan);
		}
	}

	//位置の更新
	g_aPlayer.pos.x += g_movePlayer.x;
	g_aPlayer.pos.y += g_movePlayer.y;

	//移動量の減衰
	g_movePlayer.x += (0.0f - g_movePlayer.x) * 0.18f;
	g_movePlayer.y += (0.0f - g_movePlayer.y) * 0.18f;

	if (g_aPlayer.pos.x < 0,0)
	{
		g_aPlayer.pos.x = (0 + (MOVE_DATA / 2));
	}

	//画面制限
	if (g_aPlayer.pos.x - VERTEX_X < 0)
	{
		g_aPlayer.pos.x = VERTEX_X;
	}
	if (g_aPlayer.pos.x + VERTEX_X > SCREEN_WIDTH)
	{
		g_aPlayer.pos.x = SCREEN_WIDTH - VERTEX_X;
	}
	if (g_aPlayer.pos.y - VERTEX_Y < 0)
	{
		g_aPlayer.pos.y = VERTEX_Y;
	}
	if (g_aPlayer.pos.y+ VERTEX_Y > SCREEN_HEIGHT)
	{
		g_aPlayer.pos.y = SCREEN_HEIGHT - VERTEX_Y;
	}

	pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y + VERTEX_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y + VERTEX_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);

	//自機の出現
	switch (g_aPlayer.State)
	{
	case PLAYERSTATE_APPEAR:
		g_aPlayer.nLife = 1;
		g_aPlayer.nCounterDisp++;
		if ((g_aPlayer.nCounterDisp % 6) == 0)
		{
			g_aPlayer.bDisp = g_aPlayer.bDisp ? false : true;
		}
		if ((g_aPlayer.nCounterDisp % 80) == 0)
		{
			g_aPlayer.State = PLAYERSTATE_NORMAL;
		}
		break;

	case PLAYERSTATE_NORMAL:
		g_aPlayer.bDisp = true;
		break;

	case PLAYERSTATE_DAMAGE:

		break;

	case PLAYERSTATE_WAIT:
		if ((g_aPlayer.nCounterDisp % 80) == 0)
		{
			g_aPlayer.State = PLAYERSTATE_APPEAR;
		}
		break;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

}

//===========================================================================================================
//描画処理
//===========================================================================================================
void DrawPlayer(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定;
	pDevice->SetTexture(0, g_pTexturePlayer);

	//ポリゴンの描画
	if (g_aPlayer.bDisp == true)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//===========================================================================================================
//プレイヤーの取得
//===========================================================================================================
PLAYER *GetPlayer(void)
{
	return &g_aPlayer;
}

//===========================================================================================================
//プレイヤーの状態の取得
//===========================================================================================================
PLAYERSTATE *GetPlayerState(void)
{
	return &g_aPlayerState;
}

//===========================================================================================================
//敵との当たり判定
//===========================================================================================================
bool HitPlayer(int nDamege)
{
	//変数宣言
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;

	pPlayer = &g_aPlayer;

	pPlayer->State = PLAYERSTATE_DAMAGE;

	pPlayer->nLife -= nDamege;

	//頂点バッファをロック
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//敵の体力が0になった
	if (pPlayer->nLife <= 0)
	{
		pPlayer->nZan -= 1;
		pPlayer->bDisp = false;
		SetExplosion(pPlayer->pos);

		if (pPlayer->nZan >= 0)
		{
			pPlayer->State = PLAYERSTATE_WAIT;
		}
		else
		{
			pPlayer->State = PLAYERSTATE_DEATH;
		}

		return true;
	}
	else
	{
		pPlayer->State = PLAYERSTATE_DAMAGE;
		pPlayer->nCounterState = 3;

		//敵の色を変更
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		return false;
	}
	pVtx += 4;

	//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

