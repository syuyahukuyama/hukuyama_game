//===================================================================
//
// �X�R�A�̏����@[score.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//=================================================================================================
//�v���g�^�C�v�錾
//=================================================================================================
HRESULT InitScore(void);
void UninitScore(void);
void DrawScore(void);
void UpdateScore(void);
void SetTextureScore(int nIdx, int nNumber);
void AddScore(int nValue);

#endif