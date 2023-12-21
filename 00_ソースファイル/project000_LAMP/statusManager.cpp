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
#include "multiValue.h"
#include "retentionManager.h"
#include "objectBillboard.h"
#include "object2D.h"
#include "player.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 14;	// ステータスの優先順位

	namespace life
	{
		const int MAX_LIFE	= 100;	// 数字
		const int MAX_DIG	= 3;	// 桁数
		const D3DXVECTOR3 POS	= D3DXVECTOR3(200.0f, 75.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(D3DXVECTOR3(64.0f, 144.0f, 0.0f) * 0.75f);	// 大きさ
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(40.0f, 0.0f, 0.0f);		// 行間
	}

	namespace knockrate
	{
		const int MAX_LIFE	= 100;	// 数字
		const int MAX_DIG	= 3;	// 桁数
		const D3DXVECTOR3 POS	= D3DXVECTOR3(200.0f, 400.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(200.0f, 200.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(200.0f, 0.0f, 0.0f);		// 行間
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CStatusManager::mc_apTextureFileItem[] =	// モデル定数(フレイル)
{
	"data\\TEXTURE\\rule000.png",			// 鉄球
	"data\\TEXTURE\\rule001.png",			// 頭
	"data\\TEXTURE\\itemicon002.png",		// エスケープ君
	"data\\TEXTURE\\itemicon001.png",		// エスケープ君
	"data\\TEXTURE\\itemicon000.png",		// エスケープ君
	"data\\TEXTURE\\rule000.png",			// エスケープ君
	"data\\TEXTURE\\rule000.png",			// エスケープ君
	"data\\TEXTURE\\rule000.png",			// エスケープ君
};

//************************************************************
//	親クラス [CStatusManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CStatusManager::CStatusManager(const int nPadID) : m_nPadID(nPadID)
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
	m_pLife = CMultiValue::Create
	( // 引数
		CValue::TEXTURE_UI,	// テクスチャ
		life::MAX_LIFE,		// 数字
		life::MAX_DIG,		// 桁数
		D3DXVECTOR3(life::POS.x + (m_nPadID * 300.0f), life::POS.y, life::POS.x),			// 位置
		life::SIZE,			// 大きさ
		life::SPACE			// 行間
	);
	if (m_pLife == NULL)
	{ // 生成に失敗した場合

	  // 失敗を返す
		return E_FAIL;
	}

	// 優先順位を設定
	m_pLife->SetPriority(PRIORITY);

	// 最大値を設定
	m_pLife->SetMax(life::MAX_LIFE);

	// 吹っ飛び率の情報の設定
	m_pKnockRate = CMultiValue::Create
	( // 引数
		CValue::TEXTURE_UI,	// テクスチャ
		life::MAX_LIFE,		// 数字
		life::MAX_DIG,		// 桁数
		D3DXVECTOR3(life::POS.x + (m_nPadID * 300.0f), life::POS.y, life::POS.x),			// 位置
		life::SIZE,			// 大きさ
		life::SPACE			// 行間
	);
	if (m_pKnockRate == NULL)
	{ // 生成に失敗した場合

	  // 失敗を返す
		return E_FAIL;
	}
	// 優先順位を設定
	m_pKnockRate->SetPriority(PRIORITY);

	if (CManager::GetInstance()->GetRetentionManager()->GetKillState() == CRetentionManager::KILL_LIFE)
	{
		m_pKnockRate->SetEnableDraw(false);
		 m_pUI = CObject2D::Create(D3DXVECTOR3(life::POS.x + (m_nPadID * 300.0f) , life::POS.y + 10.0f, life::POS.x), D3DXVECTOR3(300.0f, 100.0f, 0.0f));
		 m_pUI->BindTexture("data\\TEXTURE\\Life_Only_UI.png");

		 m_pItemUI = CObject2D::Create(D3DXVECTOR3(life::POS.x + (m_nPadID * 300.0f) + 130.0f, life::POS.y - 0.0f, life::POS.x), D3DXVECTOR3(50.0f, 50.0f, 0.0f));
	}
	else if (CManager::GetInstance()->GetRetentionManager()->GetKillState() == CRetentionManager::KILL_KNOCK)
	{
		m_pLife->SetEnableDraw(false);
		m_pUI = CObject2D::Create(D3DXVECTOR3(life::POS.x + (m_nPadID * 300.0f) , life::POS.y + 10.0f, life::POS.x), D3DXVECTOR3(300.0f, 100.0f, 0.0f));
		m_pUI->BindTexture("data\\TEXTURE\\Damage_Only_UI.png");

		m_pItemUI = CObject2D::Create(D3DXVECTOR3(life::POS.x + (m_nPadID * 300.0f) + 130.0f, life::POS.y - 30.0f, life::POS.x), D3DXVECTOR3(50.0f, 50.0f, 0.0f));
	}

	if (m_pKnockRate == NULL)
	{ // 生成に失敗した場合

	  // 失敗を返す
		return E_FAIL;
	}
	m_pItemUI->BindTexture(mc_apTextureFileItem[0]);
	m_pItemUI->SetEnableDraw(false);
	// 優先順位を設定
	m_pItemUI->SetPriority(PRIORITY);

	switch (m_nPadID)
	{
	case 0:
		m_pUI->SetColor(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));
		break;
	case 1:
		m_pUI->SetColor(D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));
		break;
	case 2:
		m_pUI->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f));
		break;
	case 3:
		m_pUI->SetColor(D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f));
		break;
	default:
		break;
	}
	// 優先順位を設定
	m_pUI->SetPriority(PRIORITY);

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

	// 背景の終了
	m_pUI->Uninit();

	// 背景の終了
	m_pItemUI->Uninit();

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
//	最大体力取得処理
//============================================================
int CStatusManager::GetNumMaxLife(void) const
{
	// 最大体力を返す
	return m_pLife->GetMax();
}

//============================================================
//	体力の描画状況の設定処理
//============================================================
void CStatusManager::SetEnableDrawLife(const bool bDraw)
{
	if (CManager::GetInstance()->GetRetentionManager()->GetKillState() == CRetentionManager::KILL_LIFE)
	{ // 死亡条件が体力の場合

		// 引数の描画状況を設定
		m_pLife->SetEnableDraw(bDraw);
	}
}

//============================================================
//	吹っ飛び率の加算処理
//============================================================
void CStatusManager::AddNumRate(const int nAdd)
{
	// 引数の吹っ飛び率を加算
	m_pKnockRate->AddNum(nAdd);
}

//============================================================
//	吹っ飛び率の設定処理
//============================================================
void CStatusManager::SetNumRate(const int nSet)
{
	// 引数の吹っ飛び率を設定
	m_pKnockRate->SetNum(nSet);
}

//============================================================
//	吹っ飛び率取得処理
//============================================================
int CStatusManager::GetNumRate(void) const
{
	// 吹っ飛び率を返す
	return m_pKnockRate->GetNum();
}

//============================================================
//	吹っ飛び率取得処理
//============================================================
int CStatusManager::GetNumMaxRate(void) const
{
	// 最大吹っ飛び率を返す
	return m_pKnockRate->GetMax();
}

//============================================================
//	吹っ飛び率の描画状況の設定処理
//============================================================
void CStatusManager::SetEnableDrawRate(const bool bDraw)
{
	if (CManager::GetInstance()->GetRetentionManager()->GetKillState() == CRetentionManager::KILL_KNOCK)
	{ // 死亡条件が吹っ飛び率の場合

		// 引数の描画状況を設定
		m_pKnockRate->SetEnableDraw(bDraw);
	}
}

//============================================================
//	UIの描画設定処理
//============================================================
void CStatusManager::SetEnableDrawUI(const bool bDraw)
{
	// UIの描画状況を設定
	m_pUI->SetEnableDraw(bDraw);
}

//============================================================
//	アイテムUIの描画設定処理
//============================================================
void CStatusManager::SetEnableDrawItemUI(const bool bDraw)
{
	// UIの描画状況を設定
	m_pItemUI->SetEnableDraw(bDraw);
}

//============================================================
//	アイテムUIのテクスチャ設定処理
//============================================================
void CStatusManager::SetTextureItemUI(const int nItemID)
{
	if (nItemID != CPlayer::ITEM_HEAL)
	{ // 回復の場合は表示の変更なし

		m_pItemUI->BindTexture(mc_apTextureFileItem[nItemID]);
	}
}

//============================================================
//	生成処理
//============================================================
CStatusManager *CStatusManager::Create(const int nPadID)
{
	// ポインタを宣言
	CStatusManager *pStatusManager = NULL;		// ステータスマネージャー生成用

	if (pStatusManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pStatusManager = new CStatusManager(nPadID);	// ステータスマネージャー
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
