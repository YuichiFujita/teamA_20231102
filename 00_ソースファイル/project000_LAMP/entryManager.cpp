//============================================================
//
//	�G���g���[�}�l�[�W���[���� [entryManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "entryManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "valueUI.h"
#include "object2D.h"
#include "multiValue.h"
#include "entryRuleManager.h"
#include "retentionManager.h"
#include "fade.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 13;	// �G���g���[�̗D�揇��

	const D3DXCOLOR COL_ENTRY	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �Q�����J���[
	const D3DXCOLOR COL_UNENTRY	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// ��Q�����J���[

	namespace number
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(165.0f, 60.0f, 0.0f);		// �ʒu
		const D3DXVECTOR3	SIZE_TITLE	= D3DXVECTOR3(242.0f, 107.0f, 0.0f);	// �^�C�g���傫��
		const D3DXVECTOR3	SIZE_VALUE	= D3DXVECTOR3(80.0f, 90.0f, 0.0f);		// �����傫��
		const D3DXVECTOR3	SPACE_POS	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// ����UI���m�̋�
		const D3DXVECTOR3	SPACE_TITLE	= D3DXVECTOR3(100.0f, 5.0f, 0.0f);		// �^�C�g����
		const D3DXVECTOR3	SPACE_VALUE	= VEC3_ZERO;							// ������
		const int			DIGIT		= 1;									// ����
	}

	namespace cpu
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(165.0f, 60.0f, 0.0f);		// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(242.0f, 107.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// ��
	}

	namespace frame
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(160.0f, 280.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(250.0f, 340.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// ��
	}

	namespace join
	{
		const POSGRID2	  PART	= POSGRID2(2, 1);	// �e�N�X�`��������
		const D3DXVECTOR3 POS	= D3DXVECTOR3(160.0f, 480.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(250.0f, 70.0f, 0.0f);		// �傫��
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// ��
	}

	namespace control
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, 620.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(505.0f, 84.0f, 0.0f);			// �傫��
	}

	namespace start
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, 370.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(380.0f, 80.0f, 0.0f);			// �傫��
	}

	namespace numcpu
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(SCREEN_CENT.x, 500.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3	SIZE_TITLE	= D3DXVECTOR3(242.0f, 107.0f, 0.0f);		// �^�C�g���傫��
		const D3DXVECTOR3	SIZE_VALUE	= D3DXVECTOR3(80.0f, 90.0f, 0.0f);			// �����傫��
		const D3DXVECTOR3	SPACE_TITLE	= D3DXVECTOR3(100.0f, 5.0f, 0.0f);			// �^�C�g����
		const D3DXVECTOR3	SPACE_VALUE	= VEC3_ZERO;								// ������
		const int			DIGIT		= 1;										// ����
	}

	namespace arrow
	{
		const float	ADD_ALPHA		= 0.02f;	// �����x�̉��Z��
		const float	ADD_SINROT		= 0.04f;	// �����x�ӂ�ӂ킳����ۂ̃T�C���J�[�u�������Z��
		const float	MAX_ADD_ALPHA	= 0.25f;	// �����x�̍ő���Z��
		const float	BASIC_ALPHA		= 0.95f;	// ��̓����x
		const float	SPACE_EDGE		= 170.0f;	// ���̋�

		const POSGRID2		PART	= POSGRID2(MAX_ENTRY_ARROW, 1);		// �e�N�X�`��������
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(80.0f, 80.0f, 0.0f);	// �傫��

		const D3DXVECTOR3	POS		= D3DXVECTOR3(SCREEN_CENT.x - SPACE_EDGE, numcpu::POS.y, 0.0f);	// �ʒu
		const D3DXVECTOR3	SPACE	= D3DXVECTOR3(SPACE_EDGE * 2.0f, 0.0f, 0.0f);					// ��
		const D3DXCOLOR		MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);		// �F
	}

	namespace bg
	{
		const D3DXVECTOR3	POS		= D3DXVECTOR3(SCREEN_CENT.x, 440.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(SCREEN_SIZE.x, 280.0f, 0.0f);	// �傫��
		const D3DXCOLOR		COL		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.625f);		// �F
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CEntryManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\entry_player.png",	// PLAYER�e�N�X�`��
	"data\\TEXTURE\\entry_flame.png",	// �t���[���e�N�X�`��
	"data\\TEXTURE\\entry002.png",		// ����\���e�N�X�`��
	"data\\TEXTURE\\entry003.png",		// �J�n�\���e�N�X�`��
	"data\\TEXTURE\\entry004.png",		// �Q���󋵃e�N�X�`��
	"data\\TEXTURE\\Arrow_Twin.png",	// ���e�N�X�`��
	"data\\TEXTURE\\entry_player.png",	// CPU�e�N�X�`��
};

//************************************************************
//	�e�N���X [CEntryManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEntryManager::CEntryManager()
{
	// �����o�ϐ����N���A
	memset(&m_apNumber[0],	0, sizeof(m_apNumber));	// �v���C���[�i���o�[�̏��
	memset(&m_apCpu[0],		0, sizeof(m_apCpu));	// �v���C���[CPU�̏��
	memset(&m_apFrame[0],	0, sizeof(m_apFrame));	// �v���C���[�t���[���̏��
	memset(&m_apJoin[0],	0, sizeof(m_apJoin));	// �v���C���[�Q���̏��
	memset(&m_apArrow[0],	0, sizeof(m_apArrow));	// �v���C���[�Q���̏��
	m_pRuleManager	= NULL;		// �G���g���[���[���̏��
	m_pControl	= NULL;			// ����\���̏��
	m_pBG		= NULL;			// �w�i�̏��
	m_pStart	= NULL;			// �J�n�\���̏��
	m_pNumCpu	= NULL;			// CPU���\���̏��
	m_state		= STATE_ENTRY;	// ���
	m_fSinAlpha	= 0.0f;			// ��������
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEntryManager::~CEntryManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEntryManager::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apNumber[0],	0, sizeof(m_apNumber));	// �v���C���[�i���o�[�̏��
	memset(&m_apCpu[0],		0, sizeof(m_apCpu));	// �v���C���[CPU�̏��
	memset(&m_apFrame[0],	0, sizeof(m_apFrame));	// �v���C���[�t���[���̏��
	memset(&m_apJoin[0],	0, sizeof(m_apJoin));	// �v���C���[�Q���̏��
	memset(&m_apArrow[0],	0, sizeof(m_apArrow));	// �v���C���[�Q���̏��
	m_pRuleManager	= NULL;		// �G���g���[���[���̏��
	m_pControl	= NULL;			// ����\���̏��
	m_pBG		= NULL;			// �w�i�̏��
	m_pStart	= NULL;			// �J�n�\���̏��
	m_pNumCpu	= NULL;			// CPU���\���̏��
	m_state		= STATE_ENTRY;	// ���
	m_fSinAlpha	= -HALF_PI;		// ��������

	// �Q�[������������
	CManager::GetInstance()->GetRetentionManager()->InitGame();

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		// �v���C���[�i���o�[�̐���
		m_apNumber[nCntEntry] = CValueUI::Create
		( // ����
			mc_apTextureFile[TEXTURE_PLAYER],						// �^�C�g���e�N�X�`���p�X
			CValue::TEXTURE_NORMAL,									// �����e�N�X�`��
			number::DIGIT,											// ����
			number::POS + (number::SPACE_POS * (float)nCntEntry),	// �ʒu
			number::SPACE_TITLE,	// �s��
			number::SPACE_VALUE,	// �����s��
			number::SIZE_TITLE,		// �^�C�g���傫��
			number::SIZE_VALUE,		// �����傫��
			VEC3_ZERO,				// �^�C�g������
			VEC3_ZERO,				// ��������
			COL_UNENTRY,			// �^�C�g���F
			COL_UNENTRY				// �����F
		);
		if (m_apNumber[nCntEntry] == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apNumber[nCntEntry]->SetPriority(PRIORITY);

		// ������ݒ�
		m_apNumber[nCntEntry]->GetMultiValue()->SetNum(nCntEntry + 1);

		// �v���C���[CPU�̐���
		m_apCpu[nCntEntry] = CObject2D::Create
		( // ����
			cpu::POS + (cpu::SPACE * (float)nCntEntry),	// �ʒu
			cpu::SIZE	// �傫��
		);
		if (m_apCpu[nCntEntry] == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apCpu[nCntEntry]->SetPriority(PRIORITY);

		// �����`���OFF�ɂ���
		m_apCpu[nCntEntry]->SetEnableDraw(false);

		// �v���C���[�t���[���̐���
		m_apFrame[nCntEntry] = CObject2D::Create
		( // ����
			frame::POS + (frame::SPACE * (float)nCntEntry),	// �ʒu
			frame::SIZE,	// �傫��
			VEC3_ZERO,		// ����
			COL_UNENTRY		// �F
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

		// �v���C���[�Q���̐���
		m_apJoin[nCntEntry] = CAnim2D::Create
		( // ����
			join::PART.x,	// �e�N�X�`����������
			join::PART.y,	// �e�N�X�`���c������
			join::POS + (join::SPACE * (float)nCntEntry),	// �ʒu
			join::SIZE		// �傫��
		);
		if (m_apJoin[nCntEntry] == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_apJoin[nCntEntry]->BindTexture(mc_apTextureFile[TEXTURE_JOIN]);

		// �D�揇�ʂ�ݒ�
		m_apJoin[nCntEntry]->SetPriority(PRIORITY);
	}

	// ����\���̐���
	m_pControl = CObject2D::Create
	( // ����
		control::POS,	// �ʒu
		control::SIZE	// �傫��
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

	// �w�i�̐���
	m_pBG = CObject2D::Create
	( // ����
		bg::POS,	// �ʒu
		bg::SIZE,	// �傫��
		VEC3_ZERO,	// ����
		bg::COL		// �F
	);
	if (m_pBG == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pBG->SetPriority(PRIORITY);

	// �����`���OFF�ɂ���
	m_pBG->SetEnableDraw(false);

	// �J�n�\���̐���
	m_pStart = CObject2D::Create
	( // ����
		start::POS,	// �ʒu
		start::SIZE	// �傫��
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

	// �����`���OFF�ɂ���
	m_pStart->SetEnableDraw(false);

	// CPU���\���̐���
	m_pNumCpu = CValueUI::Create
	( // ����
		mc_apTextureFile[TEXTURE_NUMCPU],	// �^�C�g���e�N�X�`���p�X
		CValue::TEXTURE_NORMAL,	// �����e�N�X�`��
		numcpu::DIGIT,			// ����
		numcpu::POS,			// �ʒu
		numcpu::SPACE_TITLE,	// �s��
		numcpu::SPACE_VALUE,	// �����s��
		numcpu::SIZE_TITLE,		// �^�C�g���傫��
		numcpu::SIZE_VALUE		// �����傫��
	);
	if (m_pNumCpu == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pNumCpu->SetPriority(PRIORITY);

	// �ŏ�����ݒ�
	m_pNumCpu->GetMultiValue()->SetMin(0);

	// �ő吔��ݒ�
	m_pNumCpu->GetMultiValue()->SetMax(MAX_PLAYER - 1);

	// �����`���OFF�ɂ���
	m_pNumCpu->SetEnableDraw(false);

	for (int i = 0; i < MAX_RULE_ARROW; i++)
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

		// �����`���OFF�ɂ���
		m_apArrow[i]->SetEnableDraw(false);
	}

	// �v���C�l����������
	CManager::GetInstance()->GetRetentionManager()->SetNumPlayer(0);

	// �G���g���[��������
	CManager::GetInstance()->GetRetentionManager()->AllSetEnableEntry(false, false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CEntryManager::Uninit(void)
{
	if (m_pRuleManager != NULL)
	{
		// �G���g���[���[���̔j��
		if (FAILED(CEntryRuleManager::Release(m_pRuleManager)))
		{ // ���s�����ꍇ

			// ���s��Ԃ�
			return E_FAIL;
		}
	}

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		// �v���C���[�i���o�[�̏I��
		m_apNumber[nCntEntry]->Uninit();

		// �v���C���[CPU�̏I��
		m_apCpu[nCntEntry]->Uninit();

		// �v���C���[�t���[���̏I��
		m_apFrame[nCntEntry]->Uninit();

		// �v���C���[�Q���̏I��
		m_apJoin[nCntEntry]->Uninit();
	}

	for (int i = 0; i < MAX_RULE_ARROW; i++)
	{ // ���̑������J��Ԃ�

		// ���̏I��
		m_apArrow[i]->Uninit();
	}

	// ����\���̏I��
	m_pControl->Uninit();

	// �w�i�̏I��
	m_pBG->Uninit();

	// �J�n�\���̏I��
	m_pStart->Uninit();

	// CPU���\���̏I��
	m_pNumCpu->Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CEntryManager::Update(void)
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::FADE_NONE)
	{ // �t�F�[�h���̏ꍇ

		// �����𔲂���
		return;
	}

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_ENTRY:

		// �G���g���[�̍X�V
		UpdateEntry();

		// CPU�̍X�V
		UpdateCpu();

		// �J�n�̍X�V
		UpdateStart();

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // �v���C���[�̍ő吔���J��Ԃ�

			// �v���C���[�i���o�[�̍X�V
			m_apNumber[nCntEntry]->Update();

			// �v���C���[CPU�̍X�V
			m_apCpu[nCntEntry]->Update();

			// �v���C���[�t���[���̍X�V
			m_apFrame[nCntEntry]->Update();

			// �v���C���[�Q���̍X�V
			m_apJoin[nCntEntry]->Update();
		}

		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // ���̑������J��Ԃ�

			// ���̍X�V
			m_apArrow[i]->Update();
		}

		// ����\���̍X�V
		m_pControl->Update();

		// �w�i�̍X�V
		m_pBG->Update();

		// �J�n�\���̍X�V
		m_pStart->Update();

		// CPU���\���̍X�V
		m_pNumCpu->Update();

		break;

	case STATE_RULE:

		if (m_pRuleManager != NULL)
		{
			// �G���g���[���[���̍X�V
			m_pRuleManager->Update();
		}

		break;

	case STATE_END:
		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
HRESULT CEntryManager::SetState(const EState state)
{
	// �����̏�Ԃ�ݒ�
	m_state = state;

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_ENTRY:

		if (m_pRuleManager != NULL)
		{
			// �G���g���[���[���̔j��
			if (FAILED(CEntryRuleManager::Release(m_pRuleManager)))
			{ // ���s�����ꍇ

				// ���s��Ԃ�
				return E_FAIL;
			}
		}

		break;

	case STATE_RULE:

		if (m_pRuleManager == NULL)
		{
			// �G���g���[���[���̐���
			m_pRuleManager = CEntryRuleManager::Create();
			if (m_pRuleManager == NULL)
			{ // ���s�����ꍇ

				// ���s��Ԃ�
				return E_FAIL;
			}
		}

		break;

	case STATE_END:

		// �V�[���̐ݒ�
		CManager::GetInstance()->SetScene(CScene::MODE_GAME);	// �Q�[�����

		break;

	default:
		assert(false);
		break;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	��������
//============================================================
CEntryManager *CEntryManager::Create(void)
{
	// �|�C���^��錾
	CEntryManager *pEntryManager = NULL;	// �G���g���[�}�l�[�W���[�����p

	if (pEntryManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pEntryManager = new CEntryManager;	// �G���g���[�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pEntryManager != NULL)
	{ // �g�p����Ă���ꍇ
		
		// �G���g���[�}�l�[�W���[�̏�����
		if (FAILED(pEntryManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pEntryManager;
			pEntryManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEntryManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CEntryManager::Release(CEntryManager *&prEntryManager)
{
	if (prEntryManager != NULL)
	{ // �g�p���̏ꍇ

		// �G���g���[�}�l�[�W���[�̏I��
		if (FAILED(prEntryManager->Uninit()))
		{ // �j���Ɏ��s�����ꍇ

			// �������J��
			delete prEntryManager;
			prEntryManager = NULL;

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �������J��
		delete prEntryManager;
		prEntryManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�G���g���[�̍X�V����
//============================================================
void CEntryManager::UpdateEntry(void)
{
	// �|�C���^��錾
	CInputPad *pPad = CManager::GetInstance()->GetPad();	// �p�b�h
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// �f�[�^�ۑ����

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		bool bEntry = true;	// �G���g���[���ł��邩
		if (pRetention->IsEntry(nCntEntry))
		{ // �G���g���[���Ă���ꍇ

			if (pRetention->IsAI(nCntEntry))
			{ // AI�̃G���g���[�Ȃ�㏑���\

				bEntry = true;
			}
			else
			{ // �l�Ԃ̃G���g���[�Ȃ�㏑���s��

				bEntry = false;
			}
		}

		if (bEntry)
		{ // �G���g���[�o����ꍇ

			if (pPad->IsTrigger(CInputPad::KEY_A, nCntEntry))
			{
				// �G���g���[��o�^
				pRetention->SetEntry(nCntEntry, true, false);

				// ������������Ԃɂ���
				m_apJoin[nCntEntry]->SetPattern(JOIN_OFF);

				// �F���Q�����̂��̂ɐݒ�
				m_apFrame[nCntEntry]->SetColor(COL_ENTRY);
				m_apNumber[nCntEntry]->SetColorTitle(COL_ENTRY);
				m_apNumber[nCntEntry]->GetMultiValue()->SetColor(COL_ENTRY);
			}
		}
		else
		{ // �G���g���[�o���Ȃ��ꍇ

			switch (m_apJoin[nCntEntry]->GetPattern())
			{ // �����󋵂��Ƃ̏���
			case JOIN_OFF:

				if (pPad->IsTrigger(CInputPad::KEY_A, nCntEntry))
				{
					// ����������Ԃɂ���
					m_apJoin[nCntEntry]->SetPattern(JOIN_ON);
				}
				else if (pPad->IsTrigger(CInputPad::KEY_B, nCntEntry))
				{
					// �G���g���[������
					pRetention->SetEntry(nCntEntry, false, false);

					// �F���Q�����̂��̂ɐݒ�
					m_apFrame[nCntEntry]->SetColor(COL_UNENTRY);
					m_apNumber[nCntEntry]->SetColorTitle(COL_UNENTRY);
					m_apNumber[nCntEntry]->GetMultiValue()->SetColor(COL_UNENTRY);
				}

				break;

			case JOIN_ON:

				if (pPad->IsTrigger(CInputPad::KEY_B, nCntEntry))
				{
					// ������������Ԃɂ���
					m_apJoin[nCntEntry]->SetPattern(JOIN_OFF);
				}

				break;

			default:
				assert(false);
				break;
			}
		}
	}

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[��]�F%d\n", pRetention->GetNumPlayer());
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[1]�F%s\n", pRetention->IsEntry(0) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[2]�F%s\n", pRetention->IsEntry(1) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[3]�F%s\n", pRetention->IsEntry(2) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[4]�F%s\n", pRetention->IsEntry(3) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[1AI]�F%s\n", pRetention->IsAI(0) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[2AI]�F%s\n", pRetention->IsAI(1) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[3AI]�F%s\n", pRetention->IsAI(2) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[4AI]�F%s\n", pRetention->IsAI(3) ? "true" : "false");
}

//============================================================
//	CPU�̍X�V����
//============================================================
void CEntryManager::UpdateCpu(void)
{
	if (IsReadyOK(1))
	{ // �S���������ς݂̏ꍇ

		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // ���̑������J��Ԃ�

			// ���̎����`���ON�ɂ���
			m_apArrow[i]->SetEnableDraw(true);
		}

		// �����`���ON�ɂ���
		m_pBG->SetEnableDraw(true);			// �w�i
		m_pStart->SetEnableDraw(true);		// �J�n�\��
		m_pNumCpu->SetEnableDraw(true);		// CPU��

		// ���̍X�V
		UpdateArrow();

		// CPU�̉����Z�̍X�V
		UpdateAddCpu();
	}
	else
	{ // �S���������ς݂ł͂Ȃ��ꍇ

		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // ���̑������J��Ԃ�

			// ���̎����`���OFF�ɂ���
			m_apArrow[i]->SetEnableDraw(false);
		}

		// �����`���OFF�ɂ���
		m_pBG->SetEnableDraw(false);		// �w�i
		m_pStart->SetEnableDraw(false);		// �J�n�\��
		m_pNumCpu->SetEnableDraw(false);	// CPU��
	}

	// �v���C���[���̍X�V
	UpdatePlayerName();
}

//============================================================
//	CPU�̉����Z�̍X�V����
//============================================================
void CEntryManager::UpdateAddCpu(void)
{
	CInputKeyboard		*pKeyboard	= CManager::GetInstance()->GetKeyboard();			// �L�[�{�[�h
	CInputPad			*pPad		= CManager::GetInstance()->GetPad();				// �p�b�h
	CRetentionManager	*pRetention	= CManager::GetInstance()->GetRetentionManager();	// �f�[�^�ۑ����
	int nNumCpu = 0;	// CPU��
	bool bAdd = false;	// ���Z����
	bool bSub = false;	// ���Z����

	// ���Z�̑����ۑ�
	bAdd = pKeyboard->IsTrigger(DIK_D) || pKeyboard->IsTrigger(DIK_RIGHT) || pPad->IsTriggerAll(CInputPad::KEY_RIGHT);
	
	// ���Z�̑����ۑ�
	bSub = pKeyboard->IsTrigger(DIK_A) || pKeyboard->IsTrigger(DIK_LEFT) || pPad->IsTriggerAll(CInputPad::KEY_LEFT);

	if (bAdd && !bSub)
	{ // ���Z����̏ꍇ

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // �v���C���[�̍ő吔���J��Ԃ�

			if (!pRetention->IsEntry(nCntEntry))
			{ // �G���g���[���Ă��Ȃ��ꍇ

				// �G���g���[��o�^
				pRetention->SetEntry(nCntEntry, true, true);

				// ����������Ԃɂ���
				m_apJoin[nCntEntry]->SetPattern(JOIN_ON);

				// �F���Q�����̂��̂ɐݒ�
				m_apFrame[nCntEntry]->SetColor(COL_ENTRY);
				m_apNumber[nCntEntry]->SetColorTitle(COL_ENTRY);
				m_apNumber[nCntEntry]->GetMultiValue()->SetColor(COL_ENTRY);

				// �����𔲂���
				break;
			}
		}
	}

	if (bSub && !bAdd)
	{ // ���Z����̏ꍇ

		for (int nCntEntry = MAX_PLAYER - 1; nCntEntry >= 0; nCntEntry--)
		{ // �v���C���[�̍ő吔���J��Ԃ�

			if (pRetention->IsEntry(nCntEntry) && pRetention->IsAI(nCntEntry))
			{ // �G���g���[���Ă��銎�ACPU�̏ꍇ

				// �G���g���[������
				pRetention->SetEntry(nCntEntry, false, false);

				// ������������Ԃɂ���
				m_apJoin[nCntEntry]->SetPattern(JOIN_OFF);

				// �F���Q�����̂��̂ɐݒ�
				m_apFrame[nCntEntry]->SetColor(COL_UNENTRY);
				m_apNumber[nCntEntry]->SetColorTitle(COL_UNENTRY);
				m_apNumber[nCntEntry]->GetMultiValue()->SetColor(COL_UNENTRY);

				// �����𔲂���
				break;
			}
		}
	}

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		if (pRetention->IsAI(nCntEntry))
		{ // AI�̏ꍇ

			// AI�����Z
			nNumCpu++;
		}
	}

	// CPU����ݒ�
	m_pNumCpu->GetMultiValue()->SetNum(nNumCpu);
}

//============================================================
//	�v���C���[���̍X�V����
//============================================================
void CEntryManager::UpdatePlayerName(void)
{
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// �f�[�^�ۑ����

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		if (pRetention->IsEntry(nCntEntry))
		{ // �G���g���[���Ă���ꍇ

			if (pRetention->IsAI(nCntEntry))
			{ // AI�̏ꍇ

				// �Q���Җ���CPU�ɂ���
				m_apNumber[nCntEntry]->SetEnableDraw(false);
				m_apCpu[nCntEntry]->SetEnableDraw(true);
			}
			else
			{ // AI�ł͂Ȃ��ꍇ

				// �Q���Җ���PLAYER�ɂ���
				m_apCpu[nCntEntry]->SetEnableDraw(false);
				m_apNumber[nCntEntry]->SetEnableDraw(true);
			}
		}
		else
		{ // �G���g���[���Ă��Ȃ��ꍇ

			// �Q���Җ���PLAYER�ɂ���
			m_apCpu[nCntEntry]->SetEnableDraw(false);
			m_apNumber[nCntEntry]->SetEnableDraw(true);
		}
	}
}

//============================================================
//	�J�n�̍X�V����
//============================================================
void CEntryManager::UpdateStart(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h

	if (IsReadyOK(2))
	{ // �J�ډ\�̏ꍇ

		if (pKeyboard->IsTrigger(DIK_RETURN)
		||  pKeyboard->IsTrigger(DIK_SPACE)
		||  pPad->IsTriggerAll(CInputPad::KEY_START))
		{
			// ���[���ݒ��Ԃɂ���
			SetState(STATE_RULE);
		}

		// �J�n�\���̐F�𖾂邭����
		m_pStart->SetColor(COL_ENTRY);
	}
	else
	{ // �J�ډ\�ł͂Ȃ��ꍇ

		// �J�n�\���̐F���Â�����
		m_pStart->SetColor(COL_UNENTRY);
	}
}

//============================================================
//	���̍X�V����
//============================================================
void CEntryManager::UpdateArrow(void)
{
	for (int i = 0; i < MAX_ENTRY_ARROW; i++)
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
			m_fSinAlpha += arrow::ADD_SINROT;
			useful::NormalizeRot(m_fSinAlpha);	// �������K��

			// �����x���Z�ʂ����߂�
			fAddAlpha = (arrow::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinAlpha) - 1.0f);

			// ���F��ݒ�
			m_apArrow[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, arrow::BASIC_ALPHA + fAddAlpha));
		}
	}
}

//============================================================
//	�����ς݂��̔���擾����
//============================================================
bool CEntryManager::IsReadyOK(const int nNumEntry) const
{
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// �f�[�^�ۑ����
	bool bAllAI = true;	// �S�Q���҂�AI��

	if (pRetention->GetNumPlayer() < nNumEntry)
	{ // �Q���l���s��

		// �񏀔����
		return false;
	}

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		if (pRetention->IsEntry(nCntEntry))
		{ // �G���g���[���Ă���ꍇ

			switch (m_apJoin[nCntEntry]->GetPattern())
			{ // �����󋵂��Ƃ̏���
			case JOIN_OFF:

				// �񏀔����
				return false;

			case JOIN_ON:
				break;

			default:
				assert(false);
				break;
			}

			if (bAllAI)
			{ // AI�ł͂Ȃ��v���C���[����������Ă��Ȃ��ꍇ

				if (!pRetention->IsAI(nCntEntry))
				{ // AI�ł͂Ȃ��v���C���[�̏ꍇ

					// �S����AI�ł͂Ȃ���Ԃɂ���
					bAllAI = false;
				}
			}
		}
	}

	if (bAllAI)
	{ // �S����AI�̏ꍇ

		// �񏀔����
		return false;
	}

	// �J�ډ\��Ԃ�
	return true;
}
