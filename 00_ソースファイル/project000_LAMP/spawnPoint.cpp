//============================================================
//
//	生成位置処理 [spawnPoint.cpp]
//	Author：藤田勇一
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
//	子クラス [CSpawnPoint] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSpawnPoint::CSpawnPoint() : CObject(CObject::LABEL_SPAWNPOINT, PRIORITY)
{
	// メンバ変数をクリア

}

//============================================================
//	デストラクタ
//============================================================
CSpawnPoint::~CSpawnPoint()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSpawnPoint::Init(void)
{
	// メンバ変数を初期化


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

		// 確保したアドレスを返す
		return pSpawnPoint;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
void CSpawnPoint::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}
