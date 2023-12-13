//============================================================
//
//	���͏��� [input.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "input.h"

//************************************************************
//	�}�N����`
//************************************************************
// �}�E�X�p
#define MAX_MOUKEY	(4)	// �}�E�X�L�[�̍ő吔

// �p�b�h�p
#define VIB_TIME		(20)	// �o�C�u�̌p������
#define VIB_TIME_DEATH	(100)	// ���S���̃o�C�u�p������

#define VIB_DAMAGE		((short)(USHRT_MAX * 0.6f))	// �_���[�W���̐U�����x��
#define VIB_BIG_DAMAGE	((short)(USHRT_MAX * 0.8f))	// ��_���[�W���̐U�����x��
#define VIB_DEATH		(USHRT_MAX)					// ���S���̐U�����x��
#define VIB_WALLDASH	((short)(USHRT_MAX * 0.6f))	// �Ǒ���̐U�����x��
#define VIB_FLAILCHAGE	((short)(USHRT_MAX * 0.4f))	// �t���C�����߂̐U�����x��
#define VIB_FLAILSHOT	((short)(USHRT_MAX * 1.0f))	// �t���C�������̐U�����x��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
LPDIRECTINPUT8 CInput::m_pInput = NULL;	// DirectInput�I�u�W�F�N�g
int CInput::m_nNumAll = 0;	// ���̓f�o�C�X�̑���

//************************************************************
//	�e�N���X [CInput] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CInput::CInput()
{
	// �����o�ϐ����N���A
	m_pDevice = NULL;	// DirectInput�f�o�C�X

	// ���̓f�o�C�X�̑��������Z
	m_nNumAll++;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CInput::~CInput()
{
	// ���̓f�o�C�X�̑��������Z
	m_nNumAll--;
}

//============================================================
//	����������
//============================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND /*hWnd*/)
{
	// �����o�ϐ���������
	m_pDevice = NULL;	// DirectInput�f�o�C�X

	if (m_pInput == NULL)
	{ // ���̓f�o�C�X���g�p����Ă��Ȃ��ꍇ

		// DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CInput::Uninit(void)
{
	// DirectInput�f�o�C�X�̔j��
	if (m_pDevice != NULL)
	{ // DirectInput�f�o�C�X���g�p���̏ꍇ

		// �A�N�Z�X���J��
		m_pDevice->Unacquire();

		// �������J��
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
	if (m_nNumAll <= 1)
	{ // �Ō��DirectInput�f�o�C�X���j�����ꂽ�ꍇ

		if (m_pInput != NULL)
		{ // DirectInput�I�u�W�F�N�g���g�p���̏ꍇ

			// �������J��
			m_pInput->Release();
			m_pInput = NULL;
		}
	}
}

//************************************************************
//	�q�N���X [CInputKeyboard] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CInputKeyboard::CInputKeyboard()
{
	// �����o�ϐ����N���A
	memset(&m_aKeyStatePress[0],   0, sizeof(m_aKeyStatePress));	// �}�E�X�̃v���X���
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyStateTrigger));	// �}�E�X�̃g���K�[���
	memset(&m_aKeyStateRelease[0], 0, sizeof(m_aKeyStateRelease));	// �}�E�X�̃����[�X���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CInputKeyboard::~CInputKeyboard()
{

}

//============================================================
//	����������
//============================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �����o�ϐ���������
	memset(&m_aKeyStatePress[0],   0, sizeof(m_aKeyStatePress));	// �}�E�X�̃v���X���
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyStateTrigger));	// �}�E�X�̃g���K�[���
	memset(&m_aKeyStateRelease[0], 0, sizeof(m_aKeyStateRelease));	// �}�E�X�̃����[�X���

	// ���͂̏�����
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// DirectInput�f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{ // �ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{ // �ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CInputKeyboard::Uninit(void)
{
	// ���͂̏I��
	CInput::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CInputKeyboard::Update(void)
{
	// �ϐ��z���錾
	BYTE aKeyState[MAX_NUMKEY];	// �L�[�{�[�h�̓��͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{ // �擾�ɐ��������ꍇ

		for (int nCntKey = 0; nCntKey < MAX_NUMKEY; nCntKey++)
		{ // �L�[�̍ő吔���J��Ԃ�

			// �L�[�{�[�h�̃����[�X����ۑ�
			m_aKeyStateRelease[nCntKey] = (m_aKeyStatePress[nCntKey] | aKeyState[nCntKey]) ^ aKeyState[nCntKey];

			// �L�[�{�[�h�̃g���K�[����ۑ�
			m_aKeyStateTrigger[nCntKey] = (m_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			// �L�[�{�[�h�̃v���X����ۑ�
			m_aKeyStatePress[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{ // �擾�Ɏ��s�����ꍇ

		// �L�[�{�[�h�ւ̃A�N�Z�X�����ēx�l��
		m_pDevice->Acquire();
	}
}

//============================================================
//	�v���X�擾����
//============================================================
bool CInputKeyboard::IsPress(int nKey)
{
	// ���͏���Ԃ�
	return (m_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//============================================================
//	�g���K�[�擾����
//============================================================
bool CInputKeyboard::IsTrigger(int nKey)
{
	// ���͏���Ԃ�
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//============================================================
//	�����[�X�擾����
//============================================================
bool CInputKeyboard::IsRelease(int nKey)
{
	// ���͏���Ԃ�
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//============================================================
//	��������
//============================================================
CInputKeyboard *CInputKeyboard::Create(HINSTANCE hInstance, HWND hWnd)
{
	// �|�C���^��錾
	CInputKeyboard *pKeyboard = NULL;	// �L�[�{�[�h�����p

	if (pKeyboard == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pKeyboard = new CInputKeyboard;	// �L�[�{�[�h
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pKeyboard != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �L�[�{�[�h�̏�����
		if (FAILED(pKeyboard->Init(hInstance, hWnd)))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pKeyboard;
			pKeyboard = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pKeyboard;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CInputKeyboard::Release(CInputKeyboard *&prKeyboard)
{
	if (prKeyboard != NULL)
	{ // �g�p���̏ꍇ

		// �L�[�{�[�h�̏I��
		prKeyboard->Uninit();

		// �������J��
		delete prKeyboard;
		prKeyboard = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//************************************************************
//	�q�N���X [CInputMouse] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CInputMouse::CInputMouse()
{
	// �����o�ϐ����N���A
	memset(&m_keyStatePress,   0, sizeof(m_keyStatePress));		// �v���X���
	memset(&m_keyStateTrigger, 0, sizeof(m_keyStateTrigger));	// �g���K�[���
	memset(&m_keyStateRelease, 0, sizeof(m_keyStateRelease));	// �����[�X���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CInputMouse::~CInputMouse()
{

}

//============================================================
//	����������
//============================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �����o�ϐ���������
	memset(&m_keyStatePress,   0, sizeof(m_keyStatePress));		// �v���X���
	memset(&m_keyStateTrigger, 0, sizeof(m_keyStateTrigger));	// �g���K�[���
	memset(&m_keyStateRelease, 0, sizeof(m_keyStateRelease));	// �����[�X���

	// �ϐ���錾
	DIPROPDWORD diprop;	// �f�o�C�X�̐ݒ�p

	// ���͂̏�����
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// DirectInput�f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{ // �ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))))
	{ // �ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �f�o�C�X�̐ݒ�
	diprop.diph.dwSize			= sizeof(diprop);
	diprop.diph.dwHeaderSize	= sizeof(diprop.diph);
	diprop.diph.dwObj	= 0;
	diprop.diph.dwHow	= DIPH_DEVICE;
	diprop.dwData		= DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{ // �f�o�C�X�̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �}�E�X�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CInputMouse::Uninit(void)
{
	// ���͂̏I��
	CInput::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CInputMouse::Update(void)
{
	// �ϐ���錾
	DIMOUSESTATE keyState;	// �}�E�X�̓��͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(keyState), &keyState)))
	{ // �擾�ɐ��������ꍇ

		for (int nCntMouKey = 0; nCntMouKey < MAX_MOUKEY; nCntMouKey++)
		{ // �}�E�X�L�[�̍ő吔���J��Ԃ�

			// �}�E�X�̃����[�X����ۑ�
			m_keyStateRelease.rgbButtons[nCntMouKey] = (m_keyStatePress.rgbButtons[nCntMouKey] | keyState.rgbButtons[nCntMouKey]) ^ keyState.rgbButtons[nCntMouKey];

			// �}�E�X�̃g���K�[����ۑ�
			m_keyStateTrigger.rgbButtons[nCntMouKey] = (m_keyStatePress.rgbButtons[nCntMouKey] ^ keyState.rgbButtons[nCntMouKey]) & keyState.rgbButtons[nCntMouKey];
		}

		// �}�E�X�̃v���X����ۑ�
		m_keyStatePress = keyState;
	}
	else
	{ // �擾�Ɏ��s�����ꍇ

		// �}�E�X�ւ̃A�N�Z�X�����ēx�l��
		m_pDevice->Acquire();
	}
}

//============================================================
//	�v���X�擾����
//============================================================
bool CInputMouse::IsPress(EKey mouKey)
{
	// ���͏���Ԃ�
	return (m_keyStatePress.rgbButtons[mouKey] & 0x80) ? true : false;
}

//============================================================
//	�g���K�[�擾����
//============================================================
bool CInputMouse::IsTrigger(EKey mouKey)
{
	// ���͏���Ԃ�
	return (m_keyStateTrigger.rgbButtons[mouKey] & 0x80) ? true : false;
}

//============================================================
//	�����[�X�擾����
//============================================================
bool CInputMouse::IsRelease(EKey mouKey)
{
	// ���͏���Ԃ�
	return (m_keyStateRelease.rgbButtons[mouKey] & 0x80) ? true : false;
}

//============================================================
//	�ړ��ʎ擾����
//============================================================
D3DXVECTOR3 CInputMouse::GetMove(void)
{
	// �}�E�X�̈ړ��ʂ�Ԃ�
	return D3DXVECTOR3
	( // �Ԃ�l
		(float)m_keyStatePress.lX,	// ���ړ���
		(float)m_keyStatePress.lY,	// �c�ړ���
		(float)m_keyStatePress.lZ	// �z�C�[���ړ���
	);
}

//============================================================
//	��������
//============================================================
CInputMouse *CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	// �|�C���^��錾
	CInputMouse *pMouse = NULL;		// �}�E�X�����p

	if (pMouse == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pMouse = new CInputMouse;	// �}�E�X
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pMouse != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �}�E�X�̏�����
		if (FAILED(pMouse->Init(hInstance, hWnd)))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pMouse;
			pMouse = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pMouse;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CInputMouse::Release(CInputMouse *&prMouse)
{
	if (prMouse != NULL)
	{ // �g�p���̏ꍇ

		// �}�E�X�̏I��
		prMouse->Uninit();

		// �������J��
		delete prMouse;
		prMouse = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//************************************************************
//	�e�N���X [CInputPad] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CInputPad::CInputPad()
{
	// �����o�ϐ����N���A
	memset(&m_aVibration[0],		0, sizeof(m_aVibration));		// �o�C�u���
	memset(&m_aKeyStatePress[0],	0, sizeof(m_aKeyStatePress));	// �v���X���
	memset(&m_aKeyStateTrigger[0],	0, sizeof(m_aKeyStateTrigger));	// �g���K�[���
	memset(&m_aKeyStateRelease[0],	0, sizeof(m_aKeyStateRelease));	// �����[�X���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CInputPad::~CInputPad()
{

}

//============================================================
//	����������
//============================================================
HRESULT CInputPad::Init(void)
{
	// �����o�ϐ���������
	memset(&m_aVibration[0],		0, sizeof(m_aVibration));		// �o�C�u���
	memset(&m_aKeyStatePress[0],	0, sizeof(m_aKeyStatePress));	// �v���X���
	memset(&m_aKeyStateTrigger[0],	0, sizeof(m_aKeyStateTrigger));	// �g���K�[���
	memset(&m_aKeyStateRelease[0],	0, sizeof(m_aKeyStateRelease));	// �����[�X���

	// XInput�̃X�e�[�g��L����
	XInputEnable(true);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CInputPad::Uninit(void)
{
	// XInput�̃X�e�[�g�𖳌���
	XInputEnable(false);
}

//============================================================
//	�X�V����
//============================================================
void CInputPad::Update(void)
{
	// �ϐ��z���錾
	XINPUT_STATE aKeyState[MAX_PLAYER];	// �p�b�h�̓��͏��

	for (int nCntJoyKey = 0; nCntJoyKey < MAX_PLAYER; nCntJoyKey++)
	{ // �v���C���[�̍ő�l�������[�v

		if (XInputGetState(nCntJoyKey, &aKeyState[nCntJoyKey]) == ERROR_SUCCESS)
		{ // �R���g���[���[���ڑ�����Ă���ꍇ

			// �p�b�h�̃����[�X����ۑ� (�{�^��)
			m_aKeyStateRelease[nCntJoyKey].Gamepad.wButtons =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.wButtons | aKeyState[nCntJoyKey].Gamepad.wButtons) ^ aKeyState[nCntJoyKey].Gamepad.wButtons;

			// �p�b�h�̃����[�X����ۑ� (L2)
			m_aKeyStateRelease[nCntJoyKey].Gamepad.bLeftTrigger =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.bLeftTrigger | aKeyState[nCntJoyKey].Gamepad.bLeftTrigger) ^ aKeyState[nCntJoyKey].Gamepad.bLeftTrigger;

			// �p�b�h�̃����[�X����ۑ� (R2)
			m_aKeyStateRelease[nCntJoyKey].Gamepad.bRightTrigger =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.bRightTrigger | aKeyState[nCntJoyKey].Gamepad.bRightTrigger) ^ aKeyState[nCntJoyKey].Gamepad.bRightTrigger;

			// �p�b�h�̃g���K�[����ۑ� (�{�^��)
			m_aKeyStateTrigger[nCntJoyKey].Gamepad.wButtons =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.wButtons ^ aKeyState[nCntJoyKey].Gamepad.wButtons) & aKeyState[nCntJoyKey].Gamepad.wButtons;

			// �p�b�h�̃g���K�[����ۑ� (L2)
			m_aKeyStateTrigger[nCntJoyKey].Gamepad.bLeftTrigger =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.bLeftTrigger ^ aKeyState[nCntJoyKey].Gamepad.bLeftTrigger) & aKeyState[nCntJoyKey].Gamepad.bLeftTrigger;

			// �p�b�h�̃g���K�[����ۑ� (R2)
			m_aKeyStateTrigger[nCntJoyKey].Gamepad.bRightTrigger =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.bRightTrigger ^ aKeyState[nCntJoyKey].Gamepad.bRightTrigger) & aKeyState[nCntJoyKey].Gamepad.bRightTrigger;

			// �p�b�h�̃v���X����ۑ�
			m_aKeyStatePress[nCntJoyKey] = aKeyState[nCntJoyKey];

			// �o�C�u���[�V�����̍X�V
			UpdateVibration(&m_aVibration[nCntJoyKey], nCntJoyKey);
		}
	}
}

//============================================================
//	�o�C�u���[�V�����̍X�V����
//============================================================
void CInputPad::UpdateVibration(SVibration *pVibration, int nPlayer)
{
	switch (pVibration->type)
	{ // �U���̍X�V
	case TYPE_NONE:		// �������Ă��Ȃ�

		// �����𔲂���
		break;

	case TYPE_DEATH:	// ���S

		if (pVibration->nTime > 0)
		{ // �J�E���^�[�� 0���傫���ꍇ

			// �J�E���^�[�����Z
			pVibration->nTime--;

			// �U�����x���̐ݒ�
			pVibration->vibration.wLeftMotorSpeed  -= (WORD)(USHRT_MAX / VIB_TIME_DEATH);	// ��
			pVibration->vibration.wRightMotorSpeed -= (WORD)(USHRT_MAX / VIB_TIME_DEATH);	// �E
		}
		else
		{ // �J�E���^�[�� 0�ȉ��ɂȂ����ꍇ

			// �������Ȃ���Ԃɐݒ�
			pVibration->type = TYPE_NONE;

			// �������̏�����
			memset(&pVibration->vibration, 0, sizeof(pVibration->vibration));
		}

		// �����𔲂���
		break;

	default:	// ��L�ȊO

		if (pVibration->nTime >= 0)
		{ // �J�E���^�[�� 0���傫���ꍇ

			// �J�E���^�[�����Z
			pVibration->nTime--;
		}
		else
		{ // �J�E���^�[�� 0�ȉ��ɂȂ����ꍇ

			// �������Ȃ���Ԃɐݒ�
			pVibration->type = TYPE_NONE;

			// �������̏�����
			memset(&pVibration->vibration, 0, sizeof(pVibration->vibration));
		}

		// �����𔲂���
		break;
	}

	// ���[�^�[�̕␳
	if (pVibration->vibration.wLeftMotorSpeed < 0)
	{ // ���̐U���� 0��菬�����ꍇ

		// ���̃��[�^�[�̕␳
		pVibration->vibration.wLeftMotorSpeed = 0;
	}
	if (pVibration->vibration.wRightMotorSpeed < 0)
	{ // �E�̐U���� 0��菬�����ꍇ

		// �E�̃��[�^�[�̕␳
		pVibration->vibration.wRightMotorSpeed = 0;
	}

	// �o�C�u����ݒ�
	XInputSetState(nPlayer, &pVibration->vibration);
}

//============================================================
//	�o�C�u�ݒ菈��
//============================================================
void CInputPad::SetVibration(EType type, int nPlayer)
{
	// �o�C�u�̏�Ԃ������̂��̂ɐݒ�
	m_aVibration[nPlayer].type = type;

	switch (m_aVibration[nPlayer].type)
	{ // �o�C�u�̎��
	case TYPE_DAMAGE:	// �_���[�W���

		// �U�����x���̐ݒ�
		m_aVibration[nPlayer].vibration.wLeftMotorSpeed  = VIB_DAMAGE;	// ��
		m_aVibration[nPlayer].vibration.wRightMotorSpeed = VIB_DAMAGE;	// �E

		// �U�����Ԃ̐ݒ�
		m_aVibration[nPlayer].nTime = VIB_TIME;

		// �����𔲂���
		break;

	case TYPE_BIG_DAMAGE:	// ��_���[�W���

		// �U�����x���̐ݒ�
		m_aVibration[nPlayer].vibration.wLeftMotorSpeed  = VIB_BIG_DAMAGE;	// ��
		m_aVibration[nPlayer].vibration.wRightMotorSpeed = VIB_BIG_DAMAGE;	// �E

		// �U�����Ԃ̐ݒ�
		m_aVibration[nPlayer].nTime = VIB_TIME;

		// �����𔲂���
		break;

	case TYPE_DEATH:	// ���S���

		// �U�����x���̐ݒ�
		m_aVibration[nPlayer].vibration.wLeftMotorSpeed  = VIB_DEATH;	// ��
		m_aVibration[nPlayer].vibration.wRightMotorSpeed = VIB_DEATH;	// �E

		// �U�����Ԃ̐ݒ�
		m_aVibration[nPlayer].nTime = VIB_TIME_DEATH;

		// �����𔲂���
		break;

	case TYPE_WALLDASH:	// �Ǒ�����

		// �U�����x���̐ݒ�
		m_aVibration[nPlayer].vibration.wLeftMotorSpeed  = VIB_WALLDASH;	// ��
		m_aVibration[nPlayer].vibration.wRightMotorSpeed = VIB_WALLDASH;	// �E

		// �U�����Ԃ̐ݒ�
		m_aVibration[nPlayer].nTime = 0;

		// �����𔲂���
		break;

	case TYPE_FLAIL_CHAGE:	// �t���C�����ߏ��

		// �U�����x���̐ݒ�
		m_aVibration[nPlayer].vibration.wLeftMotorSpeed = VIB_FLAILCHAGE;	// ��
		m_aVibration[nPlayer].vibration.wRightMotorSpeed = VIB_FLAILCHAGE;	// �E

		// �U�����Ԃ̐ݒ�
		m_aVibration[nPlayer].nTime = 1;

		// �����𔲂���
		break;

	case TYPE_FLAIL_SHOT:	// �t���C���������

		// �U�����x���̐ݒ�
		m_aVibration[nPlayer].vibration.wLeftMotorSpeed = VIB_FLAILCHAGE;	// ��
		m_aVibration[nPlayer].vibration.wRightMotorSpeed = VIB_FLAILCHAGE;	// �E

		// �U�����Ԃ̐ݒ�
		m_aVibration[nPlayer].nTime = 30;

		// �����𔲂���
		break;

	case TYPE_FLAIL_FULL:	// �t���C���ő嗭�ߓ������

		// �U�����x���̐ݒ�
		m_aVibration[nPlayer].vibration.wLeftMotorSpeed = VIB_FLAILSHOT;	// ��
		m_aVibration[nPlayer].vibration.wRightMotorSpeed = VIB_FLAILSHOT;	// �E

		// �U�����Ԃ̐ݒ�
		m_aVibration[nPlayer].nTime = 60;

		// �����𔲂���
		break;
	}

	// �o�C�u����ݒ�
	XInputSetState(nPlayer, &m_aVibration[nPlayer].vibration);
}

//============================================================
//	�v���X�擾���� (�{�^��)
//============================================================
bool CInputPad::IsPress(EKey joyKey, int nPlayer)
{
	// ���͏���Ԃ�
	return (m_aKeyStatePress[nPlayer].Gamepad.wButtons & (1 << joyKey)) ? true : false;
}

//============================================================
//	�g���K�[�擾���� (�{�^��)
//============================================================
bool CInputPad::IsTrigger(EKey joyKey, int nPlayer)
{
	// ���͏���Ԃ�
	return (m_aKeyStateTrigger[nPlayer].Gamepad.wButtons & (1 << joyKey)) ? true : false;
}

//============================================================
//	�����[�X�擾���� (�{�^��)
//============================================================
bool CInputPad::IsRelease(EKey joyKey, int nPlayer)
{
	// ���͏���Ԃ�
	return (m_aKeyStateRelease[nPlayer].Gamepad.wButtons & (1 << joyKey)) ? true : false;
}

//============================================================
//	�S�v���X�擾���� (�{�^��)
//============================================================
bool CInputPad::IsPressAll(EKey joyKey)
{
	for (int nCntJoyKey = 0; nCntJoyKey < MAX_PLAYER; nCntJoyKey++)
	{ // �v���C���[�̍ő�l�������[�v

		if (m_aKeyStatePress[nCntJoyKey].Gamepad.wButtons & (1 << joyKey))
		{ // ���݂̃p�b�h�œ��͂��m�F���ꂽ�ꍇ

			// ���͏�񂠂��Ԃ�
			return true;
		}
	}

	// ���͏�񖳂���Ԃ�
	return false;
}

//============================================================
//	�S�g���K�[�擾���� (�{�^��)
//============================================================
bool CInputPad::IsTriggerAll(EKey joyKey)
{
	for (int nCntJoyKey = 0; nCntJoyKey < MAX_PLAYER; nCntJoyKey++)
	{ // �v���C���[�̍ő�l�������[�v

		if (m_aKeyStateTrigger[nCntJoyKey].Gamepad.wButtons & (1 << joyKey))
		{ // ���݂̃p�b�h�œ��͂��m�F���ꂽ�ꍇ

			// ���͏�񂠂��Ԃ�
			return true;
		}
	}

	// ���͏�񖳂���Ԃ�
	return false;
}

//============================================================
//	�S�����[�X�擾���� (�{�^��)
//============================================================
bool CInputPad::IsReleaseAll(EKey joyKey)
{
	for (int nCntJoyKey = 0; nCntJoyKey < MAX_PLAYER; nCntJoyKey++)
	{ // �v���C���[�̍ő�l�������[�v

		if (m_aKeyStateRelease[nCntJoyKey].Gamepad.wButtons & (1 << joyKey))
		{ // ���݂̃p�b�h�œ��͂��m�F���ꂽ�ꍇ

			// ���͏�񂠂��Ԃ�
			return true;
		}
	}

	// ���͏�񖳂���Ԃ�
	return false;
}

//============================================================
//	�v���X�擾���� (L�X�e�B�b�NX)
//============================================================
SHORT CInputPad::GetPressLStickX(int nPlayer)
{
	// �X�e�B�b�N�̌X���̒l��␳
	useful::LimitNum(m_aKeyStatePress[nPlayer].Gamepad.sThumbLX, (SHORT)-SHRT_MAX, (SHORT)SHRT_MAX);

	// �X�e�B�b�N�̌X���̒l��Ԃ� (���E)
	return m_aKeyStatePress[nPlayer].Gamepad.sThumbLX;
}

//============================================================
//	�v���X�擾���� (L�X�e�B�b�NY)
//============================================================
SHORT CInputPad::GetPressLStickY(int nPlayer)
{
	// �X�e�B�b�N�̌X���̒l��␳
	useful::LimitNum(m_aKeyStatePress[nPlayer].Gamepad.sThumbLY, (SHORT)-SHRT_MAX, (SHORT)SHRT_MAX);

	// �X�e�B�b�N�̌X���̒l��Ԃ� (�㉺)
	return -m_aKeyStatePress[nPlayer].Gamepad.sThumbLY;
}

//============================================================
//	�v���X�擾���� (R�X�e�B�b�NX)
//============================================================
SHORT CInputPad::GetPressRStickX(int nPlayer)
{
	// �X�e�B�b�N�̌X���̒l��␳
	useful::LimitNum(m_aKeyStatePress[nPlayer].Gamepad.sThumbRX, (SHORT)-SHRT_MAX, (SHORT)SHRT_MAX);

	// �X�e�B�b�N�̌X���̒l��Ԃ� (���E)
	return m_aKeyStatePress[nPlayer].Gamepad.sThumbRX;
}

//============================================================
//	�v���X�擾���� (R�X�e�B�b�NY)
//============================================================
SHORT CInputPad::GetPressRStickY(int nPlayer)
{
	// �X�e�B�b�N�̌X���̒l��␳
	useful::LimitNum(m_aKeyStatePress[nPlayer].Gamepad.sThumbRY, (SHORT)-SHRT_MAX, (SHORT)SHRT_MAX);

	// �X�e�B�b�N�̌X���̒l��Ԃ� (�㉺)
	return -m_aKeyStatePress[nPlayer].Gamepad.sThumbRY;
}

//============================================================
//	�v���X�擾���� (L�X�e�B�b�N����)
//============================================================
float CInputPad::GetPressLStickRot(int nPlayer)
{
	// �ϐ���錾
	float fRot = 0.0f;	// �X�e�B�b�N�X��

	// �X�e�b�N�̌X�����v�Z
	fRot = atan2f((float)GetPressLStickY(nPlayer), (float)GetPressLStickX(nPlayer));

	// �X�e�b�N�̌X����Ԃ�
	return fRot;
}

//============================================================
//	�v���X�擾���� (R�X�e�B�b�N����)
//============================================================
float CInputPad::GetPressRStickRot(int nPlayer)
{
	// �ϐ���錾
	float fRot = 0.0f;	// �X�e�B�b�N�X��

	// �X�e�b�N�̌X�����v�Z
	fRot = atan2f((float)GetPressRStickY(nPlayer), (float)GetPressRStickX(nPlayer));

	// �X�e�b�N�̌X����Ԃ�
	return fRot;
}

//============================================================
//	��������
//============================================================
CInputPad *CInputPad::Create(void)
{
	// �|�C���^��錾
	CInputPad *pPad = NULL;		// �p�b�h�����p

	if (pPad == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pPad = new CInputPad;	// �p�b�h
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pPad != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �p�b�h�̏�����
		if (FAILED(pPad->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pPad;
			pPad = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pPad;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CInputPad::Release(CInputPad *&prPad)
{
	if (prPad != NULL)
	{ // �g�p���̏ꍇ

		// �p�b�h�̏I��
		prPad->Uninit();

		// �������J��
		delete prPad;
		prPad = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}
