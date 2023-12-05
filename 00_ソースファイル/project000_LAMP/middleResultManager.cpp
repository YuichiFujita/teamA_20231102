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

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 14;	// 中間リザルトの優先順位

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
		const float	ADD_MOVE	= 0.4f;		// フェード移動量
		const float	ADD_ACCEL_MOVE = 1.75f;	// フェード加速移動量
	}

	// ランキングタイトル基本情報
	namespace title
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(280.0f, 100.0f, 0.0f);	// タイトル位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(400.0f, 130.0f, 0.0f);	// タイトル大きさ

		const int	WAIT_FRAME	= 24;		// タイトル待機フレーム
		const float	INIT_SCALE	= 0.025f;	// タイトル初期拡大率
		const float	ADD_SCALE	= 0.09f;	// タイトル加算拡大率
		const float	SET_SCALE	= 1.0f;		// タイトル設定拡大率
	}

	// 勝利ポイント基本情報
	namespace win
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(760.0f, 100.0f, 0.0f);			// 位置
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(230.0f, 0.0f, 0.0f);				// 行間
		const D3DXVECTOR3 SPACE_VALUE	= D3DXVECTOR3(80.0f, 0.0f, 0.0f);		// 数字行間
		const D3DXVECTOR3 SIZE_TITLE	= D3DXVECTOR3(300.0f, 100.0f, 0.0f);	// タイトル大きさ
		const D3DXVECTOR3 SIZE_VALUE	= D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// 数字大きさ

		const float	DIGIT = 2;	// 桁数
	}

	// 勝利ポイント背景基本情報
	namespace winBG
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(860.0f, 100.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(720.0f, 145.0f, 0.0f);	// 大きさ

		const int	WAIT_FRAME	= 24;		// タイトル待機フレーム
		const float	INIT_SCALE	= 0.025f;	// タイトル初期拡大率
		const float	ADD_SCALE	= 0.09f;	// タイトル加算拡大率
		const float	SET_SCALE	= 1.0f;		// タイトル設定拡大率
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CMiddleResultManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\middleResult000.png",	// フェードテクスチャ
	NULL,	// ランキングタイトルテクスチャ
	"data\\TEXTURE\\bg.png",	// 勝利ポイント背景テクスチャ
	NULL,	// 勝利ポイントタイトルテクスチャ
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
	m_pFade			= NULL;	// フェードの情報
	m_pTitle		= NULL;	// タイトルの情報
	m_pWinPointBG	= NULL;	// 勝利ポイントの背景情報
	m_pWinPoint		= NULL;	// 勝利ポイントの情報

	m_state		= STATE_FADEIN;	// 状態
	m_fMoveY	= 0.0f;			// 縦移動量
	m_fScale	= 0.0f;			// 拡大率
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
	m_pFade			= NULL;	// フェードの情報
	m_pTitle		= NULL;	// タイトルの情報
	m_pWinPointBG	= NULL;	// 勝利ポイントの背景情報
	m_pWinPoint		= NULL;	// 勝利ポイントの情報

	m_state		= STATE_FADEIN;	// 状態
	m_fMoveY	= 0.0f;			// 縦移動量
	m_fScale	= 1.0f;			// 拡大率
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

	case STATE_WAIT:

		//// TODO：仮の処理
		//m_nCounterState++;
		//if (m_nCounterState == 60)
		//{
		//	m_nCounterState = 0;
		//	m_state = STATE_FADEOUT;
		//}

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

		//if ()
		{ // 勝者が決まっていない場合

			// ゲームに遷移
			CManager::GetInstance()->SetScene(CScene::MODE_GAME);
		}
		//else
		//{ // 勝者が決まった場合

		//	// リザルトに遷移
		//	CManager::GetInstance()->SetScene(CScene::MODE_RESULT);
		//}

		break;

	default:
		assert(false);
		break;
	}

	// フェードの更新
	m_pFade->Update();

	// タイトルの更新
	m_pTitle->Update();

	// 勝利ポイント背景の更新
	m_pWinPointBG->Update();

	// 勝利ポイントの更新
	m_pWinPoint->Update();
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

		// 待機状態にする
		m_state = STATE_WAIT;
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

		// フェードを停止位置に補正
		posFade.y = fade::DOWN_MIDDLE_POS.y;

		// 移動量を初期化
		m_fMoveY = 0.0f;

		// フェードアウト待機状態にする
		m_state = STATE_FADEOUT_WAIT;
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

		// フェードを停止位置に補正
		posFade.y = fade::DOWN_POS.y;

		// 移動量を初期化
		m_fMoveY = 0.0f;

		// 終了状態にする
		m_state = STATE_END;
	}

	// フェード位置を反映
	m_pFade->SetVec3Position(posFade);
}
