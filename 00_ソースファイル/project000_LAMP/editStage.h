//============================================================
//
//	�G�f�B�b�g�X�e�[�W�w�b�_�[ [editStage.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_STAGE_H_
#define _EDIT_STAGE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditStageManager;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�X�e�[�W�N���X
class CEditStage
{
public:
	// �z�u����
	enum EThing
	{
		THING_GROUND = 0,	// �n��
		THING_BLOCK,		// �u���b�N
		THING_OBSTACLE,		// ��Q��
		THING_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEditStage();

	// �f�X�g���N�^
	virtual ~CEditStage();

	// �������z�֐�
	virtual HRESULT Init(void)	= 0;	// ������
	virtual void Uninit(void)	= 0;	// �I��
	virtual void Update(void)	= 0;	// �X�V
	virtual void DrawDebugControl(void) = 0;	// ����\���`��
	virtual void DrawDebugInfo(void) = 0;		// ���\���`��
	virtual void SaveInfo(void) = 0;			// ���ۑ�
	virtual void LoadInfo(void) = 0;			// ���Ǎ�
	virtual void Save(FILE *pFile) = 0;			// �ۑ�

	// �ÓI�����o�֐�
	static CEditStage *Create(CEditStageManager *pEdit, const EThing thing);	// ����
	static HRESULT Release(CEditStage *&prEditGround);	// �j��

	// �����o�֐�
	CEditStageManager *GetPtrEditStage(void) const;	// �G�f�B�b�g�X�e�[�W���擾

private:
	// �����o�ϐ�
	CEditStageManager *m_pEdit;	// �G�f�B�b�g�X�e�[�W�̏��
};

#endif	// _EDIT_STAGE_H_
