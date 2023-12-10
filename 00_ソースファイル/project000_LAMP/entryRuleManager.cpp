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

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 13;	// �G���g���[�̗D�揇��

	const D3DXCOLOR COL_RULE	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �Q�����J���[
	const D3DXCOLOR COL_UNRULE	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// ��Q�����J���[

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
const char *CEntryRuleManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\entry_player.png",	// PLAYER�e�N�X�`��
	"data\\TEXTURE\\entry_flame.png",	// �t���[���e�N�X�`��
	"data\\TEXTURE\\entry002.png",	// ����\���e�N�X�`��
	"data\\TEXTURE\\entry003.png",	// �J�n�\���e�N�X�`��
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
	m_pControl	= NULL;	// ����\���̏��
	m_pStart	= NULL;	// �J�n�\���̏��
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
	m_pControl	= NULL;	// ����\���̏��
	m_pStart	= NULL;	// �J�n�\���̏��

	// �Q�[������������
	CManager::GetInstance()->GetRetentionManager()->InitGame();

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
HRESULT CEntryRuleManager::Uninit(void)
{
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
void CEntryRuleManager::Update(void)
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::FADE_NONE)
	{ // �t�F�[�h���̏ꍇ

		// �����𔲂���
		return;
	}

	// ����\���̍X�V
	m_pControl->Update();

	// �J�n�\���̍X�V
	m_pStart->Update();
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
