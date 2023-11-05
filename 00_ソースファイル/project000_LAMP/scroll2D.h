//============================================================
//
//	スクロール2Dヘッダー [scroll2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCROLL2D_H_
#define _SCROLL2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object2D.h"

//************************************************************
//	クラス定義
//************************************************************
// スクロール2Dクラス
class CScroll2D : public CObject2D
{
public:
	// コンストラクタ
	CScroll2D();
	CScroll2D(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CScroll2D();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// 大きさ設定
	void SetColor(const D3DXCOLOR& rCol) override;			// 色設定

	// オーバーロードしたオーバーライド関数
	HRESULT Init	// 初期化
	( // 引数
		const float fMoveU,	// 横座標の移動量
		const float fMoveV	// 縦座標の移動量
	);

	// 静的メンバ関数
	static CScroll2D *Create	// 生成
	( // 引数
		const float fMoveU,			// 横座標の移動量
		const float fMoveV,			// 縦座標の移動量
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rSize = VEC3_ONE,	// 大きさ
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXCOLOR& rCol = XCOL_WHITE		// 色
	);

	// メンバ関数
	void SetTexU(const float fTexU);	// 横座標の開始位置設定
	void SetTexV(const float fTexV);	// 縦座標の開始位置設定
	void SetMoveU(const float fMoveU);	// 横座標の移動量設定
	void SetMoveV(const float fMoveV);	// 縦座標の移動量設定

private:
	// メンバ変数
	float m_fTexU;	// テクスチャ横座標の開始位置
	float m_fTexV;	// テクスチャ縦座標の開始位置
	float m_fMoveU;	// テクスチャ横座標の移動量
	float m_fMoveV;	// テクスチャ縦座標の移動量
};

#endif	// _SCROLL2D_H_
