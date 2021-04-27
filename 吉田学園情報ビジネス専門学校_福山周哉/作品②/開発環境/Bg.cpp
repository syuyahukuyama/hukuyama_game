//===================================================================
//
// �w�i�̏����@[bg.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "Bg.h"

//===========================================================================================================
//�}�N����`
//===========================================================================================================
#define MAX_BG (3)																					//�w�i�e�N�X�`���̍ő吔
//#define MAX_SPEED (0.0f)																				//�w�i�̈ړ�

//===========================================================================================================
//�O���[�o���ϐ�
//===========================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureBg = NULL;																//�w�i�̃e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;														//�w�i
float g_fBg;																						//�w�i�̃X�s�[�h

//===========================================================================================================
//����������
//===========================================================================================================
HRESULT InitBg(void)
{
	//������
	g_fBg = 0.0f;

	//�ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	MODE mode = GetMode();

	//�e�N�X�`���̓ǂݍ���
	switch (mode)
	{
	case MODE_SELECT:
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky002.jpg", &g_pTextureBg);
		break;

	case MODE_GAME:
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.jpg", &g_pTextureBg);
		break;

	case MODE_GAMESECOND:
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bg002.jpg", &g_pTextureBg);
		break;

	case MODE_GAMETHIRD:
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.jpg", &g_pTextureBg);
		break;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBg, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);


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
	g_pVtxBuffBg->Unlock();

	return S_OK;
}

//===========================================================================================================
//�I������
//===========================================================================================================
void UninitBg(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureBg != NULL)
	{
		g_pTextureBg->Release();
		g_pTextureBg = NULL;
	}
	
}

//===========================================================================================================
//�X�V����
//===========================================================================================================
void UpdateBg(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	//g_fBg += MAX_SPEED;


	//���_���̐ݒ�
	/*pVtx[0].tex = D3DXVECTOR2(0.0 + g_fBg, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0 + g_fBg, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0 + g_fBg, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0 + g_fBg, 0.0);*/

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg->Unlock();
}

//===========================================================================================================
//�`�揈��
//===========================================================================================================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBg);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);
}
