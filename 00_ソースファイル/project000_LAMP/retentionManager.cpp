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
	m_result = RESULT_NONE;	// �N���A��
	m_nTime  = 0;			// �o�߃^�C��
	m_nNumPlayer = 0;		// �v���C���[��
	memset(&m_aEntry[0], 0, sizeof(m_aEntry));	// �G���g���[��
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
	m_result = RESULT_NONE;	// �N���A��
	m_nTime  = 0;			// �o�߃^�C��
	m_nNumPlayer = 0;		// �v���C���[��

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
//	�N���A�󋵂̐ݒ菈��
//============================================================
void CRetentionManager::SetResult(const EResult result)
{
	if (result > RESULT_NONE && result < RESULT_MAX)
	{ // ���U���g���͈͓��̏ꍇ

		// �����̃N���A�󋵂�ݒ�
		m_result = result;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�N���A�󋵎擾����
//============================================================
CRetentionManager::EResult CRetentionManager::GetResult(void) const
{
	// �N���A�󋵂�Ԃ�
	return m_result;
}

//============================================================
//	�o�߃^�C���̐ݒ菈��
//============================================================
void CRetentionManager::SetTime(const long nTime)
{
	// �����̌o�߃^�C����ݒ�
	m_nTime = nTime;
}

//============================================================
//	�o�߃^�C���擾����
//============================================================
long CRetentionManager::GetTime(void) const
{
	// �o�߃^�C����Ԃ�
	return m_nTime;
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
