//============================================================
//
//	ステータスマネージャーヘッダー [statusManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _STATUS_MANAGER_H_
#define _STATUS_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	前方宣言
//************************************************************
class CMultiValue;	// マルチ数字クラス

//************************************************************
//	クラス定義
//************************************************************
// ステータスマネージャークラス
class CStatusManager
{
public:
	// コンストラクタ
	explicit CStatusManager(const int nPadID);

	// デストラクタ
	~CStatusManager();

	// メンバ関数
	HRESULT Init(void);		// 初期化
	HRESULT Uninit(void);	// 終了
	void Update(void);		// 更新
	void Draw(void);		// 描画

	void AddNumLife(const int nAdd);			// 体力加算
	void SetNumLife(const int nSet);			// 体力設定
	int  GetNumLife(void) const;				// 体力取得
	int  GetNumMaxLife(void) const;				// 最大体力取得
	void SetEnableDrawLife(const bool bDraw);	// 体力の描画設定
	void AddNumRate(const int nAdd);			// 吹っ飛び率加算
	void SetNumRate(const int nSet);			// 吹っ飛び率設定
	int  GetNumRate(void) const;				// 吹っ飛び率取得
	int  GetNumMaxRate(void) const;				// 最大吹っ飛び率取得
	void SetEnableDrawRate(const bool bDraw);	// 吹っ飛び率の描画設定

	// 静的メンバ関数
	static CStatusManager *Create(const int nPadID);	// 生成
	static HRESULT Release(CStatusManager *&prStatusManager);	// 破棄

private:
	// メンバ変数
	CMultiValue *m_pLife;		// 体力の情報
	CMultiValue *m_pKnockRate;	// 吹っ飛び率の情報

	const int m_nPadID;	// パッドID定数
};

#endif	// _STATUS_MANAGER_H_
