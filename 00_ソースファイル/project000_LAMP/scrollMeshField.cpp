//============================================================
//
//	スクロールメッシュフィールド処理 [scrollMeshField.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "scrollMeshField.h"

//************************************************************
//	子クラス [CScrollMeshField] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScrollMeshField::CScrollMeshField()
{
	// メンバ変数をクリア
	m_fTexU  = 0.0f;	// テクスチャ横座標の開始位置
	m_fTexV  = 0.0f;	// テクスチャ縦座標の開始位置
	m_fMoveU = 0.0f;	// テクスチャ横座標の移動量
	m_fMoveV = 0.0f;	// テクスチャ縦座標の移動量
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CScrollMeshField::CScrollMeshField(const CObject::ELabel label, const int nPriority) : CObjectMeshField(label, nPriority)
{
	// メンバ変数をクリア
	m_fTexU  = 0.0f;	// テクスチャ横座標の開始位置
	m_fTexV  = 0.0f;	// テクスチャ縦座標の開始位置
	m_fMoveU = 0.0f;	// テクスチャ横座標の移動量
	m_fMoveV = 0.0f;	// テクスチャ縦座標の移動量
}

//============================================================
//	デストラクタ
//============================================================
CScrollMeshField::~CScrollMeshField()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CScrollMeshField::Init(void)
{
	// メンバ変数を初期化
	m_fTexU  = 0.0f;	// テクスチャ横座標の開始位置
	m_fTexV  = 0.0f;	// テクスチャ縦座標の開始位置
	m_fMoveU = 0.0f;	// テクスチャ横座標の移動量
	m_fMoveV = 0.0f;	// テクスチャ縦座標の移動量

	if (FAILED(CObjectMeshField::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// スクロールのテクスチャ座標の設定
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);

	// 成功を返す
	return S_OK;
}

//============================================================
//	初期化処理 (オーバーロード)
//============================================================
HRESULT CScrollMeshField::Init(const float fMoveU, const float fMoveV)
{
	// メンバ変数を初期化
	m_fTexU  = 0.0f;	// テクスチャ横座標の開始位置
	m_fTexV  = 0.0f;	// テクスチャ縦座標の開始位置
	m_fMoveU = fMoveU;	// テクスチャ横座標の移動量
	m_fMoveV = fMoveV;	// テクスチャ縦座標の移動量

	if (FAILED(CObjectMeshField::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// スクロールのテクスチャ座標の設定
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CScrollMeshField::Uninit(void)
{
	// オブジェクト2Dの終了
	CObjectMeshField::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CScrollMeshField::Update(void)
{
	// スクロールを加算
	m_fTexU += m_fMoveU;
	m_fTexV += m_fMoveV;

	if (m_fTexU > 1.0f)
	{ // 1.0fより大きくなった場合

		// 開始地点を補正
		m_fTexU -= 1.0f;
	}

	if (m_fTexV > 1.0f)
	{ // 1.0fより大きくなった場合

		// 開始地点を補正
		m_fTexV -= 1.0f;
	}

	// オブジェクト2Dの更新
	CObjectMeshField::Update();

	// スクロールのテクスチャ座標の設定
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	描画処理
//============================================================
void CScrollMeshField::Draw(void)
{
	// オブジェクト2Dの描画
	CObjectMeshField::Draw();
}

//============================================================
//	位置の設定処理
//============================================================
void CScrollMeshField::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 位置の設定
	CObjectMeshField::SetVec3Position(rPos);

	// スクロールのテクスチャ座標の設定
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	向きの設定処理
//============================================================
void CScrollMeshField::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 向きの設定
	CObjectMeshField::SetVec3Rotation(rRot);

	// スクロールのテクスチャ座標の設定
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	大きさの設定処理
//============================================================
void CScrollMeshField::SetVec2Sizing(const D3DXVECTOR2& rSize)
{
	// 大きさの設定
	CObjectMeshField::SetVec2Sizing(rSize);

	// スクロールのテクスチャ座標の設定
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	色の設定処理
//============================================================
void CScrollMeshField::SetColor(const D3DXCOLOR& rCol)
{
	// 色の設定
	CObjectMeshField::SetColor(rCol);

	// スクロールのテクスチャ座標の設定
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	生成処理
//============================================================
CScrollMeshField *CScrollMeshField::Create
(
	const float fMoveU,			// 横座標の移動量
	const float fMoveV,			// 縦座標の移動量
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR2& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const D3DCULL cull,			// カリング状況
	const bool bLight			// ライティング状況
)
{
	// ポインタを宣言
	CScrollMeshField *pScrollMeshField = NULL;	// スクロールメッシュフィールド生成用

	if (pScrollMeshField == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pScrollMeshField = new CScrollMeshField;	// スクロールメッシュフィールド
	}
	else { assert(false); return NULL; }	// 使用中

	if (pScrollMeshField != NULL)
	{ // 確保に成功している場合

		// スクロールメッシュフィールドの初期化
		if (FAILED(pScrollMeshField->Init(fMoveU, fMoveV)))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pScrollMeshField;
			pScrollMeshField = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pScrollMeshField->SetVec3Position(rPos);

		// 向きを設定
		pScrollMeshField->SetVec3Rotation(rRot);

		// 大きさを設定
		pScrollMeshField->SetVec2Sizing(rSize);

		// 色を設定
		pScrollMeshField->SetColor(rCol);

		// カリングを設定
		pScrollMeshField->SetCulling(cull);

		// ライティングを設定
		pScrollMeshField->SetLighting(bLight);

		// 分割数を設定
		if (FAILED(pScrollMeshField->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// メモリ開放
			delete pScrollMeshField;
			pScrollMeshField = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pScrollMeshField;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	分割数の設定処理
//============================================================
HRESULT CScrollMeshField::SetPattern(const POSGRID2& rPart)
{
	// 分割数の設定
	if (FAILED(CObjectMeshField::SetPattern(rPart)))
	{ // 分割数の設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// スクロールのテクスチャ座標の設定
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);

	// 成功を返す
	return S_OK;
}

//============================================================
//	横座標の開始位置設定処理
//============================================================
void CScrollMeshField::SetTexU(const float fTexU)
{
	// 引数の横座標の開始位置を代入
	m_fTexU = fTexU;

	// スクロールのテクスチャ座標の設定
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	縦座標の開始位置設定処理
//============================================================
void CScrollMeshField::SetTexV(const float fTexV)
{
	// 引数の縦座標の開始位置を代入
	m_fTexV = fTexV;

	// スクロールのテクスチャ座標の設定
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	横座標の移動量設定処理
//============================================================
void CScrollMeshField::SetMoveU(const float fMoveU)
{
	// 引数の横座標の移動量を代入
	m_fMoveU = fMoveU;

	// スクロールのテクスチャ座標の設定
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	縦座標の移動量設定処理
//============================================================
void CScrollMeshField::SetMoveV(const float fMoveV)
{
	// 引数の縦座標の移動量を代入
	m_fMoveV = fMoveV;

	// スクロールのテクスチャ座標の設定
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}
