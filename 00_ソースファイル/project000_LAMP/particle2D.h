//============================================================
//
//	パーティクル2Dヘッダー [particle2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PARTICLE2D_H_
#define _PARTICLE2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// パーティクル2Dクラス
class CParticle2D : CObject
{
public:
	// コンストラクタ
	CParticle2D();

	// デストラクタ
	~CParticle2D();

	// パーティクル2D種類列挙
	enum EType
	{
		TYPE_NONE = 0,	// なし
		TYPE_DAMAGE,	// ダメージ
		TYPE_ITEM,		// アイテム
		TYPE_GETITEM,	// アイテム取得
		TYPE_MAX		// この列挙型の総数
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// オーバーロードしたオーバーライド関数
	HRESULT Init	// 初期化
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXCOLOR& rCol		// 色
	);

	// 静的メンバ関数
	static CParticle2D *Create	// 生成
	( // 引数
		const EType type,					// 種類
		const D3DXVECTOR3& rPos,			// 位置
		const D3DXCOLOR& rCol = XCOL_WHITE	// 色
	);

private:
	// メンバ関数
	void Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);	// ダメージ
	void Item(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);		// アイテム
	void GetItem(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);	// アイテム取得

	// 静的メンバ変数
	static const int mc_aLife[TYPE_MAX];	// 寿命定数

	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXCOLOR m_col;	// 色
	EType m_type;		// 種類
	int  m_nLife;		// 寿命
};

#endif	// _PARTICLE2D_H_
