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
#define	RESULT_PRIO	(14)	// リザルトの優先順位
#define	NUM_WIN		(2)		// 勝利ロゴの数
#define	NUM_FRAME	(4)		// フレームの数

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;		// オブジェクト2Dクラス
class CAnim2D;		// オブジェクト2Dクラス
class CTimerManager;	// タイマーマネージャークラス
class CPlayerEntry;

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

		TEXTURE_WHO = 0,
		TEXTURE_WIN,
		TEXTURE_WINNERFRAME,
		TEXTURE_FRAME,
		TEXTURE_RESTART,
		TEXTURE_BACK,
		TEXTURE_ICON_PLAYER1,
		TEXTURE_ICON_PLAYER2,
		TEXTURE_ICON_PLAYER3,
		TEXTURE_ICON_PLAYER4,
		TEXTURE_BLACKOUT,
		TEXTURE_PLAYER_ENTRY,
		TEXTURE_WIN2,
		TEXTURE_RANK,
		TEXTURE_REMACH_BG,
		TEXTURE_MAX				// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,			// 何もしない状態
		STATE_FADEIN,			// フェードイン状態
		STATE_WIN,
		STATE_BIG_FRAME,
		STATE_FRAME,
		STATE_HOLD,
		STATE_SELECT,
		STATE_WAIT,				// 遷移待機状態
		STATE_MAX				// この列挙型の総数
	};

	//ここの処理で使っているオブジェクトの列挙型
	enum EObj
	{
		OBJ_WIN = 0,	//勝利ロゴ
		OBJ_WIN_NUM,
		OBJ_BIGFRAME,
		OBJ_FRAME,		//フレーム
		OBJ_ICON,
		OBJ_PLAYER,
		OBJ_NUMBER,
		OBJ_COVER,
		OBJ_SELECT,		//セレクト
		OBJ_MAX
	};

	// 選択列挙
	enum ESelect
	{
		SELECT_YES = 0,	// YES
		SELECT_NO,		// NO
		SELECT_MAX		// この列挙型の総数
	};
	//<******************************
	//ランクの列挙型
	//<******************************
	enum RANK
	{
		RANK_FIRST = 0,	//一位
		RANK_SECOND,	//二位
		RANK_THIRD,		//三位
		RANK_FOURTH,	//四位
		RANK_MAX
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
	void UpdateSelect(void);		// 選択更新
	void UpdateTransition(void);	// 遷移決定
	void SkipStaging(void);			// 演出スキップ
	void UpdateWin(void);			// 勝利ロゴ
	void UpdateBigFrame(void);		// 巨大フレーム
	void UpdateFrame(void);			// フレーム
	void UpdateNumber(void);		// 番号
	void UpdateCover(void);			// カバー

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CAnim2D *m_apWinLog[NUM_WIN];		// 勝利ロゴ
	CObject2D *m_pBigFrame;				// 巨大フレーム
	CObject2D *m_apFrame[NUM_FRAME];	// フレーム
	CAnim2D *m_apNumber[NUM_FRAME];		// フレーム
	CObject2D *m_apSelect[SELECT_MAX];	// 選択肢
	CObject2D *m_pFade;					// フェードの情報
	CObject2D *m_pCover;				//カバー
	CObject2D *m_apIcon[NUM_FRAME];		//プレイヤーのアイコン
	CAnim2D *m_apWinNum[NUM_FRAME];
	CObject2D *m_apPlayerEntry[NUM_FRAME];
	EState m_state;						// 状態
	D3DXVECTOR3 m_arPos[OBJ_MAX];		// サイズ
	D3DXVECTOR3 m_rPos[5];
	D3DXVECTOR3 m_arOriginPos[OBJ_MAX][NUM_FRAME];
	D3DXVECTOR3 m_arSize[OBJ_MAX];		// 位置
	int m_nCounterState;				// 状態管理カウンター
	int m_nSelect;						// 現在の選択
	int m_nOldSelect;					// 前回の選択
	int m_anNum[OBJ_MAX];				//　数
	int m_nNumPlay;						//何人遊んでいるか
	int m_nPattern;						//番号パターン
	int m_anWinPoint[RANK::RANK_MAX];	//勝利数
	int m_anWaitTime[OBJ_MAX];			//待機時間
	int m_anRank[RANK::RANK_MAX];		//順位順に並べるための変数
	int m_anSaveRank[RANK::RANK_MAX];	//順位順の値を保存
	bool m_bSkiped;						//スキップされているかどうか
	bool m_abSizeFinish[OBJ_MAX];		//サイズ拡大・縮小が終了しているか
	bool m_abool[OBJ_MAX];				// 判定用
};

#endif	// _RESULTMANAGER_H_
