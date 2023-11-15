//============================================================
//
//	エントリー画面処理 [sceneEntry.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneEntry.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "entryManager.h"
#include "retentionManager.h"

#include "player.h"
#include "stage.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CEntryManager *CSceneEntry::m_pEntryManager = NULL;	// エントリーマネージャー

//************************************************************
//	子クラス [CSceneEntry] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneEntry::CSceneEntry(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneEntry::~CSceneEntry()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneEntry::Init(void)
{
	//--------------------------------------------------------
	//	エントリーの初期化
	//--------------------------------------------------------
	// エントリーマネージャーの生成
	m_pEntryManager = CEntryManager::Create();
	if (m_pEntryManager == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// シーンの初期化
	CScene::Init();

	//--------------------------------------------------------
	//	初期設定
	//--------------------------------------------------------
	// カメラを設定
	CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_ROTATE);	// 回転状態に設定
	CManager::GetInstance()->GetCamera()->SetDestRotate();					// カメラの初期情報を設定

	// BGMの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GENERAL);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CSceneEntry::Uninit(void)
{
	// エントリーマネージャーの破棄
	if (FAILED(CEntryManager::Release(m_pEntryManager)))
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
void CSceneEntry::Update(void)
{
	if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_F2))
	{
		// プレイヤーを全参加させる
		CManager::GetInstance()->GetRetentionManager()->AllSetEnableEntry(true);
		CManager::GetInstance()->GetRetentionManager()->SetNumPlayer(MAX_PLAYER);

		// シーンの設定
		CManager::GetInstance()->SetScene(CScene::MODE_GAME);	// ゲーム画面
	}

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "　[デバッグ操作]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F1]：デバッグ表示のON/OFF\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[F2]：プレイヤーの全参加・遷移\n");

	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "　[デバッグ情報]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");

	if (m_pEntryManager != NULL)
	{ // 使用中の場合

		// エントリーマネージャーの更新
		m_pEntryManager->Update();
	}
	else { assert(false); }	// 非使用中

	// シーンの更新
	CScene::Update();
}

//============================================================
//	描画処理
//============================================================
void CSceneEntry::Draw(void)
{

}

//============================================================
//	エントリーマネージャー取得処理
//============================================================
CEntryManager *CSceneEntry::GetEntryManager(void)
{
	// エントリーマネージャーのポインタを返す
	return m_pEntryManager;
}
