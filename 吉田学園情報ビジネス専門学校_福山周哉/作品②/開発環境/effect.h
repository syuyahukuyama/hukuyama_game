//===================================================================
//
// ウインドウの処理　[explosion.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

//=================================================================================================
//プロトタイプ宣言
//=================================================================================================
HRESULT InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos);
void SetVertexExplosion(int nIdx);

#endif 
