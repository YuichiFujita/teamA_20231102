//============================================================
//
//	�t�F�[�h�w�b�_�[ [fade.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FADE_H_
#define _FADE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �t�F�[�h�N���X
class CFade
{
public:
	// �t�F�[�h��ԗ�
	enum EFade
	{
		FADE_NONE = 0,	// �������Ă��Ȃ����
		FADE_WAIT,		// �t�F�[�h�]�C���
		FADE_IN,		// �t�F�[�h�C�����
		FADE_OUT,		// �t�F�[�h�A�E�g���
		FADE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CFade();

	// �f�X�g���N�^
	~CFade();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Set(const CScene::EMode mode, const int nWait);	// ���V�[���ւ̃t�F�[�h�ݒ�
	EFade GetState(void) const;	// �t�F�[�h��Ԏ擾

	// �ÓI�����o�֐�
	static CFade *Create(void);				// ����
	static HRESULT Release(CFade *&pFade);	// �j��

private:
	// �����o�ϐ�
	CObject2D *m_pObject2D;		// �t�F�[�h�\���̏��
	EFade m_fade;				// �t�F�[�h���
	CScene::EMode m_modeNext;	// ���̃V�[�����[�h
	int m_nCounterWait;			// �]�C�Ǘ��J�E���^�[
};

#endif	// _FADE_H_
