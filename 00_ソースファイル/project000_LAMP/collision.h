//============================================================
//
//	����֐��w�b�_�[ [collision.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	���O��Ԑ錾
//************************************************************
// ������
namespace collision
{
	bool Box2D	// XZ���ʂ̋�`�̓����蔻��
	( // ����
		D3DXVECTOR3 centerPos,		// ����ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown	// ����ڕW�T�C�Y(���E���E�O)
	);
	bool Box3D	// 3���̋�`�̓����蔻��
	( // ����
		D3DXVECTOR3 centerPos,		// ����ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown	// ����ڕW�T�C�Y(���E���E�O)
	);
	bool Circle2D	// XZ���ʂ̉~�̓����蔻��
	( // ����
		D3DXVECTOR3 centerPos,	// ����ʒu
		D3DXVECTOR3 targetPos,	// ����ڕW�ʒu
		float fCenterRadius,	// ���蔼�a
		float fTargetRadius,	// ����ڕW���a
		float *pLength = NULL	// ����ڕW�Ƃ̋���
	);
	bool Circle3D	// 3���̉~�̓����蔻��
	( // ����
		D3DXVECTOR3 centerPos,	// ����ʒu
		D3DXVECTOR3 targetPos,	// ����ڕW�ʒu
		float fCenterRadius,	// ���蔼�a
		float fTargetRadius		// ����ڕW���a
	);
	bool Sector	// XZ���ʂ̐�`�̓����蔻��
	( // ����
		D3DXVECTOR3 centerPos,	// ����ʒu
		D3DXVECTOR3 targetPos,	// ����ڕW�ʒu
		float fCenterRot,		// �������
		float fRadius,			// ���E�͈�
		float fAngle,			// ����p
		float *pLength = NULL	// ����ڕW�Ƃ̋���
	);

	bool BoxPillar	// �p���̏Փ˔���
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown	// ����ڕW�T�C�Y(���E���E�O)
	);
	bool CirclePillar	// �~���̏Փ˔���
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		float fCenterRadius,		// ���蔼�a
		float fTargetRadius			// ����ڕW���a
	);

	bool ResponseBox3D	// �O���̋�`�̏Փ˔���
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown,	// ����ڕW�T�C�Y(���E���E�O)
		D3DXVECTOR3 *pMove = NULL,	// �ړ���
		bool *pUp = NULL,	// �ォ��̔���
		bool *pSide = NULL,	// ������̔���
		bool *pDown = NULL	// ������̔���
	);

	bool ResponseSingleX	// X���̏Փ˔���
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown,	// ����ڕW�T�C�Y(���E���E�O)
		D3DXVECTOR3 *pMove = NULL,	// �ړ���
		bool *pLeft = NULL,			// ������̔���
		bool *pRight = NULL			// �E����̔���
	);
	bool ResponseSingleY	// Y���̏Փ˔���
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown,	// ����ڕW�T�C�Y(���E���E�O)
		D3DXVECTOR3 *pMove = NULL,	// �ړ���
		bool *pDown = NULL,			// ������̔���
		bool *pUp = NULL			// �ォ��̔���
	);
	bool ResponseSingleZ	// Z���̏Փ˔���
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown,	// ����ڕW�T�C�Y(���E���E�O)
		D3DXVECTOR3 *pMove = NULL,	// �ړ���
		bool *pBefore = NULL,		// �O����̔���
		bool *pAfter = NULL			// �ォ��̔���
	);

	bool InBoxPillar	// �p���̓�������
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3 originPos,		// ���茴�_�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 originSizeUp,	// ���茴�_�T�C�Y(�E�E��E��)
		D3DXVECTOR3 originSizeDown	// ���茴�_�T�C�Y(���E���E�O)
	);
	bool InCirclePillar	// �~���̓�������
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3 originPos,		// ���茴�_�ʒu
		float fCenterRadius,		// ���蔼�a
		float fOriginRadius			// ���茴�_���a
	);

	float LineOuterProduct	// �O�ς̍��E����
	( // ����
		D3DXVECTOR3 posLeft,	// ���E�������W
		D3DXVECTOR3 posRight,	// ���E���E���W
		D3DXVECTOR3 pos			// ����ʒu
	);
	float LineHeightOuterProduct	// �O�ς̏㉺����
	( // ����
		D3DXVECTOR3 posLeft,	// ���E�������W
		D3DXVECTOR3 posRight,	// ���E���E���W
		D3DXVECTOR3 pos			// ����ʒu
	);

	bool TriangleOuterPillar	// �O�p���̓����蔻��
	( // ����
		D3DXVECTOR3 pos0,	// �O�p�̊e���_
		D3DXVECTOR3 pos1,	// �O�p�̊e���_
		D3DXVECTOR3 pos2,	// �O�p�̊e���_
		D3DXVECTOR3 pos		// ����ʒu
	);
	bool BoxOuterPillar		// �l�p���̓����蔻��
	( // ����
		D3DXVECTOR3 pos0,	// �l�p�̊e���_
		D3DXVECTOR3 pos1,	// �l�p�̊e���_
		D3DXVECTOR3 pos2,	// �l�p�̊e���_
		D3DXVECTOR3 pos3,	// �l�p�̊e���_
		D3DXVECTOR3 pos		// ����ʒu
	);
}

#endif	// _COLLISION_H_
