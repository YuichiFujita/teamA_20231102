//============================================================
//
//	�G�f�B�b�g�X�e�[�W���� [editStage.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editStage.h"
#include "manager.h"
#include "editStageManager.h"
#include "editGround.h"
#include "editBlock.h"
#include "editObstacle.h"
#include "editSpawnPoint.h"

//************************************************************
//	�e�N���X [CEditStage] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditStage::CEditStage()
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pEdit = NULL;	// �G�f�B�b�g�X�e�[�W�̏��

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditStage::~CEditStage()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	��������
//============================================================
CEditStage *CEditStage::Create(CEditStageManager *pEdit, const EThing thing)
{
#if _DEBUG

	// �|�C���^��錾
	CEditStage *pEditStage = NULL;	// �G�f�B�b�g�X�e�[�W�����p

	if (pEditStage == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		switch (thing)
		{ // �z�u�����Ƃ̏���
		case THING_GROUND:
			pEditStage = new CEditGround;		// �G�f�B�b�g�n��
			break;

		case THING_BLOCK:
			pEditStage = new CEditBlock;		// �G�f�B�b�g�u���b�N
			break;

		case THING_OBSTACLE:
			pEditStage = new CEditObstacle;		// �G�f�B�b�g��Q��
			break;

		case THING_SPAWNPOINT:
			pEditStage = new CEditSpawnPoint;	// �G�f�B�b�g�����ʒu
			break;

		default:
			assert(false);
			break;
		}
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pEditStage != NULL)
	{ // �g�p����Ă���ꍇ

		// �G�f�B�b�g�X�e�[�W�̏���ݒ�
		pEditStage->m_pEdit = pEdit;
		
		// �G�f�B�b�g�X�e�[�W�̏�����
		if (FAILED(pEditStage->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pEditStage;
			pEditStage = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEditStage;
	}
	else { assert(false); return NULL; }	// �m�ێ��s

#else	// NDEBUG

	// NULL��Ԃ�
	return NULL;

#endif	// _DEBUG
}

//============================================================
//	�j������
//============================================================
HRESULT CEditStage::Release(CEditStage *&prEditStage)
{
#if _DEBUG

	if (prEditStage != NULL)
	{ // �g�p���̏ꍇ

		// �G�f�B�b�g�X�e�[�W�̏I��
		prEditStage->Uninit();

		// �������J��
		delete prEditStage;
		prEditStage = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	�G�f�B�b�g�X�e�[�W���擾����
//============================================================
CEditStageManager *CEditStage::GetPtrEditStage(void) const
{
#if _DEBUG

	// �G�f�B�b�g����Ԃ�
	return m_pEdit;

#else	// NDEBUG

	// NULL��Ԃ�
	return NULL;

#endif	// _DEBUG
}
