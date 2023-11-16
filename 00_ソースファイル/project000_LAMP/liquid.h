//============================================================
//
//	液体ヘッダー [liquid.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _LIQUID_H_
#define _LIQUID_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CScrollMeshField;	// スクロールメッシュフィールドクラス

//************************************************************
//	クラス定義
//************************************************************
// 液体クラス
class CLiquid : public CObject
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_SEA = 0,	// 液体
		TYPE_MAX		// この列挙型の総数
	};

	// 上下列挙
	enum ELiquid
	{
		LIQUID_LOW = 0,	// 下
		LIQUID_HIGH,	// 上
		LIQUID_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CLiquid();

	// デストラクタ
	~CLiquid();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CLiquid *Create(void);	// 生成

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// 静的メンバ変数
	static const char *mc_apTextureFile[][LIQUID_MAX];	// テクスチャ定数

	// メンバ変数
	CScrollMeshField *m_apLiquid[LIQUID_MAX];	// 液体の情報
	float m_fSinRot;	// 波打ち向き
};

#endif	// _LIQUID_H_
