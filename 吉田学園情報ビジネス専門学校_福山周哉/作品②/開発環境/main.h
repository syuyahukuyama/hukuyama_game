//===================================================================
//
// �E�C���h�E�̏����@[main.h]
// Author: fukuyama syuya
//
//===================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#include "main.h"

//=================================================================================================
//�C���N���[�h�t�@�C��
//=================================================================================================
#include <windows.h>
#include "d3dx9.h"						//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"						//���͏����ɕK�v
#pragma comment (lib,"dinput8.lib")		//���͏����ɕK�v
#include "xaudio2.h"					//�T�E���h�Đ��ɕK�v

//=================================================================================================
//���C�u�����̃����N
//=================================================================================================
#pragma comment(lib, "d3d9.lib")		//�`�揈��
#pragma comment(lib, "d3dx9.lib")		//�`�揈��
#pragma comment(lib, "dxguid.lib")		//�`�揈��
#pragma comment(lib, "winmm.lib")		//FPS

//=================================================================================================
//�}�N����`
//=================================================================================================
#define SCREEN_WIDTH (1280)				//��ʂ̕�
#define SCREEN_HEIGHT (810)				//��ʂ̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//=================================================================================================
//���_�t�H�[�}�b�g�ɍ��킹���\����
//=================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	float rhw;				//1.0�ŌŒ�
	D3DCOLOR col;			//���_�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`�����W
}VERTEX_2D;

//=================================================================================================
//���[�h�̎��
//=================================================================================================
typedef enum
{
	MODE_TITLE = 0,			//�^�C�g�����
	MODE_SELECT,			//�Q�[���I�����
	MODE_GAME,				//�Q�[��1���
	MODE_GAMESECOND,		//�Q�[��2���
	MODE_GAMETHIRD,			//�Q�[��3���
	MODE_RESULT,			//���U���g���
	MODE_TUTORIAL,			//�`���[�g���A�����
	MODE_MAX
}MODE;

//=================================================================================================
//�X�e�[�W�̎��
//=================================================================================================
typedef enum
{
	STAGEINFO_FRIST = 0,		//�X�e�[�W�T
	STAGEINFO_SECOND,			//�X�e�[�W�U
	STAGEINFO_THIRD,			//�X�e�[�W�V
	STAGEINFO_MAX,
}STAGEINFO;

//=================================================================================================
//�v���g�^�C�v�錾
//=================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInsutance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
LPDIRECT3DDEVICE9 GetDevice(void);
void DrawFPS(void);
void SetMode(MODE mode);
MODE GetMode(void);
STAGEINFO GetStageInfo(void);

#endif