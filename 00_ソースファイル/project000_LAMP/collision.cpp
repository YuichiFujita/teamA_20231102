//============================================================
//
//	����֐����� [collision.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "collision.h"
#include "manager.h"
#include "renderer.h"

//============================================================
//	XZ���ʂ̋�`�̓����蔻��
//============================================================
bool collision::Box2D
(
	D3DXVECTOR3 centerPos,		// ����ʒu
	D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
	D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
	D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
	D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
	D3DXVECTOR3 targetSizeDown	// ����ڕW�T�C�Y(���E���E�O)
)
{
	if (centerPos.x + centerSizeUp.x   > targetPos.x - targetSizeDown.x
	&&  centerPos.z + centerSizeUp.z   > targetPos.z - targetSizeDown.z
	&&  centerPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x
	&&  centerPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // ������̏ꍇ

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	3���̋�`�̓����蔻��
//============================================================
bool collision::Box3D
(
	D3DXVECTOR3 centerPos,		// ����ʒu
	D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
	D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
	D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
	D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
	D3DXVECTOR3 targetSizeDown	// ����ڕW�T�C�Y(���E���E�O)
)
{
	if (centerPos.x + centerSizeUp.x   > targetPos.x - targetSizeDown.x
	&&  centerPos.y + centerSizeUp.y   > targetPos.y - targetSizeDown.y
	&&  centerPos.z + centerSizeUp.z   > targetPos.z - targetSizeDown.z
	&&  centerPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x
	&&  centerPos.y - centerSizeDown.y < targetPos.y + targetSizeUp.y
	&&  centerPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // ������̏ꍇ

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	XZ���ʂ̉~�̓����蔻��
//============================================================
bool collision::Circle2D
(
	D3DXVECTOR3 centerPos,	// ����ʒu
	D3DXVECTOR3 targetPos,	// ����ڕW�ʒu
	float fCenterRadius,	// ���蔼�a
	float fTargetRadius,	// ����ڕW���a
	float *pLength			// ����ڕW�Ƃ̋���
)
{
	// �ϐ���錾
	float fLength  = 0.0f;	// ����ʒu�Ɣ���ڕW�ʒu�̊Ԃ̋���

	// ����ʒu�Ɣ���ڕW�ʒu�̋��������߂�
	fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
			+ (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (pLength != NULL)
	{ // �|�C���^���g�p����Ă���ꍇ

		// ����ڕW�Ƃ̋�������
		*pLength = fLength;
	}

	if (fLength < ((fCenterRadius + fTargetRadius) * (fCenterRadius + fTargetRadius)))
	{ // ������̏ꍇ

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	3���̉~�̓����蔻��
//============================================================
bool collision::Circle3D
(
	D3DXVECTOR3 centerPos,	// ����ʒu
	D3DXVECTOR3 targetPos,	// ����ڕW�ʒu
	float fCenterRadius,	// ���蔼�a
	float fTargetRadius		// ����ڕW���a
)
{
	// �ϐ���錾
	float fLength  = 0.0f;	// ����ʒu�Ɣ���ڕW�ʒu�̊Ԃ̋���

	// ����ʒu�Ɣ���ڕW�ʒu�̋��������߂�
	fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
			+ (centerPos.y - targetPos.y) * (centerPos.y - targetPos.y)
			+ (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (fLength < ((fCenterRadius + fTargetRadius) * (fCenterRadius + fTargetRadius)))
	{ // ������̏ꍇ

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	XZ���ʂ̐�`�̓����蔻��
//============================================================
bool collision::Sector
(
	D3DXVECTOR3 centerPos,	// ����ʒu
	D3DXVECTOR3 targetPos,	// ����ڕW�ʒu
	float fCenterRot,		// �������
	float fRadius,			// ���E�͈�
	float fAngle,			// ����p
	float *pLength			// ����ڕW�Ƃ̋���
)
{
	// �ϐ���錾
	bool  bHit = false;		// �����蔻��̌���
	float fLength;			// ���S�ʒu�ƖڕW�ʒu�̋���
	float fHalfAngle;		// �����̊p�x�̔����̒l�̑���p
	D3DXVECTOR3 vecToPos;	// ���[�ƈʒu�̃x�N�g��

	// �ϐ��z���錾
	float fRotEdge[2];		// ��`�̉��̊p�x     [��] 0�F�� 1�F�E
	D3DXVECTOR3 posEdge[2];	// ��`�̉��̐�[�ʒu [��] 0�F�� 1�F�E
	D3DXVECTOR3 vecEdge[2];	// ��`�̉��x�N�g��   [��] 0�F�� 1�F�E

	// ���S�ʒu�ƖڕW�ʒu�̋������߂�
	fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
			+ (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (pLength != NULL)
	{ // �|�C���^���g�p����Ă���ꍇ

		// ����ڕW�Ƃ̋�������
		*pLength = fLength;
	}

	if (fLength < fRadius * fRadius)
	{ // �~�͈͓̔��̏ꍇ

		// �����̊p�x�̔����̒l�����߂�
		fHalfAngle = fAngle * 0.5f;

		// ��`�̍����̊p�x�����߂�
		fRotEdge[0] = fCenterRot + fHalfAngle;	// �p�x�����ɌX����
		useful::NormalizeRot(fRotEdge[0]);		// �����𐳋K��

		// ��`�̉E���̊p�x�����߂�
		fRotEdge[1] = fCenterRot - fHalfAngle;	// �p�x���E�ɌX����
		useful::NormalizeRot(fRotEdge[1]);		// �����𐳋K��

		// ��`�̍����̐�[�ʒu�����߂�
		posEdge[0].x = centerPos.x + sinf(fRotEdge[0]) * 1.0f;
		posEdge[0].y = 0.0f;
		posEdge[0].z = centerPos.z + cosf(fRotEdge[0]) * 1.0f;

		// ��`�̉E���̐�[�ʒu�����߂�
		posEdge[1].x = centerPos.x + sinf(fRotEdge[1]) * 1.0f;
		posEdge[1].y = 0.0f;
		posEdge[1].z = centerPos.z + cosf(fRotEdge[1]) * 1.0f;

		// ��`�̍����̃x�N�g�������߂�
		vecEdge[0] = posEdge[0] - centerPos;
		vecEdge[0].y = 0.0f;

		// ��`�̉E���̃x�N�g�������߂�
		vecEdge[1] = posEdge[1] - centerPos;
		vecEdge[1].y = 0.0f;

		// ���[�ƈʒu�̃x�N�g�������߂�
		vecToPos = targetPos - centerPos;

		if ((vecEdge[0].z * vecToPos.x) - (vecEdge[0].x * vecToPos.z) < 0
		&&  (vecEdge[1].z * vecToPos.x) - (vecEdge[1].x * vecToPos.z) > 0)
		{ // ��`�̗����͈͓̔��̏ꍇ

			// �������Ă����Ԃɂ���
			bHit = true;
		}
	}

	// �����蔻��̌��ʂ�Ԃ�
	return bHit;
}

//============================================================
//	�p���̏Փ˔���
//============================================================
bool collision::BoxPillar
(
	D3DXVECTOR3& rCenterPos,	// ����ʒu
	D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
	D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
	D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
	D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
	D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
	D3DXVECTOR3 targetSizeDown	// ����ڕW�T�C�Y(���E���E�O)
)
{
	// �ϐ���錾
	bool bHit = false;	// �Փ˔��茋��

	// �O��̓����蔻��
	if (rCenterPos.x + centerSizeUp.x > targetPos.x - targetSizeDown.x
	&&  rCenterPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x)
	{ // ���E�͈͓̔��̏ꍇ

		if (rCenterPos.z    + centerSizeUp.z >  targetPos.z - targetSizeDown.z
		&&  rCenterPosOld.z + centerSizeUp.z <= targetPos.z - targetSizeDown.z)
		{ // �O����̓����蔻��

			// �ʒu��␳
			rCenterPos.z = targetPos.z - targetSizeDown.z - centerSizeUp.z;

			// �Փˏ�Ԃɂ���
			bHit = true;
		}
		else if (rCenterPos.z    - centerSizeDown.z <  targetPos.z + targetSizeUp.z
		     &&  rCenterPosOld.z - centerSizeDown.z >= targetPos.z + targetSizeUp.z)
		{ // �ォ��̓����蔻��

			// �ʒu��␳
			rCenterPos.z = targetPos.z + targetSizeUp.z + centerSizeDown.z;

			// �Փˏ�Ԃɂ���
			bHit = true;
		}
	}

	// ���E�̓����蔻��
	if (rCenterPos.z + centerSizeUp.z > targetPos.z - targetSizeDown.z
	&&  rCenterPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // �O��͈͓̔��̏ꍇ

		if (rCenterPos.x    + centerSizeUp.x >  targetPos.x - targetSizeDown.x
		&&  rCenterPosOld.x + centerSizeUp.x <= targetPos.x - targetSizeDown.x)
		{ // ������̓����蔻��

			// �ʒu��␳
			rCenterPos.x = targetPos.x - targetSizeDown.x - centerSizeUp.x;

			// �Փˏ�Ԃɂ���
			bHit = true;
		}
		else if (rCenterPos.x    - centerSizeDown.x <  targetPos.x + targetSizeUp.x
		     &&  rCenterPosOld.x - centerSizeDown.x >= targetPos.x + targetSizeUp.x)
		{ // �E����̓����蔻��
			
			// �ʒu��␳
			rCenterPos.x = targetPos.x + targetSizeUp.x + centerSizeDown.x;

			// �Փˏ�Ԃɂ���
			bHit = true;
		}
	}

	// �Փ˔����Ԃ�
	return bHit;
}

//============================================================
//	�~���̏Փ˔���
//============================================================
bool collision::CirclePillar
(
	D3DXVECTOR3& rCenterPos,	// ����ʒu
	D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
	float fCenterRadius,		// ���蔼�a
	float fTargetRadius			// ����ڕW���a
)
{
	// �ϐ���錾
	float fLength = 0.0f;	// ����ʒu�Ɣ���ڕW�ʒu�̊Ԃ̋���

	// ����ʒu�Ɣ���ڕW�ʒu�̋��������߂�
	fLength = (rCenterPos.x - targetPos.x) * (rCenterPos.x - targetPos.x)
			+ (rCenterPos.z - targetPos.z) * (rCenterPos.z - targetPos.z);

	if (fLength < (fCenterRadius + fTargetRadius) * (fCenterRadius + fTargetRadius))
	{ // ������̏ꍇ

		// �ϐ���錾
		float fCenterRot = atan2f(rCenterPos.x - targetPos.x, rCenterPos.z - targetPos.z);	// ����ڕW���猩���������

		// �ʒu��␳
		rCenterPos.x = targetPos.x + sinf(fCenterRot) * (fCenterRadius + fTargetRadius);
		rCenterPos.z = targetPos.z + cosf(fCenterRot) * (fCenterRadius + fTargetRadius);

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	�O���̋�`�̏Փ˔���
//============================================================
bool collision::ResponseBox3D
(
	D3DXVECTOR3& rCenterPos,	// ����ʒu
	D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
	D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
	D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
	D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
	D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
	D3DXVECTOR3 targetSizeDown,	// ����ڕW�T�C�Y(���E���E�O)
	D3DXVECTOR3 *pMove,	// �ړ���
	bool *pUp,			// �ォ��̔���
	bool *pSide,		// ������̔���
	bool *pDown			// ������̔���
)
{
	// �ϐ���錾
	bool bHit = false;	// �Փ˔��茋��

	// ���E�̓����蔻��
	if (rCenterPos.y + centerSizeUp.y   > targetPos.y - targetSizeDown.y
	&&  rCenterPos.y - centerSizeDown.y < targetPos.y + targetSizeUp.y
	&&  rCenterPos.z + centerSizeUp.z   > targetPos.z - targetSizeDown.z
	&&  rCenterPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // �㉺�ƑO��͈͓̔��̏ꍇ

		if (rCenterPos.x    + centerSizeUp.x >  targetPos.x - targetSizeDown.x
		&&  rCenterPosOld.x + centerSizeUp.x <= targetPos.x - targetSizeDown.x)
		{ // ������̓����蔻��

			// �ʒu��␳
			rCenterPos.x = targetPos.x - targetSizeDown.x - centerSizeUp.x;

			// �Փˏ�Ԃɂ���
			bHit = true;

			if (pMove != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �ړ��ʂ�������
				pMove->x = 0.0f;
			}

			if (pSide != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// ���ɓ������Ă����Ԃ�ݒ�
				*pSide = true;
			}
		}
		else if (rCenterPos.x    - centerSizeDown.x <  targetPos.x + targetSizeUp.x
		     &&  rCenterPosOld.x - centerSizeDown.x >= targetPos.x + targetSizeUp.x)
		{ // �E����̓����蔻��
			
			// �ʒu��␳
			rCenterPos.x = targetPos.x + targetSizeUp.x + centerSizeDown.x;

			// �Փˏ�Ԃɂ���
			bHit = true;

			if (pMove != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �ړ��ʂ�������
				pMove->x = 0.0f;
			}

			if (pSide != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// ���ɓ������Ă����Ԃ�ݒ�
				*pSide = true;
			}
		}
	}

	// �㉺�̓����蔻��
	if (rCenterPos.x + centerSizeUp.x   > targetPos.x - targetSizeDown.x
	&&  rCenterPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x
	&&  rCenterPos.z + centerSizeUp.z   > targetPos.z - targetSizeDown.z
	&&  rCenterPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // ���E�ƑO��͈͓̔��̏ꍇ

		if (rCenterPos.y    + centerSizeUp.y >  targetPos.y - targetSizeDown.y
		&&  rCenterPosOld.y + centerSizeUp.y <= targetPos.y - targetSizeDown.y)
		{ // ������̓����蔻��

			// �ʒu��␳
			rCenterPos.y = targetPos.y - targetSizeDown.y - centerSizeUp.y;

			// �Փˏ�Ԃɂ���
			bHit = true;

			if (pMove != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �ړ��ʂ�������
				pMove->y = 0.0f;
			}

			if (pDown != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// ���ɓ������Ă����Ԃ�ݒ�
				*pDown = true;
			}
		}
		else if (rCenterPos.y    - centerSizeDown.y <  targetPos.y + targetSizeUp.y
		     &&  rCenterPosOld.y - centerSizeDown.y >= targetPos.y + targetSizeUp.y)
		{ // �ォ��̓����蔻��
			
			// �ʒu��␳
			rCenterPos.y = targetPos.y + targetSizeUp.y + centerSizeDown.y;

			// �Փˏ�Ԃɂ���
			bHit = true;

			if (pMove != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �ړ��ʂ�������
				pMove->y = 0.0f;
			}

			if (pUp != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// ��ɓ������Ă����Ԃ�ݒ�
				*pUp = true;
			}
		}
	}

	// �O��̓����蔻��
	if (rCenterPos.x + centerSizeUp.x   > targetPos.x - targetSizeDown.x
	&&  rCenterPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x
	&&  rCenterPos.y + centerSizeUp.y   > targetPos.y - targetSizeDown.y
	&&  rCenterPos.y - centerSizeDown.y < targetPos.y + targetSizeUp.y)
	{ // ���E�Ə㉺�͈͓̔��̏ꍇ

		if (rCenterPos.z    + centerSizeUp.z >  targetPos.z - targetSizeDown.z
		&&  rCenterPosOld.z + centerSizeUp.z <= targetPos.z - targetSizeDown.z)
		{ // �O����̓����蔻��

			// �ʒu��␳
			rCenterPos.z = targetPos.z - targetSizeDown.z - centerSizeUp.z;

			// �Փˏ�Ԃɂ���
			bHit = true;

			if (pMove != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �ړ��ʂ�������
				pMove->z = 0.0f;
			}

			if (pSide != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// ���ɓ������Ă����Ԃ�ݒ�
				*pSide = true;
			}
		}
		else if (rCenterPos.z    - centerSizeDown.z <  targetPos.z + targetSizeUp.z
		     &&  rCenterPosOld.z - centerSizeDown.z >= targetPos.z + targetSizeUp.z)
		{ // �ォ��̓����蔻��

			// �ʒu��␳
			rCenterPos.z = targetPos.z + targetSizeUp.z + centerSizeDown.z;

			// �Փˏ�Ԃɂ���
			bHit = true;

			if (pMove != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �ړ��ʂ�������
				pMove->z = 0.0f;
			}

			if (pSide != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// ���ɓ������Ă����Ԃ�ݒ�
				*pSide = true;
			}
		}
	}

	// �Փ˔����Ԃ�
	return bHit;
}

//============================================================
//	X���̏Փ˔���
//============================================================
bool collision::ResponseSingleX
(
	D3DXVECTOR3& rCenterPos,	// ����ʒu
	D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
	D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
	D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
	D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
	D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
	D3DXVECTOR3 targetSizeDown,	// ����ڕW�T�C�Y(���E���E�O)
	D3DXVECTOR3 *pMove,	// �ړ���
	bool bYColl,		// Y����
	bool bZColl,		// Z����
	bool *pLeft,		// ������̔���
	bool *pRight		// �E����̔���
)
{
	// �ϐ���錾
	bool bColl = true;	// ����\��
	bool bHit = false;	// �Փ˔��茋��

	if (bYColl)
	{ // Y������s���ꍇ

		if (rCenterPos.y + centerSizeUp.y   <= targetPos.y - targetSizeDown.y
		||  rCenterPos.y - centerSizeDown.y >= targetPos.y + targetSizeUp.y)
		{ // �㉺�͈̔͊O�̏ꍇ

			// ����s�\��Ԃɂ���
			bColl = false;
		}
	}

	if (bZColl)
	{ // Z������s���ꍇ

		if (rCenterPos.z + centerSizeUp.z   <= targetPos.z - targetSizeDown.z
		||  rCenterPos.z - centerSizeDown.z >= targetPos.z + targetSizeUp.z)
		{ // �O��͈̔͊O�̏ꍇ

			// ����s�\��Ԃɂ���
			bColl = false;
		}
	}

	// ���E�̓����蔻��
	if (bColl)
	{ // ����\�ȏꍇ

		if (rCenterPos.x    + centerSizeUp.x >  targetPos.x - targetSizeDown.x
		&&  rCenterPosOld.x + centerSizeUp.x <= targetPos.x - targetSizeDown.x)
		{ // ������̓����蔻��

			// �ʒu��␳
			rCenterPos.x = targetPos.x - targetSizeDown.x - centerSizeUp.x;

			// �Փˏ�Ԃɂ���
			bHit = true;

			if (pMove != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �ړ��ʂ�������
				pMove->x = 0.0f;
			}

			if (pLeft != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// ���ɓ������Ă����Ԃ�ݒ�
				*pLeft = true;
			}
		}
		else if (rCenterPos.x    - centerSizeDown.x <  targetPos.x + targetSizeUp.x
		     &&  rCenterPosOld.x - centerSizeDown.x >= targetPos.x + targetSizeUp.x)
		{ // �E����̓����蔻��
			
			// �ʒu��␳
			rCenterPos.x = targetPos.x + targetSizeUp.x + centerSizeDown.x;

			// �Փˏ�Ԃɂ���
			bHit = true;

			if (pMove != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �ړ��ʂ�������
				pMove->x = 0.0f;
			}

			if (pRight != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �E�ɓ������Ă����Ԃ�ݒ�
				*pRight = true;
			}
		}
	}

	// �Փ˔����Ԃ�
	return bHit;
}

//============================================================
//	Y���̏Փ˔���
//============================================================
bool collision::ResponseSingleY
(
	D3DXVECTOR3& rCenterPos,	// ����ʒu
	D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
	D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
	D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
	D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
	D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
	D3DXVECTOR3 targetSizeDown,	// ����ڕW�T�C�Y(���E���E�O)
	D3DXVECTOR3 *pMove,	// �ړ���
	bool bXColl,		// X����
	bool bZColl,		// Z����
	bool *pDown,		// ������̔���
	bool *pUp			// �ォ��̔���
)
{
	// �ϐ���錾
	bool bColl = true;	// ����\��
	bool bHit = false;	// �Փ˔��茋��

	if (bXColl)
	{ // X������s���ꍇ

		if (rCenterPos.x + centerSizeUp.x   <= targetPos.x - targetSizeDown.x
		||  rCenterPos.x - centerSizeDown.x >= targetPos.x + targetSizeUp.x)
		{ // ���E�͈̔͊O�̏ꍇ

			// ����s�\��Ԃɂ���
			bColl = false;
		}
	}

	if (bZColl)
	{ // Z������s���ꍇ

		if (rCenterPos.z + centerSizeUp.z   <= targetPos.z - targetSizeDown.z
		||  rCenterPos.z - centerSizeDown.z >= targetPos.z + targetSizeUp.z)
		{ // �O��͈̔͊O�̏ꍇ

			// ����s�\��Ԃɂ���
			bColl = false;
		}
	}

	// �㉺�̓����蔻��
	if (bColl)
	{ // ����\�ȏꍇ

		if (rCenterPos.y    + centerSizeUp.y >  targetPos.y - targetSizeDown.y
		&&  rCenterPosOld.y + centerSizeUp.y <= targetPos.y - targetSizeDown.y)
		{ // ������̓����蔻��

			// �ʒu��␳
			rCenterPos.y = targetPos.y - targetSizeDown.y - centerSizeUp.y;

			// �Փˏ�Ԃɂ���
			bHit = true;

			if (pMove != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �ړ��ʂ�������
				pMove->y = 0.0f;
			}

			if (pDown != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// ���ɓ������Ă����Ԃ�ݒ�
				*pDown = true;
			}
		}
		else if (rCenterPos.y    - centerSizeDown.y <  targetPos.y + targetSizeUp.y
		     &&  rCenterPosOld.y - centerSizeDown.y >= targetPos.y + targetSizeUp.y)
		{ // �ォ��̓����蔻��
			
			// �ʒu��␳
			rCenterPos.y = targetPos.y + targetSizeUp.y + centerSizeDown.y;

			// �Փˏ�Ԃɂ���
			bHit = true;

			if (pMove != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �ړ��ʂ�������
				pMove->y = 0.0f;
			}

			if (pUp != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// ��ɓ������Ă����Ԃ�ݒ�
				*pUp = true;
			}
		}
	}

	// �Փ˔����Ԃ�
	return bHit;
}

//============================================================
//	Z���̏Փ˔���
//============================================================
bool collision::ResponseSingleZ
(
	D3DXVECTOR3& rCenterPos,	// ����ʒu
	D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
	D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
	D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
	D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
	D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
	D3DXVECTOR3 targetSizeDown,	// ����ڕW�T�C�Y(���E���E�O)
	D3DXVECTOR3 *pMove,	// �ړ���
	bool bXColl,		// X����
	bool bYColl,		// Y����
	bool *pBefore,		// �O����̔���
	bool *pAfter		// �ォ��̔���
)
{
	// �ϐ���錾
	bool bColl = true;	// ����\��
	bool bHit = false;	// �Փ˔��茋��

	if (bXColl)
	{ // X������s���ꍇ

		if (rCenterPos.x + centerSizeUp.x   <= targetPos.x - targetSizeDown.x
		||  rCenterPos.x - centerSizeDown.x >= targetPos.x + targetSizeUp.x)
		{ // ���E�͈̔͊O�̏ꍇ

			// ����s�\��Ԃɂ���
			bColl = false;
		}
	}

	if (bYColl)
	{ // Y������s���ꍇ

		if (rCenterPos.y + centerSizeUp.y   <= targetPos.y - targetSizeDown.y
		||  rCenterPos.y - centerSizeDown.y >= targetPos.y + targetSizeUp.y)
		{ // �㉺�͈̔͊O�̏ꍇ

			// ����s�\��Ԃɂ���
			bColl = false;
		}
	}

	// �O��̓����蔻��
	if (bColl)
	{ // ����\�ȏꍇ

		if (rCenterPos.z    + centerSizeUp.z >  targetPos.z - targetSizeDown.z
		&&  rCenterPosOld.z + centerSizeUp.z <= targetPos.z - targetSizeDown.z)
		{ // �O����̓����蔻��

			// �ʒu��␳
			rCenterPos.z = targetPos.z - targetSizeDown.z - centerSizeUp.z;

			// �Փˏ�Ԃɂ���
			bHit = true;

			if (pMove != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �ړ��ʂ�������
				pMove->z = 0.0f;
			}

			if (pBefore != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �O�ɓ������Ă����Ԃ�ݒ�
				*pBefore = true;
			}
		}
		else if (rCenterPos.z    - centerSizeDown.z <  targetPos.z + targetSizeUp.z
		     &&  rCenterPosOld.z - centerSizeDown.z >= targetPos.z + targetSizeUp.z)
		{ // �ォ��̓����蔻��

			// �ʒu��␳
			rCenterPos.z = targetPos.z + targetSizeUp.z + centerSizeDown.z;

			// �Փˏ�Ԃɂ���
			bHit = true;

			if (pMove != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// �ړ��ʂ�������
				pMove->z = 0.0f;
			}

			if (pAfter != NULL)
			{ // �|�C���^���g�p����Ă���ꍇ

				// ��ɓ������Ă����Ԃ�ݒ�
				*pAfter = true;
			}
		}
	}

	// �Փ˔����Ԃ�
	return bHit;
}

//============================================================
//	�p���̓�������
//============================================================
bool collision::InBoxPillar
(
	D3DXVECTOR3& rCenterPos,	// ����ʒu
	D3DXVECTOR3 originPos,		// ���茴�_�ʒu
	D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
	D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
	D3DXVECTOR3 originSizeUp,	// ���茴�_�T�C�Y(�E�E��E��)
	D3DXVECTOR3 originSizeDown	// ���茴�_�T�C�Y(���E���E�O)
)
{
	// �ϐ���錾
	bool bHit = false;	// ���茋��

	// ���E�̕␳
	if (useful::LimitNum(rCenterPos.x, originPos.x - originSizeDown.x + centerSizeUp.x, originPos.x + originSizeUp.x - centerSizeDown.x))
	{ // �␳���s��ꂽ�ꍇ

		// ���肵����Ԃɂ���
		bHit = true;
	}

	// �O��̕␳
	if (useful::LimitNum(rCenterPos.z, originPos.z - originSizeDown.z + centerSizeUp.z, originPos.z + originSizeUp.z - centerSizeDown.z))
	{ // �␳���s��ꂽ�ꍇ

		// ���肵����Ԃɂ���
		bHit = true;
	}

	// ���茋�ʂ�Ԃ�
	return bHit;
}

//============================================================
//	�~���̓�������
//============================================================
bool collision::InCirclePillar
(
	D3DXVECTOR3& rCenterPos,	// ����ʒu
	D3DXVECTOR3 originPos,		// ���茴�_�ʒu
	float fCenterRadius,		// ���蔼�a
	float fOriginRadius			// ���茴�_���a
)
{
	// �ϐ���錾
	float fLength = 0.0f;	// ����ʒu�Ɣ���ڕW�ʒu�̊Ԃ̋���

	// ����ʒu�Ɣ���ڕW�ʒu�̋��������߂�
	fLength = (rCenterPos.x - originPos.x) * (rCenterPos.x - originPos.x)
			+ (rCenterPos.z - originPos.z) * (rCenterPos.z - originPos.z);

	if (fLength > (fOriginRadius - fCenterRadius) * (fOriginRadius - fCenterRadius))
	{ // ����O�̏ꍇ

		// �ϐ���錾
		float fCenterRot = atan2f(rCenterPos.x - originPos.x, rCenterPos.z - originPos.z);	// ����ڕW���猩���������

		// �ʒu��␳
		rCenterPos.x = originPos.x + sinf(fCenterRot) * (fOriginRadius - fCenterRadius);
		rCenterPos.z = originPos.z + cosf(fCenterRot) * (fOriginRadius - fCenterRadius);

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	�O�ς̍��E����
//============================================================
//	���E�����猩�č��E�ǂ���ɂ��邩�̔���Ɏg�p
//============================================================
float collision::LineOuterProduct
(
	D3DXVECTOR3 posLeft,	// ���E�������W
	D3DXVECTOR3 posRight,	// ���E���E���W
	D3DXVECTOR3 pos			// ����ʒu
)
{
	// �ϐ���錾
	D3DXVECTOR3 vecLine;	// ���E���x�N�g��
	D3DXVECTOR3 vecToPos;	// ���[�ƈʒu�̃x�N�g��

	// ���E���x�N�g�������߂�
	vecLine = posRight - posLeft;

	// ���[�ƈʒu�̃x�N�g�������߂�
	vecToPos = pos - posLeft;

	// �O�ς̌v�Z���ʂ�Ԃ�
	return (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
}

//============================================================
//	�O�ς̏㉺����
//============================================================
//	���E�����猩�ď㉺�ǂ���ɂ��邩�̔���Ɏg�p
//============================================================
float collision::LineHeightOuterProduct
(
	D3DXVECTOR3 posLeft,	// ���E�������W
	D3DXVECTOR3 posRight,	// ���E���E���W
	D3DXVECTOR3 pos			// ����ʒu
)
{
	// �ϐ���錾
	D3DXVECTOR3 vecLine;	// ���E���x�N�g��
	D3DXVECTOR3 vecToPos;	// ���[�ƈʒu�̃x�N�g��

	// ���E���x�N�g�������߂�
	vecLine = posRight - posLeft;

	// ���[�ƈʒu�̃x�N�g�������߂�
	vecToPos = pos - posLeft;

	// �O�ς̌v�Z���ʂ�Ԃ�
	return (vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y);
}

//============================================================
//	�O�p���̓����蔻��
//============================================================
bool collision::TriangleOuterPillar
(
	D3DXVECTOR3 pos0,	// �O�p�̊e���_
	D3DXVECTOR3 pos1,	// �O�p�̊e���_
	D3DXVECTOR3 pos2,	// �O�p�̊e���_
	D3DXVECTOR3 pos		// ����ʒu
)
{
	if (collision::LineOuterProduct(pos0, pos1, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos1, pos2, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos2, pos0, pos) <= 0.0f)
	{ // ������̏ꍇ

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	�l�p���̓����蔻��
//============================================================
bool collision::BoxOuterPillar
(
	D3DXVECTOR3 pos0,	// �l�p�̊e���_
	D3DXVECTOR3 pos1,	// �l�p�̊e���_
	D3DXVECTOR3 pos2,	// �l�p�̊e���_
	D3DXVECTOR3 pos3,	// �l�p�̊e���_
	D3DXVECTOR3 pos		// ����ʒu
)
{
	if (collision::LineOuterProduct(pos0, pos1, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos1, pos2, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos2, pos3, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos3, pos0, pos) <= 0.0f)
	{ // ������̏ꍇ

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
// ���̓����蔻��
//============================================================
bool collision::Square
( // ����
	D3DXVECTOR3 posSquare,			//���̈ʒu
	D3DXVECTOR3 *posTarget,			//�Փ˂������̈ʒu
	D3DXVECTOR3 posTargetOld,		//�Փ˂������̉ߋ��ʒu
	D3DXVECTOR3 rotSquare,			//���̊p�x
	D3DXVECTOR3 offsetMax,			//���̍ő�ʒu�I�t�Z�b�g
	D3DXVECTOR3 offsetMin			//���̍ŏ��ʒu�I�t�Z�b�g
)
{
	D3DXVECTOR3 posCorner[8] = {};									//���̊p
	D3DXVECTOR3 posPlaneCenter[6] = {};								//���̊e�ʂ̒��S
	D3DXVECTOR3 vecIntersect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���Ƃ̏Փ˓_
	D3DXVECTOR3 vecNorPlaneCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���ʂ̖@���x�N�g��
	D3DXPLANE plane = D3DXPLANE(0.0f, 0.0f, 0.0f, 0.0f);			//�e�ʂ̕���
	float lengthX, lengthY, lengthZ;								//�e�������̑傫��

	//�e�������ő傫����}��
	lengthX = fabsf(offsetMax.x - offsetMin.x) * 0.5f;
	lengthY = fabsf(offsetMax.y - offsetMin.y) * 0.5f;
	lengthZ = fabsf(offsetMax.z - offsetMin.z) * 0.5f;

	//���̊e�ʂ̒��S�����߂�
	posPlaneCenter[0] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(lengthX, 0.0f, 0.0f));
	posPlaneCenter[1] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(-lengthX, 0.0f, 0.0f));
	posPlaneCenter[2] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, lengthY, 0.0f));
	posPlaneCenter[3] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, -lengthY, 0.0f));
	posPlaneCenter[4] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, 0.0f, lengthZ));
	posPlaneCenter[5] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, 0.0f, -lengthZ));

	//pos�����̒��ɓ����Ă���
	if (SquareTrigger(posSquare, *posTarget, rotSquare, offsetMax, offsetMin) == true)
	{
		//6�ʑS�ĂŊm���߂�
		for (int nCnt = 0; nCnt < 6; nCnt++)
		{
			//�e�ʂ̖@���x�N�g�����v�Z����
			vecNorPlaneCenter = posSquare - posPlaneCenter[nCnt];
			D3DXVec3Normalize(&vecNorPlaneCenter, &vecNorPlaneCenter);

			//�@���x�N�g�����畽�ʂ̎����v�Z����
			D3DXPlaneFromPointNormal(&plane, &posPlaneCenter[nCnt], &vecNorPlaneCenter);

			//���ʂ̎��Ɠ_����
			if (D3DXPlaneDotCoord(&plane, posTarget) > 0.0f && D3DXPlaneDotCoord(&plane, &posTargetOld) <= 0.0f)
			{
				//���ʂƈʒu�ߋ��ʒu����Փ˓_���Z�o
				D3DXPlaneIntersectLine(&vecIntersect, &plane, posTarget, &posTargetOld);

				//�Փ˓_���C��
				if (posPlaneCenter[nCnt].x == 0.0f)
				{
					vecIntersect.x = 0.0f;
				}

				if (posPlaneCenter[nCnt].y == 0.0f)
				{
					vecIntersect.y = 0.0f;
				}

				if (posPlaneCenter[nCnt].z == 0.0f)
				{
					vecIntersect.z = 0.0f;
				}

				//�ʒu�C��
				*posTarget = vecIntersect - (vecNorPlaneCenter * 7.0f);

				return true;
			}
		}
	}

	return false;
}

//============================================================
//���̓����蔻��(���O����̂�)
//============================================================
bool collision::SquareTrigger
(// ����
	D3DXVECTOR3 posSquare,		//���̈ʒu
	D3DXVECTOR3 posTarget,		//�Փ˂������̈ʒu
	D3DXVECTOR3 rotSquare,		//���̊p�x
	D3DXVECTOR3 offsetMax,		//���̍ő�ʒu�I�t�Z�b�g
	D3DXVECTOR3 offsetMin		//���̍ŏ��ʒu�I�t�Z�b�g
)						  
{
	bool bSquare;							//����pbool
	bSquare = false;

	D3DXVECTOR3 posCorner[8];			//���̊p
	D3DXVECTOR3 posPlaneCenter[6];		//���̊e�ʂ̒��S
	D3DXVECTOR3 vecNorPlaneCenter[6];	//���ʂ̖@���x�N�g��
	D3DXPLANE plane[6];					//�e�ʂ̕���
	int nCheckCollision = 0;			//�ʂœ����������ǂ����m�F�p�ϐ�

	//���̊e�ʂ̒��S�����߂�
	posPlaneCenter[0] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(offsetMax.x, 0.0f, 0.0f));
	posPlaneCenter[1] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(offsetMin.x, 0.0f, 0.0f));
	posPlaneCenter[2] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, offsetMax.y, 0.0f));
	posPlaneCenter[3] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, offsetMin.y, 0.0f));
	posPlaneCenter[4] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, 0.0f, offsetMax.z));
	posPlaneCenter[5] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, 0.0f, offsetMin.z));

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		//�e�ʂ̖@���x�N�g�����v�Z����
		vecNorPlaneCenter[nCnt] = posSquare - posPlaneCenter[nCnt];
		D3DXVec3Normalize(&vecNorPlaneCenter[nCnt], &vecNorPlaneCenter[nCnt]);

		//�@���x�N�g�����畽�ʂ̎����v�Z����
		D3DXPlaneFromPointNormal(&plane[nCnt], &posPlaneCenter[nCnt], &vecNorPlaneCenter[nCnt]);

		//���ʂ̎��Ɠ_����
		if (D3DXPlaneDotCoord(&plane[nCnt], &posTarget) > 0.0f)
		{
			nCheckCollision++;
		}
	}

	//�S�Ă̖ʂœ�����������
	if (nCheckCollision == 6)
	{
		bSquare = true;
	}

	return bSquare;
}

//========================================
//3������Ԃł̍s��ɂ���]���W�ϊ��֐�
//(�C�ӂ̓_����̃I�t�Z�b�g�ʒu���p�x�Ƌ����ŕϊ�)
//========================================
D3DXVECTOR3 collision::PosRelativeMtx
(
	D3DXVECTOR3 posO,		//��]���S�_
	D3DXVECTOR3 rot,		//�p�x
	D3DXVECTOR3 offset		//�I�t�Z�b�g�ʒu
)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	D3DXVECTOR3 posAnswer;																//�ԓ��p�ϐ�
	D3DXMATRIX mtxO, mtxAnswer;															//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;														//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;									//�v�Z�p�}�g���b�N�X

	//�p�[�c�̃��[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&mtxO);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel,
		posO.x, posO.y, posO.z);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxTransModel);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxO);

	mtxPalent = mtxO;

	//�p�[�c�̃��[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&mtxAnswer);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		3.14f, 3.14f, 3.14f);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel,
		offset.x, offset.y, offset.z);
	D3DXMatrixMultiply(&mtxAnswer, &mtxAnswer, &mtxTransModel);

	//�Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���������킹��
	D3DXMatrixMultiply(&mtxAnswer,
		&mtxAnswer,
		&mtxPalent);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxAnswer);

	//�ϐ��p�ϐ��ɑ��
	posAnswer.x = mtxAnswer._41;
	posAnswer.y = mtxAnswer._42;
	posAnswer.z = mtxAnswer._43;

	return posAnswer;
}