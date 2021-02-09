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

//=================================================================================================
//プレイヤーの構造体
//=================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;																			//位置
	D3DXVECTOR3 posold;																			//前回の位置
	D3DXVECTOR3 move;																			//プレイヤーの移動
	float fWidth;																				//幅
	float fHeight;																				//高さ
	int nLife;																					//体力
	PLAYERSTATE State;																			//状態
	int nCounterState;																			//状態カウンター
	int nCounterDisp;																			//表示カウンター
	int nZan;																					//残機
	int nCountAnim;																				//アニメーション
	int nPatternAnim;																			//アニメーションパターンNO
	int nCountAnimSe;																			//アニメーションカウンターセカンド
	int nPatternAnimSe;																			//アニメーションパターンNOセカンド
	bool bJump;																					//ジャンプしているかどうか
	bool bReset;																				//スタート地点に戻る
	bool bUse;																					//使われているかどうか
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
