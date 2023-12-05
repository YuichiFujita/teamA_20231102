//============================================================
//
//	AI���� [playerAI.cpp]
//	Author�F������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "playerAI.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sound.h"
#include "flail.h"
#include "collision.h"
#include "retentionManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float	DEAD_ZONE = (float)USHRT_MAX * 0.01f;	// �X�e�B�b�N�̖�������X����

	const float	DASH_UP = 10.0f;	// �_�b�V���㏸��
	const float	DASH_SIDE = 15.0f;	// �_�b�V�����ړ���
	const float	DASH_REV = 0.25f;	// �_�b�V���̌��Z�W��
	const float	DASH_MINMOVE = 0.06f;	// �_�b�V���ēx�\�ɂȂ�ړ���

	const float STICK_LENGTH = 2.458f;
}

//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerAI::CPlayerAI()
{
	//�����l���
	m_pFlail = NULL;
	m_pPos = NULL;
	m_pMove = NULL;
	m_pDestRot = NULL;
	m_pCounterFlail = NULL;
	m_nPadID = 0;
	fRotstickR = 0.0f;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayerAI::~CPlayerAI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayerAI::Init
(
	CFlail* pFlail,
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pMove,
	D3DXVECTOR3* pDestRot,
	int* pCounterFlail,
	const int nPadID
)
{
	//�����l���
	m_pFlail = pFlail;
	m_pPos = pPos;
	m_pMove = pMove;
	m_pDestRot = pDestRot;
	m_pCounterFlail = pCounterFlail;
	m_nPadID = nPadID;

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	AI�S�ʂ��܂Ƃ߂�ꏊ
//============================================================
CPlayer::EMotion CPlayerAI::playerAI
(
	const int nMotionOld
)
{
	m_nMotionOld = nMotionOld;

	AIselect();

	return CPlayer::MOTION_IDOL;
}

//============================================================
//	�s���I���Ɋւ���AI
//============================================================
void CPlayerAI::AIselect()
{
	float fLengthMin = flail::FLAIL_RADIUS * (m_pFlail->GetNumChain() - 1);
	int nApproach = 0;
	int nApproachNum = 0;

	for (int nCntPlayer = 0; nCntPlayer < CManager::GetInstance()->GetRetentionManager()->GetNumPlayer(); nCntPlayer++)
	{
		CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);

		if (player != NULL && nCntPlayer != m_nPadID && player->GetState() != CPlayer::STATE_DEATH)
		{
			D3DXVECTOR3 vec;
			float length;

			// �v���C���[�ƃt���C���̃x�N�g�������߂�
			vec = player->GetVec3Position() - *m_pPos;
			vec.y = 0.0f;	// Y�͖���

			// ���������߂�
			length = D3DXVec3Length(&vec);

			if (length < flail::FLAIL_RADIUS * (m_pFlail->GetNumChain() - 1))
			{
				if (length < fLengthMin)
				{
					fLengthMin = length;
					nApproachNum = nCntPlayer;
				}

				nApproach++;
			}
		}
	}

	if (nApproach == 1)
	{
		CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(nApproachNum);
		D3DXVECTOR3 vec;

		// �v���C���[�ƃt���C���̃x�N�g�������߂�
		vec = player->GetVec3Position() - *m_pPos;
		vec.y = 0.0f;	// Y�͖���

		fRotstickR = atan2f(vec.x, vec.z);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[AI�ڕW�p�x]�F%f\n", fRotstickR);
	}
}

//============================================================
//	�U���Ɋւ���AI
//============================================================
void CPlayerAI::AIatack()
{

}

//============================================================
//	�ړ��Ɋւ���AI
//============================================================
void CPlayerAI::AImove()
{

}

//============================================================
//	�_�b�V���Ɋւ���AI
//============================================================
void CPlayerAI::AIDash
(
	D3DXVECTOR3& rMove,
	D3DXVECTOR3& rDashRot,
	D3DXVECTOR3& rDestRot,
	float& rPlusMove,
	bool& rDash
)
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		// �J����

	if (pCamera == NULL) { assert(false); }	// ��g�p��

	// �ϐ���錾
	float fStick = D3DXVec3Length(&rMove);	// �X�e�B�b�N�̓|����

	if (-1.0f < fStick)
	{ // �f�b�h�]�[���ȏ�̏ꍇ

		D3DXVECTOR3 dashPos = VEC3_ZERO;
		float rotMove;
		bool bJumpSelect = false;
		rotMove = m_pDestRot->y + HALF_PI;

		// �ړ��ʂ��X�V
		dashPos.x += sinf(rotMove + pCamera->GetVec3Rotation().y + HALF_PI) * 700.0f;
		dashPos.y = -1.0f;
		dashPos.z += cosf(rotMove + pCamera->GetVec3Rotation().y + HALF_PI) * 700.0f;

		if (Collision(dashPos))
		{
			int nProb = rand() % 10000;

			if (nProb > 9990)
			{
				bJumpSelect = true;
			}
			else
			{
				bJumpSelect = false;
			}
		}
		else
		{
			int nProb = rand() % 1000;

			if (nProb > 9999)
			{
				bJumpSelect = true;
			}
			else
			{
				bJumpSelect = false;
			}
		}

		if (bJumpSelect)
		{ // �_�b�V���̑��삪�s��ꂽ�ꍇ

			if (!rDash)
			{ // �_�b�V�����Ă��Ȃ��ꍇ

			  // ��ړ��ʂ����Z
				rMove.y += DASH_UP;

				// �v���X�ړ��ʂ����Z
				rPlusMove += DASH_SIDE;

				// �_�b�V�����̌�����ۑ�
				rDashRot.y = rotMove + pCamera->GetVec3Rotation().y + HALF_PI;

				m_pDestRot->y = rotMove + pCamera->GetVec3Rotation().y - HALF_PI;

				// �_�b�V�����Ă����Ԃɂ���
				rDash = true;

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_JUMP);	// �W�����v��
			}
		}
	}

	//if (rDash)
	//{ // �_�b�V�����Ă���ꍇ

	//  // �ړ��ʂ��X�V
	//	rMove.x += sinf(rDashRot.y) * rPlusMove;
	//	rMove.z += cosf(rDashRot.y) * rPlusMove;

	//	// �v���X�ړ��ʂ����Z
	//	rPlusMove += (0.0f - rPlusMove) * DASH_REV;

	//	if (fabsf(rPlusMove) < DASH_MINMOVE)
	//	{ // �ړ��ʂ����l��菭�Ȃ��Ȃ����ꍇ

	//	  // �ă_�b�V���ł���悤�ɂ���
	//		rPlusMove = 0.0f;
	//		rDash = false;
	//	}
	//}
}

//============================================================
//	��������
//============================================================
CPlayerAI *CPlayerAI::Create
( // ����
	CFlail* pFlail,
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pMove,
	D3DXVECTOR3* pDestRot,
	int* pCounterFlail,
	const int nPadID
)
{
	// �|�C���^��錾
	CPlayerAI *pAI = NULL;	// ���f��UI�����p

	if (pAI == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

	  // �������m��
		pAI = new CPlayerAI;	// ���f��UI
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pAI != NULL)
	{ // �g�p����Ă���ꍇ

	  // ���f��UI�̏�����
		if (FAILED(pAI->Init(pFlail, pPos, pMove, pDestRot, pCounterFlail, nPadID)))
		{ // �������Ɏ��s�����ꍇ

		  // �������J��
			delete pAI;
			pAI = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pAI;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�����蔻�菈��(�n�ʁA�u���b�N�A��Q��)
//============================================================
bool CPlayerAI::Collision(D3DXVECTOR3& rPos)
{
	if (CollisionGround(CPlayer::AXIS_X, rPos))
	{
		return true;
	}

	return false;
}
bool CPlayerAI::CollisionGround(const CPlayer::EAxis axis, D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(40.0f, 40.0f, 40.0f);		// �v���C���[�ŏ��傫��
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(40.0f, 40.0f, 40.0f);		// �v���C���[�ő�傫��
	bool bMin = false;	// �s�̕����̔����
	bool bMax = false;	// ���̕����̔����
	bool bHit = false;	// ���n�̔�����
	bool bHitBox = false;	// �ڐG�̔�����
	bool bHitBoxCheck = false;	// �ڐG�̔�����

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

	  // �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (pObjectTop != NULL)
		{ // �擪�����݂���ꍇ

		  // �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (pObjCheck != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

			  // �ϐ���錾
				D3DXVECTOR3 posGround = VEC3_ZERO;		// �n�Ոʒu
				D3DXVECTOR3 rotGround = VEC3_ZERO;		// �n�Ռ���
				D3DXVECTOR3 sizeMinGround = VEC3_ZERO;	// �n�Սŏ��傫��
				D3DXVECTOR3 sizeMaxGround = VEC3_ZERO;	// �n�Սő�傫��

														// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_GROUND)
				{ // �I�u�W�F�N�g���x�����n�Ղł͂Ȃ��ꍇ

				  // ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �n�Ղ̈ʒu��ݒ�
				posGround = pObjCheck->GetVec3Position();

				// �n�Ղ̌�����ݒ�
				rotGround = pObjCheck->GetVec3Rotation();

				// �n�Ղ̍ŏ��̑傫����ݒ�
				sizeMinGround = pObjCheck->GetVec3Sizing();
				sizeMinGround.y *= 2.0f;	// �c�̑傫����{�ɂ���

											// �n�Ղ̍ő�̑傫����ݒ�
				sizeMaxGround = pObjCheck->GetVec3Sizing();
				//sizeMaxGround.y = 0.0f;		// �c�̑傫����������

				if (collision::SquareTrigger(posGround, rPos, rotGround, sizeMaxGround, sizeMinGround))
				{
					bHitBoxCheck = true;
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}

	return bHitBoxCheck;
}