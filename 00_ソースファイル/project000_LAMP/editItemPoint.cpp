//============================================================
//
//	�G�f�B�b�g�A�C�e�������ʒu���� [editItemPoint.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editItemPoint.h"
#include "manager.h"
#include "editStageManager.h"
#include "stage.h"
#include "effect3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_CREATE	(DIK_0)	// �����L�[
#define NAME_CREATE	("0")	// �����\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float	POS_EFFECT_RADIUS = 120.0f;	// �ʒu�\���̃G�t�F�N�g���a
	const int	EFFECT_LIFE = 10;			// �G�t�F�N�g����
}

//************************************************************
//	�e�N���X [CEditItemPoint] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditItemPoint::CEditItemPoint()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditItemPoint::~CEditItemPoint()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditItemPoint::Init(void)
{
#if _DEBUG

	// ������Ԃ�
	return S_OK;

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	�I������
//============================================================
void CEditItemPoint::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditItemPoint::Update(void)
{
#if _DEBUG

	// �|�C���^��錾
	CEditStageManager *pEdit = GetPtrEditStage();	// �G�f�B�b�g�X�e�[�W���
	if (pEdit == NULL)
	{ // �G�f�B�b�g�X�e�[�W�����݂��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	// �A�C�e�������ʒu�̐���
	CreateItemPoint();

	// �A�C�e�������ʒu�̑S�\��
	LookAllItemPoint();

	// �G�f�B�b�g�ʒu�̕\��
	LookEffect(pEdit->GetVec3Position(), XCOL_RED);

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditItemPoint::DrawDebugControl(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�v���b�N�̏��

	pDebug->Print(CDebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditItemPoint::DrawDebugInfo(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�v���b�N�̏��
}

//============================================================
//	���ۑ�����
//============================================================
void CEditItemPoint::SaveInfo(void)
{

}

//============================================================
//	���Ǎ�����
//============================================================
void CEditItemPoint::LoadInfo(void)
{

}

//============================================================
//	�ۑ�����
//============================================================
void CEditItemPoint::Save(FILE *pFile)
{
#if _DEBUG

	if (pFile != NULL)
	{ // �t�@�C�������݂���ꍇ

		// ���o���������o��
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#	�A�C�e�������ʒu�̔z�u���\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		// ���J�n�n�_�������o��
		fprintf(pFile, "STAGE_ITEMPOINTSET\n\n");

		for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
		{ // �D�揇�ʂ̑������J��Ԃ�
	
			// �|�C���^��錾
			CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g
	
			if (pObjectTop != NULL)
			{ // �擪�����݂���ꍇ
	
				// �|�C���^��錾
				CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p
	
				while (pObjCheck != NULL)
				{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�
		
					// �|�C���^��錾
					CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g
	
					if (pObjCheck->GetLabel() != CObject::LABEL_ITEMPOINT)
					{ // �I�u�W�F�N�g���x�����A�C�e�������ʒu�ł͂Ȃ��ꍇ
	
						// ���̃I�u�W�F�N�g�ւ̃|�C���^����
						pObjCheck = pObjectNext;
	
						// ���̌J��Ԃ��Ɉڍs
						continue;
					}

					// �A�C�e�������ʒu�̏����擾
					D3DXVECTOR3 posItemPoint = pObjCheck->GetVec3Position();	// �ʒu
	
					// ���������o��
					fprintf(pFile, "	ITEMPOINTSET\n");
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posItemPoint.x, posItemPoint.y, posItemPoint.z);
					fprintf(pFile, "	END_ITEMPOINTSET\n\n");

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;
				}
			}
		}

		// ���I���n�_�������o��
		fprintf(pFile, "END_STAGE_ITEMPOINTSET\n\n");
	}

#endif	// _DEBUG
}

//============================================================
//	�A�C�e�������ʒu�̃G�t�F�N�g�\������
//============================================================
void CEditItemPoint::LookEffect(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �A�C�e�������ʒu�ɃG�t�F�N�g��\��
	CEffect3D::Create(rPos, POS_EFFECT_RADIUS, CEffect3D::TYPE_NORMAL, EFFECT_LIFE, VEC3_ZERO, VEC3_ZERO, rCol);
}

//============================================================
//	�A�C�e�������ʒu�̐�������
//============================================================
void CEditItemPoint::CreateItemPoint(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���
	CEditStageManager *pEdit = GetPtrEditStage();	// �G�f�B�b�g�X�e�[�W���
	if (pEdit == NULL)
	{ // �G�f�B�b�g�X�e�[�W�����݂��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	// �ϐ���錾
	D3DXVECTOR3 posEdit = pEdit->GetVec3Position();	// �G�f�B�b�g�̈ʒu

	// �A�C�e�������ʒu��z�u
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		// �A�C�e�������ʒu�̐���
		CItemSpawnPoint *pItemPoint = CItemSpawnPoint::Create(posEdit);
		assert(pItemPoint != NULL);

		// ���ۑ���ݒ�
		pEdit->UnSave();
	}
}

//============================================================
//	�A�C�e�������ʒu�̑S�\��
//============================================================
void CEditItemPoint::LookAllItemPoint(void)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (pObjectTop != NULL)
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (pObjCheck != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_ITEMPOINT)
				{ // �I�u�W�F�N�g���x�����A�C�e�������ʒu�ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �ϐ���錾
				D3DXVECTOR3 posSpawn = pObjCheck->GetVec3Position();	// �A�C�e�������ʒu

				// �A�C�e�������ʒu�̃G�t�F�N�g�\��
				LookEffect
				( // ����
					posSpawn,	// �ʒu
					XCOL_CYAN	// �F
				);

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}
