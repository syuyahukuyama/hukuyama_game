//===================================================================
//
// �E�C���h�E�̏����@[bullet.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "effect.h"
#include "player.h"
#include "input.h"

//=================================================================================================
//�}�N����`
//=================================================================================================
#define MAX_BULLET (2048)																		//�e�̍ő吔
#define BULLET_SIZE (15)																		//�e�̑傫��

#define VERTEX_CHAR_X (180)																		//�G�̈ʒu���WX
#define VERTEX_CHAR_Y (180)																		//�G�̈ʒu���WY

#define VERTEX_HRFF_X (10)																		//���@�̓����蔻��X
#define VERTEX_HRFF_Y (10)																		//���@�̓����蔻��Y

//=================================================================================================
//�e�̍\����
//=================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;																			//�ʒu
	D3DXVECTOR3 move;																			//�ړ���
	D3DXCOLOR cor;																				//�e�̐F
	int nLife;																					//����
	int nDamage;																				//�e�̃_���[�W��
	bool bUse;																					//�g�p���Ă��邩�ǂ���
	BULLETTYPE type;																			//���
}BULLET;

//=================================================================================================
//�O���[�o���ϐ�
//=================================================================================================
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;														//�e�̃e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;												//�o�b�t�@��														
BULLET g_aBullet[MAX_BULLET];																	//�e�̏��												

//=================================================================================================
//�v���g�^�C�v�錾
//=================================================================================================
void SetVertexBullet(int nIdy);

//=================================================================================================
//����������
//=================================================================================================
HRESULT InitBullet(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bullet001.png", &g_pTextureBullet);

	//�e�̐ݒ�
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].nDamage = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		SetVertexBullet(nCntBullet);

		//rhm�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 100, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 100, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 100, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 100, 255);

		//���_�e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();
	return S_OK;
}

//=================================================================================================
//�I������
//=================================================================================================
void UninitBullet(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
}

//=================================================================================================
//�X�V����
//=================================================================================================
void UpdateBullet(void)
{
	//�ϐ��錾
	BULLET *pBullet;
	PLAYER *pPlayer;
	PLAYERSTATE *pPlayerState;
	
	VERTEX_2D *pVtx;

	pBullet = &g_aBullet[0];
	pPlayer = GetPlayer();
	pPlayerState = GetPlayerState();
	

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pBullet++)
	{
		//�e�̍X�V
		if (pBullet->bUse == true)			//�e���g���Ă��邩
		{
			//�ʒu�̍X�V
			pBullet->pos += pBullet->move;

			//���_���W�̐ݒ�
			SetVertexBullet(nCntBullet);

			//�G�t�F�N�g�̐ݒ�
			//SetEffect(pBullet->pos);

			ENEMY *pEnemy;
			pEnemy = GetENEMY();

			if (pBullet->type == BULLETTYPE_PLAYER)
			{

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{

					if (pEnemy->bUse == true)
					{

						if (pEnemy->pos.y - VERTEX_CHAR_Y <= pBullet->pos.y &&
							pEnemy->pos.y + VERTEX_CHAR_Y >= pBullet->pos.y &&
							pEnemy->pos.x - VERTEX_CHAR_X <= pBullet->pos.x &&
							pEnemy->pos.x + VERTEX_CHAR_X >= pBullet->pos.x)
						{
							pBullet->bUse = false;
							HitEnemy(nCntEnemy, pBullet->nDamage);

							//����
							PlaySound(SOUND_LABEL_SE_EXPLOSION);
						}
					}
				}

			}
			else if (pBullet->type == BULLETTYPE_ENEMY)
			{

				if (pPlayer->State == PLAYERSTATE_NORMAL)
				{
					if (pPlayer->pos.y - VERTEX_HRFF_Y <= pBullet->pos.y &&
						pPlayer->pos.y + VERTEX_HRFF_Y >= pBullet->pos.y &&
						pPlayer->pos.x - VERTEX_HRFF_X <= pBullet->pos.x &&
						pPlayer->pos.x + VERTEX_HRFF_X >= pBullet->pos.x)
					{
						HitPlayer(pPlayer->nLife);
					}
				}
			}

			//�����`�F�b�N
			pBullet->nLife--;

			if (pBullet->nLife <= 0)
			{
 				pBullet->bUse = false;
			}
			
		}
	}
	

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//=================================================================================================
//�`�揈��
//=================================================================================================
void DrawBullet(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);

	//�|���S���̕`��
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}		
	}
}

//=================================================================================================
//�e�̒��_���W�̐ݒ�
//=================================================================================================
void SetVertexBullet(int nIdy)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdy * 4;

	//�e�̒��_���W
	pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdy].pos.x - BULLET_SIZE, g_aBullet[nIdy].pos.y + BULLET_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdy].pos.x - BULLET_SIZE, g_aBullet[nIdy].pos.y - BULLET_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdy].pos.x + BULLET_SIZE, g_aBullet[nIdy].pos.y + BULLET_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdy].pos.x + BULLET_SIZE, g_aBullet[nIdy].pos.y - BULLET_SIZE, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//=================================================================================================
//�e�̐ݒ�
//=================================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nDamage, int nLife,BULLETTYPE type,D3DXCOLOR cor)
{
	//�ϐ��錾
	BULLET *pBullet;
	VERTEX_2D *pVtx;

	pBullet = &g_aBullet[0];

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			//�ʒu�̐ݒ�
			pBullet->pos = pos;
			
			//�ړ��ʂ�ݒ�
			pBullet->move = move;

			//���_���W�̐ݒ�
			SetVertexBullet(nCntBullet);

			//�e�̍U����
			pBullet->nDamage = nDamage;

			//������ݒ�
			pBullet->nLife = nLife;
			
			//�^�C�v
			pBullet->type = type;

			pBullet->bUse = true;

			//�e�̐F
			pBullet->cor = cor;

			break;

		}
		//pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}