//============================================================
//
//	�G���g���[�}�l�[�W���[�w�b�_�[ [entryManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENTRY_MANAGER_H_
#define _ENTRY_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "input.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_ENTRY_ARROW	(2)	// ���̑���

//************************************************************
//	�O���錾
//************************************************************
class CValueUI;				// ����UI�N���X
class CAnim2D;				// �A�j���[�V����2D�N���X
class CObject2D;			// �I�u�W�F�N�g2D�N���X
class CEntryRuleManager;	// �G���g���[���[���}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G���g���[�}�l�[�W���[�N���X
class CEntryManager
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_PLAYER = 0,	// PLAYER�e�N�X�`��
		TEXTURE_FRAME,		// �t���[���e�N�X�`��
		TEXTURE_CONTROL,	// ����\���e�N�X�`��
		TEXTURE_START,		// �J�n�\���e�N�X�`��
		TEXTURE_JOIN,		// �Q���󋵃e�N�X�`��
		TEXTURE_ARROW,		// ���e�N�X�`��
		TEXTURE_CPU,		// CPU�e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_ENTRY = 0,	// �G���g���[��t���
		STATE_RULE,			// ���[���ݒ���
		STATE_END,			// �I�����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �Q����
	enum EJoin
	{
		JOIN_OFF = 0,	// �Q��OFF
		JOIN_ON,		// �Q��ON
		JOIN_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEntryManager();

	// �f�X�g���N�^
	~CEntryManager();

	// �����o�֐�
	HRESULT Init(void);		// ������
	HRESULT Uninit(void);	// �I��
	void Update(void);		// �X�V
	HRESULT SetState(const EState state);	// ��Ԑݒ�

	// �ÓI�����o�֐�
	static CEntryManager *Create(void);	// ����
	static HRESULT Release(CEntryManager *&prEntryManager);	// �j��

private:
	// �����o�֐�
	void UpdateEntry(void);			// �G���g���[�̍X�V
	void UpdateCpu(void);			// CPU�̍X�V
	void UpdateAddCpu(void);		// CPU�̉����Z�̍X�V
	void UpdatePlayerName(void);	// �v���C���[���̍X�V
	void UpdateStart(void);			// �J�n�̍X�V
	void UpdateArrow(void);			// ���̍X�V
	bool IsReadyOK(const int nNumEntry) const;	// �����ς݂��̔���擾

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CEntryRuleManager *m_pRuleManager;		// �G���g���[���[���̏��
	CValueUI *m_apNumber[MAX_PLAYER];		// �v���C���[�i���o�[�̏��
	CAnim2D *m_apJoin[MAX_PLAYER];			// �v���C���[�Q���̏��
	CObject2D *m_apFrame[MAX_PLAYER];		// �v���C���[�t���[���̏��
	CAnim2D *m_apArrow[MAX_ENTRY_ARROW];	// ���̏��
	CObject2D *m_pControl;	// ����\���̏��
	CObject2D *m_pBG;		// �w�i�̏��
	CObject2D *m_pStart;	// �J�n�\���̏��
	CValueUI *m_pNumCpu;	// CPU���\���̏��
	EState m_state;			// ���
	float m_fSinAlpha;		// ��������
};

#endif	// _ENTRY_MANAGER_H_
