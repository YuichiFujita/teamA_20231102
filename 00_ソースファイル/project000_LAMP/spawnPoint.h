//============================================================
//
//	生成位置ヘッダー [spawnPoint.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SPAWN_POINT_H_
#define _SPAWN_POINT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// 生成位置クラス
class CSpawnPoint : public CObject
{
public:
	// コンストラクタ
	CSpawnPoint();

	// デストラクタ
	~CSpawnPoint();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 三軸の位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 三軸の位置取得
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// 向き取得
	int GetIndex(void) const override;						// インデックス取得

	// 静的メンバ関数
	static CSpawnPoint *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);
	static CObject *GetSavePoint(int Idx);

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// メンバ変数
	const int m_Idx;		//スポーン番号
	static int m_nNumIdx;	//スポーン番号の個数
	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_rot;		//向き
};

#endif	// _SPAWN_POINT_H_
