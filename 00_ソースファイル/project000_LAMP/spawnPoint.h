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

	// 静的メンバ関数
	static CSpawnPoint *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// メンバ変数

};

#endif	// _SPAWN_POINT_H_
