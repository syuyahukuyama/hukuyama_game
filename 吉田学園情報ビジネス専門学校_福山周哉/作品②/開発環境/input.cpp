//===================================================================
//
// インプットの処理　[input.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "input.h"

//===========================================================================================================
//グローバル変数
//===========================================================================================================
LPDIRECTINPUT8 g_pInput = NULL;																			//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;																//入力デバイスへのポインタ
BYTE g_aKeyState[256];																					//キーボード情報
BYTE g_aKeyStateTrigger[256];																			//キーボード情報
BYTE g_aKeyStateRelese[256];																			//リリース情報
BYTE g_aKeyStateRepeat[256];																			//キーリピート情報
int g_aCountRepeat[256];																				//キーリピートカウンター

//===========================================================================================================
//初期化処理
//===========================================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイスの設定
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットの設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードの設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//アクセス権の取得
	g_pDevKeyboard->Acquire();
	return S_OK;
}

//=================================================================================================
//終了処理
//=================================================================================================
void UninitKeyboard(void)
{
	if (g_pDevKeyboard != NULL)
	{
		//アクセス権の破棄
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
		if (g_pInput != NULL)
		{
			g_pInput->Release();
			g_pInput = NULL;
		}
	}
}

//=================================================================================================
//更新処理
//=================================================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[256];

	//デバイスからデータを取得
  	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nKey = 0; nKey < 256; nKey++)
		{
			g_aKeyStateTrigger[nKey] = (~g_aKeyState[nKey])&aKeyState[nKey];
			g_aKeyState[nKey] = aKeyState[nKey];
		}
	}
	else
	{
		//アクセス権の取得
		g_pDevKeyboard->Acquire();
	}
}

//=================================================================================================
//キーボード情報の取得
//=================================================================================================
bool GetKeyboardPress(int nKey)
{
	return(g_aKeyState[nKey] & 0x80) ? true	:	false;
}

//キーボードトリガー
bool GetKeyboardTrigger(int nKey)
{
	return(g_aKeyStateTrigger[nKey] & 0x80) ? true	:	false;
}