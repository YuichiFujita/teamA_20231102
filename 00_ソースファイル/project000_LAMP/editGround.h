//============================================================
//
//	�G�f�B�b�g�n�Ճw�b�_�[ [editGround.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_GROUND_H_
#define _EDIT_GROUND_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "ground.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditStageManager;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�n�ՃN���X
class CEditGround
{
public:
	// �R���X�g���N�^
	CEditGround();

	// �f�X�g���N�^
	~CEditGround();

	// �n�Տ��\����
	struct SInfo
	{
		CGround *pGround;		// �n�Տ��
		CGround::EType type;	// �n�Վ��
	};

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SetDisp(const bool bDisp);	// �\���ݒ�
	void DrawDebugControl(void);	// ����\���`��
	void DrawDebugInfo(void);		// ���\���`��
	void Save(FILE *pFile);			// �ۑ�

	// �ÓI�����o�֐�
	static CEditGround *Create(CEditStageManager *pEdit);	// ����
	static HRESULT Release(CEditGround *&prEditGround);	// �j��

private:
	// �����o�֐�
	void UpdateChangeType(void);	// ��ޕύX�̍X�V
	void CreateGround(void);		// �n�Ր���
	void ReleaseGround(void);		// �n�Քj��

	void DeleteCollisionGround(const bool bRelase);	// �n�Ղ̍폜����
	void InitAllColorGround(void);					// �n�Ղ̐F�S������

	// �����o�ϐ�
	CEditStageManager *m_pEdit;	// �G�f�B�b�g�X�e�[�W�̏��
	SInfo m_ground;	// �n�Քz�u���
};

#endif	// _EDIT_GROUND_H_
