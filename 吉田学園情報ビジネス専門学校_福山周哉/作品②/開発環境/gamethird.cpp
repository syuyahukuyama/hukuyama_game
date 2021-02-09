//===================================================================
//
// �Q�[����ʂ̏����@[game.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "gamethird.h"
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
#include "coin.h"

//===========================================================================================================
//	�}�N����`
//===========================================================================================================
#define MAX_LINE (2)																					//�G�̉���
#define MAX_POS_Y  (SCREEN_WIDTH / 3)																	//�G��Y���W�̈ʒu
#define MAX_POS_X  (280.0f)																				//�G��X���W�̈ʒu

#define SCREEN_WIDTH_HALF (120.0f)																		//X���̈ʒu
#define SCREEN_HEIGHT_Y (700.0f)																		//Y���̈ʒu

#define VERTEX_Y (54)																					//���S����[�܂ł�Y�̋���

//===========================================================================================================
//	�O���[�o���ϐ�
//===========================================================================================================
//bool g_bPause = false;																					//�|�[�Y�����ǂ���
//GAMEJUB g_pResult = GAMECLAER_RESULT;																	//�Q�[�����U���g
//int g_nCount;																							//�X�e�[�W�J�E���g

//===========================================================================================================
//	�Q�[���̏���������
//===========================================================================================================
HRESULT InitThirdGame(void)
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
	PlaySound(SOUND_LABEL_BGM009);

	//�ʏ�u���b�N
	SetBlock(D3DXVECTOR3(100, 765, 0.0f), 80, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 2.0f, 1.0f);
	SetBlock(D3DXVECTOR3(140, 45, 0.0f), 40, 720, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 18.0f);
	SetBlock(D3DXVECTOR3(100, 405, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(140, 45, 0.0f), 1000, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 25.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1100, 45, 0.0f), 40, 640, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 16.0f);
	SetBlock(D3DXVECTOR3(280, 765, 0.0f), 860, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 21.0f, 1.0f);
	SetBlock(D3DXVECTOR3(280, 155, 0.0f), 40, 520, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 13.0f);
	SetBlock(D3DXVECTOR3(1100, 45, 0.0f), 40, 640, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 16.0f);
	SetBlock(D3DXVECTOR3(280, 645, 0.0f), 460, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 12.0f, 1.0f);
	SetBlock(D3DXVECTOR3(820, 645, 0.0f), 120, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 3.0f, 1.0f);
	SetBlock(D3DXVECTOR3(980, 155, 0.0f), 40, 400, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 10.0f);
	SetBlock(D3DXVECTOR3(280, 155, 0.0f), 720, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 18.0f, 1.0f);
	SetBlock(D3DXVECTOR3(740, 645, 0.0f), 80, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 2.0f, 1.0f);

	SetBlock(D3DXVECTOR3(420, 270, 0.0f), 40, 240, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 6.0f);
	SetBlock(D3DXVECTOR3(860, 270, 0.0f), 40, 240, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 6.0f);
	SetBlock(D3DXVECTOR3(530, 270, 0.0f), 40, 240, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 6.0f);
	SetBlock(D3DXVECTOR3(640, 270, 0.0f), 40, 240, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 6.0f);
	SetBlock(D3DXVECTOR3(750, 270, 0.0f), 40, 240, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 6.0f);

	//�B���u���b�N
	SetBlock(D3DXVECTOR3(60, 640, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(20, 510, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(100, 285, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(10, 165, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(180, 280, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(240, 410, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(150, 540, 0.0f), 80, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(260, 670, 0.0f), 40, 20, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(940, 645, 0.0f), 160, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 4.0f, 1.0f);

	SetBlock(D3DXVECTOR3(320, 520, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(380, 390, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	//�����蔻��Ȃ��u���b�N
	SetBlock(D3DXVECTOR3(1100, 685, 0.0f), 40, 80, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 2.0f);

	//�ゾ������u���b�N
	SetBlock(D3DXVECTOR3(460, 430, 0.0f), 200, 40, BLOCK_UPJUDGMENT, BLOCKTEX_STONE, GAMESTAGE_ONE, 5.0f, 1.0f);
	SetBlock(D3DXVECTOR3(750, 430, 0.0f), 120, 40, BLOCK_UPJUDGMENT, BLOCKTEX_STONE, GAMESTAGE_ONE, 3.0f, 1.0f);

	//�S�[���u���b�N
	SetBlock(D3DXVECTOR3(460, 470, 0.0f), 400, 40, BLOCK_GOAL, BLOCKTEX_GOAL, GAMESTAGE_ONE, 10.0f, 1.0f);

	return S_OK;
}

//===========================================================================================================
//	�Q�[���̏I������
//===========================================================================================================
void UninitThirdGame(void)
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
void UpdateThirdGame(void)
{
	//�ϐ��錾
	PLAYER *pPlayer;

	pPlayer = GetPlayer();

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
	if (GetKeyboardTrigger(DIK_L))
	{
		SetFade(FADE_OUT, MODE_GAMETHIRD);
	}

	if (pPlayer->pos.y > SCREEN_HEIGHT - VERTEX_Y + 100)
	{
		pPlayer->bReset = true;
		pPlayer->pos = D3DXVECTOR3(60, SCREEN_HEIGHT_Y, 0.0f);

		SetFade(FADE_OUT, MODE_GAMETHIRD);
		PlaySound(SOUND_LABEL_SE_AHIRU);
	}

	if (GetKeyboardTrigger(DIK_S))
	{
		SetFade(FADE_OUT, MODE_SELECT);
	}

}

//===========================================================================================================
//	�Q�[���̕`�揈��
//===========================================================================================================
void DrawThirdGame(void)
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
}