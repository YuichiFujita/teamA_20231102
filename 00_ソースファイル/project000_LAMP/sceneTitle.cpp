//============================================================
//
//	�^�C�g����ʏ��� [sceneTitle.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneTitle.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "titleManager.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CTitleManager *CSceneTitle::m_pTitleManager = NULL;	// �^�C�g���}�l�[�W���[

//************************************************************
//	�q�N���X [CSceneTitle] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneTitle::CSceneTitle(const EMode mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneTitle::~CSceneTitle()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneTitle::Init(void)
{
	//--------------------------------------------------------
	//	�^�C�g���̏�����
	//--------------------------------------------------------
	// �^�C�g���}�l�[�W���[�̐���
	m_pTitleManager = CTitleManager::Create();
	if (m_pTitleManager == NULL)
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
	CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_ROTATE);	// �J��������]��Ԃɐݒ�
	CManager::GetInstance()->GetCamera()->SetDestRotate();					// �ڕW�ʒu��ݒ�
	CManager::GetInstance()->GetCamera()->SetEnableUpdate(false);			// �X�V���~

	// BGM�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GENERAL);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CSceneTitle::Uninit(void)
{
	// �^�C�g���}�l�[�W���[�̔j��
	if (FAILED(CTitleManager::Release(m_pTitleManager)))
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
void CSceneTitle::Update(void)
{
	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "�@[�f�o�b�O���]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");

	if (m_pTitleManager != NULL)
	{ // �g�p���̏ꍇ

		// �^�C�g���}�l�[�W���[�̍X�V
		m_pTitleManager->Update();
	}
	else { assert(false); }	// ��g�p��

	// �V�[���̍X�V
	CScene::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSceneTitle::Draw(void)
{

}

//============================================================
//	�^�C�g���}�l�[�W���[�擾����
//============================================================
CTitleManager *CSceneTitle::GetTitleManager(void)
{
	// �^�C�g���}�l�[�W���[��Ԃ�
	return m_pTitleManager;
}
