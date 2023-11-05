//============================================================
//
//	タイトル画面処理 [sceneTitle.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneTitle.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "titleManager.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CTitleManager *CSceneTitle::m_pTitleManager = NULL;	// タイトルマネージャー

//************************************************************
//	子クラス [CSceneTitle] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneTitle::CSceneTitle(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneTitle::~CSceneTitle()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneTitle::Init(void)
{
	//--------------------------------------------------------
	//	タイトルの初期化
	//--------------------------------------------------------
	// タイトルマネージャーの生成
	m_pTitleManager = CTitleManager::Create();
	if (m_pTitleManager == NULL)
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
	CManager::GetInstance()->GetCamera()->SetState(CCamera::STATE_ROTATE);	// カメラを回転状態に設定
	CManager::GetInstance()->GetCamera()->SetDestRotate();					// 目標位置を設定
	CManager::GetInstance()->GetCamera()->SetEnableUpdate(false);			// 更新を停止

	// BGMの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GENERAL);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CSceneTitle::Uninit(void)
{
	// タイトルマネージャーの破棄
	if (FAILED(CTitleManager::Release(m_pTitleManager)))
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
void CSceneTitle::Update(void)
{
	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "　[デバッグ情報]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");

	if (m_pTitleManager != NULL)
	{ // 使用中の場合

		// タイトルマネージャーの更新
		m_pTitleManager->Update();
	}
	else { assert(false); }	// 非使用中

	// シーンの更新
	CScene::Update();
}

//============================================================
//	描画処理
//============================================================
void CSceneTitle::Draw(void)
{

}

//============================================================
//	タイトルマネージャー取得処理
//============================================================
CTitleManager *CSceneTitle::GetTitleManager(void)
{
	// タイトルマネージャーを返す
	return m_pTitleManager;
}
