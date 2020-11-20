//===================================================================
//
// �Q�[����ʂ̏����@[game.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "game.h"
#include "main.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "enemy.h"
#include "score.h"
#include "explosion.h"
#include "sound.h"
#include "input.h"
#include "fade.h"
#include "pause.h"
#include "effect.h"
#include "result.h"

//===========================================================================================================
//�}�N����`
//===========================================================================================================
#define MAX_LINE (2)																					//�G�̉���
#define MAX_POS_Y  (SCREEN_WIDTH / 3)																	//�G��Y���W�̈ʒu
#define MAX_POS_X  (280.0f)																				//�G��X���W�̈ʒu

#define SCREEN_WIDTH (1280)																				//��ʂ̕�
#define SCREEN_HEIGHT (1080)																			//��ʂ̍���


//===========================================================================================================
//�O���[�o���ϐ�
//===========================================================================================================
bool g_bPause = false;																					//�|�[�Y�����ǂ���
GAMEJUB g_pResult = GAMECLAER_RESULT;																	//�Q�[�����U���g

//===========================================================================================================
//�Q�[���̏���������
//===========================================================================================================
HRESULT InitGame(void)
{
	//�w�i�̏�����
	InitBg();

	//�|���S���̏���������
	InitPlayer();

	//�e�̏���������
	InitBullet();

	//�G�̏���������
	InitEnemy();

	//�X�R�A�̏���������
	InitScore();

	//�����̏���������
	InitExplosion();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�|�[�Y�̏���������
	InitPause();

	//�G�̕\��
	for (int nCntEnemy = 0; nCntEnemy < MAX_LINE; nCntEnemy++)
	{
		//�G�̈ʒu�Ƒ̗�
		SetEnemy(D3DXVECTOR3(MAX_POS_Y *(nCntEnemy + 1), MAX_POS_X, 0.0f),1500, 0,20);
	}

	//����
	PlaySound(SOUND_LABEL_BGM008);

	return S_OK;
}

//===========================================================================================================
//�Q�[���̏I������
//===========================================================================================================
void UninitGame(void)
{
	//�T�E���h���ꎞ��~
	StopSound();

	//�|���S���̏I������
	UninitPlayer();

	//�w�i�̏I������
	UninitBg();

	//�e�̏I������
	UninitBullet();

	//�G�̏I������
	UninitEnemy();

	//�X�R�A�̏I������
	UninitScore();

	//�����̏I������
	UninitExplosion();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�|�[�Y�̏I������
	UninitPause();
}

//===========================================================================================================
//�Q�[���̍X�V����
//===========================================================================================================
void UpdateGame(void)
{
	//�ϐ��錾
	PLAYER *pPlayer;

	pPlayer = GetPlayer();


	//�|�[�Y���j���[�̍X�V����
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == true)
	{//�|�[�Y�̍X�V����
		UpdatePause();
	}
	else
	{
		//�w�i�̍X�V
		UpdateBg();

		//�|���S���̍X�V����
		UpdatePlayer();

		//�e�̍X�V����
		UpdateBullet();

		//�G�̍X�V����
		UpdateEnemy();

		//�X�R�A�̍X�V����
		UpdateScore();

		//�����̍X�V����
		UpdateExplosion();

		//�G�t�F�N�g�̍X�V����
		UpdateEffect();

	}
	
	//�N���A���U���g��ʂ֔�ԏ���
	int nCntEnemy = GetCntEnemy();

	if (nCntEnemy == 0)
	{
		g_pResult = GAMECLAER_RESULT;
		SetFade(FADE_OUT, MODE_RESULT);
	}

	//�Q�[���I�[�o�[���U���g��ʂ̔�ԏ���
	if (pPlayer->nZan == 0)
	{
		g_pResult = GAMEOVER_RESULT;
		SetFade(FADE_OUT, MODE_RESULT);
	}
}

//===========================================================================================================
//�Q�[���̕`�揈��
//===========================================================================================================
void DrawGame(void)
{
	//�w�i�̕`�揈��
	DrawBg();

	//�|���S���̕`�揈��
	DrawPlayer();

	//�G�̕`�揈��
	DrawEnemy();

	//�e�̕`�揈��
	DrawBullet();

	//�X�R�A�̕`�揈��
	DrawScore();

	//�����̕`�揈��
	DrawExplosion();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	if (g_bPause == true)
	{
		//�|�[�Y�̕`�揈��
		DrawPause();
	}
}

//===========================================================================================================
//	���U���g
//===========================================================================================================
GAMEJUB GetGameResult(void)
{
	return g_pResult;
}

//===========================================================================================================
//	�|�[�Y����
//===========================================================================================================
void SetPause(bool bPause)
{
	//�|�[�Y�����ǂ���
	g_bPause = bPause;
}