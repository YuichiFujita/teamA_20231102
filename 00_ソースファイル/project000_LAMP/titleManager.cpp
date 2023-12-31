//============================================================
//
//	タイトルマネージャー処理 [titleManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "titleManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"
#include "texture.h"
#include "model.h"
#include "object2D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define TITLE_PRIO	(14)	// タイトルの優先順位

#define POS_LOGO_NEVER	(D3DXVECTOR3(SCREEN_WIDTH * 0.3f,  300.0f, 0.0f))	// タイトルロゴの位置 (NEVER)
#define POS_LOGO_GIVE	(D3DXVECTOR3(SCREEN_WIDTH * 0.3f,  300.0f, 0.0f))	// タイトルロゴの位置 (GIVE)
#define POS_LOGO_UP		(D3DXVECTOR3(1050.0f, 320.0f, 0.0f))	// タイトルロゴの位置 (UP!)

#define SIZE_TITLE	(D3DXVECTOR3(600.0f, 600.0f, 0.0f) * 0.8f)	// タイトルロゴの大きさ
#define INIT_SCALE	(15.0f)	// タイトルロゴの初期拡大率
#define SUB_SCALE	(0.65f)	// タイトルロゴ拡大率の減算量

#define COL_FADE	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f))	// タイトルフェードの色
#define SUB_ALPHA	(0.008f)	// α値の減算量

#define POS_SELECTBG	(D3DXVECTOR3(640.0f, 560.0f, 0.0f))					// 選択背景の位置
#define SIZE_SELECTBG	(D3DXVECTOR3((float)SCREEN_WIDTH, 120.0f, 0.0f))	// 選択背景の大きさ
#define COL_SELECTBG	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f))					// 選択背景の色

#define POS_SELECT		(D3DXVECTOR3(SCREEN_WIDTH * 0.75f, 600.0f, 0.0f))	// 選択の位置
#define SPACE_SELECT	(D3DXVECTOR3(560.0f, 0.0f, 0.0f))	// 選択の空間
#define SIZE_SELECT		(D3DXVECTOR3(466.0f, 64.0f, 0.0f))	// 選択の大きさ

#define CHOICE_COL	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// 選択中カラー
#define DEFAULT_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// 非選択中カラー

#define LOGO_POS (D3DXVECTOR3(SCREEN_WIDTH * 0.35f,  320.0f, 0.0f))


D3DXVECTOR3 aTitle_Size[] =
{
	D3DXVECTOR3(1000.0f, 600.0f, 0.0f) * 0.9f,
	D3DXVECTOR3(600.0f, 600.0f, 0.0f) * 0.9f
};
//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CTitleManager::mc_apLogoTextureFile[] =	// ロゴテクスチャ定数
{
	"data\\TEXTURE\\Crack.png",	//テクスチャ
	"data\\TEXTURE\\Logo_Ball.png",	// テクスチャ
};
const char *CTitleManager::mc_apSelectTextureFile[] =	// 選択テクスチャ定数
{
	"data\\TEXTURE\\Press_Start.png",	// STARTテクスチャ
	"data\\TEXTURE\\title004.png",	// MANUALテクスチャ
};

//************************************************************
//	親クラス [CTitleManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTitleManager::CTitleManager()
{
	// メンバ変数をクリア
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));		// タイトル表示の情報
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択表示の情報
	m_pFade = NULL;			// フェードの情報
	m_pSelectBG = NULL;		// 選択背景の情報
	m_state = STATE_NONE;	// 状態
	m_fScale = 0.0f;		// タイトル拡大率
	m_nSelect = 0;			// 現在の選択
	m_nOldSelect = 0;		// 前回の選択
}

//============================================================
//	デストラクタ
//============================================================
CTitleManager::~CTitleManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTitleManager::Init(void)
{
	// 変数配列を宣言
	D3DXVECTOR3 aPosLogo[] =	// ロゴの位置
	{ // 初期値
		POS_LOGO_NEVER,	// NEVERの位置
		POS_LOGO_GIVE,	// GIVEの位置
		POS_LOGO_UP,	// UP!の位置
	};

	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ

	// メンバ変数を初期化
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));		// タイトル表示の情報
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択表示の情報
	m_pFade = NULL;				// フェードの情報
	m_pSelectBG = NULL;			// 選択背景の情報
	m_state = STATE_FADEOUT;	// 状態
	m_fScale = INIT_SCALE;		// タイトル拡大率
	m_nSelect = 0;				// 現在の選択
	m_nOldSelect = 0;			// 前回の選択


	//--------------------------------------------------------
	//	選択表示の生成・設定
	//--------------------------------------------------------
	for (int nCntTitle = 0; nCntTitle < SELECT_MAX; nCntTitle++)
	{ // 選択項目の総数分繰り返す

		// 選択表示の生成
		m_apSelect[nCntTitle] = CObject2D::Create
		( // 引数
			POS_SELECT + (SPACE_SELECT * (float)nCntTitle),	// 位置
			SIZE_SELECT	// 大きさ
		);
		if (m_apSelect[nCntTitle] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_apSelect[nCntTitle]->BindTexture(pTexture->Regist(mc_apSelectTextureFile[nCntTitle]));

		// 優先順位を設定
		m_apSelect[nCntTitle]->SetPriority(TITLE_PRIO);

		// 描画をしない設定にする
		m_apSelect[nCntTitle]->SetEnableDraw(false);
	}

	//--------------------------------------------------------
	//	タイトルロゴの生成・設定
	//--------------------------------------------------------
	for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
	{ // タイトルロゴの総数分繰り返す
		m_apLogo[nCntTitle] = CObject2D::Create
		( // 引数
			LOGO_POS,	// 位置
			aTitle_Size[nCntTitle] * INIT_SCALE	// 大きさ
		);
		if (m_apLogo[nCntTitle] == NULL)
		{ // 生成に失敗した場合

		  // 失敗を返す
			assert(false);
			return E_FAIL;
		}
		// テクスチャを登録・割当
		m_apLogo[nCntTitle]->BindTexture(pTexture->Regist(mc_apLogoTextureFile[nCntTitle]));

		// 優先順位を設定
		m_apLogo[nCntTitle]->SetPriority(TITLE_PRIO);

		// 描画をしない設定にする
		m_apLogo[nCntTitle]->SetEnableDraw(false);
	}

	//--------------------------------------------------------
	//	フェードの生成・設定
	//--------------------------------------------------------
	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SCREEN_SIZE,	// 大きさ
		VEC3_ZERO,		// 向き
		COL_FADE		// 色
	);
	if (m_pFade == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pFade->SetPriority(TITLE_PRIO);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTitleManager::Uninit(void)
{
	for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
	{ // タイトルロゴの総数分繰り返す

		// タイトルロゴの終了
		m_apLogo[nCntTitle]->Uninit();
	}

	for (int nCntTitle = 0; nCntTitle < SELECT_MAX; nCntTitle++)
	{ // 選択項目の総数分繰り返す

		// 選択表示の終了
		m_apSelect[nCntTitle]->Uninit();
	}

	// フェードの終了
	m_pFade->Uninit();

	
}

//============================================================
//	更新処理
//============================================================
void CTitleManager::Update(void)
{
	// 遷移決定の更新
	UpdateStart();

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない状態
		break;

	case STATE_FADEOUT:	// フェードアウト状態

		// フェードアウトの更新
		UpdateFade();

		break;

	case STATE_MOVE:	// タイトル縮小状態

		// タイトル移動の更新
		UpdateMove();

		break;

	case STATE_WAIT:	// 遷移待機状態
		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
	{ // タイトルロゴの総数分繰り返す

		// タイトルロゴの更新
		m_apLogo[nCntTitle]->Update();
	}

	for (int nCntTitle = 0; nCntTitle < SELECT_MAX; nCntTitle++)
	{ // 選択項目の総数分繰り返す

		// 選択表示の更新
		m_apSelect[nCntTitle]->Update();
	}

	// フェードの更新
	m_pFade->Update();
}

//============================================================
//	生成処理
//============================================================
CTitleManager *CTitleManager::Create(void)
{
	// ポインタを宣言
	CTitleManager *pTitleManager = NULL;	// タイトルマネージャー生成用

	if (pTitleManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pTitleManager = new CTitleManager;	// タイトルマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pTitleManager != NULL)
	{ // 使用されている場合
		
		// タイトルマネージャーの初期化
		if (FAILED(pTitleManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pTitleManager;
			pTitleManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pTitleManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CTitleManager::Release(CTitleManager *&prTitleManager)
{
	if (prTitleManager != NULL)
	{ // 使用中の場合

		// タイトルマネージャーの終了
		prTitleManager->Uninit();

		// メモリ開放
		delete prTitleManager;
		prTitleManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	フェードアウトの更新処理
//============================================================
void CTitleManager::UpdateFade(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	if (colFade.a > 0.0f)
	{ // 透明になっていない場合

		// 透明度を減算
		colFade.a -= SUB_ALPHA;
	}
	else
	{ // 透明になった場合

		// 透明度を補正
		colFade.a = 0.0f;

		for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
		{ // タイトルロゴの総数分繰り返す

			// 描画をする設定にする
			m_apLogo[nCntTitle]->SetEnableDraw(true);
		}
		m_apLogo[0]->SetEnableDraw(false);
		for (int nCntTitle = 0; nCntTitle < SELECT_MAX; nCntTitle++)
		{ // 選択項目の総数分繰り返す

			// 選択表示を描画する設定にする
			m_apSelect[nCntTitle]->SetEnableDraw(true);
		}

		// 状態を変更
		m_state = STATE_MOVE;	// タイトル移動状態
	}

	// 透明度を反映
	m_pFade->SetColor(colFade);
}

//============================================================
//	タイトル移動の更新処理
//============================================================
void CTitleManager::UpdateMove(void)
{
	if (m_fScale > 1.0f)
	{ // 拡大率が最小値より大きい場合

		// 拡大率を減算
		m_fScale -= SUB_SCALE;

		for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
		{ // タイトルロゴの総数分繰り返す

			// タイトルロゴの大きさを設定
			m_apLogo[nCntTitle]->SetVec3Sizing(aTitle_Size[nCntTitle] * m_fScale);
		}
	}
	else
	{ // 拡大率が最小値以下の場合

		// 拡大率を補正
		m_fScale = 1.0f;

		for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
		{ // タイトルロゴの総数分繰り返す

			// タイトルロゴの大きさを設定
			m_apLogo[nCntTitle]->SetVec3Sizing(aTitle_Size[nCntTitle]);
			// 描画をする設定にする
			m_apLogo[nCntTitle]->SetEnableDraw(true);
		}

		// 状態を変更
		m_state = STATE_WAIT;	// 遷移待機状態

		// カメラの更新を再開
		CManager::GetInstance()->GetCamera()->SetEnableUpdate(true);

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT);	// 決定音01
	}
}

//============================================================
//	遷移決定の更新処理
//============================================================
void CTitleManager::UpdateStart(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド

	if (pKeyboard->IsTrigger(DIK_RETURN)
	||  pKeyboard->IsTrigger(DIK_SPACE)
	||  pPad->IsTriggerAll(CInputPad::KEY_A)
	||  pPad->IsTriggerAll(CInputPad::KEY_B)
	||  pPad->IsTriggerAll(CInputPad::KEY_X)
	||  pPad->IsTriggerAll(CInputPad::KEY_Y)
	||  pPad->IsTriggerAll(CInputPad::KEY_START))
	{
		if (m_state != STATE_WAIT)
		{ // 遷移待機状態ではない場合

			// 演出スキップ
			SkipStaging();

			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT);	// 決定音01
		}
		else
		{ // 遷移待機状態の場合

			if (CManager::GetInstance()->GetFade()->GetState() == CFade::FADE_NONE)
			{ // フェード中ではない場合

				switch (m_nSelect)
				{ // 選択ごとの処理
				case SELECT_GAME:

					// シーンの設定
					CManager::GetInstance()->SetScene(CScene::MODE_ENTRY);	// エントリー画面

					break;

				default:
					assert(false);
					break;
				}

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
			}
		}
	}
}

//============================================================
//	演出スキップ処理
//============================================================
void CTitleManager::SkipStaging(void)
{
	// タイトルロゴを表示状態に設定・大きさを正規化
	for (int nCntTitle = 0; nCntTitle < LOGO_MAX; nCntTitle++)
	{ // タイトルロゴの総数分繰り返す

		// タイトルロゴの大きさを設定
		m_apLogo[nCntTitle]->SetVec3Sizing(aTitle_Size[nCntTitle]);

		// 描画をする設定にする
		m_apLogo[nCntTitle]->SetEnableDraw(true);
	}

	// 選択表示を描画する設定にする
	for (int nCntTitle = 0; nCntTitle < SELECT_MAX; nCntTitle++)
	{ // 選択項目の総数分繰り返す

		m_apSelect[nCntTitle]->SetEnableDraw(true);
	}

	// フェードを透明にする
	m_pFade->SetColor(XCOL_ABLACK);


	// カメラの更新を再開
	CManager::GetInstance()->GetCamera()->SetEnableUpdate(true);

	// 状態を変更
	m_state = STATE_WAIT;	// 遷移待機状態
}
