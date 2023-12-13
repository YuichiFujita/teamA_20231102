//============================================================
//
//	�G�f�B�b�g�A�C�e�������ʒu�w�b�_�[ [editItemPoint.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_ITEMPOINT_H_
#define _EDIT_ITEMPOINT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "editStage.h"
#include "ItemSpawnPoint.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditStageManager;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�A�C�e�������ʒu�N���X
class CEditItemPoint : public CEditStage
{
public:
	// �R���X�g���N�^
	CEditItemPoint();

	// �f�X�g���N�^
	~CEditItemPoint() override;

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
	void LookEffect(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);	// �A�C�e�������ʒu�̃G�t�F�N�g�\��

	void CreateItemPoint(void);		// �A�C�e�������ʒu����
	void LookAllItemPoint(void);	// �A�C�e�������ʒu�̑S�\��
};

#endif	// _EDIT_ITEMPOINT_H_
