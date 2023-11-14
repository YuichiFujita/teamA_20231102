//============================================================
//
//	�`���[�g���A����ʏ��� [sceneTutorial.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneTutorial.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "tutorialManager.h"

#include "player.h"
#include "stage.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CTutorialManager *CSceneTutorial::m_pTutorialManager = NULL;	// �`���[�g���A���}�l�[�W���[

//************************************************************
//	�q�N���X [CSceneTutorial] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneTutorial::CSceneTutorial(const EMode mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneTutorial::~CSceneTutorial()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneTutorial::Init(void)
{
	//--------------------------------------------------------
	//	�`���[�g���A���̏�����
	//--------------------------------------------------------
	// �`���[�g���A���}�l�[�W���[�̐���
	m_pTutorialManager = CTutorialManager::Create();
	if (m_pTutorialManager == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �V�[���̏�����
	CScene::Init();

	//--------------------------------------------------------
	//	�����ݒ�
	//--------------------------------------------------------
	// �J������ݒ�
	CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_LOOKDOWN);	// �J�����������낵��Ԃɐݒ�
	CManager::GetInstance()->GetCamera()->SetDestLookDown();					// �J�������̏�����

	// BGM�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TUTORIAL);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CSceneTutorial::Uninit(void)
{
	// �`���[�g���A���}�l�[�W���[�̔j��
	if (FAILED(CTutorialManager::Release(m_pTutorialManager)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �V�[���̏I��
	CScene::Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CSceneTutorial::Update(void)
{
	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "�@[�f�o�b�O���]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");

	if (m_pTutorialManager != NULL)
	{ // �g�p���̏ꍇ

		// �`���[�g���A���}�l�[�W���[�̍X�V
		m_pTutorialManager->Update();
	}
	else { assert(false); }	// ��g�p��

	// �V�[���̍X�V
	CScene::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSceneTutorial::Draw(void)
{

}

//============================================================
//	�`���[�g���A���}�l�[�W���[�擾����
//============================================================
CTutorialManager *CSceneTutorial::GetTutorialManager(void)
{
	// �`���[�g���A���}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pTutorialManager;
}
