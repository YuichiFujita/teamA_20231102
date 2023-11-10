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
#include "player.h"

//************************************************************
//	�}�N����`
//************************************************************

#if _DEBUG

#define TIME_LIMIT	(0)	// ��������

#else

#define TIME_LIMIT	(240)	// ��������

#endif

#define TIME_POS		(D3DXVECTOR3(710.0f, 50.0f, 0.0f))	// �^�C�}�[�ʒu
#define TIME_VAL_SIZE	(D3DXVECTOR3(72.0f, 96.0f, 0.0f))	// �^�C�}�[�����傫��
#define TIME_PART_SIZE	(D3DXVECTOR3(42.0f, 96.0f, 0.0f))	// �^�C�}�[��؂�傫��
#define TIME_VAL_SPACE	(D3DXVECTOR3(TIME_VAL_SIZE.x  * 0.85f, 0.0f, 0.0f))	// �^�C�}�[������
#define TIME_PART_SPACE	(D3DXVECTOR3(TIME_PART_SIZE.x * 0.85f, 0.0f, 0.0f))	// �^�C�}�[��؂��

#define SCO_POS		(D3DXVECTOR3(825.0f, 50.0f, 0.0f))		// �X�R�A�ʒu
#define SCO_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// �X�R�A�傫��
#define SCO_SPACE	(D3DXVECTOR3(SCO_SIZE.x, 0.0f, 0.0f))	// �X�R�A��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CGameManager	*CSceneGame::m_pGameManager  = NULL;	// �Q�[���}�l�[�W���[
CTimerManager	*CSceneGame::m_pTimerManager = NULL;	// �^�C�}�[�}�l�[�W���[
CPause	*CSceneGame::m_pPause	= NULL;					// �|�[�Y

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
	//	��������
	//--------------------------------------------------------
	// �^�C�}�[�}�l�[�W���[�̐���
	m_pTimerManager = CTimerManager::Create
	( // ����
		CTimerManager::TIME_SEC,	// �ݒ�^�C��
		TIME_LIMIT,					// ��������
		TIME_POS,					// �ʒu
		TIME_VAL_SIZE,				// �����̑傫��
		TIME_PART_SIZE,				// ��؂�̑傫��
		TIME_VAL_SPACE,				// �����̋�
		TIME_PART_SPACE				// ��؂�̋�
	);
	if (m_pTimerManager == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���S�̎����`���ON�ɂ���
	m_pTimerManager->SetEnableLogoDraw(true);

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

	//--------------------------------------------------------
	//	�����ݒ�
	//--------------------------------------------------------
	// �J������ݒ�
	CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_LOOKDOWN);	// �J�����������낵��Ԃɐݒ�

	// �v���C���[���o��
	CScene::GetPlayer()->SetSpawn();

	// UI�̕`��󋵂�ݒ�
	SetEnableDrawUI(m_bDrawUI);

	// �|�[�Y�̕`��󋵂�ݒ�
	SetEnableDrawPause(m_bDrawPause);

	// BGM�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

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

	// �^�C�}�[�}�l�[�W���[�̔j��
	if (FAILED(CTimerManager::Release(m_pTimerManager)))
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
		// �G�f�B�b�g�󋵂𔽓]
		//CGameManager::GetEditStage()->SetEnableEdit((!CGameManager::GetEditStage()->IsEdit()) ? true : false);
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
		// �v���C���[�̏o����ݒ�
		CScene::GetPlayer()->SetSpawn();
	}

#if 0
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F7))
	{
		// ���U���g�ɑJ��
		//CScene::GetPlayer()->SetState(CPlayer::STATE_CLEAR);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F8))
	{
		// ���U���g�ɑJ��
		//CScene::GetPlayer()->SetState(CPlayer::STATE_OVER);
	}
#endif

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "�@[�f�o�b�O����]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F1]�F�f�o�b�O�\����ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F2]�F�G�f�B�b�g���[�h��ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F3]�FUI�`���ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F4]�F�|�[�Y�`���ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F5]�F�J���������ON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F6]�F�v���C���[�X�|�[��\n");

#if 0
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F7]�F�������U���g�J��\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F8]�F���s���U���g�J��\n");
#endif

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

	if (m_pGameManager->GetState() == CGameManager::STATE_NORMAL)
	{ // �Q�[���}�l�[�W���[���ʏ��Ԃ̏ꍇ

		if (m_pTimerManager != NULL)
		{ // �g�p���̏ꍇ

			// �^�C�}�[�}�l�[�W���[�̍X�V
			m_pTimerManager->Update();
		}
		else { assert(false); }	// ��g�p��

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
//	�^�C�}�[�}�l�[�W���[�擾����
//============================================================
CTimerManager *CSceneGame::GetTimerManager(void)
{
	// �^�C�}�[�}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pTimerManager;
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

	// �^�C�}�[�̕`��󋵂�ݒ�
	m_pTimerManager->SetEnableDraw(bDraw);
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
