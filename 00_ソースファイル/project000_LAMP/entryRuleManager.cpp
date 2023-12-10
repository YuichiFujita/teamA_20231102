//============================================================
//
//	エントリールールマネージャー処理 [entryRuleManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "entryRuleManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"
#include "multiValue.h"
#include "retentionManager.h"
#include "fade.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 13;	// エントリーの優先順位

	const D3DXCOLOR COL_RULE	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 参加中カラー
	const D3DXCOLOR COL_UNRULE	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// 非参加中カラー

	namespace number
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(165.0f, 90.0f, 0.0f);		// 位置
		const D3DXVECTOR3	SIZE_TITLE	= D3DXVECTOR3(242.0f, 107.0f, 0.0f);	// タイトル大きさ
		const D3DXVECTOR3	SIZE_VALUE	= D3DXVECTOR3(80.0f, 90.0f, 0.0f);		// 数字大きさ
		const D3DXVECTOR3	SPACE_POS	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// 数字UI同士の空白
		const D3DXVECTOR3	SPACE_TITLE	= D3DXVECTOR3(100.0f, 5.0f, 0.0f);		// タイトル空白
		const D3DXVECTOR3	SPACE_VALUE	= VEC3_ZERO;							// 数字空白
		const int			DIGIT		= 1;									// 桁数
	}

	namespace frame
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(160.0f, 360.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(250.0f, 440.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// 空白
	}

	namespace control
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, 630.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(505.0f, 84.0f, 0.0f);			// 大きさ
	}

	namespace start
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(1100.0f, 600.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(300.0f, 180.0f, 0.0f);	// 大きさ
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CEntryRuleManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\entry_player.png",	// PLAYERテクスチャ
	"data\\TEXTURE\\entry_flame.png",	// フレームテクスチャ
	"data\\TEXTURE\\entry002.png",	// 操作表示テクスチャ
	"data\\TEXTURE\\entry003.png",	// 開始表示テクスチャ
};

//************************************************************
//	親クラス [CEntryRuleManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEntryRuleManager::CEntryRuleManager()
{
	// メンバ変数をクリア
	m_pControl	= NULL;	// 操作表示の情報
	m_pStart	= NULL;	// 開始表示の情報
}

//============================================================
//	デストラクタ
//============================================================
CEntryRuleManager::~CEntryRuleManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEntryRuleManager::Init(void)
{
	// メンバ変数を初期化
	m_pControl	= NULL;	// 操作表示の情報
	m_pStart	= NULL;	// 開始表示の情報

	// ゲーム情報を初期化
	CManager::GetInstance()->GetRetentionManager()->InitGame();

	// 操作表示の生成
	m_pControl = CObject2D::Create
	( // 引数
		control::POS,	// 位置
		control::SIZE	// 大きさ
	);
	if (m_pControl == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pControl->BindTexture(mc_apTextureFile[TEXTURE_CONTROL]);

	// 優先順位を設定
	m_pControl->SetPriority(PRIORITY);

	// 開始表示の生成
	m_pStart = CObject2D::Create
	( // 引数
		start::POS,	// 位置
		start::SIZE	// 大きさ
	);
	if (m_pStart == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pStart->BindTexture(mc_apTextureFile[TEXTURE_START]);

	// 優先順位を設定
	m_pStart->SetPriority(PRIORITY);

	// プレイ人数を初期化
	CManager::GetInstance()->GetRetentionManager()->SetNumPlayer(0);

	// エントリーを初期化
	CManager::GetInstance()->GetRetentionManager()->AllSetEnableEntry(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CEntryRuleManager::Uninit(void)
{
	// 操作表示の終了
	m_pControl->Uninit();

	// 開始表示の終了
	m_pStart->Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CEntryRuleManager::Update(void)
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::FADE_NONE)
	{ // フェード中の場合

		// 処理を抜ける
		return;
	}

	// 操作表示の更新
	m_pControl->Update();

	// 開始表示の更新
	m_pStart->Update();
}

//============================================================
//	生成処理
//============================================================
CEntryRuleManager *CEntryRuleManager::Create(void)
{
	// ポインタを宣言
	CEntryRuleManager *pEntryRuleManager = NULL;	// エントリールールマネージャー生成用

	if (pEntryRuleManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pEntryRuleManager = new CEntryRuleManager;	// エントリールールマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pEntryRuleManager != NULL)
	{ // 使用されている場合
		
		// エントリールールマネージャーの初期化
		if (FAILED(pEntryRuleManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pEntryRuleManager;
			pEntryRuleManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pEntryRuleManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CEntryRuleManager::Release(CEntryRuleManager *&prEntryRuleManager)
{
	if (prEntryRuleManager != NULL)
	{ // 使用中の場合

		// エントリールールマネージャーの終了
		if (FAILED(prEntryRuleManager->Uninit()))
		{ // 破棄に失敗した場合

			// メモリ開放
			delete prEntryRuleManager;
			prEntryRuleManager = NULL;

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prEntryRuleManager;
		prEntryRuleManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
