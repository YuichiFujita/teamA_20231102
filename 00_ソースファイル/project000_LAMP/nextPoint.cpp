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
#include "player.h"
#include "collision.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY = 4;	// �͈̗͂D�揇��
}

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

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �ҋ@�Q�[�W�̐���
	m_pGauge = CObjectGauge3D::Create
	( // ����
		LABEL_UI,
		this,
		10,
		2,
		350.0f,
		D3DXVECTOR3(220.0f, 40.0f, 0.0f)
	);

	// �e�̕`���OFF�ɂ���
	SetEnableDepthShadow(false);
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
	if (Collision())
	{ // �v���C���[�S�����͈͓��̏ꍇ


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

				break;
			}

			float fPlayerRadius = pPlayer->GetRadius();	// �v���C���[���a

			// ��`����
			bool bHit = collision::Box2D
			( // ����
				pPlayer->GetVec3Position(),							// ����ʒu
				GetVec3Position(),									// ����ڕW�ʒu
				D3DXVECTOR3(fPlayerRadius, 0.0f, fPlayerRadius),	// ����T�C�Y(�E�E��E��)
				D3DXVECTOR3(fPlayerRadius, 0.0f, fPlayerRadius),	// ����T�C�Y(���E���E�O)
				GetVec3Sizing(),									// ����ڕW�T�C�Y(�E�E��E��)
				GetVec3Sizing()										// ����ڕW�T�C�Y(���E���E�O)
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
