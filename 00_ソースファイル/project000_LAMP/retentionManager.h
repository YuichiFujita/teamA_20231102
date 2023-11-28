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
		WIN_SURVIVE = 0,	// 生き残り勝利
		WIN_MAX				// この列挙型の総数
	};

	// 生存ランキング列挙
	enum ERank
	{
		RANK_4TH = 0,	// 生存ランキング：四位
		RANK_3RD,		// 生存ランキング：三位
		RANK_2ND,		// 生存ランキング：二位
		RANK_1ST,		// 生存ランキング：一位
		RANK_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CRetentionManager();

	// デストラクタ
	~CRetentionManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了

	void SetKillState(const EKill kill);	// 討伐条件設定
	EKill GetKillState(void) const;			// 討伐条件取得
	void SetWinState(const EWin win);		// 勝利条件設定
	EWin GetWinState(void) const;			// 勝利条件取得
	void SetNumPlayer(const int nNum);		// プレイ人数設定
	int GetNumPlayer(void) const;			// プレイ人数取得

	void AllSetEnableEntry(const bool bEntry);				// 全エントリー状況設定
	void SetEnableEntry(const int nID, const bool bEntry);	// エントリー状況設定
	bool IsEntry(const int nID) const;						// エントリー状況取得

	void InitSurvivalRank(void);				// 生存ランキング初期化
	void SetSurvivalRank(const int nPlayerID);	// 生存ランキング設定
	ERank GetSurvivalRank(const int nID) const;	// 生存ランキング取得

	// 静的メンバ関数
	static CRetentionManager *Create(void);	// 生成
	static HRESULT Release(CRetentionManager *&prRetentionManager);	// 破棄

private:
	// メンバ変数
	EKill	m_stateKill;	// 討伐条件
	EWin	m_stateWin;		// 勝利条件
	int		m_nNumPlayer;	// プレイヤー数

	int		m_aSurvivalRank[MAX_PLAYER];	// 昇順の生存ランキング
	bool	m_aEntry[MAX_PLAYER];			// エントリー状況
};

#endif	// _RETENTION_MANAGER_H_
