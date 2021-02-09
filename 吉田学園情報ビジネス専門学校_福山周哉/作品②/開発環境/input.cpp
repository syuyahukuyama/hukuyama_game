//===================================================================
//
// �C���v�b�g�̏����@[input.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "input.h"

//===========================================================================================================
//�O���[�o���ϐ�
//===========================================================================================================
LPDIRECTINPUT8 g_pInput = NULL;																			//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;																//���̓f�o�C�X�ւ̃|�C���^
BYTE g_aKeyState[256];																					//�L�[�{�[�h���
BYTE g_aKeyStateTrigger[256];																			//�L�[�{�[�h���
BYTE g_aKeyStateRelese[256];																			//�����[�X���
BYTE g_aKeyStateRepeat[256];																			//�L�[���s�[�g���
int g_aCountRepeat[256];																				//�L�[���s�[�g�J�E���^�[

//===========================================================================================================
//����������
//===========================================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X�̐ݒ�
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h�̐ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�A�N�Z�X���̎擾
	g_pDevKeyboard->Acquire();
	return S_OK;
}

//=================================================================================================
//�I������
//=================================================================================================
void UninitKeyboard(void)
{
	if (g_pDevKeyboard != NULL)
	{
		//�A�N�Z�X���̔j��
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
//�X�V����
//=================================================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[256];

	//�f�o�C�X����f�[�^���擾
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
		//�A�N�Z�X���̎擾
		g_pDevKeyboard->Acquire();
	}
}

//=================================================================================================
//�L�[�{�[�h���̎擾
//=================================================================================================
bool GetKeyboardPress(int nKey)
{
	return(g_aKeyState[nKey] & 0x80) ? true	:	false;
}

//�L�[�{�[�h�g���K�[
bool GetKeyboardTrigger(int nKey)
{
	return(g_aKeyStateTrigger[nKey] & 0x80) ? true	:	false;
}