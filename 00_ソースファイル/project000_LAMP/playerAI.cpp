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
#include "obstacle.h"

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
	const float	STICK_REV = 0.00015f;	// �X�e�B�b�N�̌X���ʂ̕␳�W��
}

//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerAI::CPlayerAI()
{
	//�����l���
	m_pFlail = NULL;
	m_pDashRot = VEC3_ZERO;
	m_stateAI = STATEAI_MOVE;
	m_currentMotion = CPlayer::MOTION_IDOL;
	m_nCounterFlail = 0;
	m_nPadID = 0;
	m_nCountAI = 0;
	m_fRotstickR = 0.0f;
	m_bDash = false;
	m_bAttack = false;
	m_bMove = false;
	m_bEmote = false;
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
HRESULT CPlayerAI::Init(const int nPadID)
{
	m_nPadID = nPadID;

	m_fRotstickL = 0.0f;

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	AI�S�ʂ��܂Ƃ߂�ꏊ
//============================================================
CPlayer::EMotion CPlayerAI::playerAI
(
	CFlail* pFlail,
	D3DXVECTOR3& pPos,
	D3DXVECTOR3& rMove,
	D3DXVECTOR3& pDestRot,
	int pCounterFlail,
	const int nMotionOld
)
{
	m_currentMotion = CPlayer::MOTION_IDOL;		// ���݂̃��[�V����
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPadID);

	//�l���
	m_pFlail = pFlail;
	m_nCounterFlail = pCounterFlail;
	m_nMotionOld = nMotionOld;

	m_currentMotion = AIselect(pFlail,pPos, rMove, pDestRot, pCounterFlail, nMotionOld);

	player->SetCounterFlail(m_nCounterFlail);

	return m_currentMotion;
}

//============================================================
//	�s���I���Ɋւ���AI
//============================================================
CPlayer::EMotion CPlayerAI::AIselect
(
	CFlail* pFlail,
	D3DXVECTOR3& rPos,
	D3DXVECTOR3& rMove,
	D3DXVECTOR3& rDestRot,
	int pCounterFlail,
	const int nMotionOld
)
{
	float fLengthMin = flail::FLAIL_RADIUS * (m_pFlail->GetNumChain() - 1);
	int nApproach = 0;
	m_nApproachNum = 0;

	for (int nCntPlayer = 0; nCntPlayer < 4; nCntPlayer++)
	{
		CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);

		if (player != NULL)
		{
			if (nCntPlayer != m_nPadID && player->GetState() != CPlayer::STATE_DEATH)
			{
				D3DXVECTOR3 vec;
				float length;

				// �v���C���[�ƃt���C���̃x�N�g�������߂�
				vec = player->GetVec3Position() - rPos;
				vec.y = 0.0f;	// Y�͖���

				// ���������߂�
				length = D3DXVec3Length(&vec);

				if (length < flail::FLAIL_RADIUS * (m_pFlail->GetNumChain() - 1))
				{
					if (length < fLengthMin)
					{
						fLengthMin = length;
						m_nApproachNum = nCntPlayer;
					}

					nApproach++;
				}
			}
		}
	}

	m_nCountAI++;

	if (m_nCountAI % 180 == 0)
	{
		m_nCountAI = 0;

		int nProb = rand() % 10000;

		switch (m_stateAI)
		{
		case CPlayerAI::STATEAI_NONE:

			if (m_nCounterFlail == flail::FLAIL_DROP)
			{
				if (nProb > 9500)
				{
					m_stateAI = STATEAI_NONE;
				}
				else if (nProb > 9000)
				{
					m_stateAI = STATEAI_MOVE;
				}
				else
				{
					m_stateAI = STATEAI_ATTACK;
				}
			}
			else
			{
				if (nProb > 9500)
				{
					m_stateAI = STATEAI_NONE;
				}
				else if (nProb > 2000)
				{
					m_stateAI = STATEAI_MOVE;
				}
				else
				{
					m_stateAI = STATEAI_ATTACK;
				}
			}

			break;

		case CPlayerAI::STATEAI_MOVE:

			if (nProb > 9000)
			{
				m_stateAI = STATEAI_NONE;
			}
			else if (nProb > 8000)
			{
				m_stateAI = STATEAI_MOVE;

				nProb = rand() % 10000;

				if (nProb > 5000)
				{
					m_fRotstickL = (float)((rand() % 629) - 314);
				}
				else
				{

				}
			}
			else
			{
				m_stateAI = STATEAI_ATTACK;
			}

			break;

		case CPlayerAI::STATEAI_ATTACK:

			if (nApproach > 0 && (m_nCounterFlail <= flail::FLAIL_THROW && m_nCounterFlail > flail::FLAIL_DEF))
			{
				m_stateAI = STATEAI_ATTACK;
			}
			else
			{
				if (nProb > 6000)
				{
					m_stateAI = STATEAI_NONE;
				}
				else if (nProb > 2000)
				{
					m_stateAI = STATEAI_MOVE;
				}
				else
				{
					m_stateAI = STATEAI_ATTACK;
				}
			}

			break;

		case CPlayerAI::STATEAI_EMOTE:

			m_stateAI = STATEAI_EMOTE;

		case CPlayerAI::STATEAI_MAX:

			m_stateAI = STATEAI_NONE;

			break;

		default:

			break;
		}
	}

	if (CManager::GetInstance()->GetRetentionManager()->GetNumSurvival() == 0 && m_stateAI != STATEAI_EMOTE)
	{ // �c��l����1�l�̏ꍇ

		m_stateAI = STATEAI_EMOTE;
		m_bEmote = true;
	}
	
	switch (m_stateAI)
	{
	case CPlayerAI::STATEAI_NONE:

		m_bMove = false;
		m_bAttack = false;

		break;

	case CPlayerAI::STATEAI_MOVE:

		if (m_nCounterFlail == flail::FLAIL_CHARGE)
		{
			m_bAttack = true;
		}
		else
		{
			m_bAttack = false;
		}

		m_bMove = true;

		break;

	case CPlayerAI::STATEAI_ATTACK:

		if (nApproach == 1)
		{
			CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nApproachNum);
			D3DXVECTOR3 vec, rodPos;

			rodPos.x = player->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld()._41;
			rodPos.y = player->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld()._42;
			rodPos.z = player->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld()._43;

			// �v���C���[�ƃt���C���̃x�N�g�������߂�
			vec = rodPos - rPos;
			vec.y = 0.0f;	// Y�͖���

			m_fRotstickR = atan2f(vec.x, vec.z);

			useful::NormalizeRot(m_fRotstickR);

			m_pFlail->SetChainRotTarget(m_fRotstickR);

			m_fRotstickL = m_fRotstickR - (D3DX_PI * 1.0f);

			if (D3DXVec3Length(&vec) < flail::FLAIL_RADIUS * (float)(((float)m_nCounterFlail / (float)flail::FLAIL_CHARGE) * (m_pFlail->GetNumChain() - 1)))
			{
				m_bAttack = false;
			}
			else
			{
				m_bAttack = true;
			}

			if (D3DXVec3Length(&vec) < 1200.0f)
			{
				m_bMove = false;
			}
			else
			{
				m_bMove = true;
			}
		}
		else if (nApproach > 1)
		{
			m_fRotstickR = 0.0f;
			m_nApproachNum = -1;

			int nProb = rand() % 10000;

			if (nProb > 3000 && m_nCounterFlail == flail::FLAIL_CHARGE)
			{
				m_bAttack = false;
			}
			else
			{
				m_bAttack = true;
			}

			m_bMove = true;
		}
		else
		{
			m_bAttack = false;
			m_bMove = true;
		}

		break;

	case CPlayerAI::STATEAI_EMOTE:

		m_bAttack = false;
		m_bMove = false;

		break;

	case CPlayerAI::STATEAI_MAX:

		break;

	default:
		break;
	}

	//CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�߂��̓G]�F%d\n", nApproach);

	m_currentMotion = AImove(pFlail, rPos, rMove, rDestRot, pCounterFlail, nMotionOld);
	m_currentMotion = AIattack(pFlail,rPos, rMove, rDestRot, pCounterFlail, nMotionOld);
	m_currentMotion = AIemote(rDestRot);

	return m_currentMotion;
}

//============================================================
//	�U���Ɋւ���AI
//============================================================
CPlayer::EMotion CPlayerAI::AIattack
(
	CFlail* pFlail,
	D3DXVECTOR3& rPos,
	D3DXVECTOR3& rMove,
	D3DXVECTOR3& rDestRot,
	int pCounterFlail,
	const int nMotionOld
)
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPadID);

	// �J�E���^�[�̒l�ɂ���ċ�����ύX
	if (m_nCounterFlail > flail::FLAIL_DEF)
	{// 0���傫����

		if (m_bAttack && m_nCounterFlail <= flail::FLAIL_CHARGE)
		{// ������{�^����������Ă��鎞
		 // �J�E���^�[�A�b�v
			m_nCounterFlail++;

			// ���l�ŃJ�E���^�[���~�߂�
			if (m_nCounterFlail > flail::FLAIL_CHARGE)
			{
				m_nCounterFlail = flail::FLAIL_CHARGE;
			}

			// ���߂Ă�ԓS����U���
			m_pFlail->SetChainRotMove((-0.002f * m_nCounterFlail) - 0.12f);

			m_pFlail->SetLengthTarget(flail::FLAIL_RADIUS * 5.0f);

			// �ړ��ʂ��X�V
			rMove.x *= 1.0f - (0.0042f * m_nCounterFlail);
			rMove.z *= 1.0f - (0.0042f * m_nCounterFlail);

			// �ڕW������ݒ�
			rDestRot.y = m_pFlail->GetChainRotTarget() + D3DX_PI;

			// �`���[�W���[�V������ݒ�
			m_currentMotion = CPlayer::MOTION_CHARGE;
		}

		// ����
		if (m_bAttack == false && m_nCounterFlail != flail::FLAIL_THROW)
		{
			// ���߂����Ԃɉ����Ĕ򋗗�����
			D3DXVECTOR3 move = VEC3_ZERO;
			float lengthTarget = flail::FLAIL_RADIUS * (float)(((float)m_nCounterFlail / (float)flail::FLAIL_CHARGE) * (m_pFlail->GetNumChain() - 1));
			move.x = (sinf(m_pFlail->GetChainRotTarget()) * 5.0f * m_nCounterFlail);
			move.z = (cosf(m_pFlail->GetChainRotTarget()) * 5.0f * m_nCounterFlail);
			m_pFlail->SetMove(move);
			m_pFlail->SetChainRotMove(m_pFlail->GetChainRotMove());

			D3DXVECTOR3 posFlail = m_pFlail->GetVec3Position();
			if (posFlail.y < 0.0f)
			{
				posFlail.y = 0.0f;
				m_pFlail->SetVec3Position(posFlail);
			}

			if (m_nApproachNum != -1)
			{
				lengthTarget = flail::FLAIL_RADIUS * (float)(((float)m_nCounterFlail / (float)flail::FLAIL_CHARGE) * (m_pFlail->GetNumChain() - 1));

				if (lengthTarget < flail::FLAIL_RADIUS * flail::FLAIL_NUM_MIN)
				{
					lengthTarget = flail::FLAIL_RADIUS * flail::FLAIL_NUM_MIN;
				}

				// �ڕW�p�x�ɍ��킹��
				m_pFlail->ShotFlail(m_pFlail->GetChainRotTarget() - D3DX_PI * 0.5f);
				m_pFlail->SetChainRotMove(0.0f);
			}
			else
			{
				lengthTarget = flail::FLAIL_RADIUS * (float)(((float)m_nCounterFlail / (float)flail::FLAIL_CHARGE) * (m_pFlail->GetNumChain() - 1)) * 0.6f;

				if (lengthTarget < flail::FLAIL_RADIUS * flail::FLAIL_NUM_MIN * 0.5f)
				{
					lengthTarget = flail::FLAIL_RADIUS * flail::FLAIL_NUM_MIN * 0.5f;
				}
			}

			m_pFlail->SetLengthTarget(lengthTarget);

			// �J�E���^�[�̐ݒ�
			m_nCounterFlail = flail::FLAIL_THROW;

			// �U�����[�V������ݒ�
			m_currentMotion = CPlayer::MOTION_ATTACK;

			// AI����Ԃ�ݒ�
			m_stateAI = STATEAI_NONE;

			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SWING);
		}

		if (m_nCounterFlail == flail::FLAIL_THROW)
		{
			// �ړ��ʂ��X�V
			rMove.x = 0.0f;
			rMove.z = 0.0f;

			// �t���C�����~�܂�����J�E���^�[�����̒i�K��
			if (m_pFlail->GetLengthChain() >= m_pFlail->GetLengthTarget())
			{
				// ���[�V�������U������ύX
				player->SetMotion(m_currentMotion);

				m_nCounterFlail = flail::FLAIL_DROP;

				// AI����Ԃ�ݒ�
				m_stateAI = STATEAI_NONE;
			}
		}
	}
	else if (m_nCounterFlail == flail::FLAIL_DEF)
	{
		// �t���C���������I�ɏ���
		m_pFlail->CatchFlail();

		// �J�E���^�[�A�b�v�J�n
		if (m_bAttack)
		{
			m_nCounterFlail++;
		}
	}
	else
	{
		// �S���ƃv���C���[�̋�������薢���̎��v���C���[�ʒu�ɓS���Œ�
		if (m_pFlail->GetLengthChain() <= flail::FLAIL_RADIUS * 3.0f)
		{
			m_nCounterFlail = flail::FLAIL_DEF;
			m_pFlail->SetMove(VEC3_ZERO);
			m_pFlail->CatchFlail();
		}
		else if (m_pFlail->GetLengthTarget() <= flail::FLAIL_RADIUS * 6.0f && m_nCounterFlail == flail::FLAIL_DROP)
		{
			m_nCounterFlail = flail::FLAIL_DEF;
			m_pFlail->SetMove(VEC3_ZERO);
			m_pFlail->CatchFlail();
		}
		else
		{
			// �����߂�
			if (m_bAttack)
			{
				m_nCounterFlail -= 1;

				if (m_nCounterFlail < -60)
				{
					m_nCounterFlail = -60;
				}

				if (m_nCounterFlail == -2)
				{
					float rot1 = m_fRotstickR;
					float rot2 = rDestRot.y;
					float rot3 = rot2 - rot1;

					useful::NormalizeRot(rot3);

					if (rot3 > 1.0f)
					{
						// ���߂Ă�ԓS����U���
						m_pFlail->SetChainRotMove(0.03f);
					}
					else if (rot3 < -1.0f)
					{
						// ���߂Ă�ԓS����U���
						m_pFlail->SetChainRotMove(-0.03f);
					}
					else
					{
						// ���߂Ă�ԓS����U���
						m_pFlail->SetChainRotMove(0.0f);
					}
				}

				// �ړ��ʂ��X�V
				rMove.x = 0.0f;
				rMove.z = 0.0f;
			}
			else
			{
				m_nCounterFlail = flail::FLAIL_DROP;
			}
		}
	}

	return m_currentMotion;
}

//============================================================
//	�ړ��Ɋւ���AI
//============================================================
CPlayer::EMotion CPlayerAI::AImove
(
	CFlail* pFlail,
	D3DXVECTOR3& rPos,
	D3DXVECTOR3& rMove,
	D3DXVECTOR3& rDestRot,
	int pCounterFlail,
	const int nMotionOld
)
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPadID);
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		// �J����

	if (m_bMove)
	{ // �f�b�h�]�[���ȏ�̏ꍇ

		// �ϐ���錾
		float fMove = 0.0f;	// �v���C���[�ړ���
		float fRotDiff = 0.0f;

		D3DXVECTOR3 movePos = VEC3_ZERO;

		// �ړ��ʂ��X�V
		movePos.x = rPos.x + sinf(m_fRotstickL + D3DX_PI) * 75.0f;
		movePos.y = -1.0f;
		movePos.z = rPos.z + cosf(m_fRotstickL + D3DX_PI) * 75.0f;

		/*CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�ړ��ʒu]�F%f %f %f\n", rPos.x, rPos.y, rPos.z);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�ړ��\���ʒu]�F%f %f %f\n", movePos.x, movePos.y, movePos.z);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�ړ��\���ʒu����]�F%f %f %f\n", movePos.x - rPos.x, movePos.y - rPos.y, movePos.z - rPos.z);*/

		if (CollisionGround(CPlayer::AXIS_X, movePos))
		{
			// �ړ��ʂ��X�V
			movePos.x = rPos.x + sinf(m_fRotstickL + D3DX_PI) * 75.0f;
			movePos.y = 1.0f;
			movePos.z = rPos.z + cosf(m_fRotstickL + D3DX_PI) * 75.0f;

			if (CollisionBlock(CPlayer::AXIS_X, movePos))
			{
				float fRotDiff = 0.0f;

				while (fRotDiff < 3.14f)
				{
					fRotDiff = (float)((rand() % 629) - 314);
					fMove = 1.2f;

					float colRot = m_fRotstickL + fRotDiff;
					useful::NormalizeRot(colRot);

					// �ړ��ʂ��X�V
					movePos.x = rPos.x + sinf(colRot + D3DX_PI) * 75.0f;
					movePos.y = -1.0f;
					movePos.z = rPos.z + cosf(colRot + D3DX_PI) * 75.0f;

					if (Collision(movePos))
					{
						m_fRotstickL = colRot;

						break;
					}
				}
			}
			else
			{
				if (CollisionObstacle(movePos))
				{
					float fRotDiff = 0.0f;

					while (fRotDiff < 3.14f)
					{
						fRotDiff = (float)((rand() % 629) - 314);
						fMove = 1.2f;

						float colRot = m_fRotstickL + fRotDiff;
						useful::NormalizeRot(colRot);

						// �ړ��ʂ��X�V
						movePos.x = rPos.x + sinf(colRot + D3DX_PI) * 75.0f;
						movePos.y = -1.0f;
						movePos.z = rPos.z + cosf(colRot + D3DX_PI) * 75.0f;

						if (Collision(movePos))
						{
							m_fRotstickL = colRot;

							break;
						}
					}
				}
				else
				{
					fMove = 1.2f;
				}
			}
		}
		else
		{
			float fRotDiff = 0.0f;

			while (fRotDiff < 3.14f)
			{
				fRotDiff = (float)((rand() % 629) - 314);
				fMove = 1.2f;

				float colRot = m_fRotstickL + fRotDiff;
				useful::NormalizeRot(colRot);

				// �ړ��ʂ��X�V
				movePos.x = rPos.x + sinf(colRot + D3DX_PI) * 75.0f;
				movePos.y = -1.0f;
				movePos.z = rPos.z + cosf(colRot + D3DX_PI) * 75.0f;

				if (Collision(movePos))
				{
					m_fRotstickL = colRot;

					break;
				}
			}
		}

		if (!m_bDash)
		{ // �_�b�V�����ł͂Ȃ��ꍇ

		  // �ړ��ʂ��X�V
			rMove.x += sinf(m_fRotstickL + D3DX_PI) * fMove;
			rMove.z += cosf(m_fRotstickL + D3DX_PI) * fMove;

			// �ړ����[�V������ݒ�
			m_currentMotion = CPlayer::MOTION_MOVE;
		}
		else
		{ // �_�b�V�����̏ꍇ

		  // �ړ��ʂ��X�V
			rMove.x += sinf(m_pDashRot.y) * fMove;
			rMove.z += cosf(m_pDashRot.y) * fMove;

			// �_�b�V�����[�V������ݒ�
			m_currentMotion = CPlayer::MOTION_DASH;
		}

		if (m_pFlail->GetLengthChain() >= flail::FLAIL_RADIUS * (m_pFlail->GetNumChain() - 1) || m_nMotionOld == CPlayer::MOTION_PULL)
		{ // �������苗���̏ꍇ

		  // �ړ��ʂ��X�V
			rMove.x *= 0.7f;
			rMove.z *= 0.7f;

			D3DXVECTOR3 vecFlail;
			float rotMove, rotFlail, rotDiff;

			vecFlail = m_pFlail->GetVec3Position() - player->GetVec3Position();

			rotMove = atan2f(rMove.x, rMove.z);
			rotFlail = atan2f(vecFlail.x, vecFlail.z);

			rotDiff = rotMove - rotFlail;
			useful::NormalizeRot(rotDiff);

			if (rotDiff > D3DX_PI * 0.5f || rotDiff < D3DX_PI * -0.5f)
			{
				// �������胂�[�V������ݒ�
				m_currentMotion = CPlayer::MOTION_PULL;
			}
		}

		// �ڕW������ݒ�
		rDestRot.y = atan2f(-rMove.x, -rMove.z);
		m_pFlail->SetChainRotTarget(rDestRot.y + D3DX_PI);
	}

	return m_currentMotion;
}

//============================================================
//	�_�b�V���Ɋւ���AI
//============================================================
void CPlayerAI::AIDash
(
	D3DXVECTOR3& rPos,
	D3DXVECTOR3& rMove,
	D3DXVECTOR3& rDashRot,
	D3DXVECTOR3& rDestRot,
	float& rPlusMove,
	int& rCounterFlail,
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
		bool bJumpSelect = false;

		// �ړ��ʂ��X�V
		dashPos.x = rPos.x + sinf(m_fRotstickL + D3DX_PI) * 700.0f;
		dashPos.y = -1.0f;
		dashPos.z = rPos.z + cosf(m_fRotstickL + D3DX_PI) * 700.0f;

		if (Collision(dashPos))
		{
			int nProb = rand() % 10000;
			
			if (nProb > 9920)
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
			int nProb = rand() % 10000;

			if (nProb > 9999)
			{
				bJumpSelect = true;
			}
			else
			{
				bJumpSelect = false;
			}
		}

		if (m_stateAI != STATEAI_EMOTE)
		{
			bJumpSelect = false;
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
				rDashRot.y = m_fRotstickL + D3DX_PI;

				// �_�b�V�����Ă����Ԃɂ���
				rDash = true;

				if (rCounterFlail > flail::FLAIL_DEF && rCounterFlail <= flail::FLAIL_CHARGE)
				{
					rCounterFlail = flail::FLAIL_DEF;
					m_stateAI = STATEAI_MOVE;
				}

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_JUMP);	// �W�����v��
			}
		}
	}

	m_pDashRot = rDashRot;
	m_bDash = rDash;

	if (m_bDash)
	{ // �_�b�V�����Ă���ꍇ

	  // �ړ��ʂ��X�V
		rMove.x += sinf(rDashRot.y) * rPlusMove;
		rMove.z += cosf(rDashRot.y) * rPlusMove;

		// �v���X�ړ��ʂ����Z
		rPlusMove += (0.0f - rPlusMove) * DASH_REV;

		if (fabsf(rPlusMove) < DASH_MINMOVE)
		{ // �ړ��ʂ����l��菭�Ȃ��Ȃ����ꍇ

		  // �ă_�b�V���ł���悤�ɂ���
			rPlusMove = 0.0f;
			m_bDash = false;
			rDash = false;
			m_stateAI = STATEAI_MOVE;
		}
	}
}

//============================================================
//	�G���[�g�Ɋւ���AI
//============================================================
CPlayer::EMotion CPlayerAI::AIemote(D3DXVECTOR3& rDestRot)
{
	if (m_bEmote)
	{
		int nProb = rand() % 4;

		if (nProb == 0)
		{
			m_currentMotion = CPlayer::MOTION_EMOTE_PROUD;
		}
		else if (nProb == 1)
		{
			m_currentMotion = CPlayer::MOTION_EMOTE_PROUD;
		}
		else if (nProb == 2)
		{
			m_currentMotion = CPlayer::MOTION_EMOTE_PROUD;
		}
		else if (nProb == 3)
		{
			m_currentMotion = CPlayer::MOTION_EMOTE_PROUD;
		}

		rDestRot.y = 0.0f;

		m_bEmote = false;
	}

	return m_currentMotion;
}

//============================================================
//	��������
//============================================================
CPlayerAI *CPlayerAI::Create(const int nPadID)
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
		if (FAILED(pAI->Init(nPadID)))
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
	if(CollisionGround(CPlayer::AXIS_X, rPos))
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

											// �n�Ղ̍ő�̑傫����ݒ�
				sizeMaxGround = pObjCheck->GetVec3Sizing();
				//sizeMaxGround.y = 0.0f;		// �c�̑傫����������

				if (collision::SquareTrigger(posGround, rPos, rotGround, sizeMaxGround, -sizeMinGround))
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
bool CPlayerAI::CollisionBlock(const CPlayer::EAxis axis, D3DXVECTOR3& rPos)
{
	bool bHit = false;	// ���n�̔�����

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
				D3DXVECTOR3 posBlock = VEC3_ZERO;	// �u���b�N�ʒu
				D3DXVECTOR3 rotBlock = VEC3_ZERO;	// �u���b�N����
				D3DXVECTOR3 sizeBlock = VEC3_ZERO;	// �u���b�N�傫��

													// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_BLOCK)
				{ // �I�u�W�F�N�g���x�����u���b�N�ł͂Ȃ��ꍇ

				  // ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �u���b�N�̈ʒu��ݒ�
				posBlock = pObjCheck->GetVec3Position();

				// �u���b�N�̌�����ݒ�
				rotBlock = pObjCheck->GetVec3Rotation();

				// �u���b�N�̑傫����ݒ�
				sizeBlock = pObjCheck->GetVec3Sizing();

				if (collision::SquareTrigger(posBlock, rPos, rotBlock, sizeBlock, -sizeBlock))
				{
					bHit = true;
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}

	// �e���̔������Ԃ�
	return bHit;
}
bool CPlayerAI::CollisionObstacle(D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	bool bHit = false;	// ���n�̔�����
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
				CObstacle::SStatusInfo status;		// ��Q���X�e�[�^�X
				D3DXVECTOR3 posObs = VEC3_ZERO;		// ��Q���ʒu
				D3DXVECTOR3 rotObs = VEC3_ZERO;		// ��Q������
				D3DXVECTOR3 sizeObsMin = VEC3_ZERO;	// ��Q���傫��
				D3DXVECTOR3 sizeObsMax = VEC3_ZERO;	// ��Q���傫��

													// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
				{ // �I�u�W�F�N�g���x������Q���ł͂Ȃ��ꍇ

				  // ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// ��Q���̃X�e�[�^�X��ݒ�
				status = CObstacle::GetStatusInfo(pObjCheck->GetType());

				// ��Q���̌�����ݒ�
				rotObs = pObjCheck->GetVec3Rotation();

				// ��Q���̈ʒu��ݒ�
				posObs = pObjCheck->GetVec3Position();
				posObs.x += sinf(rotObs.y + status.fAngleCenter) * status.fLengthCenter;
				posObs.y = 0.0f;
				posObs.z += cosf(rotObs.y + status.fAngleCenter) * status.fLengthCenter;

				// ��Q���̑傫����ݒ�
				sizeObsMax = status.sizeColl;
				sizeObsMin = sizeObsMax * -1.0f;

				// ��Q���Ƃ̔�������s
				if (collision::SquareTrigger(posObs, rPos, rotObs, sizeObsMax, sizeObsMin))
				{
					bHit = true;
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}

	// �������Ԃ�
	return bHit;
}

void CPlayerAI::SetRotstickL(const float RotstickL)
{
	m_fRotstickL = RotstickL;
}