//===================================================================
//
// �E�C���h�E�̏����@[explosion.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "effect.h"
#include "enemy.h"
#include "sound.h"

//=================================================================================================
//�}�N����`
//=================================================================================================
#define VERTEX_CENTER_X	(30)																	//���S���W��X���W
#define VERTEX_CENTER_Y	(30)																	//���S���W��Y���W

#define MAX_EFFECT (10000)																		//�����̐�

#define MAX_PATTERN (8)																			//�A�j���[�V�����p�^�[��NO.�̍ő吔
#define TEXTURE_X	(8)																			//�A�j���[�V�����p�^�[���̉��̍ő吔
#define TEXTURE_Y	(1)																			//�A�j���[�V�����p�^�[���̏c�̍ő吔

#define PLAY_ANIM	((1.0f / TEXTURE_X) * pEffect->nPatternAnim)								//�e�N�X�`���̓���
#define UV_U		((1.0f / TEXTURE_X) + PLAY_ANIM)											//U�̒l
#define UV_V		(1.0f / TEXTURE_Y)															//V�̒l

//=================================================================================================
//�����̍\����
//=================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;																			//�ʒu
	int nLife;																					//����
	bool bUse;																					//�g�p���Ă��邩�ǂ���
}EFFECT;

//=================================================================================================
//�O���[�o���ϐ�
//=================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;													//�����̃e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;											//�o�b�t�@��
EFFECT g_aEffect[MAX_EFFECT];																//�����̏��

//=================================================================================================
//����������
//=================================================================================================
HRESULT InitEffect(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\effect000.jpg", &g_pTextureEffect);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//���_���W�̐ݒ�
		SetVertexEffect(nCntEffect);

		//rhm�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);

		//�G�̈ʒu
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect->Unlock();
	return S_OK;
}

//=================================================================================================
//�I������
//=================================================================================================
void UninitEffect(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntEnekyType = 0; nCntEnekyType < 3; nCntEnekyType++)
	{
		if (g_pTextureEffect != NULL)
		{
			g_pTextureEffect->Release();
			g_pTextureEffect = NULL;
		}
	}
}

//=================================================================================================
//�X�V����
//=================================================================================================
void UpdateEffect(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	EFFECT *pEffect;

	pEffect = &g_aEffect[0];

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	//
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		//�G�t�F�N�g�̍X�V
		if (pEffect->bUse == true)			//�g���Ă��邩�ǂ���
		{
			//�����`�F�b�N
			pEffect->nLife--;
			
			if (pEffect->nLife <= 0)
			{
				pEffect->bUse = false;
			}
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//=================================================================================================
//�`�揈��
//=================================================================================================
void DrawEffect(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//���Z�\��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEffect);

	//�|���S���̕`��
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	//���ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


//=================================================================================================
//�����̐ݒ�
//=================================================================================================
void SetEffect(D3DXVECTOR3 pos)
{
	//�ϐ��錾
	EFFECT *pEffect;

	pEffect = &g_aEffect[0];

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == false)
		{
			//�ʒu�̐ݒ�
			pEffect->pos = pos;

			//���_���W�̐ݒ�
			SetVertexEffect(nCntEffect);
			pEffect->nLife = 30;

			//�G�̃^�C�v
			pEffect->bUse = true;
			break;
		}
	}
}

//=================================================================================================
//�����̒��_���W
//=================================================================================================
void SetVertexEffect(int nIdx)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	//�e�̏����ڂ�
	pVtx += nIdx * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - VERTEX_CENTER_X, g_aEffect[nIdx].pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - VERTEX_CENTER_X, g_aEffect[nIdx].pos.y - VERTEX_CENTER_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + VERTEX_CENTER_X, g_aEffect[nIdx].pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + VERTEX_CENTER_X, g_aEffect[nIdx].pos.y - VERTEX_CENTER_Y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}
