//============================================================
//
//	�J�n�\��[Ready.cpp]
//	Author�FTANNO RYUNOSUKE
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "Ready.h"
#include "manager.h"
#include "fade.h"
#include "sound.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "retentionManager.h"
#include "player.h"
#include "particle2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY = 12;	// �J�n�̗D�揇��
}

//************************************************************
//	�q�N���X [CReady] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CReady::CReady()
{
	// �����o�ϐ����N���A

}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CReady::CReady(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	
}

//============================================================
//	�f�X�g���N�^
//============================================================
CReady::~CReady()
{

}

//============================================================
//	����������
//============================================================
HRESULT CReady::Init(void)
{
	// �����o�ϐ���������
	m_pTexReady = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, -100.0f, 0.0f), D3DXVECTOR3(405.0f, 209.0f, 0.0f));
	m_pTexReady->BindTexture("data\\TEXTURE\\Ready.png");
	
	m_pTexFight = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(355.0f, 210.0f, 0.0f));
	m_pTexFight->BindTexture("data\\TEXTURE\\Fight.png");
	m_pTexFight->SetEnableDraw(false);
	m_nStateCount = 120;
	m_pFlare = CAnim2D::Create(23, 2, SCREEN_CENT, SCREEN_SIZE);
	m_pFlare->BindTexture("data\\TEXTURE\\lensFlare.jpg");
	m_pFlare->SetCounter(2);
	m_pFlare->SetAdd(true);
	m_pFlare->SetEnableStop(true);
	m_pFlare->SetLabel(LABEL_EFFECT);

	m_pTexReady->SetPriority(PRIORITY);
	m_pTexFight->SetPriority(PRIORITY);
	m_pFlare->SetPriority(PRIORITY);

	// �T�E���h�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_NOISE);	// �m�C�Y��

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CReady::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	if (m_pTexReady != NULL)
	{
		m_pTexReady->Uninit();
		m_pTexReady = NULL;
	}
	if (m_pTexFight != NULL)
	{
		m_pTexFight->Uninit();
		m_pTexFight = NULL;
	}
	if (m_pFlare != NULL)
	{
		m_pFlare->Uninit();
		m_pFlare = NULL;
	}
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CReady::Update(void)
{
	if (CManager::GetInstance()->GetFade()->GetState() == CFade::FADE_NONE)
	{
		m_pTexReady->SetVec3Position(D3DXVECTOR3(m_pTexReady->GetVec3Position() + (D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f) - m_pTexReady->GetVec3Position())* 0.1f));
		m_nStateCount--;

		if (m_nStateCount <= 0)
		{
			if (m_nStateCount == 0)
			{
				m_pFlare->SetEnableStop(false);

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_FIGHT);	// �퓬��
			}
			m_pTexReady->SetEnableDraw(false);
			m_pTexFight->SetEnableDraw(true);
			if (m_pFlare->GetLoopAnimation() > 0)
			{ 
				CSceneGame::GetGameManager()->SetState(CGameManager::STATE_NORMAL);

				for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
				{ // �v���C���[�����J��Ԃ�

					CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);	// �v���C���[���

					if (pPlayer != NULL)
					{ // �v���C���[�����݂���ꍇ

						// �v���C���[���o��
						pPlayer->SetSpawn();
					}
				}
				
				Uninit();
			}
		}
	}
}

//============================================================
//	�`�揈��
//============================================================
void CReady::Draw(void)
{

}

//============================================================
//	��������
//============================================================
CReady *CReady::Create()
{	// �|�C���^��錾
	CReady *pReady = NULL;	// �A�j���[�V����2D�����p

	if (pReady == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

	  // �������m��
		pReady = new CReady(ELabel::LABEL_UI);	// �A�j���[�V����2D
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pReady != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

	  //������
		if (FAILED(pReady->Init()))
		{ // �������Ɏ��s�����ꍇ

		  // �������J��
			delete pReady;
			pReady = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		

		// �m�ۂ����A�h���X��Ԃ�
		return pReady;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
void CReady::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}