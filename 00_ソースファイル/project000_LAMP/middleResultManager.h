//============================================================
//
//	���ԃ��U���g�}�l�[�W���[�w�b�_�[ [middleResultManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MIDDLE_RESULT_MANAGER_H_
#define _MIDDLE_RESULT_MANAGER_H_

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
// ���ԃ��U���g�}�l�[�W���[�N���X
class CMiddleResultManager
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_FADE = 0,	// �t�F�[�h�e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CMiddleResultManager();

	// �f�X�g���N�^
	~CMiddleResultManager();

	// �����o�֐�
	HRESULT Init(void);		// ������
	HRESULT Uninit(void);	// �I��
	void Update(void);		// �X�V

	// �ÓI�����o�֐�
	static CMiddleResultManager *Create(void);	// ����
	static HRESULT Release(CMiddleResultManager *&prMiddleResultManager);	// �j��

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CObject2D *m_pFade;	// �t�F�[�h�̏��
};

#endif	// _MIDDLE_RESULT_MANAGER_H_
