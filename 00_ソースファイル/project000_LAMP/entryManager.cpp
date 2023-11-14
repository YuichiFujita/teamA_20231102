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
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 14;	// �G���g���[�̗D�揇��

	namespace lesson
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(640.0f, 580.0f, 0.0f);	// ���b�X���\���̈ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(1000.0f, 270.0f, 0.0f);	// ���b�X���\���̈ʒu
	}

	namespace control
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(1140.0f, 85.0f, 0.0f);	// ������@�̈ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(264.0f, 150.0f, 0.0f);	// ������@�̈ʒu
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CEntryManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\entry000.png",	// ��������e�N�X�`��
};
const char *CEntryManager::mc_apLessonTextureFile[] =	// ���b�X���e�N�X�`���萔
{
	"data\\TEXTURE\\lesson000.png",	// �W�����v�����e�N�X�`��
	"data\\TEXTURE\\lesson001.png",	// �X���C�f�B���O�����e�N�X�`��
	"data\\TEXTURE\\lesson002.png",	// �Ǒ�������e�N�X�`��
	"data\\TEXTURE\\lesson003.png",	// �ǃW�����v�����e�N�X�`��
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
	m_pExplain = NULL;	// �����\���̏��
	m_pControl = NULL;	// ��������̏��
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
	m_pExplain = NULL;	// �����\���̏��
	m_pControl = NULL;	// ��������̏��

	// �����\���̐���
	m_pExplain = CObject2D::Create
	( // ����
		lesson::POS,	// �ʒu
		lesson::SIZE	// �傫��
	);
	if (m_pExplain == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pExplain->SetPriority(PRIORITY);

	// ��������̐���
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

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CEntryManager::Uninit(void)
{
	// �����\���̏I��
	m_pExplain->Uninit();

	// ��������̏I��
	m_pControl->Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CEntryManager::Update(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h

	if (pKeyboard->IsTrigger(DIK_RETURN) || pPad->IsTrigger(CInputPad::KEY_START))
	{
		// �V�[���̐ݒ�
		CManager::GetInstance()->SetScene(CScene::MODE_TITLE);	// �^�C�g�����
	}

	// �����\���̍X�V
	m_pExplain->Update();

	// ��������̍X�V
	m_pControl->Update();
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
