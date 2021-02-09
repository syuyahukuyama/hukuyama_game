//===================================================================
//
// �E�C���h�E�̏����@[explosion.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "boss.h"
#include "enemy.h"
#include "sound.h"

//=================================================================================================
//�}�N����`
//=================================================================================================
#define VERTEX_CENTER_X	(30)																	//���S���W��X���W
#define VERTEX_CENTER_Y	(30)																	//���S���W��Y���W

#define MAX_BOSSENEMY (1)																		//�{�X�̐�

#define MAX_PATTERN (3)																			//�A�j���[�V�����p�^�[��NO.�̍ő吔
#define TEXTURE_X	(8)																			//�A�j���[�V�����p�^�[���̉��̍ő吔
#define TEXTURE_Y	(1)																			//�A�j���[�V�����p�^�[���̏c�̍ő吔

#define PLAY_ANIM	((1.0f / TEXTURE_X) * pBossEnemy->nPatternAnim)								//�e�N�X�`���̓���
#define UV_U		((1.0f / TEXTURE_X) + PLAY_ANIM)											//U�̒l
#define UV_V		(1.0f / TEXTURE_Y)															//V�̒l

//=================================================================================================
//�{�X�̍\����
//=================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;																			//�ʒu
	int nCountAnim;																				//�A�j���[�V����
	int nPatternAnim;																			//�A�j���[�V�����p�^�[��NO
	bool bUse;																					//�g�p���Ă��邩�ǂ���
}BOSSENEMY;

//=================================================================================================
//�O���[�o���ϐ�
//=================================================================================================
LPDIRECT3DTEXTURE9 g_apTextureBossEnemy = NULL;													//�����̃e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBossEnemy = NULL;												//�o�b�t�@��
BOSSENEMY g_aBossEnemy;																			//�{�X�̏��

//=================================================================================================
//����������
//=================================================================================================
HRESULT InitBossEnemy(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\boss001.png", &g_apTextureBossEnemy);

	//�{�X�̏�����
	for (int nCntBossEnemy = 0; nCntBossEnemy < MAX_BOSSENEMY; nCntBossEnemy++)
	{
		g_aBossEnemy.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBossEnemy.nCountAnim = 0;
		g_aBossEnemy.nPatternAnim = 0;
		g_aBossEnemy.bUse = false;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BOSSENEMY, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBossEnemy, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBossEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBossEnemy = 0; nCntBossEnemy < MAX_BOSSENEMY; nCntBossEnemy++)
	{
		//���_���W�̐ݒ�
		SetVertexBossEnemy(nCntBossEnemy);

		//rhm�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�G�̈ʒu
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 0.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBossEnemy->Unlock();
	return S_OK;
}

//=================================================================================================
//�I������
//=================================================================================================
void UninitBossEnemy(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBossEnemy != NULL)
	{
		g_pVtxBuffBossEnemy->Release();
		g_pVtxBuffBossEnemy = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntEnekyType = 0; nCntEnekyType < 1; nCntEnekyType++)
	{
		if (g_pVtxBuffBossEnemy != NULL)
		{
			g_pVtxBuffBossEnemy->Release();
			g_pVtxBuffBossEnemy = NULL;
		}
	}
}

//=================================================================================================
//�X�V����
//=================================================================================================
void UpdateBossEnemy(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	BOSSENEMY *pBossEnemy;

	pBossEnemy = &g_aBossEnemy;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBossEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//
	for (int nCntBossEnemy = 0; nCntBossEnemy < MAX_BOSSENEMY; nCntBossEnemy++, pBossEnemy++)
	{
		//�����̍X�V
		if (pBossEnemy->bUse == true)			//�g���Ă��邩�ǂ���
		{
			//�J�E���^�[�A�b�v
			pBossEnemy->nCountAnim += 1;

			if ((pBossEnemy->nCountAnim % 3) == 0)
			{
				//�A�j���[�V�����p�^�[��NO���X�V
				pBossEnemy->nPatternAnim = (pBossEnemy->nPatternAnim + 1) % MAX_PATTERN;
			}

			//�A�j���[�V�����p�^�[���̍X�V
			pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V);
			pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V);
			pVtx[3].tex = D3DXVECTOR2(UV_U, 0.0f);
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBossEnemy->Unlock();
}

//=================================================================================================
//�`�揈��
//=================================================================================================
void DrawBossEnemy(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBossEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureBossEnemy);

	//�|���S���̕`��
	for (int nCntBossEnemy = 0; nCntBossEnemy < MAX_BOSSENEMY; nCntBossEnemy++)
	{
		if (g_aBossEnemy.bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBossEnemy * 4, 2);
		}
	}
}

//=================================================================================================
//�����̐ݒ�
//=================================================================================================
void SetBossEnemy(D3DXVECTOR3 pos)
{
	//�ϐ��錾
	BOSSENEMY *pBossEnemy;

	pBossEnemy = &g_aBossEnemy;

	for (int nCntBossEnemy = 0; nCntBossEnemy < MAX_BOSSENEMY; nCntBossEnemy++, pBossEnemy++)
	{
		if (pBossEnemy->bUse == false)
		{
			//�ʒu�̐ݒ�
			pBossEnemy->pos = pos;

			//���_���W�̐ݒ�
			SetVertexBossEnemy(nCntBossEnemy);

			//�G�̃^�C�v
			pBossEnemy->bUse = true;
			break;
		}
	}
}

//=================================================================================================
//�����̒��_���W
//=================================================================================================
void SetVertexBossEnemy(int nIdx)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBossEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//�e�̏����ڂ�
	pVtx += nIdx * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aBossEnemy.pos.x - VERTEX_CENTER_X, g_aBossEnemy.pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBossEnemy.pos.x - VERTEX_CENTER_X, g_aBossEnemy.pos.y - VERTEX_CENTER_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBossEnemy.pos.x + VERTEX_CENTER_X, g_aBossEnemy.pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBossEnemy.pos.x + VERTEX_CENTER_X, g_aBossEnemy.pos.y - VERTEX_CENTER_Y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBossEnemy->Unlock();
}
