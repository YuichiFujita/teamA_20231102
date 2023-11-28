//============================================================
//
//	リザルトマネージャー処理 [resultManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "resultManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"
#include "texture.h"
#include "model.h"
#include "object2D.h"
#include "timerManager.h"
#include "retentionManager.h"

//************************************************************
//	マクロ定義
//************************************************************
#define RESULT_PRIO	(14)	// リザルトの優先順位

#define SETCOL_FADE		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f))	// α値の停止値
#define SIZE_FADE		(SCREEN_SIZE * 0.95f)	// フェードの大きさ
#define INITCOL_FADE	(XCOL_AWHITE)	// α値の初期値
#define ADD_ALPHA		(0.008f)		// α値の加算量

#define POS_RESULT_MISSION	(D3DXVECTOR3(380.0f, 150.0f, 0.0f))	// リザルト表示の遅刻回避の位置
#define POS_RESULT_RESULT	(D3DXVECTOR3(960.0f, 150.0f, 0.0f))	// リザルト表示の成功失敗の位置
#define SIZE_RESULT			(D3DXVECTOR3(632.7f, 203.5f, 0.0f))	// リザルト表示の大きさ
#define SET_RESULT_SCALE	(15.0f)	// リザルト表示の初期拡大率
#define SUB_RESULT_SCALE	(0.65f)	// リザルト表示拡大率の減算量

#define POS_TIME_LOGO	(D3DXVECTOR3(290.0f, 330.0f, 0.0f))	// タイムロゴ位置
#define SIZE_TIME_LOGO	(D3DXVECTOR3(487.5f, 154.7f, 0.0f))	// タイムロゴ大きさ
#define POS_TIME		(D3DXVECTOR3(555.0f, 330.0f, 0.0f))	// タイム位置
#define SIZE_TIME_VAL	(D3DXVECTOR3(98.0f, 117.0f, 0.0f))	// タイム数字大きさ
#define SIZE_TIME_PART	(D3DXVECTOR3(50.0f, 117.0f, 0.0f))	// タイム区切り大きさ
#define SPACE_TIME_VAL	(D3DXVECTOR3(SIZE_TIME_VAL.x  * 0.85f, 0.0f, 0.0f))	// タイム数字空白
#define SPACE_TIME_PART	(D3DXVECTOR3(SIZE_TIME_PART.x * 0.85f, 0.0f, 0.0f))	// タイム区切り空白
#define SET_TIME_SCALE	(8.0f)	// タイム表示の初期拡大率
#define SUB_TIME_SCALE	(0.3f)	// タイム表示拡大率の減算量
#define TIME_WAIT_CNT	(3)		// タイム表示状態への変更待機フレーム数

#define POS_CONT_LOGO	(D3DXVECTOR3(SCREEN_CENT.x, 490.0f, 0.0f))	// コンテニューロゴ位置
#define SIZE_CONT_LOGO	(D3DXVECTOR3(576.0f, 172.0f, 0.0f))	// コンテニューロゴ大きさ
#define POS_CONT_YES	(D3DXVECTOR3(400.0f, 610.0f, 0.0f))	// コンテニュー表示のYESの位置
#define POS_CONT_NO		(D3DXVECTOR3(880.0f, 610.0f, 0.0f))	// コンテニュー表示のNOの位置
#define SIZE_CONT		(D3DXVECTOR3(222.0f, 94.0f, 0.0f))	// コンテニュー表示の大きさ
#define SET_CONT_SCALE	(8.0f)	// コンテニュー表示の初期拡大率
#define SUB_CONT_SCALE	(0.3f)	// コンテニュー表示拡大率の減算量
#define CONT_WAIT_CNT	(3)		// コンテニュー表示状態への変更待機フレーム数

#define CHOICE_COL	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// 選択中カラー
#define DEFAULT_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// 非選択中カラー

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CResultManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\result000.png",		// 遅刻回避テクスチャ
	"data\\TEXTURE\\result001.png",		// 成功テクスチャ
	"data\\TEXTURE\\result002.png",		// 失敗テクスチャ
	"data\\TEXTURE\\result003.png",		// タイム表示テクスチャ
	"data\\TEXTURE\\continue000.png",	// コンテニュー表示テクスチャ
	"data\\TEXTURE\\continue001.png",	// YESテクスチャ
	"data\\TEXTURE\\continue002.png",	// NOテクスチャ
};

//************************************************************
//	親クラス [CResultManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CResultManager::CResultManager()
{
	// メンバ変数をクリア
	memset(&m_apResult[0], 0, sizeof(m_apResult));		// リザルト表示の情報
	memset(&m_apContinue[0], 0, sizeof(m_apContinue));	// コンテニュー表示の情報
	m_pContLogo		= NULL;			// コンテニューロゴの情報
	m_pTimeLogo		= NULL;			// タイムロゴの情報
	m_pFade			= NULL;			// フェードの情報
	m_pTime			= NULL;			// タイムの情報
	m_state			= STATE_NONE;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nSelect		= SELECT_YES;	// 現在の選択
	m_nOldSelect	= SELECT_YES;	// 前回の選択
	m_fScale		= 0.0f;			// ポリゴン拡大率
}

//============================================================
//	デストラクタ
//============================================================
CResultManager::~CResultManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CResultManager::Init(void)
{
	// 変数配列を宣言
	static D3DXVECTOR3 aPosResult[] =	// リザルトの位置
	{
		POS_RESULT_MISSION,	// MISSION位置
		POS_RESULT_RESULT,	// RESULT位置
	};
	static D3DXVECTOR3 aPosContinue[] =	// コンテニューの位置
	{
		POS_CONT_YES,	// YES位置
		POS_CONT_NO	,	// NO位置
	};

	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ

	// メンバ変数を初期化
	memset(&m_apResult[0], 0, sizeof(m_apResult));		// リザルト表示の情報
	memset(&m_apContinue[0], 0, sizeof(m_apContinue));	// コンテニュー表示の情報
	m_pContLogo		= NULL;			// コンテニューロゴの情報
	m_pTimeLogo		= NULL;			// タイムロゴの情報
	m_pFade			= NULL;			// フェードの情報
	m_pTime			= NULL;			// タイムの情報
	m_state			= STATE_FADEIN;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nSelect		= SELECT_YES;	// 現在の選択
	m_nOldSelect	= SELECT_YES;	// 前回の選択
	m_fScale		= 0.0f;			// ポリゴン拡大率

	//--------------------------------------------------------
	//	フェードの生成・設定
	//--------------------------------------------------------
	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SIZE_FADE,		// 大きさ
		VEC3_ZERO,		// 向き
		INITCOL_FADE	// 色
	);
	if (m_pFade == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pFade->SetPriority(RESULT_PRIO);

	//--------------------------------------------------------
	//	リザルト表示の生成・設定
	//--------------------------------------------------------
	for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{ // リザルト表示の総数分繰り返す

		// リザルト表示の生成
		m_apResult[nCntResult] = CObject2D::Create
		( // 引数
			aPosResult[nCntResult],			// 位置
			SIZE_RESULT * SET_RESULT_SCALE	// 大きさ
		);
		if (m_apResult[nCntResult] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apResult[nCntResult]->SetPriority(RESULT_PRIO);

		// 描画をしない設定にする
		m_apResult[nCntResult]->SetEnableDraw(false);
	}

	// リザルト表示のテクスチャを設定
	SetTexResult();

	//--------------------------------------------------------
	//	タイムロゴ表示の生成・設定
	//--------------------------------------------------------
	// タイムロゴ表示の生成
	m_pTimeLogo = CObject2D::Create
	( // 引数
		POS_TIME_LOGO,					// 位置
		SIZE_TIME_LOGO * SET_TIME_SCALE	// 大きさ
	);
	if (m_pTimeLogo == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pTimeLogo->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_TIME]));

	// 優先順位を設定
	m_pTimeLogo->SetPriority(RESULT_PRIO);

	// 描画をしない設定にする
	m_pTimeLogo->SetEnableDraw(false);

	//--------------------------------------------------------
	//	タイム表示の生成・設定
	//--------------------------------------------------------
	// タイマーマネージャーの生成
	m_pTime = CTimerManager::Create
	( // 引数
		CTimerManager::TIME_SEC,			// 設定タイム
		0,									// 制限時間
		POS_TIME,							// 位置
		SIZE_TIME_VAL * SET_TIME_SCALE,		// 数字の大きさ
		SIZE_TIME_PART * SET_TIME_SCALE,	// 区切りの大きさ
		SPACE_TIME_VAL,						// 数字の空白
		SPACE_TIME_PART						// 区切りの空白
	);
	if (m_pTime == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pTime->SetPriority(RESULT_PRIO);

	// 描画をしない設定にする
	m_pTime->SetEnableDraw(false);

	// タイムを設定
	if (!m_pTime->SetMSec(CManager::GetInstance()->GetRetentionManager()->GetTime()))
	{ // 設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	コンテニューロゴ表示の生成・設定
	//--------------------------------------------------------
	// コンテニューロゴ表示の生成
	m_pContLogo = CObject2D::Create
	( // 引数
		POS_CONT_LOGO,					// 位置
		SIZE_CONT_LOGO * SET_CONT_SCALE	// 大きさ
	);
	if (m_pContLogo == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pContLogo->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_CONTINUE]));

	// 優先順位を設定
	m_pContLogo->SetPriority(RESULT_PRIO);

	// 描画をしない設定にする
	m_pContLogo->SetEnableDraw(false);

	//--------------------------------------------------------
	//	コンテニュー表示の生成・設定
	//--------------------------------------------------------
	for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
	{ // 選択肢の総数分繰り返す

		// コンテニュー表示の生成
		m_apContinue[nCntResult] = CObject2D::Create
		( // 引数
			aPosContinue[nCntResult],	// 位置
			SIZE_CONT * SET_CONT_SCALE	// 大きさ
		);
		if (m_apContinue[nCntResult] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apContinue[nCntResult]->SetPriority(RESULT_PRIO);

		// 描画をしない設定にする
		m_apContinue[nCntResult]->SetEnableDraw(false);

		// 色を設定
		m_apContinue[nCntResult]->SetColor(DEFAULT_COL);
	}

	// テクスチャを登録・割当
	m_apContinue[SELECT_YES]->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_YES]));
	m_apContinue[SELECT_NO]->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_NO]));

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CResultManager::Uninit(void)
{
	// タイムの破棄
	if (FAILED(CTimerManager::Release(m_pTime)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{ // リザルト表示の総数分繰り返す

		// リザルト表示の終了
		m_apResult[nCntResult]->Uninit();
	}

	for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
	{ // 選択肢の総数分繰り返す

		// コンテニュー表示の終了
		m_apContinue[nCntResult]->Uninit();
	}

	// コンテニューロゴ表示の終了
	m_pContLogo->Uninit();

	// タイムロゴ表示の終了
	m_pTimeLogo->Uninit();

	// フェードの終了
	m_pFade->Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CResultManager::Update(void)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない状態

		// 無し

		break;

	case STATE_FADEIN:	// フェードイン状態

		// フェードインの更新
		UpdateFade();

		break;

	case STATE_RESULT:	// リザルト表示状態

		// リザルト表示の更新
		UpdateResult();

		break;

	case STATE_TIME_WAIT:	// タイム表示待機状態

		// 表示待機の更新
		if (UpdateDrawWait(TIME_WAIT_CNT))
		{ // 待機完了の場合

			// タイム表示の拡大率を設定
			m_fScale = SET_TIME_SCALE;

			// タイム表示の描画開始
			m_pTimeLogo->SetEnableDraw(true);
			m_pTime->SetEnableDraw(true);

			// 状態を変更
			m_state = STATE_TIME;	// タイム表示状態
		}

		break;

	case STATE_TIME:	// タイム表示状態

		// タイム表示の更新
		UpdateTime();

		break;

	case STATE_CONTINUE_WAIT:	// コンテニュー表示待機状態

		// 表示待機の更新
		if (UpdateDrawWait(CONT_WAIT_CNT))
		{ // 待機完了の場合

			// コンテニュー表示の拡大率を設定
			m_fScale = SET_CONT_SCALE;

			// コンテニューロゴ表示の描画開始
			m_pContLogo->SetEnableDraw(true);
			
			for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
			{ // 選択肢の総数分繰り返す

				// コンテニュー表示の描画開始
				m_apContinue[nCntResult]->SetEnableDraw(true);
			}

			// 状態を変更
			m_state = STATE_CONTINUE;	// コンテニュー表示状態
		}

		break;

	case STATE_CONTINUE:	// コンテニュー表示待機状態表示状態

		// コンテニュー表示の更新
		UpdateContinue();

		break;

	case STATE_WAIT:	// 遷移待機状態

		// 選択の更新
		UpdateSelect();

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 遷移決定の更新
	UpdateTransition();

	for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{ // リザルト表示の総数分繰り返す

		// リザルト表示の更新
		m_apResult[nCntResult]->Update();
	}

	for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
	{ // 選択肢の総数分繰り返す

		// コンテニュー表示の更新
		m_apContinue[nCntResult]->Update();
	}

	// コンテニューロゴ表示の更新
	m_pContLogo->Update();

	// タイムロゴ表示の更新
	m_pTimeLogo->Update();

	// タイムの更新
	m_pTime->Update();

	// フェードの更新
	m_pFade->Update();
}

//============================================================
//	生成処理
//============================================================
CResultManager *CResultManager::Create(void)
{
	// ポインタを宣言
	CResultManager *pResultManager = NULL;	// リザルトマネージャー生成用

	if (pResultManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pResultManager = new CResultManager;	// リザルトマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pResultManager != NULL)
	{ // 使用されている場合
		
		// リザルトマネージャーの初期化
		if (FAILED(pResultManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pResultManager;
			pResultManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pResultManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CResultManager::Release(CResultManager *&prResultManager)
{
	if (prResultManager != NULL)
	{ // 使用中の場合

		// リザルトマネージャーの終了
		if (FAILED(prResultManager->Uninit()))
		{ // 破棄に失敗した場合

			// メモリ開放
			delete prResultManager;
			prResultManager = NULL;

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prResultManager;
		prResultManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	フェードインの更新処理
//============================================================
void CResultManager::UpdateFade(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	if (colFade.a < SETCOL_FADE.a)
	{ // 透明量が設定値未満の場合

		// 透明度を加算
		colFade.a += ADD_ALPHA;
	}
	else
	{ // 透明量が設定値以上の場合

		// 透明度を補正
		colFade.a = SETCOL_FADE.a;

		for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
		{ // リザルト表示の総数分繰り返す

			// リザルト表示の描画開始
			m_apResult[nCntResult]->SetEnableDraw(true);
		}

		// リザルト表示の拡大率を設定
		m_fScale = SET_RESULT_SCALE;

		// 状態を変更
		m_state = STATE_RESULT;	// リザルト表示状態
	}

	// 透明度を反映
	m_pFade->SetColor(colFade);
}

//============================================================
//	リザルト表示の更新処理
//============================================================
void CResultManager::UpdateResult(void)
{
	if (m_fScale > 1.0f)
	{ // 拡大率が最小値より大きい場合

		// 拡大率を減算
		m_fScale -= SUB_RESULT_SCALE;

		for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
		{ // リザルト表示の総数分繰り返す

			// リザルト表示の大きさを設定
			m_apResult[nCntResult]->SetVec3Sizing(SIZE_RESULT * m_fScale);
		}
	}
	else
	{ // 拡大率が最小値以下の場合

		for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
		{ // リザルト表示の総数分繰り返す

			// リザルト表示の大きさを設定
			m_apResult[nCntResult]->SetVec3Sizing(SIZE_RESULT);
		}

		switch (CManager::GetInstance()->GetRetentionManager()->GetResult())
		{ // リザルトごとの処理
		case CRetentionManager::RESULT_FAILED:

			// 状態を変更
			m_state = STATE_CONTINUE_WAIT;	// コンテニュー表示待機状態

			break;

		case CRetentionManager::RESULT_CLEAR:

			// 状態を変更
			m_state = STATE_TIME_WAIT;	// タイム表示待機状態

			break;

		default:

			// エラーメッセージボックス
			MessageBox(NULL, "リザルトなしが設定されています", "警告！", MB_ICONWARNING);

			// 状態を変更
			m_state = STATE_TIME_WAIT;	// タイム表示待機状態

			break;
		}

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
	}
}

//============================================================
//	タイム表示処理
//============================================================
void CResultManager::UpdateTime(void)
{
	if (m_fScale > 1.0f)
	{ // 拡大率が最小値より大きい場合

		// 拡大率を減算
		m_fScale -= SUB_TIME_SCALE;

		// タイム表示の大きさを設定
		m_pTimeLogo->SetVec3Sizing(SIZE_TIME_LOGO * m_fScale);
		m_pTime->SetScalingValue(SIZE_TIME_VAL * m_fScale);
		m_pTime->SetScalingPart(SIZE_TIME_PART * m_fScale);
	}
	else
	{ // 拡大率が最小値以下の場合

		// 拡大率を補正
		m_fScale = 1.0f;

		// タイム表示の大きさを設定
		m_pTimeLogo->SetVec3Sizing(SIZE_TIME_LOGO);
		m_pTime->SetScalingValue(SIZE_TIME_VAL);
		m_pTime->SetScalingPart(SIZE_TIME_PART);

		// 状態を変更
		m_state = STATE_CONTINUE_WAIT;	// コンテニュー表示待機状態

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
	}
}

//============================================================
//	コンテニュー表示処理
//============================================================
void CResultManager::UpdateContinue(void)
{
	if (m_fScale > 1.0f)
	{ // 拡大率が最小値より大きい場合

		// 拡大率を減算
		m_fScale -= SUB_CONT_SCALE;

		// コンテニューロゴ表示の大きさを設定
		m_pContLogo->SetVec3Sizing(SIZE_CONT_LOGO * m_fScale);

		for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
		{ // 選択肢の総数分繰り返す

			// コンテニュー表示の大きさを設定
			m_apContinue[nCntResult]->SetVec3Sizing(SIZE_CONT * m_fScale);
		}
	}
	else
	{ // 拡大率が最小値以下の場合

		// 拡大率を補正
		m_fScale = 1.0f;

		// コンテニューロゴ表示の大きさを設定
		m_pContLogo->SetVec3Sizing(SIZE_CONT_LOGO);

		for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
		{ // 選択肢の総数分繰り返す

			// コンテニュー表示の大きさを設定
			m_apContinue[nCntResult]->SetVec3Sizing(SIZE_CONT);
		}

		// 状態を変更
		m_state = STATE_WAIT;	// 遷移待機状態

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
	}
}

//============================================================
//	選択の更新処理
//============================================================
void CResultManager::UpdateSelect(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド

	if (pKeyboard->IsTrigger(DIK_A)
	||  pKeyboard->IsTrigger(DIK_LEFT)
	||  pPad->IsTrigger(CInputPad::KEY_LEFT))
	{ // 左移動の操作が行われた場合

		// 左に選択をずらす
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
	}
	if (pKeyboard->IsTrigger(DIK_D)
	||  pKeyboard->IsTrigger(DIK_RIGHT)
	||  pPad->IsTrigger(CInputPad::KEY_RIGHT))
	{ // 右移動の操作が行われた場合

		// 右に選択をずらす
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
	}

	// 前回の選択要素の色を黒に設定
	m_apContinue[m_nOldSelect]->SetColor(DEFAULT_COL);

	// 現在の選択要素の色を白に設定
	m_apContinue[m_nSelect]->SetColor(CHOICE_COL);

	// 現在の選択要素を代入
	m_nOldSelect = m_nSelect;
}

//============================================================
//	遷移決定の更新処理
//============================================================
void CResultManager::UpdateTransition(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド

	if (pKeyboard->IsTrigger(DIK_RETURN)
	||  pKeyboard->IsTrigger(DIK_SPACE)
	||  pPad->IsTrigger(CInputPad::KEY_A)
	||  pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X)
	||  pPad->IsTrigger(CInputPad::KEY_Y)
	||  pPad->IsTrigger(CInputPad::KEY_START))
	{
		if (m_state != STATE_WAIT)
		{ // 遷移待機状態ではない場合

			// 演出スキップ
			SkipStaging();

			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
		}
		else
		{ // 遷移待機状態の場合

			if (CManager::GetInstance()->GetFade()->GetState() == CFade::FADE_NONE)
			{ // フェード中ではない場合

				switch (m_nSelect)
				{ // 選択ごとの処理
				case SELECT_YES:

					// シーンの設定
					CManager::GetInstance()->SetScene(CScene::MODE_GAME);	// ゲーム画面

					break;

				case SELECT_NO:

					// シーンの設定
					CManager::GetInstance()->SetScene(CScene::MODE_RANKING);	// ランキング画面

					break;
				}

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// 決定音00
			}
		}
	}
}

//============================================================
//	演出スキップ処理
//============================================================
void CResultManager::SkipStaging(void)
{
	// リザルト表示の描画をONにし、大きさを設定
	for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{ // リザルト表示の総数分繰り返す

		// リザルト表示の描画開始
		m_apResult[nCntResult]->SetEnableDraw(true);

		// リザルト表示の大きさを設定
		m_apResult[nCntResult]->SetVec3Sizing(SIZE_RESULT);
	}

	if (CManager::GetInstance()->GetRetentionManager()->GetResult() == CRetentionManager::RESULT_CLEAR)
	{ // クリアしている場合

		// タイム表示をONにする
		m_pTimeLogo->SetEnableDraw(true);
		m_pTime->SetEnableDraw(true);

		// タイム表示の大きさを設定
		m_pTimeLogo->SetVec3Sizing(SIZE_TIME_LOGO);
		m_pTime->SetScalingValue(SIZE_TIME_VAL);
		m_pTime->SetScalingPart(SIZE_TIME_PART);
	}

	// コンテニューロゴ表示の描画開始
	m_pContLogo->SetEnableDraw(true);

	// コンテニューロゴ表示の大きさを設定
	m_pContLogo->SetVec3Sizing(SIZE_CONT_LOGO);

	for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
	{ // 選択肢の総数分繰り返す

		// コンテニュー表示の描画開始
		m_apContinue[nCntResult]->SetEnableDraw(true);

		// コンテニュー表示の大きさを設定
		m_apContinue[nCntResult]->SetVec3Sizing(SIZE_CONT);
	}

	// フェードの透明度を設定
	m_pFade->SetColor(SETCOL_FADE);

	// 状態を変更
	m_state = STATE_WAIT;	// 遷移待機状態
}

//============================================================
//	リザルト表示のテクスチャの設定処理
//============================================================
void CResultManager::SetTexResult(void)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ

	// MISSIONテクスチャを登録・割当
	m_apResult[0]->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_MISSION]));

	// RESULTテクスチャを登録・割当
	switch (CManager::GetInstance()->GetRetentionManager()->GetResult())
	{ // リザルトごとの処理
	case CRetentionManager::RESULT_FAILED:

		// FAILEDテクスチャ
		m_apResult[1]->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_FAILED]));

		break;

	case CRetentionManager::RESULT_CLEAR:

		// CLEARテクスチャ
		m_apResult[1]->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_CLEAR]));

		break;

	default:

		// エラーメッセージボックス
		MessageBox(NULL, "リザルトなしが設定されています", "警告！", MB_ICONWARNING);

		break;
	}
}

//============================================================
//	表示待機処理
//============================================================
bool CResultManager::UpdateDrawWait(const int nWait)
{
	if (m_nCounterState < nWait)
	{ // カウンターが待機カウントまで達していない場合

		// カウンターを加算
		m_nCounterState++;

		// 待機未完了を返す
		return false;
	}
	else
	{ // カウンターが待機完了した場合

		// カウンターを初期化
		m_nCounterState = 0;

		// 待機完了を返す
		return true;
	}
}
