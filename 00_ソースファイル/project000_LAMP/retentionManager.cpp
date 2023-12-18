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
//	�}�N����`
//************************************************************
#define INIT_WINPOINT	(3)		// ���������|�C���g

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
	m_stateKill		= KILL_LIFE;		// ��������
	m_stateWin		= WIN_SURVIVE;		// ��������
	m_nNumPlayer	= 0;				// �v���C���[��
	m_nNumSurvival	= 0;				// �����v���C���[��
	m_nWinPoint		= INIT_WINPOINT;	// �����|�C���g��

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
void CRetentionManager::AllSetEnableEntry(const bool bEntry, const bool bAI)
{
	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		// �G���g���[�󋵂�ݒ�
		m_aEntry[nCntEntry] = bEntry;

		// AI�󋵂�ݒ�
		m_aAI[nCntEntry] = bAI;
	}
}

//============================================================
//	�G���g���[�̐ݒ菈��
//============================================================
void CRetentionManager::SetEntry(const int nID, const bool bEntry, const bool bAI)
{
	if (bEntry)
	{ // �G���g���[����ꍇ

		if (!m_aEntry[nID])
		{ // �G���g���[��񂪖����ꍇ

			// �v���C���[�������Z
			m_nNumPlayer++;
		}

		// �����C���f�b�N�X�̃G���g���[�󋵂�ݒ�
		m_aEntry[nID] = bEntry;

		// �����C���f�b�N�X��AI�󋵂�ݒ�
		m_aAI[nID] = bAI;
	}
	else
	{ // �G���g���[���������ꍇ

		if (m_aEntry[nID])
		{ // �G���g���[��񂪂���ꍇ

			// �v���C���[�������Z
			m_nNumPlayer--;
		}

		// �����C���f�b�N�X�̃G���g���[�󋵂�ݒ�
		m_aEntry[nID] = bEntry;

		// �����C���f�b�N�X��AI�󋵂�ݒ�
		m_aAI[nID] = bAI;
	}
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
//	AI�擾����
//============================================================
bool CRetentionManager::IsAI(const int nID) const
{
	// �����C���f�b�N�X��AI�󋵂�Ԃ�
	return m_aAI[nID];
}

//============================================================
//	���������L���O����������
//============================================================
void CRetentionManager::InitWinRank(void)
{
	// ���������L���O��������
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		m_aWinRank[nCntPlayer] = RANK_4TH;
	}
}

//============================================================
//	���������L���O�ݒ菈��
//============================================================
void CRetentionManager::SetWinRank(void)
{
	// �ϐ��z���錾
	SSortData aSortData[MAX_PLAYER];	// �\�[�g������p
	for (int i = 0; i < MAX_PLAYER; i++)
	{ // �v���C���[�ő吔���J��Ԃ�

		// �\�[�g����������
		aSortData[i].nWinPoint = m_aPlayerWin[i];	// �����|�C���g�ێ���
		aSortData[i].nPlayerID = i;					// �v���C���[�ԍ�
	}

	// �\�[�g����p���ď��������L���O���X�V
	{
		// �ϐ���錾
		SSortData keepData;	// �\�[�g�p
		int	nCurrentMaxID;	// �ő�l�̃C���f�b�N�X

		for (int nCntKeep = 0; nCntKeep < (MAX_PLAYER - 1); nCntKeep++)
		{ // ����ւ���l�̑��� -1�񕪌J��Ԃ�

			// ���݂̌J��Ԃ������� (�v�f1�Ƃ���)
			nCurrentMaxID = nCntKeep;

			for (int nCntSort = nCntKeep + 1; nCntSort < MAX_PLAYER; nCntSort++)
			{ // ����ւ���l�̑��� -nCntKeep���J��Ԃ�

				if (aSortData[nCurrentMaxID].nWinPoint < aSortData[nCntSort].nWinPoint)
				{ // �ő�l�ɐݒ肳��Ă���l���A���݂̒l�̂ق����傫���ꍇ

					// ���݂̗v�f�ԍ����ő�l�ɐݒ�
					nCurrentMaxID = nCntSort;
				}
			}

			if (nCntKeep != nCurrentMaxID)
			{ // �ő�l�̗v�f�ԍ��ɕϓ����������ꍇ

				// �v�f�̓���ւ�
				keepData = aSortData[nCntKeep];
				aSortData[nCntKeep] = aSortData[nCurrentMaxID];
				aSortData[nCurrentMaxID] = keepData;
			}
		}
	}

	// �X�V�������������L���O�����Ƀv���C���[���Ƃ̏��ʂ�ݒ�
	{
		// �ϐ���錾
		int nOldPoint = NONE_IDX;		// ���̃v���C���[�̊l���|�C���g��
		int nCurrentRank = NONE_IDX;	// ���݂̍ō������L���O

		for (int nCntRank = 0; nCntRank < m_nNumPlayer; nCntRank++)
		{ // �v���C���[�̐l�����J��Ԃ�

			if (nOldPoint == aSortData[nCntRank].nWinPoint)
			{ // �����l���|�C���g���̏ꍇ

				// �O��̏��ʂ�ێ����Ă���v���C���[�ԍ��̔z��v�f�ɐݒ�
				m_aWinRank[aSortData[nCntRank].nPlayerID] = nCurrentRank;
			}
			else
			{ // �����l���|�C���g���ł͂Ȃ��ꍇ

				// ���݂̍ō������L���O��������
				nCurrentRank++;

				// ����̏��ʂ�ێ����Ă���v���C���[�ԍ��̔z��v�f�ɐݒ�
				m_aWinRank[aSortData[nCntRank].nPlayerID] = nCurrentRank;
			}

			// ���݂̃v���C���[�̊l���|�C���g����ۑ�
			nOldPoint = aSortData[nCntRank].nWinPoint;
		}
	}
}

//============================================================
//	���������L���O�擾����
//============================================================
CRetentionManager::ERank CRetentionManager::GetWinRank(const int nID)
{
	return (ERank)m_aWinRank[nID];
}

//============================================================
//	���������L���O��ʃv���C���[�̃C���f�b�N�X�擾����
//============================================================
int CRetentionManager::GetWinRank1st(void) const
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		if (m_aWinRank[nCntPlayer] == RANK_1ST)
		{ // ���݂̃C���f�b�N�X�������L���O��ʂ̏ꍇ

			// ��ʃv���C���[�̃C���f�b�N�X��Ԃ�
			return nCntPlayer;
		}
	}

	// �����L���O1�ʂ����݂��Ȃ�
	assert(false);
	return NONE_IDX;	// ��O��Ԃ�
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

			switch (m_stateWin)
			{ // �����������Ƃ̏���
			case WIN_SURVIVE:

				if (m_nNumSurvival <= 0)
				{ // ������������Ă��Ȃ��ꍇ

					// ���������L���O1�ʃv���C���[�̏����|�C���g�����Z
					m_aPlayerWin[m_aSurvivalRank[RANK_1ST]]++;
				}

				break;

			case WIN_KILL:

				for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
				{ // �v���C���[�̍ő吔���J��Ԃ�

					CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);	// �v���C���[���
					if (pPlayer == NULL)
					{ // ���݂��Ȃ��ꍇ

						break;
					}

					if (pPlayer->GetPadID() == nPlayerID)
					{ // ���񎀖S�����v���C���[�̏ꍇ

						CPlayer *pPlayerAttack = pPlayer->GetLastAttackPlayer();	// �ŏI�U���v���C���[���
						if (pPlayerAttack == NULL)
						{ // �U������Ă��Ȃ��ꍇ

							break;
						}

						if (pPlayerAttack->GetState() != CPlayer::STATE_DEATH)
						{ // �Ō�ɍU�����Ă����v���C���[�������Ă���ꍇ

							// �ŏI�U����^�����v���C���[�̏����|�C���g�����Z
							m_aPlayerWin[pPlayerAttack->GetPadID()]++;
						}
					}
				}

				break;

			default:
				assert(false);
				break;
			}

			if (m_nNumSurvival <= 0)
			{ // ������������Ă��Ȃ��ꍇ

				// ���������L���O�ݒ� (�X�V)
				SetWinRank();
			}

			// �����𔲂���
			break;
		}
	}
}

//============================================================
//	���������L���O�擾����
//============================================================
CRetentionManager::ERank CRetentionManager::GetSurvivalRank(const int nID) const
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		if (m_aSurvivalRank[nCntPlayer] == nID)
		{ // �����L���O���ݒ肳��Ă��Ȃ��ꍇ

			// �����̃v���C���[�C���f�b�N�X��ݒ�
			return (ERank)nCntPlayer;
		}
	}

	// 4�ʂ�Ԃ� (��O)
	assert(false);
	return RANK_4TH;
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
	else { assert(false); return NONE_IDX; }	// �͈͊O
}
