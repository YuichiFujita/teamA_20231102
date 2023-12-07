//============================================================
//
//	���ԃ��U���g�}�l�[�W���[���� [middleResultManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "middleResultManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"
#include "valueUI.h"
#include "multiValue.h"
#include "fade.h"
#include "player.h"
#include "retentionManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 14;	// ���ԃ��U���g�̗D�揇��
	const int WAIT_FRAME = 8;	// �J�ڂ̗]�C�t���[��

	// �t�F�[�h��{���
	namespace fade
	{
		const D3DXVECTOR3 UP_POS	= D3DXVECTOR3(SCREEN_CENT.x, -900.0f, 0.0f);					// �t�F�[�h��ʒu
		const D3DXVECTOR3 CENT_POS	= D3DXVECTOR3(SCREEN_CENT.x, SCREEN_CENT.y, 0.0f);				// �t�F�[�h�����ʒu
		const D3DXVECTOR3 DOWN_POS	= D3DXVECTOR3(SCREEN_CENT.x, SCREEN_HEIGHT + 900.0f, 0.0f);		// �t�F�[�h���ʒu
		const D3DXVECTOR3 UP_MIDDLE_POS		= D3DXVECTOR3(CENT_POS.x, UP_POS.y + 300.0f, 0.0f);		// �t�F�[�h�㒆���ʒu
		const D3DXVECTOR3 DOWN_MIDDLE_POS	= D3DXVECTOR3(CENT_POS.x, DOWN_POS.y - 300.0f, 0.0f);	// �t�F�[�h�������ʒu

		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(SCREEN_SIZE.x, 1770.0f, 0.0f);	// �t�F�[�h�傫��
		const D3DXCOLOR		COL		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.88f);			// �t�F�[�h�F

		const int	WAIT_FRAME	= 8;		// �t�F�[�h�ҋ@�t���[��
		const float	ADD_MOVE	= 0.65f;	// �t�F�[�h�ړ���
		const float	ADD_ACCEL_MOVE = 2.1f;	// �t�F�[�h�����ړ���
	}

	// �����L���O�^�C�g����{���
	namespace title
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(280.0f, 160.0f, 0.0f);	// �^�C�g���ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(400.0f, 130.0f, 0.0f);	// �^�C�g���傫��

		const int	WAIT_FRAME	= 24;		// �^�C�g���ҋ@�t���[��
		const float	INIT_SCALE	= 0.025f;	// �^�C�g�������g�嗦
		const float	ADD_SCALE	= 0.09f;	// �^�C�g�����Z�g�嗦
		const float	SET_SCALE	= 1.0f;		// �^�C�g���ݒ�g�嗦
	}

	// �����|�C���g��{���
	namespace win
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(760.0f, 160.0f, 0.0f);			// �ʒu
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(230.0f, 0.0f, 0.0f);				// �s��
		const D3DXVECTOR3 SPACE_VALUE	= D3DXVECTOR3(80.0f, 0.0f, 0.0f);		// �����s��
		const D3DXVECTOR3 SIZE_TITLE	= D3DXVECTOR3(300.0f, 100.0f, 0.0f);	// �^�C�g���傫��
		const D3DXVECTOR3 SIZE_VALUE	= D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// �����傫��

		const float	DIGIT = 2;	// ����
	}

	// �����|�C���g�w�i��{���
	namespace winBG
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(860.0f, 160.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(620.0f, 165.0f, 0.0f);	// �傫��

		const int	WAIT_FRAME	= 24;		// �^�C�g���ҋ@�t���[��
		const float	INIT_SCALE	= 0.025f;	// �^�C�g�������g�嗦
		const float	ADD_SCALE	= 0.09f;	// �^�C�g�����Z�g�嗦
		const float	SET_SCALE	= 1.0f;		// �^�C�g���ݒ�g�嗦
	}

	// �v���C���[�i���o�[��{���
	namespace number
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(160.0f, 350.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3	SIZE_TITLE	= D3DXVECTOR3(242.0f, 107.0f, 0.0f);	// �^�C�g���傫��
		const D3DXVECTOR3	SIZE_VALUE	= D3DXVECTOR3(80.0f, 90.0f, 0.0f);		// �����傫��
		const D3DXVECTOR3	SPACE_POS	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// ����UI���m�̋�
		const D3DXVECTOR3	SPACE_TITLE	= D3DXVECTOR3(100.0f, 5.0f, 0.0f);		// �^�C�g����
		const D3DXVECTOR3	SPACE_VALUE	= VEC3_ZERO;							// ������
		const D3DXCOLOR		COL_NOTJOIN = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);	// ��Q�����̐F
		const int			DIGIT		= 1;									// ����
	}

	// �v���C���[�����|�C���g��{���
	namespace playerPoint
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(160.0f, 460.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3	SIZE_TITLE	= D3DXVECTOR3(242.0f, 107.0f, 0.0f);	// �^�C�g���傫��
		const D3DXVECTOR3	SIZE_VALUE	= D3DXVECTOR3(80.0f, 90.0f, 0.0f);		// �����傫��
		const D3DXVECTOR3	SPACE_POS	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// ����UI���m�̋�
		const D3DXVECTOR3	SPACE_TITLE	= D3DXVECTOR3(-40.0f, 100.0f, 0.0f);	// �^�C�g����
		const D3DXVECTOR3	SPACE_VALUE	= D3DXVECTOR3(80.0f, 0.0f, 0.0f);		// ������
		const int			DIGIT		= 2;									// ����

		const int	WAIT_FRAME	= 8;		// �v���C���[�����|�C���g�ҋ@�t���[��
		const float	INIT_SCALE	= 0.025f;	// �v���C���[�����|�C���g�����g�嗦
		const float	ADD_SCALE	= 0.15f;	// �v���C���[�����|�C���g���Z�g�嗦
		const float	SET_SCALE	= 1.0f;		// �v���C���[�����|�C���g�ݒ�g�嗦
	}

	// �v���C���[�t���[����{���
	namespace frame
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(160.0f, 460.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(340.0f, 340.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// ��

		const int	WAIT_FRAME	= 24;		// �v���C���[�t���[���ҋ@�t���[��
		const float	INIT_SCALE	= 0.025f;	// �v���C���[�t���[�������g�嗦
		const float	ADD_SCALE	= 0.09f;	// �v���C���[�t���[�����Z�g�嗦
		const float	SET_SCALE	= 1.0f;		// �v���C���[�t���[���ݒ�g�嗦
	}

	// �����{���
	namespace control
	{
		const float	ADD_ALPHA		= 0.0045f;	// �����x�̉��Z��
		const float	ADD_SINROT		= 0.04f;	// �����x�ӂ�ӂ킳����ۂ̃T�C���J�[�u�������Z��
		const float	MAX_ADD_ALPHA	= 0.45f;	// �����x�̍ő���Z��
		const float	BASIC_ALPHA		= 0.85f;	// ��̓����x

		const D3DXVECTOR3	POS		= D3DXVECTOR3(1080.0f, 660.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(370.0f, 90.0f, 0.0f);		// �傫��
		const D3DXCOLOR		MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);	// �F
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CMiddleResultManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\middleResult000.png",	// �t�F�[�h�e�N�X�`��
	"data\\TEXTURE\\Mid_result.png",	// �����L���O�^�C�g���e�N�X�`��
	"data\\TEXTURE\\white_flame.png",				// �����|�C���g�w�i�e�N�X�`��
	"data\\TEXTURE\\target_score.png",	// �����|�C���g�^�C�g���e�N�X�`��
	"data\\TEXTURE\\entry_player.png",		// �v���C���[�e�N�X�`��
	"data\\TEXTURE\\Score.png",	// �����|�C���g�e�N�X�`��
	"data\\TEXTURE\\entry_flame.png",		// �t���[���e�N�X�`��
	"data\\TEXTURE\\Press_Start_Crack.png",	// ������@�e�N�X�`��
};

//************************************************************
//	�e�N���X [CMiddleResultManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMiddleResultManager::CMiddleResultManager()
{
	// �����o�ϐ����N���A
	memset(&m_apNumber[0], 0, sizeof(m_apNumber));					// �v���C���[�i���o�[�̏��
	memset(&m_apPlayerWinPoint[0], 0, sizeof(m_apPlayerWinPoint));	// �v���C���[�����|�C���g�̏��
	memset(&m_apFrame[0], 0, sizeof(m_apFrame));					// �v���C���[�t���[���̏��

	m_pControl		= NULL;	// ����̏��
	m_pFade			= NULL;	// �t�F�[�h�̏��
	m_pTitle		= NULL;	// �^�C�g���̏��
	m_pWinPointBG	= NULL;	// �����|�C���g�̔w�i���
	m_pWinPoint		= NULL;	// �����|�C���g�̏��

	m_state		= STATE_FADEIN;	// ���
	m_fMoveY	= 0.0f;			// �c�ړ���
	m_fScale	= 0.0f;			// �g�嗦
	m_fSinAlpha	= 0.0f;			// ��������
	m_nCounterState = 0;		// ��ԊǗ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CMiddleResultManager::~CMiddleResultManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMiddleResultManager::Init(void)
{
	//--------------------------------------------------------
	//	�����o�ϐ��̏�����
	//--------------------------------------------------------
	memset(&m_apNumber[0], 0, sizeof(m_apNumber));					// �v���C���[�i���o�[�̏��
	memset(&m_apPlayerWinPoint[0], 0, sizeof(m_apPlayerWinPoint));	// �v���C���[�����|�C���g�̏��
	memset(&m_apFrame[0], 0, sizeof(m_apFrame));					// �v���C���[�t���[���̏��

	m_pControl		= NULL;	// ����̏��
	m_pFade			= NULL;	// �t�F�[�h�̏��
	m_pTitle		= NULL;	// �^�C�g���̏��
	m_pWinPointBG	= NULL;	// �����|�C���g�̔w�i���
	m_pWinPoint		= NULL;	// �����|�C���g�̏��

	m_state		= STATE_FADEIN;	// ���
	m_fMoveY	= 0.0f;			// �c�ړ���
	m_fScale	= 1.0f;			// �g�嗦
	m_fSinAlpha	= -HALF_PI;		// ��������
	m_nCounterState = 0;		// ��ԊǗ��J�E���^�[

	//--------------------------------------------------------
	//	�t�F�[�h�̐���
	//--------------------------------------------------------
	m_pFade = CObject2D::Create
	( // ����
		fade::UP_POS,	// �ʒu
		fade::SIZE,		// �傫��
		VEC3_ZERO,		// ����
		fade::COL		// �F
	);
	if (m_pFade == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pFade->BindTexture(mc_apTextureFile[TEXTURE_FADE]);

	// �D�揇�ʂ�ݒ�
	m_pFade->SetPriority(PRIORITY);

	//--------------------------------------------------------
	//	�^�C�g���̐���
	//--------------------------------------------------------
	m_pTitle = CObject2D::Create
	( // ����
		title::POS,						// �ʒu
		title::SIZE * title::INIT_SCALE	// �傫��
	);
	if (m_pTitle == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pTitle->BindTexture(mc_apTextureFile[TEXTURE_TITLE]);

	// �D�揇�ʂ�ݒ�
	m_pTitle->SetPriority(PRIORITY);

	// �����`���OFF�ɂ���
	m_pTitle->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�����|�C���g�w�i�̐���
	//--------------------------------------------------------
	m_pWinPointBG = CObject2D::Create
	( // ����
		winBG::POS,						// �ʒu
		winBG::SIZE * winBG::INIT_SCALE	// �傫��
	);
	if (m_pWinPointBG == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pWinPointBG->BindTexture(mc_apTextureFile[TEXTURE_WIN_BG]);

	// �D�揇�ʂ�ݒ�
	m_pWinPointBG->SetPriority(PRIORITY);

	// �����`���OFF�ɂ���
	m_pWinPointBG->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�����|�C���g�̐���
	//--------------------------------------------------------
	m_pWinPoint = CValueUI::Create
	( // ����
		mc_apTextureFile[TEXTURE_WIN],	// �^�C�g���e�N�X�`���p�X
		CValue::TEXTURE_UI,	// �����e�N�X�`��
		win::DIGIT,			// ����
		win::POS,			// �ʒu
		win::SPACE,			// �s��
		win::SPACE_VALUE,	// �����s��
		win::SIZE_TITLE,	// �^�C�g���傫��
		win::SIZE_VALUE		// �����傫��
	);
	if (m_pWinPoint == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pWinPoint->SetPriority(PRIORITY);

	// �����`���OFF�ɂ���
	m_pWinPoint->SetEnableDraw(false);

	// ������ݒ�
	m_pWinPoint->GetMultiValue()->SetNum(CManager::GetInstance()->GetRetentionManager()->GetWinPoint());

	//--------------------------------------------------------
	//	�v���C���[�t���[���E�����|�C���g�E�i���o�[�̐���
	//--------------------------------------------------------
	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		// �|�C���g��錾
		CPlayer *pPlayer = CScene::GetPlayer(nCntEntry);	// �v���C���[���

		// �ϐ���錾
		int nPlayerWinPoint = (pPlayer == NULL) ? 0 : pPlayer->GetWinPoint();			// �v���C���[�����|�C���g
		D3DXCOLOR colPolygon = (pPlayer == NULL) ? number::COL_NOTJOIN : XCOL_WHITE;	// �|���S���F

		// �v���C���[�t���[���̐���
		m_apFrame[nCntEntry] = CObject2D::Create
		( // ����
			frame::POS + (frame::SPACE * (float)nCntEntry),	// �ʒu
			frame::SIZE * frame::INIT_SCALE,				// �傫��
			VEC3_ZERO,		// ����
			colPolygon		// �F
		);
		if (m_apFrame[nCntEntry] == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_apFrame[nCntEntry]->BindTexture(mc_apTextureFile[TEXTURE_FRAME]);

		// �D�揇�ʂ�ݒ�
		m_apFrame[nCntEntry]->SetPriority(PRIORITY);

		// �����`���OFF�ɂ���
		m_apFrame[nCntEntry]->SetEnableDraw(false);

		// �v���C���[�����|�C���g�̐���
		m_apPlayerWinPoint[nCntEntry] = CValueUI::Create
		( // ����
			mc_apTextureFile[TEXTURE_PLAYER_POINT],							// �^�C�g���e�N�X�`���p�X
			CValue::TEXTURE_UI,												// �����e�N�X�`��
			playerPoint::DIGIT,												// ����
			playerPoint::POS + (playerPoint::SPACE_POS * (float)nCntEntry),	// �ʒu
			playerPoint::SPACE_TITLE,							// �s��
			playerPoint::SPACE_VALUE,							// �����s��
			playerPoint::SIZE_TITLE * playerPoint::INIT_SCALE,	// �^�C�g���傫��
			playerPoint::SIZE_VALUE * playerPoint::INIT_SCALE,	// �����傫��
			VEC3_ZERO,	// �^�C�g������
			VEC3_ZERO,	// ��������
			colPolygon,	// �^�C�g���F
			colPolygon	// �����F
		);
		if (m_apPlayerWinPoint[nCntEntry] == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apPlayerWinPoint[nCntEntry]->SetPriority(PRIORITY);

		// �����`���OFF�ɂ���
		m_apPlayerWinPoint[nCntEntry]->SetEnableDraw(false);

		// ������ݒ�
		m_apPlayerWinPoint[nCntEntry]->GetMultiValue()->SetNum(nPlayerWinPoint);

		// �v���C���[�i���o�[�̐���
		m_apNumber[nCntEntry] = CValueUI::Create
		( // ����
			mc_apTextureFile[TEXTURE_PLAYER],						// �^�C�g���e�N�X�`���p�X
			CValue::TEXTURE_NORMAL,									// �����e�N�X�`��
			number::DIGIT,											// ����
			number::POS + (number::SPACE_POS * (float)nCntEntry),	// �ʒu
			number::SPACE_TITLE,							// �s��
			number::SPACE_VALUE,							// �����s��
			number::SIZE_TITLE * playerPoint::INIT_SCALE,	// �^�C�g���傫��
			number::SIZE_VALUE * playerPoint::INIT_SCALE,	// �����傫��
			VEC3_ZERO,	// �^�C�g������
			VEC3_ZERO,	// ��������
			colPolygon,	// �^�C�g���F
			colPolygon	// �����F
		);
		if (m_apNumber[nCntEntry] == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apNumber[nCntEntry]->SetPriority(PRIORITY);

		// �����`���OFF�ɂ���
		m_apNumber[nCntEntry]->SetEnableDraw(false);

		// ������ݒ�
		m_apNumber[nCntEntry]->GetMultiValue()->SetNum(nCntEntry + 1);
	}

	//--------------------------------------------------------
	//	����̐���
	//--------------------------------------------------------
	m_pControl = CObject2D::Create
	( // ����
		control::POS,	// �ʒu
		control::SIZE,	// �傫��
		VEC3_ZERO,		// ����
		XCOL_AWHITE		// �F
	);
	if (m_pControl == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pControl->BindTexture(mc_apTextureFile[TEXTURE_CONTROL]);

	// �D�揇�ʂ�ݒ�
	m_pControl->SetPriority(PRIORITY);

	// �����`���OFF�ɂ���
	m_pControl->SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CMiddleResultManager::Uninit(void)
{
	// �t�F�[�h�̏I��
	m_pFade->Uninit();

	// �^�C�g���̏I��
	m_pTitle->Uninit();

	// �����|�C���g�w�i�̏I��
	m_pWinPointBG->Uninit();

	// �����|�C���g�̏I��
	m_pWinPoint->Uninit();

	// ����̏I��
	m_pControl->Uninit();

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		// �v���C���[�i���o�[�̏I��
		m_apNumber[nCntEntry]->Uninit();

		// �v���C���[�����|�C���g�̏I��
		m_apPlayerWinPoint[nCntEntry]->Uninit();

		// �v���C���[�t���[���̏I��
		m_apFrame[nCntEntry]->Uninit();
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CMiddleResultManager::Update(void)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_FADEIN:

		// �t�F�[�h�C���̍X�V
		UpdateFadeIn();

		break;

	case STATE_FADEIN_WAIT:

		// �t�F�[�h�C���ҋ@�̍X�V
		UpdateFadeInWait();

		break;

	case STATE_FADEIN_ACCEL:

		// �t�F�[�h�C�������̍X�V
		UpdateFadeInAccel();

		break;

	case STATE_RANK_TITLE_WAIT:

		// �����L���O�^�C�g���ҋ@�̍X�V
		UpdateRankTitleWait();

		break;

	case STATE_RANK_TITLE:

		// �����L���O�^�C�g���\���̍X�V
		UpdateRankTitle();

		break;

	case STATE_WINPOINT_WAIT:

		// �����|�C���g�ҋ@�̍X�V
		UpdateWinPointWait();

		break;

	case STATE_WINPOINT:

		// �����|�C���g�\���̍X�V
		UpdateWinPoint();

		break;

	case STATE_FRAME_WAIT:

		// �v���C���[�t���[���ҋ@�̍X�V
		UpdateFrameWait();

		break;

	case STATE_FRAME:

		// �v���C���[�t���[���\���̍X�V
		UpdateFrame();

		break;

	case STATE_PLAYERPOINT_WAIT:

		// �v���C���[�����|�C���g�ҋ@�̍X�V
		UpdatePlayerPointWait();

		break;

	case STATE_PLAYERPOINT:

		// �v���C���[�����|�C���g�\���̍X�V
		UpdatePlayerPoint();

		break;

	case STATE_WAIT:

		// �ҋ@�̍X�V
		UpdateWait();

		break;

	case STATE_FADEOUT:

		// �t�F�[�h�A�E�g�̍X�V
		UpdateFadeOut();

		break;

	case STATE_FADEOUT_WAIT:

		// �t�F�[�h�A�E�g�ҋ@�̍X�V
		UpdateFadeOutWait();

		break;

	case STATE_FADEOUT_ACCEL:

		// �t�F�[�h�A�E�g�����̍X�V
		UpdateFadeOutAccel();

		break;

	case STATE_END:

		if (CManager::GetInstance()->GetFade()->GetState() == CFade::FADE_NONE)
		{ // �t�F�[�h���ł͂Ȃ��ꍇ

			//if ()
			{ // ���҂����܂��Ă��Ȃ��ꍇ

				// �Q�[���ɑJ��
				CManager::GetInstance()->SetScene(CScene::MODE_GAME, WAIT_FRAME);
			}
			//else
			//{ // ���҂����܂����ꍇ

			//	// ���U���g�ɑJ��
			//	CManager::GetInstance()->SetScene(CScene::MODE_RESULT, WAIT_FRAME);
			//}
		}

		break;

	default:
		assert(false);
		break;
	}

	// �J�ڌ���̍X�V
	UpdateTransition();

	// �t�F�[�h�̍X�V
	m_pFade->Update();

	// �^�C�g���̍X�V
	m_pTitle->Update();

	// �����|�C���g�w�i�̍X�V
	m_pWinPointBG->Update();

	// �����|�C���g�̍X�V
	m_pWinPoint->Update();

	// ����̍X�V
	m_pControl->Update();

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		// �v���C���[�i���o�[�̍X�V
		m_apNumber[nCntEntry]->Update();

		// �v���C���[�����|�C���g�̍X�V
		m_apPlayerWinPoint[nCntEntry]->Update();

		// �v���C���[�t���[���̍X�V
		m_apFrame[nCntEntry]->Update();
	}
}

//============================================================
//	��������
//============================================================
CMiddleResultManager *CMiddleResultManager::Create(void)
{
	// �|�C���^��錾
	CMiddleResultManager *pMiddleResultManager = NULL;	// ���ԃ��U���g�}�l�[�W���[�����p

	if (pMiddleResultManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pMiddleResultManager = new CMiddleResultManager;	// ���ԃ��U���g�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pMiddleResultManager != NULL)
	{ // �g�p����Ă���ꍇ
		
		// ���ԃ��U���g�}�l�[�W���[�̏�����
		if (FAILED(pMiddleResultManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pMiddleResultManager;
			pMiddleResultManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pMiddleResultManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CMiddleResultManager::Release(CMiddleResultManager *&prMiddleResultManager)
{
	if (prMiddleResultManager != NULL)
	{ // �g�p���̏ꍇ

		// ���ԃ��U���g�}�l�[�W���[�̏I��
		if (FAILED(prMiddleResultManager->Uninit()))
		{ // �j���Ɏ��s�����ꍇ

			// �������J��
			delete prMiddleResultManager;
			prMiddleResultManager = NULL;

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �������J��
		delete prMiddleResultManager;
		prMiddleResultManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�t�F�[�h�C���̍X�V����
//============================================================
void CMiddleResultManager::UpdateFadeIn(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// �t�F�[�h�ʒu

	// �ړ��ʂ����Z
	m_fMoveY += fade::ADD_MOVE;

	// �t�F�[�h�̈ʒu�����Z
	posFade.y += m_fMoveY;

	if (posFade.y > fade::UP_MIDDLE_POS.y)
	{ // ���݈ʒu����~�ʒu�𒴂����ꍇ

		// �t�F�[�h���~�ʒu�ɕ␳
		posFade.y = fade::UP_MIDDLE_POS.y;

		// �ړ��ʂ�������
		m_fMoveY = 0.0f;

		// �t�F�[�h�C���ҋ@��Ԃɂ���
		m_state = STATE_FADEIN_WAIT;
	}

	// �t�F�[�h�ʒu�𔽉f
	m_pFade->SetVec3Position(posFade);
}

//============================================================
//	�t�F�[�h�C���ҋ@�̍X�V����
//============================================================
void CMiddleResultManager::UpdateFadeInWait(void)
{
	// �J�E���^�[�����Z
	m_nCounterState++;

	if (m_nCounterState >= fade::WAIT_FRAME)
	{ // �ҋ@���I�������ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �t�F�[�h�C��������Ԃɂ���
		m_state = STATE_FADEIN_ACCEL;
	}
}

//============================================================
//	�t�F�[�h�C�������̍X�V����
//============================================================
void CMiddleResultManager::UpdateFadeInAccel(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// �t�F�[�h�ʒu

	// �ړ��ʂ����Z
	m_fMoveY += fade::ADD_ACCEL_MOVE;

	// �t�F�[�h�̈ʒu�����Z
	posFade.y += m_fMoveY;

	if (posFade.y > fade::CENT_POS.y)
	{ // ���݈ʒu����~�ʒu�𒴂����ꍇ

		// �t�F�[�h���~�ʒu�ɕ␳
		posFade.y = fade::CENT_POS.y;

		// �ړ��ʂ�������
		m_fMoveY = 0.0f;

		// �����L���O�^�C�g���ҋ@��Ԃɂ���
		m_state = STATE_RANK_TITLE_WAIT;
	}

	// �t�F�[�h�ʒu�𔽉f
	m_pFade->SetVec3Position(posFade);
}

//============================================================
//	�����L���O�^�C�g���ҋ@�̍X�V����
//============================================================
void CMiddleResultManager::UpdateRankTitleWait(void)
{
	// �J�E���^�[�����Z
	m_nCounterState++;

	if (m_nCounterState >= title::WAIT_FRAME)
	{ // �ҋ@���I�������ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �����L���O�^�C�g���\����Ԃɂ���
		m_state = STATE_RANK_TITLE;

		// �����L���O�^�C�g���̎����`���ON�ɂ���
		m_pTitle->SetEnableDraw(true);

		// �����L���O�^�C�g���̊g�嗦��ݒ�
		m_fScale = title::INIT_SCALE;
	}
}

//============================================================
//	�����L���O�^�C�g���\���̍X�V����
//============================================================
void CMiddleResultManager::UpdateRankTitle(void)
{
	// �g�嗦�����Z
	m_fScale += title::ADD_SCALE;

	if (m_fScale < title::SET_SCALE)
	{ // �܂��傫���Ȃ�ꍇ

		// �����L���O�^�C�g���傫���𔽉f
		m_pTitle->SetVec3Sizing(title::SIZE * m_fScale);
	}
	else
	{ // �傫���Ȃ�؂����ꍇ

		// �����L���O�^�C�g���傫���𔽉f
		m_pTitle->SetVec3Sizing(title::SIZE);

		// �g�嗦��������
		m_fScale = 1.0f;

		// �����|�C���g�ҋ@��Ԃɂ���
		m_state = STATE_WINPOINT_WAIT;
	}
}

//============================================================
//	�����|�C���g�ҋ@�̍X�V����
//============================================================
void CMiddleResultManager::UpdateWinPointWait(void)
{
	// �J�E���^�[�����Z
	m_nCounterState++;

	if (m_nCounterState >= winBG::WAIT_FRAME)
	{ // �ҋ@���I�������ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �����|�C���g�\����Ԃɂ���
		m_state = STATE_WINPOINT;

		// �����|�C���g�w�i�̎����`���ON�ɂ���
		m_pWinPointBG->SetEnableDraw(true);

		// �����|�C���g�w�i�̊g�嗦��ݒ�
		m_fScale = winBG::INIT_SCALE;
	}
}

//============================================================
//	�����|�C���g�\���̍X�V����
//============================================================
void CMiddleResultManager::UpdateWinPoint(void)
{
	// �g�嗦�����Z
	m_fScale += winBG::ADD_SCALE;

	if (m_fScale < winBG::SET_SCALE)
	{ // �܂��傫���Ȃ�ꍇ

		// �����|�C���g�w�i�̑傫���𔽉f
		m_pWinPointBG->SetVec3Sizing(winBG::SIZE * m_fScale);
	}
	else
	{ // �傫���Ȃ�؂����ꍇ

		// �����|�C���g�w�i�̑傫���𔽉f
		m_pWinPointBG->SetVec3Sizing(winBG::SIZE);

		// �g�嗦��������
		m_fScale = 1.0f;

		// �����|�C���g�\���̎����`���ON�ɂ���
		m_pWinPoint->SetEnableDraw(true);

		// �v���C���[�t���[���ҋ@��Ԃɂ���
		m_state = STATE_FRAME_WAIT;
	}
}

//============================================================
//	�v���C���[�t���[���ҋ@�̍X�V����
//============================================================
void CMiddleResultManager::UpdateFrameWait(void)
{
	// �J�E���^�[�����Z
	m_nCounterState++;

	if (m_nCounterState >= frame::WAIT_FRAME)
	{ // �ҋ@���I�������ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �v���C���[�t���[���\����Ԃɂ���
		m_state = STATE_FRAME;

		// �v���C���[�t���[���̊g�嗦��ݒ�
		m_fScale = frame::INIT_SCALE;

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // �v���C���[�̍ő吔���J��Ԃ�

			// �v���C���[�t���[���̎����`���ON�ɂ���
			m_apFrame[nCntEntry]->SetEnableDraw(true);
		}
	}
}

//============================================================
//	�v���C���[�t���[���\���̍X�V����
//============================================================
void CMiddleResultManager::UpdateFrame(void)
{
	// �g�嗦�����Z
	m_fScale += frame::ADD_SCALE;

	if (m_fScale < frame::SET_SCALE)
	{ // �܂��傫���Ȃ�ꍇ

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // �v���C���[�̍ő吔���J��Ԃ�

			// �v���C���[�t���[���̑傫���𔽉f
			m_apFrame[nCntEntry]->SetVec3Sizing(frame::SIZE * m_fScale);
		}
	}
	else
	{ // �傫���Ȃ�؂����ꍇ

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // �v���C���[�̍ő吔���J��Ԃ�

			// �v���C���[�t���[���̑傫���𔽉f
			m_apFrame[nCntEntry]->SetVec3Sizing(frame::SIZE);
		}

		// �g�嗦��������
		m_fScale = 1.0f;

		// �v���C���[�����|�C���g�ҋ@��Ԃɂ���
		m_state = STATE_PLAYERPOINT_WAIT;
	}
}

//============================================================
//	�v���C���[�����|�C���g�ҋ@�̍X�V����
//============================================================
void CMiddleResultManager::UpdatePlayerPointWait(void)
{
	// �J�E���^�[�����Z
	m_nCounterState++;

	if (m_nCounterState >= playerPoint::WAIT_FRAME)
	{ // �ҋ@���I�������ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �v���C���[�����|�C���g�\����Ԃɂ���
		m_state = STATE_PLAYERPOINT;

		// �v���C���[�����|�C���g�̊g�嗦��ݒ�
		m_fScale = playerPoint::INIT_SCALE;

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // �v���C���[�̍ő吔���J��Ԃ�

			// �v���C���[�����|�C���g�̎����`���ON�ɂ���
			m_apPlayerWinPoint[nCntEntry]->SetEnableDraw(true);

			// �v���C���[�i���o�[�̎����`���ON�ɂ���
			m_apNumber[nCntEntry]->SetEnableDraw(true);
		}
	}
}

//============================================================
//	�v���C���[�����|�C���g�\���̍X�V����
//============================================================
void CMiddleResultManager::UpdatePlayerPoint(void)
{
	// �g�嗦�����Z
	m_fScale += playerPoint::ADD_SCALE;

	if (m_fScale < playerPoint::SET_SCALE)
	{ // �܂��傫���Ȃ�ꍇ

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // �v���C���[�̍ő吔���J��Ԃ�

			// �v���C���[�i���o�[�E�����|�C���g�̑傫���𔽉f
			m_apNumber[nCntEntry]->SetScalingTitle(number::SIZE_TITLE * m_fScale);
			m_apNumber[nCntEntry]->GetMultiValue()->SetVec3Sizing(number::SIZE_VALUE * m_fScale);
			m_apPlayerWinPoint[nCntEntry]->SetScalingTitle(playerPoint::SIZE_TITLE * m_fScale);
			m_apPlayerWinPoint[nCntEntry]->GetMultiValue()->SetVec3Sizing(playerPoint::SIZE_VALUE * m_fScale);
		}
	}
	else
	{ // �傫���Ȃ�؂����ꍇ

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // �v���C���[�̍ő吔���J��Ԃ�

			// �v���C���[�i���o�[�E�����|�C���g�̑傫���𔽉f
			m_apNumber[nCntEntry]->SetScalingTitle(number::SIZE_TITLE);
			m_apNumber[nCntEntry]->GetMultiValue()->SetVec3Sizing(number::SIZE_VALUE);
			m_apPlayerWinPoint[nCntEntry]->SetScalingTitle(playerPoint::SIZE_TITLE);
			m_apPlayerWinPoint[nCntEntry]->GetMultiValue()->SetVec3Sizing(playerPoint::SIZE_VALUE);
		}

		// �g�嗦��������
		m_fScale = 1.0f;

		// ����̎����`���ON�ɂ���
		m_pControl->SetEnableDraw(true);

		// �ҋ@��Ԃɂ���
		m_state = STATE_WAIT;
	}
}

//============================================================
//	�ҋ@�̍X�V����
//============================================================
void CMiddleResultManager::UpdateWait(void)
{
	// �ϐ���錾
	D3DXCOLOR colControl = m_pControl->GetColor();	// ����\���F

	if (colControl.a < control::MIN_COL.a)
	{ // �����x���Œ�����Ⴂ�ꍇ

		// �����x�����Z
		colControl.a += control::ADD_ALPHA;

		if (colControl.a > control::MIN_COL.a)
		{ // �����x�����߂����ꍇ

			// �����x��␳
			colControl.a = control::MIN_COL.a;
		}

		// ����\���F��ݒ�
		m_pControl->SetColor(colControl);
	}
	else
	{ // �����x���Œ���ȏ�̏ꍇ

		// �ϐ���錾
		float fAddAlpha = 0.0f;	// �����x�̉��Z��

		// �����x���グ��
		m_fSinAlpha += control::ADD_SINROT;
		useful::NormalizeRot(m_fSinAlpha);	// �������K��

		// �����x���Z�ʂ����߂�
		fAddAlpha = (control::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinAlpha) - 1.0f);

		// ����\���F��ݒ�
		m_pControl->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, control::BASIC_ALPHA + fAddAlpha));
	}
}

//============================================================
//	�t�F�[�h�A�E�g�̍X�V����
//============================================================
void CMiddleResultManager::UpdateFadeOut(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// �t�F�[�h�ʒu

	// �ړ��ʂ����Z
	m_fMoveY += fade::ADD_MOVE;

	// �t�F�[�h�̈ʒu�����Z
	posFade.y += m_fMoveY;

	if (posFade.y > fade::DOWN_MIDDLE_POS.y)
	{ // ���݈ʒu����~�ʒu�𒴂����ꍇ

		// �S�ʒu�̈ړ�
		MovePositionAll(D3DXVECTOR3(0.0f, posFade.y - fade::DOWN_POS.y, 0.0f));

		// �t�F�[�h���~�ʒu�ɕ␳
		posFade.y = fade::DOWN_MIDDLE_POS.y;

		// �ړ��ʂ�������
		m_fMoveY = 0.0f;

		// �t�F�[�h�A�E�g�ҋ@��Ԃɂ���
		m_state = STATE_FADEOUT_WAIT;
	}
	else
	{ // ���݈ʒu����~�ʒu�𒴂��Ă��Ȃ��ꍇ

		// �S�ʒu�̈ړ�
		MovePositionAll(D3DXVECTOR3(0.0f, m_fMoveY, 0.0f));
	}

	// �t�F�[�h�ʒu�𔽉f
	m_pFade->SetVec3Position(posFade);
}

//============================================================
//	�t�F�[�h�A�E�g�ҋ@�̍X�V����
//============================================================
void CMiddleResultManager::UpdateFadeOutWait(void)
{
	// �J�E���^�[�����Z
	m_nCounterState++;

	if (m_nCounterState >= fade::WAIT_FRAME)
	{ // �ҋ@���I�������ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �t�F�[�h�A�E�g������Ԃɂ���
		m_state = STATE_FADEOUT_ACCEL;
	}
}

//============================================================
//	�t�F�[�h�A�E�g�����̍X�V����
//============================================================
void CMiddleResultManager::UpdateFadeOutAccel(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// �t�F�[�h�ʒu

	// �ړ��ʂ����Z
	m_fMoveY += fade::ADD_ACCEL_MOVE;

	// �t�F�[�h�̈ʒu�����Z
	posFade.y += m_fMoveY;

	if (posFade.y > fade::DOWN_POS.y)
	{ // ���݈ʒu����~�ʒu�𒴂����ꍇ

		// �S�ʒu�̈ړ�
		MovePositionAll(D3DXVECTOR3(0.0f, posFade.y - fade::DOWN_POS.y, 0.0f));

		// �t�F�[�h���~�ʒu�ɕ␳
		posFade.y = fade::DOWN_POS.y;

		// �ړ��ʂ�������
		m_fMoveY = 0.0f;

		// �I����Ԃɂ���
		m_state = STATE_END;
	}
	else
	{ // ���݈ʒu����~�ʒu�𒴂��Ă��Ȃ��ꍇ

		// �S�ʒu�̈ړ�
		MovePositionAll(D3DXVECTOR3(0.0f, m_fMoveY, 0.0f));
	}

	// �t�F�[�h�ʒu�𔽉f
	m_pFade->SetVec3Position(posFade);
}

//============================================================
//	�J�ڌ���̍X�V����
//============================================================
void CMiddleResultManager::UpdateTransition(void)
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

			if (m_state != STATE_FADEOUT		// �t�F�[�h�A�E�g���
			&&  m_state != STATE_FADEOUT_WAIT	// �t�F�[�h�A�E�g�ҋ@���
			&&  m_state != STATE_FADEOUT_ACCEL	// �t�F�[�h�A�E�g�������
			&&  m_state != STATE_END)			// �I�����
			{ // ��L�̏�Ԃł͂Ȃ��ꍇ

				// ���o�X�L�b�v
				SkipStaging();

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
			}
		}
		else
		{ // �J�ڑҋ@��Ԃ̏ꍇ

			// ����̎����`���OFF�ɂ���
			m_pControl->SetEnableDraw(false);

			// �t�F�[�h�A�E�g��Ԃɂ���
			m_state = STATE_FADEOUT;
		}
	}
}

//============================================================
//	���o�X�L�b�v����
//============================================================
void CMiddleResultManager::SkipStaging(void)
{
	// ����������
	m_fMoveY = 0.0f;	// �ړ���
	m_fScale = 1.0f;	// �g�嗦

	// �����`���ON�ɂ���
	m_pTitle->SetEnableDraw(true);		// �����L���O�^�C�g��
	m_pWinPointBG->SetEnableDraw(true);	// �����|�C���g�w�i
	m_pWinPoint->SetEnableDraw(true);	// �����|�C���g�\��
	m_pControl->SetEnableDraw(true);	// ����

	// �t�F�[�h�ʒu�𔽉f
	m_pFade->SetVec3Position(D3DXVECTOR3(fade::CENT_POS.x, fade::CENT_POS.y, 0.0f));

	// �����L���O�^�C�g���傫���𔽉f
	m_pTitle->SetVec3Sizing(title::SIZE);

	// �����|�C���g�w�i�̑傫���𔽉f
	m_pWinPointBG->SetVec3Sizing(winBG::SIZE);

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		// �����`���ON�ɂ���
		m_apFrame[nCntEntry]->SetEnableDraw(true);			// �v���C���[�t���[��
		m_apPlayerWinPoint[nCntEntry]->SetEnableDraw(true);	// �v���C���[�����|�C���g
		m_apNumber[nCntEntry]->SetEnableDraw(true);			// �v���C���[�i���o�[

		// �v���C���[�t���[���̑傫���𔽉f
		m_apFrame[nCntEntry]->SetVec3Sizing(frame::SIZE);

		// �v���C���[�i���o�[�E�����|�C���g�̑傫���𔽉f
		m_apNumber[nCntEntry]->SetScalingTitle(number::SIZE_TITLE);
		m_apNumber[nCntEntry]->GetMultiValue()->SetVec3Sizing(number::SIZE_VALUE);
		m_apPlayerWinPoint[nCntEntry]->SetScalingTitle(playerPoint::SIZE_TITLE);
		m_apPlayerWinPoint[nCntEntry]->GetMultiValue()->SetVec3Sizing(playerPoint::SIZE_VALUE);
	}

	// ��Ԃ�ύX
	m_state = STATE_WAIT;	// �J�ڑҋ@���
}

//============================================================
//	�S�ʒu�̈ړ�����
//============================================================
void CMiddleResultManager::MovePositionAll(const D3DXVECTOR3 & rAddMove)
{
	// �t�F�[�h�̈ʒu�̈ړ�
	m_pFade->SetVec3Position(m_pFade->GetVec3Position() + rAddMove);

	// �^�C�g���̈ʒu�̈ړ�
	m_pTitle->SetVec3Position(m_pTitle->GetVec3Position() + rAddMove);

	// �����|�C���g�w�i�̈ʒu�̈ړ�
	m_pWinPointBG->SetVec3Position(m_pWinPointBG->GetVec3Position() + rAddMove);

	// �����|�C���g�̈ʒu�̈ړ�
	m_pWinPoint->SetVec3Position(m_pWinPoint->GetVec3Position() + rAddMove);

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		// �v���C���[�i���o�[�̈ʒu�̈ړ�
		m_apNumber[nCntEntry]->SetVec3Position(m_apNumber[nCntEntry]->GetVec3Position() + rAddMove);

		// �v���C���[�����|�C���g�̈ʒu�̈ړ�
		m_apPlayerWinPoint[nCntEntry]->SetVec3Position(m_apPlayerWinPoint[nCntEntry]->GetVec3Position() + rAddMove);

		// �v���C���[�t���[���̈ʒu�̈ړ�
		m_apFrame[nCntEntry]->SetVec3Position(m_apFrame[nCntEntry]->GetVec3Position() + rAddMove);
	}
}
