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
		TEXTURE_NUMCPU,		// CPU���e�N�X�`��
		TEXTURE_FLAILARROW,	// �t���C�����e�N�X�`��
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

	// �G���g���[��ԗ�
	enum EStateEntry
	{
		STATE_ENTRY_NONE_JOIN = 0,	// �����̖��������
		STATE_ENTRY_CPU_BG,			// CPU���w�i�̉��o���
		STATE_ENTRY_CPU_UI,			// CPU��UI�̉��o���
		STATE_ENTRY_NUMCPU,			// CPU���̕ύX���
		STATE_ENTRY_MAX				// ���̗񋓌^�̑���
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
	void UpdateFlail(void);			// �t���C���̍X�V
	void UpdateCpu(void);			// CPU�̍X�V
	void UpdateAddCpu(void);		// CPU�̉����Z�̍X�V
	void UpdatePlayerName(void);	// �v���C���[���̍X�V
	void UpdateControlUI(void);		// ����UI�̍X�V
	void UpdateStartUI(void);		// �J�nUI�̍X�V
	void UpdateStart(void);			// �J�n�̍X�V
	void UpdateArrow(void);			// ���̍X�V
	void UpdateCpuBG(void);			// CPU���w�i�̉��o�̍X�V
	void UpdateCpuUI(void);			// CPU��UI�̉��o�̍X�V
	void UpdateUIAll(void);			// UI�I�u�W�F�N�g�̑S�X�V
	void SetCpuObject(const bool bStart);		// CPU���o�̐ݒ�
	bool IsReadyOK(const int nNumEntry) const;	// �����ς݂��̔���擾

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CEntryRuleManager *m_pRuleManager;			// �G���g���[���[���̏��
	CValueUI	*m_apNumber[MAX_PLAYER];		// �v���C���[�i���o�[�̏��
	CObject2D	*m_apCpu[MAX_PLAYER];			// �v���C���[CPU�̏��
	CAnim2D		*m_apJoin[MAX_PLAYER];			// �v���C���[�Q���̏��
	CObject2D	*m_apFrame[MAX_PLAYER];			// �v���C���[�t���[���̏��
	CAnim2D		*m_apArrow[MAX_ENTRY_ARROW];	// ���̏��
	CAnim2D		*m_apFlailArrow[MAX_PLAYER][MAX_ENTRY_ARROW];	// �t���C�����̏��

	CObject2D	*m_pControl;	// ����\���̏��
	CObject2D	*m_pBG;			// �w�i�̏��
	CObject2D	*m_pStart;		// �J�n�\���̏��
	CValueUI	*m_pNumCpu;		// CPU���\���̏��
	EStateEntry	m_stateEntry;	// �G���g���[���
	EState	m_state;			// ���
	float	m_fScale;			// �g�嗦
	float	m_fSinStartAlpha;	// �J�n�\���̓�������
	float	m_fSinControlAlpha;	// ����\���̓�������
	float	m_fSinArrowAlpha;	// ���\���̓�������
};

#endif	// _ENTRY_MANAGER_H_
