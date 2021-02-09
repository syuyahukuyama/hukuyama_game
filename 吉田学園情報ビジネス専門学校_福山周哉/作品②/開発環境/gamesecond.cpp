//===================================================================
//
// �Q�[����ʂ̏����@[game.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "gamesecond.h"
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

#define SCREEN_WIDTH_HALF (120.0f)																		//X���̈ʒu
#define SCREEN_HEIGHT_Y (700.0f)																		//Y���̈ʒu

#define VERTEX_Y (54)																					//���S����[�܂ł�Y�̋���

//===========================================================================================================
//	�O���[�o���ϐ�
//===========================================================================================================
//bool g_bPause = false;																				//�|�[�Y�����ǂ���
//GAMEJUB g_pResult = GAMECLAER_RESULT;																	//�Q�[�����U���g
//int g_nCount;																							//�X�e�[�W�J�E���g

//===========================================================================================================
//	�Z�J���h�Q�[���̏���������
//===========================================================================================================
HRESULT InitSecondGame(void)
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
	PlaySound(SOUND_LABEL_BGM010);

	//�ʏ�u���b�N
	SetBlock(D3DXVECTOR3(300, 770, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(380, 510, 0), 40, 200, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 5.0f);

	SetBlock(D3DXVECTOR3(540, 510, 0), 40, 200, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 5.0f);

	SetBlock(D3DXVECTOR3(740, 530, 0), 40, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(780, 250, 0), 40, 440, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 11.0f);

	SetBlock(D3DXVECTOR3(1240, 30, 0), 40, 800, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 20.0f);

	SetBlock(D3DXVECTOR3(160, 30, 0), 1000, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 25.0f, 1.0f);

	SetBlock(D3DXVECTOR3(780, 770, 0), 480, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 12.0f, 1.0f);

	SetBlock(D3DXVECTOR3(800, 640, 0), 360, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 9.0f, 1.0f);

	SetBlock(D3DXVECTOR3(880, 510, 0), 360, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 9.0f, 1.0f);

	SetBlock(D3DXVECTOR3(800, 380, 0), 360, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 9.0f, 1.0f);

	SetBlock(D3DXVECTOR3(880, 250, 0), 360, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 9.0f, 1.0f);

	SetBlock(D3DXVECTOR3(500, 270, 0), 200, 40, BLOCK_NORMAL, BLOCKTEX_STONE, GAMESTAGE_TWO, 4.0f, 1.0f);

	//�B���u���b�N
	SetBlock(D3DXVECTOR3(340, 640, 0), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(1200, 140, 0), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(430, 410, 0), 40, 40, BLOCK_HIDDEN, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);

	//����u���b�N
	SetBlock(D3DXVECTOR3(100, 770, 0), 40, 40, BLOCK_BREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(140, 770, 0), 40, 40, BLOCK_BREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(180, 770, 0), 40, 40, BLOCK_BREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);

	//�����蔻��Ȃ��u���b�N
	SetBlock(D3DXVECTOR3(220, 770, 0), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(260, 770, 0), 40, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);

	SetBlock(D3DXVECTOR3(420, 510, 0), 120, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_TWO, 3.0f, 1.0f);

	SetBlock(D3DXVECTOR3(250, 200, 0), 120, 40, BLOCK_NOTCOLLISION, BLOCKTEX_STONE, GAMESTAGE_TWO, 3.0f, 1.0f);

	//�W�����v�ł�������u���b�N
	SetBlock(D3DXVECTOR3(580, 510, 0), 40, 40, BLOCK_NOJAMPBREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(620, 515, 0), 40, 40, BLOCK_NOJAMPBREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(660, 520, 0), 40, 40, BLOCK_NOJAMPBREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(700, 525, 0), 40, 40, BLOCK_NOJAMPBREAK, BLOCKTEX_STONE, GAMESTAGE_TWO, 1.0f, 1.0f);

	//���Ӗ��
	SetBlock(D3DXVECTOR3(390, 560, 0.0f), 200, 150, BLOCK_ARROW, BLOCKTEX_ARROW, GAMESTAGE_ONE, 1.0f, 1.0f);

	//�S�[���u���b�N
	SetBlock(D3DXVECTOR3(0, 190, 0), 120, 40, BLOCK_GOAL, BLOCKTEX_GOAL, GAMESTAGE_TWO, 3.0f, 1.0f);

	return S_OK;
}

//===========================================================================================================
//	�Z�J���h�Q�[���̏I������
//===========================================================================================================
void UninitSecondGame(void)
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
//	�Z�J���h�Q�[���̍X�V����
//===========================================================================================================
void UpdateSecondGame(void)
{
	//�ϐ��錾
	PLAYER *pPlayer;

	pPlayer = GetPlayer();
	
	//�|�[�Y���j���[�̍X�V����
	/*if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == true)*/
	{//�|�[�Y�̍X�V����
	 //UpdatePause();
	}
		//�w�i�̍X�V
		UpdateBg();

		//�v���C���[�̍X�V����
		UpdatePlayer();

		//�u���b�N�̏I������
		UpdateBlock();

		//�����̍X�V����
		UpdateExplosion();

	if (pPlayer->pos.y > SCREEN_HEIGHT - VERTEX_Y + 100)
	{
		pPlayer->bReset = true;
		pPlayer->pos = D3DXVECTOR3(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_Y, 0.0f);

		SetFade(FADE_OUT, MODE_GAMESECOND);
		PlaySound(SOUND_LABEL_SE_AHIRU);
	}

	if (GetKeyboardTrigger(DIK_S))
	{
		SetFade(FADE_OUT, MODE_SELECT);
	}
}

//===========================================================================================================
//	�Z�J���h�Q�[���̕`�揈��
//===========================================================================================================
void DrawSecondGame(void)
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

	//if (g_bPause == true)
	//{
	//	//�|�[�Y�̕`�揈��
	//	//DrawPause();
	//}
}