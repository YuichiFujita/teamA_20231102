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
#include "player.h"
#include "flail.h"
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

	namespace flailArrow
	{
		const float	SPACE_EDGE		= 130.0f;	// ���̋�

		const POSGRID2		PART	= POSGRID2(MAX_ENTRY_ARROW, 1);		// �e�N�X�`��������
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(40.0f, 40.0f, 0.0f);	// �傫��
		const D3DXVECTOR3	POS		= D3DXVECTOR3(frame::POS.x - SPACE_EDGE, frame::POS.y, 0.0f);	// �ʒu
		const D3DXVECTOR3	SPACE	= D3DXVECTOR3(SPACE_EDGE * 2.0f, 0.0f, 0.0f);					// ��
	}

	namespace join
	{
		const POSGRID2	  PART	= POSGRID2(1, 2);	// �e�N�X�`��������
		const D3DXVECTOR3 POS	= D3DXVECTOR3(160.0f, 480.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(250.0f*1.3f, 42.0f*1.3f, 0.0f);		// �傫��
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// ��
	}

	namespace control
	{
		const float	ADD_ALPHA		= 0.008f;	// �����x�̉��Z��
		const float	ADD_SINROT		= 0.06f;	// �����x�ӂ�ӂ킳����ۂ̃T�C���J�[�u�������Z��
		const float	MAX_ADD_ALPHA	= 0.5f;		// �����x�̍ő���Z��
		const float	BASIC_ALPHA		= 0.95f;	// ��̓����x

		const D3DXVECTOR3	POS		= D3DXVECTOR3(SCREEN_CENT.x, 620.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(505.0f, 84.0f, 0.0f);			// �傫��
		const D3DXCOLOR		MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);	// �F
	}

	namespace start
	{
		const float	ADD_ALPHA		= 0.008f;	// �����x�̉��Z��
		const float	ADD_SINROT		= 0.06f;	// �����x�ӂ�ӂ킳����ۂ̃T�C���J�[�u�������Z��
		const float	MAX_ADD_ALPHA	= 0.5f;		// �����x�̍ő���Z��
		const float	BASIC_ALPHA		= 0.95f;	// ��̓����x

		const D3DXVECTOR3	POS		= D3DXVECTOR3(SCREEN_CENT.x, 380.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(573.0f, 99.0f, 0.0f);			// �傫��
		const D3DXCOLOR		MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);	// �F
	}

	namespace numcpu
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(SCREEN_CENT.x - 40.0f, 500.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3	SIZE_TITLE	= D3DXVECTOR3(306.0f * 0.9f, 98.0f * 0.9f, 0.0f);	// �^�C�g���傫��
		const D3DXVECTOR3	SIZE_VALUE	= D3DXVECTOR3(80.0f, 95.0f, 0.0f);			// �����傫��
		const D3DXVECTOR3	SPACE_TITLE	= D3DXVECTOR3(190.0f, 0.0f, 0.0f);			// �^�C�g����
		const D3DXVECTOR3	SPACE_VALUE	= VEC3_ZERO;								// ������
		const int			DIGIT		= 1;										// ����

		const float	INIT_SCALE	= 0.01f;	// �^�C�g�������g�嗦
		const float	ADD_SCALE	= 0.12f;	// �^�C�g�����Z�g�嗦
		const float	SET_SCALE	= 1.0f;		// �^�C�g���ݒ�g�嗦
	}

	namespace arrow
	{
		const float	ADD_ALPHA		= 0.02f;	// �����x�̉��Z��
		const float	ADD_SINROT		= 0.04f;	// �����x�ӂ�ӂ킳����ۂ̃T�C���J�[�u�������Z��
		const float	MAX_ADD_ALPHA	= 0.25f;	// �����x�̍ő���Z��
		const float	BASIC_ALPHA		= 0.95f;	// ��̓����x
		const float	SPACE_EDGE		= 240.0f;	// ���̋�

		const POSGRID2		PART	= POSGRID2(MAX_ENTRY_ARROW, 1);		// �e�N�X�`��������
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(80.0f, 80.0f, 0.0f);	// �傫��

		const D3DXVECTOR3	POS		= D3DXVECTOR3(SCREEN_CENT.x - SPACE_EDGE, numcpu::POS.y, 0.0f);	// �ʒu
		const D3DXVECTOR3	SPACE	= D3DXVECTOR3(SPACE_EDGE * 2.0f, 0.0f, 0.0f);					// ��
		const D3DXCOLOR		MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);		// �F
	}

	namespace bg
	{
		const float	ADD_SIZE_X	= 250.0f;	// ���T�C�Y�̉��Z��

		const D3DXVECTOR3	POS	= D3DXVECTOR3(0.0f, 440.0f, 0.0f);		// �ʒu
		const D3DXCOLOR		COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.625f);	// �F

		const D3DXVECTOR3	INIT_SIZE	= D3DXVECTOR3(0.0f, 280.0f, 0.0f);	// �����̑傫��
		const D3DXVECTOR3	DEST_SIZE	= D3DXVECTOR3(SCREEN_SIZE.x * 2.0f, 280.0f, 0.0f);	// �ڕW�̑傫��
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
	"data\\TEXTURE\\Readycheck.png",	// �Q���󋵃e�N�X�`��
	"data\\TEXTURE\\Arrow_Twin.png",	// ���e�N�X�`��
	"data\\TEXTURE\\cpu.png",			// CPU�e�N�X�`��
	"data\\TEXTURE\\mum_cpu.png",		// CPU���e�N�X�`��
	"data\\TEXTURE\\Arrow_Twin.png",	// �t���C�����e�N�X�`��
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
	m_pControl		= NULL;		// ����\���̏��
	m_pBG			= NULL;		// �w�i�̏��
	m_pStart		= NULL;		// �J�n�\���̏��
	m_pNumCpu		= NULL;		// CPU���\���̏��
	m_fScale		= 0.0f;		// �g�嗦
	m_fSinStartAlpha	= 0.0f;	// �J�n�\���̓�������
	m_fSinControlAlpha	= 0.0f;	// ����\���̓�������
	m_fSinArrowAlpha	= 0.0f;	// ���\���̓�������

	m_stateEntry	= STATE_ENTRY_NONE_JOIN;	// �G���g���[���
	m_state			= STATE_ENTRY;				// ���
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

	m_pRuleManager	= NULL;	// �G���g���[���[���̏��
	m_pControl		= NULL;	// ����\���̏��
	m_pBG			= NULL;	// �w�i�̏��
	m_pStart		= NULL;	// �J�n�\���̏��
	m_pNumCpu		= NULL;	// CPU���\���̏��
	m_fScale		= 1.0f;	// �g�嗦

	m_fSinStartAlpha	= -HALF_PI;	// �J�n�\���̓�������
	m_fSinControlAlpha	= -HALF_PI;	// ����\���̓�������
	m_fSinArrowAlpha	= -HALF_PI;	// ���\���̓�������
	m_stateEntry	= STATE_ENTRY_NONE_JOIN;	// �G���g���[���
	m_state			= STATE_ENTRY;				// ���

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

		// �e�N�X�`����o�^�E����
		m_apCpu[nCntEntry]->BindTexture(mc_apTextureFile[TEXTURE_CPU]);

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

		// �����`���OFF�ɂ���
		m_apJoin[nCntEntry]->SetEnableDraw(false);

		for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
		{ // ���̑������J��Ԃ�

			// �t���C�����̐���
			m_apFlailArrow[nCntEntry][nCntArrow] = CAnim2D::Create
			( // ����
				flailArrow::PART.x,	// �e�N�X�`����������
				flailArrow::PART.y,	// �e�N�X�`���c������
				flailArrow::POS + ((float)nCntArrow * flailArrow::SPACE) + ((float)nCntEntry * frame::SPACE),	// �ʒu
				flailArrow::SIZE,	// �傫��
				VEC3_ZERO,			// ����
				XCOL_WHITE			// �F
			);
			if (m_apFlailArrow[nCntEntry][nCntArrow] == NULL)
			{ // �����Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �e�N�X�`����o�^�E����
			m_apFlailArrow[nCntEntry][nCntArrow]->BindTexture(mc_apTextureFile[TEXTURE_FLAILARROW]);

			// �D�揇�ʂ�ݒ�
			m_apFlailArrow[nCntEntry][nCntArrow]->SetPriority(PRIORITY);

			// �p�^�[����ݒ�
			m_apFlailArrow[nCntEntry][nCntArrow]->SetPattern(nCntArrow);

			// �����`���OFF�ɂ���
			m_apFlailArrow[nCntEntry][nCntArrow]->SetEnableDraw(false);
		}
	}

	// ����\���̐���
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

	// �w�i�̐���
	m_pBG = CObject2D::Create
	( // ����
		bg::POS,		// �ʒu
		bg::INIT_SIZE,	// �傫��
		VEC3_ZERO,		// ����
		bg::COL			// �F
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
		start::SIZE * numcpu::INIT_SCALE,	// �傫��
		VEC3_ZERO,	// ����
		XCOL_AWHITE	// �F
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
		CValue::TEXTURE_ZERO,	// �����e�N�X�`��
		numcpu::DIGIT,			// ����
		numcpu::POS,			// �ʒu
		numcpu::SPACE_TITLE,	// �s��
		numcpu::SPACE_VALUE,	// �����s��
		numcpu::SIZE_TITLE * numcpu::INIT_SCALE,	// �^�C�g���傫��
		numcpu::SIZE_VALUE * numcpu::INIT_SCALE		// �����傫��
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

		for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
		{ // ���̑������J��Ԃ�

			// �t���C�����̏I��
			m_apFlailArrow[nCntEntry][nCntArrow]->Uninit();
		}
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

		// �t���C���̍X�V
		UpdateFlail();

		switch (m_stateEntry)
		{ // �G���g���[��Ԃ��Ƃ̏���
		case STATE_ENTRY_NONE_JOIN:

			if (IsReadyOK(1))
			{ // �S���������ς݂̏ꍇ

				// CPU���o�̊J�n�ݒ�
				SetCpuObject(true);

				// CPU���w�i�̎����`���ON�ɂ���
				m_pBG->SetEnableDraw(true);

				// CPU���w�i�̉��o��Ԃɂ���
				m_stateEntry = STATE_ENTRY_CPU_BG;
			}

			break;

		case STATE_ENTRY_CPU_BG:

			// CPU���w�i�̉��o�̍X�V
			UpdateCpuBG();

			break;

		case STATE_ENTRY_CPU_UI:

			// CPU��UI�̉��o�̍X�V
			UpdateCpuUI();

			break;

		case STATE_ENTRY_NUMCPU:

			// CPU�̍X�V
			UpdateCpu();

			// �J�nUI�̍X�V
			UpdateStartUI();

			break;

		default:
			assert(false);
			break;
		}

		// �J�n�̍X�V
		UpdateStart();

		// �v���C���[���̍X�V
		UpdatePlayerName();

		// ����UI�̍X�V
		UpdateControlUI();

		// UI�I�u�W�F�N�g�̑S�X�V
		UpdateUIAll();

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
		{ // �G���g���[�҂�����ꍇ

			if (pRetention->IsAI(nCntEntry))
			{ // AI�̃G���g���[�Ȃ�㏑���\

				bEntry = true;
			}
			else
			{ // �l�Ԃ̃G���g���[�Ȃ�㏑���s��

				bEntry = false;
			}

			// �����\���̎����`���ON�ɂ���
			m_apJoin[nCntEntry]->SetEnableDraw(true);
		}
		else
		{ // �G���g���[�҂����Ȃ��ꍇ

			// �����\���̎����`���OFF�ɂ���
			m_apJoin[nCntEntry]->SetEnableDraw(false);
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

				for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
				{ // ���̑������J��Ԃ�

					// �t���C�����̎����`���ON�ɂ���
					m_apFlailArrow[nCntEntry][nCntArrow]->SetEnableDraw(true);
				}

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// �I�𑀍쉹00
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

					for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
					{ // ���̑������J��Ԃ�

						// �t���C�����̎����`���OFF�ɂ���
						m_apFlailArrow[nCntEntry][nCntArrow]->SetEnableDraw(false);
					}

					// �T�E���h�̍Đ�
					CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// �I�𑀍쉹01
				}
				else if (pPad->IsTrigger(CInputPad::KEY_B, nCntEntry))
				{
					// �G���g���[������
					pRetention->SetEntry(nCntEntry, false, false);

					// �F���Q�����̂��̂ɐݒ�
					m_apFrame[nCntEntry]->SetColor(COL_UNENTRY);
					m_apNumber[nCntEntry]->SetColorTitle(COL_UNENTRY);
					m_apNumber[nCntEntry]->GetMultiValue()->SetColor(COL_UNENTRY);

					for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
					{ // ���̑������J��Ԃ�

						// �t���C�����̎����`���OFF�ɂ���
						m_apFlailArrow[nCntEntry][nCntArrow]->SetEnableDraw(false);
					}

					// �T�E���h�̍Đ�
					CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
				}

				break;

			case JOIN_ON:

				if (pPad->IsTrigger(CInputPad::KEY_B, nCntEntry))
				{
					// ������������Ԃɂ���
					m_apJoin[nCntEntry]->SetPattern(JOIN_OFF);

					for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
					{ // ���̑������J��Ԃ�
						
						// �t���C�����̎����`���ON�ɂ���
						m_apFlailArrow[nCntEntry][nCntArrow]->SetEnableDraw(true);
					}

					// �T�E���h�̍Đ�
					CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
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
//	�t���C���̍X�V����
//============================================================
void CEntryManager::UpdateFlail(void)
{
	// �|�C���^��錾
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// �f�[�^�ۑ����
	CInputPad *pPad = CManager::GetInstance()->GetPad();	// �p�b�h

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		if (!pRetention->IsEntry(nCntEntry))
		{ // �G���g���[���Ă��Ȃ��ꍇ

			break;
		}

		if (m_apJoin[nCntEntry]->GetPattern() != JOIN_OFF)
		{ // �������o���Ă���ꍇ

			break;
		}

		// �|�C���^��錾
		CPlayer *pPlayer = CScene::GetPlayer(nCntEntry);	// �v���C���[���
		CFlail *pFlail = pPlayer->GetFlail();				// �t���C�����

		// �ϐ���錾
		int nType = pFlail->GetType();	// �t���C�����

		if (pPad->IsTrigger(CInputPad::KEY_RIGHT))
		{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

			// �E�ɑI�������炷
			nType = (nType + (CFlail::FLAIL_MAX - 1)) % CFlail::FLAIL_MAX;

			// �t���C����ނ𔽉f
			pFlail->SetType(nType);
			pRetention->SetFlail(nCntEntry, nType);

			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
		}
		if (pPad->IsTrigger(CInputPad::KEY_LEFT))
		{ // ���ړ��̑��삪�s��ꂽ�ꍇ

			// ���ɑI�������炷
			nType = (nType + 1) % CFlail::FLAIL_MAX;

			// �t���C����ނ𔽉f
			pFlail->SetType(nType);
			pRetention->SetFlail(nCntEntry, nType);

			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
		}
	}
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
		m_pBG->SetEnableDraw(true);		// �w�i
		m_pStart->SetEnableDraw(true);	// �J�n�\��
		m_pNumCpu->SetEnableDraw(true);	// CPU��

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

		// �����̖�������Ԃɂ���
		m_stateEntry = STATE_ENTRY_NONE_JOIN;
	}
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

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00

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

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00

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
//	����UI�̍X�V����
//============================================================
void CEntryManager::UpdateControlUI(void)
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
//	�J�nUI�̍X�V����
//============================================================
void CEntryManager::UpdateStartUI(void)
{
	if (!IsReadyOK(2))
	{ // ��l�ȏオ����OK���Ă��Ȃ��ꍇ

		// �֐��𔲂���
		return;
	}

	// �ϐ���錾
	D3DXCOLOR colStart = m_pStart->GetColor();	// �J�n�\���F

	if (colStart.a < start::MIN_COL.a)
	{ // �����x���Œ�����Ⴂ�ꍇ

		// �����x�����Z
		colStart.a += start::ADD_ALPHA;

		if (colStart.a > start::MIN_COL.a)
		{ // �����x�����߂����ꍇ

			// �����x��␳
			colStart.a = start::MIN_COL.a;
		}

		// ����\���F��ݒ�
		m_pStart->SetColor(colStart);
	}
	else
	{ // �����x���Œ���ȏ�̏ꍇ

		// �ϐ���錾
		float fAddAlpha = 0.0f;	// �����x�̉��Z��

		// �����x���グ��
		m_fSinStartAlpha += start::ADD_SINROT;
		useful::NormalizeRot(m_fSinStartAlpha);	// �������K��

		// �����x���Z�ʂ����߂�
		fAddAlpha = (start::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinStartAlpha) - 1.0f);

		// ����\���F��ݒ�
		m_pStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, start::BASIC_ALPHA + fAddAlpha));
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

		if (m_stateEntry == STATE_ENTRY_NUMCPU)
		{ // CPU���̐ݒ��Ԃ̏ꍇ

			if (pKeyboard->IsTrigger(DIK_RETURN)
			||  pKeyboard->IsTrigger(DIK_SPACE)
			||  pPad->IsTriggerAll(CInputPad::KEY_START))
			{
				// ���[���ݒ��Ԃɂ���
				SetState(STATE_RULE);

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// �I�𑀍쉹01
			}
		}

		// �J�n�\���̐F�𖾂邭����
		D3DXCOLOR col = m_pStart->GetColor();
		m_pStart->SetColor(D3DXCOLOR(COL_ENTRY.r, COL_ENTRY.g, COL_ENTRY.b, col.a));
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
//	CPU���w�i�̉��o�̍X�V����
//============================================================
void CEntryManager::UpdateCpuBG(void)
{
	// �ϐ���錾
	D3DXVECTOR3 sizeBG = m_pBG->GetVec3Sizing();	// �w�i�̑傫��

	// �傫�������Z
	sizeBG.x += bg::ADD_SIZE_X;

	if (sizeBG.x >= bg::DEST_SIZE.x)
	{ // �ڕW�̑傫���ȏ�̏ꍇ

		// �傫����␳
		sizeBG.x = bg::DEST_SIZE.x;

		// �����`���ON�ɂ���
		m_pStart->SetEnableDraw(true);	// �J�n�\��
		m_pNumCpu->SetEnableDraw(true);	// CPU��

		// UI���o�̊g�嗦��ݒ�
		m_fScale = numcpu::INIT_SCALE;

		// CPU��UI�̉��o��Ԃɂ���
		m_stateEntry = STATE_ENTRY_CPU_UI;
	}

	// �w�i�̑傫���𔽉f
	m_pBG->SetVec3Sizing(sizeBG);
}

//============================================================
//	CPU��UI�̉��o�̍X�V����
//============================================================
void CEntryManager::UpdateCpuUI(void)
{
	// �g�嗦�����Z
	m_fScale += numcpu::ADD_SCALE;

	if (m_fScale < numcpu::SET_SCALE)
	{ // �܂��傫���Ȃ�ꍇ

		// �傫���𔽉f
		m_pStart->SetVec3Sizing(start::SIZE * m_fScale);	// �J�n�\��
		m_pNumCpu->SetScalingTitle(numcpu::SIZE_TITLE * m_fScale);	// CPU���^�C�g��
		m_pNumCpu->GetMultiValue()->SetVec3Sizing(numcpu::SIZE_VALUE * m_fScale);	// CPU���l
	}
	else
	{ // �傫���Ȃ�؂����ꍇ

		// �傫���𔽉f
		m_pStart->SetVec3Sizing(start::SIZE);	// �J�n�\��
		m_pNumCpu->SetScalingTitle(numcpu::SIZE_TITLE);	// CPU���^�C�g��
		m_pNumCpu->GetMultiValue()->SetVec3Sizing(numcpu::SIZE_VALUE);	// CPU���l

		// �g�嗦��������
		m_fScale = 1.0f;

		// CPU���̕ύX��Ԃɂ���
		m_stateEntry = STATE_ENTRY_NUMCPU;
	}
}

//============================================================
//	UI�I�u�W�F�N�g�̑S�X�V����
//============================================================
void CEntryManager::UpdateUIAll(void)
{
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

		for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
		{ // ���̑������J��Ԃ�

			// �t���C�����̍X�V
			m_apFlailArrow[nCntEntry][nCntArrow]->Update();
		}
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
}

//============================================================
//	CPU���o�̐ݒ菈��
//============================================================
void CEntryManager::SetCpuObject(const bool bStart)
{
	if (bStart)
	{ // �J�n���̐ݒ�̏ꍇ

		// �����`���OFF�ɂ���
		m_pBG->SetEnableDraw(false);		// �w�i
		m_pStart->SetEnableDraw(false);		// �J�n�\��
		m_pNumCpu->SetEnableDraw(false);	// CPU��

		// �傫����ݒ�
		m_pBG->SetVec3Sizing(bg::INIT_SIZE);												// �w�i
		m_pStart->SetVec3Sizing(start::SIZE * numcpu::INIT_SCALE);							// �J�n�\��
		m_pNumCpu->SetScalingTitle(numcpu::SIZE_TITLE * numcpu::INIT_SCALE);				// CPU���^�C�g��
		m_pNumCpu->GetMultiValue()->SetVec3Sizing(numcpu::SIZE_VALUE * numcpu::INIT_SCALE);	// CPU���l

		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // ���̑������J��Ԃ�
	
			// ���̐F��ݒ�
			m_apArrow[i]->SetColor(XCOL_AWHITE);
		}

		// �J�n�\���̐F��ݒ�
		m_pStart->SetColor(XCOL_AWHITE);
	}
	else
	{ // �X�L�b�v���̐ݒ�̏ꍇ

		// �g�嗦��������
		m_fScale = 1.0f;

		// �����`���ON�ɂ���
		m_pBG->SetEnableDraw(true);		// �w�i
		m_pStart->SetEnableDraw(true);	// �J�n�\��
		m_pNumCpu->SetEnableDraw(true);	// CPU��

		// �傫����ݒ�
		m_pBG->SetVec3Sizing(bg::DEST_SIZE);							// �w�i
		m_pStart->SetVec3Sizing(start::SIZE);							// �J�n�\��
		m_pNumCpu->SetScalingTitle(numcpu::SIZE_TITLE);					// CPU���^�C�g��
		m_pNumCpu->GetMultiValue()->SetVec3Sizing(numcpu::SIZE_VALUE);	// CPU���l

		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // ���̑������J��Ԃ�

			// ���̐F��ݒ�
			m_apArrow[i]->SetColor(XCOL_AWHITE);
		}

		// �J�n�\���̐F��ݒ�
		m_pStart->SetColor(XCOL_AWHITE);
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
