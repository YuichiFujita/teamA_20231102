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
		TEXTURE_PLAYER = 0,	// PLAYER�e�N�X�`��
		TEXTURE_FRAME,		// �t���[���e�N�X�`��
		TEXTURE_CONTROL,	// ����\���e�N�X�`��
		TEXTURE_START,		// �J�n�\���e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
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


	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CObject2D *m_pControl;	// ����\���̏��
	CObject2D *m_pStart;	// �J�n�\���̏��
};

#endif	// _ENTRYRULE_MANAGER_H_
