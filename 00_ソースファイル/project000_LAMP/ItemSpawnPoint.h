//============================================================
//
//アイテムスポーンポイント処理 [ItemSpawnPoint.h]
//Author：kazuki watanebe
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ITEMSPAWNPOINT_H_
#define _ITEMSPAWNPOINT_H_

#include "main.h"
#include "object.h"
#include "Item.h"



//<**************************************
//クラス宣言
//<**************************************
class CItemSpawnPoint : public CObject
{
public:

	//コンストラクタ・デストラクタ
	CItemSpawnPoint();
	~CItemSpawnPoint();

	//オーバーライド関数
	HRESULT Init(void) override;			//初期化
	void Uninit(void) override;				//終了
	void Update(void) override;				//更新
	void Draw(void) override {};		// 描画

	//<****************
	//位置系
	//<****************
	void SetVec3Position(const D3DXVECTOR3& rPos) override { m_pos = rPos; }	// 三軸の位置設定
	D3DXVECTOR3 GetVec3Position(void) const override { return m_pos; }			// 三軸の位置取得
	int GetIndex(void) const override { return m_nIdx; }						// インデックス取得

	// 静的メンバ関数
	static CItemSpawnPoint *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos	// 位置
	);

	static CObject *GetPoint(int Idx);

private:

	//スポーンポイントの最大値
#define MAX_SPAWNPOINT (20)

	// オーバーライド関数
	void Release(void) override {CObject::Release();}	// 破棄

	// メンバ変数
	const int m_nIdx;		//スポーン番号
	static int m_nNumIdx;	//スポーン番号の個数
	D3DXVECTOR3 m_pos;
	static D3DXVECTOR3 m_rPos[MAX_SPAWNPOINT];		//位置
	static CItem *m_pItem;
	static int m_nCnt;
	int m_nRand;
};

#endif

