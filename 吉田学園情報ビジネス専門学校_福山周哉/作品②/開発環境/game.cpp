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
bool g_bPause = false;																					//�|�[�Y�����ǂ���
GAMEJUB g_pResult = GAMECLAER_RESULT;																	//�Q�[�����U���g
int g_nCount;																							//�X�e�[�W�J�E���g

//===========================================================================================================
//	�Q�[���̏���������
//===========================================================================================================
HRESULT InitGame(void)
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
	PlaySound(SOUND_LABEL_BGM011);

	//�ʏ�u���b�N
	SetBlock(D3DXVECTOR3(700, 485, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(660, 485, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(620, 485, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(580, 445, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(540, 445, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(500, 445, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(460, 445, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(180, 405, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(460, 485, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(460, 405, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(360, 765, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(320, 765, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(300, 765, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(100, 765, 0.0f), 120, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 3.0f, 1.0f);
	SetBlock(D3DXVECTOR3(60, 525, 0.0f), 40, 280, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 7.0f);
	SetBlock(D3DXVECTOR3(100, 525, 0.0f), 360, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 9.0f, 1.0f);

	SetBlock(D3DXVECTOR3(840, 735, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(820, 735, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(960, 615, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1000, 575, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1040, 535, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(180, 180, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(220, 180, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(440, 220, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(480, 220, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(520, 220, 0.0f), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(900, 295, 0.0f), 100, 15, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 2.0f, 1.0f);

	SetBlock(D3DXVECTOR3(20, 285, 0.0f), 100, 15, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_ONE, 2.0f, 1.0f);

	//�B���u���b�N
	SetBlock(D3DXVECTOR3(420, 640, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(820, 580, 0.0f), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	//�����蔻��Ȃ��u���b�N
	SetBlock(D3DXVECTOR3(550, 765, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(590, 765, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(630, 765, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(880, 695, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(920, 655, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(1080, 500, 0.0f), 200, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 5.0f, 1.0f);

	SetBlock(D3DXVECTOR3(220, 485, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(220, 445, 0.0f), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_ONE, 1.0f, 1.0f);

	//�������̓����蔻��u���b�N
	SetBlock(D3DXVECTOR3(0, 405, 0.0f), 480, 40, BLOCK_UNDERJUDGMENT, BLOCKTEX_STONE, GAMESTAGE_ONE, 12.0f, 1.0f);

	//�S�[���u���b�N
	SetBlock(D3DXVECTOR3(1160, 255, 0.0f), 40, 40, BLOCK_GOAL, BLOCKTEX_GOAL, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1200, 255, 0.0f), 40, 40, BLOCK_GOAL, BLOCKTEX_GOAL, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1240, 255, 0.0f), 40, 40, BLOCK_GOAL, BLOCKTEX_GOAL, GAMESTAGE_ONE, 1.0f, 1.0f);

	//�W�����v�ł�������u���b�N
	SetBlock(D3DXVECTOR3(660, 285, 0.0f), 130, 15, BLOCK_NOJAMPBREAK, BLOCKTEX_STONE, GAMESTAGE_ONE, 3.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1080, 215, 0.0f), 80, 40, BLOCK_NOJAMPBREAK, BLOCKTEX_STONE, GAMESTAGE_ONE, 2.0f, 1.0f);

	//���Ӗ��
	SetBlock(D3DXVECTOR3(1040, 540, 0.0f), 200, 150, BLOCK_ARROW, BLOCKTEX_ARROW, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1080, 540, 0.0f), 200, 150, BLOCK_ARROW, BLOCKTEX_ARROW, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1120, 540, 0.0f), 200, 150, BLOCK_ARROW, BLOCKTEX_ARROW, GAMESTAGE_ONE, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(-60, 540, 0.0f), 200, 150, BLOCK_ARROW, BLOCKTEX_ARROW, GAMESTAGE_ONE, 1.0f, 1.0f);

	return S_OK;
}

//===========================================================================================================
//	�Q�[���̏I������
//===========================================================================================================
void UninitGame(void)
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
void UpdateGame(void)
{
	//�ϐ��錾
	PLAYER *pPlayer;

	pPlayer = GetPlayer();
	g_nCount++;

	//�|�[�Y���j���[�̍X�V����
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == true)
	{//�|�[�Y�̍X�V����
		//UpdatePause();
	}
	else
	{
		//�w�i�̍X�V
		UpdateBg();

		//�v���C���[�̍X�V����
		UpdatePlayer();

		//�u���b�N�̏I������
		UpdateBlock();

		//�����̍X�V����
		UpdateExplosion();
	}

	if (GetKeyboardTrigger(DIK_S))
	{
		SetFade(FADE_OUT, MODE_SELECT);
	}

	if (pPlayer->pos.y > SCREEN_HEIGHT - VERTEX_Y + 100)
	{
		pPlayer->bReset = true;
		pPlayer->pos = D3DXVECTOR3(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_Y, 0.0f);

		SetFade(FADE_OUT, MODE_GAME);
		PlaySound(SOUND_LABEL_SE_AHIRU);
	}

}

//===========================================================================================================
//	�Q�[���̕`�揈��
//===========================================================================================================
void DrawGame(void)
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

	if (g_bPause == true)
	{
		//�|�[�Y�̕`�揈��
		//DrawPause();
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