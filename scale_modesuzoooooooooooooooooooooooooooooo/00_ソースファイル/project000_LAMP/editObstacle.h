//============================================================
//
//	�G�f�B�b�g��Q���w�b�_�[ [editObstacle.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_OBSTACLE_H_
#define _EDIT_OBSTACLE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "editStage.h"
#include "obstacle.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditStageManager;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g��Q���N���X
class CEditObstacle : public CEditStage
{
public:
	// �R���X�g���N�^
	CEditObstacle();

	// �f�X�g���N�^
	~CEditObstacle() override;

	// ��Q�����\����
	struct SInfo
	{
		CObstacle::EType type;	// ��Q�����
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void DrawDebugControl(void) override;	// ����\���`��
	void DrawDebugInfo(void) override;		// ���\���`��
	void SaveInfo(void) override;			// ���ۑ�
	void LoadInfo(void) override;			// ���Ǎ�
	void Save(FILE *pFile) override;		// �ۑ�

private:
	// �����o�֐�
	void UpdateChangeType(void);	// ��ޕύX�̍X�V
	void CreateObstacle(void);		// ��Q������
	void ReleaseObstacle(void);		// ��Q���j��

	void DeleteCollisionObstacle(const bool bRelase);	// ��Q���̍폜����
	void InitAllColorObstacle(void);					// ��Q���̐F�S������

	// �ÓI�����o�ϐ�
	static SInfo m_save;	// �ۑ����

	// �����o�ϐ�
	CObstacle *m_pObstacle;	// ��Q�����
	SInfo m_obstacle;		// ��Q���z�u���
};

#endif	// _EDIT_OBSTACLE_H_
