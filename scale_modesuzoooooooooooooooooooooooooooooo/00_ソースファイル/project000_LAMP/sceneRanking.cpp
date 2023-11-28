//============================================================
//
//	ランキング画面処理 [sceneRanking.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneRanking.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "rankingManager.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CRankingManager *CSceneRanking::m_pRankingManager = NULL;	// ランキングマネージャー

//************************************************************
//	子クラス [CSceneRanking] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneRanking::CSceneRanking(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneRanking::~CSceneRanking()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneRanking::Init(void)
{
	//--------------------------------------------------------
	//	ランキングの初期化
	//--------------------------------------------------------
	// ランキングマネージャーの生成
	m_pRankingManager = CRankingManager::Create();
	if (m_pRankingManager == NULL)
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
HRESULT CSceneRanking::Uninit(void)
{
	// ランキングマネージャーの破棄
	if (FAILED(CRankingManager::Release(m_pRankingManager)))
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
void CSceneRanking::Update(void)
{
	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "　[デバッグ情報]\n");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "======================================\n");

	if (m_pRankingManager != NULL)
	{ // 使用中の場合

		// ランキングマネージャーの更新
		m_pRankingManager->Update();
	}
	else { assert(false); }	// 非使用中

	// シーンの更新
	CScene::Update();
}

//============================================================
//	描画処理
//============================================================
void CSceneRanking::Draw(void)
{

}

//============================================================
//	ランキングマネージャー取得処理
//============================================================
CRankingManager *CSceneRanking::GetRankingManager(void)
{
	// ランキングマネージャーを返す
	return m_pRankingManager;
}
