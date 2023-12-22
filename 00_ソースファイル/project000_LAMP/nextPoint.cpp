//============================================================
//
//	�J�ڈʒu���� [nextPoint.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "nextPoint.h"
#include "manager.h"
#include "renderer.h"
#include "objectGauge3D.h"
#include "controlPoint.h"
#include "player.h"
#include "collision.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "retentionManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY = 4;	// �͈̗͂D�揇��
	const int	WAIT_FRAME = 8;	// �J�ڂ̗]�C�t���[��

	namespace gauge
	{
		const CObjectGauge3D::EType	FRAMEVISUAL = CObjectGauge3D::TYPE_TUTORIAL;	// �t���[��������
		const CObject::ELabel	LABEL	= CObject::LABEL_UI;				// �I�u�W�F�N�g���x��
		const D3DXVECTOR3	SIZE		= D3DXVECTOR3(650.0f, 50.0f, 0.0f);	// �Q�[�W�傫��
		const D3DXVECTOR3	SIZE_FRAME	= D3DXVECTOR3(680.0f, 80.0f, 0.0f);	// �t���[���傫��
		const D3DXCOLOR		COL_FRONT	= XCOL_WHITE;						// �\�Q�[�W�F
		const D3DXCOLOR		COL_BACK	= D3DXCOLOR(0.4f, 0.4f, 0.5f, 1.0f);// ���Q�[�W�F
		const int	MAX_NUM			= 120;	// �ő�\���l
		const int	CHANGE_FRAME	= 2;	// �\���l�ϓ��t���[��
		const float	POSY_UP			= 1.0f;	// �\��Y�ʒu�̉��Z��
	}

	namespace control
	{
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(800.0f, 0.0f, 800.0f);	// �傫��
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(1400.0f, 0.0f, 0.0f);		// ��
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CNextPoint::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\field_font000.png",	// �ʏ�e�N�X�`��
};

//************************************************************
//	�q�N���X [CNextPoint] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CNextPoint::CNextPoint() : CObject3D(CObject::LABEL_NEXT, PRIORITY)
{
	// �����o�ϐ����N���A
	m_pGauge = NULL;	// �ҋ@�Q�[�W���
	m_pControl = NULL;	// ����\���̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CNextPoint::~CNextPoint()
{

}

//============================================================
//	����������
//============================================================
HRESULT CNextPoint::Init(void)
{
	// �����o�ϐ���������
	m_pGauge = NULL;	// �ҋ@�Q�[�W���
	m_pControl = NULL;	// ����\���̏��

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	BindTexture(mc_apTextureFile[TEXTURE_NORMAL]);

	// �ҋ@�Q�[�W�̐���
	m_pGauge = CObjectGauge3D::Create
	( // ����
		gauge::LABEL,			// �I�u�W�F�N�g���x��
		this,					// �e�I�u�W�F�N�g
		gauge::MAX_NUM,			// �ő�\���l
		gauge::CHANGE_FRAME,	// �\���l�ϓ��t���[��
		gauge::POSY_UP,			// �\��Y�ʒu�̉��Z��
		gauge::SIZE,			// �Q�[�W�傫��
		gauge::COL_FRONT,		// �\�Q�[�W�F
		gauge::COL_BACK			// ���Q�[�W�F
	);

	// �D�揇�ʂ�ݒ�
	m_pGauge->SetPriority(2);

	// �l��ݒ�
	m_pGauge->SetNum(0);

	// ����\���̐���
	m_pControl = CControlPoint::Create
	( // ����
		VEC3_ZERO,		// �ʒu
		control::SIZE	// �傫��
	);

	// ���g�̉e�̕`���OFF�E�e�̉f�荞�݂�ON�ɂ���
	SetEnableDepthShadow(true);
	SetEnableZTex(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CNextPoint::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CNextPoint::Update(void)
{
	if (CManager::GetInstance()->GetMode() != CScene::MODE_GAME)
	{ // �Q�[�����[�h�ł͂Ȃ��ꍇ

		return;
	}

	if (m_pGauge->GetNum() < m_pGauge->GetMaxNum())
	{ // �Q�[�W���ő�ł͂Ȃ��ꍇ

		if (Collision())
		{ // �v���C���[�S�����͈͓��̏ꍇ

			// �Q�[�W�����Z
			m_pGauge->AddNum(1);
		}
		else
		{ // �v���C���[�S�����͈͊O�̏ꍇ

			// �Q�[�W�����Z
			m_pGauge->AddNum(-2);
		}
	}
	else
	{ // �Q�[�W���ő�̏ꍇ

		// �`���[�g���A���̏I���𔽉f
		CManager::GetInstance()->GetRetentionManager()->EndTutorial();

		// �Q�[���ɑJ��
		CManager::GetInstance()->SetScene(CScene::MODE_GAME, WAIT_FRAME);
	}

	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CNextPoint::Draw(void)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw();
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CNextPoint::SetVec3Position(const D3DXVECTOR3 & rPos)
{
	// ���g�̈ʒu��ݒ�
	CObject3D::SetVec3Position(rPos);

	// ����\���̈ʒu��ݒ�
	m_pControl->SetVec3Position(rPos + control::SPACE);
}

//============================================================
//	��������
//============================================================
CNextPoint *CNextPoint::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize	// �傫��
)
{
	// �|�C���^��錾
	CNextPoint *pNextPoint = NULL;	// �J�ڈʒu�����p

	if (pNextPoint == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pNextPoint = new CNextPoint;	// �J�ڈʒu
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pNextPoint != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �J�ڈʒu�̏�����
		if (FAILED(pNextPoint->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pNextPoint;
			pNextPoint = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pNextPoint->SetVec3Position(rPos);

		// �傫����ݒ�
		pNextPoint->SetVec3Sizing(rSize);

		// �m�ۂ����A�h���X��Ԃ�
		return pNextPoint;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�v���C���[�S���Ƃ̓����蔻��
//============================================================
bool CNextPoint::Collision(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);	// �v���C���[���
		if (pPlayer != NULL)
		{ // �v���C���[���g�p����Ă���ꍇ

			if (pPlayer->GetState() == CPlayer::STATE_DEATH)
			{ // ���S���Ă����ꍇ

				continue;
			}

			float fPlayerRadius = pPlayer->GetRadius();	// �v���C���[���a

			// ��`����
			bool bHit = collision::Box2D
			( // ����
				pPlayer->GetVec3Position(),								// ����ʒu
				GetVec3Position(),										// ����ڕW�ʒu
				D3DXVECTOR3(fPlayerRadius, 0.0f, fPlayerRadius) * 0.5f,	// ����T�C�Y(�E�E��E��)
				D3DXVECTOR3(fPlayerRadius, 0.0f, fPlayerRadius) * 0.5f,	// ����T�C�Y(���E���E�O)
				GetVec3Sizing() * 0.5f,									// ����ڕW�T�C�Y(�E�E��E��)
				GetVec3Sizing() * 0.5f									// ����ڕW�T�C�Y(���E���E�O)
			);
			if (!bHit)
			{ // ����O�̏ꍇ

				// �S�����͈͊O�ł͂Ȃ�����false
				return false;
			}
		}
	}

	// �����ɂ��ǂ蒅���ΑS�����͈͓� == true
	return true;
}
