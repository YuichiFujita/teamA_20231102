//============================================================
//
//	中間リザルトマネージャー処理 [middleResultManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "middleResultManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 14;	// 中間リザルトの優先順位

	// フェード基本情報
	namespace fade
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, -120.0f, 0.0f);	// フェード位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(SCREEN_SIZE.x, 1770.0f, 0.0f);	// フェード大きさ
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CMiddleResultManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\middleResult000.png",	// 操作説明テクスチャ
};

//************************************************************
//	親クラス [CMiddleResultManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CMiddleResultManager::CMiddleResultManager()
{
	// メンバ変数をクリア
	m_pFade = NULL;	// フェードの情報
}

//============================================================
//	デストラクタ
//============================================================
CMiddleResultManager::~CMiddleResultManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CMiddleResultManager::Init(void)
{
	// メンバ変数を初期化
	m_pFade = NULL;	// フェードの情報

	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		fade::POS,	// 位置
		fade::SIZE	// 大きさ
	);
	if (m_pFade == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pFade->BindTexture(mc_apTextureFile[TEXTURE_FADE]);

	// 優先順位を設定
	m_pFade->SetPriority(PRIORITY);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CMiddleResultManager::Uninit(void)
{
	// フェードの終了
	m_pFade->Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CMiddleResultManager::Update(void)
{
	// フェードの更新
	m_pFade->Update();
}

//============================================================
//	生成処理
//============================================================
CMiddleResultManager *CMiddleResultManager::Create(void)
{
	// ポインタを宣言
	CMiddleResultManager *pMiddleResultManager = NULL;	// 中間リザルトマネージャー生成用

	if (pMiddleResultManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pMiddleResultManager = new CMiddleResultManager;	// 中間リザルトマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pMiddleResultManager != NULL)
	{ // 使用されている場合
		
		// 中間リザルトマネージャーの初期化
		if (FAILED(pMiddleResultManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pMiddleResultManager;
			pMiddleResultManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pMiddleResultManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CMiddleResultManager::Release(CMiddleResultManager *&prMiddleResultManager)
{
	if (prMiddleResultManager != NULL)
	{ // 使用中の場合

		// 中間リザルトマネージャーの終了
		if (FAILED(prMiddleResultManager->Uninit()))
		{ // 破棄に失敗した場合

			// メモリ開放
			delete prMiddleResultManager;
			prMiddleResultManager = NULL;

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prMiddleResultManager;
		prMiddleResultManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
