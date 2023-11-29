//============================================================
//
//	ポーズ処理 [pause.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "pause.h"
#include "renderer.h"
#include "manager.h"
#include "sceneGame.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "object2D.h"
#include "timerManager.h"

//************************************************************
//	マクロ定義
//************************************************************
#define PAUSE_PRIO	(14)	// ポーズ表示の優先順位

#define PAUSE_POS	(D3DXVECTOR3(640.0f, 210.0f, 0.0f))	// セレクトメニューの位置
#define PAUSE_SIZE	(D3DXVECTOR3(440.0f, 110.0f, 0.0f))	// セレクトメニューの大きさ
#define PAUSE_SPACE	(D3DXVECTOR3(0.0f, 150.0f, 0.0f))	// セレクトメニューの空白

#define BG_COL		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f))	// 背景カラー
#define CHOICE_COL	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// 選択中カラー
#define DEFAULT_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// 非選択中カラー

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CPause::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\pause000.png",	// 再開テクスチャ
	"data\\TEXTURE\\pause001.png",	// リトライテクスチャ
	"data\\TEXTURE\\pause002.png",	// 終了テクスチャ
};

//************************************************************
//	子クラス [CPause] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPause::CPause()
{
	// メンバ変数をクリア
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択情報
	m_pBG		 = NULL;			// 背景情報
	m_nSelect	 = SELECT_RESUME;	// 現在の選択
	m_nOldSelect = SELECT_RESUME;	// 前回の選択
	m_bPause	 = false;			// ポーズ状況
}

//============================================================
//	デストラクタ
//============================================================
CPause::~CPause()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPause::Init(void)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャ

	// メンバ変数を初期化
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択情報
	m_pBG		 = NULL;			// 背景情報
	m_nSelect	 = SELECT_RESUME;	// 現在の選択
	m_nOldSelect = SELECT_RESUME;	// 前回の選択
	m_bPause	 = false;			// ポーズ状況

	// 背景情報の生成
	m_pBG = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SCREEN_SIZE,	// 大きさ
		VEC3_ZERO,		// 向き
		BG_COL			// 色
	);
	if (m_pBG == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pBG->SetPriority(PAUSE_PRIO);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // 選択肢の項目数分繰り返す

		// 選択情報の生成
		m_apSelect[nCntPause] = CObject2D::Create
		( // 引数
			PAUSE_POS + (PAUSE_SPACE * (float)nCntPause),	// 位置
			PAUSE_SIZE,	// 大きさ
			VEC3_ZERO,	// 向き
			DEFAULT_COL	// 色
		);
		if (m_apSelect[nCntPause] == NULL)
		{ // 非使用中の場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apSelect[nCntPause]->SetPriority(PAUSE_PRIO);

		// テクスチャを登録・割当
		m_apSelect[nCntPause]->BindTexture(pTexture->Regist(mc_apTextureFile[nCntPause]));
	}

	// 描画状況の設定
	SetEnableDraw(m_bPause);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPause::Uninit(void)
{
	// 背景情報の終了
	m_pBG->Uninit();

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // 選択肢の項目数分繰り返す

		// 選択情報の終了
		m_apSelect[nCntPause]->Uninit();
	}
}

//============================================================
//	更新処理
//============================================================
void CPause::Update(void)
{
	if (CManager::GetInstance()->GetFade()->GetState() == CFade::FADE_NONE)
	{ // フェードしていない場合

		if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_P)
		||  CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_START))
		{
			// ポーズ状況を切り替え
			m_bPause = (!m_bPause) ? true : false;

			// タイムの計測状況を切り替え
			CSceneGame::GetTimerManager()->EnableStop(m_bPause);

			// 現在の選択を初期化
			m_nSelect = SELECT_RESUME;

			// 描画状況の設定
			SetEnableDraw(m_bPause);

			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// 決定音00
		}
	}

	if (m_bPause)
	{ // ポーズ中の場合

		// 選択操作
		Select();

		// 前回の選択要素の色を黒に設定
		m_apSelect[m_nOldSelect]->SetColor(DEFAULT_COL);

		// 現在の選択要素の色を白に設定
		m_apSelect[m_nSelect]->SetColor(CHOICE_COL);

		// 現在の選択要素を代入
		m_nOldSelect = m_nSelect;
	}

	// 背景情報の更新
	m_pBG->Update();

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // 選択肢の項目数分繰り返す

		// 選択情報の更新
		m_apSelect[nCntPause]->Update();
	}
}

//============================================================
//	描画処理
//============================================================
void CPause::Draw(void)
{

}

//============================================================
//	ポーズ状況取得処理
//============================================================
bool CPause::IsPause(void)
{
	// ポーズ状況を返す
	return m_bPause;
}

//============================================================
//	描画状況の設定処理
//============================================================
void CPause::SetEnableDraw(const bool bDraw)
{
	// 変数を宣言
	bool bDrawPause = bDraw;	// ポーズの描画状況

	if (!CSceneGame::IsDrawPause())
	{ // ポーズを描画しない状況だった場合

		bDrawPause = false;
	}

	// 背景情報の描画状況を設定
	m_pBG->SetEnableDraw(bDrawPause);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // 選択肢の項目数分繰り返す

		// 選択情報の描画状況を設定
		m_apSelect[nCntPause]->SetEnableDraw(bDrawPause);
	}
}

//============================================================
//	生成処理
//============================================================
CPause *CPause::Create(void)
{
	// ポインタを宣言
	CPause *pPause = NULL;		// ポーズ生成用

	if (pPause == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pPause = new CPause;	// ポーズ
	}
	else { assert(false); return NULL; }	// 使用中

	if (pPause != NULL)
	{ // 確保に成功している場合

		// ポーズの初期化
		if (FAILED(pPause->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pPause;
			pPause = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pPause;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CPause::Release(CPause *&prPause)
{
	if (prPause != NULL)
	{ // 使用中の場合

		// ポーズの終了
		prPause->Uninit();

		// メモリ開放
		delete prPause;
		prPause = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	選択操作処理
//============================================================
void CPause::Select(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド

	if (CManager::GetInstance()->GetFade()->GetState() == CFade::FADE_NONE)
	{ // フェードしていない場合

		if (pKeyboard->IsTrigger(DIK_W)
		||  pKeyboard->IsTrigger(DIK_UP)
		||  pPad->IsTrigger(CInputPad::KEY_UP))
		{ // 上移動の操作が行われた場合

			// 上に選択をずらす
			m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;

			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
		}
		if (pKeyboard->IsTrigger(DIK_S)
		||  pKeyboard->IsTrigger(DIK_DOWN)
		||  pPad->IsTrigger(CInputPad::KEY_DOWN))
		{ // 下移動の操作が行われた場合

			// 下に選択をずらす
			m_nSelect = (m_nSelect + 1) % SELECT_MAX;

			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
		}

		if (pKeyboard->IsTrigger(DIK_RETURN)  || pKeyboard->IsTrigger(DIK_SPACE)
		||  pPad->IsTrigger(CInputPad::KEY_A) || pPad->IsTrigger(CInputPad::KEY_B)
		||  pPad->IsTrigger(CInputPad::KEY_X) || pPad->IsTrigger(CInputPad::KEY_Y))
		{ // 決定の操作が行われた場合

			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// 決定音00

			switch (m_nSelect)
			{ // 選択ごとの処理
			case SELECT_RESUME:	// 再開

				// ポーズを終了する
				m_bPause = false;

				// タイムの計測を再開する
				CSceneGame::GetTimerManager()->EnableStop(m_bPause);

				// 描画状況の設定
				SetEnableDraw(m_bPause);

				// 処理を抜ける
				break;

			case SELECT_RETRY:	// リトライ

				// シーンの設定
				CManager::GetInstance()->SetScene(CScene::MODE_GAME);	// ゲーム画面

				// 処理を抜ける
				break;

			case SELECT_EXIT:	// 終了

				// シーンの設定
				CManager::GetInstance()->SetScene(CScene::MODE_TITLE);	// タイトル画面

				// 処理を抜ける
				break;
			}
		}
	}
}
