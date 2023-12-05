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

	// コンストラクタ
	CPlayerAI();

	// デストラクタ
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

	// 静的メンバ関数
	static CPlayerAI *Create	// 生成
	( // 引数
		CFlail* pFlail,
		D3DXVECTOR3* pPos,
		D3DXVECTOR3* pMove,
		D3DXVECTOR3* pDestRot,
		int* pCounterFlail,
		const int nPadID
	);

private:

	//メンバ関数
	bool Collision(D3DXVECTOR3& rPos);		// 長さ設定
	bool CollisionGround(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);

	// メンバ変数
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