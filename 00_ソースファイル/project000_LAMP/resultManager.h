//============================================================
//
//	���U���g�}�l�[�W���[�w�b�_�[ [resultManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�}�N����`
//************************************************************
#define	RESULT_PRIO	(14)	// ���U���g�̗D�揇��
#define	NUM_WIN		(2)		// �������S�̐�
#define	NUM_FRAME	(4)		// �t���[���̐�

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;		// �I�u�W�F�N�g2D�N���X
class CAnim2D;		// �I�u�W�F�N�g2D�N���X
class CTimerManager;	// �^�C�}�[�}�l�[�W���[�N���X
class CPlayerEntry;

//************************************************************
//	�N���X��`
//************************************************************
// ���U���g�}�l�[�W���[�N���X
class CResultManager
{
public:
	// �e�N�X�`����
	enum ETexture
	{

		TEXTURE_WHO = 0,
		TEXTURE_WIN,
		TEXTURE_WINNERFRAME,
		TEXTURE_FRAME,
		TEXTURE_RESTART,
		TEXTURE_BACK,
		TEXTURE_ICON_PLAYER1,
		TEXTURE_ICON_PLAYER2,
		TEXTURE_ICON_PLAYER3,
		TEXTURE_ICON_PLAYER4,
		TEXTURE_BLACKOUT,
		TEXTURE_PLAYER_ENTRY,
		TEXTURE_WIN2,
		TEXTURE_RANK,
		TEXTURE_REMACH_BG,
		TEXTURE_MAX				// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,			// �������Ȃ����
		STATE_FADEIN,			// �t�F�[�h�C�����
		STATE_WIN,
		STATE_BIG_FRAME,
		STATE_FRAME,
		STATE_HOLD,
		STATE_SELECT,
		STATE_WAIT,				// �J�ڑҋ@���
		STATE_MAX				// ���̗񋓌^�̑���
	};

	//�����̏����Ŏg���Ă���I�u�W�F�N�g�̗񋓌^
	enum EObj
	{
		OBJ_WIN = 0,	//�������S
		OBJ_WIN_NUM,
		OBJ_BIGFRAME,
		OBJ_FRAME,		//�t���[��
		OBJ_ICON,
		OBJ_PLAYER,
		OBJ_NUMBER,
		OBJ_COVER,
		OBJ_SELECT,		//�Z���N�g
		OBJ_MAX
	};

	// �I���
	enum ESelect
	{
		SELECT_YES = 0,	// YES
		SELECT_NO,		// NO
		SELECT_MAX		// ���̗񋓌^�̑���
	};
	//<******************************
	//�����N�̗񋓌^
	//<******************************
	enum RANK
	{
		RANK_FIRST = 0,	//���
		RANK_SECOND,	//���
		RANK_THIRD,		//�O��
		RANK_FOURTH,	//�l��
		RANK_MAX
	};

	// �R���X�g���N�^
	CResultManager();

	// �f�X�g���N�^
	~CResultManager();

	// �����o�֐�
	HRESULT Init(void);		// ������
	HRESULT Uninit(void);	// �I��
	void Update(void);		// �X�V

	// �ÓI�����o�֐�
	static CResultManager *Create(void);	// ����
	static HRESULT Release(CResultManager *&prResultManager);	// �j��

private:

	// �����o�֐�
	void UpdateFade(void);			// �t�F�[�h�C��
	void UpdateSelect(void);		// �I���X�V
	void UpdateTransition(void);	// �J�ڌ���
	void SkipStaging(void);			// ���o�X�L�b�v
	void UpdateWin(void);			// �������S
	void UpdateBigFrame(void);		// ����t���[��
	void UpdateFrame(void);			// �t���[��
	void UpdateNumber(void);		// �ԍ�
	void UpdateCover(void);			// �J�o�[

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CAnim2D *m_apWinLog[NUM_WIN];		// �������S
	CObject2D *m_pBigFrame;				// ����t���[��
	CObject2D *m_apFrame[NUM_FRAME];	// �t���[��
	CAnim2D *m_apNumber[NUM_FRAME];		// �t���[��
	CObject2D *m_apSelect[SELECT_MAX];	// �I����
	CObject2D *m_pFade;					// �t�F�[�h�̏��
	CObject2D *m_pCover;				//�J�o�[
	CObject2D *m_apIcon[NUM_FRAME];		//�v���C���[�̃A�C�R��
	CAnim2D *m_apWinNum[NUM_FRAME];
	CObject2D *m_apPlayerEntry[NUM_FRAME];
	EState m_state;						// ���
	D3DXVECTOR3 m_arPos[OBJ_MAX];		// �T�C�Y
	D3DXVECTOR3 m_rPos[5];
	D3DXVECTOR3 m_arOriginPos[OBJ_MAX][NUM_FRAME];
	D3DXVECTOR3 m_arSize[OBJ_MAX];		// �ʒu
	int m_nCounterState;				// ��ԊǗ��J�E���^�[
	int m_nSelect;						// ���݂̑I��
	int m_nOldSelect;					// �O��̑I��
	int m_anNum[OBJ_MAX];				//�@��
	int m_nNumPlay;						//���l�V��ł��邩
	int m_nPattern;						//�ԍ��p�^�[��
	int m_anWinPoint[RANK::RANK_MAX];	//������
	int m_anWaitTime[OBJ_MAX];			//�ҋ@����
	int m_anRank[RANK::RANK_MAX];		//���ʏ��ɕ��ׂ邽�߂̕ϐ�
	int m_anSaveRank[RANK::RANK_MAX];	//���ʏ��̒l��ۑ�
	bool m_bSkiped;						//�X�L�b�v����Ă��邩�ǂ���
	bool m_abSizeFinish[OBJ_MAX];		//�T�C�Y�g��E�k�����I�����Ă��邩
	bool m_abool[OBJ_MAX];				// ����p
};

#endif	// _RESULTMANAGER_H_
