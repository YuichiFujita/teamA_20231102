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
#include "retentionManager.h"
#include "fade.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 14;	// エントリーの優先順位

	const D3DXCOLOR COL_ENTRY	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 参加中カラー
	const D3DXCOLOR COL_UNENTRY	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// 非参加中カラー

	namespace number
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(165.0f, 90.0f, 0.0f);		// 位置
		const D3DXVECTOR3	SIZE_TITLE	= D3DXVECTOR3(242.0f, 107.0f, 0.0f);	// タイトル大きさ
		const D3DXVECTOR3	SIZE_VALUE	= D3DXVECTOR3(80.0f, 90.0f, 0.0f);		// 数字大きさ
		const D3DXVECTOR3	SPACE_POS	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// 数字UI同士の空白
		const D3DXVECTOR3	SPACE_TITLE	= D3DXVECTOR3(100.0f, 5.0f, 0.0f);		// タイトル空白
		const D3DXVECTOR3	SPACE_VALUE	= VEC3_ZERO;							// 数字空白
		const int			DIGIT		= 1;									// 桁数
	}

	namespace frame
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(160.0f, 360.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(250.0f, 440.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(320.0f, 0.0f, 0.0f);		// 空白
	}

	namespace control
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, 630.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(505.0f, 84.0f, 0.0f);			// 大きさ
	}

	namespace start
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(1100.0f, 600.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(300.0f, 180.0f, 0.0f);	// 大きさ
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CEntryManager::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\entry_player.png",	// PLAYERテクスチャ
	"data\\TEXTURE\\entry_flame.png",	// フレームテクスチャ
	"data\\TEXTURE\\entry002.png",	// 操作表示テクスチャ
	"data\\TEXTURE\\entry003.png",	// 開始表示テクスチャ
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
	memset(&m_apFrame[0],	0, sizeof(m_apFrame));	// プレイヤーフレームの情報
	m_pControl	= NULL;	// 操作表示の情報
	m_pStart	= NULL;	// 開始表示の情報
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
	memset(&m_apFrame[0],	0, sizeof(m_apFrame));	// プレイヤーフレームの情報
	m_pControl	= NULL;	// 操作表示の情報
	m_pStart	= NULL;	// 開始表示の情報

	// ゲーム情報を初期化
	CManager::GetInstance()->GetRetentionManager()->InitGame();

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

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
	}

	// 操作表示の生成
	m_pControl = CObject2D::Create
	( // 引数
		control::POS,	// 位置
		control::SIZE	// 大きさ
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

	// 開始表示の生成
	m_pStart = CObject2D::Create
	( // 引数
		start::POS,	// 位置
		start::SIZE	// 大きさ
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

	// プレイ人数を初期化
	CManager::GetInstance()->GetRetentionManager()->SetNumPlayer(0);

	// エントリーを初期化
	CManager::GetInstance()->GetRetentionManager()->AllSetEnableEntry(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CEntryManager::Uninit(void)
{
	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		// プレイヤーナンバーの終了
		m_apNumber[nCntEntry]->Uninit();

		// プレイヤーフレームの終了
		m_apFrame[nCntEntry]->Uninit();
	}

	// 操作表示の終了
	m_pControl->Uninit();

	// 開始表示の終了
	m_pStart->Uninit();

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

	// エントリーの更新
	UpdateEntry();

	// 開始の更新
	UpdateStart();

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		// プレイヤーナンバーの更新
		m_apNumber[nCntEntry]->Update();

		// プレイヤーフレームの更新
		m_apFrame[nCntEntry]->Update();
	}

	// 操作表示の更新
	m_pControl->Update();

	// 開始表示の更新
	m_pStart->Update();
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

	// 変数を宣言
	int nNumPlayer = pRetention->GetNumPlayer();	// プレイ人数

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		if (pPad->IsTrigger(CInputPad::KEY_A, nCntEntry))
		{
			if (!pRetention->IsEntry(nCntEntry))
			{ // エントリーしていない場合

				// エントリーを登録
				pRetention->SetEnableEntry(nCntEntry, true);
				nNumPlayer++;	// エントリー数加算

				// 色を参加時のものに設定
				m_apFrame[nCntEntry]->SetColor(COL_ENTRY);
				m_apNumber[nCntEntry]->SetColorTitle(COL_ENTRY);
				m_apNumber[nCntEntry]->GetMultiValue()->SetColor(COL_ENTRY);
			}
		}
		else if (pPad->IsTrigger(CInputPad::KEY_B, nCntEntry))
		{
			if (pRetention->IsEntry(nCntEntry))
			{ // エントリーしている場合

				// エントリーを解除
				pRetention->SetEnableEntry(nCntEntry, false);
				nNumPlayer--;	// エントリー数減算

				// 色を非参加時のものに設定
				m_apFrame[nCntEntry]->SetColor(COL_UNENTRY);
				m_apNumber[nCntEntry]->SetColorTitle(COL_UNENTRY);
				m_apNumber[nCntEntry]->GetMultiValue()->SetColor(COL_UNENTRY);
			}
		}
	}

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー数]：%d\n", nNumPlayer);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー1]：%s\n", pRetention->IsEntry(0) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー2]：%s\n", pRetention->IsEntry(1) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー3]：%s\n", pRetention->IsEntry(2) ? "true" : "false");
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[エントリー4]：%s\n", pRetention->IsEntry(3) ? "true" : "false");

	// プレイ人数を設定
	pRetention->SetNumPlayer(nNumPlayer);
}

//============================================================
//	開始の更新処理
//============================================================
void CEntryManager::UpdateStart(void)
{
	// ポインタを宣言
	CInputPad *pPad = CManager::GetInstance()->GetPad();	// パッド

	for (int nCntEntry = 0; nCntEntry < MAX_PLAYER; nCntEntry++)
	{ // プレイヤーの最大数分繰り返す

		if (pPad->IsTrigger(CInputPad::KEY_START, nCntEntry))
		{
			if (CManager::GetInstance()->GetRetentionManager()->GetNumPlayer() >= 2)
			{ // エントリー数が二人以上の場合

				// シーンの設定
				CManager::GetInstance()->SetScene(CScene::MODE_GAME);	// ゲーム画面
			}
		}
	}
}
