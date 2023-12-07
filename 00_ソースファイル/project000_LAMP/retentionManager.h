//============================================================
//
//	�f�[�^�ۑ��}�l�[�W���[�w�b�_�[ [retentionManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RETENTION_MANAGER_H_
#define _RETENTION_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "input.h"

//************************************************************
//	�N���X��`
//************************************************************
// �f�[�^�ۑ��}�l�[�W���[�N���X
class CRetentionManager
{
public:
	// ����������
	enum EKill
	{
		KILL_LIFE = 0,	// �̗͐�
		KILL_KNOCK,		// ������΂���
		KILL_MAX		// ���̗񋓌^�̑���
	};

	// ����������
	enum EWin
	{
		WIN_SURVIVE = 0,	// �����c�菟��
		WIN_MAX				// ���̗񋓌^�̑���
	};

	// ���������L���O��
	enum ESurvival
	{
		SURVIVAL_1ST = 0,	// ���������L���O�F���
		SURVIVAL_2ND,		// ���������L���O�F���
		SURVIVAL_3RD,		// ���������L���O�F�O��
		SURVIVAL_4TH,		// ���������L���O�F�l��
		SURVIVAL_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CRetentionManager();

	// �f�X�g���N�^
	~CRetentionManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��

	void SetKillState(const EKill kill);	// ���������ݒ�
	EKill GetKillState(void) const;			// ���������擾
	void SetWinState(const EWin win);		// ���������ݒ�
	EWin GetWinState(void) const;			// ���������擾
	void SetNumPlayer(const int nNum);		// �v���C�l���ݒ�
	int GetNumPlayer(void) const;			// �v���C�l���擾
	void InitNumSurvival(void);				// �����l��������
	int GetNumSurvival(void) const;			// �����l���擾
	void SetWinPoint(const int nWinPoint);	// �����|�C���g�ݒ�
	int GetWinPoint(void) const;			// �����|�C���g�擾
	void AllSetEnableEntry(const bool bEntry);				// �S�G���g���[�󋵐ݒ�
	void SetEnableEntry(const int nID, const bool bEntry);	// �G���g���[�󋵐ݒ�
	bool IsEntry(const int nID) const;						// �G���g���[�󋵎擾

	void InitSurvivalRank(void);					// ���������L���O������
	void SetSurvivalRank(const int nPlayerID);		// ���������L���O�ݒ�
	ESurvival GetSurvivalRank(const int nID) const;	// ���������L���O�擾

	// �ÓI�����o�֐�
	static CRetentionManager *Create(void);	// ����
	static HRESULT Release(CRetentionManager *&prRetentionManager);	// �j��

private:
	// �����o�ϐ�
	EKill	m_stateKill;	// ��������
	EWin	m_stateWin;		// ��������
	int		m_nNumPlayer;	// �v���C���[��
	int		m_nNumSurvival;	// �����v���C���[��
	int		m_nWinPoint;	// �����|�C���g��

	int		m_aSurvivalRank[MAX_PLAYER];	// �~���̐��������L���O
	bool	m_aEntry[MAX_PLAYER];			// �G���g���[��
};

#endif	// _RETENTION_MANAGER_H_
