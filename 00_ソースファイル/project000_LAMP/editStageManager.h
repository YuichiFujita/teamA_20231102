//============================================================
//
//	�G�f�B�b�g�X�e�[�W�}�l�[�W���[�w�b�_�[ [editStageManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_STAGEMANAGER_H_
#define _EDIT_STAGEMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "editStage.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�N���X
class CEditStageManager
{
public:
	// �R���X�g���N�^
	CEditStageManager();

	// �f�X�g���N�^
	~CEditStageManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void UnSave(void);	// ���ۑ��ݒ�
	void SetEnableEdit(const bool bEdit);		// �G�f�B�b�g�󋵐ݒ�
	bool IsEdit(void) const;					// �G�f�B�b�g�󋵎擾
	D3DXVECTOR3 GetVec3Position(void) const;	// �ʒu�擾
	D3DXVECTOR3 GetVec3Rotation(void) const;	// �����擾
	D3DXVECTOR3 GetVec3Sizing(void) const;		// �傫���擾

	// �ÓI�����o�֐�
	static CEditStageManager *Create(void);	// ����
	static HRESULT Release(CEditStageManager *&prEditStageManager);	// �j��

private:
	// �����o�֐�
	void UpdateChangeThing(void);	// �z�u���ύX�̍X�V
	void UpdateChangeMove(void);	// �ړ��ʕύX�̍X�V
	void UpdatePosition(void);		// �ʒu�X�V
	void UpdateRotation(void);		// �����X�V
	void UpdateSizing(void);		// �傫���X�V
	void DrawDebugControl(void);	// ����\���`��
	void DrawDebugInfo(void);		// ���\���`��
	void SaveStage(void);			// �X�e�[�W�ۑ�
	void Save(void);				// �ۑ�

	// �����o�ϐ�
	CEditStage	*m_pStage;		// �G�f�B�b�g�X�e�[�W�̏��
	CEditStage::EThing m_thing;	// �z�u��
	D3DXVECTOR3	m_pos;	// �ʒu
	D3DXVECTOR3	m_rot;	// ����
	D3DXVECTOR3	m_size;	// �傫��
	float m_fMove;		// �ʒu�ړ���
	bool m_bSave;		// �ۑ���
	bool m_bEdit;		// �G�f�B�b�g��
};

#endif	// _EDIT_STAGEMANAGER_H_
