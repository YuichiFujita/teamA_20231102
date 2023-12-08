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
#include "player.h"

//************************************************************
//	�e�N���X [CRetentionManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRetentionManager::CRetentionManager()
{
	// �����o�ϐ����N���A
	memset(&m_aSurvivalRank[0], 0, sizeof(m_aSurvivalRank));	// �~���̐��������L���O
	memset(&m_aWinRank[0], 0, sizeof(m_aWinRank));				// �~���̏��������L���O
	memset(&m_aPlayerWin[0], 0, sizeof(m_aPlayerWin));			// �v���C���[�|�C���g��
	memset(&m_aEntry[0], 0, sizeof(m_aEntry));					// �G���g���[��
	m_stateKill		= KILL_LIFE;	// ��������
	m_stateWin		= WIN_SURVIVE;	// ��������
	m_nNumPlayer	= 0;			// �v���C���[��
	m_nNumSurvival	= 0;			// �����v���C���[��
	m_nWinPoint		= 0;			// �����|�C���g��
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
	memset(&m_aSurvivalRank[0], 0, sizeof(m_aSurvivalRank));	// �~���̐��������L���O
	memset(&m_aWinRank[0], 0, sizeof(m_aWinRank));				// �~���̏��������L���O
	memset(&m_aPlayerWin[0], 0, sizeof(m_aPlayerWin));			// �v���C���[�|�C���g��
	memset(&m_aEntry[0], 0, sizeof(m_aEntry));					// �G���g���[��
	m_stateKill		= KILL_LIFE;	// ��������
	m_stateWin		= WIN_SURVIVE;	// ��������
	m_nNumPlayer	= 0;			// �v���C���[��
	m_nNumSurvival	= 0;			// �����v���C���[��
	m_nWinPoint		= 0;			// �����|�C���g��

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
//	�����l���̏���������
//============================================================
void CRetentionManager::InitNumSurvival(void)
{
	// �v���C���[�l�����𐶑��l���ɐݒ�
	m_nNumSurvival = m_nNumPlayer;
}

//============================================================
//	�����l���擾����
//============================================================
int CRetentionManager::GetNumSurvival(void) const
{
	// �����l����Ԃ�
	return m_nNumSurvival;
}

//============================================================
//	�����|�C���g�̐ݒ菈��
//============================================================
void CRetentionManager::SetWinPoint(const int nWinPoint)
{
	// �����̏����|�C���g��ݒ�
	m_nWinPoint = nWinPoint;
}

//============================================================
//	�����|�C���g�擾����
//============================================================
int CRetentionManager::GetWinPoint(void) const
{
	// �����|�C���g��Ԃ�
	return m_nWinPoint;
}

//============================================================
//	�Q�[���J�n���̏���������
//============================================================
void CRetentionManager::InitGame(void)
{
	// �����|�C���g����������
	m_nWinPoint = 0;

	// ���������L���O��������
	InitSurvivalRank();

	// ���������L���O��������
	InitWinRank();

	// �v���C���[�|�C���g����������
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		m_aPlayerWin[nCntPlayer] = 0;
	}
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

//============================================================
//	���������L���O����������
//============================================================
void CRetentionManager::InitWinRank(void)
{
	// ���������L���O��������
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		m_aWinRank[nCntPlayer] = NONE_IDX;
	}
}

//============================================================
//	���������L���O�\�[�g����
//============================================================
void CRetentionManager::SortWinRank(void)
{
	// TODO�F���������L���O�̃\�[�g�쐬
}

//============================================================
//	���������L���O����������
//============================================================
void CRetentionManager::InitSurvivalRank(void)
{
	// ���������L���O��������
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		m_aSurvivalRank[nCntPlayer] = NONE_IDX;
	}
}

//============================================================
//	���������L���O�ݒ菈��
//============================================================
void CRetentionManager::SetSurvivalRank(const int nPlayerID)
{
	// ���������L���O���X�V
	for (int nCntPlayer = m_nNumPlayer - 1; nCntPlayer >= 0; nCntPlayer--)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		if (m_aSurvivalRank[nCntPlayer] == NONE_IDX)
		{ // �����L���O���ݒ肳��Ă��Ȃ��ꍇ

			// �����̃v���C���[�C���f�b�N�X��ݒ�
			m_aSurvivalRank[nCntPlayer] = nPlayerID;

			// �����l�������Z
			m_nNumSurvival--;

			if (m_nNumSurvival <= 0)
			{ // ������������Ă��Ȃ��ꍇ

				// ���������L���O1�ʃv���C���[�̏����|�C���g�����Z
				m_aPlayerWin[m_aSurvivalRank[SURVIVAL_1ST]]++;
			}

			// �����𔲂���
			break;
		}
	}
}

//============================================================
//	���������L���O�擾����
//============================================================
CRetentionManager::ESurvival CRetentionManager::GetSurvivalRank(const int nID) const
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		if (m_aSurvivalRank[nCntPlayer] == nID)
		{ // �����L���O���ݒ肳��Ă��Ȃ��ꍇ

			// �����̃v���C���[�C���f�b�N�X��ݒ�
			return (ESurvival)nCntPlayer;
		}
	}

	// 4�ʂ�Ԃ� (��O)
	assert(false);
	return SURVIVAL_4TH;
}

//============================================================
//	�v���C���[�|�C���g���擾����
//============================================================
int CRetentionManager::GetPlayerWin(const int nID) const
{
	if (nID > NONE_IDX && nID < MAX_PLAYER)
	{ // �C���f�b�N�X���͈͓��̏ꍇ

		// �����v���C���[�̃|�C���g��Ԃ�
		return m_aPlayerWin[nID];
	}
	else { assert(false); }	// �͈͊O
}
