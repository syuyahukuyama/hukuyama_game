//===================================================================
//
// �G�̏����@[enemy.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "enemy.h"
#include "sound.h"
#include "explosion.h"
#include "fade.h"
#include "score.h"
#include "bullet.h"
#include "player.h"
#include "effect.h"
#include "input.h"

//=================================================================================================
//�}�N����`
//=================================================================================================
#define MAX_TYPE_ENEMY (1)																				//�G�̎��

#define VERTEX_CHAR_X (200)																				//�G�̈ʒu���WX
#define VERTEX_CHAR_Y (200)																				//�G�̈ʒu���WY

#define VERTEX_X (500)																					//���S����[�܂ł�X�̋���
#define VERTEX_Y (500)																					//���S����[�܂ł�Y�̋���

#define SCREEN_WIDTH_HALF (SCREEN_WIDTH / 2)															//X���̈ʒu

//=================================================================================================
//�O���[�o���ϐ�
//=================================================================================================
LPDIRECT3DTEXTURE9 g_apTextureEnemy = NULL;																//�G�l�~�[�̃e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_aVtxBuffEnemy = NULL;															//�G�l�~�[�̃o�b�t�@�[
ENEMY g_aEnemy[MAX_ENEMY];																				//�G�l�~�[�̍ő吔
D3DXVECTOR3 g_moveEnemy;																				//�G�̈ړ���
int g_nCntEnemy;
int nCounter;																							//�G�̒e�̔��˃J�E���^�[

//=================================================================================================
//�G�̏���������
//=================================================================================================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\boss003.png", &g_apTextureEnemy);

	//�G�̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nMove = 0;
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].bUse = false;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_aVtxBuffEnemy, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N
	g_aVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//�G�l�~�[�̈ʒu
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - VERTEX_CHAR_X, g_aEnemy[nCntEnemy].pos.y + VERTEX_CHAR_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - VERTEX_CHAR_X, g_aEnemy[nCntEnemy].pos.y - VERTEX_CHAR_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + VERTEX_CHAR_X, g_aEnemy[nCntEnemy].pos.y + VERTEX_CHAR_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + VERTEX_CHAR_X, g_aEnemy[nCntEnemy].pos.y - VERTEX_CHAR_Y, 0.0f);

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
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_aVtxBuffEnemy->Unlock();
	return S_OK;
}

//=================================================================================================
//�I������
//=================================================================================================
void UninitEnemy(void)
{
	//���_�o�b�t�@�̊J��
	if (g_aVtxBuffEnemy != NULL)
	{
		g_aVtxBuffEnemy->Release();
		g_aVtxBuffEnemy = NULL;
	}
	
	//�e�N�X�`���̊J��
	for (int nCntEnekyType = 0; nCntEnekyType < MAX_TYPE_ENEMY; nCntEnekyType++)
	{
		if (g_apTextureEnemy != NULL)
		{
			g_apTextureEnemy->Release();
			g_apTextureEnemy = NULL;
		}
	}
}

//=================================================================================================
//�X�V����
//=================================================================================================
void UpdateEnemy(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	ENEMY  *pEnemy;
	PLAYER *pPlayer;

	pEnemy = &g_aEnemy[0];
	g_nCntEnemy = 0;
	pPlayer = GetPlayer();
	nCounter++;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_aVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		//�G�̍X�V
		if (pEnemy->bUse == true)			//�G���g���Ă��邩
		{
			g_nCntEnemy++;

			if (pEnemy->pos.x > SCREEN_WIDTH)
			{
				pEnemy->nMove += 1;
			}

			//�ʒu�̍X�V
			pEnemy->pos.x += g_moveEnemy.x;
			pEnemy->pos.y += g_moveEnemy.y;

			//�G�̒e�̔���

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(0.1f, 1.8f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(0.2f, 2.0f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			
			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(0.4f, 2.15f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(0.6f, 2.25f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(0.8f, 2.3f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.0f, 2.25f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.2f, 2.15f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.4f, 2.0f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 60 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.6f, 1.8f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.8f, 0.0f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.0f, 0.2f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.15f, 0.4f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.25f, 0.6f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.3f, 0.8f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.25f, 1.0f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.15f, 1.2f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.0f, 1.4f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.8f, 1.6f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 75 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(1.85f, 1.8f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			//�t��]

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-0.1f, 2.5f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(0.0f, 1.6f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-0.2f, 2.3f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-0.4f, 2.45f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-0.6f, 2.55f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-0.8f, 2.6f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-1.0f, 2.55f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-1.2f, 2.45f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-1.4f, 2.3f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 70 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-1.6f, 2.1f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			
			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(2.1f, 0.0f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.3f, 0.2f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.45f, 0.4f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.55f, 0.6f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.6f, 0.8f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.55f, 1.0f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.45f, 1.2f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.3f, 1.4f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-2.1f, 1.6f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			if (nCounter % 50 == 0)
			{
				SetBullet(pEnemy->pos, D3DXVECTOR3(-1.85f, 1.8f, 0.0f), 1, 1000, BULLETTYPE_ENEMY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			switch (pEnemy->state)
			{
			case ENEMYSTATE_DAMAGE:
				pEnemy->nCounterstate--;
				if (pEnemy->nCounterstate <= 0)
				{
					pEnemy->state = ENEMYSTATE_NORMAL;
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				break;
			}
		}
		pVtx += 4;
	}
}

//=================================================================================================
//�`�揈��
//=================================================================================================
void DrawEnemy(void)
{
	//�ϐ��錾
	ENEMY *pEnemy;
	LPDIRECT3DDEVICE9 pDevice;

	pEnemy = &g_aEnemy[0];
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_aVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�|���S���̕`��
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureEnemy);

		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=================================================================================================
//�G�̐ݒ�
//=================================================================================================
void SetEnemy(D3DXVECTOR3 pos,int nLife, int nType,int nMove)
{
	ENEMY *pEnemy;
	VERTEX_2D *pVtx;

	pEnemy = &g_aEnemy[0];

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_aVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			//�ʒu�̐ݒ�
			pEnemy->pos = pos;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - VERTEX_CHAR_X, pEnemy->pos.y + VERTEX_CHAR_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x - VERTEX_CHAR_X, pEnemy->pos.y - VERTEX_CHAR_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x + VERTEX_CHAR_X, pEnemy->pos.y + VERTEX_CHAR_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + VERTEX_CHAR_X, pEnemy->pos.y - VERTEX_CHAR_Y, 0.0f);

			//�G�̗̑�
			pEnemy->nLife = nLife;

			//�G�̈ړ�
			pEnemy->nMove = nMove;

			//�G�̃^�C�v
			pEnemy->nType = nType;
			pEnemy->bUse = true;
			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_aVtxBuffEnemy->Unlock();
}

//=================================================================================================
//�G�̎擾
//=================================================================================================
ENEMY *GetENEMY(void)
{
	return &g_aEnemy[0];
}

//�G�Ƃ̓����蔻��
bool HitEnemy(int nIdx, int nDamege)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_aVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//�G�̗̑͂�0�ɂȂ���
	g_aEnemy[nIdx].nLife -= nDamege;

	pVtx += nIdx * 4;

	if (g_aEnemy[nIdx].nLife <= 0)
	{
		g_aEnemy[nIdx].bUse = false;
		AddScore(ENEMY_POINT);
		SetExplosion(g_aEnemy[nIdx].pos);
		return true;
	}
	else
	{
		g_aEnemy[nIdx].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nIdx].nCounterstate = 5;

		//�G�̐F��ύX
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		return false;
	}
	

	//���_�o�b�t�@���A�����b�N
	g_aVtxBuffEnemy->Unlock();
}

//�g���Ă���G�̐��̎擾
int GetCntEnemy(void)
{
	return g_nCntEnemy;
}