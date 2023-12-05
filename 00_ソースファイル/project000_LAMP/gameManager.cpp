//============================================================
//
//	ゲームマネージャー処理 [gameManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "gameManager.h"
#include "manager.h"
#include "scene.h"
#include "sceneGame.h"
#include "camera.h"
#include "player.h"
#include "retentionManager.h"
#include "middleResultManager.h"

#include "editStageManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	WAIT_RESULT_FRAME = 120;	// ゲーム画面からゲーム内リザルト画面に移動する際の余韻フレーム
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CMiddleResultManager	*CGameManager::m_pMiddleResult	= NULL;	// 中間リザルトの情報
CEditStageManager		*CGameManager::m_pEditStage		= NULL;	// エディットステージの情報

//************************************************************
//	親クラス [CGameManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGameManager::CGameManager()
{
	// メンバ変数をクリア
	m_state = STATE_NONE;	// 状態
	m_nCounterState = 0;	// 状態管理カウンター
}

//============================================================
//	デストラクタ
//============================================================
CGameManager::~CGameManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGameManager::Init(void)
{
	m_pReady = CReady::Create();
	// メンバ変数を初期化
	m_state = STATE_READY;	// 状態
	m_nCounterState = 0;	// 状態管理カウンター

	// 生存人数を初期化
	CManager::GetInstance()->GetRetentionManager()->InitNumSurvival();

	// 生存ランキングを初期化
	CManager::GetInstance()->GetRetentionManager()->InitSurvivalRank();

	if (m_pMiddleResult == NULL)
	{ // 中間リザルトが使用されていない場合

		// 中間リザルトの生成
		m_pMiddleResult = CMiddleResultManager::Create();
		if (m_pMiddleResult == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else { assert(false); }	// 使用済み

#if _DEBUG

	if (m_pEditStage == NULL)
	{ // エディットステージが使用されていない場合

		// エディットステージの生成
		m_pEditStage = CEditStageManager::Create();
		if (m_pEditStage == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else { assert(false); }	// 使用済み

#endif	// _DEBUG

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGameManager::Uninit(void)
{
	if (m_pMiddleResult != NULL)
	{ // 中間リザルトが使用されている場合

		// 中間リザルトの破棄
		CMiddleResultManager::Release(m_pMiddleResult);
	}

	if (m_pEditStage != NULL)
	{ // エディットステージが使用されている場合

		// エディットステージの破棄
		CEditStageManager::Release(m_pEditStage);
	}
}

//============================================================
//	更新処理
//============================================================
void CGameManager::Update(void)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:
		break;

	case STATE_READY:
		break;

	case STATE_NORMAL:

		if (CManager::GetInstance()->GetRetentionManager()->GetNumSurvival() <= 0)
		{ // 生き残りがいない場合

			// カウンターを加算
			m_nCounterState++;

			if (m_nCounterState >= WAIT_RESULT_FRAME)
			{ // 余韻が経過した場合

				// カウンターを初期化
				m_nCounterState = 0;

				// リザルトに移行
				m_state = STATE_RESULT;
			}
		}

		break;

	case STATE_RESULT:

		if (m_pMiddleResult != NULL)
		{ // 中間リザルトが使用されている場合

			// 中間リザルトの更新
			m_pMiddleResult->Update();
		}

		break;

	case STATE_END:
		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (m_pEditStage != NULL)
	{ // エディットステージが使用されている場合

		// エディットステージの更新
		m_pEditStage->Update();
	}
}

//============================================================
//	状態の設定処理
//============================================================
void CGameManager::SetState(const EState state)
{
	if (state > NONE_IDX && state < STATE_MAX)
	{ // 範囲内の場合

		// 状態を設定
		m_state = state;
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	状態取得処理
//============================================================
CGameManager::EState CGameManager::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	生成処理
//============================================================
CGameManager *CGameManager::Create(void)
{
	// ポインタを宣言
	CGameManager *pGameManager = NULL;	// ゲームマネージャー生成用

	if (pGameManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pGameManager = new CGameManager;	// ゲームマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pGameManager != NULL)
	{ // 使用されている場合
		
		// ゲームマネージャーの初期化
		if (FAILED(pGameManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pGameManager;
			pGameManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pGameManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CGameManager::Release(CGameManager *&prGameManager)
{
	if (prGameManager != NULL)
	{ // 使用中の場合

		// ゲームマネージャーの終了
		prGameManager->Uninit();

		// メモリ開放
		delete prGameManager;
		prGameManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	中間リザルト取得処理
//============================================================
CMiddleResultManager * CGameManager::GetMiddleResult(void)
{
	// 中間リザルトを返す
	return m_pMiddleResult;
}

//============================================================
//	エディットステージ取得処理
//============================================================
CEditStageManager *CGameManager::GetEditStage(void)
{
	// エディットステージの情報を返す
	return m_pEditStage;
}
