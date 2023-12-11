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
	const int PRIORITY = 14;	// �G���g���[�̗D�揇��

	namespace fade
	{
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �������F
		const D3DXCOLOR SET_COL		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);	// �ݒ�F

		const float	ADD_ALPHA	= 0.025f;	// �����x�̉��Z��
		const float	SUB_ALPHA	= 0.025f;	// �����x�̌��Z��
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CEntryRuleManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	NULL,	// �e�N�X�`��
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
	m_pFade	= NULL;			// �t�F�[�h�̏��
	m_state	= STATE_FADEIN;	// ���
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
	m_pFade	= NULL;			// �t�F�[�h�̏��
	m_state	= STATE_FADEIN;	// ���

	// ����\���̐���
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

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CEntryRuleManager::Uninit(void)
{
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
	case STATE_FADEIN:

		// �t�F�[�h�C���̍X�V
		UpdateFadeIn();

		break;

	case STATE_WAIT:



		break;

	case STATE_FADEOUT:

		// �t�F�[�h�A�E�g�̍X�V
		UpdateFadeOut();

		break;

	case STATE_ENTRYBACK:

		// �G���g���[��Ԃɖ߂�
		CSceneEntry::GetEntryManager()->SetState(CEntryManager::STATE_ENTRY);

		// �֐��𔲂���
		return;

	default:
		assert(false);
		break;
	}

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

		// �ҋ@��Ԃɂ���
		m_state = STATE_WAIT;
	}

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

	// �����x�𔽉f
	m_pFade->SetColor(colFade);
}
