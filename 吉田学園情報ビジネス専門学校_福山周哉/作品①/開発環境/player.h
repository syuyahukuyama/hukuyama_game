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

typedef struct
{
	D3DXVECTOR3 pos;																			//�ʒu
	int nLife;																					//�̗�
	PLAYERSTATE State;																			//���
	int nCounterState;																			//��ԃJ�E���^�[
	int nCounterDisp;																			//�\���J�E���^�[
	int nZan;																					//�c�@
	bool bDisp;																					//�\��ON/OFF
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
