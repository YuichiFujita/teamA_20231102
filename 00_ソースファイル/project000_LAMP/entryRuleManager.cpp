//============================================================
//
//	�G���g���[���[���}�l�[�W���[���� [entryRuleManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "entryRuleManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"
#include "multiValue.h"
#include "retentionManager.h"
#include "fade.h"
#include "sceneEntry.h"
#include "entryManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY	= 14;	// �G���g���[�̗D�揇��
	const int	MAX_SELECT	= CEntryRuleManager::RULE_MAX + 1;	// ���[���I���̑��� (����������܂�)
	const int	SELECT_GAME = CEntryRuleManager::RULE_MAX;		// �Q�[���J�ڑI��ԍ�

	// �t�F�[�h���
	namespace fade
	{
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �������F
		const D3DXCOLOR SET_COL		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);	// �ݒ�F

		const float	ADD_ALPHA	= 0.025f;	// �����x�̉��Z��
		const float	SUB_ALPHA	= 0.025f;	// �����x�̌��Z��
	}

	// ���[���^�C�g�����
	namespace rule
	{
		const POSGRID2		PART	= POSGRID2(1, CEntryRuleManager::RULE_MAX);	// �e�N�X�`��������
		const D3DXVECTOR3	POS		= D3DXVECTOR3(400.0f, 120.0f, 0.0f);		// �ʒu
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(300.0f, 100.0f, 0.0f);		// �傫��
		const D3DXVECTOR3	SPACE	= D3DXVECTOR3(0.0f, 140.0f, 0.0f);			// ��
	}

	// �J�n�{�^�����
	namespace start
	{
		const D3DXVECTOR3	POS		= D3DXVECTOR3(SCREEN_CENT.x, 580.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(440.0f, 140.0f, 0.0f);		// �傫��
	}

	// �����|�C���g�����
	namespace winpoint
	{
		const D3DXVECTOR3	POS		= D3DXVECTOR3(830.0f, 120.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// �傫��
		const D3DXVECTOR3	SPACE	= D3DXVECTOR3(100.0f, 0.0f, 0.0f);		// �s��

		const int	DIGIT	= 2;	// ����
	}

	// ���j�������
	namespace kill
	{
		const POSGRID2		PART	= POSGRID2(1, CRetentionManager::KILL_MAX);						// �e�N�X�`��������
		const D3DXVECTOR3	POS		= D3DXVECTOR3(880.0f, winpoint::POS.y + rule::SPACE.y, 0.0f);	// �ʒu
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(300.0f, 100.0f, 0.0f);							// �傫��
	}

	// �����������
	namespace win
	{
		const POSGRID2		PART	= POSGRID2(1, CRetentionManager::WIN_MAX);								// �e�N�X�`��������
		const D3DXVECTOR3	POS		= D3DXVECTOR3(880.0f, winpoint::POS.y + (rule::SPACE.y * 2.0f), 0.0f);	// �ʒu
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(300.0f, 100.0f, 0.0f);									// �傫��
	}

	// �I�����
	namespace select
	{
		const float	ADD_SIZE = 15.0f;	// �傫�����Z��

		const D3DXVECTOR3	POS	= D3DXVECTOR3(880.0f, rule::POS.y, 0.0f);	// �ʒu
		const D3DXCOLOR		COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);		// �F

		const D3DXVECTOR3	SIZE_RULE	= D3DXVECTOR3(kill::SIZE.x + ADD_SIZE, kill::SIZE.y + ADD_SIZE, 0.0f);		// ���[���I�����̑傫��
		const D3DXVECTOR3	SIZE_START	= D3DXVECTOR3(start::SIZE.x + ADD_SIZE, start::SIZE.y + ADD_SIZE, 0.0f);	// �J�n�I�����̑傫��

		const D3DXCOLOR CHOICE_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �I�𒆃J���[
		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);	// ��I�𒆃J���[
	}

	// �����
	namespace arrow
	{
		const float	ADD_ALPHA		= 0.02f;	// �����x�̉��Z��
		const float	ADD_SINROT		= 0.04f;	// �����x�ӂ�ӂ킳����ۂ̃T�C���J�[�u�������Z��
		const float	MAX_ADD_ALPHA	= 0.25f;	// �����x�̍ő���Z��
		const float	BASIC_ALPHA		= 0.95f;	// ��̓����x
		const float	SPACE_EDGE		= 50.0f;	// ���̋�

		const POSGRID2		PART	= POSGRID2(1, MAX_ARROW);			// �e�N�X�`��������
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(90.0f, 90.0f, 9.0f);	// �傫��

		const D3DXVECTOR3	POS		= D3DXVECTOR3(select::POS.x - (select::SIZE_RULE.x * 0.5f) - SPACE_EDGE, select::POS.y, 0.0f);	// �ʒu
		const D3DXVECTOR3	SPACE	= D3DXVECTOR3(select::SIZE_RULE.x + (SPACE_EDGE * 2.0f), 0.0f, 0.0f);	// ��
		const D3DXCOLOR		MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);	// �F
	}

	// ������
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
const char *CEntryRuleManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\ruleTitle000.png",	// ���[���^�C�g���e�N�X�`��
	"data\\TEXTURE\\rule000.png",		// ���j�����e�N�X�`��
	"data\\TEXTURE\\rule001.png",		// ���������e�N�X�`��
	NULL,								// �J�n�{�^���e�N�X�`��
	NULL,								// ����\���e�N�X�`��
	NULL,								// ���e�N�X�`��
};

//************************************************************
//	�e�N���X [CEntryRuleManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEntryRuleManager::CEntryRuleManager()
{
	// �����o�ϐ����N���A
	memset(&m_apRuleTitle[0], 0, sizeof(m_apRuleTitle));	// ���[���^�C�g���̏��
	memset(&m_apArrow[0], 0, sizeof(m_apArrow));			// ���̏��
	m_pWinPoint	= NULL;	// �����|�C���g���̏��
	m_pKill		= NULL;	// ���j�����̏��
	m_pWin		= NULL;	// ���������̏��
	m_pSelect	= NULL;	// �I���̏��
	m_pStart	= NULL;	// �J�n�{�^���̏��
	m_pControl	= NULL;	// ����\���̏��
	m_pFade		= NULL;	// �t�F�[�h�̏��
	m_fSinControlAlpha	= 0.0f;			// ����\���̓�������
	m_fSinArrowAlpha	= 0.0f;			// ���\���̓�������
	m_state			= STATE_INIT;		// ���
	m_nSelect		= RULE_WINPOINT;	// ���݂̑I��
	m_nOldSelect	= RULE_WINPOINT;	// �O��̑I��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEntryRuleManager::~CEntryRuleManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEntryRuleManager::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apRuleTitle[0], 0, sizeof(m_apRuleTitle));	// ���[���^�C�g���̏��
	memset(&m_apArrow[0], 0, sizeof(m_apArrow));			// ���̏��
	m_pWinPoint	= NULL;	// �����|�C���g���̏��
	m_pKill		= NULL;	// ���j�����̏��
	m_pWin		= NULL;	// ���������̏��
	m_pSelect	= NULL;	// �I���̏��
	m_pStart	= NULL;	// �J�n�{�^���̏��
	m_pControl	= NULL;	// ����\���̏��
	m_pFade		= NULL;	// �t�F�[�h�̏��
	m_fSinControlAlpha	= -HALF_PI;		// ����\���̓�������
	m_fSinArrowAlpha	= -HALF_PI;		// ���\���̓�������
	m_state			= STATE_INIT;		// ���
	m_nSelect		= RULE_WINPOINT;	// ���݂̑I��
	m_nOldSelect	= RULE_WINPOINT;	// �O��̑I��

	// �t�F�[�h�̐���
	{
		m_pFade = CObject2D::Create
		( // ����
			SCREEN_CENT,	// �ʒu
			SCREEN_SIZE,	// �傫��
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
	}

	// ���[���^�C�g���̐���
	{
		for (int i = 0; i < RULE_MAX; i++)
		{ // ���[���̑������J��Ԃ�

			// ���[���^�C�g���̐���
			m_apRuleTitle[i] = CAnim2D::Create
			( // ����
				rule::PART.x,	// �e�N�X�`����������
				rule::PART.y,	// �e�N�X�`���c������
				rule::POS + ((float)i * rule::SPACE),	// �ʒu
				rule::SIZE								// �傫��
			);
			if (m_apRuleTitle[i] == NULL)
			{ // �����Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �e�N�X�`����o�^�E����
			m_apRuleTitle[i]->BindTexture(mc_apTextureFile[TEXTURE_RULE_TITLE]);

			// �D�揇�ʂ�ݒ�
			m_apRuleTitle[i]->SetPriority(PRIORITY);

			// �p�^�[����ݒ�
			m_apRuleTitle[i]->SetPattern(i);
		}
	}

	// ���̐���
	{
		for (int i = 0; i < MAX_ARROW; i++)
		{ // ���̑������J��Ԃ�

			// ���̐���
			m_apArrow[i] = CAnim2D::Create
			( // ����
				arrow::PART.x,	// �e�N�X�`����������
				arrow::PART.y,	// �e�N�X�`���c������
				arrow::POS + ((float)i * arrow::SPACE),	// �ʒu
				arrow::SIZE,	// �傫��
				VEC3_ZERO,		// ����
				XCOL_AWHITE		// �F
			);
			if (m_apArrow[i] == NULL)
			{ // �����Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �e�N�X�`����o�^�E����
			m_apArrow[i]->BindTexture(mc_apTextureFile[TEXTURE_ARROW]);

			// �D�揇�ʂ�ݒ�
			m_apArrow[i]->SetPriority(PRIORITY);

			// �p�^�[����ݒ�
			m_apArrow[i]->SetPattern(i);
		}
	}

	// �I���̐���
	{
		m_pSelect = CObject2D::Create
		( // ����
			select::POS,		// �ʒu
			select::SIZE_RULE,	// �傫��
			VEC3_ZERO,			// ����
			select::COL			// �F
		);
		if (m_pSelect == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_pSelect->SetPriority(PRIORITY);
	}

	// �J�n�{�^���̐���
	{
		m_pStart = CObject2D::Create
		( // ����
			start::POS,			// �ʒu
			start::SIZE,		// �傫��
			VEC3_ZERO,			// ����
			select::DEFAULT_COL	// �F
		);
		if (m_pStart == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_pStart->BindTexture(mc_apTextureFile[TEXTURE_START]);

		// �D�揇�ʂ�ݒ�
		m_pStart->SetPriority(PRIORITY);
	}

	// ����\���̐���
	{
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
		m_pStart->BindTexture(mc_apTextureFile[TEXTURE_CONTROL]);

		// �D�揇�ʂ�ݒ�
		m_pControl->SetPriority(PRIORITY);
	}

	// �����|�C���g���̐���
	{
		m_pWinPoint = CMultiValue::Create
		( // ����
			CValue::TEXTURE_NORMAL,	// �e�N�X�`��
			0,						// ����
			winpoint::DIGIT,		// ����
			winpoint::POS,			// �ʒu
			winpoint::SIZE,			// �傫��
			winpoint::SPACE,		// �s��
			VEC3_ZERO,				// ����
			select::CHOICE_COL		// �F
		);
		if (m_pWinPoint == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_pWinPoint->SetPriority(PRIORITY);

		// �ő�l��ݒ�
		m_pWinPoint->SetMax(MAX_WINPOINT);

		// �ŏ��l��ݒ�
		m_pWinPoint->SetMin(MIN_WINPOINT);

		// �l��ݒ�
		m_pWinPoint->SetNum(CManager::GetInstance()->GetRetentionManager()->GetWinPoint());
	}

	// ���j�����̐���
	{
		m_pKill = CAnim2D::Create
		( // ����
			kill::PART.x,		// �e�N�X�`����������
			kill::PART.y,		// �e�N�X�`���c������
			kill::POS,			// �ʒu
			kill::SIZE,			// �傫��
			VEC3_ZERO,			// ����
			select::DEFAULT_COL	// �F
		);
		if (m_pKill == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_pKill->BindTexture(mc_apTextureFile[TEXTURE_KILL]);

		// �D�揇�ʂ�ݒ�
		m_pKill->SetPriority(PRIORITY);

		// �p�^�[����ݒ�
		m_pKill->SetPattern(CManager::GetInstance()->GetRetentionManager()->GetKillState());
	}

	// ���������̐���
	{
		m_pWin = CAnim2D::Create
		( // ����
			win::PART.x,		// �e�N�X�`����������
			win::PART.y,		// �e�N�X�`���c������
			win::POS,			// �ʒu
			win::SIZE,			// �傫��
			VEC3_ZERO,			// ����
			select::DEFAULT_COL	// �F
		);
		if (m_pWin == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_pWin->BindTexture(mc_apTextureFile[TEXTURE_WIN]);

		// �D�揇�ʂ�ݒ�
		m_pWin->SetPriority(PRIORITY);

		// �p�^�[����ݒ�
		m_pWin->SetPattern(CManager::GetInstance()->GetRetentionManager()->GetWinState());
	}

	// UI�̎����`���OFF�ɂ���
	SetEnableUI(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CEntryRuleManager::Uninit(void)
{
	for (int i = 0; i < RULE_MAX; i++)
	{ // ���[���̑������J��Ԃ�

		// ���[���^�C�g���̏I��
		m_apRuleTitle[i]->Uninit();
	}

	for (int i = 0; i < MAX_ARROW; i++)
	{ // ���̑������J��Ԃ�

		// ���̏I��
		m_apArrow[i]->Uninit();
	}

	// �I���̏I��
	m_pSelect->Uninit();

	// �J�n�{�^���̏I��
	m_pStart->Uninit();

	// ����\���̏I��
	m_pControl->Uninit();

	// �����|�C���g���̏I��
	m_pWinPoint->Uninit();

	// ���j�����̏I��
	m_pKill->Uninit();

	// ���������̏I��
	m_pWin->Uninit();

	// �t�F�[�h�̏I��
	m_pFade->Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CEntryRuleManager::Update(void)
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::FADE_NONE)
	{ // �t�F�[�h���̏ꍇ

		// �����𔲂���
		return;
	}

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_INIT:

		// UI�̎����`���ON�ɂ���
		SetEnableUI(true);

		// �t�F�[�h�C����Ԃɂ���
		m_state = STATE_FADEIN;

		// �����𔲂����Ƀt�F�[�h�C���Ɉڍs

	case STATE_FADEIN:

		// �t�F�[�h�C���̍X�V
		UpdateFadeIn();

		break;

	case STATE_RULESET:

		// �I�𑀍�
		Select();

		// ���葀��
		Decide();

		// ���[���ύX����
		ChangeRule();

		// ����X�V
		UpdateControl();

		// ���X�V
		UpdateArrow();

		break;

	case STATE_FADEOUT:

		// �t�F�[�h�A�E�g�̍X�V
		UpdateFadeOut();

		break;

	case STATE_ENTRYBACK:

		// UI�̎����`���OFF�ɂ���
		SetEnableUI(false);

		// �G���g���[��Ԃɖ߂�
		CSceneEntry::GetEntryManager()->SetState(CEntryManager::STATE_ENTRY);

		// �֐��𔲂���
		return;

	default:
		assert(false);
		break;
	}

	for (int i = 0; i < RULE_MAX; i++)
	{ // ���[���̑������J��Ԃ�

		// ���[���^�C�g���̍X�V
		m_apRuleTitle[i]->Update();
	}

	for (int i = 0; i < MAX_ARROW; i++)
	{ // ���̑������J��Ԃ�

		// ���̍X�V
		m_apArrow[i]->Update();
	}

	// �I���̍X�V
	m_pSelect->Update();

	// �J�n�{�^���̍X�V
	m_pStart->Update();

	// ����\���̍X�V
	m_pControl->Update();

	// �����|�C���g���̍X�V
	m_pWinPoint->Update();

	// ���j�����̍X�V
	m_pKill->Update();

	// ���������̍X�V
	m_pWin->Update();

	// �t�F�[�h�̍X�V
	m_pFade->Update();
}

//============================================================
//	��������
//============================================================
CEntryRuleManager *CEntryRuleManager::Create(void)
{
	// �|�C���^��錾
	CEntryRuleManager *pEntryRuleManager = NULL;	// �G���g���[���[���}�l�[�W���[�����p

	if (pEntryRuleManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pEntryRuleManager = new CEntryRuleManager;	// �G���g���[���[���}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pEntryRuleManager != NULL)
	{ // �g�p����Ă���ꍇ
		
		// �G���g���[���[���}�l�[�W���[�̏�����
		if (FAILED(pEntryRuleManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pEntryRuleManager;
			pEntryRuleManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEntryRuleManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CEntryRuleManager::Release(CEntryRuleManager *&prEntryRuleManager)
{
	if (prEntryRuleManager != NULL)
	{ // �g�p���̏ꍇ

		// �G���g���[���[���}�l�[�W���[�̏I��
		if (FAILED(prEntryRuleManager->Uninit()))
		{ // �j���Ɏ��s�����ꍇ

			// �������J��
			delete prEntryRuleManager;
			prEntryRuleManager = NULL;

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �������J��
		delete prEntryRuleManager;
		prEntryRuleManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�t�F�[�h�C���̍X�V����
//============================================================
void CEntryRuleManager::UpdateFadeIn(void)
{
	// �ϐ���錾
	D3DXCOLOR colFade = m_pFade->GetColor();	// �t�F�[�h�̐F

	// �����x�����Z
	colFade.a += fade::ADD_ALPHA;

	if (colFade.a >= fade::SET_COL.a)
	{ // �����x���オ�肫�����ꍇ

		// �����x��␳
		colFade.a = fade::SET_COL.a;

		// ���[���ݒ��Ԃɂ���
		m_state = STATE_RULESET;
	}

	// UI�̓����x�̐ݒ�
	SetAlphaUI(colFade.a * 2.0f, false);

	// �����x�𔽉f
	m_pFade->SetColor(colFade);
}

//============================================================
//	�t�F�[�h�A�E�g�̍X�V����
//============================================================
void CEntryRuleManager::UpdateFadeOut(void)
{
	// �ϐ���錾
	D3DXCOLOR colFade = m_pFade->GetColor();	// �t�F�[�h�̐F

	// �����x�����Z
	colFade.a -= fade::SUB_ALPHA;

	if (colFade.a <= fade::INIT_COL.a)
	{ // �����x�������肫�����ꍇ

		// �����x��␳
		colFade.a = fade::INIT_COL.a;

		// �G���g���[�߂���Ԃɂ���
		m_state = STATE_ENTRYBACK;
	}

	// UI�̓����x�̐ݒ�
	SetAlphaUI(colFade.a * 2.0f, true);

	// �����x�𔽉f
	m_pFade->SetColor(colFade);
}

//============================================================
//	����̍X�V����
//============================================================
void CEntryRuleManager::UpdateControl(void)
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
		m_fSinControlAlpha += control::ADD_SINROT;
		useful::NormalizeRot(m_fSinControlAlpha);	// �������K��

		// �����x���Z�ʂ����߂�
		fAddAlpha = (control::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinControlAlpha) - 1.0f);

		// ����\���F��ݒ�
		m_pControl->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, control::BASIC_ALPHA + fAddAlpha));
	}
}

//============================================================
//	���̍X�V����
//============================================================
void CEntryRuleManager::UpdateArrow(void)
{
	for (int i = 0; i < MAX_ARROW; i++)
	{ // ���̑������J��Ԃ�

		// �ϐ���錾
		D3DXCOLOR colArrow = m_apArrow[i]->GetColor();	// ���F

		if (colArrow.a < arrow::MIN_COL.a)
		{ // �����x���Œ�����Ⴂ�ꍇ

			// �����x�����Z
			colArrow.a += arrow::ADD_ALPHA;

			if (colArrow.a > arrow::MIN_COL.a)
			{ // �����x�����߂����ꍇ

				// �����x��␳
				colArrow.a = arrow::MIN_COL.a;
			}

			// ���F��ݒ�
			m_apArrow[i]->SetColor(colArrow);
		}
		else
		{ // �����x���Œ���ȏ�̏ꍇ

			// �ϐ���錾
			float fAddAlpha = 0.0f;	// �����x�̉��Z��

			// �����x���グ��
			m_fSinArrowAlpha += arrow::ADD_SINROT;
			useful::NormalizeRot(m_fSinArrowAlpha);	// �������K��

			// �����x���Z�ʂ����߂�
			fAddAlpha = (arrow::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinArrowAlpha) - 1.0f);

			// ���F��ݒ�
			m_apArrow[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, arrow::BASIC_ALPHA + fAddAlpha));
		}
	}
}

//============================================================
//	�I�𑀍쏈��
//============================================================
void CEntryRuleManager::Select(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h

	CObject *apSelect[MAX_SELECT] =	// �I���I�u�W�F�N�g
	{
		m_pWinPoint,	// �����|�C���g
		m_pKill,		// ���j����
		m_pWin,			// ��������
		m_pStart		// �J�n�{�^��
	};

	// �ϐ���錾
	D3DXVECTOR3 posSelect = m_pSelect->GetVec3Position();	// �I���ʒu
	D3DXVECTOR3 sizeSelect = m_pSelect->GetVec3Sizing();	// �I��傫��

	if (pKeyboard->IsTrigger(DIK_W)
	||  pKeyboard->IsTrigger(DIK_UP)
	||  pPad->IsTrigger(CInputPad::KEY_UP))
	{ // ��ړ��̑��삪�s��ꂽ�ꍇ

		// ��ɑI�������炷
		m_nSelect = (m_nSelect + (MAX_SELECT - 1)) % MAX_SELECT;

		if (m_nSelect != SELECT_GAME)
		{ // �I�����Q�[���J�ڂł͂Ȃ��ꍇ

			// �ʒu��ύX
			posSelect.x = select::POS.x;
			posSelect.y = select::POS.y + rule::SPACE.y * (float)m_nSelect;

			// �傫����ύX
			sizeSelect = select::SIZE_RULE;

			for (int i = 0; i < MAX_ARROW; i++)
			{ // ���̑������J��Ԃ�

				// ���̈ʒu��ύX
				m_apArrow[i]->SetVec3Position(D3DXVECTOR3(arrow::POS.x + (arrow::SPACE.x * (float)i), arrow::POS.y + rule::SPACE.y * (float)m_nSelect, 0.0f));

				// �����`���ON�ɂ���
				m_apArrow[i]->SetEnableDraw(true);
			}
		}
		else
		{ // �I�����Q�[���J�ڂ̏ꍇ

			// �ʒu��ύX
			posSelect = start::POS;

			// �傫����ύX
			sizeSelect = select::SIZE_START;

			for (int i = 0; i < MAX_ARROW; i++)
			{ // ���̑������J��Ԃ�

				// �����`���OFF�ɂ���
				m_apArrow[i]->SetEnableDraw(false);
			}
		}

		// �T�E���h�̍Đ�
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
	}
	if (pKeyboard->IsTrigger(DIK_S)
	||  pKeyboard->IsTrigger(DIK_DOWN)
	||  pPad->IsTrigger(CInputPad::KEY_DOWN))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// ���ɑI�������炷
		m_nSelect = (m_nSelect + 1) % MAX_SELECT;

		if (m_nSelect != SELECT_GAME)
		{ // �I�����Q�[���J�ڂł͂Ȃ��ꍇ

			// �ʒu��ύX
			posSelect.x = select::POS.x;
			posSelect.y = select::POS.y + rule::SPACE.y * (float)m_nSelect;

			// �傫����ύX
			sizeSelect = select::SIZE_RULE;

			for (int i = 0; i < MAX_ARROW; i++)
			{ // ���̑������J��Ԃ�

				// ���̈ʒu��ύX
				m_apArrow[i]->SetVec3Position(D3DXVECTOR3(arrow::POS.x + (arrow::SPACE.x * (float)i), arrow::POS.y + rule::SPACE.y * (float)m_nSelect, 0.0f));

				// �����`���ON�ɂ���
				m_apArrow[i]->SetEnableDraw(true);
			}
		}
		else
		{ // �I�����Q�[���J�ڂ̏ꍇ

			// �ʒu��ύX
			posSelect = start::POS;

			// �傫����ύX
			sizeSelect = select::SIZE_START;

			for (int i = 0; i < MAX_ARROW; i++)
			{ // ���̑������J��Ԃ�

				// �����`���OFF�ɂ���
				m_apArrow[i]->SetEnableDraw(false);
			}
		}

		// �T�E���h�̍Đ�
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
	}

	// �I�����f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�I��]�F%d\n", m_nSelect);

	// �O��̑I��v�f�̐F�����ɐݒ�
	apSelect[m_nOldSelect]->SetColor(select::DEFAULT_COL);

	// ���݂̑I��v�f�̐F�𔒂ɐݒ�
	apSelect[m_nSelect]->SetColor(select::CHOICE_COL);

	// ���݂̑I��v�f����
	m_nOldSelect = m_nSelect;

	// �I���ʒu�𔽉f
	m_pSelect->SetVec3Position(posSelect);

	// �I��傫���𔽉f
	m_pSelect->SetVec3Sizing(sizeSelect);
}

//============================================================
//	���葀�쏈��
//============================================================
void CEntryRuleManager::Decide(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h

	if (pKeyboard->IsTrigger(DIK_RETURN)
	||  pPad->IsTrigger(CInputPad::KEY_A) || pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X) || pPad->IsTrigger(CInputPad::KEY_Y))
	{ // ����̑��삪�s��ꂽ�ꍇ

		switch (m_nSelect)
		{ // �I�����Ƃ̏���
		case RULE_WINPOINT:
		case RULE_KILL:
		case RULE_WIN:
			break;

		case SELECT_GAME:

			// �c�[���ύX���f
			SetRule();

			// �I����Ԃɂ���
			CSceneEntry::GetEntryManager()->SetState(CEntryManager::STATE_END);	// �Q�[���ɑJ��

			break;

		default:
			assert(false);
			break;
		}
	}

	if (pKeyboard->IsTrigger(DIK_SPACE))
	{ // �߂鑀�삪�s��ꂽ�ꍇ

		// �t�F�[�h�A�E�g��Ԃɂ���
		m_state = STATE_FADEOUT;
	}
}

//============================================================
//	���[���ύX���쏈��
//============================================================
void CEntryRuleManager::ChangeRule(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h

	// �ϐ���錾
	int nKill = m_pKill->GetPattern();	// ���j����
	int nWin  = m_pWin->GetPattern();	// ��������

	if (pKeyboard->IsTrigger(DIK_A)
	||  pKeyboard->IsTrigger(DIK_LEFT)
	||  pPad->IsTrigger(CInputPad::KEY_LEFT))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		switch (m_nSelect)
		{ // �I�����Ƃ̏���
		case RULE_WINPOINT:

			// �����|�C���g�����Z
			m_pWinPoint->AddNum(-1);

			break;

		case RULE_KILL:

			// ���ɑI�������炷
			nKill = (nKill + (CRetentionManager::KILL_MAX - 1)) % CRetentionManager::KILL_MAX;

			break;

		case RULE_WIN:

			// ���ɑI�������炷
			nWin = (nWin + (CRetentionManager::WIN_MAX - 1)) % CRetentionManager::WIN_MAX;

			break;

		case SELECT_GAME:
			break;

		default:
			assert(false);
			break;
		}

		// �T�E���h�̍Đ�
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
	}
	if (pKeyboard->IsTrigger(DIK_D)
	||  pKeyboard->IsTrigger(DIK_RIGHT)
	||  pPad->IsTrigger(CInputPad::KEY_RIGHT))
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		switch (m_nSelect)
		{ // �I�����Ƃ̏���
		case RULE_WINPOINT:

			// �����|�C���g�����Z
			m_pWinPoint->AddNum(1);

			break;

		case RULE_KILL:

			// �E�ɑI�������炷
			nKill = (nKill + 1) % CRetentionManager::KILL_MAX;

			break;

		case RULE_WIN:

			// �E�ɑI�������炷
			nWin = (nWin + 1) % CRetentionManager::WIN_MAX;

			break;

		case SELECT_GAME:
			break;

		default:
			assert(false);
			break;
		}

		// �T�E���h�̍Đ�
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
	}

	// ���j�����𔽉f
	m_pKill->SetPattern(nKill);

	// ���������𔽉f
	m_pWin->SetPattern(nWin);
}

//============================================================
//	���[���ύX���f����
//============================================================
void CEntryRuleManager::SetRule(void)
{
	// �|�C���^��錾
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// �f�[�^�ۑ����

	// �����|�C���g����ݒ�
	pRetention->SetWinPoint(m_pWinPoint->GetNum());

	// ���j������ݒ�
	pRetention->SetKillState((CRetentionManager::EKill)m_pKill->GetPattern());

	// ����������ݒ�
	pRetention->SetWinState((CRetentionManager::EWin)m_pWin->GetPattern());
}

//============================================================
//	UI�̕`��󋵂̐ݒ菈��
//============================================================
void CEntryRuleManager::SetEnableUI(const bool bDraw)
{
	for (int i = 0; i < RULE_MAX; i++)
	{ // ���[���̑������J��Ԃ�

		// ���[���^�C�g���̕`���ݒ�
		m_apRuleTitle[i]->SetEnableDraw(bDraw);
	}

	for (int i = 0; i < MAX_ARROW; i++)
	{ // ���̑������J��Ԃ�

		// ���̕`���ݒ�
		m_apArrow[i]->SetEnableDraw(bDraw);
	}

	// �I���̕`���ݒ�
	m_pSelect->SetEnableDraw(bDraw);

	// �J�n�{�^���̕`���ݒ�
	m_pStart->SetEnableDraw(bDraw);

	// ����\���̕`���ݒ�
	m_pControl->SetEnableDraw(bDraw);

	// �����|�C���g���̕`���ݒ�
	m_pWinPoint->SetEnableDraw(bDraw);

	// ���j�����̕`���ݒ�
	m_pKill->SetEnableDraw(bDraw);

	// ���������̕`���ݒ�
	m_pWin->SetEnableDraw(bDraw);
}

//============================================================
//	UI�̓����x�̐ݒ菈��
//============================================================
void CEntryRuleManager::SetAlphaUI(const float fAlpha, const bool bFadeOut)
{
	// �ϐ���錾
	D3DXCOLOR col = XCOL_WHITE;	// �F

	for (int i = 0; i < RULE_MAX; i++)
	{ // ���[���̑������J��Ԃ�

		// ���[���^�C�g���̓����x��ݒ�
		col = m_apRuleTitle[i]->GetColor();
		m_apRuleTitle[i]->SetColor(D3DXCOLOR(col.r, col.g, col.b, fAlpha));
	}

	// �J�n�{�^���̓����x��ݒ�
	col = m_pStart->GetColor();
	m_pStart->SetColor(D3DXCOLOR(col.r, col.g, col.b, fAlpha));

	// �����|�C���g���̓����x��ݒ�
	col = m_pWinPoint->GetColor();
	m_pWinPoint->SetColor(D3DXCOLOR(col.r, col.g, col.b, fAlpha));

	// ���j�����̓����x��ݒ�
	col = m_pKill->GetColor();
	m_pKill->SetColor(D3DXCOLOR(col.r, col.g, col.b, fAlpha));

	// ���������̓����x��ݒ�
	col = m_pWin->GetColor();
	m_pWin->SetColor(D3DXCOLOR(col.r, col.g, col.b, fAlpha));

	if (bFadeOut)
	{ // �t�F�[�h�A�E�g���̏ꍇ

		// �ϐ���錾
		float fSetAlpha = 0.0f;	// �ݒ蓧���x
		float fMaxAlpha = 0.0f;	// �ő哧���x

		for (int i = 0; i < MAX_ARROW; i++)
		{ // ���̑������J��Ԃ�

			// �F����ݒ�
			col = m_apArrow[i]->GetColor();	// �F
			fMaxAlpha = col.a;				// �ő哧���x
			fSetAlpha = fAlpha;				// �ݒ蓧���x

			// �����x�𐧌�
			useful::LimitNum(fSetAlpha, 0.0f, fMaxAlpha);

			// ���̓����x��ݒ�
			m_apArrow[i]->SetColor(D3DXCOLOR(col.r, col.g, col.b, fSetAlpha));
		}

		// �I���̓����x��ݒ�
		{
			// �F����ݒ�
			col = m_pSelect->GetColor();	// �F
			fMaxAlpha = col.a;				// �ő哧���x
			fSetAlpha = fAlpha;				// �ݒ蓧���x

			// �����x�𐧌�
			useful::LimitNum(fSetAlpha, 0.0f, fMaxAlpha);

			// �I���̓����x��ݒ�
			m_pSelect->SetColor(D3DXCOLOR(col.r, col.g, col.b, fSetAlpha));
		}

		// ����\���̓����x��ݒ�
		{
			// �F����ݒ�
			col = m_pControl->GetColor();	// �F
			fMaxAlpha = col.a;				// �ő哧���x
			fSetAlpha = fAlpha;				// �ݒ蓧���x

			// �����x�𐧌�
			useful::LimitNum(fSetAlpha, 0.0f, fMaxAlpha);

			// ����\���̓����x��ݒ�
			m_pControl->SetColor(D3DXCOLOR(col.r, col.g, col.b, fSetAlpha));
		}
	}
}
