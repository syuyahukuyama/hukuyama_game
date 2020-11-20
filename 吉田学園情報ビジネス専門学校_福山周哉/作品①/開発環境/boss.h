//===================================================================
//
// ボスの処理　[boss.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _BOSSENEMY_H_
#define _BOSSENEMY_H_

#include "main.h"

//=================================================================================================
//プロトタイプ宣言
//=================================================================================================
HRESULT InitBossEnemy(void);
void UninitBossEnemy(void);
void UpdateBossEnemy(void);
void DrawBossEnemy(void);
void SetBossEnemy(D3DXVECTOR3 pos);
void SetVertexBossEnemy(int nIdx);

#endif 
