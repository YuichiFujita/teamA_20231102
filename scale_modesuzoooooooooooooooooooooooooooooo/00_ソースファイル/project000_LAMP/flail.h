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

//************************************************************
//	前方宣言
//************************************************************
class CMultiModel;	// モデルクラス

//************************************************************
//	クラス定義
//************************************************************
// モデルUIクラス
class CFlail : public CObjectModel
{
public:
	// モデル列挙
	enum EModel
	{
		MODEL_FLAIL = 0,	// 鉄球
		MODEL_PREFABHUT,	// プレハブ小屋
		MODEL_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CFlail();

	// デストラクタ
	~CFlail();

	// 鎖構造体
	struct SChain
	{
		CMultiModel *multiModel;
		D3DXVECTOR3 rotOld;
	};
	
	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	void SetVec3PosOrg(const D3DXVECTOR3& rPosOrg);		// 回転原点設定
	D3DXVECTOR3 GetVec3PosOrg(void);					// 回転原点取得

	void SetPlayerID(const int& rPlayerID);				// プレイヤー番号設定
	int GetPlayerID(void);								// プレイヤー番号取得

	void SetMove(const D3DXVECTOR3& rMove);				// 位置設定
	D3DXVECTOR3 GetMove(void);							// 位置取得

	void SetChainRot(const float& rChainRot);			// 角度設定
	float GetChainRot(void);							// 角度取得

	void SetChainRotMove(const float& rChainRotMove);	// 目標角度設定
	float GetChainRotMove(void);						// 目標角度取得

	void SetLengthChain(const float& rLengthChain);		// 長さ設定
	float GetLengthChain(void);							// 長さ取得

	// 静的メンバ関数
	static CFlail *Create	// 生成
	( // 引数
		const CPlayer& rPlayer,					// プレイヤー
		const D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXVECTOR3& rScale = VEC3_ONE	// 大きさ
	);

private:
	//メンバ関数
	void Collision(D3DXVECTOR3& rPos);		// 長さ設定
	void CollisionGround(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);
	void CollisionBlock(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);
	void BindParent(const CPlayer& rPlayer);
	void UpdateChain(void);		// 更新

	// 静的メンバ変数
	static const char *mc_apModelFile[];	// モデル定数

	// メンバ変数
	SChain		m_chain[10];		// 鎖
	D3DXVECTOR3 m_posOrg;			// 回転原点
	D3DXVECTOR3 m_oldPos;			// 過去の位置
	D3DXVECTOR3	m_move;				// 移動量
	int			m_nPlayerID;		// プレイヤーID
	float		m_fChainRot;		// 角度
	float		m_fChainRotMove;	// 目標角度
	float		m_fLengthChain;		// 鎖の長さ

};

#endif	// _FLAIL_H_
