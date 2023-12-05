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

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 14;	// ���ԃ��U���g�̗D�揇��

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
		const float	ADD_MOVE	= 0.4f;		// �t�F�[�h�ړ���
		const float	ADD_ACCEL_MOVE = 1.75f;	// �t�F�[�h�����ړ���
	}

	// �����L���O�^�C�g����{���
	namespace title
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(280.0f, 100.0f, 0.0f);	// �^�C�g���ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(400.0f, 130.0f, 0.0f);	// �^�C�g���傫��

		const int	WAIT_FRAME	= 24;		// �^�C�g���ҋ@�t���[��
		const float	INIT_SCALE	= 0.025f;	// �^�C�g�������g�嗦
		const float	ADD_SCALE	= 0.09f;	// �^�C�g�����Z�g�嗦
		const float	SET_SCALE	= 1.0f;		// �^�C�g���ݒ�g�嗦
	}

	// �����|�C���g��{���
	namespace win
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(760.0f, 100.0f, 0.0f);			// �ʒu
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(230.0f, 0.0f, 0.0f);				// �s��
		const D3DXVECTOR3 SPACE_VALUE	= D3DXVECTOR3(80.0f, 0.0f, 0.0f);		// �����s��
		const D3DXVECTOR3 SIZE_TITLE	= D3DXVECTOR3(300.0f, 100.0f, 0.0f);	// �^�C�g���傫��
		const D3DXVECTOR3 SIZE_VALUE	= D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// �����傫��

		const float	DIGIT = 2;	// ����
	}

	// �����|�C���g�w�i��{���
	namespace winBG
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(860.0f, 100.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(720.0f, 145.0f, 0.0f);	// �傫��

		const int	WAIT_FRAME	= 24;		// �^�C�g���ҋ@�t���[��
		const float	INIT_SCALE	= 0.025f;	// �^�C�g�������g�嗦
		const float	ADD_SCALE	= 0.09f;	// �^�C�g�����Z�g�嗦
		const float	SET_SCALE	= 1.0f;		// �^�C�g���ݒ�g�嗦
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CMiddleResultManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\middleResult000.png",	// �t�F�[�h�e�N�X�`��
	NULL,	// �����L���O�^�C�g���e�N�X�`��
	"data\\TEXTURE\\bg.png",	// �����|�C���g�w�i�e�N�X�`��
	NULL,	// �����|�C���g�^�C�g���e�N�X�`��
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
	m_pFade			= NULL;	// �t�F�[�h�̏��
	m_pTitle		= NULL;	// �^�C�g���̏��
	m_pWinPointBG	= NULL;	// �����|�C���g�̔w�i���
	m_pWinPoint		= NULL;	// �����|�C���g�̏��

	m_state		= STATE_FADEIN;	// ���
	m_fMoveY	= 0.0f;			// �c�ړ���
	m_fScale	= 0.0f;			// �g�嗦
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
	m_pFade			= NULL;	// �t�F�[�h�̏��
	m_pTitle		= NULL;	// �^�C�g���̏��
	m_pWinPointBG	= NULL;	// �����|�C���g�̔w�i���
	m_pWinPoint		= NULL;	// �����|�C���g�̏��

	m_state		= STATE_FADEIN;	// ���
	m_fMoveY	= 0.0f;			// �c�ړ���
	m_fScale	= 1.0f;			// �g�嗦
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

	case STATE_WAIT:

		//// TODO�F���̏���
		//m_nCounterState++;
		//if (m_nCounterState == 60)
		//{
		//	m_nCounterState = 0;
		//	m_state = STATE_FADEOUT;
		//}

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

		//if ()
		{ // ���҂����܂��Ă��Ȃ��ꍇ

			// �Q�[���ɑJ��
			CManager::GetInstance()->SetScene(CScene::MODE_GAME);
		}
		//else
		//{ // ���҂����܂����ꍇ

		//	// ���U���g�ɑJ��
		//	CManager::GetInstance()->SetScene(CScene::MODE_RESULT);
		//}

		break;

	default:
		assert(false);
		break;
	}

	// �t�F�[�h�̍X�V
	m_pFade->Update();

	// �^�C�g���̍X�V
	m_pTitle->Update();

	// �����|�C���g�w�i�̍X�V
	m_pWinPointBG->Update();

	// �����|�C���g�̍X�V
	m_pWinPoint->Update();
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

		// �ҋ@��Ԃɂ���
		m_state = STATE_WAIT;
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

		// �t�F�[�h���~�ʒu�ɕ␳
		posFade.y = fade::DOWN_MIDDLE_POS.y;

		// �ړ��ʂ�������
		m_fMoveY = 0.0f;

		// �t�F�[�h�A�E�g�ҋ@��Ԃɂ���
		m_state = STATE_FADEOUT_WAIT;
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

		// �t�F�[�h���~�ʒu�ɕ␳
		posFade.y = fade::DOWN_POS.y;

		// �ړ��ʂ�������
		m_fMoveY = 0.0f;

		// �I����Ԃɂ���
		m_state = STATE_END;
	}

	// �t�F�[�h�ʒu�𔽉f
	m_pFade->SetVec3Position(posFade);
}
