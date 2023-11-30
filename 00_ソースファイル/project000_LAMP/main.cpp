//============================================================
//
//	���C������ [main.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "manager.h"
#include "resource.h"

#ifdef _DEBUG	// �f�o�b�O����

#include "debug.h"

#endif	// _DEBUG

//************************************************************
//	���C�u���������N
//************************************************************
#pragma comment(lib, "d3d9.lib")	// �`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")	// [d3d9.lib] �̊g�����C�u����
#pragma comment(lib, "dxguid.lib")	// DirectX�R���|�[�l���g (���i) �g�p�ɕK�v
#pragma comment(lib, "winmm.lib")	// �V�X�e�������擾�ɕK�v
#pragma comment(lib, "dinput8.lib")	// ���͏����ɕK�v
#pragma comment(lib, "xinput.lib")	// �p�b�h�g�p�ɕK�v

//************************************************************
//	�}�N����`
//************************************************************
#define CLASS_NAME	"WindowClass"		// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME	"CHAIN AND STAR"	// �E�C���h�E�̖��O (�L���v�V�����ɕ\��)

#define FPS		(60)			// �A�v���P�[�V������FPS�l
#define APP_FPS	(1000 / FPS)	// �A�v���P�[�V�����̌o�ߎ��Ԃ̔���p

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// �E�C���h�E�v���V�[�W��

//============================================================
//	���C���֐�
//============================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hInstancePrev*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	// �I�����Ƀ��������[�N���o��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �ϐ���錾
	WNDCLASSEX wcex =
	{ // �����l
		sizeof(WNDCLASSEX),									// WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,											// �E�C���h�E�̃X�^�C��
		WindowProc,											// �E�C���h�E�v���V�[�W��
		0,													// �ʏ�͎g�p���Ȃ�
		0,													// �ʏ�͎g�p���Ȃ�
		hInstance,											// �C���X�^���X�n���h��
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),	// �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL, IDC_ARROW),						// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),							// �N���C�A���g�̈�̔w�i�F
		NULL,												// ���j���[�o�[
		CLASS_NAME,											// �E�C���h�E�N���X�̖��O
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1))		// �t�@�C���̃A�C�R��
	};
	RECT rect =		// ��ʃT�C�Y
	{ // �����l
		0,				// �E�C���h�E�̍��� X���W
		0,				// �E�C���h�E�̍��� Y���W
		SCREEN_WIDTH,	// �E�C���h�E�̕�
		SCREEN_HEIGHT	// �E�C���h�E�̍���
	};
	HWND  hWnd;	// �E�C���h�E�n���h�� (���ʎq)
	MSG   msg;	// ���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime  = 0;				// ���ݎ���
	DWORD dwExecLastTime = timeGetTime();	// �Ō�ɏ�����������
	CManager *pManager = NULL;	// �}�l�[�W���[�I�u�W�F�N�g

	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�C���h�E�𐶐�
	hWnd = CreateWindowEx
	( // ����
		0,							// �g���E�C���h�E�X�^�C��
		CLASS_NAME,					// �E�C���h�E�N���X�̖��O
		WINDOW_NAME,				// �E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,		// �E�C���h�E�X�^�C��
		CW_USEDEFAULT,				// �E�C���h�E�̍��� X ���W
		CW_USEDEFAULT,				// �E�C���h�E�̍��� Y ���W
		(rect.right - rect.left),	// �E�C���h�E�̕�
		(rect.bottom - rect.top),	// �E�C���h�E�̍���
		NULL,						// �e�E�C���h�E�̃n���h��
		NULL,						// ���j���[�n���h���܂��͎q�E�C���h�E ID
		hInstance,					// �C���X�^���X�n���h��
		NULL						// �E�C���h�E�쐬�f�[�^
	);

	// ����\��ݒ�
	timeBeginPeriod(1);

	// �����_���֐��̎���쐬
	srand((unsigned int)time(0));

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);		// �E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);				// �N���C�A���g�̈���X�V

	// �}�l�[�W���[�̐���
	pManager = CManager::Create(hInstance, hWnd, TRUE);
	if (pManager == NULL)
	{ // ��g�p���̏ꍇ

		// ��O��Ԃ�
		return -1;
	}

	// ���b�Z�[�W���[�v
	while (1)
	{ // �������[�v

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{ // Windows�̏���

			if (msg.message == WM_QUIT)
			{ // WM_QUIT���b�Z�[�W���󂯎�����ꍇ

				// ���b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{ // WM_QUIT���b�Z�[�W���󂯎���Ă��Ȃ��ꍇ

				// ���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);	// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);	// �E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{ // DirectX�̏���

			// ���ݎ������擾
			dwCurrentTime = timeGetTime();

#ifdef _DEBUG	// �f�o�b�O����

			// FPS�̑���
			pManager->GetDebug()->MeasureFps(dwCurrentTime);

#endif	// _DEBUG

			if ((dwCurrentTime - dwExecLastTime) >= APP_FPS)
			{ // 60����1�b�o��

				if (pManager != NULL)
				{ // �g�p���̏ꍇ

					// �}�l�[�W���[�̍X�V
					pManager->Update();
				}
				else { assert(false); return -1; }	// ��g�p��

				if (pManager != NULL)
				{ // �g�p���̏ꍇ

					// �}�l�[�W���[�̕`��
					pManager->Draw();
				}
				else { assert(false); return -1; }	// ��g�p��

				// �����J�n�̎��� [���ݎ���] ��ۑ�
				dwExecLastTime = dwCurrentTime;

#ifdef _DEBUG	// �f�o�b�O����

				// �t���[���J�E���g�̉��Z
				pManager->GetDebug()->AddFrameCount();

#endif	// _DEBUG

			}
		}
	}

	// �}�l�[�W���[�̔j��
	if (FAILED(CManager::Release(pManager)))
	{ // �j���Ɏ��s�����ꍇ

		// ��O��Ԃ�
		return -1;
	}

	// ����\��߂�
	timeEndPeriod(1);

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I���R�[�h��Ԃ�
	return (int)msg.wParam;
}

//============================================================
//	�E�C���h�E�v���V�[�W��
//============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{ // ���b�Z�[�W�R�[�h���Ƃ̏���
	case WM_KEYDOWN:	// �L�[���͌��m

		switch (wParam)
		{ // w�p�����[�^���Ƃ̏���
		case VK_ESCAPE:	// [ESC] �L�[�������ꂽ�ꍇ

#ifdef NDEBUG	// �����[�X����

			if (IDYES == MessageBox(hWnd, "�{���ɏI�����܂����H", "�I�����b�Z�[�W", MB_YESNO))
			{ // YES �������ꂽ�ꍇ

				// �E�C���h�E��j������
				DestroyWindow(hWnd);	// WM_DESTROY���b�Z�[�W�𑗂�
			}
			else
			{ // NO �������ꂽ�ꍇ

				// �l��Ԃ�
				return 0;
			}

#else	// NDEBUG

			// �E�C���h�E��j������
			DestroyWindow(hWnd);	// WM_DESTROY���b�Z�[�W�𑗂�

#endif	// NDEBUG

			// �����𔲂���
			break;
		}

		// �����𔲂���
		break;

	case WM_CLOSE:		// �E�C���h�E�I���m�F

#ifdef NDEBUG	// �����[�X����

		if (IDYES == MessageBox(hWnd, "�{���ɏI�����܂����H", "�I�����b�Z�[�W", MB_YESNO))
		{ // YES �������ꂽ�ꍇ

			// �E�C���h�E��j������
			DestroyWindow(hWnd);	// WM_DESTROY���b�Z�[�W�𑗂�
		}
		else
		{ // NO �������ꂽ�ꍇ

			// �l��Ԃ�
			return 0;
		}

#else	// NDEBUG

		// �E�C���h�E��j������
		DestroyWindow(hWnd);	// WM_DESTROY���b�Z�[�W�𑗂�

#endif	// NDEBUG

		// �����𔲂���
		break;

	case WM_DESTROY:	// �E�C���h�E�I��

		// WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);

		// �����𔲂���
		break;
	}

	// �f�t�H���g�̃E�C���h�E�v���V�[�W���̏I���R�[�h��Ԃ�
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
