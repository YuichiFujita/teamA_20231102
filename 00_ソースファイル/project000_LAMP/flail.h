//============================================================
//
//	フレイルヘッダー [flail.h]
//	Author：中村陸
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FLAIL_H_
#define _FLAIL_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectModel.h"
#include "multiModel.h"
#include "player.h"
#include "objectOrbit.h"

namespace flail
{
	const int	FLAIL_CHARGE = 120;		// フレイル溜め状態
	const int	FLAIL_THROW = 240;		// フレイル投げ状態
	const int	FLAIL_DROP = -1;		// フレイル落下状態
	const int	FLAIL_DEF = 0;			// フレイル取得状態
	const int	FLAIL_NUM_MAX = 80;		// フレイルの最大数
	const int	FLAIL_NUM_MIN = 40;		// フレイルの最小数
	const float	FLAIL_RADIUS = 20.0f;	// フレイルの半径
}

//************************************************************
//	前方宣言
//************************************************************
class CMultiModel;	// モデルクラス

//************************************************************
//	クラス定義
//************************************************************
// フレイルクラス
class CFlail : public CObjectModel
{
public:
	// モデル列挙
	enum EModelFlail
	{
		FLAIL_NORMAL = 0,	// 鉄球
		FLAIL_PREFABHUT,	// プレハブ小屋
		FLAIL_MAX			// この列挙型の総数
	};

	enum EModelChain
	{
		CHAIN_NORMAL = 0,	// 鉄球
		CHAIN_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CFlail();

	// デストラクタ
	~CFlail();

	// 鎖構造体
	struct SChain
	{
		CMultiModel *multiModel;
		D3DXVECTOR3 posOld;
		D3DXVECTOR3 rotOld;
		bool collision;
	};
	
	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	void SetVec3PosOrg(const D3DXVECTOR3& rPosOrg);			// 回転原点設定
	D3DXVECTOR3 GetVec3PosOrg(void);						// 回転原点取得

	void SetPlayerID(const int& rPlayerID);					// プレイヤー番号設定
	int GetPlayerID(void);									// プレイヤー番号取得

	void SetNumChain(const int& rNumChain);					// プレイヤー番号設定
	int GetNumChain(void);									// プレイヤー番号取得

	void SetMove(const D3DXVECTOR3& rMove);					// 位置設定
	D3DXVECTOR3 GetMove(void);								// 位置取得

	void SetChainRot(const float& rChainRot);				// 角度設定
	float GetChainRot(void);								// 角度取得

	void SetChainRotTarget(const float& rChainRotTarget);	// 目標角度設定
	float GetChainRotTarget(void);							// 目標角度取得

	void SetChainRotMove(const float& rChainRotMove);		// 目標角度設定
	float GetChainRotMove(void);							// 目標角度取得

	void SetLengthChain(const float& rLengthChain);			// 長さ設定
	float GetLengthChain(void);								// 長さ取得

	void SetLengthTarget(const float& rLengthTarget);		// 目標長さ設定
	float GetLengthTarget(void);							// 目標長さ取得

	void SetHit(const bool& Hit);						// 目標長さ設定
	bool GetHit(void);									// 目標長さ取得

	void CatchFlail(void);
	void ShotFlail(const float rot);

	void InitOrbit(void);	// 軌跡初期化
	void VanishOrbit(void);	// 軌跡消失

	// 静的メンバ関数
	static CFlail *Create	// 生成
	( // 引数
		CScene::EMode mode,						// モード
		const CPlayer& rPlayer,					// プレイヤー
		const D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXVECTOR3& rScale = VEC3_ONE	// 大きさ
	);

private:
	//メンバ関数
	void Collision(D3DXVECTOR3& rPos);		// 長さ設定
	bool CollisionChain(D3DXVECTOR3& rPos);		// 長さ設定
	bool CollisionGround(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);
	bool CollisionBlock(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);
	bool CollisionObstacle(D3DXVECTOR3& rPos);		// 障害物との当たり判定
	void BindParent(const CPlayer& rPlayer);
	void UpdateChain(void);		// 更新
	void UpdateFlailPos(void);	// 更新
	void UpdateDropFlailPos(float& rRot);	// 更新

	// 静的メンバ変数
	static const char *mc_apModelFileFlail[];	// モデル定数
	static const char *mc_apModelFileChain[];	// モデル定数

	// メンバ変数
	SChain		m_chain[flail::FLAIL_NUM_MAX];	// 鎖
	CObjectOrbit * m_pOrbit;
	D3DXVECTOR3 m_posOrg;						// 回転原点
	D3DXVECTOR3 m_oldPos;						// 過去の位置
	D3DXVECTOR3	m_move;							// 移動量
	int			m_nPlayerID;					// プレイヤーID
	int			m_nNumChain;					// 鎖の数
	int			m_nDamage;						// ダメージ
	int			m_nHitCount;					// ダメージ
	int			m_nfulChainF;					// 鎖の数
	int			m_nfulChainP;					// 鎖の数
	float		m_fChainRot;					// 角度
	float		m_fChainRotOld;					// 角度
	float		m_fChainRotTarget;				// 目標角度
	float		m_fChainRotMove;				// 角速度
	float		m_fLengthChain;					// 鎖の長さ
	float		m_fLengthTarget;				// 鎖の長さ目標
	bool		m_bHit;							// 鎖の衝突チェック

};

#endif	// _FLAIL_H_
