//============================================================
//
//	AI�N���X�w�b�_�[ [playerAI.h]
//	Author�F������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYERAI_H_
#define _PLAYERAI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "player.h"

//************************************************************
//	�O���錾
//************************************************************
class CFlail;	// �t���C���N���X

//************************************************************
//	���O��Ԑ錾
//************************************************************
// ������
class CPlayerAI
{
public:

	// �R���X�g���N�^
	CPlayerAI();

	// �f�X�g���N�^
	~CPlayerAI();

	HRESULT Init
	(
		CFlail* pFlail,
		D3DXVECTOR3* pPos,
		D3DXVECTOR3* pMove,
		D3DXVECTOR3* pDestRot,
		int* pCounterFlail,
		const int nPadID
	);

	CPlayer::EMotion playerAI(const int nMotionOld);

	void AIselect(void);
	void AIatack(void);
	void AImove(void);

	void AIDash
	(
		D3DXVECTOR3& rMove,
		D3DXVECTOR3& rDashRot,
		D3DXVECTOR3& rDestRot,
		float& rPlusMove,
		bool& rDash
	);

	// �ÓI�����o�֐�
	static CPlayerAI *Create	// ����
	( // ����
		CFlail* pFlail,
		D3DXVECTOR3* pPos,
		D3DXVECTOR3* pMove,
		D3DXVECTOR3* pDestRot,
		int* pCounterFlail,
		const int nPadID
	);

private:

	//�����o�֐�
	bool Collision(D3DXVECTOR3& rPos);		// �����ݒ�
	bool CollisionGround(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);

	// �����o�ϐ�
	CFlail* m_pFlail;
	D3DXVECTOR3* m_pPos;
	D3DXVECTOR3* m_pMove;
	D3DXVECTOR3* m_pDestRot;
	int* m_pCounterFlail;
	int m_nMotionOld;
	int m_nPadID;

	float fRotstickR;

	bool m_bAtack;
};

#endif	// _PLAYERAI_H_