//===================================================================
//
// �R�C���̏����@[coin.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "coin.h"

//===========================================================================================================
//�}�N����`
//===========================================================================================================
#define MAX_COIN (1)																					//�e�N�X�`���̍ő吔

//===========================================================================================================
//�O���[�o���ϐ�
//===========================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureCoin = NULL;																//�R�C���̃e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCoin = NULL;															//�R�C���̃o�b�t�@
COIN g_Coin[MAX_COIN];

//===========================================================================================================
//����������
//===========================================================================================================
HRESULT InitCoin(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\coin000.png", &g_pTextureCoin);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffCoin, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

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

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCoin->Unlock();

	return S_OK;
}

//===========================================================================================================
//�I������
//===========================================================================================================
void UninitCoin(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffCoin != NULL)
	{
		g_pVtxBuffCoin->Release();
		g_pVtxBuffCoin = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureCoin != NULL)
	{
		g_pTextureCoin->Release();
		g_pTextureCoin = NULL;
	}
}

//===========================================================================================================
//�X�V����
//===========================================================================================================
void UpdateCoin(void)
{
}

//===========================================================================================================
//�`�揈��
//===========================================================================================================
void DrawCoin(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCoin, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureCoin);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//===========================================================================================================
//�R�C���̐ݒ�
//===========================================================================================================
void SetCoin(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	COIN *pCoin;
	VERTEX_2D *pVtx;

	pCoin = &g_Coin[0];

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++, pCoin++)
	{
		//�ʒu�̐ݒ�
		pCoin->pos;

		pCoin->fHeight;

		pCoin->fWidth;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pCoin->pos.x, pCoin->pos.y + fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pCoin->pos.x, pCoin->pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pCoin->pos.x + fWidth, pCoin->pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pCoin->pos.x + fWidth, pCoin->pos.y, 0.0f);
	}
	pVtx += 4;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCoin->Unlock();
}

//===========================================================================================================
//�R�C���̎擾
//===========================================================================================================
COIN *GetCoin(void)
{
	return &g_Coin[0];
}