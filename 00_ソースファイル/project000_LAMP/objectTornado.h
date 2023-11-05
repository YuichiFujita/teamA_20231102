//============================================================
//
//	オブジェクト竜巻ヘッダー [objectTornado.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_TORNADO_H_
#define _OBJECT_TORNADO_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクト竜巻クラス
class CObjectTornado : public CObject
{
public:
	// コンストラクタ
	CObjectTornado();
	CObjectTornado(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectTornado();

	// 竜巻構造体
	struct STornado
	{
		D3DXVECTOR3	pos;		// 位置
		D3DXVECTOR3	rot;		// 向き
		D3DXVECTOR3	direRot;	// 成長向き
		D3DXCOLOR	col;		// 色
		D3DXMATRIX	mtxWorld;	// ワールドマトリックス
		D3DXMATRIX *pMtxParent;	// 親のマトリックス
		float fMoveRot;			// 向きの変更量
		float fThickness;		// ポリゴンの太さ
		float fOuterPlus;		// ポリゴン外周の y座標加算量
		float fSetWidth;		// 生成時の横ずれ量
		float fSetAlpha;		// 生成時の透明度
		float fAddWidth;		// 横ずれの加算量
		float fAddHeight;		// 縦ずれの加算量
		float fSubAlpha;		// 透明度の減算量
		float fGrowWidth;		// 横ずれの成長量
		float fGrowHeight;		// 縦ずれの成長量
		float fGrowAlpha;		// 透明度の成長量
		D3DCULL cull;			// カリング状況
		bool bLight;			// ライティング状況
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetColor(const D3DXCOLOR& rCol) override;			// 色設定
	D3DXCOLOR GetColor(void) const override;				// 色取得

	// 静的メンバ関数
	static CObjectTornado *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,			// 位置
		const D3DXVECTOR3& rDireRot,		// 成長向き
		const D3DXCOLOR& rCol,				// 色
		D3DXMATRIX *pMtxParent,				// 親のマトリックス
		const int   nNumAround,				// 渦の周回数
		const int   nPattern,				// 渦の分割数
		const float fMoveRot,				// 向きの変更量
		const float fThickness,				// ポリゴンの太さ
		const float fOuterPlus,				// ポリゴン外周の y座標加算量
		const float fSetWidth,				// 生成時の横ずれ量
		const float fSetAlpha,				// 生成時の透明度
		const float fAddWidth,				// 横ずれの加算量
		const float fAddHeight,				// 縦ずれの加算量
		const float fSubAlpha,				// 透明度の減算量
		const float fGrowWidth,				// 横ずれの成長量
		const float fGrowHeight,			// 縦ずれの成長量
		const float fGrowAlpha,				// 透明度の成長量
		const D3DCULL cull = D3DCULL_NONE,	// カリング状況
		const bool bLight = false			// ライティング状況
	);

	// メンバ関数
	void SetGrow	// 成長設定
	( // 引数
		float fMoveRot,		// 向きの変更量
		float fGrowWidth,	// 横ずれの成長量
		float fGrowHeight,	// 縦ずれの成長量
		float fGrowAlpha	// 透明度の成長量
	);
	HRESULT SetVortex	// 渦設定
	( // 引数
		const int nNumAround,	// 渦の周回数
		const int nPattern		// 渦の分割数
	);

	void DeleteMatrixParent(void);						// 親マトリックス削除
	void SetMatrixParent(D3DXMATRIX *pMtxParent);		// 親マトリックス設定
	void SetDirectionRotation(const D3DXVECTOR3& rRot);	// 成長向き設定
	D3DXVECTOR3 GetDirectionRotation(void) const;		// 成長向き取得

	void SetThickness(const float fThickness);	// ポリゴンの太さ設定
	float GetThickness(void) const;				// ポリゴンの太さ取得
	void SetOuterPlus(const float fOuterPlus);	// ポリゴン外周の y座標加算量設定
	float GetOuterPlus(void) const;				// ポリゴン外周の y座標加算量取得
	void SetWidth(const float fSetWidth);		// 生成時の横ずれ量設定
	float GetWidth(void) const;					// 生成時の横ずれ量取得
	void SetAlpha(const float fSetAlpha);		// 生成時の透明度設定
	float GetAlpha(void) const;					// 生成時の透明度取得
	void SetAddWidth(const float fAddWidth);	// 横ずれの加算量設定
	float GetAddWidth(void) const;				// 横ずれの加算量取得
	void SetAddHeight(const float fAddHeight);	// 縦ずれの加算量設定
	float GetAddHeight(void) const;				// 縦ずれの加算量取得
	void SetSubAlpha(const float fSubAlpha);	// 透明度の減算量設定
	float GetSubAlpha(void) const;				// 透明度の減算量取得
	void SetCulling(const D3DCULL cull);		// カリング設定
	D3DCULL GetCulling(void) const;				// カリング取得
	void SetLighting(const bool bLight);		// ライティング設定
	bool GetLighting(void) const;				// ライティング取得

protected:
	// メンバ関数
	void SetVtx(void);	// 頂点情報の設定

private:
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	STornado m_tornado;	// 竜巻の情報
	int m_nNumVtx;		// 必要頂点数
	int m_nNumAround;	// 渦の周回数
	int m_nPattern;		// 渦の分割数
	int m_nTextureID;	// テクスチャインデックス
};

#endif	// _OBJECT_TORNADO_H_
