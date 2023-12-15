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

	// �s���p�^�[����
	enum EStateAI
	{
		STATEAI_NONE = 0,	// �Î~���
		STATEAI_MOVE,		// �ړ����
		STATEAI_ATTACK,		// �U�����
		STATEAI_EMOTE,		// �G���[�g���
		STATEAI_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CPlayerAI();

	// �f�X�g���N�^
	~CPlayerAI();

	HRESULT Init(const int nPadID);

	CPlayer::EMotion playerAI
	(
		CFlail* pFlail,
		D3DXVECTOR3& rPos,
		D3DXVECTOR3& rMove,
		D3DXVECTOR3& rDestRot,
		int pCounterFlail,
		const int nMotionOld
	);

	CPlayer::EMotion AIselect
	(
		CFlail* pFlail,
		D3DXVECTOR3& rPos,
		D3DXVECTOR3& rMove,
		D3DXVECTOR3& rDestRot,
		int pCounterFlail,
		const int nMotionOld
	);

	CPlayer::EMotion AIattack
	(
		CFlail* pFlail,
		D3DXVECTOR3& rPos,
		D3DXVECTOR3& rMove,
		D3DXVECTOR3& rDestRot,
		int pCounterFlail,
		const int nMotionOld
	);

	CPlayer::EMotion AImove
	(
		CFlail* pFlail,
		D3DXVECTOR3& rPos,
		D3DXVECTOR3& rMove,
		D3DXVECTOR3& rDestRot,
		int pCounterFlail,
		const int nMotionOld
	);

	CPlayer::EMotion AIemote(D3DXVECTOR3& rDestRot);

	void AIDash
	(
		D3DXVECTOR3& rPos,
		D3DXVECTOR3& rMove,
		D3DXVECTOR3& rDashRot,
		D3DXVECTOR3& rDestRot,
		float& rPlusMove,
		int& rCounterFlail,
		bool& rDash
	);

	void SetRotstickL(const float RotstickL);

	// �ÓI�����o�֐�
	static CPlayerAI *Create(const int nPadID);	// ����

private:

	//�����o�֐�
	bool Collision(D3DXVECTOR3& rPos);		// �����ݒ�
	bool CollisionGround(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);
	bool CollisionBlock(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);
	bool CollisionObstacle(D3DXVECTOR3& rPos);

	// �����o�ϐ�
	CFlail* m_pFlail;
	D3DXVECTOR3 m_pDashRot;
	D3DXVECTOR3 m_oldPos;
	EStateAI m_stateAI;
	CPlayer::EMotion m_currentMotion;
	int m_nCounterFlail;
	int m_nMotionOld;
	int m_nPadID;
	int m_nApproachNum;
	int m_nCountAI;
	bool m_bDash;

	float m_fRotstickR;
	float m_fRotstickL;

	bool m_bAttack;
	bool m_bMove;
	bool m_bEmote;
};

#endif	// _PLAYERAI_H_