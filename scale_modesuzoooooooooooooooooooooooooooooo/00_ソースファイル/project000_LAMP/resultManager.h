//============================================================
//
//	リザルトマネージャーヘッダー [resultManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	マクロ定義
//************************************************************
#define NUM_RESULT	(2)	// リザルト表示に必要なポリゴン数

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;		// オブジェクト2Dクラス
class CTimerManager;	// タイマーマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// リザルトマネージャークラス
class CResultManager
{
public:
	// テクスチャ列挙
	enum ETexture
	{
		TEXTURE_MISSION = 0,	// 遅刻回避テクスチャ
		TEXTURE_CLEAR,			// 成功テクスチャ
		TEXTURE_FAILED,			// 失敗テクスチャ
		TEXTURE_TIME,			// タイム表示テクスチャ
		TEXTURE_CONTINUE,		// コンテニュー表示テクスチャ
		TEXTURE_YES,			// YESテクスチャ
		TEXTURE_NO,				// NOテクスチャ
		TEXTURE_MAX				// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,			// 何もしない状態
		STATE_FADEIN,			// フェードイン状態
		STATE_RESULT,			// リザルト表示状態
		STATE_TIME_WAIT,		// タイム表示待機状態
		STATE_TIME,				// タイム表示状態
		STATE_CONTINUE_WAIT,	// コンテニュー表示待機状態
		STATE_CONTINUE,			// コンテニュー表示状態
		STATE_WAIT,				// 遷移待機状態
		STATE_MAX				// この列挙型の総数
	};

	// 選択列挙
	enum ESelect
	{
		SELECT_YES = 0,	// YES
		SELECT_NO,		// NO
		SELECT_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CResultManager();

	// デストラクタ
	~CResultManager();

	// メンバ関数
	HRESULT Init(void);		// 初期化
	HRESULT Uninit(void);	// 終了
	void Update(void);		// 更新

	// 静的メンバ関数
	static CResultManager *Create(void);	// 生成
	static HRESULT Release(CResultManager *&prResultManager);	// 破棄

private:
	// メンバ関数
	void UpdateFade(void);			// フェードイン
	void UpdateResult(void);		// リザルト表示
	void UpdateTime(void);			// タイム表示
	void UpdateContinue(void);		// コンテニュー表示
	void UpdateSelect(void);		// 選択更新
	void UpdateTransition(void);	// 遷移決定
	void SkipStaging(void);			// 演出スキップ
	void SetTexResult(void);		// リザルト表示のテクスチャ設定
	bool UpdateDrawWait(const int nWait);	// 表示待機

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CObject2D *m_apResult[NUM_RESULT];		// リザルト表示の情報
	CObject2D *m_apContinue[SELECT_MAX];	// コンテニュー表示の情報
	CObject2D *m_pContLogo;	// コンテニューロゴの情報
	CObject2D *m_pTimeLogo;	// タイムロゴの情報
	CObject2D *m_pFade;		// フェードの情報
	CTimerManager *m_pTime;	// タイムの情報
	EState m_state;			// 状態
	int m_nCounterState;	// 状態管理カウンター
	int m_nSelect;			// 現在の選択
	int m_nOldSelect;		// 前回の選択
	float m_fScale;			// ポリゴン拡大率
};

#endif	// _RESULTMANAGER_H_
