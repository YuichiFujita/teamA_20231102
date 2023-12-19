//============================================================
//
//	�����L���O�}�l�[�W���[���� [rankingManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "rankingManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"
#include "texture.h"
#include "object2D.h"
#include "anim2D.h"
#include "timerManager.h"
#include "retentionManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	// �����L���O��{���
	const char* RANKING_BIN = "data\\BIN\\ranking.bin";	// �����L���O���o�C�i��

	const int		PRIORITY	= 14;	// �����L���O�̗D�揇��
	const D3DXCOLOR	COL_NEWRANK = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);	// �ϓ������X�R�A�̐F

	// �t�F�[�h��{���
	namespace fade
	{
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(SCREEN_SIZE.x * 0.7f, SCREEN_SIZE.y * 0.95f, 0.0f);	// �t�F�[�h�̑傫��
		const D3DXCOLOR		SET_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);	// ���l�̒�~�l

		const D3DXCOLOR		INIT_COL	= XCOL_AWHITE;	// ���l�̏����l
		const float			ADD_ALPHA	= 0.008f;		// ���l�̉��Z��
	}

	// �����L���O���S��{���
	namespace logo
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, 115.0f, 0.0f);	// �����L���O���S�\���̈ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(473.0f, 199.0f, 0.0f);		// �����L���O���S�\���̑傫��
		const float SET_SCALE	= 15.0f;	// �����L���O���S�\���̏����g�嗦
		const float SUB_SCALE	= 0.65f;	// �����L���O���S�\���̊g�嗦���Z��
	}

	// ���ʊ�{���
	namespace rank
	{
		const D3DXVECTOR3	POS		= D3DXVECTOR3(310.0f, 250.0f, 0.0f);	// ���ʕ\���̈ʒu
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(183.0f, 108.0f, 0.0f);	// ���ʕ\���̑傫��
		const D3DXVECTOR3	SPACE	= D3DXVECTOR3(0.0f, 100.0f, 0.0f);		// ���ʕ\���̋�

		const POSGRID2	TEX_PART	= POSGRID2(NUM_RANKING, 1);	// ���ʕ\���̃e�N�X�`��������
		const D3DXCOLOR	SET_COL		= XCOL_WHITE;	// ���l�̒�~�l
		const D3DXCOLOR	INIT_COL	= XCOL_AWHITE;	// ���l�̏����l
		const float		ADD_ALPHA	= 0.02f;		// ���l�̉��Z��
	}

	// �N���A�^�C����{���
	namespace time
	{
		const D3DXVECTOR3 POS			= D3DXVECTOR3(485.0f, 250.0f, 0.0f);	// �N���A�^�C���\���̈ʒu
		const D3DXVECTOR3 SPACE			= D3DXVECTOR3(0.0f, 100.0f, 0.0f);		// �N���A�^�C���\���̋�
		const D3DXVECTOR3 SIZE_VAL		= D3DXVECTOR3(88.0f, 105.0f, 0.0f);		// �N���A�^�C���\���̐����傫��
		const D3DXVECTOR3 SIZE_PART		= D3DXVECTOR3(45.0f, 105.0f, 0.0f);		// �N���A�^�C���\���̋�؂�傫��
		const D3DXVECTOR3 SPACE_VAL		= D3DXVECTOR3(SIZE_VAL.x  * 0.85f, 0.0f, 0.0f);	// �N���A�^�C���\���̐�����
		const D3DXVECTOR3 SPACE_PART	= D3DXVECTOR3(SIZE_PART.x * 0.85f, 0.0f, 0.0f);	// �N���A�^�C���\���̋�؂��

		const float	SET_SCALE		= 0.05f;	// �N���A�^�C���\���̏����g�嗦
		const float	ADD_SCALE		= 0.1f;		// �N���A�^�C���\���̊g�嗦���Z��
		const float	MAX_SUB_SCALE	= 0.085f;	// �N���A�^�C���\���̊g�嗦�̍ő匸�Z��
		const int	WAIT_CNT		= 15;		// �N���A�^�C���\���ҋ@�t���[��
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CRankingManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\ranking000.png",	// �����L���O���S�e�N�X�`��
	"data\\TEXTURE\\ranking001.png",	// �����L���O���ʃe�N�X�`��
};
long CRankingManager::m_aRanking[NUM_RANKING] = { 0 };	// �����L���O���
int CRankingManager::m_nNewRankID = NONE_IDX;	// �ϓ������X�R�A�̃C���f�b�N�X

//************************************************************
//	�e�N���X [CRankingManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRankingManager::CRankingManager()
{
	// �����o�ϐ����N���A
	memset(&m_apTime[0], 0, sizeof(m_apTime));	// �N���A�^�C���̏��
	memset(&m_apRank[0], 0, sizeof(m_apRank));	// ���ʂ̏��
	m_pLogo		= NULL;			// �����L���O���S�̏��
	m_pFade		= NULL;			// �t�F�[�h�̏��
	m_state		= STATE_NONE;	// ���
	m_fScale	= 0.0f;			// �|���S���g�嗦
	m_nCounterState = 0;		// ��ԊǗ��J�E���^�[
	m_nCounterDraw = 0;			// �`��Ǘ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CRankingManager::~CRankingManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CRankingManager::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apTime[0], 0, sizeof(m_apTime));	// �N���A�^�C���̏��
	memset(&m_apRank[0], 0, sizeof(m_apRank));	// ���ʂ̏��
	m_pLogo		= NULL;					// �����L���O���S�̏��
	m_pFade		= NULL;					// �t�F�[�h�̏��
	m_state		= STATE_FADEIN;			// ���
	m_fScale	= 0.0f;					// �|���S���g�嗦
	m_nCounterState = 0;				// ��ԊǗ��J�E���^�[
	m_nCounterDraw = NUM_RANKING - 1;	// �`��Ǘ��J�E���^�[

	// �����L���O�Ǎ�
	Load();

	//--------------------------------------------------------
	//	�t�F�[�h�̐����E�ݒ�
	//--------------------------------------------------------
	// �t�F�[�h�̐���
	m_pFade = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		fade::SIZE,		// �傫��
		VEC3_ZERO,		// ����
		fade::INIT_COL	// �F
	);
	if (m_pFade == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pFade->SetPriority(PRIORITY);

	//--------------------------------------------------------
	//	�����L���O���S�\���̐����E�ݒ�
	//--------------------------------------------------------
	// �����L���O���S�\���̐���
	m_pLogo = CObject2D::Create
	( // ����
		logo::POS,						// �ʒu
		logo::SIZE * logo::SET_SCALE	// �傫��
	);
	if (m_pLogo == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����ݒ�
	m_pLogo->BindTexture(mc_apTextureFile[TEXTURE_LOGO]);

	// �D�揇�ʂ�ݒ�
	m_pLogo->SetPriority(PRIORITY);

	// �`������Ȃ��ݒ�ɂ���
	m_pLogo->SetEnableDraw(false);

	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // �����L���O�̏�ʕ\�������J��Ԃ�

		//----------------------------------------------------
		//	���ʕ\���̐����E�ݒ�
		//----------------------------------------------------
		// ���ʕ\���̐���
		m_apRank[nCntRank] = CAnim2D::Create
		( // ����
			rank::TEX_PART.x,								// ��������
			rank::TEX_PART.y,								// �c������
			rank::POS + (rank::SPACE * (float)nCntRank),	// �ʒu
			rank::SIZE,										// �傫��
			VEC3_ZERO,										// ����
			rank::INIT_COL									// �F
		);
		if (m_apRank[nCntRank] == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����ݒ�
		m_apRank[nCntRank]->BindTexture(mc_apTextureFile[TEXTURE_RANK]);

		// �D�揇�ʂ�ݒ�
		m_apRank[nCntRank]->SetPriority(PRIORITY);

		// �`������Ȃ��ݒ�ɂ���
		m_apRank[nCntRank]->SetEnableDraw(false);

		// �p�^�[���̐ݒ�
		m_apRank[nCntRank]->SetPattern(nCntRank);

		//----------------------------------------------------
		//	�N���A�^�C���\���̐����E�ݒ�
		//----------------------------------------------------
		// �N���A�^�C���\���̐���
		m_apTime[nCntRank] = CTimerManager::Create
		( // ����
			CTimerManager::TIME_SEC,						// �ݒ�^�C��
			0,												// ��������
			time::POS + (time::SPACE * (float)nCntRank),	// �ʒu
			time::SIZE_VAL * time::SET_SCALE,				// �����̑傫��
			time::SIZE_PART * time::SET_SCALE,				// ��؂�̑傫��
			time::SPACE_VAL,								// �����̋�
			time::SPACE_PART								// ��؂�̋�
		);
		if (m_apTime[nCntRank] == NULL)
		{ // ��g�p���̏ꍇ
	
			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	
		// �D�揇�ʂ�ݒ�
		m_apTime[nCntRank]->SetPriority(PRIORITY);
	
		// �`������Ȃ��ݒ�ɂ���
		m_apTime[nCntRank]->SetEnableDraw(false);

		if (m_nNewRankID == nCntRank)
		{ // �l�̕ϓ����������ꍇ

			// �F��ݒ�
			m_apTime[nCntRank]->SetColor(COL_NEWRANK);

			// �ϓ������X�R�A�̃C���f�b�N�X��������
			m_nNewRankID = NONE_IDX;
		}
	
		// �^�C����ݒ�
		if (!m_apTime[nCntRank]->SetMSec(m_aRanking[nCntRank]))
		{ // �ݒ�Ɏ��s�����ꍇ
	
			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CRankingManager::Uninit(void)
{
	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // �����L���O�̏�ʕ\�������J��Ԃ�

		// �N���A�^�C���̔j��
		if (FAILED(CTimerManager::Release(m_apTime[nCntRank])))
		{ // �j���Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// ���ʕ\���̏I��
		m_apRank[nCntRank]->Uninit();
	}

	// �����L���O���S�̏I��
	m_pLogo->Uninit();

	// �t�F�[�h�̏I��
	m_pFade->Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CRankingManager::Update(void)
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

	case STATE_LOGO:	// �����L���O���S�\�����

		// �����L���O���S�\���̍X�V
		UpdateLogo();

		break;

	case STATE_RANK:	// ���ʕ\�����

		// ���ʕ\���̍X�V
		UpdateRank();

		break;

	case STATE_TIME_WAIT:	// �N���A�^�C���\���ҋ@���

		// �\���ҋ@�̍X�V
		if (UpdateDrawWait(time::WAIT_CNT))
		{ // �ҋ@�����̏ꍇ

			// �N���A�^�C���\���̏�����
			InitTime();
		}

		break;

	case STATE_TIME:	// �N���A�^�C���\�����

		// �N���A�^�C���\���̍X�V
		UpdateTime();

		break;

	case STATE_WAIT:	// �J�ڑҋ@���

		// ����

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �J�ڌ���̍X�V
	UpdateTransition();

	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // �����L���O�̏�ʕ\�������J��Ԃ�

		// �N���A�^�C���̍X�V
		m_apTime[nCntRank]->Update();

		// ���ʕ\���̍X�V
		m_apRank[nCntRank]->Update();
	}

	// �����L���O���S�̍X�V
	m_pLogo->Update();

	// �t�F�[�h�̍X�V
	m_pFade->Update();
}

//============================================================
//	�ݒ菈��
//============================================================
void CRankingManager::Set(const long nValue)
{
	// �ϐ���錾
	long nLowestID = NUM_RANKING - 1;	// �ŉ��ʂ̔z��C���f�b�N�X

	// �Ǎ�
	Load();

	if (nValue < m_aRanking[nLowestID])
	{ // �ŉ��ʂ̃N���A�^�C����葁���N���A�^�C���̏ꍇ

		// �\�[�g
		Sort(nValue);

		// �ۑ�
		Save();

		// �X�R�A�ϓ��C���f�b�N�X�̐ݒ�
		SetNewRank(nValue);
	}
}

//============================================================
//	��������
//============================================================
CRankingManager *CRankingManager::Create(void)
{
	// �|�C���^��錾
	CRankingManager *pRankingManager = NULL;	// �����L���O�}�l�[�W���[�����p

	if (pRankingManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pRankingManager = new CRankingManager;	// �����L���O�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pRankingManager != NULL)
	{ // �g�p����Ă���ꍇ
		
		// �����L���O�}�l�[�W���[�̏�����
		if (FAILED(pRankingManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pRankingManager;
			pRankingManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pRankingManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CRankingManager::Release(CRankingManager *&prRankingManager)
{
	if (prRankingManager != NULL)
	{ // �g�p���̏ꍇ

		// �����L���O�}�l�[�W���[�̏I��
		if (FAILED(prRankingManager->Uninit()))
		{ // �j���Ɏ��s�����ꍇ

			// �������J��
			delete prRankingManager;
			prRankingManager = NULL;

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �������J��
		delete prRankingManager;
		prRankingManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�t�F�[�h�C���̍X�V����
//============================================================
void CRankingManager::UpdateFade(void)
{
	// �ϐ���錾
	D3DXCOLOR colFade = m_pFade->GetColor();	// �t�F�[�h�̐F

	if (colFade.a < fade::SET_COL.a)
	{ // �����ʂ��ݒ�l�����̏ꍇ

		// �����x�����Z
		colFade.a += fade::ADD_ALPHA;
	}
	else
	{ // �����ʂ��ݒ�l�ȏ�̏ꍇ

		// �����x��␳
		colFade.a = fade::SET_COL.a;

		// �����L���O���S�\���̏�����
		InitLogo();

		// ��Ԃ�ύX
		m_state = STATE_LOGO;	// �����L���O���S�\�����
	}

	// �����x�𔽉f
	m_pFade->SetColor(colFade);
}

//============================================================
//	�t�F�[�h�C���̕␳����
//============================================================
void CRankingManager::RevisionFade(void)
{
	// �t�F�[�h�̓����x��ݒ�
	m_pFade->SetColor(fade::SET_COL);
}

//============================================================
//	�����L���O���S�\���̏���������
//============================================================
void CRankingManager::InitLogo(void)
{
	// �����`���ON�ɐݒ�
	m_pLogo->SetEnableDraw(true);

	// �����L���O���S�\���̊g�嗦��ݒ�
	m_fScale = logo::SET_SCALE;
}

//============================================================
//	�����L���O���S�\���̍X�V����
//============================================================
void CRankingManager::UpdateLogo(void)
{
	if (m_fScale > 1.0f)
	{ // �g�嗦���ŏ��l���傫���ꍇ

		// �g�嗦�����Z
		m_fScale -= logo::SUB_SCALE;

		// �����L���O���S�\���̑傫����ݒ�
		m_pLogo->SetVec3Sizing(logo::SIZE * m_fScale);
	}
	else
	{ // �g�嗦���ŏ��l�ȉ��̏ꍇ

		// �����L���O�\���̕␳
		RevisionLogo();

		// ���ʕ\���̏�����
		InitRank();

		// ��Ԃ�ύX
		m_state = STATE_RANK;	// ���ʕ\�����

		// �T�E���h�̍Đ�
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
	}
}

//============================================================
//	�����L���O���S�\���̕␳����
//============================================================
void CRankingManager::RevisionLogo(void)
{
	// �g�嗦��������
	m_fScale = 1.0f;

	// �����L���O���S�\���̑傫����ݒ�
	m_pLogo->SetVec3Sizing(logo::SIZE);
}

//============================================================
//	���ʕ\���̏���������
//============================================================
void CRankingManager::InitRank(void)
{
	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // �����L���O�̏�ʕ\�������J��Ԃ�

		// �����x�𔽉f
		m_apRank[nCntRank]->SetEnableDraw(true);
	}
}

//============================================================
//	���ʕ\���̍X�V����
//============================================================
void CRankingManager::UpdateRank(void)
{
	// �ϐ���錾
	D3DXCOLOR colRank = m_apRank[0]->GetColor();	// ���ʂ̐F

	if (colRank.a < rank::SET_COL.a)
	{ // �����ʂ��ݒ�l�����̏ꍇ

		// �����x�����Z
		colRank.a += rank::ADD_ALPHA;
	}
	else
	{ // �����ʂ��ݒ�l�ȏ�̏ꍇ

		// �����x��␳
		colRank.a = rank::SET_COL.a;

		// ��Ԃ�ύX
		m_state = STATE_TIME_WAIT;	// �N���A�^�C���\���ҋ@���
	}

	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // �����L���O�̏�ʕ\�������J��Ԃ�

		// �����x�𔽉f
		m_apRank[nCntRank]->SetColor(colRank);
	}
}

//============================================================
//	���ʕ\���̕␳����
//============================================================
void CRankingManager::RevisionRank(void)
{
	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // �����L���O�̏�ʕ\�������J��Ԃ�

		// �����x�𔽉f
		m_apRank[nCntRank]->SetColor(rank::SET_COL);
	}
}

//============================================================
//	�N���A�^�C���\���̏���������
//============================================================
void CRankingManager::InitTime(void)
{
	if (m_nCounterDraw >= 0)
	{ // �`�悵�I����Ă��Ȃ��ꍇ

		// �����`���ON�ɐݒ�
		m_apTime[m_nCounterDraw]->SetEnableDraw(true);

		// �N���A�^�C���\���̊g�嗦��ݒ�
		m_fScale = time::SET_SCALE;

		// �N���A�^�C���\����Ԃɂ���
		m_state = STATE_TIME;
	}
	else
	{ // �`�悵�I������ꍇ

		// �g�嗦��������
		m_fScale = 1.0f;

		// �ҋ@��Ԃɂ���
		m_state = STATE_WAIT;
	}
}

//============================================================
//	�N���A�^�C���\���̍X�V����
//============================================================
void CRankingManager::UpdateTime(void)
{
	if (m_fScale < 1.0f)
	{ // �g�嗦���ŏ��l���傫���ꍇ

		// �g�嗦�����Z
		m_fScale += time::ADD_SCALE - ((time::MAX_SUB_SCALE) / (float)(NUM_RANKING - 1)) * fabsf((float)m_nCounterDraw - (float)(NUM_RANKING - 1));

		// �N���A�^�C���\���̑傫����ݒ�
		m_apTime[m_nCounterDraw]->SetScalingValue(time::SIZE_VAL * m_fScale);
		m_apTime[m_nCounterDraw]->SetScalingPart(time::SIZE_PART * m_fScale);
	}
	else
	{ // �g�嗦���ŏ��l�ȉ��̏ꍇ

		// �N���A�^�C���\���̕␳
		RevisionTime();

		// ��Ԃ�ύX
		m_state = STATE_TIME_WAIT;	// �N���A�^�C���\���ҋ@���

		// �T�E���h�̍Đ�
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
	}
}

//============================================================
//	�N���A�^�C���\���̕␳����
//============================================================
void CRankingManager::RevisionTime(void)
{
	// �g�嗦��������
	m_fScale = 1.0f;

	// �N���A�^�C���\���̑傫����ݒ�
	m_apTime[m_nCounterDraw]->SetScalingValue(time::SIZE_VAL);
	m_apTime[m_nCounterDraw]->SetScalingPart(time::SIZE_PART);

	// �`��J�E���^�[�����Z
	m_nCounterDraw--;
}

//============================================================
//	�J�ڌ���̍X�V����
//============================================================
void CRankingManager::UpdateTransition(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h

	if (pKeyboard->IsTrigger(DIK_RETURN)
	||  pKeyboard->IsTrigger(DIK_SPACE)
	||  pPad->IsTriggerAll(CInputPad::KEY_A)
	||  pPad->IsTriggerAll(CInputPad::KEY_B)
	||  pPad->IsTriggerAll(CInputPad::KEY_X)
	||  pPad->IsTriggerAll(CInputPad::KEY_Y)
	||  pPad->IsTriggerAll(CInputPad::KEY_START))
	{
		if (m_state != STATE_WAIT)
		{ // �J�ڑҋ@��Ԃł͂Ȃ��ꍇ

			// ���o�X�L�b�v
			SkipStaging();

			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
		}
		else
		{ // �J�ڑҋ@��Ԃ̏ꍇ

			if (CManager::GetInstance()->GetFade()->GetState() == CFade::FADE_NONE)
			{ // �t�F�[�h���ł͂Ȃ��ꍇ

				// �V�[���̐ݒ�
				CManager::GetInstance()->SetScene(CScene::MODE_TITLE);	// �^�C�g�����

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// ���艹00
			}
		}
	}
}

//============================================================
//	���o�X�L�b�v����
//============================================================
void CRankingManager::SkipStaging(void)
{
	// �t�F�[�h�C���̕␳
	RevisionFade();

	// �����L���O���S�\���̏������E�␳
	InitLogo();
	RevisionLogo();

	// ���ʕ\���̏������E�␳
	InitRank();
	RevisionRank();

	// �N���A�^�C���\���̏������E�␳
	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // �����L���O�̏�ʕ\�������J��Ԃ�

		// �����`���ON�ɐݒ�
		m_apTime[nCntRank]->SetEnableDraw(true);

		// �N���A�^�C���\���̑傫����ݒ�
		m_apTime[nCntRank]->SetScalingValue(time::SIZE_VAL);
		m_apTime[nCntRank]->SetScalingPart(time::SIZE_PART);
	}

	// ��Ԃ�ύX
	m_state = STATE_WAIT;	// �J�ڑҋ@���
}

//============================================================
//	�\���ҋ@����
//============================================================
bool CRankingManager::UpdateDrawWait(const int nWait)
{
	if (m_nCounterState < nWait)
	{ // �J�E���^�[���ҋ@�J�E���g�܂ŒB���Ă��Ȃ��ꍇ

		// �J�E���^�[�����Z
		m_nCounterState++;

		// �ҋ@��������Ԃ�
		return false;
	}
	else
	{ // �J�E���^�[���ҋ@���������ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �ҋ@������Ԃ�
		return true;
	}
}

//============================================================
//	�\�[�g����
//============================================================
void CRankingManager::Sort(const long nValue)
{
	// �ϐ���錾
	long nKeepNum;		// �\�[�g�p
	int	nCurrentMinID;	// �ŏ��l�̃C���f�b�N�X

	// ���݂̍ŉ��ʂ̏��Ə�������
	m_aRanking[NUM_RANKING - 1] = nValue;

	for (int nCntKeep = 0; nCntKeep < (NUM_RANKING - 1); nCntKeep++)
	{ // ����ւ���l�̑��� -1�񕪌J��Ԃ�

		// ���݂̌J��Ԃ������� (�v�f1�Ƃ���)
		nCurrentMinID = nCntKeep;

		for (int nCntSort = nCntKeep + 1; nCntSort < NUM_RANKING; nCntSort++)
		{ // ����ւ���l�̑��� -nCntKeep���J��Ԃ�

			if (m_aRanking[nCurrentMinID] > m_aRanking[nCntSort])	
			{ // �ŏ��l�ɐݒ肳��Ă���l���A���݂̒l�̂ق����������ꍇ

				// ���݂̗v�f�ԍ����ŏ��l�ɐݒ�
				nCurrentMinID = nCntSort;
			}
		}

		if (nCntKeep != nCurrentMinID)
		{ // �ŏ��l�̗v�f�ԍ��ɕϓ����������ꍇ

			// �v�f�̓���ւ�
			nKeepNum					= m_aRanking[nCntKeep];
			m_aRanking[nCntKeep]		= m_aRanking[nCurrentMinID];
			m_aRanking[nCurrentMinID]	= nKeepNum;
		}
	}
}

//============================================================
//	�X�R�A�ϓ��C���f�b�N�X�̐ݒ菈��
//============================================================
void CRankingManager::SetNewRank(const long nValue)
{
	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // �����L���O�̏�ʕ\�������J��Ԃ�

		if (m_aRanking[nCntRank] == nValue)
		{ // ����ւ�����l�ƈ�v�����ꍇ

			// ��v�����l��ݒ�
			m_nNewRankID = nCntRank;
		}
	}
}

//============================================================
//	�ۑ�����
//============================================================
void CRankingManager::Save(void)
{
	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �o�C�i���t�@�C���������o�������ŊJ��
	pFile = fopen(RANKING_BIN, "wb");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		// �t�@�C���ɐ��l�������o��
		fwrite(&m_aRanking[0], sizeof(long), NUM_RANKING, pFile);

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�����L���O�t�@�C���̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}

//============================================================
//	�Ǎ�����
//============================================================
void CRankingManager::Load(void)
{
	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �o�C�i���t�@�C����ǂݍ��ݕ����ŊJ��
	pFile = fopen(RANKING_BIN, "rb");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		// �t�@�C���̐��l��ǂݍ���
		fread(&m_aRanking[0], sizeof(long), NUM_RANKING, pFile);

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�����L���O�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// �o�C�i���t�@�C���������o�������ŊJ��
		pFile = fopen(RANKING_BIN, "wb");

		if (pFile != NULL)
		{ // �t�@�C�����J�����ꍇ

			// �ϐ��z���錾
			long nMaxTime = CTimerManager::GetMaxTime();	// �ő�^�C��

			for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
			{ // �����L���O�̏�ʕ\�������J��Ԃ�

				// �����L���O�����N���A
				m_aRanking[nCntRank] = nMaxTime;
			}

			// �t�@�C���ɐ��l�������o��
			fwrite(&m_aRanking[0], sizeof(long), NUM_RANKING, pFile);

			// �t�@�C�������
			fclose(pFile);
		}
		else
		{ // �t�@�C�����J���Ȃ������ꍇ

			// �G���[���b�Z�[�W�{�b�N�X
			MessageBox(NULL, "�����L���O�t�@�C���̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
		}
	}
}
