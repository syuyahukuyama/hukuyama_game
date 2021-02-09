//===================================================================
//
// �E�C���h�E�̏����@[explosion.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "effect.h"
#include "block.h"
#include "sound.h"

//=================================================================================================
//�}�N����`
//=================================================================================================
#define VERTEX_CENTER_X	(120)																	//���S���W��X���W
#define VERTEX_CENTER_Y	(60)																	//���S���W��Y���W

#define MAX_EXPLOSION (256)																		//�����̐�

#define MAX_PATTERN (4)																			//�A�j���[�V�����p�^�[��NO.�̍ő吔
#define TEXTURE_X	(4)																			//�A�j���[�V�����p�^�[���̉��̍ő吔
#define TEXTURE_Y	(1)																			//�A�j���[�V�����p�^�[���̏c�̍ő吔

#define PLAY_ANIM	((1.0f / TEXTURE_X) * pExplosion->nPatternAnim)								//�e�N�X�`���̓���
#define UV_U		((1.0f / TEXTURE_X) + PLAY_ANIM)											//U�̒l
#define UV_V		(1.0f / TEXTURE_Y)															//V�̒l

//=================================================================================================
//�����̍\����
//=================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;																			//�ʒu
	int nCountAnim;																				//�A�j���[�V����
	int nPatternAnim;																			//�A�j���[�V�����p�^�[��NO
	bool bUse;																					//�g�p���Ă��邩�ǂ���
}EXPLOSION;

//=================================================================================================
//�O���[�o���ϐ�
//=================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;													//�����̃e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;												//�o�b�t�@��
EXPLOSION g_aExplosion[MAX_EXPLOSION];															//�����̏��

//=================================================================================================
//����������
//=================================================================================================
HRESULT InitExplosion(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\effect000.png", &g_pTextureExplosion);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].nCountAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//���_���W�̐ݒ�
		SetVertexExplosion(nCntExplosion);

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

		//�����̈ʒu
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.25f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f, 0.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
	return S_OK;
}

//=================================================================================================
//�I������
//=================================================================================================
void UninitExplosion(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntEnekyType = 0; nCntEnekyType < 3; nCntEnekyType++)
	{
		if (g_pTextureExplosion != NULL)
		{
			g_pTextureExplosion->Release();
			g_pTextureExplosion = NULL;
		}
	}
}

//=================================================================================================
//�X�V����
//=================================================================================================
void UpdateExplosion(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	EXPLOSION *pExplosion;

	pExplosion = &g_aExplosion[0];

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	//
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		//�����̍X�V
		if (pExplosion->bUse == true)			//�g���Ă��邩�ǂ���
		{
			//�J�E���^�[�A�b�v
			pExplosion->nCountAnim += 1;

			if ((pExplosion->nCountAnim % 5) == 0)
			{
				//�A�j���[�V�����p�^�[��NO���X�V
				pExplosion->nPatternAnim = (pExplosion->nPatternAnim + 1) % MAX_PATTERN;
			}

			//�A�j���[�V�����p�^�[���̍X�V
			pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V);
			pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V);
			pVtx[3].tex = D3DXVECTOR2(UV_U, 0.0f);

			//���_���W�̐ݒ�
			SetVertexExplosion(nCntExplosion);

			//�����G�t�F�N�g�̍폜
			if (pExplosion->nPatternAnim == 3)
			{
				pExplosion->nPatternAnim = 0;
				pExplosion->bUse = false;
			}
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

//=================================================================================================
//�`�揈��
//=================================================================================================
void DrawExplosion(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	//�|���S���̕`��
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

//=================================================================================================
//�����̐ݒ�
//=================================================================================================
void SetExplosion(D3DXVECTOR3 pos)
{
	//�ϐ��錾
	EXPLOSION *pExplosion;

	pExplosion = &g_aExplosion[0];

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == false)
		{
			//�ʒu�̐ݒ�
			pExplosion->pos = pos;

			//���_���W�̐ݒ�
			SetVertexExplosion(nCntExplosion);

			//�G�̃^�C�v
			pExplosion->bUse = true;
			break;
		}
	}
}

//=================================================================================================
//�����̒��_���W
//=================================================================================================
void SetVertexExplosion(int nIdx)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	//�e�̏����ڂ�
	pVtx += nIdx * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x - VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x - VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y - VERTEX_CENTER_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x + VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x + VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y - VERTEX_CENTER_Y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}