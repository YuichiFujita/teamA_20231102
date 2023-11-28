//============================================================
//
//	�Q�[���}�l�[�W���[���� [gameManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gameManager.h"
#include "manager.h"
#include "scene.h"
#include "sceneGame.h"
#include "camera.h"
#include "player.h"
#include "retentionManager.h"
#include "editStageManager.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CEditStageManager *CGameManager::m_pEditStage = NULL;	// �G�f�B�b�g�X�e�[�W�̏��

//************************************************************
//	�e�N���X [CGameManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGameManager::CGameManager()
{
	// �����o�ϐ����N���A
	m_state = STATE_NONE;	// ���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CGameManager::~CGameManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGameManager::Init(void)
{
	// �����o�ϐ���������
	m_state = STATE_NORMAL;	// ���

	// ���������L���O��������
	CManager::GetInstance()->GetRetentionManager()->InitSurvivalRank();

#if _DEBUG

	if (m_pEditStage == NULL)
	{ // �G�f�B�b�g�X�e�[�W���g�p����Ă��Ȃ��ꍇ

		// �G�f�B�b�g�X�e�[�W�̐���
		m_pEditStage = CEditStageManager::Create();
		if (m_pEditStage == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}
	}
	else { assert(false); }	// �g�p�ς�

#endif	// _DEBUG

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGameManager::Uninit(void)
{
	if (m_pEditStage != NULL)
	{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

		// �G�f�B�b�g�X�e�[�W�̔j��
		CEditStageManager::Release(m_pEditStage);
	}
}

//============================================================
//	�X�V����
//============================================================
void CGameManager::Update(void)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:

		// ����

		break;

	case STATE_NORMAL:

		// ����

		break;

	case STATE_END:

		// ����

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (m_pEditStage != NULL)
	{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

		// �G�f�B�b�g�X�e�[�W�̍X�V
		m_pEditStage->Update();
	}
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CGameManager::SetState(const EState state)
{
	if (state > NONE_IDX && state < STATE_MAX)
	{ // �͈͓��̏ꍇ

		// ��Ԃ�ݒ�
		m_state = state;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	��Ԏ擾����
//============================================================
CGameManager::EState CGameManager::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	��������
//============================================================
CGameManager *CGameManager::Create(void)
{
	// �|�C���^��錾
	CGameManager *pGameManager = NULL;	// �Q�[���}�l�[�W���[�����p

	if (pGameManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pGameManager = new CGameManager;	// �Q�[���}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pGameManager != NULL)
	{ // �g�p����Ă���ꍇ
		
		// �Q�[���}�l�[�W���[�̏�����
		if (FAILED(pGameManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pGameManager;
			pGameManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pGameManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CGameManager::Release(CGameManager *&prGameManager)
{
	if (prGameManager != NULL)
	{ // �g�p���̏ꍇ

		// �Q�[���}�l�[�W���[�̏I��
		prGameManager->Uninit();

		// �������J��
		delete prGameManager;
		prGameManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�G�f�B�b�g�X�e�[�W�擾����
//============================================================
CEditStageManager *CGameManager::GetEditStage(void)
{
	// �G�f�B�b�g�X�e�[�W�̏���Ԃ�
	return m_pEditStage;
}
