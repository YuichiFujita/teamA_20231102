//============================================================
//
//	モーションヘッダー [motion.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MOTION_H_
#define _MOTION_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_PARTS	(24)	// パーツの最大数
#define MAX_MOTION	(16)	// モーションの最大数
#define MAX_KEY		(8)		// キーの最大数

//************************************************************
//	前方宣言
//************************************************************
class CMultiModel;	// マルチモデルクラス

//************************************************************
//	クラス定義
//************************************************************
// モーションクラス
class CMotion
{
public:
	// コンストラクタ
	CMotion();

	// デストラクタ
	~CMotion();

	// パーツ管理構造体
	struct SKey
	{
		D3DXVECTOR3 pos;	// モデル位置
		D3DXVECTOR3 rot;	// モデル向き
	};

	// ポーズ管理構造体
	struct SKeyInfo
	{
		SKey aKey[MAX_PARTS];	// キーモデル情報
		int  nFrame;	// キー再生フレーム数
	};

	// キー管理構造体
	struct SMotionInfo
	{
		SKeyInfo aKeyInfo[MAX_KEY];	// キー情報
		int  nNumKey;	// キー総数
		bool bLoop;		// ループ ON/OFF
	};

	// モーション管理構造体
	struct SInfo
	{
		SMotionInfo aMotionInfo[MAX_MOTION];	// モーション情報
		int  nNumMotion;	// モーション総数
		int  nType;			// モーション種類
		int  nPose;			// モーションポーズ番号
		int  nCounter;		// モーションカウンター
		bool bFinish;		// モーション終了状況
	};

	// メンバ関数
	HRESULT Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Set(const int nType);	// 設定
	void SetInfo(const SMotionInfo info);					// モーション情報設定
	void SetEnableUpdate(const bool bUpdate);				// 更新状況設定
	void SetModel(CMultiModel **ppModel, const int nNum);	// モデル情報設定
	int  GetType(void) const;			// 種類取得
	int  GetPose(void) const;			// ポーズ番号取得
	int  GetCounter(void) const;		// モーションカウンター取得
	bool IsFinish(void) const;			// 終了取得
	bool IsLoop(const int nType) const;	// ループ取得

	// 静的メンバ関数
	static CMotion *Create(void);				// 生成
	static HRESULT Release(CMotion *&prMotion);	// 破棄

private:
	// メンバ変数
	SInfo m_info;				// モーション情報
	CMultiModel **m_ppModel;	// モデル情報
	int m_nNumModel;			// モデルのパーツ数
	bool m_bUpdate;				// 更新状況
};

#endif	// _MOTION_H_
