//===================================================================
//
// �E�C���h�E�̏����@[player.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "effect.h"
#include "fade.h"
#include "sound.h"

//==========================================================================================================
//�}�N����`
//==========================================================================================================
#define MOVE_DATA (10)

#define SCREEN_WIDTH_HALF (SCREEN_WIDTH / 2)															//X���̈ʒu
#define SCREEN_HEIGHT_Y ((SCREEN_HEIGHT / 4) * 3)														//Y���̈ʒu

#define VERTEX_X (25)																					//���S����[�܂ł�X�̋���
#define VERTEX_Y (25)																					//���S����[�܂ł�Y�̋���
#define MAX_ZAN (1)																						//�ő�c�@

#define VERTEX_CHAR_X (40)																				//�G�̈ʒu���WX
#define VERTEX_CHAR_Y (40)																				//�G�̈ʒu���WY

//==========================================================================================================
//�O���[�o���ϐ�
//==========================================================================================================
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;																//�e�N�X�`���̃|���S��
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;														//�o�b�t�@�|���S��
int g_nCountAnim;																						//�A�j���[�V�����J�E���^�[
int g_nPatternAnim;																						//�A�j���[�V�����p�^�[��NO
D3DXVECTOR3 g_posPlayer;																				//�v���C���[�̈ʒu
D3DXVECTOR3 g_rotPlayer;																				//�v���C���[�̉�]�p
float g_flenghtPlayer;																					//�v���C���[�̑Ίp���̒���
float g_fAnglePlayer;																					//�v���C���[�̑Ίp���̊p�x
D3DXVECTOR3 g_movePlayer;																				//�v���C���[�̈ړ���
PLAYER g_aPlayer;																						//�v���C���[�̏��
PLAYERSTATE g_aPlayerState;																				//�v���C���[�̏�ԏ��

//===========================================================================================================
//����������
//===========================================================================================================
HRESULT InitPlayer(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\player000.png", &g_pTexturePlayer);

	//�v���C���[���̏�����
	g_aPlayer.pos = D3DXVECTOR3(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_Y, 0.0f);
	g_aPlayer.nLife = 0;
	g_aPlayer.State = PLAYERSTATE_APPEAR;
	g_aPlayer.nCounterDisp = 0;
	g_aPlayer.nCounterState = 0;
	g_aPlayer.nZan = MAX_ZAN;
	g_aPlayer.bDisp = true;

	g_nCountAnim = 0;
	g_nPatternAnim = 0;

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y + VERTEX_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y + VERTEX_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);

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

	//�|���S���̏����ʒu
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}


//===========================================================================================================
//�I������
//===========================================================================================================
void UninitPlayer(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//===========================================================================================================
//�X�V����
//===========================================================================================================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;
	ENEMY *pEnemy;

	pEnemy = GetENEMY();
	pPlayer = GetPlayer();

	//�X�V����
	//g_nCountAnim++;			//�A�j���[�V�����̃J�E���^�[�X�V
	//if ((g_nCountAnim % 2) == 0)
	//{
	//	g_nPatternAnim = (g_nPatternAnim + 1) % 8;
	//}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//�|���S���̏㉺���Eor�ᑬ�ړ�
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{
			g_movePlayer.x += sinf(D3DX_PI) / 2;
			g_movePlayer.y += cosf(D3DX_PI) / 2;
		}
		else
		{
			g_movePlayer.x += sinf(D3DX_PI) * 3;
			g_movePlayer.y += cosf(D3DX_PI) * 3;
		}
	}

	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{
			g_movePlayer.x -= sinf(-D3DX_PI) / 2;
			g_movePlayer.y -= cosf(-D3DX_PI) / 2;
		}
		else
		{
			g_movePlayer.x -= sinf(-D3DX_PI) * 3;
			g_movePlayer.y -= cosf(-D3DX_PI) * 3;
		}
	}

	else if (GetKeyboardPress(DIK_A) == true)
	{
		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{
			g_movePlayer.x += sinf(D3DX_PI / -2) / 2;
			g_movePlayer.y += cosf(D3DX_PI / -2) / 2;
		}
		else
		{
			g_movePlayer.x += sinf(D3DX_PI / -2) * 3;
			g_movePlayer.y += cosf(D3DX_PI / -2) * 3;
		}
	}

	else if (GetKeyboardPress(DIK_D) == true)
	{
		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{
			g_movePlayer.x += sinf(D3DX_PI / 2) / 2;
			g_movePlayer.y += cosf(D3DX_PI / 2) / 2;
		}
		else
		{
			g_movePlayer.x += sinf(D3DX_PI / 2) * 3;
			g_movePlayer.y += cosf(D3DX_PI / 2) * 3;
		}
	}

	//�e�𔭎�
	if (GetKeyboardPress(DIK_SPACE) == true)
	{
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(1.2f, -25.0f, 0.0f), 2, 100, BULLETTYPE_PLAYER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(-1.2f, -25.0f, 0.0f), 2, 100, BULLETTYPE_PLAYER,D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//����
		PlaySound(SOUND_LABEL_SE_SHOT);
	}

	//�G�̒e�ɓ�������
	if (pPlayer->State == PLAYERSTATE_NORMAL)
	{
		if (pPlayer->pos.y > pEnemy->pos.y - VERTEX_X &&
			pPlayer->pos.y < pEnemy->pos.y + VERTEX_X &&
			pPlayer->pos.x > pEnemy->pos.x - VERTEX_X &&
			pPlayer->pos.x < pEnemy->pos.x + VERTEX_X)
		{
			HitPlayer(pPlayer->nZan);
		}
	}

	//�ʒu�̍X�V
	g_aPlayer.pos.x += g_movePlayer.x;
	g_aPlayer.pos.y += g_movePlayer.y;

	//�ړ��ʂ̌���
	g_movePlayer.x += (0.0f - g_movePlayer.x) * 0.18f;
	g_movePlayer.y += (0.0f - g_movePlayer.y) * 0.18f;

	if (g_aPlayer.pos.x < 0,0)
	{
		g_aPlayer.pos.x = (0 + (MOVE_DATA / 2));
	}

	//��ʐ���
	if (g_aPlayer.pos.x - VERTEX_X < 0)
	{
		g_aPlayer.pos.x = VERTEX_X;
	}
	if (g_aPlayer.pos.x + VERTEX_X > SCREEN_WIDTH)
	{
		g_aPlayer.pos.x = SCREEN_WIDTH - VERTEX_X;
	}
	if (g_aPlayer.pos.y - VERTEX_Y < 0)
	{
		g_aPlayer.pos.y = VERTEX_Y;
	}
	if (g_aPlayer.pos.y+ VERTEX_Y > SCREEN_HEIGHT)
	{
		g_aPlayer.pos.y = SCREEN_HEIGHT - VERTEX_Y;
	}

	pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y + VERTEX_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x - VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y + VERTEX_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x + VERTEX_X, g_aPlayer.pos.y - VERTEX_Y, 0.0f);

	//���@�̏o��
	switch (g_aPlayer.State)
	{
	case PLAYERSTATE_APPEAR:
		g_aPlayer.nLife = 1;
		g_aPlayer.nCounterDisp++;
		if ((g_aPlayer.nCounterDisp % 6) == 0)
		{
			g_aPlayer.bDisp = g_aPlayer.bDisp ? false : true;
		}
		if ((g_aPlayer.nCounterDisp % 80) == 0)
		{
			g_aPlayer.State = PLAYERSTATE_NORMAL;
		}
		break;

	case PLAYERSTATE_NORMAL:
		g_aPlayer.bDisp = true;
		break;

	case PLAYERSTATE_DAMAGE:

		break;

	case PLAYERSTATE_WAIT:
		if ((g_aPlayer.nCounterDisp % 80) == 0)
		{
			g_aPlayer.State = PLAYERSTATE_APPEAR;
		}
		break;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

}

//===========================================================================================================
//�`�揈��
//===========================================================================================================
void DrawPlayer(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�;
	pDevice->SetTexture(0, g_pTexturePlayer);

	//�|���S���̕`��
	if (g_aPlayer.bDisp == true)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//===========================================================================================================
//�v���C���[�̎擾
//===========================================================================================================
PLAYER *GetPlayer(void)
{
	return &g_aPlayer;
}

//===========================================================================================================
//�v���C���[�̏�Ԃ̎擾
//===========================================================================================================
PLAYERSTATE *GetPlayerState(void)
{
	return &g_aPlayerState;
}

//===========================================================================================================
//�G�Ƃ̓����蔻��
//===========================================================================================================
bool HitPlayer(int nDamege)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;

	pPlayer = &g_aPlayer;

	pPlayer->State = PLAYERSTATE_DAMAGE;

	pPlayer->nLife -= nDamege;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//�G�̗̑͂�0�ɂȂ���
	if (pPlayer->nLife <= 0)
	{
		pPlayer->nZan -= 1;
		pPlayer->bDisp = false;
		SetExplosion(pPlayer->pos);

		if (pPlayer->nZan >= 0)
		{
			pPlayer->State = PLAYERSTATE_WAIT;
		}
		else
		{
			pPlayer->State = PLAYERSTATE_DEATH;
		}

		return true;
	}
	else
	{
		pPlayer->State = PLAYERSTATE_DAMAGE;
		pPlayer->nCounterState = 3;

		//�G�̐F��ύX
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		return false;
	}
	pVtx += 4;

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

