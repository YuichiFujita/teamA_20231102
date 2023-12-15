//============================================================
//
//	データ保存マネージャーヘッダー [retentionManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RETENTION_MANAGER_H_
#define _RETENTION_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "input.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_WINPOINT	(6)	// 最大勝利ポイント
#define MIN_WINPOINT	(1)	// 最小勝利ポイント

//************************************************************
//	クラス定義
//************************************************************
// データ保存マネージャークラス
class CRetentionManager
{
public:
	// 討伐条件列挙
	enum EKill
	{
		KILL_LIFE = 0,	// 体力制
		KILL_KNOCK,		// 吹っ飛ばし制
		KILL_MAX		// この列挙型の総数
	};

	// 勝利条件列挙
	enum EWin
	{
		WIN_SURVIVE = 0,	// 生存数
		WIN_KILL,			// キル数
		WIN_MAX				// この列挙型の総数
	};

	// ランキング列挙
	enum ERank
	{
		RANK_1ST = 0,	// ランキング：一位
		RANK_2ND,		// ランキング：二位
		RANK_3RD,		// ランキング：三位
		RANK_4TH,		// ランキング：四位
		RANK_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CRetentionManager();

	// デストラクタ
	~CRetentionManager();

	// ソート構造体
	struct SSortData
	{
		int nWinPoint;	// 勝利ポイント保持数
		int nPlayerID;	// プレイヤー番号
	};

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了

	void SetKillState(const EKill kill);	// 討伐条件設定
	EKill GetKillState(void) const;			// 討伐条件取得
	void SetWinState(const EWin win);		// 勝利条件設定
	EWin GetWinState(void) const;			// 勝利条件取得
	void SetNumPlayer(const int nNum);		// プレイ人数設定
	int GetNumPlayer(void) const;			// プレイ人数取得
	void InitNumSurvival(void);				// 生存人数初期化
	int GetNumSurvival(void) const;			// 生存人数取得
	void SetWinPoint(const int nWinPoint);	// 勝利ポイント設定
	int GetWinPoint(void) const;			// 勝利ポイント取得
	void InitGame(void);					// ゲーム開始時の初期化

	void AllSetEnableEntry(const bool bEntry, const bool bAI);				// 全エントリー状況設定
	void SetEnableEntry(const int nID, const bool bEntry, const bool bAI);	// エントリー状況設定
	bool IsEntry(const int nID) const;	// エントリー状況取得
	bool IsAI(const int nID) const;		// AI状況取得

	void InitWinRank(void);						// 勝利ランキング初期化
	void SetWinRank(void);						// 勝利ランキング設定
	int GetWinRank1st(void) const;				// 勝利ランキング一位プレイヤー取得
	void InitSurvivalRank(void);				// 生存ランキング初期化
	void SetSurvivalRank(const int nPlayerID);	// 生存ランキング設定
	ERank GetSurvivalRank(const int nID) const;	// 生存ランキング取得
	int GetPlayerWin(const int nID) const;		// プレイヤーポイント数取得

	// 静的メンバ関数
	static CRetentionManager *Create(void);	// 生成
	static HRESULT Release(CRetentionManager *&prRetentionManager);	// 破棄

private:
	// メンバ変数
	EKill	m_stateKill;	// 討伐条件
	EWin	m_stateWin;		// 勝利条件
	int		m_nNumPlayer;	// プレイヤー数
	int		m_nNumSurvival;	// 生存プレイヤー数
	int		m_nWinPoint;	// 勝利ポイント数

	int		m_aSurvivalRank[MAX_PLAYER];	// 降順の生存ランキング
	int		m_aWinRank[MAX_PLAYER];			// 降順の勝利ランキング
	int		m_aPlayerWin[MAX_PLAYER];		// プレイヤーポイント数
	bool	m_aEntry[MAX_PLAYER];			// エントリー状況
	bool	m_aAI[MAX_PLAYER];				// AI状況
};

#endif	// _RETENTION_MANAGER_H_
