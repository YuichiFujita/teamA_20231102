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
		TEXTURE_ = 0,	// �e�N�X�`��
		TEXTURE_MAX		// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_FADEIN = 0,	// �t�F�[�h�C�����
		STATE_WAIT,			// �ҋ@���
		STATE_FADEOUT,		// �t�F�[�h�A�E�g���
		STATE_ENTRYBACK,	// �G���g���[�߂����
		STATE_MAX			// ���̗񋓌^�̑���
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

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CObject2D *m_pFade;	// �t�F�[�h�̏��
	EState m_state;		// ���
};

#endif	// _ENTRYRULE_MANAGER_H_
