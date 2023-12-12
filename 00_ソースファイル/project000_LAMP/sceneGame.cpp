//============================================================
//
//	�Q�[����ʏ��� [sceneGame.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneGame.h"
#include "manager.h"
#include "sound.h"
#include "input.h"

#include "gameManager.h"
#include "timerManager.h"
#include "stage.h"
#include "pause.h"
#include "mask.h"
#include "player.h"
#include "block.h"
#include "obstacle.h"
#include "retentionManager.h"
#include "gameManager.h"
#include "editStageManager.h"
#include "ready.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	D3DXCOLOR COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);	// �}�X�N�F
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CGameManager	*CSceneGame::m_pGameManager	= NULL;	// �Q�[���}�l�[�W���[
CPause			*CSceneGame::m_pPause		= NULL;	// �|�[�Y

bool CSceneGame::m_bControlCamera = false;	// �J�����̑����
bool CSceneGame::m_bDrawUI = true;			// UI�̕`���
bool CSceneGame::m_bDrawPause = true;		// �|�[�Y�̕`���

//************************************************************
//	�q�N���X [CSceneGame] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneGame::CSceneGame(const EMode mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneGame::~CSceneGame()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneGame::Init(void)
{
	//--------------------------------------------------------
	//	�Z�b�g�A�b�v
	//--------------------------------------------------------
	// �u���b�N�̃Z�b�g�A�b�v
	CBlock::LoadSetup();

	// ��Q���̃Z�b�g�A�b�v
	CObstacle::LoadSetup();

	//--------------------------------------------------------
	//	��������
	//--------------------------------------------------------
	// �V�[���̏�����
	CScene::Init();		// �X�e�[�W�E�v���C���[�̐���

	// �Q�[���}�l�[�W���[�̐���
	m_pGameManager = CGameManager::Create();
	if (m_pGameManager == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �|�[�Y�̐���
	m_pPause = CPause::Create();
	if (m_pPause == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �}�X�N�̐��� (�v���C���[�̕Ǘ��\���p�|���S��)
	CMask::Create(SCREEN_CENT, SCREEN_SIZE, COL);

	//--------------------------------------------------------
	//	�����ݒ�
	//--------------------------------------------------------
	// �J������ݒ�
	CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_LOOKDOWN);	// �J�����������낵��Ԃɐݒ�

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // �v���C���[�����J��Ԃ�

		CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);	// �v���C���[���

		if (pPlayer != NULL)
		{ // �v���C���[�����݂���ꍇ

			// �v���C���[���o��
			pPlayer->SetSpawn();
			pPlayer->SetState(CPlayer::STATE_NONE);
		}
	}

	// �����낵�J�����̖ڕW�ʒu�̐ݒ�
	CManager::GetInstance()->GetCamera()->SetDestLookDown();

	// UI�̕`��󋵂�ݒ�
	SetEnableDrawUI(m_bDrawUI);

	// �|�[�Y�̕`��󋵂�ݒ�
	SetEnableDrawPause(m_bDrawPause);

	// BGM�̍Đ�
	int nRand = rand() % 3;

	switch (nRand)
	{
	case 0:
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);
		break;
	case 1:
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME_1);
		break;
	case 2:
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME_2);
		break;
	default:
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);
		break;
	}
	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CSceneGame::Uninit(void)
{
	// �Q�[���}�l�[�W���[�̔j��
	if (FAILED(CGameManager::Release(m_pGameManager)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �|�[�Y�̔j��
	if (FAILED(CPause::Release(m_pPause)))
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
void CSceneGame::Update(void)
{
#if _DEBUG

	if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F2))
	{
		// �G�f�B�b�g�󋵂̐ؑ�
		CGameManager::GetEditStage()->SwitchEnableEdit();
	}
	if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F3))
	{
		// UI�̕`��󋵂𔽓]
		SetEnableDrawUI((!m_bDrawUI) ? true : false);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F4))
	{
		// �|�[�Y�̕`��󋵂𔽓]
		SetEnableDrawPause((!m_bDrawPause) ? true : false);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F5))
	{
		// �J�����̑���󋵂𔽓]
		SetEnableControlCamera((!m_bControlCamera) ? true : false);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F6))
	{
		// �Q�[����ʂɑJ��
		CManager::GetInstance()->SetMode(CScene::MODE_GAME);
	}

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "�@[�f�o�b�O����]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F1]�F�f�o�b�O�\����ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F2]�F�G�f�B�b�g���[�h��ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F3]�FUI�`���ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F4]�F�|�[�Y�`���ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F5]�F�J���������ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F6]�F�Q�[���̃��g���C\n");

	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "�@[�f�o�b�O���]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");

#endif

	if (m_pGameManager != NULL)
	{ // �g�p���̏ꍇ

		// �Q�[���}�l�[�W���[�̍X�V
		m_pGameManager->Update();
	}
	else { assert(false); }	// ��g�p��

	if (m_pGameManager->GetState() != CGameManager::STATE_RESULT)
	{ // �Q�[���}�l�[�W���[�����U���g��ԈȊO�̏ꍇ

		if (m_pPause != NULL)
		{ // �g�p���̏ꍇ

			// �|�[�Y�̍X�V
			m_pPause->Update();
		}
		else { assert(false); }	// ��g�p��

		if (!m_pPause->IsPause())
		{ // �|�[�Y���ł͂Ȃ��ꍇ

			// �V�[���̍X�V
			CScene::Update();
		}

#if _DEBUG

		else
		{ // �|�[�Y���̏ꍇ

			if (CManager::GetInstance()->GetCamera()->GetState() == CCamera::STATE_CONTROL)
			{ // �J�����������Ԃ̏ꍇ

				// �J�����̍X�V
				CManager::GetInstance()->GetCamera()->Update();
			}
		}

#endif	// _DEBUG

	}
}

//============================================================
//	�`�揈��
//============================================================
void CSceneGame::Draw(void)
{

}

//============================================================
//	�Q�[���}�l�[�W���[�擾����
//============================================================
CGameManager *CSceneGame::GetGameManager(void)
{
	// �Q�[���}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pGameManager;
}

//============================================================
//	�|�[�Y�擾����
//============================================================
CPause *CSceneGame::GetPause(void)
{
	// �|�[�Y�̃|�C���^��Ԃ�
	return m_pPause;
}

//============================================================
//	UI�̕`��󋵂̐ݒ菈��
//============================================================
void CSceneGame::SetEnableDrawUI(const bool bDraw)
{
	// ������UI�̕`��󋵂�ݒ�
	m_bDrawUI = bDraw;

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // �v���C���[�����J��Ԃ�

		CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);	// �v���C���[���

		if (pPlayer != NULL)
		{ // �v���C���[�����݂���ꍇ

			// �v���C���[�̕`��󋵂�ݒ�
			pPlayer->SetEnableDrawUI(bDraw);
		}
	}
}

//============================================================
//	UI�̕`��󋵎擾����
//============================================================
bool CSceneGame::IsDrawUI(void)
{
	// UI�̕`��󋵂�Ԃ�
	return m_bDrawUI;
}

//============================================================
//	�|�[�Y�̕`��󋵂̐ݒ菈��
//============================================================
void CSceneGame::SetEnableDrawPause(const bool bDraw)
{
	// �����̃|�[�Y�̕`��󋵂�ݒ�
	m_bDrawPause = bDraw;

	// �|�[�Y�̕`��󋵂�ݒ�
	m_pPause->SetEnableDraw(m_pPause->IsPause());
}

//============================================================
//	�|�[�Y�̕`��󋵎擾����
//============================================================
bool CSceneGame::IsDrawPause(void)
{
	// �|�[�Y�̕`��󋵂�Ԃ�
	return m_bDrawPause;
}

//============================================================
//	�J�����̑���󋵂̐ݒ菈��
//============================================================
void CSceneGame::SetEnableControlCamera(const bool bControl)
{
	// �����̃J��������󋵂�ݒ�
	m_bControlCamera = bControl;

	// �J�����̑���󋵂�ݒ�
	if (bControl)
	{ // ���삷��󋵂̏ꍇ

		// �����ԂɕύX
		CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_CONTROL);
	}
	else
	{ // ���삵�Ȃ��󋵂̏ꍇ

		// �����낵��ԂɕύX
		CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_LOOKDOWN);
	}
}
