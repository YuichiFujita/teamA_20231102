//============================================================
//
//	���U���g�}�l�[�W���[���� [resultManager.cpp]
//	Author�Fkazuki watanabe
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "resultManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"
#include "texture.h"
#include "model.h"
#include "object2D.h"
#include "timerManager.h"
#include "retentionManager.h"
#include "anim2D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define CHOICE_COL	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// �I�𒆃J���[
#define DEFAULT_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// ��I�𒆃J���[

//***********************************************************
//�l�[���X�y�[�X�̒�`
//***********************************************************
//�t�F�[�h�֘A
namespace Fade
{
	const D3DXCOLOR SETCOL_FADE		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));					// ���l�̒�~�l
	const D3DXVECTOR3 SIZE_FADE		(SCREEN_SIZE);											// �t�F�[�h�̑傫��
	const D3DXCOLOR INITCOL_FADE	(XCOL_AWHITE);											// ���l�̏����l
	const float ADD_ALPHA			(0.008f);												// ���l�̉��Z��

}
//�������S�֘A
namespace Win
{
	//�ԍ��̗񋓌^
	enum Number
	{
		NUMBER_ZERO = 0,	//�O�Ԗ�
		NUMBER_ONE,			//�P�Ԗ�
		NUMBER_MAX
	};

	//�萔�̒�`
	const D3DXVECTOR3	POS							= D3DXVECTOR3(625.0f, 350.0f, 0.0f);	// �ʒu
	const D3DXVECTOR3	DESTPOS						= D3DXVECTOR3(265.0f, 60.0f, 0.0f);		// �ʒu
	const float			DISTANCE[NUMBER_MAX]		= { 170.0f,150.0f};						// �Ԋu
	const D3DXVECTOR3	INIT_SIZE					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y�̏�����

	const D3DXVECTOR3	DESTSIZE_WIN[NUMBER_MAX]	= { D3DXVECTOR3(540.0f, 140.0f, 0.0f),
														D3DXVECTOR3(500.0f, 100.0f, 0.0f) };	// �ړI�̈ʒu

	const D3DXVECTOR3	DESTSIZE_NUMBER[NUMBER_MAX] = { D3DXVECTOR3(210.0f, 100.0f, 0.0f),
														D3DXVECTOR3(180.0f, 80.0f, 0.0f) };		// �ړI�̈ʒu

	const int			MAX_WAIT					= 300;									// �ҋ@���Ԃ̍ő�l
	const float			VALUE_INERTIA[NUMBER_MAX]	= {0.015f,0.05f};						// �����̒l
}
//����t���[���֘A
namespace BigFrame
{
	//�萔�̒�`
	const D3DXVECTOR3	POS				= D3DXVECTOR3(270.0f, 400.0f, 0.0f);				// �ʒu
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �T�C�Y�̏�����
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(530.0f, 610.0f, 0.0f);				// �ړI�̈ʒu
	const int			MAX_WAIT		= 10;												// �ҋ@���Ԃ̍ő�l
	const float			VALUE_INERTIA	= 0.015f;											// �����̒l

}
namespace Number
{
	const D3DXVECTOR3	POS				= D3DXVECTOR3(640.0f, 110.0f, 0.0f);					// �ʒu
	const float			DISTANCE		= 170.0f;											// �Ԋu
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �T�C�Y�̏�����
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(128.0f, 140.0f, 0.0f);				// �ړI�̈ʒu
	const int			NUM = 3;															// �\�������鐔
	const int			MAX_WAIT = 5;														// �ҋ@���Ԃ̍ő�l
	const float			VALUE_INERTIA = 0.09f;												// �����̒l

}
//�t���[���֘A
namespace Frame
{
	//�萔�̒�`
	const D3DXVECTOR3	POS				= D3DXVECTOR3(920.0f, 110.0f, 0.0f);					// �ʒu
	const float			DISTANCE		= 170.0f;											// �Ԋu
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f,0.0f, 0.0f);						// �T�C�Y�̏�����
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(700.0f, 160.0f, 0.0f);				// �ړI�̈ʒu
	const int			NUM				= 3;												// �\�������鐔
	const int			MAX_WAIT		= 5;												// �ҋ@���Ԃ̍ő�l
	const float			VALUE_INERTIA	= 0.025f;											// �����̒l
}
//�J�o�[�֘A
namespace Cover
{
	//�萔�̒�`
	const D3DXVECTOR3	POS				= D3DXVECTOR3(600.0f, 370.0f, 0.0f);				// �ʒu
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �T�C�Y�̏�����
	const D3DXCOLOR		INIT_COL		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);				// �F�̏�����
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(700.0f, 450.0f, 0.0f);				// �ړI�̈ʒu
	const float			VALUE_INERTIA	= 0.06f;											// �����̒l

}
//�I�����֘A
namespace Select
{
	const D3DXVECTOR3	POS			= D3DXVECTOR3(500.0f, 400.0f, 0.0f);					// �ʒu
	const float			DISTANCE	= 200.0f;												// �Ԋu
	const int			PRIORITY	= 15;													// �D�揇��
	const float			INIT_SIZE	= 120.0f;												// �T�C�Y�̏�����
}
//
namespace Icon
{

	//�萔�̒�`
	const D3DXVECTOR3	POS = D3DXVECTOR3(730.0f, 110.0f, 0.0f);					// �ʒu
	const float			DISTANCE = 170.0f;											// �Ԋu
	const D3DXVECTOR3	INIT_SIZE = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// �T�C�Y�̏�����
	const D3DXVECTOR3	DESTSIZE = D3DXVECTOR3(125.0f, 125.0f, 0.0f);				// �ړI�̈ʒu
	const int			NUM = 3;												// �\�������鐔
	const int			MAX_WAIT = 5;												// �ҋ@���Ԃ̍ő�l
	const float			VALUE_INERTIA = 0.025f;

}
namespace Winner
{
	//�萔�̒�`
	const D3DXVECTOR3	POS = D3DXVECTOR3(900.0f, 110.0f, 0.0f);					// �ʒu
	const float			DISTANCE = 170.0f;											// �Ԋu
	const D3DXVECTOR3	DESTSIZE = D3DXVECTOR3(125.0f, 125.0f, 0.0f);				// �ړI�̈ʒu
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CResultManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\Winner_.png",				// ������
	"data\\TEXTURE\\PlayerCount.png",			// �N��
	"data\\TEXTURE\\Winner_flame.png",			// ���҃t���[��
	"data\\TEXTURE\\ranking_flame.png",			// �t���[��
	"data\\TEXTURE\\YES.png",					// �Đ�
	"data\\TEXTURE\\NO.png",					// �߂�
	"data\\TEXTURE\\ICON_PLAYER000.png",
	"data\\TEXTURE\\ICON_PLAYER001.png",
	"data\\TEXTURE\\ICON_PLAYER002.png",
	"data\\TEXTURE\\ICON_PLAYER003.png",
	"data\\TEXTURE\\entry_player.png",

};

//************************************************************
//	�e�N���X [CResultManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CResultManager::CResultManager()
{
	// �����o�ϐ����N���A
	//�I�u�W�F�N�g�̐������������s��
	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{
		m_arPos[nCnt] = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �ʒu
		m_arSize[nCnt] = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �T�C�Y
		m_anNum[nCnt] = 0;									// ���݂̔ԍ�
		m_anWaitTime[nCnt] = 0;								// �ҋ@����
		m_abool[nCnt] = false;								// ����p
		m_abSizeFinish[nCnt] = false;						// �T�C�Y�ύX����p

		for (int nCntFrame = 0; nCntFrame < NUM_FRAME; nCntFrame++)
		{
			m_arOriginPos[nCnt][nCntFrame] = VEC3_ZERO;
		}
	}

	for (int nCnt = 0; nCnt < NUM_FRAME; nCnt++)
	{
		m_apFrame[nCnt] = nullptr;
		m_apIcon[nCnt] = nullptr;
		m_apWinNum[nCnt] = nullptr;
		m_anRank[nCnt] = NULL;
	}


	m_pBigFrame		= nullptr;		// ����t���[��
	m_pFade			= NULL;			// �t�F�[�h�̏��
	m_state			= STATE_NONE;	// ���
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nSelect		= SELECT_YES;	// ���݂̑I��
	m_nOldSelect	= SELECT_YES;	// �O��̑I��
	m_bSkiped = false;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CResultManager::~CResultManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CResultManager::Init(void)
{
	//�I�u�W�F�N�g�̐������������s��
	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{
		m_arPos[nCnt]	= (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �ʒu
		m_arSize[nCnt]	= (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �T�C�Y
		m_anNum[nCnt] = 0;									// ���݂̔ԍ�
		m_anWaitTime[nCnt] = 0;								// �ҋ@����
		m_abool[nCnt] = false;								// ����p
		m_abSizeFinish[nCnt] = false;						// �T�C�Y�ύX����p

		for (int nCntFrame = 0; nCntFrame < NUM_FRAME; nCntFrame++)
		{
			m_arOriginPos[nCnt][nCntFrame] = VEC3_ZERO;
		}
	}

	for (int nCnt = 0; nCnt < NUM_FRAME; nCnt++)
	{
		m_apFrame[nCnt] = nullptr;
		m_apIcon[nCnt] = nullptr;
		m_apWinNum[nCnt] = nullptr;
		m_anRank[nCnt] = NULL;
		m_anSaveRank[nCnt] = NULL;
	}

	// �����o�ϐ���������
	memset(&m_apWinLog[0], 0, sizeof(&m_apWinLog));
	memset(&m_apSelect[0], 0, sizeof(&m_apSelect));

	m_pBigFrame		= nullptr;		//����t���[��		
	m_pFade			= NULL;			// �t�F�[�h�̏��
	m_pCover		= NULL;			// �t�F�[�h�̏��
	m_state			= STATE_FADEIN;	// ���
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nSelect		= SELECT_YES;	// ���݂̑I��
	m_nOldSelect	= SELECT_YES;	// �O��̑I��
	m_bSkiped = false;

	//--------------------------------------------------------
	//	�t�F�[�h�̐����E�ݒ�
	//--------------------------------------------------------
	// �t�F�[�h�̐���
	m_pFade = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		Fade::SIZE_FADE,		// �傫��
		VEC3_ZERO,		// ����
		Fade::INITCOL_FADE	// �F
	);
	if (m_pFade == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}
	// �D�揇�ʂ�ݒ�
	m_pFade->SetPriority(RESULT_PRIO);

	//--------------------------------------------------------
	//	����t���[�������E�ݒ�
	//--------------------------------------------------------
	m_pBigFrame = CObject2D::Create
	(
		D3DXVECTOR3(BigFrame::POS.x, BigFrame::POS.y, BigFrame::POS.z),
		Win::INIT_SIZE
	);

	// �e�N�X�`����ݒ�
	m_pBigFrame->BindTexture(mc_apTextureFile[TEXTURE_WINNERFRAME]);

	// �D�揇�ʂ�ݒ�
	m_pBigFrame->SetPriority(RESULT_PRIO);

	//--------------------------------------------------------
	//	�������S�����E�ݒ�
	//--------------------------------------------------------
	for (int nCnt = 0; nCnt < NUM_WIN; nCnt++)
	{
		//���݂̃J�E���g�ŏ����𕪂���
		switch (nCnt)
		{
			//
			case 0:

				m_apWinLog[nCnt] = CAnim2D::Create
				(
					1,
					1,
					D3DXVECTOR3(Win::POS.x + Win::DISTANCE[Win::NUMBER_ZERO] * nCnt, Win::POS.y, Win::POS.z),
					Win::INIT_SIZE
				);


			break;

			//
			case 1:

				m_apWinLog[nCnt] = CAnim2D::Create
				(
					1,
					MAX_PLAYER,
					D3DXVECTOR3(Win::POS.x + Win::DISTANCE[Win::NUMBER_ZERO] * nCnt, Win::POS.y, Win::POS.z),
					Win::INIT_SIZE
				);

				//�����ŏ����҂̔ԍ����o��(�P�ʂ̃v���C���[�̔ԍ����擾���Ă���)
				m_apWinLog[nCnt]->SetPattern(CManager::GetInstance()->GetRetentionManager()->GetWinRank1st());

			break;

		}

		if (m_apWinLog[nCnt] == NULL)
		{ // �����Ɏ��s�����ꍇ

		  // ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����ݒ�
		m_apWinLog[nCnt]->BindTexture(mc_apTextureFile[nCnt]);

		// �D�揇�ʂ�ݒ�
		m_apWinLog[nCnt]->SetPriority(RESULT_PRIO);
	}

	//--------------------------------------------------------
	//	�t���[�������E�ݒ�
	//--------------------------------------------------------
	for (int nCnt = 0; nCnt < NUM_FRAME; nCnt++)
	{
		m_apNumber[nCnt] = CAnim2D::Create
		(	10, 
			1, 
			D3DXVECTOR3(Number::POS.x, Number::POS.y + Number::DISTANCE * nCnt, Number::POS.z),
			Number::INIT_SIZE);

		m_apNumber[nCnt]->BindTexture("data\\TEXTURE\\number002.png");
		m_apNumber[nCnt]->SetPattern(nCnt + 1);
		m_apNumber[nCnt]->SetPriority(RESULT_PRIO);

		m_apFrame[nCnt] = CObject2D::Create
		(
			D3DXVECTOR3(Frame::POS.x, Frame::POS.y + Frame::DISTANCE * nCnt, Frame::POS.z),
			Frame::INIT_SIZE
		);

		if (m_apWinLog[nCnt] == NULL)
		{ // �����Ɏ��s�����ꍇ

		  // ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����ݒ�
		m_apFrame[nCnt]->BindTexture(mc_apTextureFile[TEXTURE_FRAME]);

		// �D�揇�ʂ�ݒ�
		m_apFrame[nCnt]->SetPriority(RESULT_PRIO);
	}

	//--------------------------------------------------------
	//	�I���������E�ݒ�
	//--------------------------------------------------------
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		m_apSelect[nCnt] = CObject2D::Create
		(
			D3DXVECTOR3(Select::POS.x + Select::DISTANCE * nCnt, Select::POS.y, Select::POS.z),
			D3DXVECTOR3(Select::INIT_SIZE, Select::INIT_SIZE, 0.0f)
		);

		if (m_apSelect[nCnt] == NULL)
		{ // �����Ɏ��s�����ꍇ

		  // ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apSelect[nCnt]->SetPriority(Select::PRIORITY);

		m_apSelect[nCnt]->SetEnableDraw(false);

		// ���݂̑I��v�f�̐F�𔒂ɐݒ�
		m_apSelect[nCnt]->SetColor(DEFAULT_COL);

	}


	// �e�N�X�`����ݒ�
	m_apSelect[SELECT_YES]->BindTexture(mc_apTextureFile[TEXTURE_RESTART]);
	m_apSelect[SELECT_NO]->BindTexture(mc_apTextureFile[TEXTURE_BACK]);

	for (int nCnt = 0; nCnt < CManager::GetInstance()->GetRetentionManager()->GetNumPlayer(); nCnt++)
	{
		m_apIcon[nCnt] = CObject2D::Create
		(
			D3DXVECTOR3(Icon::POS.x, Icon::POS.y + Icon::DISTANCE * nCnt, Icon::POS.z),
			Icon::DESTSIZE
		);

		m_arOriginPos[EObj::OBJ_ICON][nCnt] = m_apIcon[nCnt]->GetVec3Position();

		m_apWinNum[nCnt] = CAnim2D::Create
		(
			1,
			MAX_PLAYER,
			D3DXVECTOR3(Winner::POS.x, Winner::POS.y + Winner::DISTANCE * nCnt, Winner::POS.z),
			Winner::DESTSIZE
		);

		m_arOriginPos[EObj::OBJ_PLAYER][nCnt] = m_apWinNum[nCnt]->GetVec3Position();

		if (m_apIcon[nCnt] == nullptr
			|| m_apWinNum[nCnt] == nullptr)
		{ // �����Ɏ��s�����ꍇ

		  // ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
		////�|���������[�h��������
		//if (CManager::GetInstance()->GetRetentionManager()->GetWinState() == CRetentionManager::WIN_KILL)
		//{
		//	for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
		//	{
		//		//��񂾂������ʂ�Ȃ�����
		//		if (m_anSaveRank[nCnt] == NULL)
		//		{
		//			m_anSaveRank[nCnt] = CManager::GetInstance()->GetRetentionManager()->GetPlayerWin(nCntRank);
		//		}
		//	}
		//	switch (nCnt)
		//	{
		//	case 0:

		//		//�P�ʂ��珇�Ɏ擾���Ă���
		//		m_anRank[nCnt] = CManager::GetInstance()->GetRetentionManager()->GetWinRank1st();

		//		break;

		//	case 1:

		//		//�P�ʂ��珇�Ɏ擾���Ă���
		//		m_anRank[nCnt] = CManager::GetInstance()->GetRetentionManager()->GetPlayerWin(nCnt);;

		//		break;

		//	case 2:

		//		//�P�ʂ��珇�Ɏ擾���Ă���
		//		m_anRank[nCnt] = CManager::GetInstance()->GetRetentionManager()->GetPlayerWin(nCnt);;

		//		break;

		//	case 3:


		//		//�P�ʂ��珇�Ɏ擾���Ă���
		//		m_anRank[nCnt] = CManager::GetInstance()->GetRetentionManager()->GetPlayerWin(nCnt);;

		//		break;
		//	}
		//}

		////�T�o�C�o���X�^�C����������
		//else if (CManager::GetInstance()->GetRetentionManager()->GetWinState() == CRetentionManager::WIN_SURVIVE)
		//{
			for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
			{
				//��񂾂������ʂ�Ȃ�����
				if (m_anSaveRank[nCnt] == NULL)
				{
					m_anSaveRank[nCnt] = CManager::GetInstance()->GetRetentionManager()->GetSurvivalRank(nCntRank);
				}
				else
				{
					break;
				}
			}

			switch (nCnt)
			{
			case 0:

				//�P�ʂ��珇�Ɏ擾���Ă���
				m_anRank[nCnt] = CManager::GetInstance()->GetRetentionManager()->GetWinRank1st();

				break;

			case 1:

				//�P�ʂ��珇�Ɏ擾���Ă���
				m_anRank[nCnt] = CManager::GetInstance()->GetRetentionManager()->GetSurvivalRank(nCnt);

				break;

			case 2:

				//�P�ʂ��珇�Ɏ擾���Ă���
				m_anRank[nCnt] = CManager::GetInstance()->GetRetentionManager()->GetSurvivalRank(nCnt);

				break;

			case 3:


				//�P�ʂ��珇�Ɏ擾���Ă���
				m_anRank[nCnt] = CManager::GetInstance()->GetRetentionManager()->GetSurvivalRank(nCnt);

				break;
			}

		// �D�揇�ʂ�ݒ�
		m_apIcon[nCnt]->SetPriority(RESULT_PRIO);
		m_apWinNum[nCnt]->SetPriority(RESULT_PRIO);

		m_apIcon[nCnt]->SetEnableDraw(false);
		m_apWinNum[nCnt]->SetEnableDraw(false);

		// ���݂̑I��v�f�̐F�𔒂ɐݒ�
		m_apIcon[nCnt]->SetColor(DEFAULT_COL);
		m_apWinNum[nCnt]->SetColor(DEFAULT_COL);
	
		//�e�N�X�`�����f
		m_apIcon[nCnt]->BindTexture(mc_apTextureFile[TEXTURE_ICON_PLAYER1 + nCnt]);

		//�e�N�X�`�����f
		m_apWinNum[nCnt]->BindTexture(mc_apTextureFile[TEXTURE_WIN]);
	}

	//--------------------------------------------------------
	//	�t�F�[�h�̐����E�ݒ�
	//--------------------------------------------------------
	// �t�F�[�h�̐���
	m_pCover = CObject2D::Create
	( // ����
		Cover::POS,			// �ʒu
		Cover::INIT_SIZE,		// �傫��
		VEC3_ZERO,			// ����
		Cover::INIT_COL	// �F
	);
	if (m_pCover == NULL)
	{ // �����Ɏ��s�����ꍇ

	  // ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}
	m_pCover->BindTexture("data\\TEXTURE\\Remach_flame.png");
	// �D�揇�ʂ�ݒ�
	m_pCover->SetPriority(RESULT_PRIO);

	//������ʂ̒l�ƃZ�[�u���Ă���l�ʂ̒l�������Ȃ�
	if (m_anRank[RANK_FIRST] == m_anSaveRank[RANK_FOURTH])
	{
		//�l�̓���ւ����s��
		m_anRank[RANK_SECOND] = m_anSaveRank[RANK_FIRST];
		m_anRank[RANK_FOURTH] = m_anSaveRank[RANK_SECOND];
	}
	//������ʂ̒l�ƃZ�[�u���Ă���O�ʂ̒l�������Ȃ�
	else if (m_anRank[RANK_FIRST] == m_anSaveRank[RANK_THIRD])
	{
		//�l�̓���ւ����s��
		m_anRank[RANK_SECOND] = m_anSaveRank[RANK_FIRST];
		m_anRank[RANK_THIRD] = m_anSaveRank[RANK_SECOND];
	}
	//������ʂ̒l�ƃZ�[�u���Ă����ʂ̒l�������Ȃ�
	else if (m_anRank[RANK_FIRST] == m_anSaveRank[RANK_SECOND])
	{
		//�l�̓���ւ����s��
		m_anRank[RANK_SECOND] = m_anSaveRank[RANK_FIRST];
		m_anRank[RANK_THIRD] = m_anSaveRank[RANK_FOURTH];
		m_anRank[RANK_FOURTH] = m_anSaveRank[RANK_THIRD];
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CResultManager::Uninit(void)
{
	//����t���[���̏I������
	if (m_pBigFrame != nullptr)
	{	
		m_pBigFrame->Uninit();
		m_pBigFrame = nullptr;
	}

	//�������S�̏I������
	for (int nCnt = 0; nCnt < NUM_WIN; nCnt++)
	{
		if (m_apWinLog[nCnt] != nullptr)
		{
			m_apWinLog[nCnt]->Uninit();
			m_apWinLog[nCnt] = nullptr;
		}
	}
	//�t���[���̏I������
	for (int nCnt = 0; nCnt < NUM_FRAME; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
		
		if (m_apFrame[nCnt] != nullptr)
		{
			m_apFrame[nCnt]->Uninit();
			m_apFrame[nCnt] = nullptr;
		}
		if (m_apIcon[nCnt] != nullptr)
		{
			m_apIcon[nCnt]->Uninit();
			m_apIcon[nCnt] = nullptr;
		}
		if (m_apWinNum[nCnt] != nullptr)
		{
			m_apWinNum[nCnt]->Uninit();
			m_apWinNum[nCnt] = nullptr;
		}
	}

	//�I�����̏I������
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		if (m_apSelect[nCnt] != nullptr)
		{
			m_apSelect[nCnt]->Uninit();
			m_apSelect[nCnt] = nullptr;
		}
	}

	// �t�F�[�h�̏I��
	m_pFade->Uninit();

	m_pCover->Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CResultManager::Update(void)
{
	for (int nCnt = 0; nCnt < CManager::GetInstance()->GetRetentionManager()->GetNumPlayer(); nCnt++)
	{
		//���g�`�F�b�N
		if (m_apIcon[nCnt] != nullptr)
		{
			m_apIcon[nCnt]->Update();

			switch (nCnt)
			{
			case 0:

				//�v���C���[���ɕ��ׂ�
				m_apIcon[m_anRank[nCnt]]
					->SetVec3Position(m_arOriginPos[EObj::OBJ_ICON][nCnt]);

				break;

			case 1:

				//�v���C���[���ɕ��ׂ�
				m_apIcon[m_anRank[nCnt]]
					->SetVec3Position(m_arOriginPos[EObj::OBJ_ICON][nCnt]);

				break;

			case 2:

				//�v���C���[���ɕ��ׂ�
				m_apIcon[m_anRank[nCnt]]
					->SetVec3Position(m_arOriginPos[EObj::OBJ_ICON][nCnt]);

				break;

			case 3:

				//�v���C���[���ɕ��ׂ�
				m_apIcon[m_anRank[nCnt]]
					->SetVec3Position(m_arOriginPos[EObj::OBJ_ICON][nCnt]);

				break;
			}
		}
		//���g�`�F�b�N
		if (m_apWinNum[nCnt] != nullptr)
		{
			m_apWinNum[nCnt]->Update();

			switch (nCnt)
			{
			case 0:

				//���̏��ʂ̃v���C���[�̃e�N�X�`���p�^�[���ɂ���
				m_apWinNum[nCnt]->SetPattern(m_anRank[nCnt]);

				//�v���C���[���ɕ��ׂ�
				m_apWinNum[m_anRank[nCnt]]
					->SetVec3Position(m_arOriginPos[EObj::OBJ_PLAYER][m_anRank[nCnt]]);

				break;

			case 1:

				//���̏��ʂ̃v���C���[�̃e�N�X�`���p�^�[���ɂ���
				m_apWinNum[nCnt]->SetPattern(m_anRank[nCnt]);

				//�v���C���[���ɕ��ׂ�
				m_apWinNum[m_anRank[nCnt]]
					->SetVec3Position(m_arOriginPos[EObj::OBJ_PLAYER][m_anRank[nCnt]]);

				break;

			case 2:

				//���̏��ʂ̃v���C���[�̃e�N�X�`���p�^�[���ɂ���
				m_apWinNum[nCnt]->SetPattern(m_anRank[nCnt]);

				//�v���C���[���ɕ��ׂ�
				m_apWinNum[m_anRank[nCnt]]
					->SetVec3Position(m_arOriginPos[EObj::OBJ_PLAYER][m_anRank[nCnt]]);

				break;

			case 3:

				//���̏��ʂ̃v���C���[�̃e�N�X�`���p�^�[���ɂ���
				m_apWinNum[nCnt]->SetPattern(m_anRank[nCnt]);

				//�v���C���[���ɕ��ׂ�
				m_apWinNum[m_anRank[nCnt]]
					->SetVec3Position(m_arOriginPos[EObj::OBJ_PLAYER][m_anRank[nCnt]]);

				break;
			}
		}
	}
	// �J�ڌ���̍X�V
	UpdateTransition();

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����

		// ����

		break;

	case STATE_FADEIN:	// �t�F�[�h�C�����

		// �t�F�[�h�C���̍X�V
		UpdateFade();

		break;

		//�������S
	case STATE_WIN:

		UpdateWin();

		break;

		//����t���[��
	case STATE_BIG_FRAME:

		UpdateBigFrame();
		
		break;

		//�t���[��
	case STATE_FRAME:

		UpdateFrame();

		UpdateNumber();

		//�����ǂ���̔����true�ɂȂ��Ă�����
		if (m_abool[EObj::OBJ_FRAME] == true
			&& m_abool[EObj::OBJ_NUMBER] == true)
		{
			//�X�L�b�v����Ă����Ԃɂ���
			m_bSkiped = true;

			//�L�[���͑ҋ@��Ԃɂ���
			m_state = STATE_HOLD;
		}

		break;

		//�L�[���͑ҋ@���
	case STATE_HOLD:

		//�����̃L�[��������Ă�����
		if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_RETURN)
			|| CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_SPACE)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_A)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_B)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_X)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_Y)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_START))
		{
			//�J�ڑҋ@��ԂɈڍs����
			m_state = STATE_SELECT;
		}

		break;

		//�I��
	case STATE_SELECT:

		UpdateCover();
		
		break;

	case STATE_WAIT:	// �J�ڑҋ@���

		//�I���̍X�V
		UpdateSelect();

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �t�F�[�h�̍X�V
	m_pFade->Update();

	m_pCover->Update();

}

//============================================================
//	��������
//============================================================
CResultManager *CResultManager::Create(void)
{
	// �|�C���^��錾
	CResultManager *pResultManager = NULL;	// ���U���g�}�l�[�W���[�����p

	if (pResultManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pResultManager = new CResultManager;	// ���U���g�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pResultManager != NULL)
	{ // �g�p����Ă���ꍇ
		
		// ���U���g�}�l�[�W���[�̏�����
		if (FAILED(pResultManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pResultManager;
			pResultManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pResultManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CResultManager::Release(CResultManager *&prResultManager)
{
	if (prResultManager != NULL)
	{ // �g�p���̏ꍇ

		// ���U���g�}�l�[�W���[�̏I��
		if (FAILED(prResultManager->Uninit()))
		{ // �j���Ɏ��s�����ꍇ

			// �������J��
			delete prResultManager;
			prResultManager = NULL;

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �������J��
		delete prResultManager;
		prResultManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}
//============================================================
//	�������S�̍X�V����
//============================================================
void CResultManager::UpdateWin(void)
{
	//�������S�̐�������
	for (int nCnt = 0; nCnt < NUM_WIN; nCnt++)
	{
		//���g�`�F�b�N
		if (m_apWinLog[nCnt] != nullptr)
		{
			//�X�V����
			m_apWinLog[nCnt]->Update();
			
			//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
			m_arSize[EObj::OBJ_WIN].x += (Win::DESTSIZE_WIN[Win::NUMBER_ZERO].x + m_arSize[EObj::OBJ_WIN].x) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

			//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
			m_arSize[EObj::OBJ_WIN_NUM].x += (Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].x + m_arSize[EObj::OBJ_WIN_NUM].x) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

			//���̃T�C�Y�𒴂������ɂȂ�����
			if (m_arSize[EObj::OBJ_WIN].x >= Win::DESTSIZE_WIN[Win::NUMBER_ZERO].x&&
				m_arSize[EObj::OBJ_WIN_NUM].x >= Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].x)
			{
				//�l���Œ肷��
				m_arSize[EObj::OBJ_WIN].x = Win::DESTSIZE_WIN[Win::NUMBER_ZERO].x;
				m_arSize[EObj::OBJ_WIN_NUM].x = Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].x;
			}

			//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
			m_arSize[EObj::OBJ_WIN].y += (Win::DESTSIZE_WIN[Win::NUMBER_ZERO].y + m_arSize[EObj::OBJ_WIN].y) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

			//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
			m_arSize[EObj::OBJ_WIN_NUM].y += (Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].y + m_arSize[EObj::OBJ_WIN].y) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

			//���̃T�C�Y�𒴂������ɂȂ�����
			if (m_arSize[EObj::OBJ_WIN].y >= Win::DESTSIZE_WIN[Win::NUMBER_ZERO].y
				&&m_arSize[EObj::OBJ_WIN_NUM].y >= Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].y)
			{//�l���Œ肷��

				m_arSize[EObj::OBJ_WIN].y = Win::DESTSIZE_WIN[Win::NUMBER_ZERO].y;
				m_arSize[EObj::OBJ_WIN_NUM].y = Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].y;
			}

			//�T�C�Y�̐ݒ���s��
			m_apWinLog[Win::NUMBER_ZERO]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN]);

			//�T�C�Y�̐ݒ���s��
			m_apWinLog[Win::NUMBER_ONE]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN_NUM]);

			//�ǂ�����ړI�̈ʒu�ɓ������Ă�����
			if (m_arSize[EObj::OBJ_WIN].x == Win::DESTSIZE_WIN[Win::NUMBER_ZERO].x &&
				m_arSize[EObj::OBJ_WIN].y == Win::DESTSIZE_WIN[Win::NUMBER_ZERO].y&&
				m_arSize[EObj::OBJ_WIN_NUM].x == Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].x &&
				m_arSize[EObj::OBJ_WIN_NUM].y == Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].y)
			{
				m_anWaitTime[EObj::OBJ_WIN]++;

				//����0.0�̈ʒu�ɂǂ̍��W����������
				if (m_arPos[EObj::OBJ_WIN].x == 0.0f&&
					m_arPos[EObj::OBJ_WIN].y == 0.0f&&
					m_arPos[EObj::OBJ_WIN].z == 0.0f)
				{
					//�u�Ԉړ���h�~���邽��
					m_arPos[EObj::OBJ_WIN].x = Win::POS.x;
					m_arPos[EObj::OBJ_WIN].y = Win::POS.y;
					m_arPos[EObj::OBJ_WIN].z = Win::POS.z;
				}
			}

			//�ҋ@���Ԃ��ő�ҋ@���Ԃ𒴂��Ă�����
			if (m_anWaitTime[EObj::OBJ_WIN] >= Win::MAX_WAIT)
			{
				//���g���Ȃ����Ƃ�
				if (m_arSize[EObj::OBJ_WIN_NUM].x == 0.0f
					&&m_arSize[EObj::OBJ_WIN_NUM].y == 0.0f)
				{
					m_arSize[EObj::OBJ_WIN_NUM] = m_arSize[EObj::OBJ_WIN];
				}

				m_anWaitTime[EObj::OBJ_WIN] = Win::MAX_WAIT;

				//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
				m_arPos[EObj::OBJ_WIN].x -= (Win::DESTPOS.x + m_arPos[EObj::OBJ_WIN].x) * 0.05f;

				//���̃T�C�Y�𒴂������ɂȂ�����
				if (m_arPos[EObj::OBJ_WIN].x <= Win::DESTPOS.x)
				{
					//�l���Œ肷��
					m_arPos[EObj::OBJ_WIN].x = Win::DESTPOS.x;
				}

				//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
				m_arPos[EObj::OBJ_WIN].y -= (Win::DESTPOS.y + m_arPos[EObj::OBJ_WIN].y) * 0.05f;

				//���̃T�C�Y�𒴂������ɂȂ�����
				if (m_arPos[EObj::OBJ_WIN].y <= Win::DESTPOS.y)
				{
					m_arPos[EObj::OBJ_WIN].y = Win::DESTPOS.y;
				}

				//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
				m_arSize[EObj::OBJ_WIN].x -= (Win::DESTSIZE_WIN[Win::NUMBER_ONE].x + m_arSize[EObj::OBJ_WIN].x) * Win::VALUE_INERTIA[Win::NUMBER_ONE];
				m_arSize[EObj::OBJ_WIN_NUM].x -= (Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].x + m_arSize[EObj::OBJ_WIN_NUM].x) * Win::VALUE_INERTIA[Win::NUMBER_ONE];

				//���̃T�C�Y�𒴂������ɂȂ�����
				if (m_arSize[EObj::OBJ_WIN].x <= Win::DESTSIZE_WIN[Win::NUMBER_ONE].x&&
					m_arSize[EObj::OBJ_WIN_NUM].x <= Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].x)
				{
					//�l���Œ肷��
					m_arSize[EObj::OBJ_WIN].x = Win::DESTSIZE_WIN[Win::NUMBER_ONE].x;
					m_arSize[EObj::OBJ_WIN_NUM].x = Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].x;
				}

				//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
				m_arSize[EObj::OBJ_WIN].y -= (Win::DESTSIZE_WIN[Win::NUMBER_ONE].y + m_arSize[EObj::OBJ_WIN].y)* Win::VALUE_INERTIA[Win::NUMBER_ONE];
				m_arSize[EObj::OBJ_WIN_NUM].y -= (Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].y + m_arSize[EObj::OBJ_WIN_NUM].y) * Win::VALUE_INERTIA[Win::NUMBER_ONE];

				//���̃T�C�Y�𒴂������ɂȂ�����
				if (m_arSize[EObj::OBJ_WIN].y <= Win::DESTSIZE_WIN[Win::NUMBER_ONE].y&&
					m_arSize[EObj::OBJ_WIN_NUM].y <= Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].y)
				{
					//�l���Œ肷��
					m_arSize[EObj::OBJ_WIN].y = Win::DESTSIZE_WIN[Win::NUMBER_ONE].y;
					m_arSize[EObj::OBJ_WIN_NUM].y = Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].y;
				}

				//�T�C�Y�̐ݒ���s��
				m_apWinLog[Win::NUMBER_ZERO]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN]);
				m_apWinLog[Win::NUMBER_ONE]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN_NUM]);

				//�ʒu��ݒ肷��
				m_apWinLog[nCnt]->SetVec3Position(D3DXVECTOR3(
					m_arPos[EObj::OBJ_WIN].x + Win::DISTANCE[Win::NUMBER_ONE] * nCnt,
					m_arPos[EObj::OBJ_WIN].y,
					m_arPos[EObj::OBJ_WIN].z));

				//�����T�C�Y�E�ʒu�����̈ʒu�ɂ�������
				if (m_apWinLog[Win::NUMBER_ZERO]->GetVec3Sizing().x == Win::DESTSIZE_WIN[Win::NUMBER_ONE].x&&
					m_apWinLog[Win::NUMBER_ZERO]->GetVec3Sizing().y == Win::DESTSIZE_WIN[Win::NUMBER_ONE].y&&
					m_apWinLog[Win::NUMBER_ONE]->GetVec3Sizing().x == Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].x&&
					m_apWinLog[Win::NUMBER_ONE]->GetVec3Sizing().y == Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].y&&
					m_arPos[EObj::OBJ_WIN].x == Win::DESTPOS.x&&
					m_arPos[EObj::OBJ_WIN].y == Win::DESTPOS.y)
				{
	
					//����t���[���̃X�e�[�g�Ɉڍs
					m_state = STATE_BIG_FRAME;
				}
			}
		}

	}
}
//============================================================
//	����t���[���̍X�V����
//============================================================
void CResultManager::UpdateBigFrame(void)
{
	//���邩�͂킩��Ȃ����O�̂���
	if (m_anWaitTime[EObj::OBJ_WIN] != 0)
	{
		//0�ɏ���������
		m_anWaitTime[EObj::OBJ_WIN] = 0;
	}

	//���g�`�F�b�N
	if (m_pBigFrame != nullptr)
	{
		m_pBigFrame->Update();

		//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
		m_arSize[EObj::OBJ_BIGFRAME].x += (BigFrame::DESTSIZE.x + m_arSize[EObj::OBJ_BIGFRAME].x) * BigFrame::VALUE_INERTIA;

		//���̃T�C�Y�𒴂������ɂȂ�����
		if (m_arSize[EObj::OBJ_BIGFRAME].x >= BigFrame::DESTSIZE.x)
		{
			//�l���Œ肷��
			m_arSize[EObj::OBJ_BIGFRAME].x = BigFrame::DESTSIZE.x;
		}

		//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������

		m_arSize[EObj::OBJ_BIGFRAME].y += (BigFrame::DESTSIZE.y + m_arSize[EObj::OBJ_BIGFRAME].y) * BigFrame::VALUE_INERTIA;

		//���̃T�C�Y�𒴂������ɂȂ�����
		if (m_arSize[EObj::OBJ_BIGFRAME].y >= BigFrame::DESTSIZE.y)
		{//�l���Œ肷��
			m_arSize[EObj::OBJ_BIGFRAME].y = BigFrame::DESTSIZE.y;
		}

		//�T�C�Y�̐ݒ���s��
		m_pBigFrame->SetVec3Sizing(m_arSize[EObj::OBJ_BIGFRAME]);

		//�ړI�̃T�C�Y�ɂȂ��Ă�����
		if (m_arSize[EObj::OBJ_BIGFRAME].x == BigFrame::DESTSIZE.x
			&&m_arSize[EObj::OBJ_BIGFRAME].y == BigFrame::DESTSIZE.y)
		{
			//�ҋ@���Ԃ𑝂₷
			m_anWaitTime[EObj::OBJ_BIGFRAME]++;

			//�ő�ҋ@���ԂɂȂ��Ă�����
			if (m_anWaitTime[EObj::OBJ_BIGFRAME] == BigFrame::MAX_WAIT)
			{
				m_anWaitTime[EObj::OBJ_BIGFRAME] = 0;
				m_state = STATE_FRAME;
			}
		}
	}

}
//============================================================
//	�t���[���̍X�V����
//============================================================
void CResultManager::UpdateFrame(void)
{
	if (m_abSizeFinish[EObj::OBJ_FRAME] == false)
	{
		//���g�`�F�b�N
		if (m_apFrame[m_anNum[EObj::OBJ_FRAME]] != nullptr)
		{
			//�`�������悤�ɂ���
			//m_apFrame[m_anNum[EObj::OBJ_FRAME]]->SetEnableDraw(true);
			m_apFrame[m_anNum[EObj::OBJ_FRAME]]->Update();

			//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
			m_arSize[EObj::OBJ_FRAME].x += (Frame::DESTSIZE.x + m_arSize[EObj::OBJ_FRAME].x) * Frame::VALUE_INERTIA;

			//���̃T�C�Y�𒴂������ɂȂ�����
			if (m_arSize[EObj::OBJ_FRAME].x >= Frame::DESTSIZE.x)
			{
				//�l���Œ肷��
				m_arSize[EObj::OBJ_FRAME].x = Frame::DESTSIZE.x;
			}

			//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
			m_arSize[EObj::OBJ_FRAME].y += (Frame::DESTSIZE.y + m_arSize[EObj::OBJ_FRAME].y) * Frame::VALUE_INERTIA;

			//���̃T�C�Y�𒴂������ɂȂ�����
			if (m_arSize[EObj::OBJ_FRAME].y >= Frame::DESTSIZE.y)
			{
				//�l���Œ肷��
				m_arSize[EObj::OBJ_FRAME].y = Frame::DESTSIZE.y;

			}

			//�T�C�Y��ݒ肷��
			m_apFrame[m_anNum[EObj::OBJ_FRAME]]->SetVec3Sizing(m_arSize[EObj::OBJ_FRAME]);

			//�����t���[���̊g�嗦�����̒l�ɂȂ��Ă�����
			if (m_apFrame[m_anNum[EObj::OBJ_FRAME]]->GetVec3Sizing().x == Frame::DESTSIZE.x
				&&m_apFrame[m_anNum[EObj::OBJ_FRAME]]->GetVec3Sizing().y == Frame::DESTSIZE.y)
			{
				//�͈͊O�ɂ����Ȃ��悤�ɂ���
				if (m_anNum[EObj::OBJ_FRAME] >= Frame::NUM)
				{
					//�t���[��
					m_anNum[EObj::OBJ_FRAME] = Frame::NUM;
					m_abSizeFinish[EObj::OBJ_FRAME] = true;

				}
				//�͈͊O�ɂ����Ă��Ȃ����
				else if (!(m_anNum[EObj::OBJ_FRAME] >= Frame::NUM))
				{
					//�t���[��
					//�ҋ@���Ԃ����Z����
					m_anWaitTime[EObj::OBJ_FRAME]++;

					//�ҋ@���Ԃ̍ő�l�𒴂������ɂȂ�����
					if (m_anWaitTime[EObj::OBJ_FRAME] >= Frame::MAX_WAIT)
					{
						m_abSizeFinish[EObj::OBJ_FRAME] = true;

						//�l�̃��Z�b�g���s��
						m_abSizeFinish[EObj::OBJ_NUMBER] = false;
						m_anWaitTime[EObj::OBJ_FRAME] = 0;
						m_anNum[EObj::OBJ_FRAME]++;
						m_arSize[EObj::OBJ_FRAME].x = Frame::INIT_SIZE.x;
						m_arSize[EObj::OBJ_FRAME].y = Frame::INIT_SIZE.y;

					}
				}
			}
			//�ԍ��EX�T�C�Y�EY�T�C�Y�����̒l�ɂȂ��Ă�����
			if (m_anNum[EObj::OBJ_FRAME] == Frame::NUM &&
				m_apFrame[m_anNum[EObj::OBJ_FRAME]]->GetVec3Sizing().x == Frame::DESTSIZE.x&&
				m_apFrame[m_anNum[EObj::OBJ_FRAME]]->GetVec3Sizing().y == Frame::DESTSIZE.y)
			{
				//�����true�ɂ���
				m_abool[EObj::OBJ_FRAME] = true;
			}
		}
	}
}
//============================================================
//	�ԍ��̍X�V����
//============================================================
void CResultManager::UpdateNumber(void)
{
	if (m_abSizeFinish[EObj::OBJ_FRAME] == true)
	{
		//���g�`�F�b�N
		if (m_apNumber[m_anNum[EObj::OBJ_NUMBER]] != nullptr)
		{
			//�ԍ�
			m_apNumber[m_anNum[EObj::OBJ_NUMBER]]->Update();

			//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
			m_arSize[EObj::OBJ_NUMBER].x += (Number::DESTSIZE.x + m_arSize[EObj::OBJ_NUMBER].x) * Number::VALUE_INERTIA;

			//���̃T�C�Y�𒴂������ɂȂ�����
			if (m_arSize[EObj::OBJ_NUMBER].x >= Number::DESTSIZE.x)
			{
				//�l���Œ肷��
				m_arSize[EObj::OBJ_NUMBER].x = Number::DESTSIZE.x;
			}

			//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
			m_arSize[EObj::OBJ_NUMBER].y += (Number::DESTSIZE.y + m_arSize[EObj::OBJ_NUMBER].y) * Number::VALUE_INERTIA;

			//���̃T�C�Y�𒴂������ɂȂ�����
			if (m_arSize[EObj::OBJ_NUMBER].y >= Number::DESTSIZE.y)
			{
				//�l���Œ肷��
				m_arSize[EObj::OBJ_NUMBER].y = Number::DESTSIZE.y;

			}

			//�T�C�Y��ݒ肷��
			m_apNumber[m_anNum[EObj::OBJ_NUMBER]]->SetVec3Sizing(m_arSize[EObj::OBJ_NUMBER]);

			//�����t���[���̊g�嗦�����̒l�ɂȂ��Ă�����
			if (m_apNumber[m_anNum[EObj::OBJ_NUMBER]]->GetVec3Sizing().x == Number::DESTSIZE.x
				&&m_apNumber[m_anNum[EObj::OBJ_NUMBER]]->GetVec3Sizing().y == Number::DESTSIZE.y)
			{
				//
				if (m_apIcon[m_anNum[EObj::OBJ_NUMBER]] != nullptr&&
					m_apWinNum[m_anNum[EObj::OBJ_NUMBER]] != nullptr)
				{
					m_apIcon[m_anNum[EObj::OBJ_NUMBER]]->SetEnableDraw(true);
					m_apWinNum[m_anNum[EObj::OBJ_NUMBER]]->SetEnableDraw(true);
				}

				//�͈͊O�ɂ����Ȃ��悤�ɂ���
				if (m_anNum[EObj::OBJ_NUMBER] >= Number::NUM)
				{
					//�ԍ�
					m_anNum[EObj::OBJ_NUMBER] = Number::NUM;
					m_abSizeFinish[EObj::OBJ_NUMBER] = true;


				}
				//�͈͊O�ɂ����Ă��Ȃ����
				else if (!(m_anNum[EObj::OBJ_NUMBER] >= Number::NUM))
				{
					//�ҋ@���Ԃ����Z����
					m_anWaitTime[EObj::OBJ_NUMBER]++;

					//�ҋ@���Ԃ̍ő�l�𒴂������ɂȂ�����
					if (m_anWaitTime[EObj::OBJ_NUMBER] >= Number::MAX_WAIT)
					{
						m_abSizeFinish[EObj::OBJ_NUMBER] = true;

						//�l�̃��Z�b�g���s��
						m_abSizeFinish[EObj::OBJ_FRAME] = false;
						m_anWaitTime[EObj::OBJ_NUMBER] = 0;
						m_anNum[EObj::OBJ_NUMBER]++;
						m_arSize[EObj::OBJ_NUMBER].x = Number::INIT_SIZE.x;
						m_arSize[EObj::OBJ_NUMBER].y = Number::INIT_SIZE.y;

					}
				}
			}
			//�ԍ��EX�T�C�Y�EY�T�C�Y�����̒l�ɂȂ��Ă�����
			if (m_anNum[EObj::OBJ_NUMBER] == Number::NUM &&
				m_apNumber[m_anNum[EObj::OBJ_NUMBER]]->GetVec3Sizing().x == Number::DESTSIZE.x&&
				m_apNumber[m_anNum[EObj::OBJ_NUMBER]]->GetVec3Sizing().y == Number::DESTSIZE.y)
			{
				//�����true�ɂ���
				m_abool[EObj::OBJ_NUMBER] = true;
			}
		}
	}
}
//============================================================
//	�t�F�[�h�C���̍X�V����
//============================================================
void CResultManager::UpdateFade(void)
{
	// �ϐ���錾
	D3DXCOLOR colFade = m_pFade->GetColor();	// �t�F�[�h�̐F

	if (colFade.a < Fade::SETCOL_FADE.a)
	{ // �����ʂ��ݒ�l�����̏ꍇ

		// �����x�����Z
		colFade.a += Fade::ADD_ALPHA;
	}
	else
	{ // �����ʂ��ݒ�l�ȏ�̏ꍇ

		// �����x��␳
		colFade.a = Fade::SETCOL_FADE.a;

		// ��Ԃ�ύX
		m_state = STATE_WIN;	// �ҋ@���
	}

	// �����x�𔽉f
	m_pFade->SetColor(colFade);
}

//============================================================
//	�I���̍X�V����
//============================================================
void CResultManager::UpdateSelect(void)
{
#if 1

	m_nOldSelect = m_nSelect;

	if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_A)
	||  CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_LEFT)
	|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_LEFT))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// ���ɑI�������炷
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		// �T�E���h�̍Đ�
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
	}
	if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_D)
	||  CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_RIGHT)
	|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_RIGHT))
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		// �E�ɑI�������炷
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;

		// �T�E���h�̍Đ�
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
	}

	// ���݂̑I��v�f�̐F�𔒂ɐݒ�
	m_apSelect[m_nOldSelect]->SetColor(DEFAULT_COL);

	// ���݂̑I��v�f�̐F�𔒂ɐݒ�
	m_apSelect[m_nSelect]->SetColor(CHOICE_COL);

#endif
}
//============================================================
//	�J�o�[�̍X�V����
//============================================================
void CResultManager::UpdateCover(void)
{
	//���g�`�F�b�N
	if (m_pCover != nullptr)
	{
		m_pCover->Update();

		//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
		m_arSize[EObj::OBJ_COVER].x += (Cover::DESTSIZE.x + m_arSize[EObj::OBJ_COVER].x) * Cover::VALUE_INERTIA;

		//���̃T�C�Y�𒴂������ɂȂ�����
		if (m_arSize[EObj::OBJ_COVER].x >= Cover::DESTSIZE.x)
		{
			//�l���Œ肷��
			m_arSize[EObj::OBJ_COVER].x = Cover::DESTSIZE.x;
		}

		//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������

		m_arSize[EObj::OBJ_COVER].y += (Cover::DESTSIZE.y + m_arSize[EObj::OBJ_COVER].y) * Cover::VALUE_INERTIA;

		//���̃T�C�Y�𒴂������ɂȂ�����
		if (m_arSize[EObj::OBJ_COVER].y >= Cover::DESTSIZE.y)
		{//�l���Œ肷��
			m_arSize[EObj::OBJ_COVER].y = Cover::DESTSIZE.y;
		}

		//�T�C�Y�̐ݒ���s��
		m_pCover->SetVec3Sizing(m_arSize[EObj::OBJ_COVER]);

		//�ǂ�����ړI�̈ʒu�ɓ������Ă�����
		if (m_arSize[EObj::OBJ_COVER].x == Cover::DESTSIZE.x &&
			m_arSize[EObj::OBJ_COVER].y == Cover::DESTSIZE.y)
		{
			//�Z���N�g�̐�������
			for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
			{
				if (m_apSelect[nCnt] != nullptr)
				{
					m_apSelect[nCnt]->SetEnableDraw(true);
					m_apSelect[nCnt]->Update();
				}
			}
			m_bSkiped = false;
			m_state = STATE_WAIT;
		}

	}
}
//============================================================
//	�J�ڌ���̍X�V����
//============================================================
void CResultManager::UpdateTransition(void)
{
	if (m_bSkiped == false)
	{
		if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_RETURN)
			|| CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_SPACE)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_A)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_B)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_X)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_Y)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_START))
		{
			if (m_state != STATE_WAIT)
			{ // �J�ڑҋ@��Ԃł͂Ȃ��ꍇ

			  //�t���[���X�e�[�g�ł͂Ȃ����
				if (m_state != STATE_FRAME)
				{
					//���o�X�L�b�v
					SkipStaging();
				}

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
			}
			else
			{ // �J�ڑҋ@��Ԃ̏ꍇ

				if (CManager::GetInstance()->GetFade()->GetState() == CFade::FADE_NONE)
				{ // �t�F�[�h���ł͂Ȃ��ꍇ

					switch (m_nSelect)
					{ // �I�����Ƃ̏���
					case SELECT_YES:

						// �V�[���̐ݒ�
						CManager::GetInstance()->SetScene(CScene::MODE_ENTRY);	// �Q�[�����

						break;

					case SELECT_NO:

						// �V�[���̐ݒ�
						CManager::GetInstance()->SetScene(CScene::MODE_TITLE);	// �����L���O���

						break;
					}

					// �T�E���h�̍Đ�
					CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// ���艹00
				}
			}
			m_bSkiped = true;
		}
	}
}

//============================================================
//	���o�X�L�b�v����
//============================================================
void CResultManager::SkipStaging(void)
{
	// �t�F�[�h�̓����x��ݒ�
	m_pFade->SetColor(Fade::SETCOL_FADE);

	//�������S�̐�������
	for (int nCnt = 0; nCnt < NUM_WIN; nCnt++)
	{
		//���g�`�F�b�N
		if (m_apWinLog[nCnt] != nullptr)
		{
			//�T�C�Y�E�ʒu�̐ݒ�
			m_arSize[EObj::OBJ_WIN].x = Win::DESTSIZE_WIN[Win::NUMBER_ONE].x;
			m_arSize[EObj::OBJ_WIN].y = Win::DESTSIZE_WIN[Win::NUMBER_ONE].y;

			//�T�C�Y�E�ʒu�̐ݒ�
			m_arSize[EObj::OBJ_WIN_NUM].x = Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].x;
			m_arSize[EObj::OBJ_WIN_NUM].y = Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].y;


			m_arPos[EObj::OBJ_WIN].x = Win::DESTPOS.x;
			m_arPos[EObj::OBJ_WIN].y = Win::DESTPOS.y;

			//�T�C�Y�E�ʒu�̃Z�b�g���s��
			m_apWinLog[Win::NUMBER_ZERO]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN]);

			//�T�C�Y�E�ʒu�̃Z�b�g���s��
			m_apWinLog[Win::NUMBER_ONE]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN_NUM]);

			m_apWinLog[nCnt]->SetVec3Position(D3DXVECTOR3(
				m_arPos[EObj::OBJ_WIN].x + Win::DISTANCE[Win::NUMBER_ONE] * nCnt,
				m_arPos[EObj::OBJ_WIN].y,
				m_arPos[EObj::OBJ_WIN].z));

			m_state = STATE_BIG_FRAME;
		}

	}

	//���g�`�F�b�N
	if (m_pBigFrame != nullptr)
	{
		//�T�C�Y�̐ݒ�
		m_arSize[EObj::OBJ_BIGFRAME].x = BigFrame::DESTSIZE.x;
		m_arSize[EObj::OBJ_BIGFRAME].y = BigFrame::DESTSIZE.y;

		//�T�C�Y�̃Z�b�g���s��
		m_pBigFrame->SetVec3Sizing(m_arSize[EObj::OBJ_BIGFRAME]);

		m_state = STATE_FRAME;

	}

	//�t���[��������
	for (int nCnt = 0; nCnt < NUM_FRAME; nCnt++)
	{
		//�`�������悤�ɂ���
		m_apFrame[nCnt]->SetEnableDraw(true);

		//�T�C�Y�̐ݒ�
		m_arSize[EObj::OBJ_FRAME].x = Frame::DESTSIZE.x;
		m_arSize[EObj::OBJ_FRAME].y = Frame::DESTSIZE.y;

		//�T�C�Y�̃Z�b�g���s��
		m_apFrame[nCnt]->SetVec3Sizing(m_arSize[EObj::OBJ_FRAME]);
		m_anNum[OBJ_FRAME] = Frame::NUM;
	}

	//�t���[��������
	for (int nCnt = 0; nCnt < NUM_FRAME; nCnt++)
	{
		//�T�C�Y�̐ݒ�
		m_arSize[EObj::OBJ_NUMBER].x = Number::DESTSIZE.x;
		m_arSize[EObj::OBJ_NUMBER].y = Number::DESTSIZE.y;

		//�T�C�Y�̃Z�b�g���s��
		m_apNumber[nCnt]->SetVec3Sizing(m_arSize[EObj::OBJ_NUMBER]);
		m_anNum[OBJ_NUMBER] = Number::NUM;

		m_apIcon[nCnt]->SetEnableDraw(true);
		m_apWinNum[nCnt]->SetEnableDraw(true);
	}
	m_state = STATE_FRAME;

}
