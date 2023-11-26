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
#include "editStage.h"
#include "ground.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditStageManager;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�n�ՃN���X
class CEditGround : public CEditStage
{
public:
	// �R���X�g���N�^
	CEditGround();

	// �f�X�g���N�^
	~CEditGround() override;

	// �n�Տ��\����
	struct SInfo
	{
		CGround::EType type;	// �n�Վ��
		D3DXVECTOR3 size;		// �傫��
		D3DXVECTOR2 partX, partY, partZ;	// �e�N�X�`��������
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
	void UpdateSizing(void);		// �傫���X�V
	void UpdateTexPart(void);		// �e�N�X�`�������̍X�V
	void UpdateChangeType(void);	// ��ޕύX�̍X�V
	void CreateGround(void);		// �n�Ր���
	void ReleaseGround(void);		// �n�Քj��

	void DeleteCollisionGround(const bool bRelase);	// �n�Ղ̍폜����
	void InitAllColorGround(void);					// �n�Ղ̐F�S������

	// �ÓI�����o�ϐ�
	static SInfo m_save;	// �ۑ����

	// �����o�ϐ�
	CGround *m_pGround;	// �n�Տ��
	SInfo m_ground;		// �n�Քz�u���
};

#endif	// _EDIT_GROUND_H_
