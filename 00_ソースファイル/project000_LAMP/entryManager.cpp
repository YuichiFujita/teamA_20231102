//============================================================
//
//	エントリーマネージャー処理 [entryManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "entryManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "valueUI.h"
#include "object2D.h"
#include "multiValue.h"
#include "entryRuleManager.h"
#include "retentionManager.h"
#include "player.h"
#include "flail.h"
#include "fade.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 13;	// エントリーの優先順位

	const D3DXCOLOR COL_ENTRY	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 参加中カラー
	const D3DXCOLOR COL_UNENTRY	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// 非参加中カラー

	namespace number
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(165.0f, 60.0f, 0.0f);		// 位置
		const D3DXVECTOR3	SIZE_TITLE	= D3DXVECTOR3(242.0f, 107.0f, 0.0f);	// タイトル大きさ
		const D3DXVECTOR3	SIZE_VALUE	= D3DXVECTOR3(80.0f, 90.0f, 0.0f);		// 数字大きさ
		const D3DXVECTOR3	SPACE_POS	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// 数字UI同士の空白
		const D3DXVECTOR3	SPACE_TITLE	= D3DXVECTOR3(100.0f, 5.0f, 0.0f);		// タイトル空白
		const D3DXVECTOR3	SPACE_VALUE	= VEC3_ZERO;							// 数字空白
		const int			DIGIT		= 1;									// 桁数
	}

	namespace cpu
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(165.0f, 60.0f, 0.0f);		// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(242.0f, 107.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// 空白
	}

	namespace frame
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(160.0f, 280.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(250.0f, 340.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// 空白
	}

	namespace flailArrow
	{
		const float	SPACE_EDGE		= 130.0f;	// 縁の空白

		const POSGRID2		PART	= POSGRID2(MAX_ENTRY_ARROW, 1);		// テクスチャ分割数
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(40.0f, 40.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3	POS		= D3DXVECTOR3(frame::POS.x - SPACE_EDGE, frame::POS.y, 0.0f);	// 位置
		const D3DXVECTOR3	SPACE	= D3DXVECTOR3(SPACE_EDGE * 2.0f, 0.0f, 0.0f);					// 空白
	}

	namespace join
	{
		const POSGRID2	  PART	= POSGRID2(1, 2);	// テクスチャ分割数
		const D3DXVECTOR3 POS	= D3DXVECTOR3(160.0f, 480.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(250.0f*1.3f, 42.0f*1.3f, 0.0f);		// 大きさ
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// 空白
	}

	namespace control
	{
		const float	ADD_ALPHA		= 0.008f;	// 透明度の加算量
		const float	ADD_SINROT		= 0.06f;	// 透明度ふわふわさせる際のサインカーブ向き加算量
		const float	MAX_ADD_ALPHA	= 0.5f;		// 透明度の最大加算量
		const float	BASIC_ALPHA		= 0.95f;	// 基準の透明度

		const D3DXVECTOR3	POS		= D3DXVECTOR3(SCREEN_CENT.x, 620.0f, 0.0f);	// 位置
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(505.0f, 84.0f, 0.0f);			// 大きさ
		const D3DXCOLOR		MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);	// 色
	}

	namespace start
	{
		const float	ADD_ALPHA		= 0.008f;	// 透明度の加算量
		const float	ADD_SINROT		= 0.06f;	// 透明度ふわふわさせる際のサインカーブ向き加算量
		const float	MAX_ADD_ALPHA	= 0.5f;		// 透明度の最大加算量
		const float	BASIC_ALPHA		= 0.95f;	// 基準の透明度

		const D3DXVECTOR3	POS		= D3DXVECTOR3(SCREEN_CENT.x, 380.0f, 0.0f);	// 位置
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(573.0f, 99.0f, 0.0f);			// 大きさ
		const D3DXCOLOR		MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);	// 色
	}

	namespace numcpu
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(SCREEN_CENT.x - 40.0f, 500.0f, 0.0f);	// 位置
		const D3DXVECTOR3	SIZE_TITLE	= D3DXVECTOR3(306.0f * 0.9f, 98.0f * 0.9f, 0.0f);	// タイトル大きさ
		const D3DXVECTOR3	SIZE_VALUE	= D3DXVECTOR3(80.0f, 95.0f, 0.0f);			// 数字大きさ
		const D3DXVECTOR3	SPACE_TITLE	= D3DXVECTOR3(190.0f, 0.0f, 0.0f);			// タイトル空白
		const D3DXVECTOR3	SPACE_VALUE	= VEC3_ZERO;								// 数字空白
		const int			DIGIT		= 1;										// 桁数

		const float	INIT_SCALE	= 0.01f;	// タイトル初期拡大率
		const float	ADD_SCALE	= 0.12f;	// タイトル加算拡大率
		const float	SET_SCALE	= 1.0f;		// タイトル設定拡大率
	}

	namespace arrow
	{
		const float	ADD_ALPHA		= 0.02f;	// 透明度の加算量
		const float	ADD_SINROT		= 0.04f;	// 透明度ふわふわさせる際のサインカーブ向き加算量
		const float	MAX_ADD_ALPHA	= 0.25f;	// 透明度の最大加算量
		const float	BASIC_ALPHA		= 0.95f;	// 基準の透明度
		const float	SPACE_EDGE		= 240.0f;	// 縁の空白

		const POSGRID2		PART	= POSGRID2(MAX_ENTRY_ARROW, 1);		// テクスチャ分割数
		const D3DXVECTOR3	SIZE	= D3DXVECTOR3(80.0f, 80.0f, 0.0f);	// 大きさ

		const D3DXVECTOR3	POS		= D3DXVECTOR3(SCREEN_CENT.x - SPACE_EDGE, numcpu::POS.y, 0.0f);	// 位置
		const D3DXVECTOR3	SPACE	= D3DXVECTOR3(SPACE_EDGE * 2.0f, 0.0f, 0.0f);					// 空白
		const D3DXCOLOR		MIN_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, BASIC_ALPHA - MAX_ADD_ALPHA);		// 色
	}

	namespace bg
	{
		const float	ADD_SIZE_X	= 250.0f;	// 横サイズの加算量

		const D3DXVECTOR3	POS	= D3DXVECTOR3(0.0f, 440.0f, 0.0f);		// 位置
		const D3DXCOLOR		COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.625f);	// 色

		const D3DXVECTOR3	INIT_SIZE	= D3DXVECTOR3(0.0f, 280.0f, 0.0f);	// 初期の大きさ
		const D3DXVECTOR3	DEST_SIZE	= D3DXVECTOR3(SCREEN_SIZE.x * 2.0f, 280.0f, 0.0f);	// 目標の大きさ
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CEntryManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\entry_player.png",	// PLAYERテクスチャ
	"data\\TEXTURE\\entry_flame.png",	// フレームテクスチャ
	"data\\TEXTURE\\entry002.png",		// 操作表示テクスチャ
	"data\\TEXTURE\\entry003.png",		// 開始表示テクスチャ
	"data\\TEXTURE\\Readycheck.png",	// 参加状況テクスチャ
	"data\\TEXTURE\\Arrow_Twin.png",	// 矢印テクスチャ
	"data\\TEXTURE\\cpu.png",			// CPUテクスチャ
	"data\\TEXTURE\\mum_cpu.png",		// CPU数テクスチャ
	"data\\TEXTURE\\Arrow_Twin.png",	// フレイル矢印テクスチャ
};

//************************************************************
//	親クラス [CEntryManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEntryManager::CEntryManager()
{
	// メンバ変数をクリア
	memset(&m_apNumber[0],	0, sizeof(m_apNumber));	// プレイヤーナンバーの情報
	memset(&m_apCpu[0],		0, sizeof(m_apCpu));	// プレイヤーCPUの情報
	memset(&m_apFrame[0],	0, sizeof(m_apFrame));	// プレイヤーフレームの情報
	memset(&m_apJoin[0],	0, sizeof(m_apJoin));	// プレイヤー参加の情報
	memset(&m_apArrow[0],	0, sizeof(m_apArrow));	// プレイヤー参加の情報

	m_pRuleManager	= NULL;		// エントリールールの情報
	m_pControl		= NULL;		// 操作表示の情報
	m_pBG			= NULL;		// 背景の情報
	m_pStart		= NULL;		// 開始表示の情報
	m_pNumCpu		= NULL;		// CPU数表示の情報
	m_fScale		= 0.0f;		// 拡大率
	m_fSinStartAlpha	= 0.0f;	// 開始表示の透明向き
	m_fSinControlAlpha	= 0.0f;	// 操作表示の透明向き
	m_fSinArrowAlpha	= 0.0f;	// 矢印表示の透明向き

	m_stateEntry	= STATE_ENTRY_NONE_JOIN;	// エントリー状態
	m_state			= STATE_ENTRY;				// 状態
}

//============================================================
//	デストラクタ
//============================================================
CEntryManager::~CEntryManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEntryManager::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apNumber[0],	0, sizeof(m_apNumber));	// プレイヤーナンバーの情報
	memset(&m_apCpu[0],		0, sizeof(m_apCpu));	// プレイヤーCPUの情報
	memset(&m_apFrame[0],	0, sizeof(m_apFrame));	// プレイヤーフレームの情報
	memset(&m_apJoin[0],	0, sizeof(m_apJoin));	// プレイヤー参加の情報
	memset(&m_apArrow[0],	0, sizeof(m_apArrow));	// プレイヤー参加の情報

	m_pRuleManager	= NULL;	// エントリールールの情報
	m_pControl		= NULL;	// 操作表示の情報
	m_pBG			= NULL;	// 背景の情報
	m_pStart		= NULL;	// 開始表示の情報
	m_pNumCpu		= NULL;	// CPU数表示の情報
	m_fScale		= 1.0f;	// 拡大率

	m_fSinStartAlpha	= -HALF_PI;	// 開始表示の透明向き
	m_fSinControlAlpha	= -HALF_PI;	// 操作表示の透明向き
	m_fSinArrowAlpha	= -HALF_PI;	// 矢印表示の透明向き
	m_stateEntry	= STATE_ENTRY_NONE_JOIN;	// エントリー状態
	m_state			= STATE_ENTRY;				// 状態

	// ゲーム情報を初期化
	CManager::GetInstance()->GetRetentionManager()->InitGame();

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		// プレイヤーナンバーの生成
		m_apNumber[nCntEntry] = CValueUI::Create
		( // 引数
			mc_apTextureFile[TEXTURE_PLAYER],						// タイトルテクスチャパス
			CValue::TEXTURE_NORMAL,									// 数字テクスチャ
			number::DIGIT,											// 桁数
			number::POS + (number::SPACE_POS * (float)nCntEntry),	// 位置
			number::SPACE_TITLE,	// 行間
			number::SPACE_VALUE,	// 数字行間
			number::SIZE_TITLE,		// タイトル大きさ
			number::SIZE_VALUE,		// 数字大きさ
			VEC3_ZERO,				// タイトル向き
			VEC3_ZERO,				// 数字向き
			COL_UNENTRY,			// タイトル色
			COL_UNENTRY				// 数字色
		);
		if (m_apNumber[nCntEntry] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apNumber[nCntEntry]->SetPriority(PRIORITY);

		// 数字を設定
		m_apNumber[nCntEntry]->GetMultiValue()->SetNum(nCntEntry + 1);

		// プレイヤーCPUの生成
		m_apCpu[nCntEntry] = CObject2D::Create
		( // 引数
			cpu::POS + (cpu::SPACE * (float)nCntEntry),	// 位置
			cpu::SIZE	// 大きさ
		);
		if (m_apCpu[nCntEntry] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_apCpu[nCntEntry]->BindTexture(mc_apTextureFile[TEXTURE_CPU]);

		// 優先順位を設定
		m_apCpu[nCntEntry]->SetPriority(PRIORITY);

		// 自動描画をOFFにする
		m_apCpu[nCntEntry]->SetEnableDraw(false);

		// プレイヤーフレームの生成
		m_apFrame[nCntEntry] = CObject2D::Create
		( // 引数
			frame::POS + (frame::SPACE * (float)nCntEntry),	// 位置
			frame::SIZE,	// 大きさ
			VEC3_ZERO,		// 向き
			COL_UNENTRY		// 色
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

		// プレイヤー参加の生成
		m_apJoin[nCntEntry] = CAnim2D::Create
		( // 引数
			join::PART.x,	// テクスチャ横分割数
			join::PART.y,	// テクスチャ縦分割数
			join::POS + (join::SPACE * (float)nCntEntry),	// 位置
			join::SIZE		// 大きさ
		);
		if (m_apJoin[nCntEntry] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_apJoin[nCntEntry]->BindTexture(mc_apTextureFile[TEXTURE_JOIN]);

		// 優先順位を設定
		m_apJoin[nCntEntry]->SetPriority(PRIORITY);

		// 自動描画をOFFにする
		m_apJoin[nCntEntry]->SetEnableDraw(false);

		for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
		{ // 矢印の総数分繰り返す

			// フレイル矢印の生成
			m_apFlailArrow[nCntEntry][nCntArrow] = CAnim2D::Create
			( // 引数
				flailArrow::PART.x,	// テクスチャ横分割数
				flailArrow::PART.y,	// テクスチャ縦分割数
				flailArrow::POS + ((float)nCntArrow * flailArrow::SPACE) + ((float)nCntEntry * frame::SPACE),	// 位置
				flailArrow::SIZE,	// 大きさ
				VEC3_ZERO,			// 向き
				XCOL_WHITE			// 色
			);
			if (m_apFlailArrow[nCntEntry][nCntArrow] == NULL)
			{ // 生成に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// テクスチャを登録・割当
			m_apFlailArrow[nCntEntry][nCntArrow]->BindTexture(mc_apTextureFile[TEXTURE_FLAILARROW]);

			// 優先順位を設定
			m_apFlailArrow[nCntEntry][nCntArrow]->SetPriority(PRIORITY);

			// パターンを設定
			m_apFlailArrow[nCntEntry][nCntArrow]->SetPattern(nCntArrow);

			// 自動描画をOFFにする
			m_apFlailArrow[nCntEntry][nCntArrow]->SetEnableDraw(false);
		}
	}

	// 操作表示の生成
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

	// 背景の生成
	m_pBG = CObject2D::Create
	( // 引数
		bg::POS,		// 位置
		bg::INIT_SIZE,	// 大きさ
		VEC3_ZERO,		// 向き
		bg::COL			// 色
	);
	if (m_pBG == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pBG->SetPriority(PRIORITY);

	// 自動描画をOFFにする
	m_pBG->SetEnableDraw(false);

	// 開始表示の生成
	m_pStart = CObject2D::Create
	( // 引数
		start::POS,	// 位置
		start::SIZE * numcpu::INIT_SCALE,	// 大きさ
		VEC3_ZERO,	// 向き
		XCOL_AWHITE	// 色
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

	// 自動描画をOFFにする
	m_pStart->SetEnableDraw(false);

	// CPU数表示の生成
	m_pNumCpu = CValueUI::Create
	( // 引数
		mc_apTextureFile[TEXTURE_NUMCPU],	// タイトルテクスチャパス
		CValue::TEXTURE_ZERO,	// 数字テクスチャ
		numcpu::DIGIT,			// 桁数
		numcpu::POS,			// 位置
		numcpu::SPACE_TITLE,	// 行間
		numcpu::SPACE_VALUE,	// 数字行間
		numcpu::SIZE_TITLE * numcpu::INIT_SCALE,	// タイトル大きさ
		numcpu::SIZE_VALUE * numcpu::INIT_SCALE		// 数字大きさ
	);
	if (m_pNumCpu == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pNumCpu->SetPriority(PRIORITY);

	// 最小数を設定
	m_pNumCpu->GetMultiValue()->SetMin(0);

	// 最大数を設定
	m_pNumCpu->GetMultiValue()->SetMax(MAX_PLAYER - 1);

	// 自動描画をOFFにする
	m_pNumCpu->SetEnableDraw(false);

	for (int i = 0; i < MAX_RULE_ARROW; i++)
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

		// 自動描画をOFFにする
		m_apArrow[i]->SetEnableDraw(false);
	}

	// プレイ人数を初期化
	CManager::GetInstance()->GetRetentionManager()->SetNumPlayer(0);

	// エントリーを初期化
	CManager::GetInstance()->GetRetentionManager()->AllSetEnableEntry(false, false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CEntryManager::Uninit(void)
{
	if (m_pRuleManager != NULL)
	{
		// エントリールールの破棄
		if (FAILED(CEntryRuleManager::Release(m_pRuleManager)))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		// プレイヤーナンバーの終了
		m_apNumber[nCntEntry]->Uninit();

		// プレイヤーCPUの終了
		m_apCpu[nCntEntry]->Uninit();

		// プレイヤーフレームの終了
		m_apFrame[nCntEntry]->Uninit();

		// プレイヤー参加の終了
		m_apJoin[nCntEntry]->Uninit();

		for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
		{ // 矢印の総数分繰り返す

			// フレイル矢印の終了
			m_apFlailArrow[nCntEntry][nCntArrow]->Uninit();
		}
	}

	for (int i = 0; i < MAX_RULE_ARROW; i++)
	{ // 矢印の総数分繰り返す

		// 矢印の終了
		m_apArrow[i]->Uninit();
	}

	// 操作表示の終了
	m_pControl->Uninit();

	// 背景の終了
	m_pBG->Uninit();

	// 開始表示の終了
	m_pStart->Uninit();

	// CPU数表示の終了
	m_pNumCpu->Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CEntryManager::Update(void)
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::FADE_NONE)
	{ // フェード中の場合

		// 処理を抜ける
		return;
	}

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_ENTRY:

		// エントリーの更新
		UpdateEntry();

		// フレイルの更新
		UpdateFlail();

		switch (m_stateEntry)
		{ // エントリー状態ごとの処理
		case STATE_ENTRY_NONE_JOIN:

			if (IsReadyOK(1))
			{ // 全員が準備済みの場合

				// CPU演出の開始設定
				SetCpuObject(true);

				// CPU数背景の自動描画をONにする
				m_pBG->SetEnableDraw(true);

				// CPU数背景の演出状態にする
				m_stateEntry = STATE_ENTRY_CPU_BG;
			}

			break;

		case STATE_ENTRY_CPU_BG:

			// CPU数背景の演出の更新
			UpdateCpuBG();

			break;

		case STATE_ENTRY_CPU_UI:

			// CPU数UIの演出の更新
			UpdateCpuUI();

			break;

		case STATE_ENTRY_NUMCPU:

			// CPUの更新
			UpdateCpu();

			// 開始UIの更新
			UpdateStartUI();

			break;

		default:
			assert(false);
			break;
		}

		// 開始の更新
		UpdateStart();

		// プレイヤー名の更新
		UpdatePlayerName();

		// 操作UIの更新
		UpdateControlUI();

		// UIオブジェクトの全更新
		UpdateUIAll();

		break;

	case STATE_RULE:

		if (m_pRuleManager != NULL)
		{
			// エントリールールの更新
			m_pRuleManager->Update();
		}

		break;

	case STATE_END:
		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
//	状態の設定処理
//============================================================
HRESULT CEntryManager::SetState(const EState state)
{
	// 引数の状態を設定
	m_state = state;

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_ENTRY:

		if (m_pRuleManager != NULL)
		{
			// エントリールールの破棄
			if (FAILED(CEntryRuleManager::Release(m_pRuleManager)))
			{ // 失敗した場合

				// 失敗を返す
				return E_FAIL;
			}
		}

		break;

	case STATE_RULE:

		if (m_pRuleManager == NULL)
		{
			// エントリールールの生成
			m_pRuleManager = CEntryRuleManager::Create();
			if (m_pRuleManager == NULL)
			{ // 失敗した場合

				// 失敗を返す
				return E_FAIL;
			}
		}

		break;

	case STATE_END:

		// シーンの設定
		CManager::GetInstance()->SetScene(CScene::MODE_GAME);	// ゲーム画面

		break;

	default:
		assert(false);
		break;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	生成処理
//============================================================
CEntryManager *CEntryManager::Create(void)
{
	// ポインタを宣言
	CEntryManager *pEntryManager = NULL;	// エントリーマネージャー生成用

	if (pEntryManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pEntryManager = new CEntryManager;	// エントリーマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pEntryManager != NULL)
	{ // 使用されている場合
		
		// エントリーマネージャーの初期化
		if (FAILED(pEntryManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pEntryManager;
			pEntryManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pEntryManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CEntryManager::Release(CEntryManager *&prEntryManager)
{
	if (prEntryManager != NULL)
	{ // 使用中の場合

		// エントリーマネージャーの終了
		if (FAILED(prEntryManager->Uninit()))
		{ // 破棄に失敗した場合

			// メモリ開放
			delete prEntryManager;
			prEntryManager = NULL;

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prEntryManager;
		prEntryManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	エントリーの更新処理
//============================================================
void CEntryManager::UpdateEntry(void)
{
	// ポインタを宣言
	CInputPad *pPad = CManager::GetInstance()->GetPad();	// パッド
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// データ保存情報

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		bool bEntry = true;	// エントリーができるか
		if (pRetention->IsEntry(nCntEntry))
		{ // エントリー者がいる場合

			if (pRetention->IsAI(nCntEntry))
			{ // AIのエントリーなら上書き可能

				bEntry = true;
			}
			else
			{ // 人間のエントリーなら上書き不可

				bEntry = false;
			}

			// 準備表示の自動描画をONにする
			m_apJoin[nCntEntry]->SetEnableDraw(true);
		}
		else
		{ // エントリー者がいない場合

			// 準備表示の自動描画をOFFにする
			m_apJoin[nCntEntry]->SetEnableDraw(false);
		}

		if (bEntry)
		{ // エントリー出来る場合

			if (pPad->IsTrigger(CInputPad::KEY_A, nCntEntry))
			{
				// エントリーを登録
				pRetention->SetEntry(nCntEntry, true, false);

				// 準備未完了状態にする
				m_apJoin[nCntEntry]->SetPattern(JOIN_OFF);

				// 色を参加時のものに設定
				m_apFrame[nCntEntry]->SetColor(COL_ENTRY);
				m_apNumber[nCntEntry]->SetColorTitle(COL_ENTRY);
				m_apNumber[nCntEntry]->GetMultiValue()->SetColor(COL_ENTRY);

				for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
				{ // 矢印の総数分繰り返す

					// フレイル矢印の自動描画をONにする
					m_apFlailArrow[nCntEntry][nCntArrow]->SetEnableDraw(true);
				}

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// 選択操作音00
			}
		}
		else
		{ // エントリー出来ない場合

			switch (m_apJoin[nCntEntry]->GetPattern())
			{ // 準備状況ごとの処理
			case JOIN_OFF:

				if (pPad->IsTrigger(CInputPad::KEY_A, nCntEntry))
				{
					// 準備完了状態にする
					m_apJoin[nCntEntry]->SetPattern(JOIN_ON);

					for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
					{ // 矢印の総数分繰り返す

						// フレイル矢印の自動描画をOFFにする
						m_apFlailArrow[nCntEntry][nCntArrow]->SetEnableDraw(false);
					}

					// サウンドの再生
					CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 選択操作音01
				}
				else if (pPad->IsTrigger(CInputPad::KEY_B, nCntEntry))
				{
					// エントリーを解除
					pRetention->SetEntry(nCntEntry, false, false);

					// 色を非参加時のものに設定
					m_apFrame[nCntEntry]->SetColor(COL_UNENTRY);
					m_apNumber[nCntEntry]->SetColorTitle(COL_UNENTRY);
					m_apNumber[nCntEntry]->GetMultiValue()->SetColor(COL_UNENTRY);

					for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
					{ // 矢印の総数分繰り返す

						// フレイル矢印の自動描画をOFFにする
						m_apFlailArrow[nCntEntry][nCntArrow]->SetEnableDraw(false);
					}

					// サウンドの再生
					CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
				}

				break;

			case JOIN_ON:

				if (pPad->IsTrigger(CInputPad::KEY_B, nCntEntry))
				{
					// 準備未完了状態にする
					m_apJoin[nCntEntry]->SetPattern(JOIN_OFF);

					for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
					{ // 矢印の総数分繰り返す
						
						// フレイル矢印の自動描画をONにする
						m_apFlailArrow[nCntEntry][nCntArrow]->SetEnableDraw(true);
					}

					// サウンドの再生
					CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
				}

				break;

			default:
				assert(false);
				break;
			}
		}
	}

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー数]：%d\n", pRetention->GetNumPlayer());
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー1]：%s\n", pRetention->IsEntry(0) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー2]：%s\n", pRetention->IsEntry(1) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー3]：%s\n", pRetention->IsEntry(2) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー4]：%s\n", pRetention->IsEntry(3) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー1AI]：%s\n", pRetention->IsAI(0) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー2AI]：%s\n", pRetention->IsAI(1) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー3AI]：%s\n", pRetention->IsAI(2) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー4AI]：%s\n", pRetention->IsAI(3) ? "true" : "false");
}

//============================================================
//	フレイルの更新処理
//============================================================
void CEntryManager::UpdateFlail(void)
{
	// ポインタを宣言
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// データ保存情報
	CInputPad *pPad = CManager::GetInstance()->GetPad();	// パッド

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		if (!pRetention->IsEntry(nCntEntry))
		{ // エントリーしていない場合

			break;
		}

		if (m_apJoin[nCntEntry]->GetPattern() != JOIN_OFF)
		{ // 準備が出来ている場合

			break;
		}

		// ポインタを宣言
		CPlayer *pPlayer = CScene::GetPlayer(nCntEntry);	// プレイヤー情報
		CFlail *pFlail = pPlayer->GetFlail();				// フレイル情報

		// 変数を宣言
		int nType = pFlail->GetType();	// フレイル種類

		if (pPad->IsTrigger(CInputPad::KEY_RIGHT))
		{ // 右移動の操作が行われた場合

			// 右に選択をずらす
			nType = (nType + (CFlail::FLAIL_MAX - 1)) % CFlail::FLAIL_MAX;

			// フレイル種類を反映
			pFlail->SetType(nType);
			pRetention->SetFlail(nCntEntry, nType);

			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
		}
		if (pPad->IsTrigger(CInputPad::KEY_LEFT))
		{ // 左移動の操作が行われた場合

			// 左に選択をずらす
			nType = (nType + 1) % CFlail::FLAIL_MAX;

			// フレイル種類を反映
			pFlail->SetType(nType);
			pRetention->SetFlail(nCntEntry, nType);

			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
		}
	}
}

//============================================================
//	CPUの更新処理
//============================================================
void CEntryManager::UpdateCpu(void)
{
	if (IsReadyOK(1))
	{ // 全員が準備済みの場合

		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // 矢印の総数分繰り返す

			// 矢印の自動描画をONにする
			m_apArrow[i]->SetEnableDraw(true);
		}

		// 自動描画をONにする
		m_pBG->SetEnableDraw(true);		// 背景
		m_pStart->SetEnableDraw(true);	// 開始表示
		m_pNumCpu->SetEnableDraw(true);	// CPU数

		// 矢印の更新
		UpdateArrow();

		// CPUの加減算の更新
		UpdateAddCpu();
	}
	else
	{ // 全員が準備済みではない場合

		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // 矢印の総数分繰り返す

			// 矢印の自動描画をOFFにする
			m_apArrow[i]->SetEnableDraw(false);
		}

		// 自動描画をOFFにする
		m_pBG->SetEnableDraw(false);		// 背景
		m_pStart->SetEnableDraw(false);		// 開始表示
		m_pNumCpu->SetEnableDraw(false);	// CPU数

		// 準備の未完了状態にする
		m_stateEntry = STATE_ENTRY_NONE_JOIN;
	}
}

//============================================================
//	CPUの加減算の更新処理
//============================================================
void CEntryManager::UpdateAddCpu(void)
{
	CInputKeyboard		*pKeyboard	= CManager::GetInstance()->GetKeyboard();			// キーボード
	CInputPad			*pPad		= CManager::GetInstance()->GetPad();				// パッド
	CRetentionManager	*pRetention	= CManager::GetInstance()->GetRetentionManager();	// データ保存情報
	int nNumCpu = 0;	// CPU数
	bool bAdd = false;	// 加算操作
	bool bSub = false;	// 減算操作

	// 加算の操作を保存
	bAdd = pKeyboard->IsTrigger(DIK_D) || pKeyboard->IsTrigger(DIK_RIGHT) || pPad->IsTriggerAll(CInputPad::KEY_RIGHT);
	
	// 減算の操作を保存
	bSub = pKeyboard->IsTrigger(DIK_A) || pKeyboard->IsTrigger(DIK_LEFT) || pPad->IsTriggerAll(CInputPad::KEY_LEFT);

	if (bAdd && !bSub)
	{ // 加算操作の場合

		for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
		{ // プレイヤーの最大数分繰り返す

			if (!pRetention->IsEntry(nCntEntry))
			{ // エントリーしていない場合

				// エントリーを登録
				pRetention->SetEntry(nCntEntry, true, true);

				// 準備完了状態にする
				m_apJoin[nCntEntry]->SetPattern(JOIN_ON);

				// 色を参加時のものに設定
				m_apFrame[nCntEntry]->SetColor(COL_ENTRY);
				m_apNumber[nCntEntry]->SetColorTitle(COL_ENTRY);
				m_apNumber[nCntEntry]->GetMultiValue()->SetColor(COL_ENTRY);

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00

				// 処理を抜ける
				break;
			}
		}
	}

	if (bSub && !bAdd)
	{ // 減算操作の場合

		for (int nCntEntry = MAX_PLAYER - 1; nCntEntry >= 0; nCntEntry--)
		{ // プレイヤーの最大数分繰り返す

			if (pRetention->IsEntry(nCntEntry) && pRetention->IsAI(nCntEntry))
			{ // エントリーしている且つ、CPUの場合

				// エントリーを解除
				pRetention->SetEntry(nCntEntry, false, false);

				// 準備未完了状態にする
				m_apJoin[nCntEntry]->SetPattern(JOIN_OFF);

				// 色を非参加時のものに設定
				m_apFrame[nCntEntry]->SetColor(COL_UNENTRY);
				m_apNumber[nCntEntry]->SetColorTitle(COL_UNENTRY);
				m_apNumber[nCntEntry]->GetMultiValue()->SetColor(COL_UNENTRY);

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00

				// 処理を抜ける
				break;
			}
		}
	}

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		if (pRetention->IsAI(nCntEntry))
		{ // AIの場合

			// AI数加算
			nNumCpu++;
		}
	}

	// CPU数を設定
	m_pNumCpu->GetMultiValue()->SetNum(nNumCpu);
}

//============================================================
//	プレイヤー名の更新処理
//============================================================
void CEntryManager::UpdatePlayerName(void)
{
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// データ保存情報

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		if (pRetention->IsEntry(nCntEntry))
		{ // エントリーしている場合

			if (pRetention->IsAI(nCntEntry))
			{ // AIの場合

				// 参加者名をCPUにする
				m_apNumber[nCntEntry]->SetEnableDraw(false);
				m_apCpu[nCntEntry]->SetEnableDraw(true);
			}
			else
			{ // AIではない場合

				// 参加者名をPLAYERにする
				m_apCpu[nCntEntry]->SetEnableDraw(false);
				m_apNumber[nCntEntry]->SetEnableDraw(true);
			}
		}
		else
		{ // エントリーしていない場合

			// 参加者名をPLAYERにする
			m_apCpu[nCntEntry]->SetEnableDraw(false);
			m_apNumber[nCntEntry]->SetEnableDraw(true);
		}
	}
}

//============================================================
//	操作UIの更新処理
//============================================================
void CEntryManager::UpdateControlUI(void)
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
//	開始UIの更新処理
//============================================================
void CEntryManager::UpdateStartUI(void)
{
	if (!IsReadyOK(2))
	{ // 二人以上が準備OKしていない場合

		// 関数を抜ける
		return;
	}

	// 変数を宣言
	D3DXCOLOR colStart = m_pStart->GetColor();	// 開始表示色

	if (colStart.a < start::MIN_COL.a)
	{ // 透明度が最低限より低い場合

		// 透明度を加算
		colStart.a += start::ADD_ALPHA;

		if (colStart.a > start::MIN_COL.a)
		{ // 透明度が超過した場合

			// 透明度を補正
			colStart.a = start::MIN_COL.a;
		}

		// 操作表示色を設定
		m_pStart->SetColor(colStart);
	}
	else
	{ // 透明度が最低限以上の場合

		// 変数を宣言
		float fAddAlpha = 0.0f;	// 透明度の加算量

		// 透明度を上げる
		m_fSinStartAlpha += start::ADD_SINROT;
		useful::NormalizeRot(m_fSinStartAlpha);	// 向き正規化

		// 透明度加算量を求める
		fAddAlpha = (start::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinStartAlpha) - 1.0f);

		// 操作表示色を設定
		m_pStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, start::BASIC_ALPHA + fAddAlpha));
	}
}

//============================================================
//	開始の更新処理
//============================================================
void CEntryManager::UpdateStart(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド

	if (IsReadyOK(2))
	{ // 遷移可能の場合

		if (m_stateEntry == STATE_ENTRY_NUMCPU)
		{ // CPU数の設定状態の場合

			if (pKeyboard->IsTrigger(DIK_RETURN)
			||  pKeyboard->IsTrigger(DIK_SPACE)
			||  pPad->IsTriggerAll(CInputPad::KEY_START))
			{
				// ルール設定状態にする
				SetState(STATE_RULE);

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 選択操作音01
			}
		}

		// 開始表示の色を明るくする
		D3DXCOLOR col = m_pStart->GetColor();
		m_pStart->SetColor(D3DXCOLOR(COL_ENTRY.r, COL_ENTRY.g, COL_ENTRY.b, col.a));
	}
	else
	{ // 遷移可能ではない場合

		// 開始表示の色を暗くする
		m_pStart->SetColor(COL_UNENTRY);
	}
}

//============================================================
//	矢印の更新処理
//============================================================
void CEntryManager::UpdateArrow(void)
{
	for (int i = 0; i < MAX_ENTRY_ARROW; i++)
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
//	CPU数背景の演出の更新処理
//============================================================
void CEntryManager::UpdateCpuBG(void)
{
	// 変数を宣言
	D3DXVECTOR3 sizeBG = m_pBG->GetVec3Sizing();	// 背景の大きさ

	// 大きさを加算
	sizeBG.x += bg::ADD_SIZE_X;

	if (sizeBG.x >= bg::DEST_SIZE.x)
	{ // 目標の大きさ以上の場合

		// 大きさを補正
		sizeBG.x = bg::DEST_SIZE.x;

		// 自動描画をONにする
		m_pStart->SetEnableDraw(true);	// 開始表示
		m_pNumCpu->SetEnableDraw(true);	// CPU数

		// UI演出の拡大率を設定
		m_fScale = numcpu::INIT_SCALE;

		// CPU数UIの演出状態にする
		m_stateEntry = STATE_ENTRY_CPU_UI;
	}

	// 背景の大きさを反映
	m_pBG->SetVec3Sizing(sizeBG);
}

//============================================================
//	CPU数UIの演出の更新処理
//============================================================
void CEntryManager::UpdateCpuUI(void)
{
	// 拡大率を加算
	m_fScale += numcpu::ADD_SCALE;

	if (m_fScale < numcpu::SET_SCALE)
	{ // まだ大きくなる場合

		// 大きさを反映
		m_pStart->SetVec3Sizing(start::SIZE * m_fScale);	// 開始表示
		m_pNumCpu->SetScalingTitle(numcpu::SIZE_TITLE * m_fScale);	// CPU数タイトル
		m_pNumCpu->GetMultiValue()->SetVec3Sizing(numcpu::SIZE_VALUE * m_fScale);	// CPU数値
	}
	else
	{ // 大きくなり切った場合

		// 大きさを反映
		m_pStart->SetVec3Sizing(start::SIZE);	// 開始表示
		m_pNumCpu->SetScalingTitle(numcpu::SIZE_TITLE);	// CPU数タイトル
		m_pNumCpu->GetMultiValue()->SetVec3Sizing(numcpu::SIZE_VALUE);	// CPU数値

		// 拡大率を初期化
		m_fScale = 1.0f;

		// CPU数の変更状態にする
		m_stateEntry = STATE_ENTRY_NUMCPU;
	}
}

//============================================================
//	UIオブジェクトの全更新処理
//============================================================
void CEntryManager::UpdateUIAll(void)
{
	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		// プレイヤーナンバーの更新
		m_apNumber[nCntEntry]->Update();

		// プレイヤーCPUの更新
		m_apCpu[nCntEntry]->Update();

		// プレイヤーフレームの更新
		m_apFrame[nCntEntry]->Update();

		// プレイヤー参加の更新
		m_apJoin[nCntEntry]->Update();

		for (int nCntArrow = 0; nCntArrow < MAX_ENTRY_ARROW; nCntArrow++)
		{ // 矢印の総数分繰り返す

			// フレイル矢印の更新
			m_apFlailArrow[nCntEntry][nCntArrow]->Update();
		}
	}

	for (int i = 0; i < MAX_RULE_ARROW; i++)
	{ // 矢印の総数分繰り返す

		// 矢印の更新
		m_apArrow[i]->Update();
	}

	// 操作表示の更新
	m_pControl->Update();

	// 背景の更新
	m_pBG->Update();

	// 開始表示の更新
	m_pStart->Update();

	// CPU数表示の更新
	m_pNumCpu->Update();
}

//============================================================
//	CPU演出の設定処理
//============================================================
void CEntryManager::SetCpuObject(const bool bStart)
{
	if (bStart)
	{ // 開始時の設定の場合

		// 自動描画をOFFにする
		m_pBG->SetEnableDraw(false);		// 背景
		m_pStart->SetEnableDraw(false);		// 開始表示
		m_pNumCpu->SetEnableDraw(false);	// CPU数

		// 大きさを設定
		m_pBG->SetVec3Sizing(bg::INIT_SIZE);												// 背景
		m_pStart->SetVec3Sizing(start::SIZE * numcpu::INIT_SCALE);							// 開始表示
		m_pNumCpu->SetScalingTitle(numcpu::SIZE_TITLE * numcpu::INIT_SCALE);				// CPU数タイトル
		m_pNumCpu->GetMultiValue()->SetVec3Sizing(numcpu::SIZE_VALUE * numcpu::INIT_SCALE);	// CPU数値

		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // 矢印の総数分繰り返す
	
			// 矢印の色を設定
			m_apArrow[i]->SetColor(XCOL_AWHITE);
		}

		// 開始表示の色を設定
		m_pStart->SetColor(XCOL_AWHITE);
	}
	else
	{ // スキップ時の設定の場合

		// 拡大率を初期化
		m_fScale = 1.0f;

		// 自動描画をONにする
		m_pBG->SetEnableDraw(true);		// 背景
		m_pStart->SetEnableDraw(true);	// 開始表示
		m_pNumCpu->SetEnableDraw(true);	// CPU数

		// 大きさを設定
		m_pBG->SetVec3Sizing(bg::DEST_SIZE);							// 背景
		m_pStart->SetVec3Sizing(start::SIZE);							// 開始表示
		m_pNumCpu->SetScalingTitle(numcpu::SIZE_TITLE);					// CPU数タイトル
		m_pNumCpu->GetMultiValue()->SetVec3Sizing(numcpu::SIZE_VALUE);	// CPU数値

		for (int i = 0; i < MAX_RULE_ARROW; i++)
		{ // 矢印の総数分繰り返す

			// 矢印の色を設定
			m_apArrow[i]->SetColor(XCOL_AWHITE);
		}

		// 開始表示の色を設定
		m_pStart->SetColor(XCOL_AWHITE);
	}
}

//============================================================
//	準備済みかの判定取得処理
//============================================================
bool CEntryManager::IsReadyOK(const int nNumEntry) const
{
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// データ保存情報
	bool bAllAI = true;	// 全参加者のAI状況

	if (pRetention->GetNumPlayer() < nNumEntry)
	{ // 参加人数不足

		// 非準備状態
		return false;
	}

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		if (pRetention->IsEntry(nCntEntry))
		{ // エントリーしている場合

			switch (m_apJoin[nCntEntry]->GetPattern())
			{ // 準備状況ごとの処理
			case JOIN_OFF:

				// 非準備状態
				return false;

			case JOIN_ON:
				break;

			default:
				assert(false);
				break;
			}

			if (bAllAI)
			{ // AIではないプレイヤーが発見されていない場合

				if (!pRetention->IsAI(nCntEntry))
				{ // AIではないプレイヤーの場合

					// 全員がAIではない状態にする
					bAllAI = false;
				}
			}
		}
	}

	if (bAllAI)
	{ // 全員がAIの場合

		// 非準備状態
		return false;
	}

	// 遷移可能を返す
	return true;
}
