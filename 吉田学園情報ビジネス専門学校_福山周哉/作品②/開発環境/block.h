//===================================================================
//
// �E�C���h�E�̏����@[block.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//===========================================================================================================
//�u���b�N�̗񋓌^
//===========================================================================================================
typedef enum
{
	BLOCK_NORMAL = 0,					//�ʏ�u���b�N
	BLOCK_HIDDEN,						//�B���u���b�N
	BLOCK_NOTCOLLISION,					//�����蔻��Ȃ��u���b�N
	BLOCK_UNDERJUDGMENT,				//�����������蔻��̂���u���b�N
	BLOCK_UPJUDGMENT,					//��ɓ����蔻��̂���u���b�N
	BLOCK_HIDDENUNDER,					//�����Ȃ��������u���b�N
	BLOCK_BREAK,						//����u���b�N
	BLOCK_NOJAMPBREAK,					//�W�����v�ł��Ȃ�����u���b�N
	BLOCK_FALL,							//�����Ă���u���b�N
	BLOCK_PIPE,							//�y��
	BLOCK_TSPRING,						//�V�o�l�u���b�N
	BLOCK_GOAL,							//�S�[���u���b�N
	BLOCK_ARROW,						//���Ӗ��
	BLOCK_SPRING,						//�o�l�u���b�N
	BLOCK_SOIL,							//�y�u���b�N
	BLOCK_MAX,
}BLOCKTYPE;

//===========================================================================================================
//�X�e�[�W�̎��
//===========================================================================================================
typedef enum
{
	GAMESTAGE_SELECT = 0,																					//�Z���N�g���
	GAMESTAGE_ONE,																							//�X�e�[�W�P
	GAMESTAGE_TWO,																							//�X�e�[�W�Q
	GAMESTAGE_THREE,																						//�X�e�[�W�R
	GAMESTAGE_MAX,
}GAMESTAGE;

//===========================================================================================================
//�u���b�N�̃e�N�X�`���̗񋓌^
//===========================================================================================================
typedef enum
{
	BLOCKTEX_STONE = 0,					//�΂̃e�N�X�`��
	BLOCKTEX_GOAL,						//�S�[���e�N�X�`��
	BLOCKTEX_PIPE,						//�y�ǃe�N�X�`��
	BLOCKTEX_ARROW,						//���e�N�X�`��
	BLOCKTEX_SPRING,					//�o�l�̃e�N�X�`��
	BLOCKTEX_SOIL,						//�y�̃e�N�X�`��
	BLOCKTEX_MAX,
}BLOCKTEX;

//===========================================================================================================
//�u���b�N�̍\����
//===========================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�u���b�N�̈ʒu
	D3DXVECTOR3 move;		//�u���b�N�̈ړ�
	BLOCKTYPE type;			//�u���b�N�̎��
	BLOCKTEX tex;			//�u���b�N�̃e�N�X�`��
	GAMESTAGE stage;		//�X�e�[�W
	float fTexV;			//�u���b�NV
	float fTexU;			//�u���b�NU
	float fWidth;			//��
	float fHeight;			//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
}BLOCK;

//==========================================================================================================
//�v���g�^�C�v�錾
//==========================================================================================================
HRESULT InitBlock(void);
void UninitBlock(void);
void DrawBlock(void);
void UpdateBlock(void);
void PlacementBlock(STAGEINFO stage);
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight,BLOCKTYPE type,BLOCKTEX tex,GAMESTAGE stage,float fTexV,float fTexU);
BLOCK *GetBlock(void);

#endif
