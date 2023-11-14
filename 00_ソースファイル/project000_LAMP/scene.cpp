//============================================================
//
//	�V�[������ [scene.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "camera.h"
#include "sceneTitle.h"
#include "sceneTutorial.h"
#include "sceneEntry.h"
#include "sceneGame.h"
#include "sceneResult.h"
#include "sceneRanking.h"

#include "stage.h"
#include "player.h"
#include "retentionManager.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CStage *CScene::m_pStage = NULL;	// �X�e�[�W
CPlayer	*CScene::m_apPlayer[MAX_PLAYER] = {};	// �v���C���[�I�u�W�F�N�g

//************************************************************
//	�e�N���X [CScene] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScene::CScene(const EMode mode)
{
	// �����o�ϐ����N���A
	m_mode = mode;	// ���[�h
}

//============================================================
//	�f�X�g���N�^
//============================================================
CScene::~CScene()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScene::Init(void)
{
	// �ϐ���錾
	CStage::ELoad load = (m_mode == MODE_TUTORIAL) ? CStage::LOAD_TUTORIAL : CStage::LOAD_GAME;	// �Ǎ��X�e�[�W
	int nNumCreate = 0;	// �v���C���[������

	// �X�e�[�W�̐���
	m_pStage = CStage::Create(load);
	if (m_pStage == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_mode == MODE_ENTRY)
	{ // �G���g���[���[�h�̏ꍇ

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{ // �v���C���[�����J��Ԃ�

			// �v���C���[�I�u�W�F�N�g�̐���
			m_apPlayer[nCntPlayer] = CPlayer::Create(m_mode, nCntPlayer);
		}
	}
	else if (m_mode == MODE_GAME)
	{ // �Q�[�����[�h�̏ꍇ

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{ // �v���C���[�����J��Ԃ�

			if (CManager::GetInstance()->GetRetentionManager()->IsEntry(nCntPlayer))
			{ // �G���g���[����Ă���ꍇ

				// �v���C���[�I�u�W�F�N�g�̐���
				m_apPlayer[nNumCreate] = CPlayer::Create(m_mode, nCntPlayer);

				// �����������Z
				nNumCreate++;
			}
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CScene::Uninit(void)
{
	// �X�e�[�W�̔j��
	if (FAILED(CStage::Release(m_pStage)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �I���ς݂̃I�u�W�F�N�g�|�C���^��NULL�ɂ���
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // �v���C���[�̑������J��Ԃ�

		// �v���C���[�I�u�W�F�N�g��NULL�ɂ���
		m_apPlayer[nCntPlayer] = NULL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CScene::Update(void)
{
	if (m_pStage != NULL)
	{ // �g�p���̏ꍇ

		// �X�e�[�W�̍X�V
		m_pStage->Update();
	}
	else { assert(false); }	// ��g�p��

	if (CManager::GetInstance()->GetLight() != NULL)
	{ // �g�p���̏ꍇ

		// ���C�g�̍X�V
		CManager::GetInstance()->GetLight()->Update();
	}
	else { assert(false); }	// ��g�p��

	if (CManager::GetInstance()->GetCamera() != NULL)
	{ // �g�p���̏ꍇ

		// �J�����̍X�V
		CManager::GetInstance()->GetCamera()->Update();
	}
	else { assert(false); }	// ��g�p��

	if (CManager::GetInstance()->GetRenderer() != NULL)
	{ // �g�p���̏ꍇ

		// �����_���[�̍X�V
		CManager::GetInstance()->GetRenderer()->Update();
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	��������
//============================================================
CScene *CScene::Create(EMode mode)
{
	// �|�C���^��錾
	CScene *pScene = NULL;	// �V�[�������p

	if (pScene == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		switch (mode)
		{ // ���[�h���Ƃ̏���
		case MODE_TITLE:

			// �^�C�g����ʂ𐶐�
			pScene = new CSceneTitle(mode);

			break;

		case MODE_TUTORIAL:

			// �`���[�g���A����ʂ𐶐�
			pScene = new CSceneTutorial(mode);

			break;

		case MODE_ENTRY:

			// �G���g���[��ʂ𐶐�
			pScene = new CSceneEntry(mode);

			break;

		case MODE_GAME:

			// �Q�[����ʂ𐶐�
			pScene = new CSceneGame(mode);

			break;

		case MODE_RESULT:

			// ���U���g��ʂ𐶐�
			pScene = new CSceneResult(mode);

			break;

		case MODE_RANKING:

			// �����L���O��ʂ𐶐�
			pScene = new CSceneRanking(mode);

			break;

		default:	// ��O����
			assert(false);
			break;
		}
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pScene != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �V�[���̏�����
		if (FAILED(pScene->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �V�[���̏I��
			if (FAILED(pScene->Uninit()))
			{ // �V�[���̏I���Ɏ��s�����ꍇ

				// �������J��
				delete pScene;
				pScene = NULL;

				// ���s��Ԃ�
				assert(false);
				return NULL;
			}

			// �������J��
			delete pScene;
			pScene = NULL;

			// ���s��Ԃ�
			assert(false);
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pScene;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CScene::Release(CScene *&prScene)
{
	if (prScene != NULL)
	{ // �g�p���̏ꍇ

		// �V�[���̏I��
		if (FAILED(prScene->Uninit()))
		{ // �V�[���̏I���Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �������J��
		delete prScene;
		prScene = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�X�e�[�W�擾����
//============================================================
CStage *CScene::GetStage(void)
{
	// �X�e�[�W�̃|�C���^��Ԃ�
	return m_pStage;
}

//============================================================
//	�v���C���[�擾����
//============================================================
CPlayer *CScene::GetPlayer(const int nID)
{
	// �����C���f�b�N�X�̃v���C���[�|�C���^��Ԃ�
	return m_apPlayer[nID];
}

//============================================================
//	���[�h�̐ݒ菈��
//============================================================
void CScene::SetMode(const EMode mode)
{
	// �����̃��[�h��ݒ�
	m_mode = mode;
}

//============================================================
//	���[�h�擾����
//============================================================
CScene::EMode CScene::GetMode(void) const
{
	// ���݂̃��[�h��Ԃ�
	return m_mode;
}
