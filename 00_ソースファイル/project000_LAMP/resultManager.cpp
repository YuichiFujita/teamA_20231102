//============================================================
//
//	リザルトマネージャー処理 [resultManager.cpp]
//	Author：kazuki watanabe
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
#include "anim2D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define CHOICE_COL	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// 選択中カラー
#define DEFAULT_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// 非選択中カラー

//***********************************************************
//ネームスペースの定義
//***********************************************************
//フェード関連
namespace Fade
{
	const D3DXCOLOR SETCOL_FADE		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));	// α値の停止値
	const D3DXVECTOR3 SIZE_FADE		(SCREEN_SIZE * 0.95f);					// フェードの大きさ
	const D3DXCOLOR INITCOL_FADE	(XCOL_AWHITE);							// α値の初期値
	const float ADD_ALPHA			(0.008f);								// α値の加算量

}
//勝利ロゴ関連
namespace Win
{
	//番号の列挙型
	enum Number
	{
		NUMBER_ZERO = 0,	//０番目
		NUMBER_ONE,			//１番目
		NUMBER_MAX
	};

	//定数の定義
	const D3DXVECTOR3	POS							= D3DXVECTOR3(425.0f, 350.0f, 0.0f);	// 位置
	const D3DXVECTOR3	DESTPOS						= D3DXVECTOR3(240.0f, 60.0f, 0.0f);		// 位置
	const float			DISTANCE[NUMBER_MAX]		= { 440.0f,100.0f};						// 間隔
	const D3DXVECTOR3	INIT_SIZE					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズの初期化
	const D3DXVECTOR3	DESTSIZE_WIN				= D3DXVECTOR3(390.0f, 85.0f, 0.0f);						// 目的の位置
	const D3DXVECTOR3	DESTSIZE_NUMBER				= D3DXVECTOR3(120.0f, 75.0f, 0.0f);						// 目的の位置
	const int			MAX_WAIT					= 10;									// 待機時間の最大値
	const float			VALUE_INERTIA[NUMBER_MAX]	= {0.015f,0.05f};						// 慣性の値
}
//巨大フレーム関連
namespace BigFrame
{
	//定数の定義
	const D3DXVECTOR3	POS				= D3DXVECTOR3(250.0f, 400.0f, 0.0f);			// 位置
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// サイズの初期化
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(413.0f, 594.0f, 0.0f);			// 目的の位置
	const int			MAX_WAIT		= 10;											// 待機時間の最大値
	const float			VALUE_INERTIA	= 0.015f;										// 慣性の値

}
//フレーム関連
namespace Frame
{
	//定数の定義
	const D3DXVECTOR3	POS				= D3DXVECTOR3(860.0f, 165.0f, 0.0f);				// 位置
	const float			DISTANCE		= 145.0f;										// 間隔
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f,0.0f, 0.0f);					// サイズの初期化
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(630.0f, 150.0f, 0.0f);			// 目的の位置
	const D3DXVECTOR3	NUMBERSIZE		= D3DXVECTOR3(630.0f, 150.0f, 0.0f);			// 目的の位置
	const int			NUM				= 3;											// 表示させる数
	const int			MAX_WAIT		= 5;											// 待機時間の最大値
	const float			VALUE_INERTIA	= 0.025f;										// 慣性の値
}
//カバー関連
namespace Cover
{
	//定数の定義
	const D3DXVECTOR3	POS				= D3DXVECTOR3(600.0f, 370.0f, 0.0f);			// 位置
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// サイズの初期化
	const D3DXCOLOR		INIT_COL		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 色の初期化
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(700.0f, 450.0f, 0.0f);			// 目的の位置
	const float			VALUE_INERTIA	= 0.06f;										// 慣性の値

}
//選択肢関連
namespace Select
{
	const D3DXVECTOR3	POS			= D3DXVECTOR3(500.0f, 400.0f, 0.0f);			// 位置
	const float			DISTANCE	= 200.0f;										// 間隔
	const int			PRIORITY	= 15;											// 優先順位
	const float			INIT_SIZE	= 120.0f;										// サイズの初期化
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CResultManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\Winner_.png",			// 勝った
	"data\\TEXTURE\\PlayerCount.png",			// 誰が
	"data\\TEXTURE\\Winner_flame.png",	// 勝者フレーム
	"data\\TEXTURE\\ranking_flame.png",	// フレーム
	"data\\TEXTURE\\YES.png",	// 再戦
	"data\\TEXTURE\\NO.png",	// 戻る
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
	m_pBigFrame		= nullptr;		// 巨大フレーム
	m_pFade			= NULL;			// フェードの情報
	m_state			= STATE_NONE;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nSelect		= SELECT_YES;	// 現在の選択
	m_nOldSelect	= SELECT_YES;	// 前回の選択
	m_bSkiped = false;
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
	//オブジェクトの数分初期化を行う
	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{
		m_arPos[nCnt]	= (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//位置
		m_arSize[nCnt]	= (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//サイズ
		m_anNum[nCnt] = 0;									// 現在の番号
		m_anWaitTime[nCnt] = 0;								// 待機時間
	}

	// メンバ変数を初期化
	memset(&m_apWinLog[0], 0, sizeof(&m_apWinLog));
	memset(&m_apFrame[0], 0, sizeof(&m_apFrame));
	memset(&m_apSelect[0], 0, sizeof(&m_apSelect));
	m_pBigFrame		= nullptr;		//巨大フレーム		
	m_pFade			= NULL;			// フェードの情報
	m_pCover		= NULL;			// フェードの情報
	m_state			= STATE_FADEIN;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nSelect		= SELECT_YES;	// 現在の選択
	m_nOldSelect	= SELECT_YES;	// 前回の選択
	m_bSkiped = false;

	//--------------------------------------------------------
	//	フェードの生成・設定
	//--------------------------------------------------------
	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		Fade::SIZE_FADE,		// 大きさ
		VEC3_ZERO,		// 向き
		Fade::INITCOL_FADE	// 色
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
	//	巨大フレーム生成・設定
	//--------------------------------------------------------
	m_pBigFrame = CObject2D::Create
	(
		D3DXVECTOR3(BigFrame::POS.x, BigFrame::POS.y, BigFrame::POS.z),
		Win::INIT_SIZE
	);

	// テクスチャを設定
	m_pBigFrame->BindTexture(mc_apTextureFile[TEXTURE_WINNERFRAME]);

	// 優先順位を設定
	m_pBigFrame->SetPriority(RESULT_PRIO);

	//--------------------------------------------------------
	//	勝利ロゴ生成・設定
	//--------------------------------------------------------
	for (int nCnt = 0; nCnt < NUM_WIN; nCnt++)
	{
		m_apWinLog[nCnt] = CObject2D::Create
		(
			D3DXVECTOR3(Win::POS.x + Win::DISTANCE[Win::NUMBER_ZERO] * nCnt , Win::POS.y, Win::POS.z),
			Win::INIT_SIZE
		);
		
		if (m_apWinLog[nCnt] == NULL)
		{ // 生成に失敗した場合

		  // 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを設定
		m_apWinLog[nCnt]->BindTexture(mc_apTextureFile[nCnt]);

		// 優先順位を設定
		m_apWinLog[nCnt]->SetPriority(RESULT_PRIO);
	}

	//--------------------------------------------------------
	//	フレーム生成・設定
	//--------------------------------------------------------
	for (int nCnt = 0; nCnt < NUM_FRAME; nCnt++)
	{
		m_apNumber[nCnt] = CAnim2D::Create(10, 1, D3DXVECTOR3(Frame::POS.x - 220.0f, Frame::POS.y + Frame::DISTANCE * nCnt, Frame::POS.z), D3DXVECTOR3(128.0f, 128.0f, 0.0f));
		m_apNumber[nCnt]->BindTexture("data\\TEXTURE\\number002.png");
		m_apNumber[nCnt]->SetPattern(nCnt + 1);
		m_apNumber[nCnt]->SetPriority(RESULT_PRIO);
		m_apFrame[nCnt] = CObject2D::Create
		(
			D3DXVECTOR3(Frame::POS.x, Frame::POS.y + Frame::DISTANCE * nCnt, Frame::POS.z),
			Frame::INIT_SIZE
		);

		if (m_apWinLog[nCnt] == NULL)
		{ // 生成に失敗した場合

		  // 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを設定
		m_apFrame[nCnt]->BindTexture(mc_apTextureFile[TEXTURE_FRAME]);

		// 優先順位を設定
		m_apFrame[nCnt]->SetPriority(RESULT_PRIO);
	}

	//--------------------------------------------------------
	//	選択肢生成・設定
	//--------------------------------------------------------
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		m_apSelect[nCnt] = CObject2D::Create
		(
			D3DXVECTOR3(Select::POS.x + Select::DISTANCE * nCnt, Select::POS.y, Select::POS.z),
			D3DXVECTOR3(Select::INIT_SIZE, Select::INIT_SIZE, 0.0f)
		);

		if (m_apSelect[nCnt] == NULL)
		{ // 生成に失敗した場合

		  // 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apSelect[nCnt]->SetPriority(Select::PRIORITY);

		m_apSelect[nCnt]->SetEnableDraw(false);

		// 現在の選択要素の色を白に設定
		m_apSelect[nCnt]->SetColor(DEFAULT_COL);

	}


	// テクスチャを設定
	m_apSelect[SELECT_YES]->BindTexture(mc_apTextureFile[TEXTURE_RESTART]);
	m_apSelect[SELECT_NO]->BindTexture(mc_apTextureFile[TEXTURE_BACK]);

	//--------------------------------------------------------
	//	フェードの生成・設定
	//--------------------------------------------------------
	// フェードの生成
	m_pCover = CObject2D::Create
	( // 引数
		Cover::POS,			// 位置
		Cover::INIT_SIZE,		// 大きさ
		VEC3_ZERO,			// 向き
		Cover::INIT_COL	// 色
	);
	if (m_pCover == NULL)
	{ // 生成に失敗した場合

	  // 失敗を返す
		assert(false);
		return E_FAIL;
	}
	m_pCover->BindTexture("data\\TEXTURE\\Remach_flame.png");
	// 優先順位を設定
	m_pCover->SetPriority(RESULT_PRIO);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CResultManager::Uninit(void)
{
	//巨大フレームの終了処理
	if (m_pBigFrame != nullptr)
	{	
		m_pBigFrame->Uninit();
		m_pBigFrame = nullptr;
	}

	//勝利ロゴの終了処理
	for (int nCnt = 0; nCnt < NUM_WIN; nCnt++)
	{
		if (m_apWinLog[nCnt] != nullptr)
		{
			m_apWinLog[nCnt]->Uninit();
			m_apWinLog[nCnt] = nullptr;
		}
	}
	//フレームの終了処理
	for (int nCnt = 0; nCnt < NUM_FRAME; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
		
		if (m_apFrame[nCnt] != nullptr)
		{
			m_apFrame[nCnt]->Uninit();
			m_apFrame[nCnt] = nullptr;
		}

	}

	//選択肢の終了処理
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		if (m_apSelect[nCnt] != nullptr)
		{
			m_apSelect[nCnt]->Uninit();
			m_apSelect[nCnt] = nullptr;
		}
	}

	// フェードの終了
	m_pFade->Uninit();

	m_pCover->Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CResultManager::Update(void)
{
	// 遷移決定の更新
	UpdateTransition();

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない状態

		// 無し

		break;

	case STATE_FADEIN:	// フェードイン状態

		// フェードインの更新
		UpdateFade();

		break;

		//勝利ロゴ
	case STATE_WIN:

		UpdateWin();

		break;

		//巨大フレーム
	case STATE_BIG_FRAME:

		UpdateBigFrame();
		
		break;

		//フレーム
	case STATE_FRAME:

		UpdateFrame();

		break;

	case STATE_SELECT:

		UpdateCover();
		
		break;

	case STATE_HOLD:

		if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_RETURN)
			|| CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_SPACE)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_A)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_B)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_X)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_Y)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_START))
		{
			//遷移待機状態に移行する
			m_state = STATE_SELECT;
		}

		break;

	case STATE_WAIT:	// 遷移待機状態

		//選択の更新
		UpdateSelect();

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// フェードの更新
	m_pFade->Update();

	m_pCover->Update();

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
//	勝利ロゴの更新処理
//============================================================
void CResultManager::UpdateWin(void)
{
	//勝利ロゴの数だけ回す
	for (int nCnt = 0; nCnt < NUM_WIN; nCnt++)
	{
		//中身チェック
		if (m_apWinLog[nCnt] != nullptr)
		{
			m_apWinLog[nCnt]->Update();

			//目的の位置に到着するまで加算し続ける
			m_arSize[EObj::OBJ_WIN].x += (Win::DESTSIZE_WIN.x + m_arSize[EObj::OBJ_WIN].x) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

			//そのサイズを超えそうになったら
			if (m_arSize[EObj::OBJ_WIN].x >= Win::DESTSIZE_WIN.x)
			{
				//値を固定する
				m_arSize[EObj::OBJ_WIN].x = Win::DESTSIZE_WIN.x;
			}

			//目的の位置に到着するまで加算し続ける

			m_arSize[EObj::OBJ_WIN].y += (Win::DESTSIZE_WIN.y + m_arSize[EObj::OBJ_WIN].y) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

			//そのサイズを超えそうになったら
			if (m_arSize[EObj::OBJ_WIN].y >= Win::DESTSIZE_WIN.y)
			{//値を固定する
				m_arSize[EObj::OBJ_WIN].y = Win::DESTSIZE_WIN.y;
			}

			//サイズの設定を行う
			m_apWinLog[nCnt]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN]);

			//どちらも目的の位置に到着していたら
			if (m_arSize[EObj::OBJ_WIN].x == Win::DESTSIZE_WIN.x &&
				m_arSize[EObj::OBJ_WIN].y == Win::DESTSIZE_WIN.y)
			{
				m_anWaitTime[EObj::OBJ_WIN]++;

				if (m_arPos[EObj::OBJ_WIN].x == 0.0f&&
					m_arPos[EObj::OBJ_WIN].y == 0.0f&&
					m_arPos[EObj::OBJ_WIN].z == 0.0f)
				{
					m_arPos[EObj::OBJ_WIN].x = Win::POS.x;
					m_arPos[EObj::OBJ_WIN].y = Win::POS.y;
					m_arPos[EObj::OBJ_WIN].z = Win::POS.z;
				}
			}

			if (m_anWaitTime[EObj::OBJ_WIN] >= Win::MAX_WAIT)
			{
				m_anWaitTime[EObj::OBJ_WIN] = Win::MAX_WAIT;

				//目的の位置に到着するまで加算し続ける
				m_arPos[EObj::OBJ_WIN].x -= (Win::DESTPOS.x + m_arPos[EObj::OBJ_WIN].x) * 0.05f;

				//そのサイズを超えそうになったら
				if (m_arPos[EObj::OBJ_WIN].x <= Win::DESTPOS.x)
				{
					//値を固定する
					m_arPos[EObj::OBJ_WIN].x = Win::DESTPOS.x;
				}

				//目的の位置に到着するまで加算し続ける
				m_arPos[EObj::OBJ_WIN].y -= (Win::DESTPOS.y + m_arPos[EObj::OBJ_WIN].y) * 0.05f;

				//そのサイズを超えそうになったら
				if (m_arPos[EObj::OBJ_WIN].y <= Win::DESTPOS.y)
				{
					m_arPos[EObj::OBJ_WIN].y = Win::DESTPOS.y;
				}

				//目的の位置に到着するまで加算し続ける
				m_arSize[EObj::OBJ_WIN].x -= (Win::DESTSIZE_WIN.x + m_arSize[EObj::OBJ_WIN].x) * Win::VALUE_INERTIA[Win::NUMBER_ONE];

				//そのサイズを超えそうになったら
				if (m_arSize[EObj::OBJ_WIN].x <= Win::DESTSIZE_WIN.x)
				{
					//値を固定する
					m_arSize[EObj::OBJ_WIN].x = Win::DESTSIZE_WIN.x;
				}

				//目的の位置に到着するまで加算し続ける

				m_arSize[EObj::OBJ_WIN].y -= (Win::DESTSIZE_WIN.y + m_arSize[EObj::OBJ_WIN].y)* Win::VALUE_INERTIA[Win::NUMBER_ONE];

				//そのサイズを超えそうになったら
				if (m_arSize[EObj::OBJ_WIN].y <= Win::DESTSIZE_WIN.y)
				{//値を固定する
					m_arSize[EObj::OBJ_WIN].y = Win::DESTSIZE_WIN.y;
				}

				//サイズの設定を行う
				m_apWinLog[nCnt]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN]);
				m_apWinLog[nCnt]->SetVec3Position(D3DXVECTOR3(
					m_arPos[EObj::OBJ_WIN].x + Win::DISTANCE[Win::NUMBER_ONE] * nCnt,
					m_arPos[EObj::OBJ_WIN].y,
					m_arPos[EObj::OBJ_WIN].z));

				//どちらも目的の位置に到着していたら
				if (m_arSize[EObj::OBJ_WIN].x == Win::DESTSIZE_WIN.x &&
					m_arSize[EObj::OBJ_WIN].y == Win::DESTSIZE_WIN.y &&
					m_arPos[EObj::OBJ_WIN].x == Win::DESTPOS.x&&
					m_arPos[EObj::OBJ_WIN].y == Win::DESTPOS.y)
				{
					m_anWaitTime[EObj::OBJ_WIN] = 0;
					m_state = STATE_BIG_FRAME;
				}
			}
		}

	}
}
//============================================================
//	巨大フレームの更新処理
//============================================================
void CResultManager::UpdateBigFrame(void)
{
	if (m_pBigFrame != nullptr)
	{
		m_pBigFrame->Update();

		//目的の位置に到着するまで加算し続ける
		m_arSize[EObj::OBJ_BIGFRAME].x += (BigFrame::DESTSIZE.x + m_arSize[EObj::OBJ_BIGFRAME].x) * BigFrame::VALUE_INERTIA;

		//そのサイズを超えそうになったら
		if (m_arSize[EObj::OBJ_BIGFRAME].x >= BigFrame::DESTSIZE.x)
		{
			//値を固定する
			m_arSize[EObj::OBJ_BIGFRAME].x = BigFrame::DESTSIZE.x;
		}

		//目的の位置に到着するまで加算し続ける

		m_arSize[EObj::OBJ_BIGFRAME].y += (BigFrame::DESTSIZE.y + m_arSize[EObj::OBJ_BIGFRAME].y) * BigFrame::VALUE_INERTIA;

		//そのサイズを超えそうになったら
		if (m_arSize[EObj::OBJ_BIGFRAME].y >= BigFrame::DESTSIZE.y)
		{//値を固定する
			m_arSize[EObj::OBJ_BIGFRAME].y = BigFrame::DESTSIZE.y;
		}

		//サイズの設定を行う
		m_pBigFrame->SetVec3Sizing(m_arSize[EObj::OBJ_BIGFRAME]);

		if (m_arSize[EObj::OBJ_BIGFRAME].x == BigFrame::DESTSIZE.x
			&&m_arSize[EObj::OBJ_BIGFRAME].y == BigFrame::DESTSIZE.y)
		{
			m_anWaitTime[EObj::OBJ_BIGFRAME]++;

			if (m_anWaitTime[EObj::OBJ_BIGFRAME] == BigFrame::MAX_WAIT)
			{
				m_anWaitTime[EObj::OBJ_BIGFRAME] = 0;
				m_state = STATE_FRAME;
			}
		}
	}

}
//============================================================
//	フレームの更新処理
//============================================================
void CResultManager::UpdateFrame(void)
{
	//中身チェック
	if (m_apFrame[m_anNum[EObj::OBJ_FRAME]] != nullptr)
	{
		//描画をするようにする
		m_apFrame[m_anNum[EObj::OBJ_FRAME]]->SetEnableDraw(true);
		m_apFrame[m_anNum[EObj::OBJ_FRAME]]->Update();

		//目的の位置に到着するまで加算し続ける
		m_arSize[EObj::OBJ_FRAME].x += (Frame::DESTSIZE.x + m_arSize[EObj::OBJ_FRAME].x) * Frame::VALUE_INERTIA;

		//そのサイズを超えそうになったら
		if (m_arSize[EObj::OBJ_FRAME].x >= Frame::DESTSIZE.x)
		{
			//値を固定する
			m_arSize[EObj::OBJ_FRAME].x = Frame::DESTSIZE.x;
		}

		//目的の位置に到着するまで加算し続ける
		m_arSize[EObj::OBJ_FRAME].y += (Frame::DESTSIZE.y + m_arSize[EObj::OBJ_FRAME].y) * Frame::VALUE_INERTIA;

		//そのサイズを超えそうになったら
		if (m_arSize[EObj::OBJ_FRAME].y >= Frame::DESTSIZE.y)
		{
			//値を固定する
			m_arSize[EObj::OBJ_FRAME].y = Frame::DESTSIZE.y;
		}

		//サイズを設定する
		m_apFrame[m_anNum[EObj::OBJ_FRAME]]->SetVec3Sizing(m_arSize[EObj::OBJ_FRAME]);

		//もしフレームの拡大率がその値になっていたら
		if (m_apFrame[m_anNum[EObj::OBJ_FRAME]]->GetVec3Sizing().x == Frame::DESTSIZE.x
			&&m_apFrame[m_anNum[EObj::OBJ_FRAME]]->GetVec3Sizing().y == Frame::DESTSIZE.y)
		{
			//範囲外にいかないようにする
			if (m_anNum[EObj::OBJ_FRAME] >= Frame::NUM)
			{
				m_anNum[EObj::OBJ_FRAME] = Frame::NUM;
			}
			//範囲外にいっていなければ
			else if (!(m_anNum[EObj::OBJ_FRAME] >= Frame::NUM))
			{
				//待機時間を加算する
				m_anWaitTime[EObj::OBJ_FRAME]++;

				//待機時間の最大値を超えそうになったら
				if (m_anWaitTime[EObj::OBJ_FRAME] >= Frame::MAX_WAIT)
				{
					//値のリセットを行う
					m_anWaitTime[EObj::OBJ_FRAME] = 0;
					m_anNum[EObj::OBJ_FRAME]++;
					m_arSize[EObj::OBJ_FRAME].x = Frame::INIT_SIZE.x;
					m_arSize[EObj::OBJ_FRAME].y = Frame::INIT_SIZE.y;
				}
			}
		}
		//番号・Xサイズ・Yサイズがその値になっていたら
		if (m_anNum[EObj::OBJ_FRAME] == Frame::NUM &&
			m_apFrame[m_anNum[EObj::OBJ_FRAME]]->GetVec3Sizing().x == Frame::DESTSIZE.x&&
			m_apFrame[m_anNum[EObj::OBJ_FRAME]]->GetVec3Sizing().y == Frame::DESTSIZE.y)
		{

			m_state = STATE_HOLD;
		}
	}

}
//============================================================
//	フェードインの更新処理
//============================================================
void CResultManager::UpdateFade(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	if (colFade.a < Fade::SETCOL_FADE.a)
	{ // 透明量が設定値未満の場合

		// 透明度を加算
		colFade.a += Fade::ADD_ALPHA;
	}
	else
	{ // 透明量が設定値以上の場合

		// 透明度を補正
		colFade.a = Fade::SETCOL_FADE.a;

		// 状態を変更
		m_state = STATE_WIN;	// 待機状態
	}

	// 透明度を反映
	m_pFade->SetColor(colFade);
}

//============================================================
//	選択の更新処理
//============================================================
void CResultManager::UpdateSelect(void)
{
#if 1

	m_nOldSelect = m_nSelect;

	if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_A)
	||  CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_LEFT)
	|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_LEFT))
	{ // 左移動の操作が行われた場合

		// 左に選択をずらす
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
	}
	if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_D)
	||  CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_RIGHT)
	|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_RIGHT))
	{ // 右移動の操作が行われた場合

		// 右に選択をずらす
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
	}

	// 現在の選択要素の色を白に設定
	m_apSelect[m_nOldSelect]->SetColor(DEFAULT_COL);

	// 現在の選択要素の色を白に設定
	m_apSelect[m_nSelect]->SetColor(CHOICE_COL);

#endif
}
//============================================================
//	カバーの更新処理
//============================================================
void CResultManager::UpdateCover(void)
{
	//中身チェック
	if (m_pCover != nullptr)
	{
		m_pCover->Update();

		//目的の位置に到着するまで加算し続ける
		m_arSize[EObj::OBJ_COVER].x += (Cover::DESTSIZE.x + m_arSize[EObj::OBJ_COVER].x) * Cover::VALUE_INERTIA;

		//そのサイズを超えそうになったら
		if (m_arSize[EObj::OBJ_COVER].x >= Cover::DESTSIZE.x)
		{
			//値を固定する
			m_arSize[EObj::OBJ_COVER].x = Cover::DESTSIZE.x;
		}

		//目的の位置に到着するまで加算し続ける

		m_arSize[EObj::OBJ_COVER].y += (Cover::DESTSIZE.y + m_arSize[EObj::OBJ_COVER].y) * Cover::VALUE_INERTIA;

		//そのサイズを超えそうになったら
		if (m_arSize[EObj::OBJ_COVER].y >= Cover::DESTSIZE.y)
		{//値を固定する
			m_arSize[EObj::OBJ_COVER].y = Cover::DESTSIZE.y;
		}

		//サイズの設定を行う
		m_pCover->SetVec3Sizing(m_arSize[EObj::OBJ_COVER]);

		//どちらも目的の位置に到着していたら
		if (m_arSize[EObj::OBJ_COVER].x == Cover::DESTSIZE.x &&
			m_arSize[EObj::OBJ_COVER].y == Cover::DESTSIZE.y)
		{
			//セレクトの数だけ回す
			for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
			{
				if (m_apSelect[nCnt] != nullptr)
				{
					m_apSelect[nCnt]->SetEnableDraw(true);
					m_apSelect[nCnt]->Update();
				}
			}
			m_bSkiped = false;
			m_state = STATE_WAIT;
		}

	}
}
//============================================================
//	遷移決定の更新処理
//============================================================
void CResultManager::UpdateTransition(void)
{
	if (m_bSkiped == false)
	{
		if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_RETURN)
			|| CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_SPACE)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_A)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_B)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_X)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_Y)
			|| CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_START))
		{
			if (m_state != STATE_WAIT)
			{ // 遷移待機状態ではない場合

			  //フレームステートではなければ
				if (m_state != STATE_FRAME)
				{
					//演出スキップ
					SkipStaging();
				}

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
						CManager::GetInstance()->SetScene(CScene::MODE_ENTRY);	// ゲーム画面

						break;

					case SELECT_NO:

						// シーンの設定
						CManager::GetInstance()->SetScene(CScene::MODE_TITLE);	// ランキング画面

						break;
					}

					// サウンドの再生
					CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// 決定音00
				}
			}
			m_bSkiped = true;
		}
	}
}

//============================================================
//	演出スキップ処理
//============================================================
void CResultManager::SkipStaging(void)
{
	// フェードの透明度を設定
	m_pFade->SetColor(Fade::SETCOL_FADE);

	//勝利ロゴの数だけ回す
	for (int nCnt = 0; nCnt < NUM_WIN; nCnt++)
	{
		//中身チェック
		if (m_apWinLog[nCnt] != nullptr)
		{
			//サイズ・位置の設定
			m_arSize[EObj::OBJ_WIN].x = Win::DESTSIZE_WIN.x;
			m_arSize[EObj::OBJ_WIN].y = Win::DESTSIZE_WIN.y;
			m_arPos[EObj::OBJ_WIN].x = Win::DESTPOS.x;
			m_arPos[EObj::OBJ_WIN].y = Win::DESTPOS.y;

			//サイズ・位置のセットを行う
			m_apWinLog[nCnt]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN]);
			m_apWinLog[nCnt]->SetVec3Position(D3DXVECTOR3(
				m_arPos[EObj::OBJ_WIN].x + Win::DISTANCE[Win::NUMBER_ONE] * nCnt,
				m_arPos[EObj::OBJ_WIN].y,
				m_arPos[EObj::OBJ_WIN].z));

			m_state = STATE_BIG_FRAME;
		}

	}

	//中身チェック
	if (m_pBigFrame != nullptr)
	{
		//サイズの設定
		m_arSize[EObj::OBJ_BIGFRAME].x = BigFrame::DESTSIZE.x;
		m_arSize[EObj::OBJ_BIGFRAME].y = BigFrame::DESTSIZE.y;

		//サイズのセットを行う
		m_pBigFrame->SetVec3Sizing(m_arSize[EObj::OBJ_BIGFRAME]);

		m_state = STATE_FRAME;

	}

	//フレーム数分回す
	for (int nCnt = 0; nCnt < NUM_FRAME; nCnt++)
	{
		//描画をするようにする
		m_apFrame[nCnt]->SetEnableDraw(true);

		//サイズの設定
		m_arSize[EObj::OBJ_FRAME].x = Frame::DESTSIZE.x;
		m_arSize[EObj::OBJ_FRAME].y = Frame::DESTSIZE.y;

		//サイズのセットを行う
		m_apFrame[nCnt]->SetVec3Sizing(m_arSize[EObj::OBJ_FRAME]);
		m_anNum[OBJ_FRAME] = Frame::NUM;
	}

	m_state = STATE_FRAME;

}
