//===================================================================
//
// �u���b�N�̏����@[block.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "block.h"
#include "sound.h"

//===========================================================================================================
//�}�N����`
//===========================================================================================================
#define MAX_BLOCK (512)																					//�u���b�N�̍ő吔
#define MAX_LINE (3)																					//�u���b�N�̉��̐�

#define MAX_TYPE_BLOCK (1)																				//�u���b�N�̎��

#define VERTEX_CHAR_X (5)																				//�u���b�N�̈ʒu���WX
#define VERTEX_CHAR_Y (5)																				//�u���b�N�̈ʒu���WY

#define VERTEX_X (5)																					//���S����[�܂ł�X�̋���
#define VERTEX_Y (5)																					//���S����[�܂ł�Y�̋���

#define SCREEN_WIDTH_HALF (SCREEN_WIDTH / 2)															//X���̈ʒu
#define SCREEN_HEIGHT_Y ((SCREEN_HEIGHT / 4) * 3.8)														//Y���̈ʒu

#define BLOCK_WIDTH (700.0f)																			//X���̃u���b�N�̈ʒu

#define GRAVITY (0.35f)																					//�d��

//=================================================================================================
//�O���[�o���ϐ�
//=================================================================================================
LPDIRECT3DTEXTURE9 g_aTextureBlock[6] = {};																//�u���b�N�̃e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_aVtxBuffBlock = NULL;															//�u���b�N�̃o�b�t�@�[
LPDIRECT3DTEXTURE9 g_aTextureBlockGoal = NULL;															//�S�[���u���b�N�̃e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_aVtxBuffBlockGoal = NULL;														//�S�[���u���b�N�̃o�b�t�@�[
BLOCK g_aBlock[MAX_BLOCK];																				//�u���b�N�̍ő吔
D3DXVECTOR3 g_moveBlock;																				//�u���b�N�̈ړ���

D3DXVECTOR3 g_movedPlayer;																				//�v���C���[�̈ړ���

//=================================================================================================
//�u���b�N�̏���������
//=================================================================================================
HRESULT InitBlock()
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	BLOCK *pBlock;
	PLAYER *pPlayer;
	STAGEINFO stage;

	pDevice = GetDevice();
	pBlock = GetBlock();
	pPlayer = GetPlayer();
	stage = GetStageInfo();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block001.jpg", &g_aTextureBlock[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block001.png", &g_aTextureBlock[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pipe000.png", &g_aTextureBlock[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\arrow.png", &g_aTextureBlock[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\spring.png", &g_aTextureBlock[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block006.png", &g_aTextureBlock[5]);

	//�u���b�N�̏�����
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].bUse = false;
		g_aBlock[nCntBlock].fHeight = 0;
		g_aBlock[nCntBlock].fWidth = 0;
		g_moveBlock = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_aVtxBuffBlock, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N
	g_aVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//�u���b�N�̈ʒu
		pVtx[0].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y + pBlock->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y, 0.0f);

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

		//�u���b�N�̈ʒu
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}
	
	//���_�o�b�t�@���A�����b�N
	g_aVtxBuffBlock->Unlock();

	//�u���b�N�̔z�u
	PlacementBlock(stage);

	return S_OK;
}

//=================================================================================================
//�I������
//=================================================================================================
void UninitBlock(void)
{
	//�u���b�N�̒��_�o�b�t�@�̊J��
	if (g_aVtxBuffBlock != NULL)
	{
		g_aVtxBuffBlock->Release();
		g_aVtxBuffBlock = NULL;
	}

	//�u���b�N�̃e�N�X�`���̊J��
	for (int nCntEnekyType = 0; nCntEnekyType < MAX_TYPE_BLOCK; nCntEnekyType++)
	{
		if (g_aTextureBlock[nCntEnekyType] != NULL)
		{
			g_aTextureBlock[nCntEnekyType]->Release();
			g_aTextureBlock[nCntEnekyType] = NULL;
		}
	}
}

//=================================================================================================
//�X�V����
//=================================================================================================
void UpdateBlock(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	BLOCK *pBlock;
	PLAYER *pPlayer;

	pBlock = &g_aBlock[0];
	pPlayer = GetPlayer();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_aVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			switch (pBlock->type)
			{
			case BLOCK_NORMAL:						//�ʏ�u���b�N
				pBlock->type = BLOCK_NORMAL;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				break;

			case BLOCK_HIDDEN:						//�B���u���b�N
				pBlock->type = BLOCK_HIDDEN;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				break;

			case BLOCK_UNDERJUDGMENT:				//�����������蔻��̂���u���b�N
				pBlock->type = BLOCK_UNDERJUDGMENT;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				break;

			case BLOCK_UPJUDGMENT:				//�ゾ�������蔻��̂���u���b�N
				pBlock->type = BLOCK_UPJUDGMENT;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				break;

			case BLOCK_BREAK:						//����u���b�N
				pBlock->type = BLOCK_BREAK;
				pVtx[0].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				break;

			case BLOCK_NOJAMPBREAK:						//�W�����v�ł�������u���b�N
				pBlock->type = BLOCK_BREAK;
				pVtx[0].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				break;

			case BLOCK_NOTCOLLISION:				//�����蔻��Ȃ��u���b�N
				pBlock->type = BLOCK_NOTCOLLISION;
				pVtx[0].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(220, 220, 255, 255);
				break;

			case BLOCK_FALL:						//������u���b�N
				pBlock->type = BLOCK_FALL;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pBlock->move.y = GRAVITY / 4;
				pBlock->pos.y += pBlock->move.y;
				break;
			}

			//�u���b�N�̍��W
			pVtx[0].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y + pBlock->fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y, 0.0f);

			//�u���b�N�̈ʒu
			pVtx[0].tex = D3DXVECTOR2(0.0f, pBlock->fTexV);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(pBlock->fTexU, pBlock->fTexV);
			pVtx[3].tex = D3DXVECTOR2(pBlock->fTexU, 0.0f);
		}
		pVtx += 4;
	}
}

//=================================================================================================
//�`�揈��
//=================================================================================================
void DrawBlock(void)
{
	//�ϐ��錾
	BLOCK *pBlock;
	LPDIRECT3DDEVICE9 pDevice;

	pBlock = &g_aBlock[0];
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_aVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�|���S���̕`��
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureBlock[pBlock->tex]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
		}
	}
}

//=================================================================================================
//�u���b�N�̐ݒ�
//=================================================================================================
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight,BLOCKTYPE type,BLOCKTEX tex,GAMESTAGE stage,float fTexU,float fTexV)
{
	BLOCK *pBlock;
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;

	pBlock = &g_aBlock[0];
	pPlayer = GetPlayer();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_aVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == false)
		{
			//�ʒu�̐ݒ�
			pBlock->pos = pos;

			pBlock->fTexV = fTexV;

			pBlock->fTexU = fTexU;

			pBlock->fHeight = fHeight;

			pBlock->fWidth = fWidth;

			pBlock->type = type;

			pBlock->tex = tex;

			pBlock->stage = stage;

			switch (pBlock->type)
			{
			case BLOCK_NORMAL:					//�ʏ�u���b�N
				pBlock->type = BLOCK_NORMAL;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				break;

			case BLOCK_HIDDEN:					//�B���u���b�N
				pBlock->type = BLOCK_HIDDEN;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				//�T�E���h
				//PlaySound(SOUND_LABEL_SE_HIT);
				break;

			case BLOCK_UNDERJUDGMENT:					//�����������蔻��̂���u���b�N
				pBlock->type = BLOCK_UNDERJUDGMENT;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 10);
				break;

			case BLOCK_BREAK:					//����u���b�N
				pBlock->type = BLOCK_BREAK;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				break;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pBlock->pos.x , pBlock->pos.y + fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pBlock->pos.x , pBlock->pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pBlock->pos.x + fWidth, pBlock->pos.y + fHeight , 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pBlock->pos.x + fWidth, pBlock->pos.y, 0.0f);

			//�G�̃^�C�v
			//pEnemy->nType = nType;
			pBlock->bUse = true;
			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_aVtxBuffBlock->Unlock();
}

//=================================================================================================
//�u���b�N�̎擾
//=================================================================================================
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}

//=================================================================================================
//�u���b�N�̔z�u
//=================================================================================================
void PlacementBlock(STAGEINFO stage)
{
	BLOCK *pBlock;
	PLAYER *pPlayer;

	pBlock = GetBlock();
	pPlayer = GetPlayer();
}