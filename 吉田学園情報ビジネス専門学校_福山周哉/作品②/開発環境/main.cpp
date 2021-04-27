//===================================================================
//
// ���C���̏����@[main.cpp]
// Author: fukuyama syuya
//
//===================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "block.h"
#include "Bg.h"
#include "game.h"
#include "gamesecond.h"
#include "gameselect.h"
#include "gamethird.h"
#include "fade.h"
#include "title.h"
#include "result.h"
#include "tutorial.h"
#include "sound.h"

//=================================================================================================
//�}�N����`
//=================================================================================================
#define CLASS_NAME "WindowClass"		//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME "�E�C���h�E�̕\��"	//�E�C���h�E�̖��O
#define WINDOW_POS_X (50)				//X���W
#define WINDOW_POS_Y (50)				//Y���W
#define ID_TIMER (121)					//�^�C�}�[��ID
#define TIMER_INTERVAL (1000/60)		//�^�C�}�[�����Ԋu
#define MAX_LINE (15)					//�G�̉���

//=================================================================================================
//�O���[�o���ϐ�
//=================================================================================================
LPDIRECT3D9 g_pD3D = NULL;							//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;				//Direct3D�f�o�C�X�ւ̃|�C���^	
LPD3DXFONT g_pFont = NULL;							//�t�H���g�̃|�C���^
int g_nCountFPS;									//FPS�J�E���^
MODE g_mode = MODE_TITLE;							//���[�h�̎��
STAGEINFO g_stage = STAGEINFO_FRIST;				//�X�e�[�W�̎��

//=================================================================================================
//���C���֐�
//=================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		"CLASS_NAME",
		LoadIcon(NULL,IDI_APPLICATION),
		
	};
	HWND hWnd;
	MSG msg;

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);		//�E�C���h�E�̒���

	//�E�C���h�E�̍쐬
	hWnd = CreateWindowEx(
		0,
		"CLASS_NAME",//�E�C���h�E�N���X�̖��O
		"����̃A�N�V�����Q�[��",//�E�C���h�E�̖��O
		//�N���E�h�X�^�C��
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,		//�E�C���h�E�̍���X���W
		CW_USEDEFAULT,		//�E�C���h�E�̍���Y���W
		SCREEN_WIDTH,		//��
		SCREEN_HEIGHT,		//����
		NULL,
		NULL,
		hInstance,
		NULL);
	DWORD dwExecLastTime;			//�������s�ŏI����
	DWORD dwFPSLastTime;			//FPS�v���ŏI����
	DWORD dwCurrentTime;			//���ݎ���
	DWORD dwFrameCount;				//�t���[���J�E���g

	dwExecLastTime =
	dwFPSLastTime =
	dwCurrentTime = timeGetTime();			//���ݎ�����ݒ�
	dwFrameCount = 0;

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			//windows�̏���
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				//0.5�b�o��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) > (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				//DirectX�̏���
				//�X�V����
				Update();

				//�`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}
	//�I������
	Uninit();

	//�E�C���h�E�N���X�̓o�^����
	UnregisterClass(
		"CLASS_NAME",
		wcex.hInstance);
	return (int)msg.wParam;
}

//=================================================================================================
//�E�C���h�E�v���V�[�W��
//=================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);//WM_QUIT
		break;
	case WM_KEYDOWN://�L�[��������
		switch (wParam)
		{
		case VK_ESCAPE://ESC�L�[
			nID = MessageBox(NULL, "�E�C���h�E����܂���낵���ł���", "�E�C���h�E�̍폜", MB_YESNO | MB_TOPMOST);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			else
			{
				nID = MessageBox(NULL, "�I�����܂���ł���", "�����Ȃ���", MB_OK);
			}
			break;
		}
		break;

	case WM_CREATE:
		//�^�C�}�[�̏�����
		SetTimer(hWnd, ID_TIMER, TIMER_INTERVAL, NULL);
		break;


		case WM_CLOSE:
		nID = MessageBox(NULL, "�{���ɕ��܂������ł���", "������", MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES)
		{
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
	
}

//=================================================================================================
//����������
//=================================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if (g_pD3D == NULL)
		{
			return E_FAIL;
		}

		//�f�B�X�N�v���C���[�h���擾
		if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		{
			return E_FAIL;
		}

		//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�[�̐ݒ�
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.BackBufferWidth = SCREEN_WIDTH;
		d3dpp.BackBufferHeight = SCREEN_HEIGHT;
		d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.Windowed = bWindow;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

		//Direct3D�f�o�C�X�̐���
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
			{
				if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
				{
					return E_FAIL;
				}
			}
		}
		//�����_�[�X�e�[�g�̐���
		g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//�T���v���[�X�e�[�g
		g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);			//�k�����A���
		g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);			//�g�厞�A���
		g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);			//U�l�A�J��Ԃ�
		g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);			//V�l�A�J��Ԃ�

		//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);			//�A���t�@�l�̍���
		g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

		//�L�[�{�[�h�̏���������
		InitKeyboard(hInstance, hWnd);

		//�T�E���h�̏���������
		InitSound(hWnd);

		//�t�H���g
		D3DXCreateFont(g_pD3DDevice, 30, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

		//���[�h�̐ݒ�
		SetMode(g_mode);

		//�t�F�[�h�̏���������
		InitFade(MODE_TITLE);

	return S_OK;
}

//=================================================================================================
//�I������
//=================================================================================================
void Uninit(void)
{
	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//�t�F�[�h�̏I������
	UninitFade();

	//�Q�[�����[�h
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;

	case MODE_TUTORIAL:
		UninitTutorial();
		break;

	case  MODE_SELECT:
		UninitSelectGame();
		break;

	case MODE_GAME:
		UninitGame();
		break;

	case MODE_GAMESECOND:
		UninitSecondGame();
		break;

	case MODE_GAMETHIRD:
		UninitThirdGame();
		break;

	case MODE_RESULT:
		UninitResult();
		break;
	}

	//�T�E���h�̏I������
	UninitSound();

	//Direct3D�f�o�C�X�̊J��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
		
	}
	//Direct3D�I�u�W�F�N�g�̊J��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
}

//=================================================================================================
//�X�V����
//=================================================================================================
void Update(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�Q�[�����[�h
	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;

	case MODE_TUTORIAL:
		UpdateTutorial();
		break;

	case  MODE_SELECT:
		UpdateSelectGame();
		break;

	case MODE_GAME:
		UpdateGame();
		break;

	case MODE_GAMESECOND:
		UpdateSecondGame();
		break;

	case MODE_GAMETHIRD:
		UpdateThirdGame();
		break;

	case MODE_RESULT:
		UpdateResult();
		break;
	}

	//�t�F�[�h�̍X�V����
	UpdateFade();
}

//=================================================================================================
//�`�揈��
//=================================================================================================
void Draw(void)
{
	//�o�b�N�o�b�t�@&�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//�Q�[�����[�h
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;

		case MODE_TUTORIAL:
			DrawTutorial();
			break;

		case MODE_SELECT:
			DrawSelectGame();
			break;

		case MODE_GAME:
			DrawGame();
			break;

		case MODE_GAMESECOND:
			DrawSecondGame();
			break;

		case MODE_GAMETHIRD:
			DrawThirdGame();
			break;

		case MODE_RESULT:
			DrawResult();
			break;
		}
		
		//�t�F�[�h�̕`�揈��
		DrawFade();

		//FPS
		#ifdef _DEBUG
			DrawFPS();
		#endif

		//�`��̏I��
		g_pD3DDevice->EndScene();

	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL,NULL,NULL,NULL);
}

LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=================================================================================================
//���[�h�̐ݒ�
//=================================================================================================
void SetMode(MODE mode)
{
	switch(g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;

	case MODE_TUTORIAL:
		UninitTutorial();
		break;

	case MODE_SELECT:
		UninitSelectGame();
		break;

	case MODE_GAME:
		UninitGame();
		break;

	case MODE_GAMESECOND:
		UninitSecondGame();
		break;

	case MODE_GAMETHIRD:
		UninitThirdGame();
		break;
	
	case MODE_RESULT:
		UninitResult();
		break;
	}
	g_mode = mode;
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		break;

	case MODE_TUTORIAL:
		InitTutorial();
		break;

	case MODE_SELECT:
		InitSelectGame();
		break;

	case MODE_GAME:
		InitGame();
		break;

	case MODE_GAMESECOND:
		InitSecondGame();
		break;

	case MODE_GAMETHIRD:
		InitThirdGame();
		break;

	case MODE_RESULT:
		InitResult();
		break;
	}
	
}

MODE GetMode(void)
{
	return g_mode;
}

//=================================================================================================
//�X�e�[�W���̎擾
//=================================================================================================
STAGEINFO GetStageInfo(void)
{
	return g_stage;
}

//void StageClear(void)
//{
//	BLOCK *pBlock;
//
//	if (g_mode == MODE_GAME)
//	{
//		switch (g_stage)
//		{
//		case 0:
//			if (pBlock->type == true)
//			{
//
//				g_stage = (STAGEINFO)1;
//				g_stage = (STAGEINFO)2;
//				//g_stage = (STAGEINFO)3;
//				break;
//			}
//		}
//	}
//}

//=================================================================================================
//FPS
//=================================================================================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

