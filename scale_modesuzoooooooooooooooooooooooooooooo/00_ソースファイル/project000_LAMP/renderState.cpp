//============================================================
//
//	レンダーステート処理 [renderState.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "renderState.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CRenderState::SInfo CRenderState::m_save = {};	// 保存レンダーステート情報

//************************************************************
//	親クラス [CRenderState] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CRenderState::CRenderState()
{
	// メンバ変数をクリア
	memset(&m_info, 0, sizeof(m_info));	// レンダーステート情報
}

//============================================================
//	デストラクタ
//============================================================
CRenderState::~CRenderState()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CRenderState::Init(void)
{
	// メンバ変数を初期化
	m_info = m_save;	// レンダーステート情報

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CRenderState::Uninit(void)
{

}

//============================================================
//	設定処理
//============================================================
void CRenderState::Set(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 設定中のレンダーステートを設定
	SetRenderState(pDevice, m_info);
}

//============================================================
//	再設定処理
//============================================================
void CRenderState::Reset(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 保存済みのレンダーステートに戻す
	SetRenderState(pDevice, m_save);
}

//============================================================
//	生成処理
//============================================================
CRenderState *CRenderState::Create(void)
{
	// ポインタを宣言
	CRenderState *pRenderState = NULL;	// レンダーステート生成用

	if (pRenderState == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pRenderState = new CRenderState;	// レンダーステート
	}
	else { assert(false); return NULL; }	// 使用中

	if (pRenderState != NULL)
	{ // 確保に成功している場合

		// レンダーステートの初期化
		if (FAILED(pRenderState->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pRenderState;
			pRenderState = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pRenderState;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	レンダーステート情報の初期化処理
//============================================================
void CRenderState::InitRenderState(const LPDIRECT3DDEVICE9 &rDevice)
{
	// αブレンド情報を初期化
	m_save.blendAlpha.op		= D3DBLENDOP_ADD;
	m_save.blendAlpha.scr		= D3DBLEND_SRCALPHA;
	m_save.blendAlpha.dest		= D3DBLEND_INVSRCALPHA;
	m_save.blendAlpha.bBlend	= true;

	// αテスト情報を初期化
	m_save.testAlpha.func	= D3DCMP_ALWAYS;
	m_save.testAlpha.nRef	= 0;
	m_save.testAlpha.bTest	= false;

	// Zテスト情報を初期化
	m_save.testZ.func		= D3DCMP_LESSEQUAL;
	m_save.testZ.bUpdate	= true;

	// カリング情報を初期化
	m_save.cull = D3DCULL_CCW;

	// ライティング状況を初期化
	m_save.bLight = true;

	// レンダーステートの初期化
	SetRenderState(rDevice, m_save);
}

//============================================================
//	レンダーステートの設定処理
//============================================================
void CRenderState::SetRenderState(const LPDIRECT3DDEVICE9 &rDevice, const SInfo info)
{
	// αブレンディングを設定
	rDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, info.blendAlpha.bBlend);
	rDevice->SetRenderState(D3DRS_BLENDOP, info.blendAlpha.op);
	rDevice->SetRenderState(D3DRS_SRCBLEND, info.blendAlpha.scr);
	rDevice->SetRenderState(D3DRS_DESTBLEND, info.blendAlpha.dest);

	// αテストを設定
	rDevice->SetRenderState(D3DRS_ALPHAFUNC, info.testAlpha.func);			// αテストの設定
	rDevice->SetRenderState(D3DRS_ALPHAREF, info.testAlpha.nRef);			// αテストの参照値設定
	rDevice->SetRenderState(D3DRS_ALPHATESTENABLE, info.testAlpha.bTest);	// αテストの有効 / 無効の設定

	// Zテストを設定
	rDevice->SetRenderState(D3DRS_ZFUNC, info.testZ.func);				// Zテストの設定
	rDevice->SetRenderState(D3DRS_ZWRITEENABLE, info.testZ.bUpdate);	// Zバッファ更新の有効 / 無効の設定

	// ポリゴンの表示状態を設定
	rDevice->SetRenderState(D3DRS_CULLMODE, info.cull);

	// ライティングを設定
	rDevice->SetRenderState(D3DRS_LIGHTING, info.bLight);
}
