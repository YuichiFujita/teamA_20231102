//============================================================
//
//	ランキングマネージャー処理 [rankingManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "rankingManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"
#include "texture.h"
#include "object2D.h"
#include "anim2D.h"
#include "timerManager.h"
#include "retentionManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	// ランキング基本情報
	const char* RANKING_BIN = "data\\BIN\\ranking.bin";	// ランキング情報バイナリ

	const int		PRIORITY	= 14;	// ランキングの優先順位
	const D3DXCOLOR	COL_NEWRANK = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);	// 変動したスコアの色

	// フェード基本情報
	namespace fade
	{
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(SCREEN_SIZE.x * 0.7f, SCREEN_SIZE.y * 0.95f, 0.0f);	// フェードの大きさ
		const D3DXCOLOR		SET_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);	// α値の停止値

		const D3DXCOLOR		INIT_COL	= XCOL_AWHITE;	// α値の初期値
		const float			ADD_ALPHA	= 0.008f;		// α値の加算量
	}

	// ランキングロゴ基本情報
	namespace logo
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, 115.0f, 0.0f);	// ランキングロゴ表示の位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(473.0f, 199.0f, 0.0f);		// ランキングロゴ表示の大きさ
		const float SET_SCALE	= 15.0f;	// ランキングロゴ表示の初期拡大率
		const float SUB_SCALE	= 0.65f;	// ランキングロゴ表示の拡大率減算量
	}

	// 順位基本情報
	namespace rank
	{
		const D3DXVECTOR3	POS		= D3DXVECTOR3(310.0f, 250.0f, 0.0f);	// 順位表示の位置
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(183.0f, 108.0f, 0.0f);	// 順位表示の大きさ
		const D3DXVECTOR3	SPACE	= D3DXVECTOR3(0.0f, 100.0f, 0.0f);		// 順位表示の空白

		const POSGRID2	TEX_PART	= POSGRID2(NUM_RANKING, 1);	// 順位表示のテクスチャ分割数
		const D3DXCOLOR	SET_COL		= XCOL_WHITE;	// α値の停止値
		const D3DXCOLOR	INIT_COL	= XCOL_AWHITE;	// α値の初期値
		const float		ADD_ALPHA	= 0.02f;		// α値の加算量
	}

	// クリアタイム基本情報
	namespace time
	{
		const D3DXVECTOR3 POS			= D3DXVECTOR3(485.0f, 250.0f, 0.0f);	// クリアタイム表示の位置
		const D3DXVECTOR3 SPACE			= D3DXVECTOR3(0.0f, 100.0f, 0.0f);		// クリアタイム表示の空白
		const D3DXVECTOR3 SIZE_VAL		= D3DXVECTOR3(88.0f, 105.0f, 0.0f);		// クリアタイム表示の数字大きさ
		const D3DXVECTOR3 SIZE_PART		= D3DXVECTOR3(45.0f, 105.0f, 0.0f);		// クリアタイム表示の区切り大きさ
		const D3DXVECTOR3 SPACE_VAL		= D3DXVECTOR3(SIZE_VAL.x  * 0.85f, 0.0f, 0.0f);	// クリアタイム表示の数字空白
		const D3DXVECTOR3 SPACE_PART	= D3DXVECTOR3(SIZE_PART.x * 0.85f, 0.0f, 0.0f);	// クリアタイム表示の区切り空白

		const float	SET_SCALE		= 0.05f;	// クリアタイム表示の初期拡大率
		const float	ADD_SCALE		= 0.1f;		// クリアタイム表示の拡大率加算量
		const float	MAX_SUB_SCALE	= 0.085f;	// クリアタイム表示の拡大率の最大減算量
		const int	WAIT_CNT		= 15;		// クリアタイム表示待機フレーム
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CRankingManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\ranking000.png",	// ランキングロゴテクスチャ
	"data\\TEXTURE\\ranking001.png",	// ランキング順位テクスチャ
};
long CRankingManager::m_aRanking[NUM_RANKING] = { 0 };	// ランキング情報
int CRankingManager::m_nNewRankID = NONE_IDX;	// 変動したスコアのインデックス

//************************************************************
//	親クラス [CRankingManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CRankingManager::CRankingManager()
{
	// メンバ変数をクリア
	memset(&m_apTime[0], 0, sizeof(m_apTime));	// クリアタイムの情報
	memset(&m_apRank[0], 0, sizeof(m_apRank));	// 順位の情報
	m_pLogo		= NULL;			// ランキングロゴの情報
	m_pFade		= NULL;			// フェードの情報
	m_state		= STATE_NONE;	// 状態
	m_fScale	= 0.0f;			// ポリゴン拡大率
	m_nCounterState = 0;		// 状態管理カウンター
	m_nCounterDraw = 0;			// 描画管理カウンター
}

//============================================================
//	デストラクタ
//============================================================
CRankingManager::~CRankingManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CRankingManager::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apTime[0], 0, sizeof(m_apTime));	// クリアタイムの情報
	memset(&m_apRank[0], 0, sizeof(m_apRank));	// 順位の情報
	m_pLogo		= NULL;					// ランキングロゴの情報
	m_pFade		= NULL;					// フェードの情報
	m_state		= STATE_FADEIN;			// 状態
	m_fScale	= 0.0f;					// ポリゴン拡大率
	m_nCounterState = 0;				// 状態管理カウンター
	m_nCounterDraw = NUM_RANKING - 1;	// 描画管理カウンター

	// ランキング読込
	Load();

	//--------------------------------------------------------
	//	フェードの生成・設定
	//--------------------------------------------------------
	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		fade::SIZE,		// 大きさ
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

	//--------------------------------------------------------
	//	ランキングロゴ表示の生成・設定
	//--------------------------------------------------------
	// ランキングロゴ表示の生成
	m_pLogo = CObject2D::Create
	( // 引数
		logo::POS,						// 位置
		logo::SIZE * logo::SET_SCALE	// 大きさ
	);
	if (m_pLogo == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを設定
	m_pLogo->BindTexture(mc_apTextureFile[TEXTURE_LOGO]);

	// 優先順位を設定
	m_pLogo->SetPriority(PRIORITY);

	// 描画をしない設定にする
	m_pLogo->SetEnableDraw(false);

	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // ランキングの上位表示数分繰り返す

		//----------------------------------------------------
		//	順位表示の生成・設定
		//----------------------------------------------------
		// 順位表示の生成
		m_apRank[nCntRank] = CAnim2D::Create
		( // 引数
			rank::TEX_PART.x,								// 横分割数
			rank::TEX_PART.y,								// 縦分割数
			rank::POS + (rank::SPACE * (float)nCntRank),	// 位置
			rank::SIZE,										// 大きさ
			VEC3_ZERO,										// 向き
			rank::INIT_COL									// 色
		);
		if (m_apRank[nCntRank] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを設定
		m_apRank[nCntRank]->BindTexture(mc_apTextureFile[TEXTURE_RANK]);

		// 優先順位を設定
		m_apRank[nCntRank]->SetPriority(PRIORITY);

		// 描画をしない設定にする
		m_apRank[nCntRank]->SetEnableDraw(false);

		// パターンの設定
		m_apRank[nCntRank]->SetPattern(nCntRank);

		//----------------------------------------------------
		//	クリアタイム表示の生成・設定
		//----------------------------------------------------
		// クリアタイム表示の生成
		m_apTime[nCntRank] = CTimerManager::Create
		( // 引数
			CTimerManager::TIME_SEC,						// 設定タイム
			0,												// 制限時間
			time::POS + (time::SPACE * (float)nCntRank),	// 位置
			time::SIZE_VAL * time::SET_SCALE,				// 数字の大きさ
			time::SIZE_PART * time::SET_SCALE,				// 区切りの大きさ
			time::SPACE_VAL,								// 数字の空白
			time::SPACE_PART								// 区切りの空白
		);
		if (m_apTime[nCntRank] == NULL)
		{ // 非使用中の場合
	
			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	
		// 優先順位を設定
		m_apTime[nCntRank]->SetPriority(PRIORITY);
	
		// 描画をしない設定にする
		m_apTime[nCntRank]->SetEnableDraw(false);

		if (m_nNewRankID == nCntRank)
		{ // 値の変動があった場合

			// 色を設定
			m_apTime[nCntRank]->SetColor(COL_NEWRANK);

			// 変動したスコアのインデックスを初期化
			m_nNewRankID = NONE_IDX;
		}
	
		// タイムを設定
		if (!m_apTime[nCntRank]->SetMSec(m_aRanking[nCntRank]))
		{ // 設定に失敗した場合
	
			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CRankingManager::Uninit(void)
{
	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // ランキングの上位表示数分繰り返す

		// クリアタイムの破棄
		if (FAILED(CTimerManager::Release(m_apTime[nCntRank])))
		{ // 破棄に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 順位表示の終了
		m_apRank[nCntRank]->Uninit();
	}

	// ランキングロゴの終了
	m_pLogo->Uninit();

	// フェードの終了
	m_pFade->Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CRankingManager::Update(void)
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

	case STATE_LOGO:	// ランキングロゴ表示状態

		// ランキングロゴ表示の更新
		UpdateLogo();

		break;

	case STATE_RANK:	// 順位表示状態

		// 順位表示の更新
		UpdateRank();

		break;

	case STATE_TIME_WAIT:	// クリアタイム表示待機状態

		// 表示待機の更新
		if (UpdateDrawWait(time::WAIT_CNT))
		{ // 待機完了の場合

			// クリアタイム表示の初期化
			InitTime();
		}

		break;

	case STATE_TIME:	// クリアタイム表示状態

		// クリアタイム表示の更新
		UpdateTime();

		break;

	case STATE_WAIT:	// 遷移待機状態

		// 無し

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 遷移決定の更新
	UpdateTransition();

	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // ランキングの上位表示数分繰り返す

		// クリアタイムの更新
		m_apTime[nCntRank]->Update();

		// 順位表示の更新
		m_apRank[nCntRank]->Update();
	}

	// ランキングロゴの更新
	m_pLogo->Update();

	// フェードの更新
	m_pFade->Update();
}

//============================================================
//	設定処理
//============================================================
void CRankingManager::Set(const long nValue)
{
	// 変数を宣言
	long nLowestID = NUM_RANKING - 1;	// 最下位の配列インデックス

	// 読込
	Load();

	if (nValue < m_aRanking[nLowestID])
	{ // 最下位のクリアタイムより早いクリアタイムの場合

		// ソート
		Sort(nValue);

		// 保存
		Save();

		// スコア変動インデックスの設定
		SetNewRank(nValue);
	}
}

//============================================================
//	生成処理
//============================================================
CRankingManager *CRankingManager::Create(void)
{
	// ポインタを宣言
	CRankingManager *pRankingManager = NULL;	// ランキングマネージャー生成用

	if (pRankingManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pRankingManager = new CRankingManager;	// ランキングマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pRankingManager != NULL)
	{ // 使用されている場合
		
		// ランキングマネージャーの初期化
		if (FAILED(pRankingManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pRankingManager;
			pRankingManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pRankingManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CRankingManager::Release(CRankingManager *&prRankingManager)
{
	if (prRankingManager != NULL)
	{ // 使用中の場合

		// ランキングマネージャーの終了
		if (FAILED(prRankingManager->Uninit()))
		{ // 破棄に失敗した場合

			// メモリ開放
			delete prRankingManager;
			prRankingManager = NULL;

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prRankingManager;
		prRankingManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	フェードインの更新処理
//============================================================
void CRankingManager::UpdateFade(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	if (colFade.a < fade::SET_COL.a)
	{ // 透明量が設定値未満の場合

		// 透明度を加算
		colFade.a += fade::ADD_ALPHA;
	}
	else
	{ // 透明量が設定値以上の場合

		// 透明度を補正
		colFade.a = fade::SET_COL.a;

		// ランキングロゴ表示の初期化
		InitLogo();

		// 状態を変更
		m_state = STATE_LOGO;	// ランキングロゴ表示状態
	}

	// 透明度を反映
	m_pFade->SetColor(colFade);
}

//============================================================
//	フェードインの補正処理
//============================================================
void CRankingManager::RevisionFade(void)
{
	// フェードの透明度を設定
	m_pFade->SetColor(fade::SET_COL);
}

//============================================================
//	ランキングロゴ表示の初期化処理
//============================================================
void CRankingManager::InitLogo(void)
{
	// 自動描画をONに設定
	m_pLogo->SetEnableDraw(true);

	// ランキングロゴ表示の拡大率を設定
	m_fScale = logo::SET_SCALE;
}

//============================================================
//	ランキングロゴ表示の更新処理
//============================================================
void CRankingManager::UpdateLogo(void)
{
	if (m_fScale > 1.0f)
	{ // 拡大率が最小値より大きい場合

		// 拡大率を減算
		m_fScale -= logo::SUB_SCALE;

		// ランキングロゴ表示の大きさを設定
		m_pLogo->SetVec3Sizing(logo::SIZE * m_fScale);
	}
	else
	{ // 拡大率が最小値以下の場合

		// ランキング表示の補正
		RevisionLogo();

		// 順位表示の初期化
		InitRank();

		// 状態を変更
		m_state = STATE_RANK;	// 順位表示状態

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
	}
}

//============================================================
//	ランキングロゴ表示の補正処理
//============================================================
void CRankingManager::RevisionLogo(void)
{
	// 拡大率を初期化
	m_fScale = 1.0f;

	// ランキングロゴ表示の大きさを設定
	m_pLogo->SetVec3Sizing(logo::SIZE);
}

//============================================================
//	順位表示の初期化処理
//============================================================
void CRankingManager::InitRank(void)
{
	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // ランキングの上位表示数分繰り返す

		// 透明度を反映
		m_apRank[nCntRank]->SetEnableDraw(true);
	}
}

//============================================================
//	順位表示の更新処理
//============================================================
void CRankingManager::UpdateRank(void)
{
	// 変数を宣言
	D3DXCOLOR colRank = m_apRank[0]->GetColor();	// 順位の色

	if (colRank.a < rank::SET_COL.a)
	{ // 透明量が設定値未満の場合

		// 透明度を加算
		colRank.a += rank::ADD_ALPHA;
	}
	else
	{ // 透明量が設定値以上の場合

		// 透明度を補正
		colRank.a = rank::SET_COL.a;

		// 状態を変更
		m_state = STATE_TIME_WAIT;	// クリアタイム表示待機状態
	}

	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // ランキングの上位表示数分繰り返す

		// 透明度を反映
		m_apRank[nCntRank]->SetColor(colRank);
	}
}

//============================================================
//	順位表示の補正処理
//============================================================
void CRankingManager::RevisionRank(void)
{
	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // ランキングの上位表示数分繰り返す

		// 透明度を反映
		m_apRank[nCntRank]->SetColor(rank::SET_COL);
	}
}

//============================================================
//	クリアタイム表示の初期化処理
//============================================================
void CRankingManager::InitTime(void)
{
	if (m_nCounterDraw >= 0)
	{ // 描画し終わっていない場合

		// 自動描画をONに設定
		m_apTime[m_nCounterDraw]->SetEnableDraw(true);

		// クリアタイム表示の拡大率を設定
		m_fScale = time::SET_SCALE;

		// クリアタイム表示状態にする
		m_state = STATE_TIME;
	}
	else
	{ // 描画し終わった場合

		// 拡大率を初期化
		m_fScale = 1.0f;

		// 待機状態にする
		m_state = STATE_WAIT;
	}
}

//============================================================
//	クリアタイム表示の更新処理
//============================================================
void CRankingManager::UpdateTime(void)
{
	if (m_fScale < 1.0f)
	{ // 拡大率が最小値より大きい場合

		// 拡大率を加算
		m_fScale += time::ADD_SCALE - ((time::MAX_SUB_SCALE) / (float)(NUM_RANKING - 1)) * fabsf((float)m_nCounterDraw - (float)(NUM_RANKING - 1));

		// クリアタイム表示の大きさを設定
		m_apTime[m_nCounterDraw]->SetScalingValue(time::SIZE_VAL * m_fScale);
		m_apTime[m_nCounterDraw]->SetScalingPart(time::SIZE_PART * m_fScale);
	}
	else
	{ // 拡大率が最小値以下の場合

		// クリアタイム表示の補正
		RevisionTime();

		// 状態を変更
		m_state = STATE_TIME_WAIT;	// クリアタイム表示待機状態

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
	}
}

//============================================================
//	クリアタイム表示の補正処理
//============================================================
void CRankingManager::RevisionTime(void)
{
	// 拡大率を初期化
	m_fScale = 1.0f;

	// クリアタイム表示の大きさを設定
	m_apTime[m_nCounterDraw]->SetScalingValue(time::SIZE_VAL);
	m_apTime[m_nCounterDraw]->SetScalingPart(time::SIZE_PART);

	// 描画カウンターを減算
	m_nCounterDraw--;
}

//============================================================
//	遷移決定の更新処理
//============================================================
void CRankingManager::UpdateTransition(void)
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

				// シーンの設定
				CManager::GetInstance()->SetScene(CScene::MODE_TITLE);	// タイトル画面

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// 決定音00
			}
		}
	}
}

//============================================================
//	演出スキップ処理
//============================================================
void CRankingManager::SkipStaging(void)
{
	// フェードインの補正
	RevisionFade();

	// ランキングロゴ表示の初期化・補正
	InitLogo();
	RevisionLogo();

	// 順位表示の初期化・補正
	InitRank();
	RevisionRank();

	// クリアタイム表示の初期化・補正
	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // ランキングの上位表示数分繰り返す

		// 自動描画をONに設定
		m_apTime[nCntRank]->SetEnableDraw(true);

		// クリアタイム表示の大きさを設定
		m_apTime[nCntRank]->SetScalingValue(time::SIZE_VAL);
		m_apTime[nCntRank]->SetScalingPart(time::SIZE_PART);
	}

	// 状態を変更
	m_state = STATE_WAIT;	// 遷移待機状態
}

//============================================================
//	表示待機処理
//============================================================
bool CRankingManager::UpdateDrawWait(const int nWait)
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

//============================================================
//	ソート処理
//============================================================
void CRankingManager::Sort(const long nValue)
{
	// 変数を宣言
	long nKeepNum;		// ソート用
	int	nCurrentMinID;	// 最小値のインデックス

	// 現在の最下位の情報と書き換え
	m_aRanking[NUM_RANKING - 1] = nValue;

	for (int nCntKeep = 0; nCntKeep < (NUM_RANKING - 1); nCntKeep++)
	{ // 入れ替える値の総数 -1回分繰り返す

		// 現在の繰り返し数を代入 (要素1とする)
		nCurrentMinID = nCntKeep;

		for (int nCntSort = nCntKeep + 1; nCntSort < NUM_RANKING; nCntSort++)
		{ // 入れ替える値の総数 -nCntKeep分繰り返す

			if (m_aRanking[nCurrentMinID] > m_aRanking[nCntSort])	
			{ // 最小値に設定されている値より、現在の値のほうが小さい場合

				// 現在の要素番号を最小値に設定
				nCurrentMinID = nCntSort;
			}
		}

		if (nCntKeep != nCurrentMinID)
		{ // 最小値の要素番号に変動があった場合

			// 要素の入れ替え
			nKeepNum					= m_aRanking[nCntKeep];
			m_aRanking[nCntKeep]		= m_aRanking[nCurrentMinID];
			m_aRanking[nCurrentMinID]	= nKeepNum;
		}
	}
}

//============================================================
//	スコア変動インデックスの設定処理
//============================================================
void CRankingManager::SetNewRank(const long nValue)
{
	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{ // ランキングの上位表示数分繰り返す

		if (m_aRanking[nCntRank] == nValue)
		{ // 入れ替わった値と一致した場合

			// 一致した値を設定
			m_nNewRankID = nCntRank;
		}
	}
}

//============================================================
//	保存処理
//============================================================
void CRankingManager::Save(void)
{
	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// バイナリファイルを書き出し方式で開く
	pFile = fopen(RANKING_BIN, "wb");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		// ファイルに数値を書き出す
		fwrite(&m_aRanking[0], sizeof(long), NUM_RANKING, pFile);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ランキングファイルの書き出しに失敗！", "警告！", MB_ICONWARNING);
	}
}

//============================================================
//	読込処理
//============================================================
void CRankingManager::Load(void)
{
	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// バイナリファイルを読み込み方式で開く
	pFile = fopen(RANKING_BIN, "rb");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		// ファイルの数値を読み込む
		fread(&m_aRanking[0], sizeof(long), NUM_RANKING, pFile);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ランキングファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// バイナリファイルを書き出し方式で開く
		pFile = fopen(RANKING_BIN, "wb");

		if (pFile != NULL)
		{ // ファイルが開けた場合

			// 変数配列を宣言
			long nMaxTime = CTimerManager::GetMaxTime();	// 最大タイム

			for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
			{ // ランキングの上位表示数分繰り返す

				// ランキング情報をクリア
				m_aRanking[nCntRank] = nMaxTime;
			}

			// ファイルに数値を書き出す
			fwrite(&m_aRanking[0], sizeof(long), NUM_RANKING, pFile);

			// ファイルを閉じる
			fclose(pFile);
		}
		else
		{ // ファイルが開けなかった場合

			// エラーメッセージボックス
			MessageBox(NULL, "ランキングファイルの書き出しに失敗！", "警告！", MB_ICONWARNING);
		}
	}
}
