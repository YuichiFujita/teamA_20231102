//============================================================
//
//	���U���g��ʏ��� [sceneResult.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneResult.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "resultManager.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CResultManager *CSceneResult::m_pResultManager = NULL;	// ���U���g�}�l�[�W���[

//************************************************************
//	�q�N���X [CSceneResult] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneResult::CSceneResult(const EMode mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneResult::~CSceneResult()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneResult::Init(void)
{
	//--------------------------------------------------------
	//	���U���g�̏�����
	//--------------------------------------------------------
	// ���U���g�}�l�[�W���[�̐���
	m_pResultManager = CResultManager::Create();
	if (m_pResultManager == NULL)
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
	CManager::GetInstance()->GetCamera()->SetDestRotate();	// �ڕW�ʒu��ݒ�

	// BGM�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_RESULT);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CSceneResult::Uninit(void)
{
	// ���U���g�}�l�[�W���[�̔j��
	if (FAILED(CResultManager::Release(m_pResultManager)))
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
void CSceneResult::Update(void)
{
	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "�@[�f�o�b�O���]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");

	if (m_pResultManager != NULL)
	{ // �g�p���̏ꍇ

		// ���U���g�}�l�[�W���[�̍X�V
		m_pResultManager->Update();
	}
	else { assert(false); }	// ��g�p��

	// �V�[���̍X�V
	CScene::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSceneResult::Draw(void)
{

}

//============================================================
//	���U���g�}�l�[�W���[�擾����
//============================================================
CResultManager *CSceneResult::GetResultManager(void)
{
	// ���U���g�}�l�[�W���[��Ԃ�
	return m_pResultManager;
}
