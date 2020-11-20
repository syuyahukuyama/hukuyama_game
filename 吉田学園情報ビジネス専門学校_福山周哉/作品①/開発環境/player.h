//===================================================================
//
// ウインドウの処理　[player.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//=================================================================================================
//プレイヤーの状態
//=================================================================================================
typedef enum
{
	PLAYERSTATE_APPEAR = 0,																		//自機の出現
	PLAYERSTATE_NORMAL,																			//通常
	PLAYERSTATE_DAMAGE,																			//ダメージ
	PLAYERSTATE_WAIT,																			//出現待ち
	PLAYERSTATE_DEATH,																			//死亡
	PLAYERSTATE_MAX,
}PLAYERSTATE;

typedef struct
{
	D3DXVECTOR3 pos;																			//位置
	int nLife;																					//体力
	PLAYERSTATE State;																			//状態
	int nCounterState;																			//状態カウンター
	int nCounterDisp;																			//表示カウンター
	int nZan;																					//残機
	bool bDisp;																					//表示ON/OFF
}PLAYER;

//=================================================================================================
//プロトタイプ宣言
//=================================================================================================
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
PLAYERSTATE *GetPlayerState(void);
bool HitPlayer(int aDamage);

#endif
