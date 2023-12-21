//============================================================
//
//	����ʒu���� [controlPoint.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "controlPoint.h"
#include "manager.h"
#include "renderer.h"
#include "objectBillboard.h"
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

	namespace tutorial
	{
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(800.0f, 500.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(0.0f, 800.0f, 0.0f);		// ��
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CControlPoint::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	NULL,	// �ʏ�e�N�X�`��
};

const char *CControlPoint::mc_apLessonFile[] =	// ���b�X���e�N�X�`���萔
{
	"data\\TEXTURE\\lesson000.png",	// �Ăэ��݃e�N�X�`��
};

//************************************************************
//	�q�N���X [CControlPoint] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CControlPoint::CControlPoint() : CObject3D(CObject::LABEL_NEXT, PRIORITY)
{
	// �����o�ϐ����N���A
	m_pTutorial = NULL;	// �`���[�g���A���\�����
}

//============================================================
//	�f�X�g���N�^
//============================================================
CControlPoint::~CControlPoint()
{

}

//============================================================
//	����������
//============================================================
HRESULT CControlPoint::Init(void)
{
	// �����o�ϐ���������
	m_pTutorial = NULL;	// �`���[�g���A���\�����

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	BindTexture(mc_apTextureFile[TEXTURE_NORMAL]);

	// �`���[�g���A���\���̐���
	m_pTutorial = CObjectBillboard::Create
	( // ����
		VEC3_ZERO,		// �ʒu
		tutorial::SIZE	// �傫��
	);

	// �e�N�X�`����o�^�E����
	m_pTutorial->BindTexture(mc_apLessonFile[LESSON_YOBIKOMI]);

	// ���x����ݒ�
	m_pTutorial->SetLabel(LABEL_UI);

	// ���g�̉e�̕`���OFF�E�e�̉f�荞�݂�ON�ɂ���
	SetEnableDepthShadow(true);
	SetEnableZTex(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CControlPoint::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CControlPoint::Update(void)
{
	if (CManager::GetInstance()->GetMode() != CScene::MODE_GAME)
	{ // �Q�[�����[�h�ł͂Ȃ��ꍇ

		return;
	}

	// �`���[�g���A���̑��씻��
	Collision();

	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CControlPoint::Draw(void)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw();
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CControlPoint::SetVec3Position(const D3DXVECTOR3 & rPos)
{
	// ���g�̈ʒu��ݒ�
	CObject3D::SetVec3Position(rPos);

	// �`���[�g���A���\���̈ʒu��ݒ�
	m_pTutorial->SetVec3Position(rPos + tutorial::SPACE);
}

//============================================================
//	��������
//============================================================
CControlPoint *CControlPoint::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize	// �傫��
)
{
	// �|�C���^��錾
	CControlPoint *pControlPoint = NULL;	// ����ʒu�����p

	if (pControlPoint == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pControlPoint = new CControlPoint;	// ����ʒu
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pControlPoint != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// ����ʒu�̏�����
		if (FAILED(pControlPoint->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pControlPoint;
			pControlPoint = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pControlPoint->SetVec3Position(rPos);

		// �傫����ݒ�
		pControlPoint->SetVec3Sizing(rSize);

		// �m�ۂ����A�h���X��Ԃ�
		return pControlPoint;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�v���C���[�S���Ƃ̓����蔻��
//============================================================
void CControlPoint::Collision(void)
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
			if (bHit)
			{ // ������̏ꍇ

				// �`���[�g���A���̑���
				UpdateTutorial(nCntPlayer);
			}
		}
	}
}

//============================================================
//	�`���[�g���A���̑��쏈��
//============================================================
void CControlPoint::UpdateTutorial(const int nID)
{

}
