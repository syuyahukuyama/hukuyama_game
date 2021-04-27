//===================================================================
//
// �v���C���[�̏����@[player.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "player.h"
#include "input.h"
#include "block.h"
#include "result.h"
#include "fade.h"
#include "effect.h"
#include "sound.h"

//==========================================================================================================
//�}�N����`
//==========================================================================================================
#define MOVE_DATA (10)
#define MOVE_Y (9.8f)																					//Y���̃W�����v

#define TEXTURE_X (4)																					//�v���C���[�̉���
#define TEXTURE_Y (2)																					//�v���C���[�̏c��

#define MAX_TEXTUER (4)																					//�e�N�X�`���̐�
#define MAX_BLOCK (256)																					//�u���b�N�̍ő吔

#define SCREEN_WIDTH_HALF (120.0f)																		//X���̈ʒu
#define SCREEN_HEIGHT_Y (700.0f)																		//Y���̈ʒu

#define VERTEX_X (18)																					//���S����[�܂ł�X�̋���
#define VERTEX_Y (54)																					//���S����[�܂ł�Y�̋���
#define MAX_ZAN (1)																						//�ő�c�@

#define VERTEX_CHAR_X (40)																				//�G�̈ʒu���WX
#define VERTEX_CHAR_Y (40)																				//�G�̈ʒu���WY

#define PLAY_ANIM	((1.0f / TEXTURE_X) * pPlayer->nPatternAnim)										//�e�N�X�`���̓���
#define UV_U		((1.0f / TEXTURE_X) + PLAY_ANIM)													//U�̒l
#define UV_V		(1.0f / TEXTURE_Y)																	//V�̒l

#define GRAVITY (0.35f)																					//�d��
//==========================================================================================================
//�O���[�o���ϐ�
//==========================================================================================================
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;																//�e�N�X�`���̃|���S��
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;														//�o�b�t�@�|���S��
int g_nCountAnim;																						//�A�j���[�V�����J�E���^�[
int g_nPatternAnim;																						//�A�j���[�V�����p�^�[��NO
int g_nCountAnimSe;																						//�A�j���[�V�����J�E���^�[�Z�J���h
int g_nPatternAnimSe;																					//�A�j���[�V�����p�^�[��NO�Z�J���h
D3DXVECTOR3 g_posPlayer;																				//�v���C���[�̈ʒu
D3DXVECTOR3 g_rotPlayer;																				//�v���C���[�̉�]�p
float g_flenghtPlayer;																					//�v���C���[�̑Ίp���̒���
float g_fAnglePlayer;																					//�v���C���[�̑Ίp���̊p�x
D3DXVECTOR3 g_movePlayer;																				//�v���C���[�̈ړ���
PLAYER g_aPlayer;																						//�v���C���[�̏��
PLAYERSTATE g_aPlayerState;																				//�v���C���[�̏�ԏ��
int nCounter;																							//�u���b�N�̓_��

//===========================================================================================================
//����������
//===========================================================================================================
HRESULT InitPlayer(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	PLAYER *pPlayer;

	pPlayer = GetPlayer();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\player001.png", &g_pTexturePlayer);

	//�v���C���[���̏�����
	g_aPlayer.pos = D3DXVECTOR3(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_Y, 0.0f);
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.posold = g_aPlayer.pos;
	g_aPlayer.nLife = 0;
	g_aPlayer.State = PLAYERSTATE_APPEAR;
	g_aPlayer.nCounterDisp = 0;
	g_aPlayer.nCounterState = 0;
	g_aPlayer.nZan = MAX_ZAN;
	g_aPlayer.bUse = true;
	g_aPlayer.fHeight = VERTEX_Y;
	g_aPlayer.fWidth = VERTEX_X;

	g_nCountAnim = 0;
	g_nPatternAnim = 0;

	g_nCountAnimSe = 0;
	g_nPatternAnimSe = 0;

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y , 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�|���S���̏����ʒu
	pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V * (g_nPatternAnimSe + 1.0f));
	pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, UV_V * g_nPatternAnimSe);
	pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V * (g_nPatternAnimSe + 1.0f));
	pVtx[3].tex = D3DXVECTOR2(UV_U, UV_V * g_nPatternAnimSe);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}


//===========================================================================================================
//�I������
//===========================================================================================================
void UninitPlayer(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//===========================================================================================================
//�X�V����
//===========================================================================================================
void UpdatePlayer(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;
	BLOCK *pBlock;

	pPlayer = GetPlayer();
	pBlock = GetBlock();
	nCounter++;

	pPlayer->posold = pPlayer->pos;

	//�v���C���[�̓o��ʒu
	if (pPlayer->bReset == true)
	{
		pPlayer->bReset = false;
		pPlayer->pos = D3DXVECTOR3(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_Y, 0.0f);
	}

	//�X�V����
	//g_nCountAnim++;			//�A�j���[�V�����̃J�E���^�[�X�V
	//if ((g_nCountAnim % 2) == 0)
	//{
	//	g_nPatternAnim = (g_nPatternAnim + 1) % 8;
	//}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (pPlayer->bUse == true)
	{
		//�A�j���[�V�����p�^�[���̍X�V
		pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V * (g_nPatternAnimSe + 1.0f));
		pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, UV_V * g_nPatternAnimSe);
		pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V * (g_nPatternAnimSe + 1.0f));
		pVtx[3].tex = D3DXVECTOR2(UV_U, UV_V * g_nPatternAnimSe);
	}

	//�|���S���̍��E
	if (GetKeyboardPress(DIK_LEFT) == true)
	{
		g_movePlayer.x += sinf(D3DX_PI / -2) * 1.2f;
		g_movePlayer.y += cosf(D3DX_PI / -2) * 1.2f;

		g_nPatternAnimSe = 1;

		//�J�E���^�[�A�b�v
		pPlayer->nCountAnim += 1;

		if ((pPlayer->nCountAnim % 3) == 0)
		{
			//�A�j���[�V�����p�^�[��NO���X�V
			pPlayer->nPatternAnim = (pPlayer->nPatternAnim + 1) % MAX_TEXTUER;
		}
	}

	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		g_movePlayer.x += sinf(D3DX_PI / 2) * 1.2f;
		g_movePlayer.y += cosf(D3DX_PI / 2) * 1.2f;

		g_nPatternAnimSe = 0;

		//�J�E���^�[�A�b�v
		pPlayer->nCountAnim += 1;

		if ((pPlayer->nCountAnim % 3) == 0)
		{
			//�A�j���[�V�����p�^�[��NO���X�V
			pPlayer->nPatternAnim = (pPlayer->nPatternAnim + 1) % MAX_TEXTUER;
		}
	}

	//�v���C���[�W�����v
	if (g_aPlayer.bJump == false)
	{
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			g_aPlayer.bJump = true;
			g_movePlayer.y -= MOVE_Y;
		}
	}
	//�d�͂̐ݒ�
	g_movePlayer.y += GRAVITY;

	//�ʒu�̍X�V
	g_aPlayer.pos.x += g_movePlayer.x;
	g_aPlayer.pos.y += g_movePlayer.y;

	//�ړ��ʂ̌���
	g_movePlayer.x += (0.0f - g_movePlayer.x) * 0.20f;

	//
	if (g_aPlayer.pos.x < 0,0)
	{
		g_aPlayer.pos.x = (0 + (MOVE_DATA / 2));
	}

	//��ʐ���
	if (g_aPlayer.pos.x - VERTEX_X < 0)
	{
		g_aPlayer.pos.x = VERTEX_X;
	}
	if (g_aPlayer.pos.x + VERTEX_X > SCREEN_WIDTH)
	{
		g_aPlayer.pos.x = SCREEN_WIDTH - VERTEX_X;
	}
	/*if (g_aPlayer.pos.y - VERTEX_Y < 0)
	{
		g_aPlayer.pos.y = VERTEX_Y;
	}*/

	if (pPlayer->bReset == false)
	{
		//�u���b�N�̓����蔻��
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
		{
			switch (pBlock->type)
			{
			case BLOCK_NORMAL:						//�ʏ�u���b�N
				if (pBlock->bUse == true)			//�u���b�N���g���Ă��邩
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_HIDDEN:						//�B���u���b�N
				if (pBlock->bUse == true)			//�u���b�N���g���Ă��邩
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						/*if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
							}
						}*/
						if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
								pBlock->type = BLOCK_NORMAL;
								PlaySound(SOUND_LABEL_SE_HIDDEN);
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						/*if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}*/
					}
				}
				break;

			case BLOCK_UNDERJUDGMENT:				//�����������蔻��̂���u���b�N
				if (pBlock->bUse == true)			//�u���b�N���g���Ă��邩
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
								PlaySound(SOUND_LABEL_SE_BLOCK);
							}
						}
					}
				}
				break;

			case BLOCK_UPJUDGMENT:					//�ゾ�������蔻��̂���u���b�N
				if (pBlock->bUse == true)			//�u���b�N���g���Ă��邩
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_HIDDENUNDER:					//�����Ȃ��������u���b�N
				if (pBlock->bUse == true)			//�u���b�N���g���Ă��邩
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
								PlaySound(SOUND_LABEL_SE_BLOCK);
								pBlock->type = BLOCK_NORMAL;
							}
						}
					}
				}
				break;

			case BLOCK_BREAK:						//����u���b�N
				if (pBlock->bUse == true)			//�u���b�N���g���Ă��邩
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
								pBlock->bUse = false;
								SetExplosion(pBlock->pos);
								PlaySound(SOUND_LABEL_SE_BLOCK);
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
								pBlock->bUse = false;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_NOJAMPBREAK:					//�W�����v�ł�������u���b�N
				if (pBlock->bUse == true)			//�u���b�N���g���Ă��邩
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pBlock->bUse = false;
								SetExplosion(pBlock->pos);
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_GOAL:						//�S�[���u���b�N
				if (pBlock->bUse == true)			//�u���b�N���g���Ă��邩
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
								SetFade(FADE_OUT, MODE_RESULT);
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_TSPRING:					//�V�o�l�u���b�N
				if (pBlock->bUse == true)			//�u���b�N���g���Ă��邩
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
								g_movePlayer.y -= 100.0f;
								SetFade(FADE_OUT, MODE_GAMETHIRD);
								PlaySound(SOUND_LABEL_SE_SPPLING);
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_FALL:						//������u���b�N
				if (pBlock->bUse == true)			//�u���b�N���g���Ă��邩
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_PIPE:						//�y��
				if (pBlock->bUse == true)			//�u���b�N���g���Ă��邩
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
								if (GetKeyboardTrigger(DIK_DOWN) == true)
								{
									pPlayer->bUse = false;
									SetFade(FADE_OUT, MODE_GAMESECOND);
									PlaySound(SOUND_LABEL_SE_ENTER);
								}
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_SPRING:
				if (pBlock->bUse == true)			//�u���b�N���g���Ă��邩
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
								g_movePlayer.y -= 100.0f;
								SetFade(FADE_OUT, MODE_GAME);
								PlaySound(SOUND_LABEL_SE_SPPLING);
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;

			case BLOCK_SOIL:
				if (pBlock->bUse == true)			//�u���b�N���g���Ă��邩
				{
					if (pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x &&
						pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth)
					{
						if (pPlayer->posold.y <= pBlock->pos.y)
						{
							if (pPlayer->pos.y > pBlock->pos.y)
							{
								pPlayer->pos.y = pBlock->pos.y;
								g_movePlayer.y = 0.0f;
								pPlayer->bJump = false;
							}
						}
						else if (pPlayer->posold.y - VERTEX_Y >= pBlock->pos.y + pBlock->fHeight)
						{
							if (pPlayer->pos.y - VERTEX_Y < pBlock->pos.y + pBlock->fHeight)
							{
								pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + VERTEX_Y;
								g_movePlayer.y = 0.0f;
							}
						}
					}
					if (pPlayer->pos.y > pBlock->pos.y &&
						pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						if (pPlayer->posold.x - VERTEX_X >= pBlock->pos.x + pBlock->fWidth)
						{
							if (pPlayer->pos.x - VERTEX_X < pBlock->pos.x + pBlock->fWidth)
							{
								pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
						else if (pPlayer->posold.x + VERTEX_X <= pBlock->pos.x)
						{
							if (pPlayer->pos.x + VERTEX_X > pBlock->pos.x)
							{
								pPlayer->pos.x = pBlock->pos.x - VERTEX_X;
								g_movePlayer.x = 0.0f;
							}
						}
					}
				}
				break;
			}
		}
	}

	pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y , 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

}

//===========================================================================================================
//�`�揈��
//===========================================================================================================
void DrawPlayer(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�;
	pDevice->SetTexture(0, g_pTexturePlayer);

	//�|���S���̕`��
	if (g_aPlayer.bUse == true)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//===========================================================================================================
//�v���C���[�̎擾
//===========================================================================================================
PLAYER *GetPlayer(void)
{
	return &g_aPlayer;
}

//===========================================================================================================
//�v���C���[�̏�Ԃ̎擾
//===========================================================================================================
PLAYERSTATE *GetPlayerState(void)
{
	return &g_aPlayerState;
}

//===========================================================================================================
//�G�Ƃ̓����蔻��
//===========================================================================================================
bool HitPlayer(int nDamege)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;

	pPlayer = &g_aPlayer;

	pPlayer->State = PLAYERSTATE_DAMAGE;

	pPlayer->nLife -= nDamege;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//�G�̗̑͂�0�ɂȂ���
	if (pPlayer->nLife <= 0)
	{
		/*pPlayer->nZan -= 1;
		pPlayer->bDisp = false;*/

		if (pPlayer->nZan >= 0)
		{
			pPlayer->State = PLAYERSTATE_WAIT;
		}
		else
		{
			pPlayer->State = PLAYERSTATE_DEATH;
		}

		return true;
	}
	else
	{
		pPlayer->State = PLAYERSTATE_DAMAGE;
		pPlayer->nCounterState = 3;

		//�G�̐F��ύX
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		return false;
	}
	pVtx += 4;

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}