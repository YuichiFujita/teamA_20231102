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

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 14;	// ���ԃ��U���g�̗D�揇��

	// �t�F�[�h��{���
	namespace fade
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, -120.0f, 0.0f);	// �t�F�[�h�ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(SCREEN_SIZE.x, 1770.0f, 0.0f);	// �t�F�[�h�傫��
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CMiddleResultManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\middleResult000.png",	// ��������e�N�X�`��
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
	m_pFade = NULL;	// �t�F�[�h�̏��
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
	// �����o�ϐ���������
	m_pFade = NULL;	// �t�F�[�h�̏��

	// �t�F�[�h�̐���
	m_pFade = CObject2D::Create
	( // ����
		fade::POS,	// �ʒu
		fade::SIZE	// �傫��
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

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CMiddleResultManager::Update(void)
{
	// �t�F�[�h�̍X�V
	m_pFade->Update();
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
