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
	const D3DXVECTOR3	DESTPOS						= D3DXVECTOR3(150.0f, 50.0f, 0.0f);		// 位置
	const float			DISTANCE[NUMBER_MAX]		= { 440.0f,100.0f};						// 間隔
	const D3DXVECTOR3	INIT_SIZE					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズの初期化
	const float			DESTSIZE[NUMBER_MAX]		= { 500.0f,100.0f};						// 目的の位置
	const int			MAX_WAIT					= 150;									// 待機時間の最大値
	const float			VALUE_INERTIA[NUMBER_MAX]	= {0.015f,0.05f};						// 慣性の値
}
//巨大フレーム関連
namespace BigFrame
{
	//定数の定義
	const D3DXVECTOR3	POS				= D3DXVECTOR3(230.0f, 400.0f, 0.0f);			// 位置
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// サイズの初期化
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(400.0f, 640.0f, 0.0f);			// 目的の位置
	const int			MAX_WAIT		= 100;											// 待機時間の最大値
	const float			VALUE_INERTIA	= 0.015f;										// 慣性の値

}
//フレーム関連
namespace Frame
{
	//定数の定義
	const D3DXVECTOR3	POS				= D3DXVECTOR3(860.0f, 90.0f, 0.0f);				// 位置
	const float			DISTANCE		= 170.0f;										// 間隔
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f,0.0f, 0.0f);					// サイズの初期化
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(700.0f, 180.0f, 0.0f);			// 目的の位置
	const int			NUM				= 3;											// 表示させる数
	const int			MAX_WAIT		= 25;											// 待機時間の最大値
	const float			VALUE_INERTIA	= 0.025f;										// 慣性の値
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CResultManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\Who.png",			// 誰が
	"data\\TEXTURE\\Win.png",			// 勝った
	"data\\TEXTURE\\entry_flame.png",	// フレーム
	"data\\TEXTURE\\continue001.png",	// 再戦
	"data\\TEXTURE\\continue002.png",	// 戻る
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
	m_state			= STATE_FADEIN;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nSelect		= SELECT_YES;	// 現在の選択
	m_nOldSelect	= SELECT_YES;	// 前回の選択

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
	m_pBigFrame->BindTexture(mc_apTextureFile[TEXTURE_FRAME]);

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
			D3DXVECTOR3(450.0f + 300.0f * nCnt,650.0f, 0.0f),
			D3DXVECTOR3(150.0f, 150.0f, 0.0f)
		);

		if (m_apSelect[nCnt] == NULL)
		{ // 生成に失敗した場合

		  // 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apSelect[nCnt]->SetPriority(RESULT_PRIO);

		m_apSelect[nCnt]->SetEnableDraw(false);
	}


	// テクスチャを設定
	m_apSelect[SELECT_YES]->BindTexture(mc_apTextureFile[TEXTURE_RESTART]);
	m_apSelect[SELECT_NO]->BindTexture(mc_apTextureFile[TEXTURE_BACK]);

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

		//勝利ロゴ
	case STATE_WIN:

		//勝利ロゴの数だけ回す
		for (int nCnt = 0; nCnt < NUM_WIN; nCnt++)
		{
			//中身チェック
			if (m_apWinLog[nCnt] != nullptr)
			{
				m_apWinLog[nCnt]->Update();

				//目的の位置に到着するまで加算し続ける
				m_arSize[EObj::OBJ_WIN].x += (Win::DESTSIZE[Win::NUMBER_ZERO] + m_arSize[EObj::OBJ_WIN].x) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

				//そのサイズを超えそうになったら
				if (m_arSize[EObj::OBJ_WIN].x >= Win::DESTSIZE[Win::NUMBER_ZERO])
				{
					//値を固定する
					m_arSize[EObj::OBJ_WIN].x = Win::DESTSIZE[Win::NUMBER_ZERO];
				}

				//目的の位置に到着するまで加算し続ける

				m_arSize[EObj::OBJ_WIN].y += (Win::DESTSIZE[Win::NUMBER_ZERO] + m_arSize[EObj::OBJ_WIN].y) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

				//そのサイズを超えそうになったら
				if (m_arSize[EObj::OBJ_WIN].y >= Win::DESTSIZE[Win::NUMBER_ZERO])
				{//値を固定する
					m_arSize[EObj::OBJ_WIN].y = Win::DESTSIZE[Win::NUMBER_ZERO];
				}

				//サイズの設定を行う
				m_apWinLog[nCnt]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN]);

				//どちらも目的の位置に到着していたら
				if (m_arSize[EObj::OBJ_WIN].x == Win::DESTSIZE[Win::NUMBER_ZERO] &&
					m_arSize[EObj::OBJ_WIN].y == Win::DESTSIZE[Win::NUMBER_ZERO])
				{
					m_anWaitTime[EObj::OBJ_WIN]++;
				}

				if (m_anWaitTime[EObj::OBJ_WIN] >= Win::MAX_WAIT)
				{
					m_anWaitTime[EObj::OBJ_WIN] = Win::MAX_WAIT;

					//目的の位置に到着するまで加算し続ける
					m_arPos[EObj::OBJ_WIN].x -= (Win::DESTPOS.x - m_arPos[EObj::OBJ_WIN].x) * Win::VALUE_INERTIA[Win::NUMBER_ONE];
					
					//そのサイズを超えそうになったら
					if (m_arPos[EObj::OBJ_WIN].x <= Win::DESTPOS.x)
					{
						//値を固定する
						m_arPos[EObj::OBJ_WIN].x = Win::DESTPOS.x;
					}

					//目的の位置に到着するまで加算し続ける

					m_arPos[EObj::OBJ_WIN].y -= (Win::DESTPOS.y + m_arPos[EObj::OBJ_WIN].y) * Win::VALUE_INERTIA[Win::NUMBER_ONE];

					//そのサイズを超えそうになったら
					if (m_arPos[EObj::OBJ_WIN].y <= Win::DESTPOS.y)
					{//値を固定する
						m_arPos[EObj::OBJ_WIN].y = Win::DESTPOS.y;
					}

					//目的の位置に到着するまで加算し続ける
					m_arSize[EObj::OBJ_WIN].x -= (Win::DESTSIZE[Win::NUMBER_ONE] + m_arSize[EObj::OBJ_WIN].x) * Win::VALUE_INERTIA[Win::NUMBER_ONE];

					//そのサイズを超えそうになったら
					if (m_arSize[EObj::OBJ_WIN].x <= Win::DESTSIZE[Win::NUMBER_ONE])
					{
						//値を固定する
						m_arSize[EObj::OBJ_WIN].x = Win::DESTSIZE[Win::NUMBER_ONE];
					}

					//目的の位置に到着するまで加算し続ける

					m_arSize[EObj::OBJ_WIN].y -= (Win::DESTSIZE[Win::NUMBER_ONE] + m_arSize[EObj::OBJ_WIN].y)* Win::VALUE_INERTIA[Win::NUMBER_ONE];

					//そのサイズを超えそうになったら
					if (m_arSize[EObj::OBJ_WIN].y <= Win::DESTSIZE[Win::NUMBER_ONE])
					{//値を固定する
						m_arSize[EObj::OBJ_WIN].y = Win::DESTSIZE[Win::NUMBER_ONE];
					}

					//サイズの設定を行う
					m_apWinLog[nCnt]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN]);
					m_apWinLog[nCnt]->SetVec3Position(D3DXVECTOR3(
						m_arPos[EObj::OBJ_WIN].x+ Win::DISTANCE[Win::NUMBER_ONE] * nCnt, 
						m_arPos[EObj::OBJ_WIN].y,
						m_arPos[EObj::OBJ_WIN].z));

					//どちらも目的の位置に到着していたら
					if (m_arSize[EObj::OBJ_WIN].x == Win::DESTSIZE[Win::NUMBER_ONE]&&
						m_arSize[EObj::OBJ_WIN].y == Win::DESTSIZE[Win::NUMBER_ONE]&&
						m_arPos[EObj::OBJ_WIN].x == Win::DESTPOS.x&&
						m_arPos[EObj::OBJ_WIN].y == Win::DESTPOS.y)
					{
						m_anWaitTime[EObj::OBJ_WIN] = 0;
						m_state = STATE_BIG_FRAME;
					}
				}
			}

		}

		break;

		//巨大フレーム
	case STATE_BIG_FRAME:

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

		break;

		//フレーム
	case STATE_FRAME:

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
			if (m_arSize[EObj::OBJ_FRAME].y>= Frame::DESTSIZE.y)
			{
				//値を固定する
				m_arSize[EObj::OBJ_FRAME].y= Frame::DESTSIZE.y;
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
				//遷移待機状態に移行する
				m_state = STATE_WAIT;
			}
		}

		break;

	case STATE_WAIT:	// 遷移待機状態

		for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
		{
			if (m_apSelect[nCnt] != nullptr)
			{
				m_apSelect[nCnt]->SetEnableDraw(true);
				m_apSelect[nCnt]->Update();
			}

		}

		//選択の更新
		UpdateSelect();

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 遷移決定の更新
	UpdateTransition();

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
	m_apSelect[m_nOldSelect]->SetColor(DEFAULT_COL);

	// 現在の選択要素の色を白に設定
	m_apSelect[m_nSelect]->SetColor(CHOICE_COL);

	// 現在の選択要素を代入
	m_nOldSelect = m_nSelect;

#endif
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
			/*SkipStaging();*/

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
	}
}

//============================================================
//	演出スキップ処理
//============================================================
void CResultManager::SkipStaging(void)
{
	// フェードの透明度を設定
	m_pFade->SetColor(Fade::SETCOL_FADE);

	// 状態を変更
	m_state = STATE_WAIT;	// 遷移待機状態
}
