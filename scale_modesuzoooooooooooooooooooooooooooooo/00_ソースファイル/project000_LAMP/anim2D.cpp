//============================================================
//
//	アニメーション2D処理 [anim2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "anim2D.h"

//************************************************************
//	子クラス [CAnim2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CAnim2D::CAnim2D()
{
	// メンバ変数をクリア
	m_nCounter		= 0;	// アニメーションカウンター
	m_nCntChange	= 0;	// パターン変更カウント
	m_nPattern		= 0;	// アニメーションパターン
	m_nMaxPtrn		= 0;	// パターンの総数
	m_nWidthPtrn	= 0;	// テクスチャの横の分割数
	m_nHeightPtrn	= 0;	// テクスチャの縦の分割数
	m_nNumLoop		= 0;	// パターン繰り返し数
	m_bStop		= false;	// 停止状況
	m_bPlayBack	= false;	// 逆再生状況
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CAnim2D::CAnim2D(const CObject::ELabel label, const int nPriority) : CObject2D(label, nPriority)
{
	// メンバ変数をクリア
	m_nCounter		= 0;	// アニメーションカウンター
	m_nCntChange	= 0;	// パターン変更カウント
	m_nPattern		= 0;	// アニメーションパターン
	m_nMaxPtrn		= 0;	// パターンの総数
	m_nWidthPtrn	= 0;	// テクスチャの横の分割数
	m_nHeightPtrn	= 0;	// テクスチャの縦の分割数
	m_nNumLoop		= 0;	// パターン繰り返し数
	m_bStop		= false;	// 停止状況
	m_bPlayBack	= false;	// 逆再生状況
}

//============================================================
//	デストラクタ
//============================================================
CAnim2D::~CAnim2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CAnim2D::Init(void)
{
	// メンバ変数を初期化
	m_nCounter		= 0;	// アニメーションカウンター
	m_nCntChange	= 0;	// パターン変更カウント
	m_nPattern		= 0;	// アニメーションパターン
	m_nMaxPtrn		= 0;	// パターンの総数
	m_nWidthPtrn	= 1;	// テクスチャの横の分割数
	m_nHeightPtrn	= 1;	// テクスチャの縦の分割数
	m_nNumLoop		= 0;	// パターン繰り返し数
	m_bStop		= false;	// 停止状況
	m_bPlayBack	= false;	// 逆再生状況

	// オブジェクト2Dの初期化
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// アニメーションのテクスチャ座標の設定
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CAnim2D::Uninit(void)
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CAnim2D::Update(void)
{
	if (m_bStop == false)
	{ // 再生状態の場合

		if (m_nCntChange > 0)
		{ // 変更カウントが 0より大きい場合

			// カウンターを加算
			m_nCounter++;

			if (m_nCounter % m_nCntChange == 0)
			{ // カウンターが変更カウントになった場合

				// カウンターを初期化
				m_nCounter = 0;

				if (m_bPlayBack == false)
				{ // 通常再生の場合

					// パターンを加算
					m_nPattern = (m_nPattern + 1) % m_nMaxPtrn;
				}
				else
				{ // 逆再生の場合

					// パターンを減算
					m_nPattern = (m_nPattern + (m_nMaxPtrn - 1)) % m_nMaxPtrn;
				}

				if (m_nPattern == 0)
				{ // パターン数が一枚目の場合

					// 繰り返し数を加算
					m_nNumLoop++;
				}
			}
		}
	}

	// オブジェクト2Dの更新
	CObject2D::Update();

	// アニメーションのテクスチャ座標の設定
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	描画処理
//============================================================
void CAnim2D::Draw(void)
{
	// オブジェクト2Dの描画
	CObject2D::Draw();
}

//============================================================
//	生成処理
//============================================================
CAnim2D *CAnim2D::Create(const int nWidthPtrn, const int nHeightPtrn, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXVECTOR3& rRot, const D3DXCOLOR& rCol)
{
	// ポインタを宣言
	CAnim2D *pAnim2D = NULL;	// アニメーション2D生成用

	if (pAnim2D == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pAnim2D = new CAnim2D;	// アニメーション2D
	}
	else { assert(false); return NULL; }	// 使用中

	if (pAnim2D != NULL)
	{ // 確保に成功している場合

		// アニメーション2Dの初期化
		if (FAILED(pAnim2D->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pAnim2D;
			pAnim2D = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャ横分割数を設定
		pAnim2D->SetWidthPattern(nWidthPtrn);

		// テクスチャ縦分割数を設定
		pAnim2D->SetHeightPattern(nHeightPtrn);

		// 位置を設定
		pAnim2D->SetVec3Position(rPos);

		// 向きを設定
		pAnim2D->SetVec3Rotation(rRot);

		// 大きさを設定
		pAnim2D->SetVec3Sizing(rSize);

		// 色を設定
		pAnim2D->SetColor(rCol);

		// 確保したアドレスを返す
		return pAnim2D;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	位置の設定処理
//============================================================
void CAnim2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 位置の設定
	CObject2D::SetVec3Position(rPos);

	// アニメーションのテクスチャ座標の設定
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	向きの設定処理
//============================================================
void CAnim2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 向きの設定
	CObject2D::SetVec3Rotation(rRot);

	// アニメーションのテクスチャ座標の設定
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	大きさの設定処理
//============================================================
void CAnim2D::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// 大きさの設定
	CObject2D::SetVec3Sizing(rSize);

	// アニメーションのテクスチャ座標の設定
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	色の設定処理
//============================================================
void CAnim2D::SetColor(const D3DXCOLOR& rCol)
{
	// 色の設定
	CObject2D::SetColor(rCol);

	// アニメーションのテクスチャ座標の設定
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	パターンの設定処理
//============================================================
void CAnim2D::SetPattern(const int nPattern)
{
	// 引数のパターン数を代入
	m_nPattern = nPattern;

	// アニメーションのテクスチャ座標の設定
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	パターンの総数の設定処理
//============================================================
void CAnim2D::SetMaxPattern(const int nMaxPtrn)
{
	// 引数のパターンの総数を代入
	m_nMaxPtrn = nMaxPtrn;

	// アニメーションのテクスチャ座標の設定
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	テクスチャの横分割数の設定処理
//============================================================
void CAnim2D::SetWidthPattern(const int nWidthPtrn)
{
	// 引数のテクスチャ横分割数を設定
	m_nWidthPtrn = nWidthPtrn;

	// パターンの総数を設定
	m_nMaxPtrn = m_nWidthPtrn * m_nHeightPtrn;

	// アニメーションのテクスチャ座標の設定
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	テクスチャの縦分割数の設定処理
//============================================================
void CAnim2D::SetHeightPattern(const int nHeightPtrn)
{
	// 引数のテクスチャ縦分割数を設定
	m_nHeightPtrn = nHeightPtrn;

	// パターンの総数を設定
	m_nMaxPtrn = m_nWidthPtrn * m_nHeightPtrn;

	// アニメーションのテクスチャ座標の設定
	CObject2D::SetAnimTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	カウンターの設定処理
//============================================================
void CAnim2D::SetCounter(const int nCntChange)
{
	// 引数のパターン変更カウントを代入
	m_nCntChange = nCntChange;
}

//============================================================
//	停止状況の設定処理
//============================================================
void CAnim2D::SetEnableStop(const bool bStop)
{
	// 引数の停止状況を代入
	m_bStop = bStop;
}

//============================================================
//	逆再生状況の設定処理
//============================================================
void CAnim2D::SetEnablePlayBack(const bool bPlayBack)
{
	// 引数の逆再生状況を代入
	m_bPlayBack = bPlayBack;
}

//============================================================
//	パターン繰り返し数の取得処理
//============================================================
int CAnim2D::GetLoopAnimation(void) const
{
	// パターン繰り返し数を返す
	return m_nNumLoop;
}
