//============================================================
//
//	ゲーム画面処理 [sceneGame.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneGame.h"
#include "manager.h"
#include "sound.h"
#include "input.h"

#include "gameManager.h"
#include "timerManager.h"
#include "stage.h"
#include "pause.h"
#include "player.h"

//************************************************************
//	マクロ定義
//************************************************************

#if _DEBUG

#define TIME_LIMIT	(0)	// 制限時間

#else

#define TIME_LIMIT	(240)	// 制限時間

#endif

#define TIME_POS		(D3DXVECTOR3(710.0f, 50.0f, 0.0f))	// タイマー位置
#define TIME_VAL_SIZE	(D3DXVECTOR3(72.0f, 96.0f, 0.0f))	// タイマー数字大きさ
#define TIME_PART_SIZE	(D3DXVECTOR3(42.0f, 96.0f, 0.0f))	// タイマー区切り大きさ
#define TIME_VAL_SPACE	(D3DXVECTOR3(TIME_VAL_SIZE.x  * 0.85f, 0.0f, 0.0f))	// タイマー数字空白
#define TIME_PART_SPACE	(D3DXVECTOR3(TIME_PART_SIZE.x * 0.85f, 0.0f, 0.0f))	// タイマー区切り空白

#define SCO_POS		(D3DXVECTOR3(825.0f, 50.0f, 0.0f))		// スコア位置
#define SCO_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// スコア大きさ
#define SCO_SPACE	(D3DXVECTOR3(SCO_SIZE.x, 0.0f, 0.0f))	// スコア空白

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CGameManager	*CSceneGame::m_pGameManager  = NULL;	// ゲームマネージャー
CTimerManager	*CSceneGame::m_pTimerManager = NULL;	// タイマーマネージャー
CPause	*CSceneGame::m_pPause	= NULL;					// ポーズ

bool CSceneGame::m_bControlCamera = false;	// カメラの操作状況
bool CSceneGame::m_bDrawUI = true;			// UIの描画状況
bool CSceneGame::m_bDrawPause = true;		// ポーズの描画状況

//************************************************************
//	子クラス [CSceneGame] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneGame::CSceneGame(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneGame::~CSceneGame()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneGame::Init(void)
{
	//--------------------------------------------------------
	//	初期生成
	//--------------------------------------------------------
	// タイマーマネージャーの生成
	m_pTimerManager = CTimerManager::Create
	( // 引数
		CTimerManager::TIME_SEC,	// 設定タイム
		TIME_LIMIT,					// 制限時間
		TIME_POS,					// 位置
		TIME_VAL_SIZE,				// 数字の大きさ
		TIME_PART_SIZE,				// 区切りの大きさ
		TIME_VAL_SPACE,				// 数字の空白
		TIME_PART_SPACE				// 区切りの空白
	);
	if (m_pTimerManager == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ロゴの自動描画をONにする
	m_pTimerManager->SetEnableLogoDraw(true);

	// シーンの初期化
	CScene::Init();		// ステージ・プレイヤーの生成

	// ゲームマネージャーの生成
	m_pGameManager = CGameManager::Create();
	if (m_pGameManager == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ポーズの生成
	m_pPause = CPause::Create();
	if (m_pPause == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	初期設定
	//--------------------------------------------------------
	// カメラを設定
	CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_LOOKDOWN);	// カメラを見下ろし状態に設定

	// プレイヤーを出現
	CScene::GetPlayer()->SetSpawn();

	// UIの描画状況を設定
	SetEnableDrawUI(m_bDrawUI);

	// ポーズの描画状況を設定
	SetEnableDrawPause(m_bDrawPause);

	// BGMの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CSceneGame::Uninit(void)
{
	// ゲームマネージャーの破棄
	if (FAILED(CGameManager::Release(m_pGameManager)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// タイマーマネージャーの破棄
	if (FAILED(CTimerManager::Release(m_pTimerManager)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ポーズの破棄
	if (FAILED(CPause::Release(m_pPause)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// シーンの終了
	CScene::Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CSceneGame::Update(void)
{
#if _DEBUG

	if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F2))
	{
		// エディット状況を反転
		//CGameManager::GetEditStage()->SetEnableEdit((!CGameManager::GetEditStage()->IsEdit()) ? true : false);
	}
	if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F3))
	{
		// UIの描画状況を反転
		SetEnableDrawUI((!m_bDrawUI) ? true : false);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F4))
	{
		// ポーズの描画状況を反転
		SetEnableDrawPause((!m_bDrawPause) ? true : false);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F5))
	{
		// カメラの操作状況を反転
		SetEnableControlCamera((!m_bControlCamera) ? true : false);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F6))
	{
		// プレイヤーの出現を設定
		CScene::GetPlayer()->SetSpawn();
	}

#if 0
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F7))
	{
		// リザルトに遷移
		//CScene::GetPlayer()->SetState(CPlayer::STATE_CLEAR);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F8))
	{
		// リザルトに遷移
		//CScene::GetPlayer()->SetState(CPlayer::STATE_OVER);
	}
#endif

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "　[デバッグ操作]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F1]：デバッグ表示のON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F2]：エディットモードのON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F3]：UI描画のON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F4]：ポーズ描画のON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F5]：カメラ操作のON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F6]：プレイヤースポーン\n");

#if 0
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F7]：成功リザルト遷移\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F8]：失敗リザルト遷移\n");
#endif

	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "　[デバッグ情報]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");

#endif

	if (m_pGameManager != NULL)
	{ // 使用中の場合

		// ゲームマネージャーの更新
		m_pGameManager->Update();
	}
	else { assert(false); }	// 非使用中

	if (m_pGameManager->GetState() == CGameManager::STATE_NORMAL)
	{ // ゲームマネージャーが通常状態の場合

		if (m_pTimerManager != NULL)
		{ // 使用中の場合

			// タイマーマネージャーの更新
			m_pTimerManager->Update();
		}
		else { assert(false); }	// 非使用中

		if (m_pPause != NULL)
		{ // 使用中の場合

			// ポーズの更新
			m_pPause->Update();
		}
		else { assert(false); }	// 非使用中

		if (!m_pPause->IsPause())
		{ // ポーズ中ではない場合

			// シーンの更新
			CScene::Update();
		}

#if _DEBUG

		else
		{ // ポーズ中の場合

			if (CManager::GetInstance()->GetCamera()->GetState() == CCamera::STATE_CONTROL)
			{ // カメラが操作状態の場合

				// カメラの更新
				CManager::GetInstance()->GetCamera()->Update();
			}
		}

#endif	// _DEBUG

	}
}

//============================================================
//	描画処理
//============================================================
void CSceneGame::Draw(void)
{

}

//============================================================
//	ゲームマネージャー取得処理
//============================================================
CGameManager *CSceneGame::GetGameManager(void)
{
	// ゲームマネージャーのポインタを返す
	return m_pGameManager;
}

//============================================================
//	タイマーマネージャー取得処理
//============================================================
CTimerManager *CSceneGame::GetTimerManager(void)
{
	// タイマーマネージャーのポインタを返す
	return m_pTimerManager;
}

//============================================================
//	ポーズ取得処理
//============================================================
CPause *CSceneGame::GetPause(void)
{
	// ポーズのポインタを返す
	return m_pPause;
}

//============================================================
//	UIの描画状況の設定処理
//============================================================
void CSceneGame::SetEnableDrawUI(const bool bDraw)
{
	// 引数のUIの描画状況を設定
	m_bDrawUI = bDraw;

	// タイマーの描画状況を設定
	m_pTimerManager->SetEnableDraw(bDraw);
}

//============================================================
//	UIの描画状況取得処理
//============================================================
bool CSceneGame::IsDrawUI(void)
{
	// UIの描画状況を返す
	return m_bDrawUI;
}

//============================================================
//	ポーズの描画状況の設定処理
//============================================================
void CSceneGame::SetEnableDrawPause(const bool bDraw)
{
	// 引数のポーズの描画状況を設定
	m_bDrawPause = bDraw;

	// ポーズの描画状況を設定
	m_pPause->SetEnableDraw(m_pPause->IsPause());
}

//============================================================
//	ポーズの描画状況取得処理
//============================================================
bool CSceneGame::IsDrawPause(void)
{
	// ポーズの描画状況を返す
	return m_bDrawPause;
}

//============================================================
//	カメラの操作状況の設定処理
//============================================================
void CSceneGame::SetEnableControlCamera(const bool bControl)
{
	// 引数のカメラ操作状況を設定
	m_bControlCamera = bControl;

	// カメラの操作状況を設定
	if (bControl)
	{ // 操作する状況の場合

		// 操作状態に変更
		CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_CONTROL);
	}
	else
	{ // 操作しない状況の場合

		// 見下ろし状態に変更
		CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_LOOKDOWN);
	}
}
