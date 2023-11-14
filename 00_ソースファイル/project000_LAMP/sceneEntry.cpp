//============================================================
//
//	�G���g���[��ʏ��� [sceneEntry.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneEntry.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "entryManager.h"

#include "player.h"
#include "stage.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CEntryManager *CSceneEntry::m_pEntryManager = NULL;	// �G���g���[�}�l�[�W���[

//************************************************************
//	�q�N���X [CSceneEntry] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneEntry::CSceneEntry(const EMode mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneEntry::~CSceneEntry()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneEntry::Init(void)
{
	//--------------------------------------------------------
	//	�G���g���[�̏�����
	//--------------------------------------------------------
	// �G���g���[�}�l�[�W���[�̐���
	m_pEntryManager = CEntryManager::Create();
	if (m_pEntryManager == NULL)
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

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // �v���C���[�̑������J��Ԃ�

		// �v���C���[���o��
		CScene::GetPlayer(nCntPlayer)->SetSpawn();
	}

	// BGM�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GENERAL);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CSceneEntry::Uninit(void)
{
	// �G���g���[�}�l�[�W���[�̔j��
	if (FAILED(CEntryManager::Release(m_pEntryManager)))
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
void CSceneEntry::Update(void)
{
	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "�@[�f�o�b�O���]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");

	if (m_pEntryManager != NULL)
	{ // �g�p���̏ꍇ

		// �G���g���[�}�l�[�W���[�̍X�V
		m_pEntryManager->Update();
	}
	else { assert(false); }	// ��g�p��

	// �V�[���̍X�V
	CScene::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSceneEntry::Draw(void)
{

}

//============================================================
//	�G���g���[�}�l�[�W���[�擾����
//============================================================
CEntryManager *CSceneEntry::GetEntryManager(void)
{
	// �G���g���[�}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pEntryManager;
}
