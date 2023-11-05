//============================================================
//
//	フェード処理 [fade.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "fade.h"
#include "manager.h"
#include "object2D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define FADE_PRIO	(15)	// フェードの優先順位
#define FADE_LEVEL	(0.05f)	// フェードのα値の加減量

#if _DEBUG

#define INIT_SCENE	(CScene::MODE_TITLE)	// 初期シーン

#else

#define INIT_SCENE	(CScene::MODE_TITLE)	// 初期シーン

#endif

//************************************************************
//	親クラス [CFade] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFade::CFade()
{
	// メンバ変数をクリア
	m_pObject2D	= NULL;					// フェード表示の情報
	m_fade		= FADE_NONE;			// フェード状態
	m_modeNext	= CScene::MODE_TITLE;	// 次のシーンモード
	m_nCounterWait = 0;					// 余韻管理カウンター
}

//============================================================
//	デストラクタ
//============================================================
CFade::~CFade()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CFade::Init(void)
{
	// メンバ変数を初期化
	m_pObject2D = NULL;			// フェード表示の情報
	m_fade		= FADE_IN;		// フェード状態
	m_modeNext	= INIT_SCENE;	// 次のシーンモード
	m_nCounterWait = 0;			// 余韻管理カウンター

	// オブジェクト2Dの生成
	m_pObject2D = CObject2D::Create
	( // 引数
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),	// 位置
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),					// 大きさ
		VEC3_ZERO,	// 向き
		XCOL_WHITE	// 色
	);
	if (m_pObject2D == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位の設定
	m_pObject2D->SetPriority(FADE_PRIO);

	// モードの設定
	CManager::GetInstance()->SetMode(m_modeNext);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CFade::Uninit(void)
{
	// オブジェクト2Dの終了
	m_pObject2D->Uninit();
}

//============================================================
//	更新処理
//============================================================
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{ // 何もしない状態ではない場合

		// 変数を宣言
		D3DXCOLOR colFade = m_pObject2D->GetColor();	// フェード色

		switch (m_fade)
		{ // フェード状態ごとの処理
		case FADE_WAIT:	// フェード余韻状態

			if (m_nCounterWait > 0)
			{ // カウンターが 0より大きい場合

				// カウンターを減算
				m_nCounterWait--;
			}
			else
			{ // カウンターが 0以下の場合

				// フェード状態を設定
				m_fade = FADE_OUT;	// フェードアウト状態
			}

			break;

		case FADE_IN:	// フェードイン状態

			// α値を減算
			colFade.a -= FADE_LEVEL;

			if (colFade.a <= 0.0f)
			{ // α値が 0.0fを下回った場合

				// α値を補正
				colFade.a = 0.0f;

				// フェード状態を設定
				m_fade = FADE_NONE;	// 何もしない状態
			}

			break;

		case FADE_OUT:	// フェードアウト状態

			// α値を加算
			colFade.a += FADE_LEVEL;

			if (colFade.a >= 1.0f)
			{ // α値が 1.0f を上回った場合

				// α値を補正
				colFade.a = 1.0f;

				// フェード状態を設定
				m_fade = FADE_IN;	// フェードイン状態

				// モードの設定
				CManager::GetInstance()->SetMode(m_modeNext);
			}

			break;

		default:	// 例外処理
			assert(false);
			break;
		}

		// 色の更新
		m_pObject2D->SetColor(colFade);

		// オブジェクト2Dの更新
		m_pObject2D->Update();
	}
}

//============================================================
//	次シーンへのフェード設定処理
//============================================================
void CFade::Set(const CScene::EMode mode, const int nWait)
{
	// 例外処理
	assert(nWait >= 0);	// 余韻フレームオーバー

	if (m_fade == FADE_NONE)
	{ // フェードが行われていない場合

		// 引数のモードを設定
		m_modeNext = mode;

		// 余韻管理カウンターを設定
		m_nCounterWait = nWait;

		// フェード状態を設定
		if (m_nCounterWait <= 0)
		{ // カウンターが 0以下の場合

			m_fade = FADE_OUT;	// フェードアウト状態
		}
		else
		{ // カウンターが 0より大きい場合

			m_fade = FADE_WAIT;	// フェード余韻状態
		}
	}
}

//============================================================
//	フェード状態取得処理
//============================================================
CFade::EFade CFade::GetState(void) const
{
	// 現在のフェード状態を返す
	return m_fade;
}

//============================================================
//	生成処理
//============================================================
CFade *CFade::Create(void)
{
	// ポインタを宣言
	CFade *pFade = NULL;	// フェード生成用

	if (pFade == NULL)
	{ // 使用されていない場合

		// メモリを確保
		pFade = new CFade;	// フェード
	}
	else { assert(false); return NULL; }	// 使用中

	if (pFade != NULL)
	{ // 確保に成功している場合

		// フェードの初期化
		if (FAILED(pFade->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pFade;
			pFade = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pFade;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CFade::Release(CFade *&prFade)
{
	if (prFade != NULL)
	{ // 使用中の場合

		// フェードの終了
		prFade->Uninit();

		// メモリ開放
		delete prFade;
		prFade = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
