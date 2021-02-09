//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : fukuyama syuya
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
//#include "sound.h"

//=================================================================================================
//�}�N����`
//=================================================================================================
#define MAX_TITLE_POLYGON (2)																//�`�悷��|���S���̍ő吔

#define POLYGON_WIDTH (1100)																//�|���S���̒���
#define POLYGON_HEIGHT (350)																//�|���S���̍���

//=================================================================================================
//�O���[�o���ϐ�
//=================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL;														//�e�N�X�`���̃^�C�g��
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;													//�o�b�t�@�̃^�C�g��

LPDIRECT3DTEXTURE9 g_pTexturePolygon[MAX_TITLE_POLYGON] = {};									//�|���S���̃e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;												//�o�b�t�@�̃|���S��
D3DXVECTOR3 g_posPolygon;

bool nCntShape;
int nCount;

//=================================================================================================
//����������
//=================================================================================================
HRESULT InitTitle(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	nCntShape = true;
	int nCount = 30;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bg1.png", &g_pTextureTitle);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title01.png", &g_pTexturePolygon[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\press_enter.png", &g_pTexturePolygon[1]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	//����
	//PlaySound(SOUND_LABEL_BGM007);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygon, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nPolygon = 0; nPolygon < MAX_TITLE_POLYGON; nPolygon++)
	{
		switch (nPolygon)
		{
		case 0:
			pVtx[0].pos = D3DXVECTOR3(200, POLYGON_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(200, 100, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(POLYGON_WIDTH, POLYGON_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(POLYGON_WIDTH, 100, 0.0f);
			break;

		case 1:
			pVtx[0].pos = D3DXVECTOR3(200, 650, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(200, 550, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(1100, 650, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1100, 550, 0.0f);
			break;
		}
		
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

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();

	return S_OK;
}

//=================================================================================================
//�I������
//=================================================================================================
void UninitTitle(void)
{
	//�T�E���h���ꎞ��~
	//StopSound();

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}

	//�|���S��
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//=================================================================================================
//�X�V����
//=================================================================================================
void UpdateTitle(void)
{
	//�ϐ��̍X�V
	nCount++;

	//�|���S���̓_��
	if (nCount % 30 == 0)
	{
		nCntShape = nCntShape ? false : true;
	}

	//ENTER�L�[��������
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(FADE_OUT, MODE_TUTORIAL);
	}

}

//=================================================================================================
//�`�揈��
//=================================================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitle);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�|���S��
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nPolygon = 0; nPolygon < MAX_TITLE_POLYGON; nPolygon++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygon[nPolygon]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygon * 4, 2);
	}
}