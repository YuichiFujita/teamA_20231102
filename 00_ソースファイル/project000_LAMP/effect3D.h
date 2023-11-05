//============================================================
//
//	エフェクト3Dヘッダー [effect3D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectBillboard.h"

//************************************************************
//	クラス定義
//************************************************************
// エフェクト3Dクラス
class CEffect3D : public CObjectBillboard
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_NORMAL = 0,	// 通常テクスチャ
		TYPE_BUBBLE,		// バブルテクスチャ
		TYPE_SMOKE,			// 煙テクスチャ
		TYPE_HEAL,			// 回復テクスチャ
		TYPE_LEAF,			// 葉っぱテクスチャ
		TYPE_PIECE_S,		// かけらテクスチャ (小)
		TYPE_PIECE_M,		// かけらテクスチャ (中)
		TYPE_PIECE_L,		// かけらテクスチャ (大)
		TYPE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEffect3D();
	CEffect3D(const EType type, const CObject::ELabel label);

	// デストラクタ
	~CEffect3D();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CEffect3D *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const float fRadius,		// 半径
		const EType type = TYPE_NORMAL,			// テクスチャ
		const int nLife = 1,					// 寿命
		const D3DXVECTOR3& rMove = VEC3_ZERO,	// 移動量
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXCOLOR& rCol = XCOL_WHITE,		// 色
		const float fSubSize = 0.0f,			// 半径の減算量
		const bool bAdd = true,					// 加算合成状況
		const CObject::ELabel label = LABEL_EFFECT	// オブジェクトラベル
	);

private:
	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	D3DXVECTOR3 m_move;		// 移動量
	const EType m_type;		// 種類定数
	int		m_nLife;		// 寿命
	float	m_fSubSize;		// 大きさの減算量
	float	m_fSubAlpha;	// 透明度の減算量
	bool	m_bAdd;			// 加算合成状況
};

#endif	// _EFFECT3D_H_
