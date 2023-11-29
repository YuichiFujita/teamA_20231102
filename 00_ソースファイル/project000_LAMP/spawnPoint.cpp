//============================================================
//
//	生成位置処理 [spawnPoint.cpp]
//	Author：森川 駿弥
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "spawnPoint.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 0;	// 生成位置の優先順位
}
//************************************************************
//	静的メンバ変数
//************************************************************
int CSpawnPoint::m_nNumIdx = 0;		// スポーン番号の個数

//************************************************************
//	子クラス [CSpawnPoint] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSpawnPoint::CSpawnPoint() : CObject(CObject::LABEL_SPAWNPOINT, PRIORITY), m_Idx(m_nNumIdx)
{
	// メンバ変数を初期化
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;

	// スポーン番号の個数を加算
	m_nNumIdx++;
}

//============================================================
//	デストラクタ
//============================================================
CSpawnPoint::~CSpawnPoint()
{
	// スポーン番号の個数を減算
	m_nNumIdx--;
}

//============================================================
//	初期化処理
//============================================================
HRESULT CSpawnPoint::Init(void)
{
	// メンバ変数を初期化
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSpawnPoint::Uninit(void)
{
	// 自身のオブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CSpawnPoint::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CSpawnPoint::Draw(void)
{

}

//============================================================
//	位置設定
//============================================================
void CSpawnPoint::SetVec3Position(const D3DXVECTOR3& rPos)
{
	m_pos = rPos;
}

//============================================================
//	位置取得
//============================================================
D3DXVECTOR3 CSpawnPoint::GetVec3Position(void) const
{
	return m_pos;
}

//============================================================
//	向き設定
//============================================================
void CSpawnPoint::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	m_rot = rRot;
	useful::Vec3NormalizeRot(m_rot);	// 向きの正規化
}

//============================================================
//	向き取得
//============================================================
D3DXVECTOR3 CSpawnPoint::GetVec3Rotation(void) const
{
	return m_rot;
}

//============================================================
//	番号取得
//============================================================
int CSpawnPoint::GetIndex(void) const
{
	return m_Idx;
}

//============================================================
//	生成処理
//============================================================
CSpawnPoint *CSpawnPoint::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot		// 向き
)
{
	// ポインタを宣言
	CSpawnPoint *pSpawnPoint = NULL;	// 生成位置生成用

	if (pSpawnPoint == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pSpawnPoint = new CSpawnPoint;	// 生成位置
	}
	else { assert(false); return NULL; }	// 使用中

	if (pSpawnPoint != NULL)
	{ // 確保に成功している場合

		// 生成位置の初期化
		if (FAILED(pSpawnPoint->Init()))
		{ // 初期化に失敗した場合

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pSpawnPoint->SetVec3Position(rPos);

		// 向きを設定
		pSpawnPoint->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pSpawnPoint;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	セーブポイント取得
//============================================================
CObject * CSpawnPoint::GetSavePoint(int Idx)
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

//============================================================
//	破棄処理
//============================================================
void CSpawnPoint::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}
