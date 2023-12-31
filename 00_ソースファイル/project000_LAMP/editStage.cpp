//============================================================
//
//	エディットステージ処理 [editStage.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editStage.h"
#include "manager.h"
#include "editStageManager.h"
#include "editGround.h"
#include "editBlock.h"
#include "editObstacle.h"
#include "editSpawnPoint.h"
#include "editItemPoint.h"

//************************************************************
//	親クラス [CEditStage] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditStage::CEditStage()
{
#if _DEBUG

	// メンバ変数をクリア
	m_pEdit = NULL;	// エディットステージの情報

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditStage::~CEditStage()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	生成処理
//============================================================
CEditStage *CEditStage::Create(CEditStageManager *pEdit, const EThing thing)
{
#if _DEBUG

	// ポインタを宣言
	CEditStage *pEditStage = NULL;	// エディットステージ生成用

	if (pEditStage == NULL)
	{ // 使用されていない場合

		// メモリ確保
		switch (thing)
		{ // 配置物ごとの処理
		case THING_GROUND:
			pEditStage = new CEditGround;		// エディット地盤
			break;

		case THING_BLOCK:
			pEditStage = new CEditBlock;		// エディットブロック
			break;

		case THING_OBSTACLE:
			pEditStage = new CEditObstacle;		// エディット障害物
			break;

		case THING_SPAWNPOINT:
			pEditStage = new CEditSpawnPoint;	// エディット生成位置
			break;

		case THING_ITEMPOINT:
			pEditStage = new CEditItemPoint;	// エディットアイテム生成位置
			break;

		default:
			assert(false);
			break;
		}
	}
	else { assert(false); return NULL; }	// 使用中

	if (pEditStage != NULL)
	{ // 使用されている場合

		// エディットステージの情報を設定
		pEditStage->m_pEdit = pEdit;
		
		// エディットステージの初期化
		if (FAILED(pEditStage->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pEditStage;
			pEditStage = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pEditStage;
	}
	else { assert(false); return NULL; }	// 確保失敗

#else	// NDEBUG

	// NULLを返す
	return NULL;

#endif	// _DEBUG
}

//============================================================
//	破棄処理
//============================================================
HRESULT CEditStage::Release(CEditStage *&prEditStage)
{
#if _DEBUG

	if (prEditStage != NULL)
	{ // 使用中の場合

		// エディットステージの終了
		prEditStage->Uninit();

		// メモリ開放
		delete prEditStage;
		prEditStage = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	エディットステージ情報取得処理
//============================================================
CEditStageManager *CEditStage::GetPtrEditStage(void) const
{
#if _DEBUG

	// エディット情報を返す
	return m_pEdit;

#else	// NDEBUG

	// NULLを返す
	return NULL;

#endif	// _DEBUG
}
