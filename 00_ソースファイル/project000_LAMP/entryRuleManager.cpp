//============================================================
//
//	エントリールールマネージャー処理 [entryRuleManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "entryRuleManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"
#include "multiValue.h"
#include "retentionManager.h"
#include "fade.h"
#include "sceneEntry.h"
#include "entryManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY	= 14;	// エントリーの優先順位
	const int	MAX_SELECT	= CEntryRuleManager::RULE_MAX + 1;	// ルール選択の総数 (完了操作も含む)
	const int	SELECT_GAME = CEntryRuleManager::RULE_MAX;		// ゲーム遷移選択番号

	// フェード情報
	namespace fade
	{
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 初期化色
		const D3DXCOLOR SET_COL		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);	// 設定色

		const float	ADD_ALPHA	= 0.025f;	// 透明度の加算量
		const float	SUB_ALPHA	= 0.025f;	// 透明度の減算量
	}

	// ルールタイトル情報
	namespace rule
	{
		const POSGRID2		PART	= POSGRID2(1, CEntryRuleManager::RULE_MAX);	// テクスチャ分割数
		const D3DXVECTOR3	POS		= D3DXVECTOR3(400.0f, 120.0f, 0.0f);		// 位置
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(300.0f, 100.0f, 0.0f);		// 大きさ
		const D3DXVECTOR3	SPACE	= D3DXVECTOR3(0.0f, 140.0f, 0.0f);			// 空白
	}

	// 開始ボタン情報
	namespace start
	{
		const D3DXVECTOR3	POS		= D3DXVECTOR3(SCREEN_CENT.x, 580.0f, 0.0f);	// 位置
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(440.0f, 140.0f, 0.0f);		// 大きさ
	}

	// 勝利ポイント数情報
	namespace winpoint
	{
		const D3DXVECTOR3	POS		= D3DXVECTOR3(830.0f, 120.0f, 0.0f);	// 位置
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3	SPACE	= D3DXVECTOR3(100.0f, 0.0f, 0.0f);		// 行間

		const int	DIGIT	= 2;	// 桁数
	}

	// 撃破条件情報
	namespace kill
	{
		const POSGRID2		PART	= POSGRID2(1, CRetentionManager::KILL_MAX);						// テクスチャ分割数
		const D3DXVECTOR3	POS		= D3DXVECTOR3(880.0f, winpoint::POS.y + rule::SPACE.y, 0.0f);	// 位置
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(300.0f, 100.0f, 0.0f);							// 大きさ
	}

	// 勝利条件情報
	namespace win
	{
		const POSGRID2		PART	= POSGRID2(1, CRetentionManager::WIN_MAX);								// テクスチャ分割数
		const D3DXVECTOR3	POS		= D3DXVECTOR3(880.0f, winpoint::POS.y + (rule::SPACE.y * 2.0f), 0.0f);	// 位置
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(300.0f, 100.0f, 0.0f);									// 大きさ
	}

	// 選択情報
	namespace select
	{
		const float	ADD_SIZE = 15.0f;	// 大きさ加算量

		const D3DXVECTOR3	POS	= D3DXVECTOR3(880.0f, rule::POS.y, 0.0f);	// 位置
		const D3DXCOLOR		COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);		// 色

		const D3DXVECTOR3	SIZE_RULE	= D3DXVECTOR3(kill::SIZE.x + ADD_SIZE, kill::SIZE.y + ADD_SIZE, 0.0f);		// ルール選択時の大きさ
		const D3DXVECTOR3	SIZE_START	= D3DXVECTOR3(start::SIZE.x + ADD_SIZE, start::SIZE.y + ADD_SIZE, 0.0f);	// 開始選択時の大きさ

		const D3DXCOLOR CHOICE_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 選択中カラー
		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);	// 非選択中カラー
	}

	// 矢印情報
	namespace arrow
	{
		const float	ADD_ALPHA		= 0.02f;	// 透明度の加算量
		const float	ADD_SINROT		= 0.04f;	// 透明度ふわふわさせる際のサインカーブ向き加算量
		const float	MAX_ADD_ALPHA	= 0.25f;	// 透明度の最大加算量
		const float	BASIC_ALPHA		= 0.95f;	// 基準の透明度
		const float	SPACE_EDGE		= 50.0f;	// 縁の空白

		const POSGRID2		PART	= POSGRID2(1, MAX_ARROW);			// テクスチャ分割数
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(90.0f, 90.0f, 9.0f);	// 大きさ

		const D3DXVECTOR3	POS		= D3DXVECTOR3(select::POS.x - (select::SIZE_RULE.x * 0.5f) - SPACE_EDGE, select::POS.y, 0.0f);	// 位置
		const D3DXVECTOR3	SPACE	= D3DXVECTOR3(select::SIZE_RULE.x + (SPACE_EDGE * 2.0f), 0.0f, 0.0f);	// 空白
		const D3DXCOLOR		MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);	// 色
	}

	// 操作情報
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
const char *CEntryRuleManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\ruleTitle000.png",	// ルールタイトルテクスチャ
	"data\\TEXTURE\\rule000.png",		// 撃破条件テクスチャ
	"data\\TEXTURE\\rule001.png",		// 勝利条件テクスチャ
	NULL,								// 開始ボタンテクスチャ
	NULL,								// 操作表示テクスチャ
	NULL,								// 矢印テクスチャ
};

//************************************************************
//	親クラス [CEntryRuleManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEntryRuleManager::CEntryRuleManager()
{
	// メンバ変数をクリア
	memset(&m_apRuleTitle[0], 0, sizeof(m_apRuleTitle));	// ルールタイトルの情報
	memset(&m_apArrow[0], 0, sizeof(m_apArrow));			// 矢印の情報
	m_pWinPoint	= NULL;	// 勝利ポイント数の情報
	m_pKill		= NULL;	// 撃破条件の情報
	m_pWin		= NULL;	// 勝利条件の情報
	m_pSelect	= NULL;	// 選択の情報
	m_pStart	= NULL;	// 開始ボタンの情報
	m_pControl	= NULL;	// 操作表示の情報
	m_pFade		= NULL;	// フェードの情報
	m_fSinControlAlpha	= 0.0f;			// 操作表示の透明向き
	m_fSinArrowAlpha	= 0.0f;			// 矢印表示の透明向き
	m_state			= STATE_INIT;		// 状態
	m_nSelect		= RULE_WINPOINT;	// 現在の選択
	m_nOldSelect	= RULE_WINPOINT;	// 前回の選択
}

//============================================================
//	デストラクタ
//============================================================
CEntryRuleManager::~CEntryRuleManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEntryRuleManager::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apRuleTitle[0], 0, sizeof(m_apRuleTitle));	// ルールタイトルの情報
	memset(&m_apArrow[0], 0, sizeof(m_apArrow));			// 矢印の情報
	m_pWinPoint	= NULL;	// 勝利ポイント数の情報
	m_pKill		= NULL;	// 撃破条件の情報
	m_pWin		= NULL;	// 勝利条件の情報
	m_pSelect	= NULL;	// 選択の情報
	m_pStart	= NULL;	// 開始ボタンの情報
	m_pControl	= NULL;	// 操作表示の情報
	m_pFade		= NULL;	// フェードの情報
	m_fSinControlAlpha	= -HALF_PI;		// 操作表示の透明向き
	m_fSinArrowAlpha	= -HALF_PI;		// 矢印表示の透明向き
	m_state			= STATE_INIT;		// 状態
	m_nSelect		= RULE_WINPOINT;	// 現在の選択
	m_nOldSelect	= RULE_WINPOINT;	// 前回の選択

	// フェードの生成
	{
		m_pFade = CObject2D::Create
		( // 引数
			SCREEN_CENT,	// 位置
			SCREEN_SIZE,	// 大きさ
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
	}

	// ルールタイトルの生成
	{
		for (int i = 0; i < RULE_MAX; i++)
		{ // ルールの総数分繰り返す

			// ルールタイトルの生成
			m_apRuleTitle[i] = CAnim2D::Create
			( // 引数
				rule::PART.x,	// テクスチャ横分割数
				rule::PART.y,	// テクスチャ縦分割数
				rule::POS + ((float)i * rule::SPACE),	// 位置
				rule::SIZE								// 大きさ
			);
			if (m_apRuleTitle[i] == NULL)
			{ // 生成に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// テクスチャを登録・割当
			m_apRuleTitle[i]->BindTexture(mc_apTextureFile[TEXTURE_RULE_TITLE]);

			// 優先順位を設定
			m_apRuleTitle[i]->SetPriority(PRIORITY);

			// パターンを設定
			m_apRuleTitle[i]->SetPattern(i);
		}
	}

	// 矢印の生成
	{
		for (int i = 0; i < MAX_ARROW; i++)
		{ // 矢印の総数分繰り返す

			// 矢印の生成
			m_apArrow[i] = CAnim2D::Create
			( // 引数
				arrow::PART.x,	// テクスチャ横分割数
				arrow::PART.y,	// テクスチャ縦分割数
				arrow::POS + ((float)i * arrow::SPACE),	// 位置
				arrow::SIZE,	// 大きさ
				VEC3_ZERO,		// 向き
				XCOL_AWHITE		// 色
			);
			if (m_apArrow[i] == NULL)
			{ // 生成に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// テクスチャを登録・割当
			m_apArrow[i]->BindTexture(mc_apTextureFile[TEXTURE_ARROW]);

			// 優先順位を設定
			m_apArrow[i]->SetPriority(PRIORITY);

			// パターンを設定
			m_apArrow[i]->SetPattern(i);
		}
	}

	// 選択の生成
	{
		m_pSelect = CObject2D::Create
		( // 引数
			select::POS,		// 位置
			select::SIZE_RULE,	// 大きさ
			VEC3_ZERO,			// 向き
			select::COL			// 色
		);
		if (m_pSelect == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_pSelect->SetPriority(PRIORITY);
	}

	// 開始ボタンの生成
	{
		m_pStart = CObject2D::Create
		( // 引数
			start::POS,			// 位置
			start::SIZE,		// 大きさ
			VEC3_ZERO,			// 向き
			select::DEFAULT_COL	// 色
		);
		if (m_pStart == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_pStart->BindTexture(mc_apTextureFile[TEXTURE_START]);

		// 優先順位を設定
		m_pStart->SetPriority(PRIORITY);
	}

	// 操作表示の生成
	{
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
		m_pStart->BindTexture(mc_apTextureFile[TEXTURE_CONTROL]);

		// 優先順位を設定
		m_pControl->SetPriority(PRIORITY);
	}

	// 勝利ポイント数の生成
	{
		m_pWinPoint = CMultiValue::Create
		( // 引数
			CValue::TEXTURE_NORMAL,	// テクスチャ
			0,						// 数字
			winpoint::DIGIT,		// 桁数
			winpoint::POS,			// 位置
			winpoint::SIZE,			// 大きさ
			winpoint::SPACE,		// 行間
			VEC3_ZERO,				// 向き
			select::CHOICE_COL		// 色
		);
		if (m_pWinPoint == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_pWinPoint->SetPriority(PRIORITY);

		// 最大値を設定
		m_pWinPoint->SetMax(MAX_WINPOINT);

		// 最小値を設定
		m_pWinPoint->SetMin(MIN_WINPOINT);

		// 値を設定
		m_pWinPoint->SetNum(CManager::GetInstance()->GetRetentionManager()->GetWinPoint());
	}

	// 撃破条件の生成
	{
		m_pKill = CAnim2D::Create
		( // 引数
			kill::PART.x,		// テクスチャ横分割数
			kill::PART.y,		// テクスチャ縦分割数
			kill::POS,			// 位置
			kill::SIZE,			// 大きさ
			VEC3_ZERO,			// 向き
			select::DEFAULT_COL	// 色
		);
		if (m_pKill == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_pKill->BindTexture(mc_apTextureFile[TEXTURE_KILL]);

		// 優先順位を設定
		m_pKill->SetPriority(PRIORITY);

		// パターンを設定
		m_pKill->SetPattern(CManager::GetInstance()->GetRetentionManager()->GetKillState());
	}

	// 勝利条件の生成
	{
		m_pWin = CAnim2D::Create
		( // 引数
			win::PART.x,		// テクスチャ横分割数
			win::PART.y,		// テクスチャ縦分割数
			win::POS,			// 位置
			win::SIZE,			// 大きさ
			VEC3_ZERO,			// 向き
			select::DEFAULT_COL	// 色
		);
		if (m_pWin == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_pWin->BindTexture(mc_apTextureFile[TEXTURE_WIN]);

		// 優先順位を設定
		m_pWin->SetPriority(PRIORITY);

		// パターンを設定
		m_pWin->SetPattern(CManager::GetInstance()->GetRetentionManager()->GetWinState());
	}

	// UIの自動描画をOFFにする
	SetEnableUI(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CEntryRuleManager::Uninit(void)
{
	for (int i = 0; i < RULE_MAX; i++)
	{ // ルールの総数分繰り返す

		// ルールタイトルの終了
		m_apRuleTitle[i]->Uninit();
	}

	for (int i = 0; i < MAX_ARROW; i++)
	{ // 矢印の総数分繰り返す

		// 矢印の終了
		m_apArrow[i]->Uninit();
	}

	// 選択の終了
	m_pSelect->Uninit();

	// 開始ボタンの終了
	m_pStart->Uninit();

	// 操作表示の終了
	m_pControl->Uninit();

	// 勝利ポイント数の終了
	m_pWinPoint->Uninit();

	// 撃破条件の終了
	m_pKill->Uninit();

	// 勝利条件の終了
	m_pWin->Uninit();

	// フェードの終了
	m_pFade->Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CEntryRuleManager::Update(void)
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::FADE_NONE)
	{ // フェード中の場合

		// 処理を抜ける
		return;
	}

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_INIT:

		// UIの自動描画をONにする
		SetEnableUI(true);

		// フェードイン状態にする
		m_state = STATE_FADEIN;

		// 処理を抜けずにフェードインに移行

	case STATE_FADEIN:

		// フェードインの更新
		UpdateFadeIn();

		break;

	case STATE_RULESET:

		// 選択操作
		Select();

		// 決定操作
		Decide();

		// ルール変更操作
		ChangeRule();

		// 操作更新
		UpdateControl();

		// 矢印更新
		UpdateArrow();

		break;

	case STATE_FADEOUT:

		// フェードアウトの更新
		UpdateFadeOut();

		break;

	case STATE_ENTRYBACK:

		// UIの自動描画をOFFにする
		SetEnableUI(false);

		// エントリー状態に戻る
		CSceneEntry::GetEntryManager()->SetState(CEntryManager::STATE_ENTRY);

		// 関数を抜ける
		return;

	default:
		assert(false);
		break;
	}

	for (int i = 0; i < RULE_MAX; i++)
	{ // ルールの総数分繰り返す

		// ルールタイトルの更新
		m_apRuleTitle[i]->Update();
	}

	for (int i = 0; i < MAX_ARROW; i++)
	{ // 矢印の総数分繰り返す

		// 矢印の更新
		m_apArrow[i]->Update();
	}

	// 選択の更新
	m_pSelect->Update();

	// 開始ボタンの更新
	m_pStart->Update();

	// 操作表示の更新
	m_pControl->Update();

	// 勝利ポイント数の更新
	m_pWinPoint->Update();

	// 撃破条件の更新
	m_pKill->Update();

	// 勝利条件の更新
	m_pWin->Update();

	// フェードの更新
	m_pFade->Update();
}

//============================================================
//	生成処理
//============================================================
CEntryRuleManager *CEntryRuleManager::Create(void)
{
	// ポインタを宣言
	CEntryRuleManager *pEntryRuleManager = NULL;	// エントリールールマネージャー生成用

	if (pEntryRuleManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pEntryRuleManager = new CEntryRuleManager;	// エントリールールマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pEntryRuleManager != NULL)
	{ // 使用されている場合
		
		// エントリールールマネージャーの初期化
		if (FAILED(pEntryRuleManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pEntryRuleManager;
			pEntryRuleManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pEntryRuleManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CEntryRuleManager::Release(CEntryRuleManager *&prEntryRuleManager)
{
	if (prEntryRuleManager != NULL)
	{ // 使用中の場合

		// エントリールールマネージャーの終了
		if (FAILED(prEntryRuleManager->Uninit()))
		{ // 破棄に失敗した場合

			// メモリ開放
			delete prEntryRuleManager;
			prEntryRuleManager = NULL;

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prEntryRuleManager;
		prEntryRuleManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	フェードインの更新処理
//============================================================
void CEntryRuleManager::UpdateFadeIn(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	// 透明度を加算
	colFade.a += fade::ADD_ALPHA;

	if (colFade.a >= fade::SET_COL.a)
	{ // 透明度が上がりきった場合

		// 透明度を補正
		colFade.a = fade::SET_COL.a;

		// ルール設定状態にする
		m_state = STATE_RULESET;
	}

	// UIの透明度の設定
	SetAlphaUI(colFade.a * 2.0f, false);

	// 透明度を反映
	m_pFade->SetColor(colFade);
}

//============================================================
//	フェードアウトの更新処理
//============================================================
void CEntryRuleManager::UpdateFadeOut(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	// 透明度を減算
	colFade.a -= fade::SUB_ALPHA;

	if (colFade.a <= fade::INIT_COL.a)
	{ // 透明度が下がりきった場合

		// 透明度を補正
		colFade.a = fade::INIT_COL.a;

		// エントリー戻し状態にする
		m_state = STATE_ENTRYBACK;
	}

	// UIの透明度の設定
	SetAlphaUI(colFade.a * 2.0f, true);

	// 透明度を反映
	m_pFade->SetColor(colFade);
}

//============================================================
//	操作の更新処理
//============================================================
void CEntryRuleManager::UpdateControl(void)
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
		m_fSinControlAlpha += control::ADD_SINROT;
		useful::NormalizeRot(m_fSinControlAlpha);	// 向き正規化

		// 透明度加算量を求める
		fAddAlpha = (control::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinControlAlpha) - 1.0f);

		// 操作表示色を設定
		m_pControl->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, control::BASIC_ALPHA + fAddAlpha));
	}
}

//============================================================
//	矢印の更新処理
//============================================================
void CEntryRuleManager::UpdateArrow(void)
{
	for (int i = 0; i < MAX_ARROW; i++)
	{ // 矢印の総数分繰り返す

		// 変数を宣言
		D3DXCOLOR colArrow = m_apArrow[i]->GetColor();	// 矢印色

		if (colArrow.a < arrow::MIN_COL.a)
		{ // 透明度が最低限より低い場合

			// 透明度を加算
			colArrow.a += arrow::ADD_ALPHA;

			if (colArrow.a > arrow::MIN_COL.a)
			{ // 透明度が超過した場合

				// 透明度を補正
				colArrow.a = arrow::MIN_COL.a;
			}

			// 矢印色を設定
			m_apArrow[i]->SetColor(colArrow);
		}
		else
		{ // 透明度が最低限以上の場合

			// 変数を宣言
			float fAddAlpha = 0.0f;	// 透明度の加算量

			// 透明度を上げる
			m_fSinArrowAlpha += arrow::ADD_SINROT;
			useful::NormalizeRot(m_fSinArrowAlpha);	// 向き正規化

			// 透明度加算量を求める
			fAddAlpha = (arrow::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinArrowAlpha) - 1.0f);

			// 矢印色を設定
			m_apArrow[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, arrow::BASIC_ALPHA + fAddAlpha));
		}
	}
}

//============================================================
//	選択操作処理
//============================================================
void CEntryRuleManager::Select(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド

	CObject *apSelect[MAX_SELECT] =	// 選択オブジェクト
	{
		m_pWinPoint,	// 勝利ポイント
		m_pKill,		// 撃破条件
		m_pWin,			// 勝利条件
		m_pStart		// 開始ボタン
	};

	// 変数を宣言
	D3DXVECTOR3 posSelect = m_pSelect->GetVec3Position();	// 選択位置
	D3DXVECTOR3 sizeSelect = m_pSelect->GetVec3Sizing();	// 選択大きさ

	if (pKeyboard->IsTrigger(DIK_W)
	||  pKeyboard->IsTrigger(DIK_UP)
	||  pPad->IsTrigger(CInputPad::KEY_UP))
	{ // 上移動の操作が行われた場合

		// 上に選択をずらす
		m_nSelect = (m_nSelect + (MAX_SELECT - 1)) % MAX_SELECT;

		if (m_nSelect != SELECT_GAME)
		{ // 選択がゲーム遷移ではない場合

			// 位置を変更
			posSelect.x = select::POS.x;
			posSelect.y = select::POS.y + rule::SPACE.y * (float)m_nSelect;

			// 大きさを変更
			sizeSelect = select::SIZE_RULE;

			for (int i = 0; i < MAX_ARROW; i++)
			{ // 矢印の総数分繰り返す

				// 矢印の位置を変更
				m_apArrow[i]->SetVec3Position(D3DXVECTOR3(arrow::POS.x + (arrow::SPACE.x * (float)i), arrow::POS.y + rule::SPACE.y * (float)m_nSelect, 0.0f));

				// 自動描画をONにする
				m_apArrow[i]->SetEnableDraw(true);
			}
		}
		else
		{ // 選択がゲーム遷移の場合

			// 位置を変更
			posSelect = start::POS;

			// 大きさを変更
			sizeSelect = select::SIZE_START;

			for (int i = 0; i < MAX_ARROW; i++)
			{ // 矢印の総数分繰り返す

				// 自動描画をOFFにする
				m_apArrow[i]->SetEnableDraw(false);
			}
		}

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
	}
	if (pKeyboard->IsTrigger(DIK_S)
	||  pKeyboard->IsTrigger(DIK_DOWN)
	||  pPad->IsTrigger(CInputPad::KEY_DOWN))
	{ // 下移動の操作が行われた場合

		// 下に選択をずらす
		m_nSelect = (m_nSelect + 1) % MAX_SELECT;

		if (m_nSelect != SELECT_GAME)
		{ // 選択がゲーム遷移ではない場合

			// 位置を変更
			posSelect.x = select::POS.x;
			posSelect.y = select::POS.y + rule::SPACE.y * (float)m_nSelect;

			// 大きさを変更
			sizeSelect = select::SIZE_RULE;

			for (int i = 0; i < MAX_ARROW; i++)
			{ // 矢印の総数分繰り返す

				// 矢印の位置を変更
				m_apArrow[i]->SetVec3Position(D3DXVECTOR3(arrow::POS.x + (arrow::SPACE.x * (float)i), arrow::POS.y + rule::SPACE.y * (float)m_nSelect, 0.0f));

				// 自動描画をONにする
				m_apArrow[i]->SetEnableDraw(true);
			}
		}
		else
		{ // 選択がゲーム遷移の場合

			// 位置を変更
			posSelect = start::POS;

			// 大きさを変更
			sizeSelect = select::SIZE_START;

			for (int i = 0; i < MAX_ARROW; i++)
			{ // 矢印の総数分繰り返す

				// 自動描画をOFFにする
				m_apArrow[i]->SetEnableDraw(false);
			}
		}

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
	}

	// 選択情報デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[選択]：%d\n", m_nSelect);

	// 前回の選択要素の色を黒に設定
	apSelect[m_nOldSelect]->SetColor(select::DEFAULT_COL);

	// 現在の選択要素の色を白に設定
	apSelect[m_nSelect]->SetColor(select::CHOICE_COL);

	// 現在の選択要素を代入
	m_nOldSelect = m_nSelect;

	// 選択位置を反映
	m_pSelect->SetVec3Position(posSelect);

	// 選択大きさを反映
	m_pSelect->SetVec3Sizing(sizeSelect);
}

//============================================================
//	決定操作処理
//============================================================
void CEntryRuleManager::Decide(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド

	if (pKeyboard->IsTrigger(DIK_RETURN)
	||  pPad->IsTrigger(CInputPad::KEY_A) || pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X) || pPad->IsTrigger(CInputPad::KEY_Y))
	{ // 決定の操作が行われた場合

		switch (m_nSelect)
		{ // 選択ごとの処理
		case RULE_WINPOINT:
		case RULE_KILL:
		case RULE_WIN:
			break;

		case SELECT_GAME:

			// ツール変更反映
			SetRule();

			// 終了状態にする
			CSceneEntry::GetEntryManager()->SetState(CEntryManager::STATE_END);	// ゲームに遷移

			break;

		default:
			assert(false);
			break;
		}
	}

	if (pKeyboard->IsTrigger(DIK_SPACE))
	{ // 戻る操作が行われた場合

		// フェードアウト状態にする
		m_state = STATE_FADEOUT;
	}
}

//============================================================
//	ルール変更操作処理
//============================================================
void CEntryRuleManager::ChangeRule(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド

	// 変数を宣言
	int nKill = m_pKill->GetPattern();	// 撃破条件
	int nWin  = m_pWin->GetPattern();	// 勝利条件

	if (pKeyboard->IsTrigger(DIK_A)
	||  pKeyboard->IsTrigger(DIK_LEFT)
	||  pPad->IsTrigger(CInputPad::KEY_LEFT))
	{ // 左移動の操作が行われた場合

		switch (m_nSelect)
		{ // 選択ごとの処理
		case RULE_WINPOINT:

			// 勝利ポイントを減算
			m_pWinPoint->AddNum(-1);

			break;

		case RULE_KILL:

			// 左に選択をずらす
			nKill = (nKill + (CRetentionManager::KILL_MAX - 1)) % CRetentionManager::KILL_MAX;

			break;

		case RULE_WIN:

			// 左に選択をずらす
			nWin = (nWin + (CRetentionManager::WIN_MAX - 1)) % CRetentionManager::WIN_MAX;

			break;

		case SELECT_GAME:
			break;

		default:
			assert(false);
			break;
		}

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
	}
	if (pKeyboard->IsTrigger(DIK_D)
	||  pKeyboard->IsTrigger(DIK_RIGHT)
	||  pPad->IsTrigger(CInputPad::KEY_RIGHT))
	{ // 右移動の操作が行われた場合

		switch (m_nSelect)
		{ // 選択ごとの処理
		case RULE_WINPOINT:

			// 勝利ポイントを加算
			m_pWinPoint->AddNum(1);

			break;

		case RULE_KILL:

			// 右に選択をずらす
			nKill = (nKill + 1) % CRetentionManager::KILL_MAX;

			break;

		case RULE_WIN:

			// 右に選択をずらす
			nWin = (nWin + 1) % CRetentionManager::WIN_MAX;

			break;

		case SELECT_GAME:
			break;

		default:
			assert(false);
			break;
		}

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
	}

	// 撃破条件を反映
	m_pKill->SetPattern(nKill);

	// 勝利条件を反映
	m_pWin->SetPattern(nWin);
}

//============================================================
//	ルール変更反映処理
//============================================================
void CEntryRuleManager::SetRule(void)
{
	// ポインタを宣言
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// データ保存情報

	// 勝利ポイント数を設定
	pRetention->SetWinPoint(m_pWinPoint->GetNum());

	// 撃破条件を設定
	pRetention->SetKillState((CRetentionManager::EKill)m_pKill->GetPattern());

	// 勝利条件を設定
	pRetention->SetWinState((CRetentionManager::EWin)m_pWin->GetPattern());
}

//============================================================
//	UIの描画状況の設定処理
//============================================================
void CEntryRuleManager::SetEnableUI(const bool bDraw)
{
	for (int i = 0; i < RULE_MAX; i++)
	{ // ルールの総数分繰り返す

		// ルールタイトルの描画を設定
		m_apRuleTitle[i]->SetEnableDraw(bDraw);
	}

	for (int i = 0; i < MAX_ARROW; i++)
	{ // 矢印の総数分繰り返す

		// 矢印の描画を設定
		m_apArrow[i]->SetEnableDraw(bDraw);
	}

	// 選択の描画を設定
	m_pSelect->SetEnableDraw(bDraw);

	// 開始ボタンの描画を設定
	m_pStart->SetEnableDraw(bDraw);

	// 操作表示の描画を設定
	m_pControl->SetEnableDraw(bDraw);

	// 勝利ポイント数の描画を設定
	m_pWinPoint->SetEnableDraw(bDraw);

	// 撃破条件の描画を設定
	m_pKill->SetEnableDraw(bDraw);

	// 勝利条件の描画を設定
	m_pWin->SetEnableDraw(bDraw);
}

//============================================================
//	UIの透明度の設定処理
//============================================================
void CEntryRuleManager::SetAlphaUI(const float fAlpha, const bool bFadeOut)
{
	// 変数を宣言
	D3DXCOLOR col = XCOL_WHITE;	// 色

	for (int i = 0; i < RULE_MAX; i++)
	{ // ルールの総数分繰り返す

		// ルールタイトルの透明度を設定
		col = m_apRuleTitle[i]->GetColor();
		m_apRuleTitle[i]->SetColor(D3DXCOLOR(col.r, col.g, col.b, fAlpha));
	}

	// 開始ボタンの透明度を設定
	col = m_pStart->GetColor();
	m_pStart->SetColor(D3DXCOLOR(col.r, col.g, col.b, fAlpha));

	// 勝利ポイント数の透明度を設定
	col = m_pWinPoint->GetColor();
	m_pWinPoint->SetColor(D3DXCOLOR(col.r, col.g, col.b, fAlpha));

	// 撃破条件の透明度を設定
	col = m_pKill->GetColor();
	m_pKill->SetColor(D3DXCOLOR(col.r, col.g, col.b, fAlpha));

	// 勝利条件の透明度を設定
	col = m_pWin->GetColor();
	m_pWin->SetColor(D3DXCOLOR(col.r, col.g, col.b, fAlpha));

	if (bFadeOut)
	{ // フェードアウト時の場合

		// 変数を宣言
		float fSetAlpha = 0.0f;	// 設定透明度
		float fMaxAlpha = 0.0f;	// 最大透明度

		for (int i = 0; i < MAX_ARROW; i++)
		{ // 矢印の総数分繰り返す

			// 色情報を設定
			col = m_apArrow[i]->GetColor();	// 色
			fMaxAlpha = col.a;				// 最大透明度
			fSetAlpha = fAlpha;				// 設定透明度

			// 透明度を制限
			useful::LimitNum(fSetAlpha, 0.0f, fMaxAlpha);

			// 矢印の透明度を設定
			m_apArrow[i]->SetColor(D3DXCOLOR(col.r, col.g, col.b, fSetAlpha));
		}

		// 選択の透明度を設定
		{
			// 色情報を設定
			col = m_pSelect->GetColor();	// 色
			fMaxAlpha = col.a;				// 最大透明度
			fSetAlpha = fAlpha;				// 設定透明度

			// 透明度を制限
			useful::LimitNum(fSetAlpha, 0.0f, fMaxAlpha);

			// 選択の透明度を設定
			m_pSelect->SetColor(D3DXCOLOR(col.r, col.g, col.b, fSetAlpha));
		}

		// 操作表示の透明度を設定
		{
			// 色情報を設定
			col = m_pControl->GetColor();	// 色
			fMaxAlpha = col.a;				// 最大透明度
			fSetAlpha = fAlpha;				// 設定透明度

			// 透明度を制限
			useful::LimitNum(fSetAlpha, 0.0f, fMaxAlpha);

			// 操作表示の透明度を設定
			m_pControl->SetColor(D3DXCOLOR(col.r, col.g, col.b, fSetAlpha));
		}
	}
}
