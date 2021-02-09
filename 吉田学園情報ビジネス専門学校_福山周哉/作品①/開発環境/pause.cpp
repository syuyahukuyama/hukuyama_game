//=============================================================================
//
// �|�[�Y���j���[�̏��� [pouse.cpp]
// Author : fukuyama syuya
//
//=============================================================================
#include "pause.h"
#include "input.h"
#include "fade.h"

//===========================================================================================================
//�O���[�o���ϐ�
//===========================================================================================================
LPDIRECT3DTEXTURE9 g_pPolygon = NULL;																	//�|�[�Y�̃|���S��
LPDIRECT3DVERTEXBUFFER9 g_pVtxPolygon = NULL;															//���_�|���S��

LPDIRECT3DTEXTURE9 g_pContinuePoly = NULL;																//�R���e�j���[�|���S��
LPDIRECT3DVERTEXBUFFER9 g_pVtxContinuePoly = NULL;														//���_�R���e�|���S��

LPDIRECT3DTEXTURE9 g_pRetryPoly = NULL;																	//���g���C�|���S��
LPDIRECT3DVERTEXBUFFER9 g_pVtxRetryPoly = NULL;															//���_���g���C�|���S��

LPDIRECT3DTEXTURE9 g_pQuitPoly = NULL;																	//�I���|���S��
LPDIRECT3DVERTEXBUFFER9 g_pVtxQuitPoly = NULL;															//���_�I���|���S��

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;															//�Ó]

//===========================================================================================================
//����������
//===========================================================================================================
HRESULT InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	//rhm�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, 120);
	pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, 120);
	pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, 120);
	pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 120);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause100.png", &g_pPolygon);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxPolygon, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxPolygon->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(400, 800, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(400, 200, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(800, 800, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(800, 200, 0.0f);

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

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxPolygon->Unlock();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause000.png", &g_pContinuePoly);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxContinuePoly, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxContinuePoly->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(450, 350, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(450, 230, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(750, 350, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(750, 230, 0.0f);

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

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxContinuePoly->Unlock();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause001.png", &g_pRetryPoly);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxRetryPoly, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxRetryPoly->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(370, 500, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(370, 400, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(820, 500, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(820, 400, 0.0f);

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

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxRetryPoly->Unlock();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause002.png", &g_pQuitPoly);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxQuitPoly, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxQuitPoly->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(370, 650, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(370, 550, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(820, 650, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(820, 550, 0.0f);

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

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxQuitPoly->Unlock();

	return S_OK;
}

//===========================================================================================================
//�I������
//===========================================================================================================
void UninitPause(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxPolygon != NULL)
	{
		g_pVtxPolygon->Release();
		g_pVtxPolygon = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pVtxPolygon != NULL)
	{
		g_pVtxPolygon->Release();
		g_pVtxPolygon = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxContinuePoly != NULL)
	{
		g_pVtxContinuePoly->Release();
		g_pVtxContinuePoly = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pVtxContinuePoly != NULL)
	{
		g_pVtxContinuePoly->Release();
		g_pVtxContinuePoly = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxRetryPoly != NULL)
	{
		g_pVtxRetryPoly->Release();
		g_pVtxRetryPoly = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pVtxRetryPoly != NULL)
	{
		g_pVtxRetryPoly->Release();
		g_pVtxRetryPoly = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxQuitPoly != NULL)
	{
		g_pVtxQuitPoly->Release();
		g_pVtxQuitPoly = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pVtxQuitPoly != NULL)
	{
		g_pVtxQuitPoly->Release();
		g_pVtxQuitPoly = NULL;
	}

}

//=================================================================================================
//�X�V����
//=================================================================================================
void UpdatePause(void)
{
	
}

//=================================================================================================
//�`�揈��
//=================================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0,NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxPolygon, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pPolygon);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxContinuePoly, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pContinuePoly);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxRetryPoly, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pRetryPoly);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxQuitPoly, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pQuitPoly);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}