//============================================================
//
//	�G�f�B�b�g�u���b�N�w�b�_�[ [editBlock.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_BLOCK_H_
#define _EDIT_BLOCK_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "editStage.h"
#include "block.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditStageManager;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�u���b�N�N���X
class CEditBlock : public CEditStage
{
public:
	// �R���X�g���N�^
	CEditBlock();

	// �f�X�g���N�^
	~CEditBlock() override;

	// �u���b�N���\����
	struct SInfo
	{
		CBlock::EType type;	// �u���b�N���
		D3DXVECTOR3 size;	// �傫��
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
	void CreateBlock(void);			// �u���b�N����
	void ReleaseBlock(void);		// �u���b�N�j��

	void DeleteCollisionBlock(const bool bRelase);	// �u���b�N�̍폜����
	void InitAllColorBlock(void);					// �u���b�N�̐F�S������

	// �ÓI�����o�ϐ�
	static SInfo m_save;	// �ۑ����

	// �����o�ϐ�
	CBlock *m_pBlock;	// �u���b�N���
	SInfo m_block;		// �u���b�N�z�u���
};

#endif	// _EDIT_BLOCK_H_
