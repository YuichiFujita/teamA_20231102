//<==============================================
//
//アイテムスポーンポイント処理 [ItemSpawnPoint.cpp]
//Author：kazuki watanebe
//
//<==============================================
#include "ItemSpawnPoint.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"

//<********************************
//静的メンバ関数の設定
//<********************************
int CItemSpawnPoint::m_nNumIdx = 0;		// スポーン番号の個数
CItem *CItemSpawnPoint::m_pItem = nullptr;
D3DXVECTOR3 CItemSpawnPoint::m_rPos[MAX_SPAWNPOINT] = {};
int CItemSpawnPoint::m_nCnt = 0;

//<**************************************
//名前定義
//<**************************************
namespace ItemSpawn
{
	const int MAX_TIME = 250;	//クールタイムの最大値
}

//<==================================
//コンストラクタ
//<==================================
CItemSpawnPoint::CItemSpawnPoint() : CObject(CObject::LABEL_ITEMPOINT, 0), m_nIdx(m_nNumIdx)
{
	//値の初期化
	m_nRand = 0;

	//番号を加算
	m_nNumIdx++;
}
//<==================================
//デストラクタ(主に静的メンバ変数を終了させるのに使う)
//<==================================
CItemSpawnPoint::~CItemSpawnPoint()
{
	//番号を減算
	m_nNumIdx--;

	//中身があったら
	if (m_nCnt != 0)
	{
		m_nCnt = 0;
	}
	
	//もし初期化されていなければ
	if (m_rPos[m_nNumIdx] != VEC3_ZERO)
	{
		//位置の初期化
		m_rPos[m_nNumIdx] = VEC3_ZERO;
	}
}
//<==================================
//初期化
//<==================================
HRESULT CItemSpawnPoint::Init(void) 
{
	//初期化をする
	m_nRand = 0;
	
	return S_OK;
}		
//<==================================
//終了処理
//<==================================
void CItemSpawnPoint::Uninit(void) 
{
	Release();

	//中身チェック
	if (m_pItem != nullptr)
	{
		m_pItem->Uninit();
		m_pItem = nullptr;
	}
}			
//<=======================================
//アイテムスポーンポイントの生成処理
//<=======================================
CItemSpawnPoint *CItemSpawnPoint::Create(const D3DXVECTOR3& rPos)
{
	//インスタンス生成用のオブジェクト
	CItemSpawnPoint *pItemSpawnPoint = new CItemSpawnPoint;

	//nullptrになっていないことを確認する
	assert(pItemSpawnPoint != nullptr);

	//成功したら
	if (SUCCEEDED(pItemSpawnPoint->Init()))
	{
		// 位置を設定
		pItemSpawnPoint->SetVec3Position(rPos);

		//位置を取得してくる
		m_rPos[pItemSpawnPoint->m_nIdx] = pItemSpawnPoint->GetVec3Position();

		//そのポインタを返す
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
//更新処理
//<=======================================
void CItemSpawnPoint::Update(void)
{
	//もしゲームモードだったら
	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::EMode::MODE_GAME)
	{
		//もしこのアイテムがnullptrではない+コリジョンが当たっていた場合
		if (m_pItem != nullptr
			&&m_pItem->Collision() == true)
		{
			m_pItem->Uninit();
			m_pItem =nullptr;
		}

		//アイテムがなければ
		if (m_pItem == nullptr)
		{
			//加算していく
			m_nCnt++;

			//もし規定値を超えていれば
			if (m_nCnt >= ItemSpawn::MAX_TIME)
			{
				//決定する
				m_nRand = rand() % m_nNumIdx + 0;

				//生成処理を行う(番号から位置を取得してくる)
				m_pItem = CItem::Create(m_rPos[m_nRand]);

				//初期化する
				m_nCnt = 0;
			}
		}
		//あったら
		else if (m_pItem != nullptr)
		{
			//何もしない
		}
	}
	
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