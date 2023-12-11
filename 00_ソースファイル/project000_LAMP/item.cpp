//============================================================
//
//	アイテムスポーン処理 [item.h]
//	Author：森川駿弥
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "item.h"
#include "player.h"
#include "input.h"
#include "scene.h"
#include "collision.h"

//============================================================
// 静的メンバ変数
//============================================================
const char *CItem::ModelFileName = "data\\MODEL\\PLAYER\\02_head.x";

namespace
{
	const float RADIUS = 50.0f;	//半径
}

//============================================================
// コンストラクタ
//============================================================
CItem::CItem()
{
	
}

//============================================================
// デストラクタ
//============================================================
CItem::~CItem()
{

}

//============================================================
// 初期化
//============================================================
HRESULT CItem::Init(void)
{
	return S_OK;
}

//============================================================
// 終了
//============================================================
void CItem::Uninit(void)
{
	// 継承元の終了
	CObjectModel::Uninit();
}

//============================================================
// 更新
//============================================================
void CItem::Update(void)
{
}

//============================================================
// 描画
//============================================================
void CItem::Draw(void)
{
	// 継承元の描画
	CObjectModel::Draw();
}

//============================================================
// 当たり判定
//============================================================
void CItem::Collision()
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		// CSceneのプレイヤー取得
		CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);

		if (pPlayer != nullptr)
		{
			//プレイヤーの位置取得
			D3DXVECTOR3 playerPos = pPlayer->GetVec3Position();
			//プレイヤーの半径取得
			float fPlayerRadius = pPlayer->GetRadius();

			//アイテムの位置取得
			D3DXVECTOR3 itemPos = GetVec3Position();
			//アイテムの半径取得
			float fItemRadisu = GetRadius();

			//円柱の当たり判定
			if (collision::Circle2D(playerPos, itemPos, fPlayerRadius, fItemRadisu) == true)
			{
				pPlayer->Hit();
			}
		}
	}
}

//============================================================
// 生成
//============================================================
CItem *CItem::Create(D3DXVECTOR3 pos)
{
	CItem *pItem = nullptr;

	if (pItem == nullptr)
	{// インスタンス生成
		pItem = new CItem;
	}

	if (pItem != nullptr)
	{
		if (FAILED(pItem->Init()))
		{
			//pItemをnullptrにする
			delete pItem;
			pItem = nullptr;

			//	nullptrを返す
			return nullptr;
		}

		// 位置設定
		pItem->SetVec3Position(pos);
	}

	// ポインタを返す
	return pItem;
}
