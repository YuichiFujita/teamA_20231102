//============================================================
//
//	エディットアイテム生成位置処理 [editItemPoint.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editItemPoint.h"
#include "manager.h"
#include "editStageManager.h"
#include "stage.h"
#include "effect3D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_CREATE	(DIK_0)	// 生成キー
#define NAME_CREATE	("0")	// 生成表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float	POS_EFFECT_RADIUS = 120.0f;	// 位置表示のエフェクト半径
	const int	EFFECT_LIFE = 10;			// エフェクト寿命
}

//************************************************************
//	親クラス [CEditItemPoint] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditItemPoint::CEditItemPoint()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditItemPoint::~CEditItemPoint()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditItemPoint::Init(void)
{
#if _DEBUG

	// 成功を返す
	return S_OK;

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	終了処理
//============================================================
void CEditItemPoint::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditItemPoint::Update(void)
{
#if _DEBUG

	// ポインタを宣言
	CEditStageManager *pEdit = GetPtrEditStage();	// エディットステージ情報
	if (pEdit == NULL)
	{ // エディットステージが存在しない場合

		// 処理を抜ける
		assert(false);
		return;
	}

	// アイテム生成位置の生成
	CreateItemPoint();

	// アイテム生成位置の全表示
	LookAllItemPoint();

	// エディット位置の表示
	LookEffect(pEdit->GetVec3Position(), XCOL_RED);

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditItemPoint::DrawDebugControl(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditItemPoint::DrawDebugInfo(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報
}

//============================================================
//	情報保存処理
//============================================================
void CEditItemPoint::SaveInfo(void)
{

}

//============================================================
//	情報読込処理
//============================================================
void CEditItemPoint::LoadInfo(void)
{

}

//============================================================
//	保存処理
//============================================================
void CEditItemPoint::Save(FILE *pFile)
{
#if _DEBUG

	if (pFile != NULL)
	{ // ファイルが存在する場合

		// 見出しを書き出し
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#	アイテム生成位置の配置情報\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		// 情報開始地点を書き出し
		fprintf(pFile, "STAGE_ITEMPOINTSET\n\n");

		for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
		{ // 優先順位の総数分繰り返す
	
			// ポインタを宣言
			CObject *pObjectTop = CObject::GetTop(nCntPri);	// 先頭オブジェクト
	
			if (pObjectTop != NULL)
			{ // 先頭が存在する場合
	
				// ポインタを宣言
				CObject *pObjCheck = pObjectTop;	// オブジェクト確認用
	
				while (pObjCheck != NULL)
				{ // オブジェクトが使用されている場合繰り返す
		
					// ポインタを宣言
					CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト
	
					if (pObjCheck->GetLabel() != CObject::LABEL_ITEMPOINT)
					{ // オブジェクトラベルがアイテム生成位置ではない場合
	
						// 次のオブジェクトへのポインタを代入
						pObjCheck = pObjectNext;
	
						// 次の繰り返しに移行
						continue;
					}

					// アイテム生成位置の情報を取得
					D3DXVECTOR3 posItemPoint = pObjCheck->GetVec3Position();	// 位置
	
					// 情報を書き出し
					fprintf(pFile, "	ITEMPOINTSET\n");
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posItemPoint.x, posItemPoint.y, posItemPoint.z);
					fprintf(pFile, "	END_ITEMPOINTSET\n\n");

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;
				}
			}
		}

		// 情報終了地点を書き出し
		fprintf(pFile, "END_STAGE_ITEMPOINTSET\n\n");
	}

#endif	// _DEBUG
}

//============================================================
//	アイテム生成位置のエフェクト表示処理
//============================================================
void CEditItemPoint::LookEffect(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// アイテム生成位置にエフェクトを表示
	CEffect3D::Create(rPos, POS_EFFECT_RADIUS, CEffect3D::TYPE_NORMAL, EFFECT_LIFE, VEC3_ZERO, VEC3_ZERO, rCol);
}

//============================================================
//	アイテム生成位置の生成処理
//============================================================
void CEditItemPoint::CreateItemPoint(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報
	CEditStageManager *pEdit = GetPtrEditStage();	// エディットステージ情報
	if (pEdit == NULL)
	{ // エディットステージが存在しない場合

		// 処理を抜ける
		assert(false);
		return;
	}

	// 変数を宣言
	D3DXVECTOR3 posEdit = pEdit->GetVec3Position();	// エディットの位置

	// アイテム生成位置を配置
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		// アイテム生成位置の生成
		CItemSpawnPoint *pItemPoint = CItemSpawnPoint::Create(posEdit);
		assert(pItemPoint != NULL);

		// 未保存を設定
		pEdit->UnSave();
	}
}

//============================================================
//	アイテム生成位置の全表示
//============================================================
void CEditItemPoint::LookAllItemPoint(void)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		// ポインタを宣言
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// 先頭オブジェクト

		if (pObjectTop != NULL)
		{ // 先頭が存在する場合

			// ポインタを宣言
			CObject *pObjCheck = pObjectTop;	// オブジェクト確認用

			while (pObjCheck != NULL)
			{ // オブジェクトが使用されている場合繰り返す

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_ITEMPOINT)
				{ // オブジェクトラベルがアイテム生成位置ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 変数を宣言
				D3DXVECTOR3 posSpawn = pObjCheck->GetVec3Position();	// アイテム生成位置

				// アイテム生成位置のエフェクト表示
				LookEffect
				( // 引数
					posSpawn,	// 位置
					XCOL_CYAN	// 色
				);

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}
}
