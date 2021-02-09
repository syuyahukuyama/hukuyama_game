//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_BGM004,			// BGM4
	SOUND_LABEL_BGM005,			// BGM5
	SOUND_LABEL_BGM006,			// BGM6
	SOUND_LABEL_BGM007,			// BGM7
	SOUND_LABEL_BGM008,			// BGM8
	SOUND_LABEL_BGM009,			// BGM9
	SOUND_LABEL_BGM010,			// BGM10
	SOUND_LABEL_BGM011,			// BGM11
	SOUND_LABEL_SE_SHOT,		// �e���ˉ�
	SOUND_LABEL_SE_HIT,			// �q�b�g��
	SOUND_LABEL_SE_HIDDEN,		// �o����
	SOUND_LABEL_SE_SPPLING,		// �o�l��
	SOUND_LABEL_SE_BLOCK,		// �j��
	SOUND_LABEL_SE_ENTER,		// �y�ǂɓ��鉹
	SOUND_LABEL_SE_AHIRU,		// �A�q����
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif

