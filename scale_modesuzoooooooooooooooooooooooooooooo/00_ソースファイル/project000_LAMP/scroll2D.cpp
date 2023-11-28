//============================================================
//
//	スクロール2D処理 [scroll2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "scroll2D.h"

//************************************************************
//	子クラス [CScroll2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScroll2D::CScroll2D()
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
CScroll2D::CScroll2D(const CObject::ELabel label, const int nPriority) : CObject2D(label, nPriority)
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
CScroll2D::~CScroll2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CScroll2D::Init(void)
{
	// メンバ変数を初期化
	m_fTexU  = 0.0f;	// テクスチャ横座標の開始位置
	m_fTexV  = 0.0f;	// テクスチャ縦座標の開始位置
	m_fMoveU = 0.0f;	// テクスチャ横座標の移動量
	m_fMoveV = 0.0f;	// テクスチャ縦座標の移動量

	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// スクロールのテクスチャ座標の設定
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);

	// 成功を返す
	return S_OK;
}

//============================================================
//	初期化処理 (オーバーロード)
//============================================================
HRESULT CScroll2D::Init(const float fMoveU, const float fMoveV)
{
	// メンバ変数を初期化
	m_fTexU  = 0.0f;	// テクスチャ横座標の開始位置
	m_fTexV  = 0.0f;	// テクスチャ縦座標の開始位置
	m_fMoveU = fMoveU;	// テクスチャ横座標の移動量
	m_fMoveV = fMoveV;	// テクスチャ縦座標の移動量

	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// スクロールのテクスチャ座標の設定
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CScroll2D::Uninit(void)
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CScroll2D::Update(void)
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
	CObject2D::Update();

	// スクロールのテクスチャ座標の設定
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	描画処理
//============================================================
void CScroll2D::Draw(void)
{
	// オブジェクト2Dの描画
	CObject2D::Draw();
}

//============================================================
//	位置の設定処理
//============================================================
void CScroll2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 位置の設定
	CObject2D::SetVec3Position(rPos);

	// スクロールのテクスチャ座標の設定
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	向きの設定処理
//============================================================
void CScroll2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 向きの設定
	CObject2D::SetVec3Rotation(rRot);

	// スクロールのテクスチャ座標の設定
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	大きさの設定処理
//============================================================
void CScroll2D::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// 大きさの設定
	CObject2D::SetVec3Sizing(rSize);

	// スクロールのテクスチャ座標の設定
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	色の設定処理
//============================================================
void CScroll2D::SetColor(const D3DXCOLOR& rCol)
{
	// 色の設定
	CObject2D::SetColor(rCol);

	// スクロールのテクスチャ座標の設定
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	生成処理
//============================================================
CScroll2D *CScroll2D::Create(const float fMoveU, const float fMoveV, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXVECTOR3& rRot, const D3DXCOLOR& rCol)
{
	// ポインタを宣言
	CScroll2D *pScroll2D = NULL;	// スクロール2D生成用

	if (pScroll2D == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pScroll2D = new CScroll2D;	// スクロール2D
	}
	else { assert(false); return NULL; }	// 使用中

	if (pScroll2D != NULL)
	{ // 確保に成功している場合

		// スクロール2Dの初期化
		if (FAILED(pScroll2D->Init(fMoveU, fMoveV)))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pScroll2D;
			pScroll2D = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pScroll2D->SetVec3Position(rPos);

		// 向きを設定
		pScroll2D->SetVec3Rotation(rRot);

		// 大きさを設定
		pScroll2D->SetVec3Sizing(rSize);

		// 色を設定
		pScroll2D->SetColor(rCol);

		// 確保したアドレスを返す
		return pScroll2D;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	横座標の開始位置設定処理
//============================================================
void CScroll2D::SetTexU(const float fTexU)
{
	// 引数の横座標の開始位置を代入
	m_fTexU = fTexU;

	// スクロールのテクスチャ座標の設定
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	縦座標の開始位置設定処理
//============================================================
void CScroll2D::SetTexV(const float fTexV)
{
	// 引数の縦座標の開始位置を代入
	m_fTexV = fTexV;

	// スクロールのテクスチャ座標の設定
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	横座標の移動量設定処理
//============================================================
void CScroll2D::SetMoveU(const float fMoveU)
{
	// 引数の横座標の移動量を代入
	m_fMoveU = fMoveU;

	// スクロールのテクスチャ座標の設定
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	縦座標の移動量設定処理
//============================================================
void CScroll2D::SetMoveV(const float fMoveV)
{
	// 引数の縦座標の移動量を代入
	m_fMoveV = fMoveV;

	// スクロールのテクスチャ座標の設定
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}
