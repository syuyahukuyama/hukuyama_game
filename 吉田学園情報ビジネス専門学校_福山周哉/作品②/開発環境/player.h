//===================================================================
//
// �E�C���h�E�̏����@[player.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//=================================================================================================
//�v���C���[�̏��
//=================================================================================================
typedef enum
{
	PLAYERSTATE_APPEAR = 0,																		//���@�̏o��
	PLAYERSTATE_NORMAL,																			//�ʏ�
	PLAYERSTATE_DAMAGE,																			//�_���[�W
	PLAYERSTATE_WAIT,																			//�o���҂�
	PLAYERSTATE_DEATH,																			//���S
	PLAYERSTATE_MAX,
}PLAYERSTATE;

//=================================================================================================
//�v���C���[�̍\����
//=================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;																			//�ʒu
	D3DXVECTOR3 posold;																			//�O��̈ʒu
	D3DXVECTOR3 move;																			//�v���C���[�̈ړ�
	float fWidth;																				//��
	float fHeight;																				//����
	int nLife;																					//�̗�
	PLAYERSTATE State;																			//���
	int nCounterState;																			//��ԃJ�E���^�[
	int nCounterDisp;																			//�\���J�E���^�[
	int nZan;																					//�c�@
	int nCountAnim;																				//�A�j���[�V����
	int nPatternAnim;																			//�A�j���[�V�����p�^�[��NO
	int nCountAnimSe;																			//�A�j���[�V�����J�E���^�[�Z�J���h
	int nPatternAnimSe;																			//�A�j���[�V�����p�^�[��NO�Z�J���h
	bool bJump;																					//�W�����v���Ă��邩�ǂ���
	bool bReset;																				//�X�^�[�g�n�_�ɖ߂�
	bool bUse;																					//�g���Ă��邩�ǂ���
}PLAYER;

//=================================================================================================
//�v���g�^�C�v�錾
//=================================================================================================
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
PLAYERSTATE *GetPlayerState(void);
bool HitPlayer(int aDamage);

#endif
