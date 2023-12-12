//============================================================
//
//	�G���g���[���[���}�l�[�W���[�w�b�_�[ [entryRuleManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENTRYRULE_MANAGER_H_
#define _ENTRYRULE_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_ARROW	(2)	// ���̑���

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X
class CAnim2D;		// �A�j���[�V����2D�N���X
class CMultiValue;	// �}���`�����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G���g���[���[���}�l�[�W���[�N���X
class CEntryRuleManager
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_RULE_TITLE = 0,	// ���[���^�C�g���e�N�X�`��
		TEXTURE_KILL,			// ���j�����e�N�X�`��
		TEXTURE_WIN,			// ���������e�N�X�`��
		TEXTURE_START,			// �J�n�{�^���e�N�X�`��
		TEXTURE_CONTROL,		// ����\���e�N�X�`��
		TEXTURE_ARROW,			// ���e�N�X�`��
		TEXTURE_MAX				// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_INIT = 0,		// ���������
		STATE_FADEIN,		// �t�F�[�h�C�����
		STATE_RULESET,		// ���[���ݒ���
		STATE_FADEOUT,		// �t�F�[�h�A�E�g���
		STATE_ENTRYBACK,	// �G���g���[�߂����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// ���[����
	enum ERule
	{
		RULE_WINPOINT = 0,	// �����|�C���g
		RULE_KILL,			// ���j����
		RULE_WIN,			// ��������
		RULE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEntryRuleManager();

	// �f�X�g���N�^
	~CEntryRuleManager();

	// �����o�֐�
	HRESULT Init(void);		// ������
	HRESULT Uninit(void);	// �I��
	void Update(void);		// �X�V

	// �ÓI�����o�֐�
	static CEntryRuleManager *Create(void);	// ����
	static HRESULT Release(CEntryRuleManager *&prEntryRuleManager);	// �j��

private:
	// �����o�֐�
	void UpdateFadeIn(void);	// �t�F�[�h�C���X�V
	void UpdateFadeOut(void);	// �t�F�[�h�A�E�g�X�V
	void UpdateControl(void);	// ����X�V
	void UpdateArrow(void);		// ���X�V

	void Select(void);		// �I�𑀍�
	void Decide(void);		// ���葀��
	void ChangeRule(void);	// ���[���ύX����
	void SetRule(void);		// ���[���ύX���f
	void SetEnableUI(const bool bDraw);	// UI�`��ݒ�
	void SetAlphaUI(const float fAlpha, const bool bFadeOut);	// UI�����x�ݒ�

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CAnim2D *m_apRuleTitle[RULE_MAX];	// ���[���^�C�g���̏��
	CAnim2D *m_apArrow[MAX_ARROW];		// ���̏��
	CMultiValue	*m_pWinPoint;	// �����|�C���g���̏��
	CAnim2D		*m_pKill;		// ���j�����̏��
	CAnim2D		*m_pWin;		// ���������̏��
	CObject2D	*m_pSelect;		// �I���̏��
	CObject2D	*m_pStart;		// �J�n�{�^���̏��
	CObject2D	*m_pControl;	// ����\���̏��
	CObject2D	*m_pFade;		// �t�F�[�h�̏��
	EState	m_state;			// ���
	int		m_nSelect;			// ���݂̑I��
	int		m_nOldSelect;		// �O��̑I��
	float	m_fSinControlAlpha;	// ����\���̓�������
	float	m_fSinArrowAlpha;	// ���\���̓�������
};

#endif	// _ENTRYRULE_MANAGER_H_
