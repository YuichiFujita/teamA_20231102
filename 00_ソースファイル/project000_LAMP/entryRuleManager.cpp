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
#include "sceneEntry.h"
#include "entryManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 14;	// エントリーの優先順位

	namespace fade
	{
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 初期化色
		const D3DXCOLOR SET_COL		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);	// 設定色

		const float	ADD_ALPHA	= 0.025f;	// 透明度の加算量
		const float	SUB_ALPHA	= 0.025f;	// 透明度の減算量
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CEntryRuleManager::mc_apTextureFile[] =	// テクスチャ定数
{
	NULL,	// テクスチャ
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
	m_pFade	= NULL;			// フェードの情報
	m_state	= STATE_FADEIN;	// 状態
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
	m_pFade	= NULL;			// フェードの情報
	m_state	= STATE_FADEIN;	// 状態

	// 操作表示の生成
	m_pFade = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SCREEN_SIZE,	// 大きさ
		VEC3_ZERO,		// 向き
		fade::INIT_COL	// 色
	);
	if (m_pFade == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pFade->SetPriority(PRIORITY);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CEntryRuleManager::Uninit(void)
{
	// フェードの終了
	m_pFade->Uninit();

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

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_FADEIN:

		// フェードインの更新
		UpdateFadeIn();

		break;

	case STATE_WAIT:



		break;

	case STATE_FADEOUT:

		// フェードアウトの更新
		UpdateFadeOut();

		break;

	case STATE_ENTRYBACK:

		// エントリー状態に戻る
		CSceneEntry::GetEntryManager()->SetState(CEntryManager::STATE_ENTRY);

		// 関数を抜ける
		return;

	default:
		assert(false);
		break;
	}

	// フェードの更新
	m_pFade->Update();
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

//============================================================
//	フェードインの更新処理
//============================================================
void CEntryRuleManager::UpdateFadeIn(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	// 透明度を加算
	colFade.a += fade::ADD_ALPHA;

	if (colFade.a >= fade::SET_COL.a)
	{ // 透明度が上がりきった場合

		// 透明度を補正
		colFade.a = fade::SET_COL.a;

		// 待機状態にする
		m_state = STATE_WAIT;
	}

	// 透明度を反映
	m_pFade->SetColor(colFade);
}

//============================================================
//	フェードアウトの更新処理
//============================================================
void CEntryRuleManager::UpdateFadeOut(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	// 透明度を減算
	colFade.a -= fade::SUB_ALPHA;

	if (colFade.a <= fade::INIT_COL.a)
	{ // 透明度が下がりきった場合

		// 透明度を補正
		colFade.a = fade::INIT_COL.a;

		// エントリー戻し状態にする
		m_state = STATE_ENTRYBACK;
	}

	// 透明度を反映
	m_pFade->SetColor(colFade);
}
