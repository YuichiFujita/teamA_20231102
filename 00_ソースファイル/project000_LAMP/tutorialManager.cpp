//============================================================
//
//	チュートリアルマネージャー処理 [tutorialManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "tutorialManager.h"
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
	const int PRIORITY = 14;	// チュートリアルの優先順位

	namespace lesson
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(640.0f, 580.0f, 0.0f);	// レッスン表示の位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(1000.0f, 270.0f, 0.0f);	// レッスン表示の位置
	}

	namespace control
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(1140.0f, 85.0f, 0.0f);	// 操作方法の位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(264.0f, 150.0f, 0.0f);	// 操作方法の位置
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CTutorialManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\tutorial000.png",	// 操作説明テクスチャ
};
const char *CTutorialManager::mc_apLessonTextureFile[] =	// レッスンテクスチャ定数
{
	"data\\TEXTURE\\lesson000.png",	// ジャンプ説明テクスチャ
	"data\\TEXTURE\\lesson001.png",	// スライディング説明テクスチャ
	"data\\TEXTURE\\lesson002.png",	// 壁走り説明テクスチャ
	"data\\TEXTURE\\lesson003.png",	// 壁ジャンプ説明テクスチャ
};

//************************************************************
//	親クラス [CTutorialManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTutorialManager::CTutorialManager()
{
	// メンバ変数をクリア
	m_pExplain = NULL;	// 説明表示の情報
	m_pControl = NULL;	// 操作説明の情報
}

//============================================================
//	デストラクタ
//============================================================
CTutorialManager::~CTutorialManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTutorialManager::Init(void)
{
	// メンバ変数を初期化
	m_pExplain = NULL;	// 説明表示の情報
	m_pControl = NULL;	// 操作説明の情報

	// 説明表示の生成
	m_pExplain = CObject2D::Create
	( // 引数
		lesson::POS,	// 位置
		lesson::SIZE	// 大きさ
	);
	if (m_pExplain == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pExplain->SetPriority(PRIORITY);

	// 操作説明の生成
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

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CTutorialManager::Uninit(void)
{
	// 説明表示の終了
	m_pExplain->Uninit();

	// 操作説明の終了
	m_pControl->Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CTutorialManager::Update(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド

	if (pKeyboard->IsTrigger(DIK_RETURN) || pPad->IsTriggerAll(CInputPad::KEY_START))
	{
		// シーンの設定
		CManager::GetInstance()->SetScene(CScene::MODE_TITLE);	// タイトル画面
	}

	// 説明表示の更新
	m_pExplain->Update();

	// 操作説明の更新
	m_pControl->Update();
}

//============================================================
//	生成処理
//============================================================
CTutorialManager *CTutorialManager::Create(void)
{
	// ポインタを宣言
	CTutorialManager *pTutorialManager = NULL;	// チュートリアルマネージャー生成用

	if (pTutorialManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pTutorialManager = new CTutorialManager;	// チュートリアルマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pTutorialManager != NULL)
	{ // 使用されている場合
		
		// チュートリアルマネージャーの初期化
		if (FAILED(pTutorialManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pTutorialManager;
			pTutorialManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pTutorialManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CTutorialManager::Release(CTutorialManager *&prTutorialManager)
{
	if (prTutorialManager != NULL)
	{ // 使用中の場合

		// チュートリアルマネージャーの終了
		if (FAILED(prTutorialManager->Uninit()))
		{ // 破棄に失敗した場合

			// メモリ開放
			delete prTutorialManager;
			prTutorialManager = NULL;

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prTutorialManager;
		prTutorialManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
