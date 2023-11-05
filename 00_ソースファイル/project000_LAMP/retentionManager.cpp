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
	m_result = RESULT_NONE;	// クリア状況
	m_nTime  = 0;			// 経過タイム
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
	m_result = RESULT_NONE;	// クリア状況
	m_nTime  = 0;			// 経過タイム

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
//	クリア状況の設定処理
//============================================================
void CRetentionManager::SetResult(const EResult result)
{
	if (result > RESULT_NONE && result < RESULT_MAX)
	{ // リザルトが範囲内の場合

		// 引数のクリア状況を設定
		m_result = result;
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	クリア状況取得処理
//============================================================
CRetentionManager::EResult CRetentionManager::GetResult(void) const
{
	// クリア状況を返す
	return m_result;
}

//============================================================
//	経過タイムの設定処理
//============================================================
void CRetentionManager::SetTime(const long nTime)
{
	// 引数の経過タイムを設定
	m_nTime = nTime;
}

//============================================================
//	経過タイム取得処理
//============================================================
long CRetentionManager::GetTime(void) const
{
	// 経過タイムを返す
	return m_nTime;
}
