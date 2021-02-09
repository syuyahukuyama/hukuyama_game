//===================================================================
//
// �Q�[����ʂ̏����@[game.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "gameselect.h"
#include "main.h"
#include "player.h"
#include "bg.h"
#include "input.h"
#include "pause.h"
#include "block.h"
#include "fade.h"
#include "result.h"
#include "effect.h"
#include "sound.h"

//===========================================================================================================
//	�}�N����`
//===========================================================================================================
#define MAX_LINE (2)																					//�G�̉���
#define MAX_POS_Y  (SCREEN_WIDTH / 3)																	//�G��Y���W�̈ʒu
#define MAX_POS_X  (280.0f)																				//�G��X���W�̈ʒu

//===========================================================================================================
//	�O���[�o���ϐ�
//===========================================================================================================
//bool g_bPause = false;																					//�|�[�Y�����ǂ���
//GAMEJUB g_pResult = GAMECLAER_RESULT;																	//�Q�[�����U���g
//int g_nCount;																							//�X�e�[�W�J�E���g

//===========================================================================================================
//	�Q�[���̏���������
//===========================================================================================================
HRESULT InitSelectGame(void)
{
	//�w�i�̏�����
	InitBg();

	//�v���C���[�̏���������
	InitPlayer();

	//�u���b�N�̏���������
	InitBlock();

	//�����̏���������
	InitExplosion();

	//�|�[�Y�̏���������
	//InitPause();

	//����
	//PlaySound(SOUND_LABEL_BGM009);

	//�u���b�N�̔z�u
	SetBlock(D3DXVECTOR3(0,740, 0), 1320, 80, BLOCK_SOIL, BLOCKTEX_SOIL, GAMESTAGE_SELECT, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(420, 700, 0), 400, 40, BLOCK_SOIL, BLOCKTEX_SOIL, GAMESTAGE_SELECT, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(460, 660, 0), 320, 40, BLOCK_SOIL, BLOCKTEX_SOIL, GAMESTAGE_SELECT, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(500, 580, 0), 240, 80, BLOCK_SOIL, BLOCKTEX_SOIL, GAMESTAGE_SELECT, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(100, 500, 0), 160, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_SELECT, 4.0f, 1.0f);

	SetBlock(D3DXVECTOR3(260, 540, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_SELECT, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(300, 580, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_SELECT, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(1000, 500, 0), 160, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_SELECT, 4.0f, 1.0f);
	SetBlock(D3DXVECTOR3(960, 540, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_SELECT, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(920, 580, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_SELECT, 1.0f, 1.0f);

	//�o�l�u���b�N
	SetBlock(D3DXVECTOR3(140, 440, 0), 60, 60, BLOCK_SPRING, BLOCKTEX_SPRING, GAMESTAGE_SELECT, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(1040, 440, 0), 60, 60, BLOCK_TSPRING, BLOCKTEX_SPRING, GAMESTAGE_SELECT, 1.0f, 1.0f);

	//�y��
	SetBlock(D3DXVECTOR3(580, 510, 0), 70, 70, BLOCK_PIPE, BLOCKTEX_PIPE, GAMESTAGE_SELECT, 1.0f, 1.0f);

	return S_OK;
}

//===========================================================================================================
//	�Q�[���̏I������
//===========================================================================================================
void UninitSelectGame(void)
{
	//�T�E���h���ꎞ��~
	StopSound();

	//�u���b�N�̏I������
	UninitBlock();

	//�v���C���[�̏I������
	UninitPlayer();

	//�����̏I������
	UninitExplosion();

	//�w�i�̏I������
	UninitBg();

	//�|�[�Y�̏I������
	//UninitPause();
}

//===========================================================================================================
//	�Q�[���̍X�V����
//===========================================================================================================
void UpdateSelectGame(void)
{
	//�ϐ��錾
	PLAYER *pPlayer;
	BLOCK *pBlock;

	pPlayer = GetPlayer();
	pBlock = GetBlock();

	//�|�[�Y���j���[�̍X�V����
	/*if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
	}*/

	//if (g_bPause == true)
	//{//�|�[�Y�̍X�V����
	// //UpdatePause();
	//}
		//�w�i�̍X�V
		UpdateBg();

		//�v���C���[�̍X�V����
		UpdatePlayer();

		//�u���b�N�̏I������
		UpdateBlock();

		//�����̍X�V����
		UpdateExplosion();

	//�N���A���U���g��ʂ֔�ԏ���
	//int nCntEnemy = GetCntEnemy();

	/*if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
	g_pResult = GAMECLAER_RESULT;
	SetFade(FADE_OUT, MODE_RESULT);
	}*/

	////�Q�[���I�[�o�[���U���g��ʂ̔�ԏ���
	//if (pPlayer->nZan == 0)
	//{
	//	g_pResult = GAMEOVER_RESULT;
	//	SetFade(FADE_OUT, MODE_RESULT);
	//}
		switch (pBlock->type)
		{
		case BLOCK_SPRING:
			SetFade(FADE_OUT, MODE_GAME);
			break;
		}


}

//===========================================================================================================
//	�Q�[���̕`�揈��
//===========================================================================================================
void DrawSelectGame(void)
{
	//�w�i�̕`�揈��
	DrawBg();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�u���b�N�̏I������
	DrawBlock();

	//�����̕`�揈��
	DrawExplosion();

	//�G�̕`�揈��
	//DrawEnemy();

	/*if (g_bPause == true)
	{
		�|�[�Y�̕`�揈��
		DrawPause();
	}*/
}