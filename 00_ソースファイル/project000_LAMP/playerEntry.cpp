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
	D3DVIEWPORT9 viewportDef;	// カメラのビューポート保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 現在のビューポートを取得
	pDevice->GetViewport(&viewportDef);

	// カメラの設定
	CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_ENTRY);

	// オブジェクトキャラクターの描画
	CObjectChara::Draw();

	// カメラの設定を元に戻す
	CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

	// ビューポートを元に戻す
	pDevice->SetViewport(&viewportDef);
}

//============================================================
//	エントリーの設定処理
//============================================================
void CPlayerEntry::SetEntry(void)
{
#if 0

	// 変数を宣言
	D3DXVECTOR3 set = VEC3_ZERO;	// 引数設定用

	// 情報を初期化
	SetState(STATE_NONE);	// 何もしない状態の設定
	SetMotion(MOTION_IDOL);	// 待機モーションを設定

	// カウンターを初期化
	m_nCounterState = 0;	// 状態管理カウンター

	// 位置を設定
	SetVec3Position(set);

	// 向きを設定
	SetVec3Rotation(set);
	m_destRot = set;

	// 移動量を初期化
	m_move = VEC3_ZERO;

	// マテリアルを再設定
	ResetMaterial();

	// 透明度を透明に再設定
	SetAlpha(0.0f);

	// 自動描画をOFFにする
	SetEnableDraw(false);

#endif
}
