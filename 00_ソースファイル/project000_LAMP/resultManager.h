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
#define	RESULT_PRIO	(14)		// ���U���g�̗D�揇��
#define	NUM_WIN		(2)			//�������S�̐�
#define	NUM_FRAME	(4)			//�t���[���̐�

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
	CObject2D *m_pCover;
	EState m_state;						// ���
	D3DXVECTOR3 m_arPos[OBJ_MAX];		// �T�C�Y
	D3DXVECTOR3 m_arSize[OBJ_MAX];		// �ʒu
	int m_nCounterState;				// ��ԊǗ��J�E���^�[
	int m_nSelect;						// ���݂̑I��
	int m_nOldSelect;					// �O��̑I��
	int m_anNum[OBJ_MAX];				//�@��
	int m_anWaitTime[OBJ_MAX];			//�@�ҋ@����
	bool m_bSkiped;
	bool m_abSizeFinish[OBJ_MAX];
	bool m_abool[OBJ_MAX];				// ����p
};

#endif	// _RESULTMANAGER_H_
