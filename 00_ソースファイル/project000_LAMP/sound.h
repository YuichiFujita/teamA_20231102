//============================================================
//
//	�T�E���h�w�b�_�[ [sound.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�N���X��`
//************************************************************
// �T�E���h�N���X
class CSound
{
public:
	// �R���X�g���N�^
	CSound();

	// �f�X�g���N�^
	~CSound();

	// �T�E���h���x����
	enum ELabel
	{
		LABEL_BGM_GENERAL = 0,	// BGM (�ėp)
		LABEL_BGM_ENTRY,		// BGM (�G���g���[)
		LABEL_BGM_TUTORIAL,		// BGM (�`���[�g���A��)
		LABEL_BGM_GAME,			// BGM (�Q�[��)
		LABEL_BGM_GAME_1,			// BGM (�Q�[��1)
		LABEL_BGM_GAME_2,			// BGM (�Q�[��2)
		LABEL_BGM_RESULT,			// BGM (���U���g)

		LABEL_SE_SELECT_000,	// �I�𑀍쉹00
		LABEL_SE_SELECT_001,	// �I�𑀍쉹01
		LABEL_SE_DECISION_000,	// ���艹00
		LABEL_SE_DECISION_001,	// ���艹01
		LABEL_SE_DECISION_002,	// ���艹02
		LABEL_SE_VIBRATION,		// �X�}�z�o�C�u��
		LABEL_SE_MESSAGE,		// ���b�Z�[�W��
		LABEL_SE_SPAWN,			// ������
		LABEL_SE_SAVE,			// �Z�[�u��
		LABEL_SE_WALK_BUILD,	// ���s���i�r���j
		LABEL_SE_WALK_OBS,		// ���s���i��Q���j
		LABEL_SE_JUMP,			// �W�����v
		LABEL_SE_SLIDE,			// �X���C�f�B���O
		LABEL_SE_JUMPPAD,		// �W�����v�p�b�h
		LABEL_SE_STICK,			// ��������
		LABEL_SE_HIT,			// �Ԃ��艹
		LABEL_SE_HIT2,			// �Ԃ��艹2
		LABEL_SE_SWING,			// �U�鉹
		LABEL_SE_DIVE,			// �_�C�u��
		LABEL_SE_NOISE,			// �m�C�Y��
		LABEL_SE_FIGHT,			// �퓬��
		LABEL_MAX,				// ���̗񋓌^�̑���
	};

	// �T�E���h���\����
	struct SSoundInfo
	{
		char *pFilename;	// �t�@�C����
		int   nCntLoop;		// ���[�v�J�E���g
	};

	// �����o�֐�
	HRESULT Init(HWND hWnd);	// ������
	void Uninit(void);			// �I��
	HRESULT Play(ELabel label);	// �Đ�
	void Stop(ELabel label);	// ��~
	void Stop(void);			// �S��~

	// �ÓI�����o�֐�
	static CSound *Create(HWND hWnd);			// ����
	static HRESULT Release(CSound *&pSound);	// �j��

private:
	// �����o�֐�
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);	// �`�����N�`�F�b�N
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// �`�����N�f�[�^�Ǎ�

	// �����o�ϐ�
	IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice    *m_apSourceVoice[LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[LABEL_MAX];						// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[LABEL_MAX];						// �I�[�f�B�I�f�[�^�T�C�Y
	static const SSoundInfo mc_aSoundInfo[LABEL_MAX];	// �T�E���h���萔
};

#endif	// _SOUND_H_
