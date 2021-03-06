//===================================================================
//
// コインの処理　[coin.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _COIN_H_
#define _COIN_H_

#include "main.h"

//=================================================================================================
//コインの構造体
//=================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;																			//位置4
	float fWidth;																				//幅
	float fHeight;																				//高さ
}COIN;

//=================================================================================================
//プロトタイプ宣言
//=================================================================================================
HRESULT InitCoin(void);
void UninitCoin(void);
void UpdateCoin(void);
void DrawCoin(void);
void PlacementCoin(STAGEINFO stage);
void SetCoin(D3DXVECTOR3 pos, float fWidth, float fHeight);

#endif
