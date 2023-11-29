//============================================================
//
//	���U���g�}�l�[�W���[���� [resultManager.cpp]
//	Author�F���c�E��
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
	const D3DXCOLOR SETCOL_FADE		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));	// ���l�̒�~�l
	const D3DXVECTOR3 SIZE_FADE		(SCREEN_SIZE * 0.95f);					// �t�F�[�h�̑傫��
	const D3DXCOLOR INITCOL_FADE	(XCOL_AWHITE);							// ���l�̏����l
	const float ADD_ALPHA			(0.008f);								// ���l�̉��Z��

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
	const D3DXVECTOR3	POS							= D3DXVECTOR3(425.0f, 350.0f, 0.0f);	// �ʒu
	const D3DXVECTOR3	DESTPOS						= D3DXVECTOR3(150.0f, 50.0f, 0.0f);		// �ʒu
	const float			DISTANCE[NUMBER_MAX]		= { 440.0f,100.0f};						// �Ԋu
	const D3DXVECTOR3	INIT_SIZE					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y�̏�����
	const float			DESTSIZE[NUMBER_MAX]		= { 500.0f,100.0f};						// �ړI�̈ʒu
	const int			MAX_WAIT					= 150;									// �ҋ@���Ԃ̍ő�l
	const float			VALUE_INERTIA[NUMBER_MAX]	= {0.015f,0.05f};						// �����̒l
}
//����t���[���֘A
namespace BigFrame
{
	//�萔�̒�`
	const D3DXVECTOR3	POS				= D3DXVECTOR3(230.0f, 400.0f, 0.0f);			// �ʒu
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �T�C�Y�̏�����
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(400.0f, 640.0f, 0.0f);			// �ړI�̈ʒu
	const int			MAX_WAIT		= 100;											// �ҋ@���Ԃ̍ő�l
	const float			VALUE_INERTIA	= 0.015f;										// �����̒l

}
//�t���[���֘A
namespace Frame
{
	//�萔�̒�`
	const D3DXVECTOR3	POS				= D3DXVECTOR3(860.0f, 90.0f, 0.0f);				// �ʒu
	const float			DISTANCE		= 170.0f;										// �Ԋu
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f,0.0f, 0.0f);					// �T�C�Y�̏�����
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(700.0f, 180.0f, 0.0f);			// �ړI�̈ʒu
	const int			NUM				= 3;											// �\�������鐔
	const int			MAX_WAIT		= 25;											// �ҋ@���Ԃ̍ő�l
	const float			VALUE_INERTIA	= 0.025f;										// �����̒l
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CResultManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\Who.png",			// �N��
	"data\\TEXTURE\\Win.png",			// ������
	"data\\TEXTURE\\entry_flame.png",	// �t���[��
	"data\\TEXTURE\\continue001.png",	// �Đ�
	"data\\TEXTURE\\continue002.png",	// �߂�
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
	m_pBigFrame		= nullptr;		// ����t���[��
	m_pFade			= NULL;			// �t�F�[�h�̏��
	m_state			= STATE_NONE;	// ���
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nSelect		= SELECT_YES;	// ���݂̑I��
	m_nOldSelect	= SELECT_YES;	// �O��̑I��
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
		m_arPos[nCnt]	= (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//�ʒu
		m_arSize[nCnt]	= (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//�T�C�Y
		m_anNum[nCnt] = 0;									// ���݂̔ԍ�
		m_anWaitTime[nCnt] = 0;								// �ҋ@����
	}

	// �����o�ϐ���������
	memset(&m_apWinLog[0], 0, sizeof(&m_apWinLog));
	memset(&m_apFrame[0], 0, sizeof(&m_apFrame));
	memset(&m_apSelect[0], 0, sizeof(&m_apSelect));
	m_pBigFrame		= nullptr;		//����t���[��		
	m_pFade			= NULL;			// �t�F�[�h�̏��
	m_state			= STATE_FADEIN;	// ���
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nSelect		= SELECT_YES;	// ���݂̑I��
	m_nOldSelect	= SELECT_YES;	// �O��̑I��

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
	m_pBigFrame->BindTexture(mc_apTextureFile[TEXTURE_FRAME]);

	// �D�揇�ʂ�ݒ�
	m_pBigFrame->SetPriority(RESULT_PRIO);

	//--------------------------------------------------------
	//	�������S�����E�ݒ�
	//--------------------------------------------------------
	for (int nCnt = 0; nCnt < NUM_WIN; nCnt++)
	{
		m_apWinLog[nCnt] = CObject2D::Create
		(
			D3DXVECTOR3(Win::POS.x + Win::DISTANCE[Win::NUMBER_ZERO] * nCnt , Win::POS.y, Win::POS.z),
			Win::INIT_SIZE
		);
		
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
			D3DXVECTOR3(450.0f + 300.0f * nCnt,650.0f, 0.0f),
			D3DXVECTOR3(150.0f, 150.0f, 0.0f)
		);

		if (m_apSelect[nCnt] == NULL)
		{ // �����Ɏ��s�����ꍇ

		  // ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apSelect[nCnt]->SetPriority(RESULT_PRIO);

		m_apSelect[nCnt]->SetEnableDraw(false);
	}


	// �e�N�X�`����ݒ�
	m_apSelect[SELECT_YES]->BindTexture(mc_apTextureFile[TEXTURE_RESTART]);
	m_apSelect[SELECT_NO]->BindTexture(mc_apTextureFile[TEXTURE_BACK]);

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
		if (m_apFrame[nCnt] != nullptr)
		{
			m_apFrame[nCnt]->Uninit();
			m_apFrame[nCnt] = nullptr;
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

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CResultManager::Update(void)
{
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

		//�������S�̐�������
		for (int nCnt = 0; nCnt < NUM_WIN; nCnt++)
		{
			//���g�`�F�b�N
			if (m_apWinLog[nCnt] != nullptr)
			{
				m_apWinLog[nCnt]->Update();

				//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
				m_arSize[EObj::OBJ_WIN].x += (Win::DESTSIZE[Win::NUMBER_ZERO] + m_arSize[EObj::OBJ_WIN].x) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

				//���̃T�C�Y�𒴂������ɂȂ�����
				if (m_arSize[EObj::OBJ_WIN].x >= Win::DESTSIZE[Win::NUMBER_ZERO])
				{
					//�l���Œ肷��
					m_arSize[EObj::OBJ_WIN].x = Win::DESTSIZE[Win::NUMBER_ZERO];
				}

				//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������

				m_arSize[EObj::OBJ_WIN].y += (Win::DESTSIZE[Win::NUMBER_ZERO] + m_arSize[EObj::OBJ_WIN].y) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

				//���̃T�C�Y�𒴂������ɂȂ�����
				if (m_arSize[EObj::OBJ_WIN].y >= Win::DESTSIZE[Win::NUMBER_ZERO])
				{//�l���Œ肷��
					m_arSize[EObj::OBJ_WIN].y = Win::DESTSIZE[Win::NUMBER_ZERO];
				}

				//�T�C�Y�̐ݒ���s��
				m_apWinLog[nCnt]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN]);

				//�ǂ�����ړI�̈ʒu�ɓ������Ă�����
				if (m_arSize[EObj::OBJ_WIN].x == Win::DESTSIZE[Win::NUMBER_ZERO] &&
					m_arSize[EObj::OBJ_WIN].y == Win::DESTSIZE[Win::NUMBER_ZERO])
				{
					m_anWaitTime[EObj::OBJ_WIN]++;
				}

				if (m_anWaitTime[EObj::OBJ_WIN] >= Win::MAX_WAIT)
				{
					m_anWaitTime[EObj::OBJ_WIN] = Win::MAX_WAIT;

					//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
					m_arPos[EObj::OBJ_WIN].x -= (Win::DESTPOS.x - m_arPos[EObj::OBJ_WIN].x) * Win::VALUE_INERTIA[Win::NUMBER_ONE];
					
					//���̃T�C�Y�𒴂������ɂȂ�����
					if (m_arPos[EObj::OBJ_WIN].x <= Win::DESTPOS.x)
					{
						//�l���Œ肷��
						m_arPos[EObj::OBJ_WIN].x = Win::DESTPOS.x;
					}

					//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������

					m_arPos[EObj::OBJ_WIN].y -= (Win::DESTPOS.y + m_arPos[EObj::OBJ_WIN].y) * Win::VALUE_INERTIA[Win::NUMBER_ONE];

					//���̃T�C�Y�𒴂������ɂȂ�����
					if (m_arPos[EObj::OBJ_WIN].y <= Win::DESTPOS.y)
					{//�l���Œ肷��
						m_arPos[EObj::OBJ_WIN].y = Win::DESTPOS.y;
					}

					//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������
					m_arSize[EObj::OBJ_WIN].x -= (Win::DESTSIZE[Win::NUMBER_ONE] + m_arSize[EObj::OBJ_WIN].x) * Win::VALUE_INERTIA[Win::NUMBER_ONE];

					//���̃T�C�Y�𒴂������ɂȂ�����
					if (m_arSize[EObj::OBJ_WIN].x <= Win::DESTSIZE[Win::NUMBER_ONE])
					{
						//�l���Œ肷��
						m_arSize[EObj::OBJ_WIN].x = Win::DESTSIZE[Win::NUMBER_ONE];
					}

					//�ړI�̈ʒu�ɓ�������܂ŉ��Z��������

					m_arSize[EObj::OBJ_WIN].y -= (Win::DESTSIZE[Win::NUMBER_ONE] + m_arSize[EObj::OBJ_WIN].y)* Win::VALUE_INERTIA[Win::NUMBER_ONE];

					//���̃T�C�Y�𒴂������ɂȂ�����
					if (m_arSize[EObj::OBJ_WIN].y <= Win::DESTSIZE[Win::NUMBER_ONE])
					{//�l���Œ肷��
						m_arSize[EObj::OBJ_WIN].y = Win::DESTSIZE[Win::NUMBER_ONE];
					}

					//�T�C�Y�̐ݒ���s��
					m_apWinLog[nCnt]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN]);
					m_apWinLog[nCnt]->SetVec3Position(D3DXVECTOR3(
						m_arPos[EObj::OBJ_WIN].x+ Win::DISTANCE[Win::NUMBER_ONE] * nCnt, 
						m_arPos[EObj::OBJ_WIN].y,
						m_arPos[EObj::OBJ_WIN].z));

					//�ǂ�����ړI�̈ʒu�ɓ������Ă�����
					if (m_arSize[EObj::OBJ_WIN].x == Win::DESTSIZE[Win::NUMBER_ONE]&&
						m_arSize[EObj::OBJ_WIN].y == Win::DESTSIZE[Win::NUMBER_ONE]&&
						m_arPos[EObj::OBJ_WIN].x == Win::DESTPOS.x&&
						m_arPos[EObj::OBJ_WIN].y == Win::DESTPOS.y)
					{
						m_anWaitTime[EObj::OBJ_WIN] = 0;
						m_state = STATE_BIG_FRAME;
					}
				}
			}

		}

		break;

		//����t���[��
	case STATE_BIG_FRAME:

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

			if (m_arSize[EObj::OBJ_BIGFRAME].x == BigFrame::DESTSIZE.x
				&&m_arSize[EObj::OBJ_BIGFRAME].y == BigFrame::DESTSIZE.y)
			{
				m_anWaitTime[EObj::OBJ_BIGFRAME]++;

				if (m_anWaitTime[EObj::OBJ_BIGFRAME] == BigFrame::MAX_WAIT)
				{
					m_anWaitTime[EObj::OBJ_BIGFRAME] = 0;
					m_state = STATE_FRAME;
				}
			}
		}

		break;

		//�t���[��
	case STATE_FRAME:

		//���g�`�F�b�N
		if (m_apFrame[m_anNum[EObj::OBJ_FRAME]] != nullptr)
		{
			//�`�������悤�ɂ���
			m_apFrame[m_anNum[EObj::OBJ_FRAME]]->SetEnableDraw(true);
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
			if (m_arSize[EObj::OBJ_FRAME].y>= Frame::DESTSIZE.y)
			{
				//�l���Œ肷��
				m_arSize[EObj::OBJ_FRAME].y= Frame::DESTSIZE.y;
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
					m_anNum[EObj::OBJ_FRAME] = Frame::NUM;
				}
				//�͈͊O�ɂ����Ă��Ȃ����
				else if (!(m_anNum[EObj::OBJ_FRAME] >= Frame::NUM))
				{
					//�ҋ@���Ԃ����Z����
					m_anWaitTime[EObj::OBJ_FRAME]++;

					//�ҋ@���Ԃ̍ő�l�𒴂������ɂȂ�����
					if (m_anWaitTime[EObj::OBJ_FRAME] >= Frame::MAX_WAIT)
					{
						//�l�̃��Z�b�g���s��
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
				//�J�ڑҋ@��ԂɈڍs����
				m_state = STATE_WAIT;
			}
		}

		break;

	case STATE_WAIT:	// �J�ڑҋ@���

		for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
		{
			if (m_apSelect[nCnt] != nullptr)
			{
				m_apSelect[nCnt]->SetEnableDraw(true);
				m_apSelect[nCnt]->Update();
			}

		}

		//�I���̍X�V
		UpdateSelect();

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �J�ڌ���̍X�V
	UpdateTransition();

	// �t�F�[�h�̍X�V
	m_pFade->Update();

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
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h

	if (pKeyboard->IsTrigger(DIK_A)
	||  pKeyboard->IsTrigger(DIK_LEFT)
	||  pPad->IsTrigger(CInputPad::KEY_LEFT))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// ���ɑI�������炷
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		// �T�E���h�̍Đ�
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
	}
	if (pKeyboard->IsTrigger(DIK_D)
	||  pKeyboard->IsTrigger(DIK_RIGHT)
	||  pPad->IsTrigger(CInputPad::KEY_RIGHT))
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		// �E�ɑI�������炷
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;

		// �T�E���h�̍Đ�
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
	}

	// �O��̑I��v�f�̐F�����ɐݒ�
	m_apSelect[m_nOldSelect]->SetColor(DEFAULT_COL);

	// ���݂̑I��v�f�̐F�𔒂ɐݒ�
	m_apSelect[m_nSelect]->SetColor(CHOICE_COL);

	// ���݂̑I��v�f����
	m_nOldSelect = m_nSelect;

#endif
}

//============================================================
//	�J�ڌ���̍X�V����
//============================================================
void CResultManager::UpdateTransition(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h

	if (pKeyboard->IsTrigger(DIK_RETURN)
	||  pKeyboard->IsTrigger(DIK_SPACE)
	||  pPad->IsTrigger(CInputPad::KEY_A)
	||  pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X)
	||  pPad->IsTrigger(CInputPad::KEY_Y)
	||  pPad->IsTrigger(CInputPad::KEY_START))
	{
		if (m_state != STATE_WAIT)
		{ // �J�ڑҋ@��Ԃł͂Ȃ��ꍇ

			// ���o�X�L�b�v
			/*SkipStaging();*/

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
	}
}

//============================================================
//	���o�X�L�b�v����
//============================================================
void CResultManager::SkipStaging(void)
{
	// �t�F�[�h�̓����x��ݒ�
	m_pFade->SetColor(Fade::SETCOL_FADE);

	// ��Ԃ�ύX
	m_state = STATE_WAIT;	// �J�ڑҋ@���
}
