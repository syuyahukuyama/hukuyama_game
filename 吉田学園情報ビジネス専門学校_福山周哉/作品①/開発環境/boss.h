//===================================================================
//
// �{�X�̏����@[boss.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _BOSSENEMY_H_
#define _BOSSENEMY_H_

#include "main.h"

//=================================================================================================
//�v���g�^�C�v�錾
//=================================================================================================
HRESULT InitBossEnemy(void);
void UninitBossEnemy(void);
void UpdateBossEnemy(void);
void DrawBossEnemy(void);
void SetBossEnemy(D3DXVECTOR3 pos);
void SetVertexBossEnemy(int nIdx);

#endif 
