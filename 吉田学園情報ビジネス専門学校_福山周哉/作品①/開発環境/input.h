//===================================================================
//
// �E�C���h�E�̏����@[input.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//=================================================================================================
//�v���g�^�C�v�錾
//=================================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);									//�I������
void UpdateKeyboard(void);									//�X�V����
bool GetKeyboardPress(int nKey);							//�L�[�{�[�h�v���X
bool GetKeyboardTrigger(int nKey);							//�L�[�{�[�h�g���K�[

#endif
