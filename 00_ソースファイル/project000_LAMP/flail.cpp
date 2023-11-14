//============================================================
//
//	モデルUI処理 [flail.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "flail.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "model.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MODEL_UI_PRIO	(14)	// モデルUI表示の優先順位

//************************************************************
//	子クラス [CFlail] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFlail::CFlail() : CObjectModel(CObject::LABEL_NONE, MODEL_UI_PRIO)
{
	m_move = VEC3_ZERO;
}

//============================================================
//	デストラクタ
//============================================================
CFlail::~CFlail()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CFlail::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	BindModel("data\\MODEL\\OBSTACLE\\obstacle017.x");

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CFlail::Uninit(void)
{
	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CFlail::Update(void)
{
	SetVec3Position(GetVec3Position() + m_move);

	m_move.x += (0.0f - m_move.x) * 0.15f;
	m_move.z += (0.0f - m_move.z) * 0.15f;

	// オブジェクトモデルの更新
	CObjectModel::Update();
}

//============================================================
//	描画処理
//============================================================
void CFlail::Draw(void)
{
	// 変数を宣言
	D3DVIEWPORT9 viewportDef;	// カメラのビューポート保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 現在のビューポートを取得
	pDevice->GetViewport(&viewportDef);

	// カメラの設定
	CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

	// オブジェクトモデルの描画
	CObjectModel::Draw();

	// カメラの設定を元に戻す
	CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

	// ビューポートを元に戻す
	pDevice->SetViewport(&viewportDef);
}

//============================================================
//	生成処理
//============================================================
CFlail *CFlail::Create
( // 引数
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR3& rScale	// 大きさ
)
{
	// ポインタを宣言
	CFlail *pModelUI = NULL;	// モデルUI生成用

	if (pModelUI == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pModelUI = new CFlail;	// モデルUI
	}
	else { assert(false); return NULL; }	// 使用中

	if (pModelUI != NULL)
	{ // 使用されている場合
		
		// モデルUIの初期化
		if (FAILED(pModelUI->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pModelUI;
			pModelUI = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pModelUI->SetVec3Position(rPos);

		// 向きを設定
		pModelUI->SetVec3Rotation(rRot);

		// 拡大率を設定
		pModelUI->SetVec3Scaling(rScale);

		// 確保したアドレスを返す
		return pModelUI;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	移動量の設定処理
//============================================================
void CFlail::SetVec3Move(const D3DXVECTOR3& rMove)
{
	// 引数の位置を設定
	m_move = rMove;
}

//============================================================
//	移動量の取得処理
//============================================================
D3DXVECTOR3 CFlail::GetVec3Move(void)
{
	// 位置を返す
	return m_move;
}