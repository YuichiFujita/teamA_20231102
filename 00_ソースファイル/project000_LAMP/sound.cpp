//============================================================
//
//	�T�E���h���� [sound.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sound.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const CSound::SSoundInfo CSound::mc_aSoundInfo[LABEL_MAX] =	// �T�E���h���
{
	{ "data\\BGM\\Title.wav", -1 },	// BGM (�ėp)
	{ "data\\BGM\\Entry.wav", -1 },	// BGM (�G���g���[)
	{ "data\\BGM\\bgm_tutorial.wav", -1 },	// BGM (�`���[�g���A��)
	{ "data\\BGM\\VS_Virus.wav", -1 },		// BGM (�Q�[��)
	{ "data\\BGM\\Grave.wav", -1 },		// BGM (�Q�[��)
	{ "data\\BGM\\Soldiers_Tears.wav", -1 },		// BGM (�Q�[��)
	{ "data\\BGM\\Result.wav", -1 },		// BGM (���U���g)


	{ "data\\SE\\select000.wav",  0 },		// �I�𑀍쉹00
	{ "data\\SE\\select001.wav",  0 },		// �I�𑀍쉹01
	{ "data\\SE\\decision000.wav",  0 },	// ���艹00
	{ "data\\SE\\decision001.wav",  0 },	// ���艹01
	{ "data\\SE\\decision002.wav",  0 },	// ���艹02
	{ "data\\SE\\vibration000.wav",  0 },	// �X�}�z�o�C�u��
	{ "data\\SE\\message000.wav",  0 },		// ���b�Z�[�W��
	{ "data\\SE\\spawn000.wav",  0 },		// ������
	{ "data\\SE\\save000.wav",  0 },		// �Z�[�u��
	{ "data\\SE\\walk000.wav",  0 },		// ���s���i�r���j
	{ "data\\SE\\walk001.wav",  0 },		// ���s���i��Q���j
	{ "data\\SE\\jump000.wav",  0 },		// �W�����v
	{ "data\\SE\\slideing000.wav",  0 },	// �X���C�f�B���O
	{ "data\\SE\\jumppad000.wav",  0 },		// �W�����v�p�b�h
	{ "data\\SE\\stick000.wav",  0 },		// ��������
	{ "data\\SE\\destruct1.wav",  0 },			// �Ԃ��艹
	{ "data\\SE\\destruct2.wav",  0 },			// �Ԃ��艹2
	{ "data\\SE\\chain2.wav",  0 },			// �U�鉹
};

//************************************************************
//	�e�N���X [CSound] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSound::CSound()
{
	// �����o�ϐ����N���A
	m_pXAudio2			= NULL;	// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	m_pMasteringVoice	= NULL;	// �}�X�^�[�{�C�X
	memset(&m_apSourceVoice[0],	0, sizeof(m_apSourceVoice));	// �\�[�X�{�C�X
	memset(&m_apDataAudio[0],	0, sizeof(m_apDataAudio));		// �I�[�f�B�I�f�[�^
	memset(&m_aSizeAudio[0],	0, sizeof(m_aSizeAudio));		// �I�[�f�B�I�f�[�^�T�C�Y
}

//============================================================
//	�f�X�g���N�^
//============================================================
CSound::~CSound()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSound::Init(HWND hWnd)
{
	// �����o�ϐ���������
	m_pXAudio2			= NULL;	// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	m_pMasteringVoice	= NULL;	// �}�X�^�[�{�C�X
	memset(&m_apSourceVoice[0],	0, sizeof(m_apSourceVoice));	// �\�[�X�{�C�X
	memset(&m_apDataAudio[0],	0, sizeof(m_apDataAudio));		// �I�[�f�B�I�f�[�^
	memset(&m_aSizeAudio[0],	0, sizeof(m_aSizeAudio));		// �I�[�f�B�I�f�[�^�T�C�Y

	// �ϐ���錾
	HRESULT hr;		// �I���m�F�p

	// COM���C�u�����̏�����
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "COM���C�u�����̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(m_pXAudio2 != NULL)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(mc_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{ // �t�@�C���|�C���^��擪�Ɉړ�

			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �\�[�X�{�C�X�̐���
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = mc_aSoundInfo[nCntSound].nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSound::Uninit(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = NULL;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;
	
	if (m_pXAudio2 != NULL)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//============================================================
//	�Z�O�����g�Đ� (�Đ����Ȃ��~)
//============================================================
HRESULT CSound::Play(ELabel label)
{
	// �ϐ���錾
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = mc_aSoundInfo[label].nCntLoop;

	// ��Ԏ擾
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // �Đ���

		// �ꎞ��~
		m_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	m_apSourceVoice[label]->Start(0);

	return S_OK;
}

//============================================================
//	�Z�O�����g��~ (���x���w��)
//============================================================
void CSound::Stop(ELabel label)
{
	// �ϐ���錾
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // �Đ���

		// �ꎞ��~
		m_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//============================================================
//	�Z�O�����g��~ (�S��)
//============================================================
void CSound::Stop(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//============================================================
//	��������
//============================================================
CSound *CSound::Create(HWND hWnd)
{
	// �|�C���^��錾
	CSound *pSound = NULL;	// �T�E���h�����p

	if (pSound == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// ���������m��
		pSound = new CSound;	// �T�E���h
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pSound != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �T�E���h�̏�����
		if (FAILED(pSound->Init(hWnd)))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pSound;
			pSound = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pSound;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CSound::Release(CSound *&prSound)
{
	if (prSound != NULL)
	{ // �g�p���̏ꍇ

		// �T�E���h�̏I��
		prSound->Uninit();

		// �������J��
		delete prSound;
		prSound = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�`�����N�̃`�F�b�N
//============================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	// �ϐ���錾
	HRESULT hr = S_OK;	// �I���m�F�p
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{ // �t�@�C���|�C���^��擪�Ɉړ�

		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{ // �`�����N�̓ǂݍ���

			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{ // �`�����N�f�[�^�̓ǂݍ���

			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':

			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;

			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{ // �t�@�C���^�C�v�̓ǂݍ���

				hr = HRESULT_FROM_WIN32(GetLastError());
			}

			break;

		default:

			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{ // �t�@�C���|�C���^���`�����N�f�[�^���ړ�

				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//============================================================
//	�`�����N�f�[�^�̓ǂݍ���
//============================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	// �ϐ���錾
	DWORD dwRead;
	
	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{ // �t�@�C���|�C���^���w��ʒu�܂ňړ�

		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{ // �f�[�^�̓ǂݍ���

		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}
