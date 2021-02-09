//===================================================================
//
// �X�R�A�̏����@[score.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "score.h"

//=================================================================================================
//�}�N����`
//=================================================================================================
#define TEXTURE_X (10)		//�e�N�X�`����X���W
#define TEXTURE_Y (1)		//�e�N�X�`����Y���W

#define MAX_SCORE (8)		//�X�R�A�̎��

//=================================================================================================
//�O���[�o���ϐ�
//=================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureScore[MAX_SCORE] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;
D3DXVECTOR3 g_posScore;								//�X�R�A�̈ʒu
D3DXVECTOR3 g_rotScore;								//�X�R�A�̉�]�p
float g_flenghtScore;								//�X�R�A�̑Ίp���̒���
float g_fAngleScore;								//�X�R�A�̑Ίp���̊p�x
int g_nScore;										//�X�R�A

//=================================================================================================
//����������
//=================================================================================================
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();
	g_nScore = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureScore[0]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffScore, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//�X�R�A�̈ʒu
		pVtx[0].pos = D3DXVECTOR3(850.0f + 50.0f * nCntScore, 1000.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(850.0f + 50.0f * nCntScore, 960.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(900.0f + 50.0f * nCntScore, 1000.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(900.0f + 50.0f * nCntScore, 960.0f, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f , 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f , 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / 10, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / 10, 0.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
	return S_OK;
}

//=================================================================================================
//�I������
//=================================================================================================
void UninitScore(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntEnekyType = 0; nCntEnekyType < 3; nCntEnekyType++)
	{
		if (g_pTextureScore[nCntEnekyType] != NULL)
		{
			g_pTextureScore[nCntEnekyType]->Release();
			g_pTextureScore[nCntEnekyType] = NULL;
		}
	}
}

//=================================================================================================
//�X�V����
//=================================================================================================
void UpdateScore(void)
{
}

//=================================================================================================
//�`�揈��
//=================================================================================================
void DrawScore(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�|���S���̕`��
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureScore[0]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//=================================================================================================
//�X�R�A�̉��Z
//=================================================================================================
void AddScore(int nValue)
{
	//�ϐ��錾
	int aNumber[8];			//�X�R�A�����[
	g_nScore += nValue;

	//�\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//�X�R�A�����[
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//����������[
		aNumber[nCntScore] = (g_nScore / powf(10, MAX_SCORE - (nCntScore + 1)));

		//�e�N�X�`���̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 0.0f);

		//�X�R�A�̏������Ɉڂ�
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}