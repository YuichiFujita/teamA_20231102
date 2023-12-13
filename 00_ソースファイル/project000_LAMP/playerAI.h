//============================================================
//
//	AIクラスヘッダー [playerAI.h]
//	Author：中村陸
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYERAI_H_
#define _PLAYERAI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "player.h"

//************************************************************
//	前方宣言
//************************************************************
class CFlail;	// フレイルクラス

//************************************************************
//	名前空間宣言
//************************************************************
// 判定空間
class CPlayerAI
{
public:

	// 行動パターン列挙
	enum EStateAI
	{
		STATEAI_NONE = 0,	// X軸
		STATEAI_MOVE,		// Y軸
		STATEAI_ATTACK,		// Z軸
		STATEAI_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CPlayerAI();

	// デストラクタ
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

	// 静的メンバ関数
	static CPlayerAI *Create(const int nPadID);	// 生成

private:

	//メンバ関数
	bool Collision(D3DXVECTOR3& rPos);		// 長さ設定
	bool CollisionGround(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);
	bool CollisionBlock(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);
	bool CollisionObstacle(D3DXVECTOR3& rPos);

	// メンバ変数
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
};

#endif	// _PLAYERAI_H_