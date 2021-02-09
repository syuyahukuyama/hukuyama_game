//===================================================================
//
// ウインドウの処理　[block.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//===========================================================================================================
//ブロックの列挙型
//===========================================================================================================
typedef enum
{
	BLOCK_NORMAL = 0,					//通常ブロック
	BLOCK_HIDDEN,						//隠しブロック
	BLOCK_NOTCOLLISION,					//当たり判定なしブロック
	BLOCK_UNDERJUDGMENT,				//下だけ当たり判定のあるブロック
	BLOCK_UPJUDGMENT,					//上に当たり判定のあるブロック
	BLOCK_HIDDENUNDER,					//見えない下だけブロック
	BLOCK_BREAK,						//壊れるブロック
	BLOCK_NOJAMPBREAK,					//ジャンプできない壊れるブロック
	BLOCK_FALL,							//落ちてくるブロック
	BLOCK_PIPE,							//土管
	BLOCK_TSPRING,						//Ⅲバネブロック
	BLOCK_GOAL,							//ゴールブロック
	BLOCK_ARROW,						//注意矢印
	BLOCK_SPRING,						//バネブロック
	BLOCK_SOIL,							//土ブロック
	BLOCK_MAX,
}BLOCKTYPE;

//===========================================================================================================
//ステージの種類
//===========================================================================================================
typedef enum
{
	GAMESTAGE_SELECT = 0,																					//セレクト画面
	GAMESTAGE_ONE,																							//ステージ１
	GAMESTAGE_TWO,																							//ステージ２
	GAMESTAGE_THREE,																						//ステージ３
	GAMESTAGE_MAX,
}GAMESTAGE;

//===========================================================================================================
//ブロックのテクスチャの列挙型
//===========================================================================================================
typedef enum
{
	BLOCKTEX_STONE = 0,					//石のテクスチャ
	BLOCKTEX_GOAL,						//ゴールテクスチャ
	BLOCKTEX_PIPE,						//土管テクスチャ
	BLOCKTEX_ARROW,						//矢印テクスチャ
	BLOCKTEX_SPRING,					//バネのテクスチャ
	BLOCKTEX_SOIL,						//土のテクスチャ
	BLOCKTEX_MAX,
}BLOCKTEX;

//===========================================================================================================
//ブロックの構造体
//===========================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//ブロックの位置
	D3DXVECTOR3 move;		//ブロックの移動
	BLOCKTYPE type;			//ブロックの種類
	BLOCKTEX tex;			//ブロックのテクスチャ
	GAMESTAGE stage;		//ステージ
	float fTexV;			//ブロックV
	float fTexU;			//ブロックU
	float fWidth;			//幅
	float fHeight;			//高さ
	bool bUse;				//使用しているかどうか
}BLOCK;

//==========================================================================================================
//プロトタイプ宣言
//==========================================================================================================
HRESULT InitBlock(void);
void UninitBlock(void);
void DrawBlock(void);
void UpdateBlock(void);
void PlacementBlock(STAGEINFO stage);
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight,BLOCKTYPE type,BLOCKTEX tex,GAMESTAGE stage,float fTexV,float fTexU);
BLOCK *GetBlock(void);

#endif
