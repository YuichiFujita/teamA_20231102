//============================================================
//
//	ゲームマネージャーヘッダー [gameManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "ready.h"
//************************************************************
//	前方宣言
//************************************************************
class CMiddleResultManager;	// 中間リザルトマネージャークラス
class CEditStageManager;	// エディットステージマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// ゲームマネージャークラス
class CGameManager
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_READY,	// 通常状態
		STATE_NORMAL,	// 通常状態
		STATE_RESULT,	// リザルト状態
		STATE_END,		// 終了状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CGameManager();

	// デストラクタ
	~CGameManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void SetState(const EState state);	// 状態設定
	EState GetState(void) const;		// 状態取得

	// 静的メンバ関数
	static CGameManager *Create(void);	// 生成
	static HRESULT Release(CGameManager *&prGameManager);	// 破棄

	static CMiddleResultManager	*GetMiddleResult(void);	// 中間リザルト取得
	static CEditStageManager	*GetEditStage(void);	// エディットステージ取得

private:
	// 静的メンバ変数
	static CMiddleResultManager	*m_pMiddleResult;	// 中間リザルトの情報
	static CEditStageManager	*m_pEditStage;		// エディットステージの情報

	// メンバ変数
	EState m_state;			// 状態
	int m_nCounterState;	// 状態管理カウンター
	CReady * m_pReady;
};

#endif	// _GAMEMANAGER_H_
