//============================================================
//
//	�G�f�B�b�g�����ʒu�w�b�_�[ [editSpawnPoint.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_SPAWNPOINT_H_
#define _EDIT_SPAWNPOINT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "editStage.h"
#include "spawnPoint.h"
#include "input.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditStageManager;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�����ʒu�N���X
class CEditSpawnPoint : public CEditStage
{
public:
	// �R���X�g���N�^
	CEditSpawnPoint();

	// �f�X�g���N�^
	~CEditSpawnPoint() override;

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
	void LookEffect	// �����ʒu�̃G�t�F�N�g�\��
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXCOLOR& rCol		// �F
	);

	void CreateSpawnPoint(void);	// �����ʒu����
	void UpdateChangeIdx(void);		// �C���f�b�N�X�ύX�̍X�V
	void LookAllSpawnPoint(void);	// �����ʒu�̑S�\��
	void CheckUseSpawnPoint(const int nMaxPoint, CObject **ppAllSpawnPoint);	// �����ʒu�̎g�p�m�F

	// �ÓI�����o�ϐ�
	static int m_nSave;	// �ۑ����

	// �����o�ϐ�
	CObject *m_apSpawnPoint[MAX_PLAYER];	// �����ʒu���
	int m_nIdxChange;	// �ύX�C���f�b�N�X
};

#endif	// _EDIT_SPAWNPOINT_H_
