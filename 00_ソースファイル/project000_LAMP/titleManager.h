//============================================================
//
//	�^�C�g���}�l�[�W���[�w�b�_�[ [titleManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TITLEMANAGER_H_
#define _TITLEMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �^�C�g���}�l�[�W���[�N���X
class CTitleManager
{
public:
	// ���S��
	enum ELogo
	{
		LOGO_TITLE = 0,	// ���S
		//LOGO_CRACK,	// �q�r
		LOGO_MAX		// ���̗񋓌^�̑���
	};

	// �I���
	enum ESelect
	{
		SELECT_GAME = 0,	// �Q�[���I��
		SELECT_MAX			// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_FADEOUT,	// �t�F�[�h�A�E�g���
		STATE_MOVE,		// �^�C�g���k�����
		STATE_WAIT,		// �J�ڑҋ@���
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CTitleManager();

	// �f�X�g���N�^
	~CTitleManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	// �ÓI�����o�֐�
	static CTitleManager *Create(void);	// ����
	static HRESULT Release(CTitleManager *&prTitleManager);	// �j��

private:
	// �����o�֐�
	void UpdateFade(void);	// �t�F�[�h�A�E�g
	void UpdateMove(void);	// �^�C�g���ړ�
	void UpdateStart(void);	// �J�ڌ���
	void ActSelect(void);	// �I�𑀍�
	void SkipStaging(void);	// ���o�X�L�b�v

	// �ÓI�����o�ϐ�
	static const char *mc_apLogoTextureFile[];		// ���S�e�N�X�`���萔
	static const char *mc_apSelectTextureFile[];	// �I���e�N�X�`���萔

	// �����o�ϐ�
	CObject2D *m_apLogo[LOGO_MAX];		// �^�C�g���\���̏��
	CObject2D *m_apSelect[SELECT_MAX];	// �I��\���̏��
	CObject2D *m_pFade;		// �t�F�[�h�̏��
	CObject2D *m_pSelectBG;	// �I��w�i�̏��
	EState m_state;		// ���
	float m_fScale;		// �^�C�g���g�嗦
	int m_nSelect;		// ���݂̑I��
	int m_nOldSelect;	// �O��̑I��
};

#endif	// _TITLEMANAGER_H_
