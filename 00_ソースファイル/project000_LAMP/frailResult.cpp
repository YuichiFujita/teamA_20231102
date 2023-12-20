//============================================================
//
//	フレイルエントリー処理 [flailEntry.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "frailResult.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "retentionManager.h"

//************************************************************
//	子クラス [CFlailEntry] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFlailResult::CFlailResult()
{

}

//============================================================
//	デストラクタ
//============================================================
CFlailResult::~CFlailResult()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CFlailResult::Init(void)
{
	// フレイルの初期化
	if (FAILED(CFlail::Init()))
	{ // 初期化に失敗した場合

	  // 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 影の表示をOFFにする
	SetEnableDepthShadow(false);
	SetEnableZTex(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CFlailResult::Uninit(void)
{
	// フレイルの終了
	CFlail::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CFlailResult::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CFlailResult::Draw(void)
{
	// 変数を宣言
	D3DVIEWPORT9 defViewport;		// カメラのビューポート保存用
	D3DVIEWPORT9 currentViewport;	// 現在のビューポート保存用
	int nPlayerID = CManager::GetInstance()->GetRetentionManager()->GetWinRank1st();	// プレイヤーインデックス

									// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	CCamera *pCamera = CManager::GetInstance()->GetCamera();	// カメラのポインタ

																// カメラのビューポートの位置を設定
	currentViewport = pCamera->GetViewport(CCamera::TYPE_RESULT);	// ビューポート取得
	currentViewport.X = (nPlayerID);								// 左上位置を設定
	pCamera->SetViewport(CCamera::TYPE_RESULT, currentViewport);		// ビューポート設定

	if (CManager::GetInstance()->GetRetentionManager()->IsEntry(nPlayerID))
	{ // 現在のフレイルがエントリーしている場合

	  // 現在のビューポートを取得
		pDevice->GetViewport(&defViewport);

		// カメラの設定
		CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_RESULT);

		// 親の棒モデルのマトリックスを取得
		D3DXMATRIX mtxStick = CScene::GetPlayer(nPlayerID)->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld();

		// フレイル位置を設定
		SetVec3Position(D3DXVECTOR3(mtxStick._41, mtxStick._42, mtxStick._43));

		// オブジェクトキャラクターの描画
		CObjectModel::Draw();

		// カメラの設定を元に戻す
		CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

		// ビューポートを元に戻す
		pDevice->SetViewport(&defViewport);
	}
}
