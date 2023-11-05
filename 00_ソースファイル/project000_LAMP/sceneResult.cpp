//============================================================
//
//	リザルト画面処理 [sceneResult.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneResult.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "resultManager.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CResultManager *CSceneResult::m_pResultManager = NULL;	// リザルトマネージャー

//************************************************************
//	子クラス [CSceneResult] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneResult::CSceneResult(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneResult::~CSceneResult()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneResult::Init(void)
{
	//--------------------------------------------------------
	//	リザルトの初期化
	//--------------------------------------------------------
	// リザルトマネージャーの生成
	m_pResultManager = CResultManager::Create();
	if (m_pResultManager == NULL)
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
	CManager::GetInstance()->GetCamera()->SetDestRotate();	// 目標位置を設定

	// BGMの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GENERAL);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CSceneResult::Uninit(void)
{
	// リザルトマネージャーの破棄
	if (FAILED(CResultManager::Release(m_pResultManager)))
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
void CSceneResult::Update(void)
{
	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "　[デバッグ情報]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");

	if (m_pResultManager != NULL)
	{ // 使用中の場合

		// リザルトマネージャーの更新
		m_pResultManager->Update();
	}
	else { assert(false); }	// 非使用中

	// シーンの更新
	CScene::Update();
}

//============================================================
//	描画処理
//============================================================
void CSceneResult::Draw(void)
{

}

//============================================================
//	リザルトマネージャー取得処理
//============================================================
CResultManager *CSceneResult::GetResultManager(void)
{
	// リザルトマネージャーを返す
	return m_pResultManager;
}
