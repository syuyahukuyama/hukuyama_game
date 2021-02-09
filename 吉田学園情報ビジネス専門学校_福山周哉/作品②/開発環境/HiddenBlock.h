//===================================================================
//
// ウインドウの処理　[block.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _HIDDENBLOCK_H_
#define _HIDEENBLOCK_H_

#include "main.h"

//===========================================================================================================
//ブロックの構造体
//===========================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR cor;			//色
	float fWidth;			//幅
	float fHeight;			//高さ
	bool bUse;				//使用しているかどうか
}HIDDENBLOCK;


HRESULT InitHiBlock(void);
void UninitHiBlock(void);
void DrawHiBlock(void);
void UpdateHiBlock(void);
void SetHiBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR cor);
HIDDENBLOCK *GetHiBlock(void);

#endif
