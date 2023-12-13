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
const char *CItem::mc_apModelFile[TYPE::TYPE_MAX] =
{
	"data\\MODEL\\PLAYER\\02_head.x",				//回復アイテム
	"data\\MODEL\\OBSTACLE\\Castle-Fence000.x",		//攻撃力アップ
	"data\\MODEL\\OBSTACLE\\Castle-Fence001.x",		//ノックバック強化
	"data\\MODEL\\OBSTACLE\\Conifer000.x",			//スーパーアーマー
	"data\\MODEL\\OBSTACLE\\Fountain000.x",			//巨大鉄球
	"data\\MODEL\\OBSTACLE\\Hardwood000.x",			//すり抜け鎖
	"data\\MODEL\\OBSTACLE\\Jar000.x",				//ダブル鎖
	"data\\MODEL\\OBSTACLE\\Jar001.x",				//燃えている鎖
};

//<***************************************************
//名前宣言
//<***************************************************
namespace
{
	const float RADIUS = 50.0f;	//半径
}

//============================================================
// コンストラクタ
//============================================================
CItem::CItem()
{
	//値の初期化
	m_nType = TYPE::TYPE_HEAL;
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
	//抽出する(燃えている鎖～回復アイテムまで抽出)
	m_nType = rand() % TYPE::TYPE_BURNING_FRAIL + TYPE::TYPE_HEAL;

	//初期化を行う
	CObjectModel::Init();

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

		//ランダムでアイテムを生成する
		pItem->BindModel(mc_apModelFile[pItem->m_nType]);
	}

	// ポインタを返す
	return pItem;
}
