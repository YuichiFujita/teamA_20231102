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

		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(SCREEN_SIZE.x, 1770.0f, 0.0f);	// フェード大きさ

		const int	WAIT_FRAME	= 8;		// フェード待機フレーム
		const float	ADD_MOVE	= 0.4f;		// フェード移動量
		const float	ADD_ACCEL_MOVE = 1.75f;	// フェード加速移動量
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CMiddleResultManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\middleResult000.png",	// 操作説明テクスチャ
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
	m_pFade		= NULL;			// フェードの情報
	m_state		= STATE_FADEIN;	// 状態
	m_fMoveY	= 0.0f;			// 縦移動量
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
	// メンバ変数を初期化
	m_pFade		= NULL;			// フェードの情報
	m_state		= STATE_FADEIN;	// 状態
	m_fMoveY	= 0.0f;			// 縦移動量
	m_nCounterState = 0;		// 状態管理カウンター

	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		fade::UP_POS,	// 位置
		fade::SIZE		// 大きさ
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

	case STATE_WAIT:

		// TODO：仮の処理
		m_nCounterState++;
		if (m_nCounterState == 60)
		{
			m_nCounterState = 0;
			m_state = STATE_FADEOUT;
		}

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
		break;

	default:
		assert(false);
		break;
	}

	// フェードの更新
	m_pFade->Update();
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

		// 待機状態にする
		m_state = STATE_WAIT;
	}

	// フェード位置を反映
	m_pFade->SetVec3Position(posFade);
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
