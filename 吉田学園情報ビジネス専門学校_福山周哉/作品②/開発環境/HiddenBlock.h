//===================================================================
//
// �E�C���h�E�̏����@[block.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _HIDDENBLOCK_H_
#define _HIDEENBLOCK_H_

#include "main.h"

//===========================================================================================================
//�u���b�N�̍\����
//===========================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR cor;			//�F
	float fWidth;			//��
	float fHeight;			//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
}HIDDENBLOCK;


HRESULT InitHiBlock(void);
void UninitHiBlock(void);
void DrawHiBlock(void);
void UpdateHiBlock(void);
void SetHiBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR cor);
HIDDENBLOCK *GetHiBlock(void);

#endif
