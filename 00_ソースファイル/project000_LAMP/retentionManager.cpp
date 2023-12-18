//============================================================
//
//	データ保存マネージャー処理 [retentionManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "retentionManager.h"
#include "manager.h"
#include "player.h"

//************************************************************
//	マクロ定義
//************************************************************
#define INIT_WINPOINT	(3)		// 初期勝利ポイント

//************************************************************
//	親クラス [CRetentionManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CRetentionManager::CRetentionManager()
{
	// メンバ変数をクリア
	memset(&m_aSurvivalRank[0], 0, sizeof(m_aSurvivalRank));	// 降順の生存ランキング
	memset(&m_aWinRank[0], 0, sizeof(m_aWinRank));				// 降順の勝利ランキング
	memset(&m_aPlayerWin[0], 0, sizeof(m_aPlayerWin));			// プレイヤーポイント数
	memset(&m_aEntry[0], 0, sizeof(m_aEntry));					// エントリー状況
	m_stateKill		= KILL_LIFE;	// 討伐条件
	m_stateWin		= WIN_SURVIVE;	// 勝利条件
	m_nNumPlayer	= 0;			// プレイヤー数
	m_nNumSurvival	= 0;			// 生存プレイヤー数
	m_nWinPoint		= 0;			// 勝利ポイント数
}

//============================================================
//	デストラクタ
//============================================================
CRetentionManager::~CRetentionManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CRetentionManager::Init(void)
{
	// メンバ変数を初期化
	memset(&m_aSurvivalRank[0], 0, sizeof(m_aSurvivalRank));	// 降順の生存ランキング
	memset(&m_aWinRank[0], 0, sizeof(m_aWinRank));				// 降順の勝利ランキング
	memset(&m_aPlayerWin[0], 0, sizeof(m_aPlayerWin));			// プレイヤーポイント数
	memset(&m_aEntry[0], 0, sizeof(m_aEntry));					// エントリー状況
	m_stateKill		= KILL_LIFE;		// 討伐条件
	m_stateWin		= WIN_SURVIVE;		// 勝利条件
	m_nNumPlayer	= 0;				// プレイヤー数
	m_nNumSurvival	= 0;				// 生存プレイヤー数
	m_nWinPoint		= INIT_WINPOINT;	// 勝利ポイント数

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		// エントリー状況を初期化
		m_aEntry[nCntEntry] = false;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CRetentionManager::Uninit(void)
{

}

//============================================================
//	生成処理
//============================================================
CRetentionManager *CRetentionManager::Create(void)
{
	// ポインタを宣言
	CRetentionManager *pRetentionManager = NULL;	// データ保存マネージャー生成用

	if (pRetentionManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pRetentionManager = new CRetentionManager;	// データ保存マネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pRetentionManager != NULL)
	{ // 使用されている場合
		
		// データ保存マネージャーの初期化
		if (FAILED(pRetentionManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pRetentionManager;
			pRetentionManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pRetentionManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CRetentionManager::Release(CRetentionManager *&prRetentionManager)
{
	if (prRetentionManager != NULL)
	{ // 使用中の場合

		// データ保存マネージャーの終了
		prRetentionManager->Uninit();

		// メモリ開放
		delete prRetentionManager;
		prRetentionManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	討伐条件の設定処理
//============================================================
void CRetentionManager::SetKillState(const EKill kill)
{
	if (kill > NONE_IDX && kill < KILL_MAX)
	{ // 討伐条件が範囲内の場合

		// 引数の討伐条件を設定
		m_stateKill = kill;
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	討伐条件取得処理
//============================================================
CRetentionManager::EKill CRetentionManager::GetKillState(void) const
{
	// 討伐条件を返す
	return m_stateKill;
}

//============================================================
//	勝利条件の設定処理
//============================================================
void CRetentionManager::SetWinState(const EWin win)
{
	if (win > NONE_IDX && win < KILL_MAX)
	{ // 勝利条件が範囲内の場合

		// 引数の勝利条件を設定
		m_stateWin = win;
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	勝利条件取得処理
//============================================================
CRetentionManager::EWin CRetentionManager::GetWinState(void) const
{
	// 勝利条件を返す
	return m_stateWin;
}

//============================================================
//	プレイ人数の設定処理
//============================================================
void CRetentionManager::SetNumPlayer(const int nNum)
{
	// プレイ人数を設定
	m_nNumPlayer = nNum;
}

//============================================================
//	プレイ人数取得処理
//============================================================
int CRetentionManager::GetNumPlayer(void) const
{
	// プレイ人数を返す
	return m_nNumPlayer;
}

//============================================================
//	生存人数の初期化処理
//============================================================
void CRetentionManager::InitNumSurvival(void)
{
	// プレイヤー人数分を生存人数に設定
	m_nNumSurvival = m_nNumPlayer;
}

//============================================================
//	生存人数取得処理
//============================================================
int CRetentionManager::GetNumSurvival(void) const
{
	// 生存人数を返す
	return m_nNumSurvival;
}

//============================================================
//	勝利ポイントの設定処理
//============================================================
void CRetentionManager::SetWinPoint(const int nWinPoint)
{
	// 引数の勝利ポイントを設定
	m_nWinPoint = nWinPoint;
}

//============================================================
//	勝利ポイント取得処理
//============================================================
int CRetentionManager::GetWinPoint(void) const
{
	// 勝利ポイントを返す
	return m_nWinPoint;
}

//============================================================
//	ゲーム開始時の初期化処理
//============================================================
void CRetentionManager::InitGame(void)
{
	// 生存ランキングを初期化
	InitSurvivalRank();

	// 勝利ランキングを初期化
	InitWinRank();

	// プレイヤーポイント数を初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // プレイヤーの最大数分繰り返す

		m_aPlayerWin[nCntPlayer] = 0;
	}
}

//============================================================
//	全エントリー状況の設定処理
//============================================================
void CRetentionManager::AllSetEnableEntry(const bool bEntry, const bool bAI)
{
	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		// エントリー状況を設定
		m_aEntry[nCntEntry] = bEntry;

		// AI状況を設定
		m_aAI[nCntEntry] = bAI;
	}
}

//============================================================
//	エントリーの設定処理
//============================================================
void CRetentionManager::SetEntry(const int nID, const bool bEntry, const bool bAI)
{
	if (bEntry)
	{ // エントリーする場合

		if (!m_aEntry[nID])
		{ // エントリー情報が無い場合

			// プレイヤー数を加算
			m_nNumPlayer++;
		}

		// 引数インデックスのエントリー状況を設定
		m_aEntry[nID] = bEntry;

		// 引数インデックスのAI状況を設定
		m_aAI[nID] = bAI;
	}
	else
	{ // エントリーを取り消す場合

		if (m_aEntry[nID])
		{ // エントリー情報がある場合

			// プレイヤー数を減算
			m_nNumPlayer--;
		}

		// 引数インデックスのエントリー状況を設定
		m_aEntry[nID] = bEntry;

		// 引数インデックスのAI状況を設定
		m_aAI[nID] = bAI;
	}
}

//============================================================
//	エントリー取得処理
//============================================================
bool CRetentionManager::IsEntry(const int nID) const
{
	// 引数インデックスのエントリー状況を返す
	return m_aEntry[nID];
}

//============================================================
//	AI取得処理
//============================================================
bool CRetentionManager::IsAI(const int nID) const
{
	// 引数インデックスのAI状況を返す
	return m_aAI[nID];
}

//============================================================
//	勝利ランキング初期化処理
//============================================================
void CRetentionManager::InitWinRank(void)
{
	// 勝利ランキングを初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // プレイヤーの最大数分繰り返す

		m_aWinRank[nCntPlayer] = RANK_4TH;
	}
}

//============================================================
//	勝利ランキング設定処理
//============================================================
void CRetentionManager::SetWinRank(void)
{
	// 変数配列を宣言
	SSortData aSortData[MAX_PLAYER];	// ソート情報代入用
	for (int i = 0; i < MAX_PLAYER; i++)
	{ // プレイヤー最大数分繰り返す

		// ソート情報を初期化
		aSortData[i].nWinPoint = m_aPlayerWin[i];	// 勝利ポイント保持数
		aSortData[i].nPlayerID = i;					// プレイヤー番号
	}

	// ソート情報を用いて勝利ランキングを更新
	{
		// 変数を宣言
		SSortData keepData;	// ソート用
		int	nCurrentMaxID;	// 最大値のインデックス

		for (int nCntKeep = 0; nCntKeep < (MAX_PLAYER - 1); nCntKeep++)
		{ // 入れ替える値の総数 -1回分繰り返す

			// 現在の繰り返し数を代入 (要素1とする)
			nCurrentMaxID = nCntKeep;

			for (int nCntSort = nCntKeep + 1; nCntSort < MAX_PLAYER; nCntSort++)
			{ // 入れ替える値の総数 -nCntKeep分繰り返す

				if (aSortData[nCurrentMaxID].nWinPoint < aSortData[nCntSort].nWinPoint)
				{ // 最大値に設定されている値より、現在の値のほうが大きい場合

					// 現在の要素番号を最大値に設定
					nCurrentMaxID = nCntSort;
				}
			}

			if (nCntKeep != nCurrentMaxID)
			{ // 最大値の要素番号に変動があった場合

				// 要素の入れ替え
				keepData = aSortData[nCntKeep];
				aSortData[nCntKeep] = aSortData[nCurrentMaxID];
				aSortData[nCurrentMaxID] = keepData;
			}
		}
	}

	// 更新した勝利ランキングを元にプレイヤーごとの順位を設定
	{
		// 変数を宣言
		int nOldPoint = NONE_IDX;		// 一つ後のプレイヤーの獲得ポイント数
		int nCurrentRank = NONE_IDX;	// 現在の最高ランキング

		for (int nCntRank = 0; nCntRank < m_nNumPlayer; nCntRank++)
		{ // プレイヤーの人数分繰り返す

			if (nOldPoint == aSortData[nCntRank].nWinPoint)
			{ // 同じ獲得ポイント数の場合

				// 前回の順位を保持しているプレイヤー番号の配列要素に設定
				m_aWinRank[aSortData[nCntRank].nPlayerID] = nCurrentRank;
			}
			else
			{ // 同じ獲得ポイント数ではない場合

				// 現在の最高ランキングを下げる
				nCurrentRank++;

				// 今回の順位を保持しているプレイヤー番号の配列要素に設定
				m_aWinRank[aSortData[nCntRank].nPlayerID] = nCurrentRank;
			}

			// 現在のプレイヤーの獲得ポイント数を保存
			nOldPoint = aSortData[nCntRank].nWinPoint;
		}
	}
}

//============================================================
//	勝利ランキング取得処理
//============================================================
CRetentionManager::ERank CRetentionManager::GetWinRank(const int nID)
{
	return (ERank)m_aWinRank[nID];
}

//============================================================
//	勝利ランキング一位プレイヤーのインデックス取得処理
//============================================================
int CRetentionManager::GetWinRank1st(void) const
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // プレイヤーの最大数分繰り返す

		if (m_aWinRank[nCntPlayer] == RANK_1ST)
		{ // 現在のインデックスがランキング一位の場合

			// 一位プレイヤーのインデックスを返す
			return nCntPlayer;
		}
	}

	// ランキング1位が存在しない
	assert(false);
	return NONE_IDX;	// 例外を返す
}

//============================================================
//	生存ランキング初期化処理
//============================================================
void CRetentionManager::InitSurvivalRank(void)
{
	// 生存ランキングを初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // プレイヤーの最大数分繰り返す

		m_aSurvivalRank[nCntPlayer] = NONE_IDX;
	}
}

//============================================================
//	生存ランキング設定処理
//============================================================
void CRetentionManager::SetSurvivalRank(const int nPlayerID)
{
	// 生存ランキングを更新
	for (int nCntPlayer = m_nNumPlayer - 1; nCntPlayer >= 0; nCntPlayer--)
	{ // プレイヤーの最大数分繰り返す

		if (m_aSurvivalRank[nCntPlayer] == NONE_IDX)
		{ // ランキングが設定されていない場合

			// 引数のプレイヤーインデックスを設定
			m_aSurvivalRank[nCntPlayer] = nPlayerID;

			// 生存人数を減算
			m_nNumSurvival--;

			switch (m_stateWin)
			{ // 勝利条件ごとの処理
			case WIN_SURVIVE:

				if (m_nNumSurvival <= 0)
				{ // だれも生存していない場合

					// 生存ランキング1位プレイヤーの勝利ポイントを加算
					m_aPlayerWin[m_aSurvivalRank[RANK_1ST]]++;
				}

				break;

			case WIN_KILL:

				for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
				{ // プレイヤーの最大数分繰り返す

					CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);	// プレイヤー情報
					if (pPlayer == NULL)
					{ // 存在しない場合

						break;
					}

					if (pPlayer->GetPadID() == nPlayerID)
					{ // 今回死亡したプレイヤーの場合

						CPlayer *pPlayerAttack = pPlayer->GetLastAttackPlayer();	// 最終攻撃プレイヤー情報
						if (pPlayerAttack == NULL)
						{ // 攻撃されていない場合

							break;
						}

						if (pPlayerAttack->GetState() != CPlayer::STATE_DEATH)
						{ // 最後に攻撃してきたプレイヤーが生きている場合

							// 最終攻撃を与えたプレイヤーの勝利ポイントを加算
							m_aPlayerWin[pPlayerAttack->GetPadID()]++;
						}
					}
				}

				break;

			default:
				assert(false);
				break;
			}

			if (m_nNumSurvival <= 0)
			{ // だれも生存していない場合

				// 勝利ランキング設定 (更新)
				SetWinRank();
			}

			// 処理を抜ける
			break;
		}
	}
}

//============================================================
//	生存ランキング取得処理
//============================================================
CRetentionManager::ERank CRetentionManager::GetSurvivalRank(const int nID) const
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // プレイヤーの最大数分繰り返す

		if (m_aSurvivalRank[nCntPlayer] == nID)
		{ // ランキングが設定されていない場合

			// 引数のプレイヤーインデックスを設定
			return (ERank)nCntPlayer;
		}
	}

	// 4位を返す (例外)
	assert(false);
	return RANK_4TH;
}

//============================================================
//	プレイヤーポイント数取得処理
//============================================================
int CRetentionManager::GetPlayerWin(const int nID) const
{
	if (nID > NONE_IDX && nID < MAX_PLAYER)
	{ // インデックスが範囲内の場合

		// 引数プレイヤーのポイントを返す
		return m_aPlayerWin[nID];
	}
	else { assert(false); return NONE_IDX; }	// 範囲外
}
