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

//************************************************************
//	親クラス [CRetentionManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CRetentionManager::CRetentionManager()
{
	// メンバ変数をクリア
	memset(&m_aEntry[0], 0, sizeof(m_aEntry));	// エントリー状況
	m_stateKill		= KILL_LIFE;	// 討伐条件
	m_stateWin		= WIN_SURVIVE;	// 勝利条件
	m_nNumPlayer	= 0;			// プレイヤー数
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
	memset(&m_aEntry[0], 0, sizeof(m_aEntry));	// エントリー状況
	m_stateKill		= KILL_KNOCK;	// 討伐条件
	m_stateWin		= WIN_SURVIVE;	// 勝利条件
	m_nNumPlayer	= 0;			// プレイヤー数

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
//	全エントリー状況の設定処理
//============================================================
void CRetentionManager::AllSetEnableEntry(const bool bEntry)
{
	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		// エントリー状況を設定
		m_aEntry[nCntEntry] = bEntry;
	}
}

//============================================================
//	エントリーの設定処理
//============================================================
void CRetentionManager::SetEnableEntry(const int nID, const bool bEntry)
{
	// 引数インデックスのエントリー状況を設定
	m_aEntry[nID] = bEntry;
}

//============================================================
//	エントリー取得処理
//============================================================
bool CRetentionManager::IsEntry(const int nID) const
{
	// 引数インデックスのエントリー状況を返す
	return m_aEntry[nID];
}
