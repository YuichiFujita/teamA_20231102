//============================================================
//
//	ステータスマネージャー処理 [statusManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "statusManager.h"
#include "manager.h"
#include "objectGauge2D.h"
#include "valueUI.h"
#include "multiValue.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 14;	// ステータスの優先順位

	namespace life
	{
		const int MAX_LIFE		= 100;	// 最大体力
		const int CHANGE_FRAME	= 10;	// ゲージ変動フレーム

		const D3DXVECTOR3	POS = D3DXVECTOR3(400.0f, 200.0f, 0.0f);		// ゲージ位置
		const D3DXVECTOR3	SIZE_GAUGE = D3DXVECTOR3(300.0f, 20.0f, 0.0f);	// ゲージ大きさ
		const D3DXCOLOR		COL_GAUGE_FRONT	= XCOL_YELLOW;	// ゲージ前景色
		const D3DXCOLOR		COL_GAUGE_BACK	= XCOL_RED;		// ゲージ背景色
	}

	namespace knockrate
	{
		const char* TITLE_TEX_PASS = "data\\TEXTURE\\continue001.png";	// タイトルテクスチャ

		const int MAX_DIG = 3;	// 数字桁数

		const D3DXVECTOR3 POS			= D3DXVECTOR3(200.0f, 400.0f, 0.0f);	// UI位置
		const D3DXVECTOR3 SPACE_TITLE	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// タイトル空白
		const D3DXVECTOR3 SPACE_VALUE	= D3DXVECTOR3(100.0f, 0.0f, 0.0f);		// 数字空白
		const D3DXVECTOR3 SIZE_TITLE	= D3DXVECTOR3(400.0f, 100.0f, 0.0f);	// タイトル大きさ
		const D3DXVECTOR3 SIZE_VALUE	= D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// 数字大きさ
	}
}

//************************************************************
//	親クラス [CStatusManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CStatusManager::CStatusManager()
{
	// メンバ変数をクリア
	m_pLife = NULL;			// 体力の情報
	m_pKnockRate = NULL;	// 吹っ飛び率の情報
}

//============================================================
//	デストラクタ
//============================================================
CStatusManager::~CStatusManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CStatusManager::Init(void)
{
	// メンバ変数を初期化
	m_pLife = NULL;			// 体力の情報
	m_pKnockRate = NULL;	// 吹っ飛び率の情報

	// 体力の情報の生成
	m_pLife = CObjectGauge2D::Create
	( // 引数
		CObject::LABEL_UI,		// オブジェクトラベル
		life::MAX_LIFE,			// 最大表示値
		life::CHANGE_FRAME,		// 表示値変動フレーム
		life::POS,				// 位置
		life::SIZE_GAUGE,		// ゲージ大きさ
		life::COL_GAUGE_FRONT,	// 表ゲージ色
		life::COL_GAUGE_BACK	// 裏ゲージ色
	);
	if (m_pLife == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 優先順位を設定
	m_pLife->SetPriority(PRIORITY);

	// 吹っ飛び率の情報の設定
	m_pKnockRate = CValueUI::Create
	( // 引数
		knockrate::TITLE_TEX_PASS,	// タイトルテクスチャパス
		CValue::TEXTURE_NORMAL,		// 数字テクスチャ
		knockrate::MAX_DIG,			// 桁数
		knockrate::POS,				// 位置
		knockrate::SPACE_TITLE,		// 行間
		knockrate::SPACE_VALUE,		// 数字行間
		knockrate::SIZE_TITLE,		// タイトル大きさ
		knockrate::SIZE_VALUE		// 数字大きさ
	);
	if (m_pKnockRate == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 優先順位を設定
	m_pKnockRate->SetPriority(PRIORITY);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CStatusManager::Uninit(void)
{
	// 体力の終了
	m_pLife->Uninit();

	// 吹っ飛び率の終了
	m_pKnockRate->Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CStatusManager::Update(void)
{
	// 体力の更新
	m_pLife->Update();

	// 吹っ飛び率の更新
	m_pKnockRate->Update();
}

//============================================================
//	描画処理
//============================================================
void CStatusManager::Draw(void)
{

}

//============================================================
//	体力の加算処理
//============================================================
void CStatusManager::AddNumLife(const int nAdd)
{
	// 引数の体力を加算
	m_pLife->AddNum(nAdd);
}

//============================================================
//	体力の設定処理
//============================================================
void CStatusManager::SetNumLife(const int nSet)
{
	// 引数の体力を設定
	m_pLife->SetNum(nSet);
}

//============================================================
//	体力取得処理
//============================================================
int CStatusManager::GetNumLife(void) const
{
	// 体力を返す
	return m_pLife->GetNum();
}

//============================================================
//	体力の描画状況の設定処理
//============================================================
void CStatusManager::SetEnableDrawLife(const bool bDraw)
{
	// 引数の描画状況を設定
	m_pLife->SetEnableDraw(bDraw);
}

//============================================================
//	吹っ飛び率の加算処理
//============================================================
void CStatusManager::AddNumRate(const int nAdd)
{
	// 引数の吹っ飛び率を加算
	m_pKnockRate->GetMultiValue()->AddNum(nAdd);
}

//============================================================
//	吹っ飛び率の設定処理
//============================================================
void CStatusManager::SetNumRate(const int nSet)
{
	// 引数の吹っ飛び率を設定
	m_pKnockRate->GetMultiValue()->SetNum(nSet);
}

//============================================================
//	吹っ飛び率取得処理
//============================================================
int CStatusManager::GetNumRate(void) const
{
	// 吹っ飛び率を返す
	return m_pKnockRate->GetMultiValue()->GetNum();
}

//============================================================
//	吹っ飛び率の描画状況の設定処理
//============================================================
void CStatusManager::SetEnableDrawRate(const bool bDraw)
{
	// 引数の描画状況を設定
	m_pKnockRate->SetEnableDraw(bDraw);
}

//============================================================
//	生成処理
//============================================================
CStatusManager *CStatusManager::Create(void)
{
	// ポインタを宣言
	CStatusManager *pStatusManager = NULL;		// ステータスマネージャー生成用

	if (pStatusManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pStatusManager = new CStatusManager;	// ステータスマネージャー
	}
	else { assert(false); return NULL; }		// 使用中

	if (pStatusManager != NULL)
	{ // 使用されている場合
		
		// ステータスマネージャーの初期化
		if (FAILED(pStatusManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pStatusManager;
			pStatusManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pStatusManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CStatusManager::Release(CStatusManager *&prStatusManager)
{
	if (prStatusManager != NULL)
	{ // 使用中の場合

		// ステータスマネージャーの終了
		if (FAILED(prStatusManager->Uninit()))
		{ // 破棄に失敗した場合

			// メモリ開放
			delete prStatusManager;
			prStatusManager = NULL;

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prStatusManager;
		prStatusManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
