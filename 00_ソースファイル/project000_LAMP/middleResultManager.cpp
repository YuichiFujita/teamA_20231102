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
#include "valueUI.h"
#include "multiValue.h"
#include "fade.h"
#include "player.h"
#include "retentionManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 14;	// 中間リザルトの優先順位
	const int WAIT_FRAME = 8;	// 遷移の余韻フレーム

	// フェード基本情報
	namespace fade
	{
		const D3DXVECTOR3 UP_POS	= D3DXVECTOR3(SCREEN_CENT.x, -900.0f, 0.0f);					// フェード上位置
		const D3DXVECTOR3 CENT_POS	= D3DXVECTOR3(SCREEN_CENT.x, SCREEN_CENT.y, 0.0f);				// フェード中央位置
		const D3DXVECTOR3 DOWN_POS	= D3DXVECTOR3(SCREEN_CENT.x, SCREEN_HEIGHT + 900.0f, 0.0f);		// フェード下位置
		const D3DXVECTOR3 UP_MIDDLE_POS		= D3DXVECTOR3(CENT_POS.x, UP_POS.y + 300.0f, 0.0f);		// フェード上中央位置
		const D3DXVECTOR3 DOWN_MIDDLE_POS	= D3DXVECTOR3(CENT_POS.x, DOWN_POS.y - 300.0f, 0.0f);	// フェード下中央位置

		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(SCREEN_SIZE.x, 1770.0f, 0.0f);	// フェード大きさ
		const D3DXCOLOR		COL		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.88f);			// フェード色

		const int	WAIT_FRAME	= 8;		// フェード待機フレーム
		const float	ADD_MOVE	= 0.65f;	// フェード移動量
		const float	ADD_ACCEL_MOVE = 2.1f;	// フェード加速移動量
	}

	// ランキングタイトル基本情報
	namespace title
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(280.0f, 160.0f, 0.0f);	// タイトル位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(400.0f, 130.0f, 0.0f);	// タイトル大きさ

		const int	WAIT_FRAME	= 24;		// タイトル待機フレーム
		const float	INIT_SCALE	= 0.025f;	// タイトル初期拡大率
		const float	ADD_SCALE	= 0.09f;	// タイトル加算拡大率
		const float	SET_SCALE	= 1.0f;		// タイトル設定拡大率
	}

	// 勝利ポイント基本情報
	namespace win
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(760.0f, 160.0f, 0.0f);			// 位置
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(230.0f, 0.0f, 0.0f);				// 行間
		const D3DXVECTOR3 SPACE_VALUE	= D3DXVECTOR3(80.0f, 0.0f, 0.0f);		// 数字行間
		const D3DXVECTOR3 SIZE_TITLE	= D3DXVECTOR3(300.0f, 100.0f, 0.0f);	// タイトル大きさ
		const D3DXVECTOR3 SIZE_VALUE	= D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// 数字大きさ

		const float	DIGIT = 2;	// 桁数
	}

	// 勝利ポイント背景基本情報
	namespace winBG
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(860.0f, 160.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(620.0f, 165.0f, 0.0f);	// 大きさ

		const int	WAIT_FRAME	= 24;		// タイトル待機フレーム
		const float	INIT_SCALE	= 0.025f;	// タイトル初期拡大率
		const float	ADD_SCALE	= 0.09f;	// タイトル加算拡大率
		const float	SET_SCALE	= 1.0f;		// タイトル設定拡大率
	}

	// プレイヤーナンバー基本情報
	namespace number
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(160.0f, 350.0f, 0.0f);	// 位置
		const D3DXVECTOR3	SIZE_TITLE	= D3DXVECTOR3(242.0f, 107.0f, 0.0f);	// タイトル大きさ
		const D3DXVECTOR3	SIZE_VALUE	= D3DXVECTOR3(80.0f, 90.0f, 0.0f);		// 数字大きさ
		const D3DXVECTOR3	SPACE_POS	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// 数字UI同士の空白
		const D3DXVECTOR3	SPACE_TITLE	= D3DXVECTOR3(100.0f, 5.0f, 0.0f);		// タイトル空白
		const D3DXVECTOR3	SPACE_VALUE	= VEC3_ZERO;							// 数字空白
		const D3DXCOLOR		COL_NOTJOIN = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);	// 非参加時の色
		const int			DIGIT		= 1;									// 桁数
	}

	// プレイヤー勝利ポイント基本情報
	namespace playerPoint
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(160.0f, 460.0f, 0.0f);	// 位置
		const D3DXVECTOR3	SIZE_TITLE	= D3DXVECTOR3(242.0f, 107.0f, 0.0f);	// タイトル大きさ
		const D3DXVECTOR3	SIZE_VALUE	= D3DXVECTOR3(80.0f, 90.0f, 0.0f);		// 数字大きさ
		const D3DXVECTOR3	SPACE_POS	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// 数字UI同士の空白
		const D3DXVECTOR3	SPACE_TITLE	= D3DXVECTOR3(-40.0f, 100.0f, 0.0f);	// タイトル空白
		const D3DXVECTOR3	SPACE_VALUE	= D3DXVECTOR3(80.0f, 0.0f, 0.0f);		// 数字空白
		const int			DIGIT		= 2;									// 桁数

		const int	WAIT_FRAME	= 8;		// プレイヤー勝利ポイント待機フレーム
		const float	INIT_SCALE	= 0.025f;	// プレイヤー勝利ポイント初期拡大率
		const float	ADD_SCALE	= 0.15f;	// プレイヤー勝利ポイント加算拡大率
		const float	SET_SCALE	= 1.0f;		// プレイヤー勝利ポイント設定拡大率
	}

	// プレイヤーフレーム基本情報
	namespace frame
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(160.0f, 460.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(340.0f, 340.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// 空白

		const int	WAIT_FRAME	= 24;		// プレイヤーフレーム待機フレーム
		const float	INIT_SCALE	= 0.025f;	// プレイヤーフレーム初期拡大率
		const float	ADD_SCALE	= 0.09f;	// プレイヤーフレーム加算拡大率
		const float	SET_SCALE	= 1.0f;		// プレイヤーフレーム設定拡大率
	}

	// 操作基本情報
	namespace control
	{
		const float	ADD_ALPHA		= 0.0045f;	// 透明度の加算量
		const float	ADD_SINROT		= 0.04f;	// 透明度ふわふわさせる際のサインカーブ向き加算量
		const float	MAX_ADD_ALPHA	= 0.45f;	// 透明度の最大加算量
		const float	BASIC_ALPHA		= 0.85f;	// 基準の透明度

		const D3DXVECTOR3	POS		= D3DXVECTOR3(1080.0f, 660.0f, 0.0f);	// 位置
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(370.0f, 90.0f, 0.0f);		// 大きさ
		const D3DXCOLOR		MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);	// 色
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CMiddleResultManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\middleResult000.png",	// フェードテクスチャ
	"data\\TEXTURE\\Mid_result.png",	// ランキングタイトルテクスチャ
	"data\\TEXTURE\\white_flame.png",				// 勝利ポイント背景テクスチャ
	"data\\TEXTURE\\target_score.png",	// 勝利ポイントタイトルテクスチャ
	"data\\TEXTURE\\entry_player.png",		// プレイヤーテクスチャ
	"data\\TEXTURE\\Score.png",	// 勝利ポイントテクスチャ
	"data\\TEXTURE\\entry_flame.png",		// フレームテクスチャ
	"data\\TEXTURE\\Press_Start_Crack.png",	// 操作方法テクスチャ
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
	memset(&m_apNumber[0], 0, sizeof(m_apNumber));					// プレイヤーナンバーの情報
	memset(&m_apPlayerWinPoint[0], 0, sizeof(m_apPlayerWinPoint));	// プレイヤー勝利ポイントの情報
	memset(&m_apFrame[0], 0, sizeof(m_apFrame));					// プレイヤーフレームの情報

	m_pControl		= NULL;	// 操作の情報
	m_pFade			= NULL;	// フェードの情報
	m_pTitle		= NULL;	// タイトルの情報
	m_pWinPointBG	= NULL;	// 勝利ポイントの背景情報
	m_pWinPoint		= NULL;	// 勝利ポイントの情報

	m_state		= STATE_FADEIN;	// 状態
	m_fMoveY	= 0.0f;			// 縦移動量
	m_fScale	= 0.0f;			// 拡大率
	m_fSinAlpha	= 0.0f;			// 透明向き
	m_nCounterState = 0;		// 状態管理カウンター
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
	//--------------------------------------------------------
	//	メンバ変数の初期化
	//--------------------------------------------------------
	memset(&m_apNumber[0], 0, sizeof(m_apNumber));					// プレイヤーナンバーの情報
	memset(&m_apPlayerWinPoint[0], 0, sizeof(m_apPlayerWinPoint));	// プレイヤー勝利ポイントの情報
	memset(&m_apFrame[0], 0, sizeof(m_apFrame));					// プレイヤーフレームの情報

	m_pControl		= NULL;	// 操作の情報
	m_pFade			= NULL;	// フェードの情報
	m_pTitle		= NULL;	// タイトルの情報
	m_pWinPointBG	= NULL;	// 勝利ポイントの背景情報
	m_pWinPoint		= NULL;	// 勝利ポイントの情報

	m_state		= STATE_FADEIN;	// 状態
	m_fMoveY	= 0.0f;			// 縦移動量
	m_fScale	= 1.0f;			// 拡大率
	m_fSinAlpha	= -HALF_PI;		// 透明向き
	m_nCounterState = 0;		// 状態管理カウンター

	//--------------------------------------------------------
	//	フェードの生成
	//--------------------------------------------------------
	m_pFade = CObject2D::Create
	( // 引数
		fade::UP_POS,	// 位置
		fade::SIZE,		// 大きさ
		VEC3_ZERO,		// 向き
		fade::COL		// 色
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

	//--------------------------------------------------------
	//	タイトルの生成
	//--------------------------------------------------------
	m_pTitle = CObject2D::Create
	( // 引数
		title::POS,						// 位置
		title::SIZE * title::INIT_SCALE	// 大きさ
	);
	if (m_pTitle == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pTitle->BindTexture(mc_apTextureFile[TEXTURE_TITLE]);

	// 優先順位を設定
	m_pTitle->SetPriority(PRIORITY);

	// 自動描画をOFFにする
	m_pTitle->SetEnableDraw(false);

	//--------------------------------------------------------
	//	勝利ポイント背景の生成
	//--------------------------------------------------------
	m_pWinPointBG = CObject2D::Create
	( // 引数
		winBG::POS,						// 位置
		winBG::SIZE * winBG::INIT_SCALE	// 大きさ
	);
	if (m_pWinPointBG == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pWinPointBG->BindTexture(mc_apTextureFile[TEXTURE_WIN_BG]);

	// 優先順位を設定
	m_pWinPointBG->SetPriority(PRIORITY);

	// 自動描画をOFFにする
	m_pWinPointBG->SetEnableDraw(false);

	//--------------------------------------------------------
	//	勝利ポイントの生成
	//--------------------------------------------------------
	m_pWinPoint = CValueUI::Create
	( // 引数
		mc_apTextureFile[TEXTURE_WIN],	// タイトルテクスチャパス
		CValue::TEXTURE_UI,	// 数字テクスチャ
		win::DIGIT,			// 桁数
		win::POS,			// 位置
		win::SPACE,			// 行間
		win::SPACE_VALUE,	// 数字行間
		win::SIZE_TITLE,	// タイトル大きさ
		win::SIZE_VALUE		// 数字大きさ
	);
	if (m_pWinPoint == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pWinPoint->SetPriority(PRIORITY);

	// 自動描画をOFFにする
	m_pWinPoint->SetEnableDraw(false);

	// 数字を設定
	m_pWinPoint->GetMultiValue()->SetNum(CManager::GetInstance()->GetRetentionManager()->GetWinPoint());

	//--------------------------------------------------------
	//	プレイヤーフレーム・勝利ポイント・ナンバーの生成
	//--------------------------------------------------------
	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		// ポイントを宣言
		CPlayer *pPlayer = CScene::GetPlayer(nCntEntry);	// プレイヤー情報

		// 変数を宣言
		int nPlayerWinPoint = (pPlayer == NULL) ? 0 : pPlayer->GetWinPoint();			// プレイヤー勝利ポイント
		D3DXCOLOR colPolygon = (pPlayer == NULL) ? number::COL_NOTJOIN : XCOL_WHITE;	// ポリゴン色

		// プレイヤーフレームの生成
		m_apFrame[nCntEntry] = CObject2D::Create
		( // 引数
			frame::POS + (frame::SPACE * (float)nCntEntry),	// 位置
			frame::SIZE * frame::INIT_SCALE,				// 大きさ
			VEC3_ZERO,		// 向き
			colPolygon		// 色
		);
		if (m_apFrame[nCntEntry] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_apFrame[nCntEntry]->BindTexture(mc_apTextureFile[TEXTURE_FRAME]);

		// 優先順位を設定
		m_apFrame[nCntEntry]->SetPriority(PRIORITY);

		// 自動描画をOFFにする
		m_apFrame[nCntEntry]->SetEnableDraw(false);

		// プレイヤー勝利ポイントの生成
		m_apPlayerWinPoint[nCntEntry] = CValueUI::Create
		( // 引数
			mc_apTextureFile[TEXTURE_PLAYER_POINT],							// タイトルテクスチャパス
			CValue::TEXTURE_UI,												// 数字テクスチャ
			playerPoint::DIGIT,												// 桁数
			playerPoint::POS + (playerPoint::SPACE_POS * (float)nCntEntry),	// 位置
			playerPoint::SPACE_TITLE,							// 行間
			playerPoint::SPACE_VALUE,							// 数字行間
			playerPoint::SIZE_TITLE * playerPoint::INIT_SCALE,	// タイトル大きさ
			playerPoint::SIZE_VALUE * playerPoint::INIT_SCALE,	// 数字大きさ
			VEC3_ZERO,	// タイトル向き
			VEC3_ZERO,	// 数字向き
			colPolygon,	// タイトル色
			colPolygon	// 数字色
		);
		if (m_apPlayerWinPoint[nCntEntry] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apPlayerWinPoint[nCntEntry]->SetPriority(PRIORITY);

		// 自動描画をOFFにする
		m_apPlayerWinPoint[nCntEntry]->SetEnableDraw(false);

		// 数字を設定
		m_apPlayerWinPoint[nCntEntry]->GetMultiValue()->SetNum(nPlayerWinPoint);

		// プレイヤーナンバーの生成
		m_apNumber[nCntEntry] = CValueUI::Create
		( // 引数
			mc_apTextureFile[TEXTURE_PLAYER],						// タイトルテクスチャパス
			CValue::TEXTURE_NORMAL,									// 数字テクスチャ
			number::DIGIT,											// 桁数
			number::POS + (number::SPACE_POS * (float)nCntEntry),	// 位置
			number::SPACE_TITLE,							// 行間
			number::SPACE_VALUE,							// 数字行間
			number::SIZE_TITLE * playerPoint::INIT_SCALE,	// タイトル大きさ
			number::SIZE_VALUE * playerPoint::INIT_SCALE,	// 数字大きさ
			VEC3_ZERO,	// タイトル向き
			VEC3_ZERO,	// 数字向き
			colPolygon,	// タイトル色
			colPolygon	// 数字色
		);
		if (m_apNumber[nCntEntry] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apNumber[nCntEntry]->SetPriority(PRIORITY);

		// 自動描画をOFFにする
		m_apNumber[nCntEntry]->SetEnableDraw(false);

		// 数字を設定
		m_apNumber[nCntEntry]->GetMultiValue()->SetNum(nCntEntry + 1);
	}

	//--------------------------------------------------------
	//	操作の生成
	//--------------------------------------------------------
	m_pControl = CObject2D::Create
	( // 引数
		control::POS,	// 位置
		control::SIZE,	// 大きさ
		VEC3_ZERO,		// 向き
		XCOL_AWHITE		// 色
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

	// 自動描画をOFFにする
	m_pControl->SetEnableDraw(false);

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

	// タイトルの終了
	m_pTitle->Uninit();

	// 勝利ポイント背景の終了
	m_pWinPointBG->Uninit();

	// 勝利ポイントの終了
	m_pWinPoint->Uninit();

	// 操作の終了
	m_pControl->Uninit();

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		// プレイヤーナンバーの終了
		m_apNumber[nCntEntry]->Uninit();

		// プレイヤー勝利ポイントの終了
		m_apPlayerWinPoint[nCntEntry]->Uninit();

		// プレイヤーフレームの終了
		m_apFrame[nCntEntry]->Uninit();
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CMiddleResultManager::Update(void)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_FADEIN:

		// フェードインの更新
		UpdateFadeIn();

		break;

	case STATE_FADEIN_WAIT:

		// フェードイン待機の更新
		UpdateFadeInWait();

		break;

	case STATE_FADEIN_ACCEL:

		// フェードイン加速の更新
		UpdateFadeInAccel();

		break;

	case STATE_RANK_TITLE_WAIT:

		// ランキングタイトル待機の更新
		UpdateRankTitleWait();

		break;

	case STATE_RANK_TITLE:

		// ランキングタイトル表示の更新
		UpdateRankTitle();

		break;

	case STATE_WINPOINT_WAIT:

		// 勝利ポイント待機の更新
		UpdateWinPointWait();

		break;

	case STATE_WINPOINT:

		// 勝利ポイント表示の更新
		UpdateWinPoint();

		break;

	case STATE_FRAME_WAIT:

		// プレイヤーフレーム待機の更新
		UpdateFrameWait();

		break;

	case STATE_FRAME:

		// プレイヤーフレーム表示の更新
		UpdateFrame();

		break;

	case STATE_PLAYERPOINT_WAIT:

		// プレイヤー勝利ポイント待機の更新
		UpdatePlayerPointWait();

		break;

	case STATE_PLAYERPOINT:

		// プレイヤー勝利ポイント表示の更新
		UpdatePlayerPoint();

		break;

	case STATE_WAIT:

		// 待機の更新
		UpdateWait();

		break;

	case STATE_FADEOUT:

		// フェードアウトの更新
		UpdateFadeOut();

		break;

	case STATE_FADEOUT_WAIT:

		// フェードアウト待機の更新
		UpdateFadeOutWait();

		break;

	case STATE_FADEOUT_ACCEL:

		// フェードアウト加速の更新
		UpdateFadeOutAccel();

		break;

	case STATE_END:

		if (CManager::GetInstance()->GetFade()->GetState() == CFade::FADE_NONE)
		{ // フェード中ではない場合

			//if ()
			{ // 勝者が決まっていない場合

				// ゲームに遷移
				CManager::GetInstance()->SetScene(CScene::MODE_GAME, WAIT_FRAME);
			}
			//else
			//{ // 勝者が決まった場合

			//	// リザルトに遷移
			//	CManager::GetInstance()->SetScene(CScene::MODE_RESULT, WAIT_FRAME);
			//}
		}

		break;

	default:
		assert(false);
		break;
	}

	// 遷移決定の更新
	UpdateTransition();

	// フェードの更新
	m_pFade->Update();

	// タイトルの更新
	m_pTitle->Update();

	// 勝利ポイント背景の更新
	m_pWinPointBG->Update();

	// 勝利ポイントの更新
	m_pWinPoint->Update();

	// 操作の更新
	m_pControl->Update();

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		// プレイヤーナンバーの更新
		m_apNumber[nCntEntry]->Update();

		// プレイヤー勝利ポイントの更新
		m_apPlayerWinPoint[nCntEntry]->Update();

		// プレイヤーフレームの更新
		m_apFrame[nCntEntry]->Update();
	}
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

//============================================================
//	フェードインの更新処理
//============================================================
void CMiddleResultManager::UpdateFadeIn(void)
{
	// 変数を宣言
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// フェード位置

	// 移動量を加算
	m_fMoveY += fade::ADD_MOVE;

	// フェードの位置を加算
	posFade.y += m_fMoveY;

	if (posFade.y > fade::UP_MIDDLE_POS.y)
	{ // 現在位置が停止位置を超えた場合

		// フェードを停止位置に補正
		posFade.y = fade::UP_MIDDLE_POS.y;

		// 移動量を初期化
		m_fMoveY = 0.0f;

		// フェードイン待機状態にする
		m_state = STATE_FADEIN_WAIT;
	}

	// フェード位置を反映
	m_pFade->SetVec3Position(posFade);
}

//============================================================
//	フェードイン待機の更新処理
//============================================================
void CMiddleResultManager::UpdateFadeInWait(void)
{
	// カウンターを加算
	m_nCounterState++;

	if (m_nCounterState >= fade::WAIT_FRAME)
	{ // 待機が終了した場合

		// カウンターを初期化
		m_nCounterState = 0;

		// フェードイン加速状態にする
		m_state = STATE_FADEIN_ACCEL;
	}
}

//============================================================
//	フェードイン加速の更新処理
//============================================================
void CMiddleResultManager::UpdateFadeInAccel(void)
{
	// 変数を宣言
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// フェード位置

	// 移動量を加算
	m_fMoveY += fade::ADD_ACCEL_MOVE;

	// フェードの位置を加算
	posFade.y += m_fMoveY;

	if (posFade.y > fade::CENT_POS.y)
	{ // 現在位置が停止位置を超えた場合

		// フェードを停止位置に補正
		posFade.y = fade::CENT_POS.y;

		// 移動量を初期化
		m_fMoveY = 0.0f;

		// ランキングタイトル待機状態にする
		m_state = STATE_RANK_TITLE_WAIT;
	}

	// フェード位置を反映
	m_pFade->SetVec3Position(posFade);
}

//============================================================
//	ランキングタイトル待機の更新処理
//============================================================
void CMiddleResultManager::UpdateRankTitleWait(void)
{
	// カウンターを加算
	m_nCounterState++;

	if (m_nCounterState >= title::WAIT_FRAME)
	{ // 待機が終了した場合

		// カウンターを初期化
		m_nCounterState = 0;

		// ランキングタイトル表示状態にする
		m_state = STATE_RANK_TITLE;

		// ランキングタイトルの自動描画をONにする
		m_pTitle->SetEnableDraw(true);

		// ランキングタイトルの拡大率を設定
		m_fScale = title::INIT_SCALE;
	}
}

//============================================================
//	ランキングタイトル表示の更新処理
//============================================================
void CMiddleResultManager::UpdateRankTitle(void)
{
	// 拡大率を加算
	m_fScale += title::ADD_SCALE;

	if (m_fScale < title::SET_SCALE)
	{ // まだ大きくなる場合

		// ランキングタイトル大きさを反映
		m_pTitle->SetVec3Sizing(title::SIZE * m_fScale);
	}
	else
	{ // 大きくなり切った場合

		// ランキングタイトル大きさを反映
		m_pTitle->SetVec3Sizing(title::SIZE);

		// 拡大率を初期化
		m_fScale = 1.0f;

		// 勝利ポイント待機状態にする
		m_state = STATE_WINPOINT_WAIT;
	}
}

//============================================================
//	勝利ポイント待機の更新処理
//============================================================
void CMiddleResultManager::UpdateWinPointWait(void)
{
	// カウンターを加算
	m_nCounterState++;

	if (m_nCounterState >= winBG::WAIT_FRAME)
	{ // 待機が終了した場合

		// カウンターを初期化
		m_nCounterState = 0;

		// 勝利ポイント表示状態にする
		m_state = STATE_WINPOINT;

		// 勝利ポイント背景の自動描画をONにする
		m_pWinPointBG->SetEnableDraw(true);

		// 勝利ポイント背景の拡大率を設定
		m_fScale = winBG::INIT_SCALE;
	}
}

//============================================================
//	勝利ポイント表示の更新処理
//============================================================
void CMiddleResultManager::UpdateWinPoint(void)
{
	// 拡大率を加算
	m_fScale += winBG::ADD_SCALE;

	if (m_fScale < winBG::SET_SCALE)
	{ // まだ大きくなる場合

		// 勝利ポイント背景の大きさを反映
		m_pWinPointBG->SetVec3Sizing(winBG::SIZE * m_fScale);
	}
	else
	{ // 大きくなり切った場合

		// 勝利ポイント背景の大きさを反映
		m_pWinPointBG->SetVec3Sizing(winBG::SIZE);

		// 拡大率を初期化
		m_fScale = 1.0f;

		// 勝利ポイント表示の自動描画をONにする
		m_pWinPoint->SetEnableDraw(true);

		// プレイヤーフレーム待機状態にする
		m_state = STATE_FRAME_WAIT;
	}
}

//============================================================
//	プレイヤーフレーム待機の更新処理
//============================================================
void CMiddleResultManager::UpdateFrameWait(void)
{
	// カウンターを加算
	m_nCounterState++;

	if (m_nCounterState >= frame::WAIT_FRAME)
	{ // 待機が終了した場合

		// カウンターを初期化
		m_nCounterState = 0;

		// プレイヤーフレーム表示状態にする
		m_state = STATE_FRAME;

		// プレイヤーフレームの拡大率を設定
		m_fScale = frame::INIT_SCALE;

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // プレイヤーの最大数分繰り返す

			// プレイヤーフレームの自動描画をONにする
			m_apFrame[nCntEntry]->SetEnableDraw(true);
		}
	}
}

//============================================================
//	プレイヤーフレーム表示の更新処理
//============================================================
void CMiddleResultManager::UpdateFrame(void)
{
	// 拡大率を加算
	m_fScale += frame::ADD_SCALE;

	if (m_fScale < frame::SET_SCALE)
	{ // まだ大きくなる場合

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // プレイヤーの最大数分繰り返す

			// プレイヤーフレームの大きさを反映
			m_apFrame[nCntEntry]->SetVec3Sizing(frame::SIZE * m_fScale);
		}
	}
	else
	{ // 大きくなり切った場合

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // プレイヤーの最大数分繰り返す

			// プレイヤーフレームの大きさを反映
			m_apFrame[nCntEntry]->SetVec3Sizing(frame::SIZE);
		}

		// 拡大率を初期化
		m_fScale = 1.0f;

		// プレイヤー勝利ポイント待機状態にする
		m_state = STATE_PLAYERPOINT_WAIT;
	}
}

//============================================================
//	プレイヤー勝利ポイント待機の更新処理
//============================================================
void CMiddleResultManager::UpdatePlayerPointWait(void)
{
	// カウンターを加算
	m_nCounterState++;

	if (m_nCounterState >= playerPoint::WAIT_FRAME)
	{ // 待機が終了した場合

		// カウンターを初期化
		m_nCounterState = 0;

		// プレイヤー勝利ポイント表示状態にする
		m_state = STATE_PLAYERPOINT;

		// プレイヤー勝利ポイントの拡大率を設定
		m_fScale = playerPoint::INIT_SCALE;

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // プレイヤーの最大数分繰り返す

			// プレイヤー勝利ポイントの自動描画をONにする
			m_apPlayerWinPoint[nCntEntry]->SetEnableDraw(true);

			// プレイヤーナンバーの自動描画をONにする
			m_apNumber[nCntEntry]->SetEnableDraw(true);
		}
	}
}

//============================================================
//	プレイヤー勝利ポイント表示の更新処理
//============================================================
void CMiddleResultManager::UpdatePlayerPoint(void)
{
	// 拡大率を加算
	m_fScale += playerPoint::ADD_SCALE;

	if (m_fScale < playerPoint::SET_SCALE)
	{ // まだ大きくなる場合

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // プレイヤーの最大数分繰り返す

			// プレイヤーナンバー・勝利ポイントの大きさを反映
			m_apNumber[nCntEntry]->SetScalingTitle(number::SIZE_TITLE * m_fScale);
			m_apNumber[nCntEntry]->GetMultiValue()->SetVec3Sizing(number::SIZE_VALUE * m_fScale);
			m_apPlayerWinPoint[nCntEntry]->SetScalingTitle(playerPoint::SIZE_TITLE * m_fScale);
			m_apPlayerWinPoint[nCntEntry]->GetMultiValue()->SetVec3Sizing(playerPoint::SIZE_VALUE * m_fScale);
		}
	}
	else
	{ // 大きくなり切った場合

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // プレイヤーの最大数分繰り返す

			// プレイヤーナンバー・勝利ポイントの大きさを反映
			m_apNumber[nCntEntry]->SetScalingTitle(number::SIZE_TITLE);
			m_apNumber[nCntEntry]->GetMultiValue()->SetVec3Sizing(number::SIZE_VALUE);
			m_apPlayerWinPoint[nCntEntry]->SetScalingTitle(playerPoint::SIZE_TITLE);
			m_apPlayerWinPoint[nCntEntry]->GetMultiValue()->SetVec3Sizing(playerPoint::SIZE_VALUE);
		}

		// 拡大率を初期化
		m_fScale = 1.0f;

		// 操作の自動描画をONにする
		m_pControl->SetEnableDraw(true);

		// 待機状態にする
		m_state = STATE_WAIT;
	}
}

//============================================================
//	待機の更新処理
//============================================================
void CMiddleResultManager::UpdateWait(void)
{
	// 変数を宣言
	D3DXCOLOR colControl = m_pControl->GetColor();	// 操作表示色

	if (colControl.a < control::MIN_COL.a)
	{ // 透明度が最低限より低い場合

		// 透明度を加算
		colControl.a += control::ADD_ALPHA;

		if (colControl.a > control::MIN_COL.a)
		{ // 透明度が超過した場合

			// 透明度を補正
			colControl.a = control::MIN_COL.a;
		}

		// 操作表示色を設定
		m_pControl->SetColor(colControl);
	}
	else
	{ // 透明度が最低限以上の場合

		// 変数を宣言
		float fAddAlpha = 0.0f;	// 透明度の加算量

		// 透明度を上げる
		m_fSinAlpha += control::ADD_SINROT;
		useful::NormalizeRot(m_fSinAlpha);	// 向き正規化

		// 透明度加算量を求める
		fAddAlpha = (control::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinAlpha) - 1.0f);

		// 操作表示色を設定
		m_pControl->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, control::BASIC_ALPHA + fAddAlpha));
	}
}

//============================================================
//	フェードアウトの更新処理
//============================================================
void CMiddleResultManager::UpdateFadeOut(void)
{
	// 変数を宣言
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// フェード位置

	// 移動量を加算
	m_fMoveY += fade::ADD_MOVE;

	// フェードの位置を加算
	posFade.y += m_fMoveY;

	if (posFade.y > fade::DOWN_MIDDLE_POS.y)
	{ // 現在位置が停止位置を超えた場合

		// 全位置の移動
		MovePositionAll(D3DXVECTOR3(0.0f, posFade.y - fade::DOWN_POS.y, 0.0f));

		// フェードを停止位置に補正
		posFade.y = fade::DOWN_MIDDLE_POS.y;

		// 移動量を初期化
		m_fMoveY = 0.0f;

		// フェードアウト待機状態にする
		m_state = STATE_FADEOUT_WAIT;
	}
	else
	{ // 現在位置が停止位置を超えていない場合

		// 全位置の移動
		MovePositionAll(D3DXVECTOR3(0.0f, m_fMoveY, 0.0f));
	}

	// フェード位置を反映
	m_pFade->SetVec3Position(posFade);
}

//============================================================
//	フェードアウト待機の更新処理
//============================================================
void CMiddleResultManager::UpdateFadeOutWait(void)
{
	// カウンターを加算
	m_nCounterState++;

	if (m_nCounterState >= fade::WAIT_FRAME)
	{ // 待機が終了した場合

		// カウンターを初期化
		m_nCounterState = 0;

		// フェードアウト加速状態にする
		m_state = STATE_FADEOUT_ACCEL;
	}
}

//============================================================
//	フェードアウト加速の更新処理
//============================================================
void CMiddleResultManager::UpdateFadeOutAccel(void)
{
	// 変数を宣言
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// フェード位置

	// 移動量を加算
	m_fMoveY += fade::ADD_ACCEL_MOVE;

	// フェードの位置を加算
	posFade.y += m_fMoveY;

	if (posFade.y > fade::DOWN_POS.y)
	{ // 現在位置が停止位置を超えた場合

		// 全位置の移動
		MovePositionAll(D3DXVECTOR3(0.0f, posFade.y - fade::DOWN_POS.y, 0.0f));

		// フェードを停止位置に補正
		posFade.y = fade::DOWN_POS.y;

		// 移動量を初期化
		m_fMoveY = 0.0f;

		// 終了状態にする
		m_state = STATE_END;
	}
	else
	{ // 現在位置が停止位置を超えていない場合

		// 全位置の移動
		MovePositionAll(D3DXVECTOR3(0.0f, m_fMoveY, 0.0f));
	}

	// フェード位置を反映
	m_pFade->SetVec3Position(posFade);
}

//============================================================
//	遷移決定の更新処理
//============================================================
void CMiddleResultManager::UpdateTransition(void)
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

			if (m_state != STATE_FADEOUT		// フェードアウト状態
			&&  m_state != STATE_FADEOUT_WAIT	// フェードアウト待機状態
			&&  m_state != STATE_FADEOUT_ACCEL	// フェードアウト加速状態
			&&  m_state != STATE_END)			// 終了状態
			{ // 上記の状態ではない場合

				// 演出スキップ
				SkipStaging();

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
			}
		}
		else
		{ // 遷移待機状態の場合

			// 操作の自動描画をOFFにする
			m_pControl->SetEnableDraw(false);

			// フェードアウト状態にする
			m_state = STATE_FADEOUT;
		}
	}
}

//============================================================
//	演出スキップ処理
//============================================================
void CMiddleResultManager::SkipStaging(void)
{
	// 情報を初期化
	m_fMoveY = 0.0f;	// 移動量
	m_fScale = 1.0f;	// 拡大率

	// 自動描画をONにする
	m_pTitle->SetEnableDraw(true);		// ランキングタイトル
	m_pWinPointBG->SetEnableDraw(true);	// 勝利ポイント背景
	m_pWinPoint->SetEnableDraw(true);	// 勝利ポイント表示
	m_pControl->SetEnableDraw(true);	// 操作

	// フェード位置を反映
	m_pFade->SetVec3Position(D3DXVECTOR3(fade::CENT_POS.x, fade::CENT_POS.y, 0.0f));

	// ランキングタイトル大きさを反映
	m_pTitle->SetVec3Sizing(title::SIZE);

	// 勝利ポイント背景の大きさを反映
	m_pWinPointBG->SetVec3Sizing(winBG::SIZE);

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		// 自動描画をONにする
		m_apFrame[nCntEntry]->SetEnableDraw(true);			// プレイヤーフレーム
		m_apPlayerWinPoint[nCntEntry]->SetEnableDraw(true);	// プレイヤー勝利ポイント
		m_apNumber[nCntEntry]->SetEnableDraw(true);			// プレイヤーナンバー

		// プレイヤーフレームの大きさを反映
		m_apFrame[nCntEntry]->SetVec3Sizing(frame::SIZE);

		// プレイヤーナンバー・勝利ポイントの大きさを反映
		m_apNumber[nCntEntry]->SetScalingTitle(number::SIZE_TITLE);
		m_apNumber[nCntEntry]->GetMultiValue()->SetVec3Sizing(number::SIZE_VALUE);
		m_apPlayerWinPoint[nCntEntry]->SetScalingTitle(playerPoint::SIZE_TITLE);
		m_apPlayerWinPoint[nCntEntry]->GetMultiValue()->SetVec3Sizing(playerPoint::SIZE_VALUE);
	}

	// 状態を変更
	m_state = STATE_WAIT;	// 遷移待機状態
}

//============================================================
//	全位置の移動処理
//============================================================
void CMiddleResultManager::MovePositionAll(const D3DXVECTOR3 & rAddMove)
{
	// フェードの位置の移動
	m_pFade->SetVec3Position(m_pFade->GetVec3Position() + rAddMove);

	// タイトルの位置の移動
	m_pTitle->SetVec3Position(m_pTitle->GetVec3Position() + rAddMove);

	// 勝利ポイント背景の位置の移動
	m_pWinPointBG->SetVec3Position(m_pWinPointBG->GetVec3Position() + rAddMove);

	// 勝利ポイントの位置の移動
	m_pWinPoint->SetVec3Position(m_pWinPoint->GetVec3Position() + rAddMove);

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		// プレイヤーナンバーの位置の移動
		m_apNumber[nCntEntry]->SetVec3Position(m_apNumber[nCntEntry]->GetVec3Position() + rAddMove);

		// プレイヤー勝利ポイントの位置の移動
		m_apPlayerWinPoint[nCntEntry]->SetVec3Position(m_apPlayerWinPoint[nCntEntry]->GetVec3Position() + rAddMove);

		// プレイヤーフレームの位置の移動
		m_apFrame[nCntEntry]->SetVec3Position(m_apFrame[nCntEntry]->GetVec3Position() + rAddMove);
	}
}
