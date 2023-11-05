//============================================================
//
//	�����L���O��ʏ��� [sceneRanking.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneRanking.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "rankingManager.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CRankingManager *CSceneRanking::m_pRankingManager = NULL;	// �����L���O�}�l�[�W���[

//************************************************************
//	�q�N���X [CSceneRanking] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneRanking::CSceneRanking(const EMode mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneRanking::~CSceneRanking()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneRanking::Init(void)
{
	//--------------------------------------------------------
	//	�����L���O�̏�����
	//--------------------------------------------------------
	// �����L���O�}�l�[�W���[�̐���
	m_pRankingManager = CRankingManager::Create();
	if (m_pRankingManager == NULL)
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
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GENERAL);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CSceneRanking::Uninit(void)
{
	// �����L���O�}�l�[�W���[�̔j��
	if (FAILED(CRankingManager::Release(m_pRankingManager)))
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
void CSceneRanking::Update(void)
{
	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "�@[�f�o�b�O���]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");

	if (m_pRankingManager != NULL)
	{ // �g�p���̏ꍇ

		// �����L���O�}�l�[�W���[�̍X�V
		m_pRankingManager->Update();
	}
	else { assert(false); }	// ��g�p��

	// �V�[���̍X�V
	CScene::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSceneRanking::Draw(void)
{

}

//============================================================
//	�����L���O�}�l�[�W���[�擾����
//============================================================
CRankingManager *CSceneRanking::GetRankingManager(void)
{
	// �����L���O�}�l�[�W���[��Ԃ�
	return m_pRankingManager;
}
