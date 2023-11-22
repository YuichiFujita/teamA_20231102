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
	CEditStage *pEditGround = NULL;	// �G�f�B�b�g�X�e�[�W�����p

	if (pEditGround == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		switch (thing)
		{ // �z�u�����Ƃ̏���
		case THING_GROUND:
			pEditGround = new CEditGround;		// �G�f�B�b�g�n��
			break;

		case THING_BLOCK:
			pEditGround = new CEditBlock;		// �G�f�B�b�g�u���b�N
			break;

		case THING_OBSTACLE:
			pEditGround = new CEditObstacle;	// �G�f�B�b�g��Q��
			break;

		default:
			assert(false);
			break;
		}
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pEditGround != NULL)
	{ // �g�p����Ă���ꍇ

		// �G�f�B�b�g�X�e�[�W�̏���ݒ�
		pEditGround->m_pEdit = pEdit;
		
		// �G�f�B�b�g�X�e�[�W�̏�����
		if (FAILED(pEditGround->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pEditGround;
			pEditGround = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEditGround;
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
HRESULT CEditStage::Release(CEditStage *&prEditGround)
{
#if _DEBUG

	if (prEditGround != NULL)
	{ // �g�p���̏ꍇ

		// �G�f�B�b�g�X�e�[�W�̏I��
		prEditGround->Uninit();

		// �������J��
		delete prEditGround;
		prEditGround = NULL;

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
