//============================================================
//
//	オブジェクトメッシュフィールドヘッダー [objectMeshField.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_MESHFIELD_H_
#define _OBJECT_MESHFIELD_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトメッシュフィールドクラス
class CObjectMeshField : public CObject
{
public:
	// コンストラクタ
	CObjectMeshField();
	CObjectMeshField(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectMeshField();

	// メッシュフィールド構造体
	struct SMeshField
	{
		D3DXVECTOR3	pos;		// 位置
		D3DXVECTOR3	rot;		// 向き
		D3DXVECTOR2	size;		// 大きさ
		D3DXCOLOR	col;		// 色
		D3DXMATRIX	mtxWorld;	// ワールドマトリックス
		D3DCULL cull;	// カリング状況
		bool bLight;	// ライティング状況
	};

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
	void SetVec2Sizing(const D3DXVECTOR2& rSize) override;	// 大きさ設定
	D3DXVECTOR2 GetVec2Sizing(void) const override;			// 大きさ取得
	void SetColor(const D3DXCOLOR& rCol) override;			// 色設定
	D3DXCOLOR GetColor(void) const override;				// 色取得

	// 静的メンバ関数
	static CObjectMeshField *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR2& rSize,	// 大きさ
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart,		// 分割数
		const D3DCULL cull = D3DCULL_CCW,	// カリング状況
		const bool bLight = true			// ライティング状況
	);

	// メンバ関数
	void SetCulling(const D3DCULL cull);		// カリング設定
	D3DCULL GetCulling(void) const;				// カリング取得
	void SetLighting(const bool bLight);		// ライティング設定
	bool GetLighting(void) const;				// ライティング取得
	HRESULT SetPattern(const POSGRID2& rPart);	// 分割数設定
	POSGRID2 GetPattern(void) const;			// 分割数取得
	void SetGapPosition(const int nID, const D3DXVECTOR3& rPos);	// 座標のずれ設定
	D3DXVECTOR3 GetGapPosition(const int nID);						// 座標のずれ取得

	void NormalizeNormal(void);									// 法線の正規化
	bool LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove);	// メッシュ着地

	void SetMeshVertexPosition(const int nID, const D3DXVECTOR3& rPos);	// メッシュの頂点位置設定
	void SetTerrain(const POSGRID2& rPart, D3DXVECTOR3 *pPosGap);		// 地形設定

	int GetNumVertex(void) const;							// 頂点数取得
	D3DXVECTOR3 GetMeshVertexPosition(const int nID);		// メッシュの頂点位置取得
	bool IsPositionRange(const D3DXVECTOR3&rPos);			// メッシュの範囲内取得 (回転考慮)
	float GetPositionHeight(const D3DXVECTOR3&rPos);		// メッシュの着地位置取得 (回転非考慮)
	float GetPositionRotateHeight(const D3DXVECTOR3&rPos);	// メッシュの着地位置取得 (回転考慮)

protected:
	// メンバ関数
	void SetVtx(bool bNor);	// 頂点情報の設定
	void SetIdx(void);		// インデックス情報の設定

	void SetScrollTex		// スクロールのテクスチャ座標の設定
	( // 引数
		const float fTexU,	// テクスチャの横座標の開始位置
		const float fTexV	// テクスチャの縦座標の開始位置
	);

private:
	// メンバ関数
	D3DXVECTOR3 GetNormalLeft(VERTEX_3D *pVtx);			// 法線の取得 (左)
	D3DXVECTOR3 GetNormalLeftTop(VERTEX_3D *pVtx);		// 法線の取得 (左上)
	D3DXVECTOR3 GetNormalLeftBottom(VERTEX_3D *pVtx);	// 法線の取得 (左下)
	D3DXVECTOR3 GetNormalRight(VERTEX_3D *pVtx);		// 法線の取得 (右)
	D3DXVECTOR3 GetNormalRightTop(VERTEX_3D *pVtx);		// 法線の取得 (右上)
	D3DXVECTOR3 GetNormalRightBottom(VERTEX_3D *pVtx);	// 法線の取得 (右下)

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;	// インデックスバッファへのポインタ
	D3DXVECTOR3 *m_pPosGapBuff;	// 座標のずれバッファへのポインタ
	D3DXVECTOR3 *m_pNorBuff;	// 法線バッファへのポインタ
	int *m_pNumNorBuff;			// 法線の使用数バッファへのポインタ

	SMeshField m_meshField;	// メッシュフィールドの情報
	POSGRID2 m_part;		// 分割数
	int m_nNumVtx;			// 必要頂点数
	int m_nNumIdx;			// 必要インデックス数
	int m_nTextureID;		// テクスチャインデックス
};

#endif	// _OBJECT_MESHFIELD_H_
