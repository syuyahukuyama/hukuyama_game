//===================================================================
//
// ウインドウの処理　[input.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//=================================================================================================
//プロトタイプ宣言
//=================================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);									//終了処理
void UpdateKeyboard(void);									//更新処理
bool GetKeyboardPress(int nKey);							//キーボードプレス
bool GetKeyboardTrigger(int nKey);							//キーボードトリガー

#endif
