//===================================================================
//
// �E�C���h�E�̏����@[effect.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//=================================================================================================
//�v���g�^�C�v�錾
//=================================================================================================
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos);
void SetVertexEffect(int nIdx);

#endif 

