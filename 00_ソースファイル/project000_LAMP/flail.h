//============================================================
//
//	モデルUIヘッダー [flail.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FLAIL_H_
#define _FLAIL_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	クラス定義
//************************************************************
// モデルUIクラス
class CFlail : public CObjectModel
{
public:
	// コンストラクタ
	CFlail();

	// デストラクタ
	~CFlail();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	void SetVec3Move(const D3DXVECTOR3& rMove);		// 位置設定
	D3DXVECTOR3 GetVec3Move(void);			// 位置取得

	// 静的メンバ関数
	static CFlail *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXVECTOR3& rScale = VEC3_ONE	// 大きさ
	);

private:

	// メンバ変数
	D3DXVECTOR3 m_move;			// 移動量

};

#endif	// _FLAIL_H_
