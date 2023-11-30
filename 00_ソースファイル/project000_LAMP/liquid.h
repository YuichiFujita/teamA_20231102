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
		TYPE_LAVA,
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

	// テクスチャ移動量構造体
	struct STexMove
	{
	public:
		// コンストラクタ
		STexMove() {};
		STexMove(const D3DXVECTOR2 TexMoveLow, const D3DXVECTOR2 TexMoveHigh)
		{ texMoveLow = TexMoveLow; texMoveHigh = TexMoveHigh; };

		// デストラクタ
		~STexMove() {};

		// メンバ変数
		union
		{
			struct
			{
				D3DXVECTOR2 texMoveLow;		// 下液体のテクスチャ移動量
				D3DXVECTOR2 texMoveHigh;	// 上液体のテクスチャ移動量
			};
			D3DXVECTOR2 aTexMove[2];	// テクスチャ移動量
		};
	};

	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// 向き取得
	void SetVec2Sizing(const D3DXVECTOR2& rSize) override;	// 大きさ設定
	D3DXVECTOR2 GetVec2Sizing(void) const override;			// 大きさ取得
	void SetColor(const D3DXCOLOR& rCol) override;			// 色設定
	D3DXCOLOR GetColor(void) const override;				// 色取得

	void SetType(const int nType) override;	// 種類設定
	int GetType(void) const override;		// 種類取得

	// 静的メンバ関数
	static CLiquid *Create	// 生成
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR2& rSize,	// 大きさ
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart,		// 分割数
		const STexMove& rTexMove,	// テクスチャ移動量
		const float fMaxUp,			// 波の最高上昇量
		const float fAddSinRot,		// 波打ち向き加算量
		const float fAddVtxRot		// 隣波の向き加算量
	);

	// メンバ関数
	HRESULT SetPattern(const POSGRID2& rPart);			// 分割数設定
	POSGRID2 GetPattern(void) const;					// 分割数取得
	void SetTexMove(const STexMove texMove);			// テクスチャ移動量設定
	STexMove GetTexMove(const ELiquid liquid) const;	// テクスチャ移動量取得

	void SetMaxUp(const float fMaxUp);			// 波の最高上昇量設定
	float GetMaxUp(void) const;					// 波の最高上昇量取得
	void SetAddSinRot(const float fAddSinRot);	// 波打ち向き加算量設定
	float GetAddSinRot(void) const;				// 波打ち向き加算量取得
	void SetAddVtxRot(const float fAddVtxRot);	// 隣波の向き加算量設定
	float GetAddVtxRot(void) const;				// 隣波の向き加算量取得

	CScrollMeshField *GetScrollMeshField(int num) const;	// 波のメッシュフィールド取得

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// 静的メンバ変数
	static const char *mc_apTextureFile[][LIQUID_MAX];	// テクスチャ定数

	// メンバ変数
	CScrollMeshField *m_apLiquid[LIQUID_MAX];	// 液体の情報
	EType m_type;		// 種類
	float m_fMaxUp;		// 波の最高上昇量
	float m_fSinRot;	// 波打ち向き
	float m_fAddSinRot;	// 波打ち向き加算量
	float m_fAddVtxRot;	// 隣波の向き加算量
};

#endif	// _LIQUID_H_
