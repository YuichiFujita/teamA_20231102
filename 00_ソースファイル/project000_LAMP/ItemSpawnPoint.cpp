//<==============================================
//
//アイテムスポーンポイント処理 [ItemSpawnPoint.cpp]
//Author：kazuki watanebe
//
//<==============================================
#include "ItemSpawnPoint.h"
#include "manager.h"
#include "renderer.h"

//<********************************
//静的メンバ関数の設定
//<********************************
int CItemSpawnPoint::m_nNumIdx = 0;		// スポーン番号の個数

//<==================================
//コンストラクタ
//<==================================
CItemSpawnPoint::CItemSpawnPoint() : CObject(CObject::LABEL_ITEMPOINT, 0), m_nIdx(m_nNumIdx)
{
	//値の初期化
	m_pos = VEC3_ZERO;

	//番号を加算
	m_nNumIdx++;
}
//<==================================
//デストラクタ
//<==================================
CItemSpawnPoint::~CItemSpawnPoint()
{
	//番号を減算
	m_nNumIdx--;
}
//<==================================
//初期化
//<==================================
HRESULT CItemSpawnPoint::Init(void) 
{
	//値の初期化
	m_pos = VEC3_ZERO;

	return S_OK;
}		
//<==================================
//終了処理
//<==================================
void CItemSpawnPoint::Uninit(void) 
{
	Release();
}			
//<=======================================
//アイテムスポーンポイントの生成処理
//<=======================================
CItemSpawnPoint *CItemSpawnPoint::Create(const D3DXVECTOR3& rPos)
{
	CItemSpawnPoint *pItemSpawnPoint = new CItemSpawnPoint;

	assert(pItemSpawnPoint != nullptr);

	//成功したら
	if (SUCCEEDED(pItemSpawnPoint->Init()))
	{
		// 位置を設定
		pItemSpawnPoint->SetVec3Position(rPos);

		return pItemSpawnPoint;
	}
	//失敗したら
	else
	{
		return nullptr;
	}

	return nullptr;
}
//<=======================================
//番号の設定
//<=======================================
CObject *CItemSpawnPoint::GetPoint(int Idx)
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

				if (pObjCheck->GetLabel() != CObject::LABEL_SPAWNPOINT)
				{ // オブジェクトラベルがスポーンポイントではない場合

				  // 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				if (pObjCheck->GetIndex() != Idx)
				{ // 番号が引き数の番号ではない場合

				  // 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 現在のオブジェクトを返す
				return pObjCheck;
			}
		}
	}

	// nullptrを返す
	return nullptr;
}