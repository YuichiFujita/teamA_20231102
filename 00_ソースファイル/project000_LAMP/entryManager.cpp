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
#include "retentionManager.h"
#include "fade.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 14;	// �G���g���[�̗D�揇��

	const D3DXCOLOR COL_ENTRY	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �Q�����J���[
	const D3DXCOLOR COL_UNENTRY	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// ��Q�����J���[

	namespace number
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(165.0f, 90.0f, 0.0f);		// �ʒu
		const D3DXVECTOR3	SIZE_TITLE	= D3DXVECTOR3(242.0f, 107.0f, 0.0f);	// �^�C�g���傫��
		const D3DXVECTOR3	SIZE_VALUE	= D3DXVECTOR3(80.0f, 90.0f, 0.0f);		// �����傫��
		const D3DXVECTOR3	SPACE_POS	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// ����UI���m�̋�
		const D3DXVECTOR3	SPACE_TITLE	= D3DXVECTOR3(100.0f, 5.0f, 0.0f);		// �^�C�g����
		const D3DXVECTOR3	SPACE_VALUE	= VEC3_ZERO;							// ������
		const int			DIGIT		= 1;									// ����
	}

	namespace frame
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(160.0f, 360.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(250.0f, 440.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// ��
	}

	namespace control
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, 630.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(505.0f, 84.0f, 0.0f);			// �傫��
	}

	namespace start
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(1100.0f, 600.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(300.0f, 180.0f, 0.0f);	// �傫��
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CEntryManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\entry_player.png",	// PLAYER�e�N�X�`��
	"data\\TEXTURE\\entry_flame.png",	// �t���[���e�N�X�`��
	"data\\TEXTURE\\entry002.png",	// ����\���e�N�X�`��
	"data\\TEXTURE\\entry003.png",	// �J�n�\���e�N�X�`��
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
	memset(&m_apFrame[0],	0, sizeof(m_apFrame));	// �v���C���[�t���[���̏��
	m_pControl	= NULL;	// ����\���̏��
	m_pStart	= NULL;	// �J�n�\���̏��
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
	memset(&m_apFrame[0],	0, sizeof(m_apFrame));	// �v���C���[�t���[���̏��
	m_pControl	= NULL;	// ����\���̏��
	m_pStart	= NULL;	// �J�n�\���̏��

	// �Q�[������������
	CManager::GetInstance()->GetRetentionManager()->InitGame();

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

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

	// �v���C�l����������
	CManager::GetInstance()->GetRetentionManager()->SetNumPlayer(0);

	// �G���g���[��������
	CManager::GetInstance()->GetRetentionManager()->AllSetEnableEntry(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CEntryManager::Uninit(void)
{
	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		// �v���C���[�i���o�[�̏I��
		m_apNumber[nCntEntry]->Uninit();

		// �v���C���[�t���[���̏I��
		m_apFrame[nCntEntry]->Uninit();
	}

	// ����\���̏I��
	m_pControl->Uninit();

	// �J�n�\���̏I��
	m_pStart->Uninit();

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

	// �G���g���[�̍X�V
	UpdateEntry();

	// �J�n�̍X�V
	UpdateStart();

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		// �v���C���[�i���o�[�̍X�V
		m_apNumber[nCntEntry]->Update();

		// �v���C���[�t���[���̍X�V
		m_apFrame[nCntEntry]->Update();
	}

	// ����\���̍X�V
	m_pControl->Update();

	// �J�n�\���̍X�V
	m_pStart->Update();
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

	// �ϐ���錾
	int nNumPlayer = pRetention->GetNumPlayer();	// �v���C�l��

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		if (pPad->IsTrigger(CInputPad::KEY_A, nCntEntry))
		{
			if (!pRetention->IsEntry(nCntEntry))
			{ // �G���g���[���Ă��Ȃ��ꍇ

				// �G���g���[��o�^
				pRetention->SetEnableEntry(nCntEntry, true);
				nNumPlayer++;	// �G���g���[�����Z

				// �F���Q�����̂��̂ɐݒ�
				m_apFrame[nCntEntry]->SetColor(COL_ENTRY);
				m_apNumber[nCntEntry]->SetColorTitle(COL_ENTRY);
				m_apNumber[nCntEntry]->GetMultiValue()->SetColor(COL_ENTRY);
			}
		}
		else if (pPad->IsTrigger(CInputPad::KEY_B, nCntEntry))
		{
			if (pRetention->IsEntry(nCntEntry))
			{ // �G���g���[���Ă���ꍇ

				// �G���g���[������
				pRetention->SetEnableEntry(nCntEntry, false);
				nNumPlayer--;	// �G���g���[�����Z

				// �F���Q�����̂��̂ɐݒ�
				m_apFrame[nCntEntry]->SetColor(COL_UNENTRY);
				m_apNumber[nCntEntry]->SetColorTitle(COL_UNENTRY);
				m_apNumber[nCntEntry]->GetMultiValue()->SetColor(COL_UNENTRY);
			}
		}
	}

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[��]�F%d\n", nNumPlayer);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[1]�F%s\n", pRetention->IsEntry(0) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[2]�F%s\n", pRetention->IsEntry(1) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[3]�F%s\n", pRetention->IsEntry(2) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�G���g���[4]�F%s\n", pRetention->IsEntry(3) ? "true" : "false");

	// �v���C�l����ݒ�
	pRetention->SetNumPlayer(nNumPlayer);
}

//============================================================
//	�J�n�̍X�V����
//============================================================
void CEntryManager::UpdateStart(void)
{
	// �|�C���^��錾
	CInputPad *pPad = CManager::GetInstance()->GetPad();	// �p�b�h

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		if (pPad->IsTrigger(CInputPad::KEY_START, nCntEntry))
		{
			if (CManager::GetInstance()->GetRetentionManager()->GetNumPlayer() >= 2)
			{ // �G���g���[������l�ȏ�̏ꍇ

				// �V�[���̐ݒ�
				CManager::GetInstance()->SetScene(CScene::MODE_GAME);	// �Q�[�����
			}
		}
	}
}
