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
#include "objectBillboard.h"
#include "timerManager.h"
#include "retentionManager.h"
#include "anim2D.h"
#include "player.h"

//************************************************************
//	マクロ定義
//************************************************************
#define CHOICE_COL	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// 選択中カラー
#define DEFAULT_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// 非選択中カラー

//***********************************************************
//	ネームスペースの定義
//***********************************************************
// フェード関連
namespace Fade
{
	const D3DXCOLOR SETCOL_FADE		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);	// α値の停止値

	const D3DXVECTOR3 SIZE_FADE		= SCREEN_SIZE;	// フェードの大きさ
	const D3DXCOLOR INITCOL_FADE	= XCOL_AWHITE;	// α値の初期値
	const float ADD_ALPHA			= 0.008f;		// α値の加算量
}

// 勝利ロゴ関連
namespace Win
{
	// 番号の列挙型
	enum Number
	{
		NUMBER_ZERO = 0,	//０番目
		NUMBER_ONE,			//１番目
		NUMBER_MAX
	};

	// 定数の定義
	const D3DXVECTOR3	POS		= D3DXVECTOR3(625.0f, 350.0f, 0.0f);	// 位置
	const D3DXVECTOR3	DESTPOS	= D3DXVECTOR3(265.0f, 60.0f, 0.0f);		// 目標位置
	const D3DXVECTOR3	INIT_SIZE = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズの初期化

	const D3DXVECTOR3	DISTANCE[NUMBER_MAX]	= { D3DXVECTOR3(195.0f, -5.0f, 0.0f),
													D3DXVECTOR3(155.0f, -5.0f, 0.0f) };	// 目的の位置

	const D3DXVECTOR3	DESTSIZE_WIN[NUMBER_MAX]	= { D3DXVECTOR3(390.0f*1.5f, 85.0f*1.5f, 0.0f),
														D3DXVECTOR3(390.0f*1.2f, 85.0f*1.2f, 0.0f) };	// 目的の大きさ

	const D3DXVECTOR3	DESTSIZE_NUMBER[NUMBER_MAX] = { D3DXVECTOR3(120.0f*1.6f, 75.0f*1.6f, 0.0f),
														D3DXVECTOR3(120.0f*1.3f, 75.0f*1.3f, 0.0f) };	// 目的の大きさ

	const int	MAX_WAIT					= 300;				// 待機時間の最大値
	const float	VALUE_INERTIA[NUMBER_MAX]	= {0.015f,0.05f};	// 慣性の値
}

// 巨大フレーム関連
namespace BigFrame
{
	//定数の定義
	const D3DXVECTOR3	POS				= D3DXVECTOR3(270.0f, 410.0f, 0.0f);	// 位置
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズの初期化
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(530.0f, 610.0f, 0.0f);	// 目的の位置
	const int			MAX_WAIT		= 10;									// 待機時間の最大値
	const float			VALUE_INERTIA	= 0.015f;								// 慣性の値
}

// 順位
namespace Number
{
	const POSGRID2		PART		= POSGRID2(4, 1);						// テクスチャ分割数
	const D3DXVECTOR3	POS			= D3DXVECTOR3(690.0f, 110.0f, 0.0f);	// 位置
	const float			DISTANCE	= 170.0f;								// 間隔
	const D3DXVECTOR3	INIT_SIZE	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズの初期化
	const D3DXVECTOR3	DESTSIZE	= D3DXVECTOR3(406.0f*0.5f, 240.0f*0.5f, 0.0f);	// 目的の位置

	const int			NUM = 3;				// 表示させる数
	const int			MAX_WAIT = 5;			// 待機時間の最大値
	const float			VALUE_INERTIA = 0.09f;	// 慣性の値

}

// フレーム関連
namespace Frame
{
	// 定数の定義
	const D3DXVECTOR3	POS				= D3DXVECTOR3(920.0f, 110.0f, 0.0f);	// 位置
	const float			CENT_POSY		= 445.0f;								// 中心位置Y
	const float			DISTANCE		= 170.0f;								// 間隔
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f,0.0f, 0.0f);			// サイズの初期化
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(700.0f, 160.0f, 0.0f);	// 目的の位置

	const int			NUM				= 3;		// 表示させる数
	const int			MAX_WAIT		= 5;		// 待機時間の最大値
	const float			VALUE_INERTIA	= 0.025f;	// 慣性の値
}

// リマッチ背景関連
namespace Cover
{
	// 定数の定義
	const D3DXVECTOR3	POS				= SCREEN_CENT;							// 位置
	const D3DXVECTOR3	INIT_SIZE		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズの初期化
	const D3DXCOLOR		INIT_COL		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色の初期化
	const D3DXVECTOR3	DESTSIZE		= D3DXVECTOR3(700.0f, 450.0f, 0.0f);	// 目的の位置
	const float			VALUE_INERTIA	= 0.06f;								// 慣性の値

}

// 選択肢関連
namespace Select
{
	const D3DXVECTOR3	POS			= D3DXVECTOR3(535.0f, 400.0f, 0.0f);	// 位置
	const float			DISTANCE	= 230.0f;								// 間隔

	const D3DXVECTOR3	INIT_SIZE[CResultManager::SELECT_MAX] = { D3DXVECTOR3(125.0f*1.3f, 54.0f*1.3f, 0.0f),
																  D3DXVECTOR3(97.0f *1.3f, 54.0f*1.3f, 0.0f) };	// 大きさ
}

// プレイヤーアイコン
namespace Icon
{

	// 定数の定義
	const D3DXVECTOR3	POS = D3DXVECTOR3(850.0f, 110.0f, 0.0f);		// 位置
	const float			DISTANCE = 170.0f;								// 間隔
	const D3DXVECTOR3	INIT_SIZE = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズの初期化
	const D3DXVECTOR3	DESTSIZE = D3DXVECTOR3(125.0f, 125.0f, 0.0f);	// 目的の位置

	const int	NUM = 3;				// 表示させる数
	const int	MAX_WAIT = 5;			// 待機時間の最大値
	const float	VALUE_INERTIA = 0.025f;

}

// プレイヤー番号
namespace Winner
{
	// 定数の定義
	const POSGRID2		PART = POSGRID2(10, 1);							// テクスチャ分割数
	const D3DXVECTOR3	POS = D3DXVECTOR3(1200.0f, 110.0f, 0.0f);		// 位置
	const float			DISTANCE = 170.0f;								// 間隔
	const D3DXVECTOR3	DESTSIZE = D3DXVECTOR3(125.0f, 125.0f, 0.0f);	// 目的の位置
}

// プレイヤータイトル
namespace Player
{
	// 定数の定義
	const D3DXVECTOR3	POS = D3DXVECTOR3(1070.0f, 110.0f, 0.0f);		// 位置
	const float			DISTANCE = 170.0f;								// 間隔
	const D3DXVECTOR3	DESTSIZE = D3DXVECTOR3(300.0f, 125.0f, 0.0f);	// 目的の位置
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CResultManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\Winner_.png",				// 勝った
	"data\\TEXTURE\\PlayerCount.png",			// 誰が
	"data\\TEXTURE\\entry_flame.png",			// 勝者フレーム
	"data\\TEXTURE\\ranking_flame.png",			// フレーム
	"data\\TEXTURE\\YES.png",					// 再戦
	"data\\TEXTURE\\NO.png",					// 戻る
	"data\\TEXTURE\\ICON_PLAYER000.png",
	"data\\TEXTURE\\ICON_PLAYER001.png",
	"data\\TEXTURE\\ICON_PLAYER002.png",
	"data\\TEXTURE\\ICON_PLAYER003.png",
	"data\\TEXTURE\\Black-Frame.png",
	"data\\TEXTURE\\entry_player.png",
	"data\\TEXTURE\\number000.png",
	"data\\TEXTURE\\ranking001.png",
	"data\\TEXTURE\\Remach_flame.png",
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
	//オブジェクトの数分初期化を行う
	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{
		m_arPos[nCnt] = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 位置
		m_arSize[nCnt] = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// サイズ
		m_anNum[nCnt] = 0;									// 現在の番号
		m_anWaitTime[nCnt] = 0;								// 待機時間
		m_abool[nCnt] = false;								// 判定用
		m_abSizeFinish[nCnt] = false;						// サイズ変更判定用

		for (int nCntFrame = 0; nCntFrame < NUM_FRAME; nCntFrame++)
		{
			//最初の位置
			m_arOriginPos[nCnt][nCntFrame] = VEC3_ZERO;
		}
	}
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
	//<************************************************
	// オブジェクトの数分初期化を行う
	//<************************************************
	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{
		m_arPos[nCnt]	= VEC3_ZERO;	// 位置
		m_arSize[nCnt]	= VEC3_ZERO;	// サイズ
		m_anNum[nCnt] = 0;				// 現在の番号
		m_anWaitTime[nCnt] = 0;			// 待機時間
		m_abool[nCnt] = false;			// 判定用
		m_abSizeFinish[nCnt] = false;	// サイズ変更判定用
		playerMotion = -1;

		//二次元配列の初期化
		for (int nCntFrame = 0; nCntFrame < NUM_FRAME; nCntFrame++)
		{
			//過去位置の保存
			m_arOriginPos[nCnt][nCntFrame] = VEC3_ZERO;
		}
	}

	//<************************************************
	//初期化
	//<************************************************
	for (int nCnt = 0; nCnt < NUM_FRAME; nCnt++)
	{
		m_apFrame[nCnt]		= nullptr;	//フレーム
		m_apIcon[nCnt]		= nullptr;	//プレイヤーアイコン
		m_apWinNum[nCnt]	= nullptr;	//勝利者番号
		m_anRank[nCnt]		= 0;		//ランク
		m_anSaveRank[nCnt]	= 0;		//保存用ランク
		m_anWinPoint[nCnt]	= 0;		//得点
	}

	// メンバ変数を初期化
	memset(&m_apWinLog[0], 0, sizeof(&m_apWinLog));
	memset(&m_apSelect[0], 0, sizeof(&m_apSelect));

	//<************************************************
	//単体の初期化
	//<************************************************
	m_pBigFrame		= nullptr;		// 巨大フレーム		
	m_pCover		= nullptr;		// フェードの情報
	m_state			= STATE_WIN;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nPattern		= 0;
	m_nSelect		= SELECT_YES;	// 現在の選択
	m_nOldSelect	= SELECT_YES;	// 前回の選択
	m_bSkiped		= false;
	m_nNumPlay		= CManager::GetInstance()->GetInstance()->GetRetentionManager()->GetNumPlayer();

	// 変数を宣言
	float fDis = (Frame::DISTANCE * 0.5f) * m_nNumPlay;	// 中心からの距離
	int nPattarn = 0;									//番号パターン

	// 従来の並び方にする
	m_rPos[0] = D3DXVECTOR3(Frame::POS.x,	Frame::CENT_POSY - fDis, 0.0f);
	m_rPos[1] = D3DXVECTOR3(Number::POS.x,	Frame::CENT_POSY - fDis, 0.0f);
	m_rPos[2] = D3DXVECTOR3(Icon::POS.x,	Frame::CENT_POSY - fDis, 0.0f);
	m_rPos[3] = D3DXVECTOR3(Winner::POS.x,	Frame::CENT_POSY - fDis, 0.0f);
	m_rPos[4] = D3DXVECTOR3(Player::POS.x,	Frame::CENT_POSY - fDis, 0.0f);

	//--------------------------------------------------------
	//	巨大フレーム生成・設定
	//--------------------------------------------------------
	{
		// 巨大フレームの生成
		m_pBigFrame = CObject2D::Create
		(
			D3DXVECTOR3(BigFrame::POS.x, BigFrame::POS.y, BigFrame::POS.z),
			Win::INIT_SIZE
		);

		// テクスチャを設定
		m_pBigFrame->BindTexture(mc_apTextureFile[TEXTURE_WINNERFRAME]);

		// 優先順位を設定
		m_pBigFrame->SetPriority(RESULT_PRIO);
	}

	//--------------------------------------------------------
	//	勝利ロゴ生成・設定
	//--------------------------------------------------------
	for (int nCnt = 0; nCnt < NUM_WIN; nCnt++)
	{
		//現在のカウントで処理を分ける
		switch (nCnt)
		{
			//誰がの部分
			case 0:

				m_apWinLog[nCnt] = CAnim2D::Create
				(
					1,
					1,
					D3DXVECTOR3(Win::POS.x + Win::DISTANCE[Win::NUMBER_ZERO].x * nCnt, Win::POS.y + Win::DISTANCE[Win::NUMBER_ZERO].y * nCnt, Win::POS.z),
					Win::INIT_SIZE
				);


			break;

			//プレイヤー番号
			case 1:

				m_apWinLog[nCnt] = CAnim2D::Create
				(
					1,
					MAX_PLAYER,
					D3DXVECTOR3(Win::POS.x + Win::DISTANCE[Win::NUMBER_ZERO].x * nCnt, Win::POS.y + Win::DISTANCE[Win::NUMBER_ZERO].y * nCnt, Win::POS.z),
					Win::INIT_SIZE
				);

				//ここで勝利者の番号を出す(１位のプレイヤーの番号を取得してくる)
				m_apWinLog[nCnt]->SetPattern(CManager::GetInstance()->GetRetentionManager()->GetWinRank1st());

			break;

		}

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

	//<*****************************************************
	//フレーム生成
	//<*****************************************************
	for (int nCnt = 0; nCnt < NUM_FRAME;nCnt++)
	{
		m_apFrame[nCnt] = CObject2D::Create
		(
			D3DXVECTOR3(m_rPos[0].x, m_rPos[0].y + Frame::DISTANCE * nCnt, m_rPos[0].z),
			Frame::INIT_SIZE
		);

		if (m_apFrame[nCnt] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// プレイ人数の上限に達していなければ
		if (nCnt < m_nNumPlay)
		{
			// テクスチャを設定
			m_apFrame[nCnt]->BindTexture(mc_apTextureFile[TEXTURE_FRAME]);
		}
		// 達していなければ
		else
		{
			// テクスチャを設定
			m_apFrame[nCnt]->BindTexture(mc_apTextureFile[TEXTURE_BLACKOUT]);
		}

		// 優先順位を設定
		m_apFrame[nCnt]->SetPriority(RESULT_PRIO);
	}

	//--------------------------------------------------------
	//	順位生成・設定
	//--------------------------------------------------------
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{
		m_apNumber[nCnt] = CAnim2D::Create
		(
			Number::PART.x, 
			Number::PART.y, 
			D3DXVECTOR3(m_rPos[1].x, m_rPos[1].y + Number::DISTANCE * nCnt, m_rPos[1].z),
			Number::INIT_SIZE
		);

		m_apNumber[nCnt]->BindTexture(mc_apTextureFile[TEXTURE_RANK]);

		m_apNumber[nCnt]->SetPriority(RESULT_PRIO);

		m_anWinPoint[nCnt] = CManager::GetInstance()->GetRetentionManager()->GetPlayerWin(nCnt);

	}

	//
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{ // 参加プレイヤー数分繰り返す

		// プレイヤーキャラアイコンの生成
		m_apIcon[nCnt] = CObject2D::Create
		(
			D3DXVECTOR3(m_rPos[2].x, m_rPos[2].y + Icon::DISTANCE * nCnt, m_rPos[2].z),
			Icon::DESTSIZE
		);

		// プレイヤーキャラアイコンの位置を保存
		m_arOriginPos[EObj::OBJ_ICON][nCnt] = m_apIcon[nCnt]->GetVec3Position();

		// プレイヤー番号の生成
		m_apWinNum[nCnt] = CAnim2D::Create
		(
			Winner::PART.x,
			Winner::PART.y,
			D3DXVECTOR3(m_rPos[3].x, m_rPos[3].y + Winner::DISTANCE * nCnt, m_rPos[3].z),
			Winner::DESTSIZE
		);

		// プレイヤー番号の位置を保存
		m_arOriginPos[EObj::OBJ_PLAYER][nCnt] = m_apWinNum[nCnt]->GetVec3Position();

		if (m_apIcon[nCnt] == nullptr
		|| m_apWinNum[nCnt] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		//<******************************************************
		//ソートをするための順位保存処理
		//<******************************************************

		//１位から順に取得していく
		m_anSaveRank[nCnt] = nCnt;
		m_anRank[nCnt] = CManager::GetInstance()->GetRetentionManager()->GetWinRank(nCnt);

		// 優先順位を設定
		m_apIcon[nCnt]->SetPriority(RESULT_PRIO);
		m_apWinNum[nCnt]->SetPriority(RESULT_PRIO);

		// 自動描画をOFFにする
		m_apIcon[nCnt]->SetEnableDraw(false);
		m_apWinNum[nCnt]->SetEnableDraw(false);

		// 現在の選択要素の色を白に設定
		m_apIcon[nCnt]->SetColor(CHOICE_COL);
		m_apWinNum[nCnt]->SetColor(CHOICE_COL);
	
		// テクスチャ反映
		m_apIcon[nCnt]->BindTexture(mc_apTextureFile[TEXTURE_ICON_PLAYER1 + nCnt]);
		m_apWinNum[nCnt]->BindTexture(mc_apTextureFile[TEXTURE_WIN2]);
	}

	//--------------------------------------------------------
	//	プレイヤータイトル生成・設定
	//--------------------------------------------------------
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{ // 参加プレイヤー数分繰り返す

		// プレイヤータイトルの生成
		m_apPlayerEntry[nCnt] = CObject2D::Create
		(
			D3DXVECTOR3(m_rPos[4].x, m_rPos[4].y + Player::DISTANCE * nCnt, m_rPos[4].z),
			Player::DESTSIZE
		);

		// テクスチャを登録・割当
		m_apPlayerEntry[nCnt]->BindTexture(mc_apTextureFile[TEXTURE_PLAYER_ENTRY]);

		// 優先順位を設定
		m_apPlayerEntry[nCnt]->SetPriority(RESULT_PRIO);

		// 自動描画をOFFにする
		m_apPlayerEntry[nCnt]->SetEnableDraw(false);
	}

	//--------------------------------------------------------
	//	リマッチ選択背景の生成・設定
	//--------------------------------------------------------
	{
		// リマッチ選択背景の生成
		m_pCover = CObject2D::Create
		( // 引数
			Cover::POS,			// 位置
			Cover::INIT_SIZE,	// 大きさ
			VEC3_ZERO,			// 向き
			Cover::INIT_COL		// 色
		);
		if (m_pCover == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_pCover->BindTexture(mc_apTextureFile[TEXTURE_REMACH_BG]);

		// 優先順位を設定
		m_pCover->SetPriority(RESULT_PRIO);
	}

	//--------------------------------------------------------
	//	選択肢生成・設定
	//--------------------------------------------------------
	{
		for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
		{ // 選択肢分繰り返す

			// 選択肢の生成
			m_apSelect[nCnt] = CObject2D::Create
			(
				D3DXVECTOR3(Select::POS.x + Select::DISTANCE * nCnt, Select::POS.y, Select::POS.z),
				Select::INIT_SIZE[nCnt]
			);
			if (m_apSelect[nCnt] == NULL)
			{ // 生成に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 優先順位を設定
			m_apSelect[nCnt]->SetPriority(RESULT_PRIO);

			// 自動描画をOFFにする
			m_apSelect[nCnt]->SetEnableDraw(false);

			// 現在の選択要素の色を白に設定
			m_apSelect[nCnt]->SetColor(DEFAULT_COL);
		}

		// テクスチャを設定
		m_apSelect[SELECT_YES]->BindTexture(mc_apTextureFile[TEXTURE_RESTART]);
		m_apSelect[SELECT_NO]->BindTexture(mc_apTextureFile[TEXTURE_BACK]);
	}

#if 1
	//--------------------------------------------------------
	//<*******************************************************
	//ランキングのソート処理
	//<*******************************************************
	//--------------------------------------------------------
	for (int nCntOrg = 0; nCntOrg < m_nNumPlay - 1; nCntOrg++)
	{ // 参加プレイヤー数分繰り返す

		for (int nCntTemp = nCntOrg; nCntTemp < m_nNumPlay; nCntTemp++)
		{ // 参加プレイヤー数分繰り返す

			if (m_anRank[nCntOrg] > m_anRank[nCntTemp])
			{
				int temp;

				temp = m_anRank[nCntOrg];
				m_anRank[nCntOrg] = m_anRank[nCntTemp];
				m_anRank[nCntTemp] = temp;

				temp = m_anSaveRank[nCntOrg];
				m_anSaveRank[nCntOrg] = m_anSaveRank[nCntTemp];
				m_anSaveRank[nCntTemp] = temp;
			}

			if (m_anSaveRank[nCntOrg] > m_anSaveRank[nCntTemp] && m_anRank[nCntOrg] == m_anRank[nCntTemp])
			{
				int temp;

				temp = m_anSaveRank[nCntOrg];
				m_anSaveRank[nCntOrg] = m_anSaveRank[nCntTemp];
				m_anSaveRank[nCntTemp] = temp;
			}
		}
	}

#else
	// TODO：順位表示のテクスチャ割り当て
#endif

	// サウンドの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_FIGHT);	// 戦闘音

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
	//フレーム終了処理
	for (int nCnt = 0; nCnt < NUM_FRAME; nCnt++)
	{
		if (m_apFrame[nCnt] != nullptr)
		{
			m_apFrame[nCnt]->Uninit();
			m_apFrame[nCnt] = nullptr;
		}
	}

	//<******************************************
	//プレイヤー数に関係しているオブジェクトの終了処理
	//<******************************************
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{
		//順位
		if (m_apNumber[nCnt] != nullptr)
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
		//プレイヤーアイコン
		if (m_apIcon[nCnt] != nullptr)
		{
			m_apIcon[nCnt]->Uninit();
			m_apIcon[nCnt] = nullptr;
		}
		//勝利者番号
		if (m_apWinNum[nCnt] != nullptr)
		{
			m_apWinNum[nCnt]->Uninit();
			m_apWinNum[nCnt] = nullptr;
		}
		//プレイヤー番号
		if (m_apPlayerEntry[nCnt] != nullptr)
		{
			m_apPlayerEntry[nCnt]->Uninit();
			m_apPlayerEntry[nCnt] = nullptr;
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

	m_pCover->Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CResultManager::Update(void)
{

	//<******************************************
	//プレイヤーアイコンの位置設定
	//<******************************************
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{
		//中身チェック
		if (m_apIcon[nCnt] != nullptr)
		{
			m_apIcon[nCnt]->Update();

			//プレイヤー順に並べる
			m_apIcon[m_anSaveRank[nCnt]]
				->SetVec3Position(m_arOriginPos[EObj::OBJ_ICON][nCnt]);
		}

		//<******************************************
		//プレイヤー順位の位置設定
		//<******************************************
		if (m_apWinNum[nCnt] != nullptr)
		{
			m_apWinNum[nCnt]->Update();

			//その順位のプレイヤーのテクスチャパターンにする
			m_apWinNum[nCnt]->SetPattern(m_anSaveRank[nCnt] + 1);

			//プレイヤー順に並べる
			m_apWinNum[nCnt]
				->SetVec3Position(m_arOriginPos[EObj::OBJ_PLAYER][nCnt]);
		}
		//中身チェック
		if (m_apPlayerEntry[nCnt] != nullptr)
		{
			//更新処理
			m_apPlayerEntry[nCnt]->Update();
		}

		m_apNumber[nCnt]->SetPattern(m_anRank[nCnt]);
	}

	// 遷移決定の更新
	UpdateTransition();

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない状態

		// 無し

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

		UpdateNumber();

		//もしどちらの判定もtrueになっていたら
		if (m_abool[EObj::OBJ_FRAME] == true
			&& m_abool[EObj::OBJ_NUMBER] == true)
		{
			//スキップされている状態にする
			m_bSkiped = true;

			//キー入力待機状態にする
			m_state = STATE_HOLD;
		}

		break;

		//キー入力待機状態
	case STATE_HOLD:

		if (playerMotion == -1)
		{
			playerMotion = rand() % 4;

			CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_anSaveRank[0]);
			player->SetMotion(CPlayer::MOTION_EMOTE_RORI + playerMotion);
		}

		//何かのキーが押されていたら
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

		//選択
	case STATE_SELECT:

		UpdateCover();
		
		break;

	case STATE_WAIT:	// 遷移待機状態

		//選択の更新
		UpdateSelect();

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

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
			//更新処理
			m_apWinLog[nCnt]->Update();
			
			//目的の位置に到着するまで加算し続ける
			m_arSize[EObj::OBJ_WIN].x += (Win::DESTSIZE_WIN[Win::NUMBER_ZERO].x + m_arSize[EObj::OBJ_WIN].x) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

			//目的の位置に到着するまで加算し続ける
			m_arSize[EObj::OBJ_WIN_NUM].x += (Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].x + m_arSize[EObj::OBJ_WIN_NUM].x) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

			//そのサイズを超えそうになったら
			if (m_arSize[EObj::OBJ_WIN].x >= Win::DESTSIZE_WIN[Win::NUMBER_ZERO].x&&
				m_arSize[EObj::OBJ_WIN_NUM].x >= Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].x)
			{
				//値を固定する
				m_arSize[EObj::OBJ_WIN].x = Win::DESTSIZE_WIN[Win::NUMBER_ZERO].x;
				m_arSize[EObj::OBJ_WIN_NUM].x = Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].x;
			}

			//目的の位置に到着するまで加算し続ける
			m_arSize[EObj::OBJ_WIN].y += (Win::DESTSIZE_WIN[Win::NUMBER_ZERO].y + m_arSize[EObj::OBJ_WIN].y) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

			//目的の位置に到着するまで加算し続ける
			m_arSize[EObj::OBJ_WIN_NUM].y += (Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].y + m_arSize[EObj::OBJ_WIN].y) * Win::VALUE_INERTIA[Win::NUMBER_ZERO];

			//そのサイズを超えそうになったら
			if (m_arSize[EObj::OBJ_WIN].y >= Win::DESTSIZE_WIN[Win::NUMBER_ZERO].y
				&&m_arSize[EObj::OBJ_WIN_NUM].y >= Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].y)
			{//値を固定する

				m_arSize[EObj::OBJ_WIN].y = Win::DESTSIZE_WIN[Win::NUMBER_ZERO].y;
				m_arSize[EObj::OBJ_WIN_NUM].y = Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].y;
			}

			//サイズの設定を行う
			m_apWinLog[Win::NUMBER_ZERO]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN]);

			//サイズの設定を行う
			m_apWinLog[Win::NUMBER_ONE]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN_NUM]);

			//どちらも目的の位置に到着していたら
			if (m_arSize[EObj::OBJ_WIN].x == Win::DESTSIZE_WIN[Win::NUMBER_ZERO].x &&
				m_arSize[EObj::OBJ_WIN].y == Win::DESTSIZE_WIN[Win::NUMBER_ZERO].y&&
				m_arSize[EObj::OBJ_WIN_NUM].x == Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].x &&
				m_arSize[EObj::OBJ_WIN_NUM].y == Win::DESTSIZE_NUMBER[Win::NUMBER_ZERO].y)
			{
				m_anWaitTime[EObj::OBJ_WIN]++;

				//もし0.0の位置にどの座標もあったら
				if (m_arPos[EObj::OBJ_WIN].x == 0.0f&&
					m_arPos[EObj::OBJ_WIN].y == 0.0f&&
					m_arPos[EObj::OBJ_WIN].z == 0.0f)
				{
					//瞬間移動を防止するため
					m_arPos[EObj::OBJ_WIN].x = Win::POS.x;
					m_arPos[EObj::OBJ_WIN].y = Win::POS.y;
					m_arPos[EObj::OBJ_WIN].z = Win::POS.z;
				}
			}

			//待機時間が最大待機時間を超えていたら
			if (m_anWaitTime[EObj::OBJ_WIN] >= Win::MAX_WAIT)
			{
				//中身がないことを
				if (m_arSize[EObj::OBJ_WIN_NUM].x == 0.0f
					&&m_arSize[EObj::OBJ_WIN_NUM].y == 0.0f)
				{
					m_arSize[EObj::OBJ_WIN_NUM] = m_arSize[EObj::OBJ_WIN];
				}

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
				m_arSize[EObj::OBJ_WIN].x -= (Win::DESTSIZE_WIN[Win::NUMBER_ONE].x + m_arSize[EObj::OBJ_WIN].x) * Win::VALUE_INERTIA[Win::NUMBER_ONE];
				m_arSize[EObj::OBJ_WIN_NUM].x -= (Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].x + m_arSize[EObj::OBJ_WIN_NUM].x) * Win::VALUE_INERTIA[Win::NUMBER_ONE];

				//そのサイズを超えそうになったら
				if (m_arSize[EObj::OBJ_WIN].x <= Win::DESTSIZE_WIN[Win::NUMBER_ONE].x&&
					m_arSize[EObj::OBJ_WIN_NUM].x <= Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].x)
				{
					//値を固定する
					m_arSize[EObj::OBJ_WIN].x = Win::DESTSIZE_WIN[Win::NUMBER_ONE].x;
					m_arSize[EObj::OBJ_WIN_NUM].x = Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].x;
				}

				//目的の位置に到着するまで加算し続ける
				m_arSize[EObj::OBJ_WIN].y -= (Win::DESTSIZE_WIN[Win::NUMBER_ONE].y + m_arSize[EObj::OBJ_WIN].y)* Win::VALUE_INERTIA[Win::NUMBER_ONE];
				m_arSize[EObj::OBJ_WIN_NUM].y -= (Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].y + m_arSize[EObj::OBJ_WIN_NUM].y) * Win::VALUE_INERTIA[Win::NUMBER_ONE];

				//そのサイズを超えそうになったら
				if (m_arSize[EObj::OBJ_WIN].y <= Win::DESTSIZE_WIN[Win::NUMBER_ONE].y&&
					m_arSize[EObj::OBJ_WIN_NUM].y <= Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].y)
				{
					//値を固定する
					m_arSize[EObj::OBJ_WIN].y = Win::DESTSIZE_WIN[Win::NUMBER_ONE].y;
					m_arSize[EObj::OBJ_WIN_NUM].y = Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].y;
				}

				//サイズの設定を行う
				m_apWinLog[Win::NUMBER_ZERO]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN]);
				m_apWinLog[Win::NUMBER_ONE]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN_NUM]);

				//位置を設定する
				m_apWinLog[nCnt]->SetVec3Position(D3DXVECTOR3(
					m_arPos[EObj::OBJ_WIN].x + Win::DISTANCE[Win::NUMBER_ONE].x * nCnt,
					m_arPos[EObj::OBJ_WIN].y + Win::DISTANCE[Win::NUMBER_ONE].y * nCnt,
					m_arPos[EObj::OBJ_WIN].z));

				//もしサイズ・位置がその位置にあったら
				if (m_apWinLog[Win::NUMBER_ZERO]->GetVec3Sizing().x == Win::DESTSIZE_WIN[Win::NUMBER_ONE].x&&
					m_apWinLog[Win::NUMBER_ZERO]->GetVec3Sizing().y == Win::DESTSIZE_WIN[Win::NUMBER_ONE].y&&
					m_apWinLog[Win::NUMBER_ONE]->GetVec3Sizing().x == Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].x&&
					m_apWinLog[Win::NUMBER_ONE]->GetVec3Sizing().y == Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].y&&
					m_arPos[EObj::OBJ_WIN].x == Win::DESTPOS.x&&
					m_arPos[EObj::OBJ_WIN].y == Win::DESTPOS.y)
				{
	
					//巨大フレームのステートに移行
					m_state = STATE_BIG_FRAME;

					// サウンドの再生
					CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
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
	//いるかはわからないが念のため
	if (m_anWaitTime[EObj::OBJ_WIN] != 0)
	{
		//0に初期化する
		m_anWaitTime[EObj::OBJ_WIN] = 0;
	}

	//中身チェック
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

		//目的のサイズになっていたら
		if (m_arSize[EObj::OBJ_BIGFRAME].x == BigFrame::DESTSIZE.x
		&&  m_arSize[EObj::OBJ_BIGFRAME].y == BigFrame::DESTSIZE.y)
		{
			//待機時間を増やす
			m_anWaitTime[EObj::OBJ_BIGFRAME]++;
			
			if (m_anWaitTime[EObj::OBJ_BIGFRAME] == 1)
			{ // 目的サイズなったばかりの場合

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
			}

			//最大待機時間になっていたら
			if (m_anWaitTime[EObj::OBJ_BIGFRAME] == BigFrame::MAX_WAIT)
			{
				CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_anSaveRank[0]);

				player->SetEnableDraw(true);
				player->GetPlayerGuide()->SetEnableDraw(false);

				m_anWaitTime[EObj::OBJ_BIGFRAME] = 0;
				m_state = STATE_FRAME;

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_JUMP);	// 戦闘音
			}
		}
	}

}
//============================================================
//	フレームの更新処理
//============================================================
void CResultManager::UpdateFrame(void)
{
	if (m_abSizeFinish[EObj::OBJ_FRAME] == false)
	{
		//中身チェック
		if (m_apFrame[m_anNum[EObj::OBJ_FRAME]] != nullptr)
		{
			//描画をするようにする
			//m_apFrame[m_anNum[EObj::OBJ_FRAME]]->SetEnableDraw(true);
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
				if (m_anNum[EObj::OBJ_FRAME] >= m_nNumPlay-1)
				{
					//フレーム
					m_anNum[EObj::OBJ_FRAME] = m_nNumPlay - 1;
					m_abSizeFinish[EObj::OBJ_FRAME] = true;
					

				}
				//範囲外にいっていなければ
				else if (!(m_anNum[EObj::OBJ_FRAME] >= m_nNumPlay - 1))
				{
					//フレーム
					//待機時間を加算する
					m_anWaitTime[EObj::OBJ_FRAME]++;

					//待機時間の最大値を超えそうになったら
					if (m_anWaitTime[EObj::OBJ_FRAME] >= Frame::MAX_WAIT)
					{
						m_abSizeFinish[EObj::OBJ_FRAME] = true;

						//値のリセットを行う
						m_abSizeFinish[EObj::OBJ_NUMBER] = false;
						m_anWaitTime[EObj::OBJ_FRAME] = 0;
						m_anNum[EObj::OBJ_FRAME]++;
						m_arSize[EObj::OBJ_FRAME].x = Frame::INIT_SIZE.x;
						m_arSize[EObj::OBJ_FRAME].y = Frame::INIT_SIZE.y;

					}
				}
			}
			//番号・Xサイズ・Yサイズがその値になっていたら
			if (m_anNum[EObj::OBJ_FRAME] == m_nNumPlay - 1 &&
				m_apFrame[m_anNum[EObj::OBJ_FRAME]]->GetVec3Sizing().x == Frame::DESTSIZE.x&&
				m_apFrame[m_anNum[EObj::OBJ_FRAME]]->GetVec3Sizing().y == Frame::DESTSIZE.y)
			{
				//判定をtrueにする
				m_abool[EObj::OBJ_FRAME] = true;
			}
		}
	}
}
//============================================================
//	番号の更新処理
//============================================================
void CResultManager::UpdateNumber(void)
{
	if (m_abSizeFinish[EObj::OBJ_FRAME] == true)
	{
		//中身チェック
		if (m_apNumber[m_anNum[EObj::OBJ_NUMBER]] != nullptr)
		{
			//番号
			m_apNumber[m_anNum[EObj::OBJ_NUMBER]]->Update();

			//目的の位置に到着するまで加算し続ける
			m_arSize[EObj::OBJ_NUMBER].x += (Number::DESTSIZE.x + m_arSize[EObj::OBJ_NUMBER].x) * Number::VALUE_INERTIA;

			//そのサイズを超えそうになったら
			if (m_arSize[EObj::OBJ_NUMBER].x >= Number::DESTSIZE.x)
			{
				//値を固定する
				m_arSize[EObj::OBJ_NUMBER].x = Number::DESTSIZE.x;
			}

			//目的の位置に到着するまで加算し続ける
			m_arSize[EObj::OBJ_NUMBER].y += (Number::DESTSIZE.y + m_arSize[EObj::OBJ_NUMBER].y) * Number::VALUE_INERTIA;

			//そのサイズを超えそうになったら
			if (m_arSize[EObj::OBJ_NUMBER].y >= Number::DESTSIZE.y)
			{
				//値を固定する
				m_arSize[EObj::OBJ_NUMBER].y = Number::DESTSIZE.y;

			}

			//サイズを設定する
			m_apNumber[m_anNum[EObj::OBJ_NUMBER]]->SetVec3Sizing(m_arSize[EObj::OBJ_NUMBER]);

			//もしフレームの拡大率がその値になっていたら
			if (m_apNumber[m_anNum[EObj::OBJ_NUMBER]]->GetVec3Sizing().x == Number::DESTSIZE.x
				&&m_apNumber[m_anNum[EObj::OBJ_NUMBER]]->GetVec3Sizing().y == Number::DESTSIZE.y)
			{

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 戦闘音

				//もしアイコン・勝利者番号・プレイヤー番号の中身があれば
				if (m_apIcon[m_anSaveRank[m_anNum[EObj::OBJ_NUMBER]]] != nullptr&&
					m_apWinNum[m_anNum[EObj::OBJ_NUMBER]] != nullptr&&
					m_apPlayerEntry[m_anNum[EObj::OBJ_NUMBER]] != nullptr)
				{
					//描画をする
					m_apIcon[m_anSaveRank[m_anNum[EObj::OBJ_NUMBER]]]->SetEnableDraw(true);
					m_apWinNum[m_anNum[EObj::OBJ_NUMBER]]->SetEnableDraw(true);
					m_apPlayerEntry[m_anNum[EObj::OBJ_NUMBER]]->SetEnableDraw(true);
				}

				//範囲外にいかないようにする
				if (m_anNum[EObj::OBJ_NUMBER] >= m_nNumPlay-1)
				{
					//番号
					m_anNum[EObj::OBJ_NUMBER] = m_nNumPlay - 1;
					m_abSizeFinish[EObj::OBJ_NUMBER] = true;


				}
				//範囲外にいっていなければ
				else if (!(m_anNum[EObj::OBJ_NUMBER] >= m_nNumPlay - 1))
				{
					//待機時間を加算する
					m_anWaitTime[EObj::OBJ_NUMBER]++;

					//待機時間の最大値を超えそうになったら
					if (m_anWaitTime[EObj::OBJ_NUMBER] >= Number::MAX_WAIT)
					{
						m_abSizeFinish[EObj::OBJ_NUMBER] = true;

						//値のリセットを行う
						m_abSizeFinish[EObj::OBJ_FRAME] = false;
						m_anWaitTime[EObj::OBJ_NUMBER] = 0;
						m_anNum[EObj::OBJ_NUMBER]++;
						m_arSize[EObj::OBJ_NUMBER].x = Number::INIT_SIZE.x;
						m_arSize[EObj::OBJ_NUMBER].y = Number::INIT_SIZE.y;
					}
				}
			}
			//番号・Xサイズ・Yサイズがその値になっていたら
			if (m_anNum[EObj::OBJ_NUMBER] == m_nNumPlay - 1 &&
				m_apNumber[m_anNum[EObj::OBJ_NUMBER]]->GetVec3Sizing().x == Number::DESTSIZE.x&&
				m_apNumber[m_anNum[EObj::OBJ_NUMBER]]->GetVec3Sizing().y == Number::DESTSIZE.y)
			{
				//判定をtrueにする
				m_abool[EObj::OBJ_NUMBER] = true;
			}
		}
	}
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
	|| CManager::GetInstance()->GetPad()->IsTriggerAll(CInputPad::KEY_LEFT))
	{ // 左移動の操作が行われた場合

		// 左に選択をずらす
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
	}
	if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_D)
	||  CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_RIGHT)
	|| CManager::GetInstance()->GetPad()->IsTriggerAll(CInputPad::KEY_RIGHT))
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

			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
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
		||  CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_SPACE)
		||  CManager::GetInstance()->GetPad()->IsTriggerAll(CInputPad::KEY_A)
		||  CManager::GetInstance()->GetPad()->IsTriggerAll(CInputPad::KEY_B)
		||  CManager::GetInstance()->GetPad()->IsTriggerAll(CInputPad::KEY_X)
		||  CManager::GetInstance()->GetPad()->IsTriggerAll(CInputPad::KEY_Y)
		||  CManager::GetInstance()->GetPad()->IsTriggerAll(CInputPad::KEY_START))
		{
			if (m_state != STATE_WAIT)
			{ // 遷移待機状態ではない場合

				// フレームステートではなければ
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
					CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
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
	//勝利ロゴの数だけ回す
	for (int nCnt = 0; nCnt < NUM_WIN; nCnt++)
	{
		//中身チェック
		if (m_apWinLog[nCnt] != nullptr)
		{
			//サイズ・位置の設定
			m_arSize[EObj::OBJ_WIN].x = Win::DESTSIZE_WIN[Win::NUMBER_ONE].x;
			m_arSize[EObj::OBJ_WIN].y = Win::DESTSIZE_WIN[Win::NUMBER_ONE].y;

			//サイズ・位置の設定
			m_arSize[EObj::OBJ_WIN_NUM].x = Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].x;
			m_arSize[EObj::OBJ_WIN_NUM].y = Win::DESTSIZE_NUMBER[Win::NUMBER_ONE].y;


			m_arPos[EObj::OBJ_WIN].x = Win::DESTPOS.x;
			m_arPos[EObj::OBJ_WIN].y = Win::DESTPOS.y;

			//サイズ・位置のセットを行う
			m_apWinLog[Win::NUMBER_ZERO]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN]);

			//サイズ・位置のセットを行う
			m_apWinLog[Win::NUMBER_ONE]->SetVec3Sizing(m_arSize[EObj::OBJ_WIN_NUM]);

			m_apWinLog[nCnt]->SetVec3Position(D3DXVECTOR3(
				m_arPos[EObj::OBJ_WIN].x + Win::DISTANCE[Win::NUMBER_ONE].x * nCnt,
				m_arPos[EObj::OBJ_WIN].y + Win::DISTANCE[Win::NUMBER_ONE].y * nCnt,
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
	for (int nCnt = 0; nCnt < m_nNumPlay - 1; nCnt++)
	{
		//描画をするようにする
		m_apFrame[nCnt]->SetEnableDraw(true);

		//サイズの設定
		m_arSize[EObj::OBJ_FRAME].x = Frame::DESTSIZE.x;
		m_arSize[EObj::OBJ_FRAME].y = Frame::DESTSIZE.y;

		//サイズのセットを行う
		m_apFrame[nCnt]->SetVec3Sizing(m_arSize[EObj::OBJ_FRAME]);
		m_anNum[OBJ_FRAME] = m_nNumPlay - 1;
	}

	//フレーム数分回す
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{
		//サイズの設定
		m_arSize[EObj::OBJ_NUMBER].x = Number::DESTSIZE.x;
		m_arSize[EObj::OBJ_NUMBER].y = Number::DESTSIZE.y;

		//サイズのセットを行う
		m_apNumber[nCnt]->SetVec3Sizing(m_arSize[EObj::OBJ_NUMBER]);
		m_anNum[OBJ_NUMBER] = m_nNumPlay - 1;

		m_apIcon[nCnt]->SetEnableDraw(true);
		m_apWinNum[nCnt]->SetEnableDraw(true);
		m_apPlayerEntry[nCnt]->SetEnableDraw(true);
	}
	m_state = STATE_FRAME;

	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_anSaveRank[0]);
	player->SetEnableDraw(true);
	player->GetPlayerGuide()->SetEnableDraw(false);
}
