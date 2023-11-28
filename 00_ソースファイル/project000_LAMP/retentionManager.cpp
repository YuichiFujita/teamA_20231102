//============================================================
//
//	�f�[�^�ۑ��}�l�[�W���[���� [retentionManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "retentionManager.h"
#include "manager.h"

//************************************************************
//	�e�N���X [CRetentionManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRetentionManager::CRetentionManager()
{
	// �����o�ϐ����N���A
	memset(&m_aEntry[0], 0, sizeof(m_aEntry));	// �G���g���[��
	m_stateKill		= KILL_LIFE;	// ��������
	m_stateWin		= WIN_SURVIVE;	// ��������
	m_nNumPlayer	= 0;			// �v���C���[��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CRetentionManager::~CRetentionManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CRetentionManager::Init(void)
{
	// �����o�ϐ���������
	memset(&m_aEntry[0], 0, sizeof(m_aEntry));	// �G���g���[��
	m_stateKill		= KILL_KNOCK;	// ��������
	m_stateWin		= WIN_SURVIVE;	// ��������
	m_nNumPlayer	= 0;			// �v���C���[��

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		// �G���g���[�󋵂�������
		m_aEntry[nCntEntry] = false;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CRetentionManager::Uninit(void)
{

}

//============================================================
//	��������
//============================================================
CRetentionManager *CRetentionManager::Create(void)
{
	// �|�C���^��錾
	CRetentionManager *pRetentionManager = NULL;	// �f�[�^�ۑ��}�l�[�W���[�����p

	if (pRetentionManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pRetentionManager = new CRetentionManager;	// �f�[�^�ۑ��}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pRetentionManager != NULL)
	{ // �g�p����Ă���ꍇ
		
		// �f�[�^�ۑ��}�l�[�W���[�̏�����
		if (FAILED(pRetentionManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pRetentionManager;
			pRetentionManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pRetentionManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CRetentionManager::Release(CRetentionManager *&prRetentionManager)
{
	if (prRetentionManager != NULL)
	{ // �g�p���̏ꍇ

		// �f�[�^�ۑ��}�l�[�W���[�̏I��
		prRetentionManager->Uninit();

		// �������J��
		delete prRetentionManager;
		prRetentionManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	���������̐ݒ菈��
//============================================================
void CRetentionManager::SetKillState(const EKill kill)
{
	if (kill > NONE_IDX && kill < KILL_MAX)
	{ // �����������͈͓��̏ꍇ

		// �����̓���������ݒ�
		m_stateKill = kill;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	���������擾����
//============================================================
CRetentionManager::EKill CRetentionManager::GetKillState(void) const
{
	// ����������Ԃ�
	return m_stateKill;
}

//============================================================
//	���������̐ݒ菈��
//============================================================
void CRetentionManager::SetWinState(const EWin win)
{
	if (win > NONE_IDX && win < KILL_MAX)
	{ // �����������͈͓��̏ꍇ

		// �����̏���������ݒ�
		m_stateWin = win;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	���������擾����
//============================================================
CRetentionManager::EWin CRetentionManager::GetWinState(void) const
{
	// ����������Ԃ�
	return m_stateWin;
}

//============================================================
//	�v���C�l���̐ݒ菈��
//============================================================
void CRetentionManager::SetNumPlayer(const int nNum)
{
	// �v���C�l����ݒ�
	m_nNumPlayer = nNum;
}

//============================================================
//	�v���C�l���擾����
//============================================================
int CRetentionManager::GetNumPlayer(void) const
{
	// �v���C�l����Ԃ�
	return m_nNumPlayer;
}

//============================================================
//	�S�G���g���[�󋵂̐ݒ菈��
//============================================================
void CRetentionManager::AllSetEnableEntry(const bool bEntry)
{
	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		// �G���g���[�󋵂�ݒ�
		m_aEntry[nCntEntry] = bEntry;
	}
}

//============================================================
//	�G���g���[�̐ݒ菈��
//============================================================
void CRetentionManager::SetEnableEntry(const int nID, const bool bEntry)
{
	// �����C���f�b�N�X�̃G���g���[�󋵂�ݒ�
	m_aEntry[nID] = bEntry;
}

//============================================================
//	�G���g���[�擾����
//============================================================
bool CRetentionManager::IsEntry(const int nID) const
{
	// �����C���f�b�N�X�̃G���g���[�󋵂�Ԃ�
	return m_aEntry[nID];
}
