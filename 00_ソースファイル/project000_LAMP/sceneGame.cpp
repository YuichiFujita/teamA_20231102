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
#include "mask.h"
#include "player.h"
#include "block.h"
#include "obstacle.h"
#include "retentionManager.h"
#include "gameManager.h"
#include "editStageManager.h"
#include "ready.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	D3DXCOLOR COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);	// マスク色
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CGameManager	*CSceneGame::m_pGameManager	= NULL;	// ゲームマネージャー
CPause			*CSceneGame::m_pPause		= NULL;	// ポーズ

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
	//	セットアップ
	//--------------------------------------------------------
	// ブロックのセットアップ
	CBlock::LoadSetup();

	// 障害物のセットアップ
	CObstacle::LoadSetup();

	//--------------------------------------------------------
	//	初期生成
	//--------------------------------------------------------
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

	// マスクの生成 (プレイヤーの壁裏表示用ポリゴン)
	CMask::Create(SCREEN_CENT, SCREEN_SIZE, COL);

	//--------------------------------------------------------
	//	初期設定
	//--------------------------------------------------------
	// カメラを設定
	CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_LOOKDOWN);	// カメラを見下ろし状態に設定

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // プレイヤー数分繰り返す

		CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);	// プレイヤー情報

		if (pPlayer != NULL)
		{ // プレイヤーが存在する場合

			// プレイヤーを出現
			pPlayer->SetSpawn();
			pPlayer->SetState(CPlayer::STATE_NONE);
		}
	}

	// 見下ろしカメラの目標位置の設定
	CManager::GetInstance()->GetCamera()->SetDestLookDown();

	// UIの描画状況を設定
	SetEnableDrawUI(m_bDrawUI);

	// ポーズの描画状況を設定
	SetEnableDrawPause(m_bDrawPause);

	// BGMの再生
	int nRand = rand() % 3;

	switch (nRand)
	{
	case 0:
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);
		break;
	case 1:
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME_1);
		break;
	case 2:
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME_2);
		break;
	default:
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);
		break;
	}
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
		// エディット状況の切替
		CGameManager::GetEditStage()->SwitchEnableEdit();
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
		// ゲーム画面に遷移
		CManager::GetInstance()->SetMode(CScene::MODE_GAME);
	}

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "　[デバッグ操作]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F1]：デバッグ表示のON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F2]：エディットモードのON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F3]：UI描画のON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F4]：ポーズ描画のON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F5]：カメラ操作のON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F6]：ゲームのリトライ\n");

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

	if (m_pGameManager->GetState() != CGameManager::STATE_RESULT)
	{ // ゲームマネージャーがリザルト状態以外の場合

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

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // プレイヤー数分繰り返す

		CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);	// プレイヤー情報

		if (pPlayer != NULL)
		{ // プレイヤーが存在する場合

			// プレイヤーの描画状況を設定
			pPlayer->SetEnableDrawUI(bDraw);
		}
	}
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
