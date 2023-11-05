//============================================================
//
//	オブジェクト3Dヘッダー [object3D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクト3Dクラス
class CObject3D : public CObject
{
public:
	// 原点列挙
	enum EOrigin
	{
		ORIGIN_CENTER = 0,	// 中心原点
		ORIGIN_DOWN,		// 下原点
		ORIGIN_MAX,			// この列挙型の総数
	};

	// コンストラクタ
	CObject3D();
	CObject3D(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObject3D();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	void BindTexture(const int nTextureID) override;		// テクスチャ割当 (インデックス)
	void BindTexture(const char *pTexturePass) override;	// テクスチャ割当 (パス)
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// 向き取得
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// 大きさ設定
	D3DXVECTOR3 GetVec3Sizing(void) const override;			// 大きさ取得
	void SetColor(const D3DXCOLOR& rCol) override;			// 色設定
	D3DXCOLOR GetColor(void) const override;				// 色取得

	// 静的メンバ関数
	static CObject3D *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,					// 位置
		const D3DXVECTOR3& rSize,					// 大きさ
		const D3DXVECTOR3& rRot = VEC3_ZERO,		// 向き
		const D3DXCOLOR& rCol = XCOL_WHITE,			// 色
		const EOrigin origin = ORIGIN_CENTER,		// 原点
		const D3DCULL cull = D3DCULL_CCW,			// カリング状況
		const bool bLight = true,					// ライティング状況
		const D3DCMPFUNC func = D3DCMP_LESSEQUAL,	// Zテスト設定
		const bool bZEnable = true					// Zバッファの使用状況
	);

	// メンバ関数
	void SetOrigin(const EOrigin origin);	// 原点設定
	EOrigin GetOrigin(void) const;			// 原点取得
	void SetCulling(const D3DCULL cull);	// カリング設定
	D3DCULL GetCulling(void) const;			// カリング取得
	void SetLighting(const bool bLight);	// ライティング設定
	bool IsLighting(void) const;			// ライティング取得
	void SetFunc(const D3DCMPFUNC func);	// Zテスト設定
	D3DCMPFUNC GetFunc(void) const;			// Zテスト取得
	void SetZEnable(const bool bEnable);	// Zバッファの使用状況設定
	bool IsZEnable(void) const;				// Zバッファの使用状況取得

	void SetVertexPosition(const int nID, const D3DXVECTOR3& rPos);	// 頂点位置設定
	D3DXVECTOR3 GetVertexPosition(const int nID);					// 頂点位置取得
	void SetGapPosition(const int nID, const D3DXVECTOR3& rPos);	// 座標のずれ設定
	D3DXVECTOR3 GetGapPosition(const int nID);						// 座標のずれ取得
	float GetPositionHeight(const D3DXVECTOR3&rPos);				// ポリゴンの着地取得

protected:
	// メンバ関数
	void SetVtx(void);	// 頂点情報の設定

private:
	// メンバ関数
	void NormalizeNormal(void);	// 法線の正規化

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXVECTOR3 *m_pPosGapBuff;			// 座標のずれバッファへのポインタ
	D3DXMATRIX  m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3	m_pos;		// 位置
	D3DXVECTOR3	m_rot;		// 向き
	D3DXVECTOR3	m_size;		// 大きさ
	D3DXCOLOR	m_col;		// 色
	EOrigin		m_origin;	// 原点
	D3DCMPFUNC	m_func;		// Zテスト設定
	bool	m_bZEnable;		// Zバッファの使用状況
	D3DCULL	m_cull;			// カリング状況
	bool	m_bLight;		// ライティング状況
	int		m_nTextureID;	// テクスチャインデックス
};

#endif	// _OBJECT3D_H_
