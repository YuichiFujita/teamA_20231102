//============================================================
//
//	プレイヤーエントリー処理 [playerEntry.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "playerEntry.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "retentionManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int SPACE_WIDTH = 320;	// 空白
}

//************************************************************
//	子クラス [CPlayerEntry] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayerEntry::CPlayerEntry(const int nPad) : CPlayer(nPad)
{

}

//============================================================
//	デストラクタ
//============================================================
CPlayerEntry::~CPlayerEntry()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayerEntry::Init(void)
{
	// プレイヤーの初期化
	if (FAILED(CPlayer::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// エントリーの設定
	SetEntry();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayerEntry::Uninit(void)
{
	// プレイヤーの終了
	CPlayer::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayerEntry::Update(void)
{
	// プレイヤーの更新
	CPlayer::Update();
}

//============================================================
//	描画処理
//============================================================
void CPlayerEntry::Draw(void)
{
	// 変数を宣言
	D3DVIEWPORT9 defViewport;		// カメラのビューポート保存用
	D3DVIEWPORT9 currentViewport;	// 現在のビューポート保存用
	int nPadID = GetPadID();		// パッドインデックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	CCamera *pCamera = CManager::GetInstance()->GetCamera();	// カメラのポインタ

	// カメラのビューポートの位置を設定
	currentViewport = pCamera->GetViewport(CCamera::TYPE_ENTRY);	// ビューポート取得
	currentViewport.X = (nPadID * SPACE_WIDTH);						// 左上位置を設定
	pCamera->SetViewport(CCamera::TYPE_ENTRY, currentViewport);		// ビューポート設定

	if (CManager::GetInstance()->GetRetentionManager()->IsEntry(nPadID))
	{ // 現在のプレイヤーがエントリーしている場合

		// 現在のビューポートを取得
		pDevice->GetViewport(&defViewport);

		// カメラの設定
		CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_ENTRY);

		// オブジェクトキャラクターの描画
		CObjectChara::Draw();

		// カメラの設定を元に戻す
		CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

		// ビューポートを元に戻す
		pDevice->SetViewport(&defViewport);
	}
}

//============================================================
//	エントリーの設定処理
//============================================================
void CPlayerEntry::SetEntry(void)
{
	// 変数を宣言
	D3DXVECTOR3 set = VEC3_ZERO;	// 引数設定用

	// 情報を初期化
	SetState(STATE_NONE);	// 何もしない状態の設定
	SetMotion(MOTION_IDOL);	// 待機モーションを設定

	// 位置を設定
	SetVec3Position(set);

	// 向きを設定
	SetVec3Rotation(set);

	// マテリアルを再設定
	ResetMaterial();

	// 透明度を不透明に設定
	SetAlpha(1.0f);
}
