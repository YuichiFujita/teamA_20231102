//============================================================
//
//	オブジェクトメッシュドームヘッダー [objectMeshDome.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_MESHDOME_H_
#define _OBJECT_MESHDOME_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトメッシュドームクラス
class CObjectMeshDome : public CObject
{
public:
	// コンストラクタ
	CObjectMeshDome();
	CObjectMeshDome(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectMeshDome();

	// メッシュドーム構造体
	struct SMeshDome
	{
		D3DXVECTOR3	pos;		// 位置
		D3DXVECTOR3	rot;		// 向き
		D3DXCOLOR	col;		// 色
		D3DXMATRIX	mtxWorld;	// ワールドマトリックス
		float fRadius;	// 半径
		D3DCULL cull;	// カリング状況
		bool bLight;	// ライティング状況
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;	// 終了
	void Update(void) override;	// 更新
	void Draw(void) override;	// 描画

	void BindTexture(const int nTextureID) override;		// テクスチャ割当 (インデックス)
	void BindTexture(const char *pTexturePass) override;	// テクスチャ割当 (パス)
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// 向き取得

	void SetColor(const D3DXCOLOR& rCol) override;	// 色設定
	D3DXCOLOR GetColor(void) const override;		// 色取得
	void SetRadius(const float fRadius) override;	// 半径設定
	float GetRadius(void) const override;			// 半径取得

	// 静的メンバ関数
	static CObjectMeshDome *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart,		// 分割数
		const float fRadius,		// 半径
		const D3DCULL cull = D3DCULL_CCW,	// カリング状況
		const bool bLight = true			// ライティング状況
	);

	// メンバ関数
	void SetCulling(const D3DCULL cull);	// カリング設定
	D3DCULL GetCulling(void) const;			// カリング取得
	void SetLighting(const bool bLight);	// ライティング設定
	bool GetLighting(void) const;			// ライティング取得

	HRESULT SetPattern(const POSGRID2& rPart);		// 分割数設定
	POSGRID2 GetPattern(void) const;				// 分割数取得
	void SetTexPattern(const POSGRID2& rTexPart);	// テクスチャ分割数設定
	POSGRID2 GetTexPattern(void) const;				// テクスチャ分割数取得

protected:
	// メンバ関数
	void SetVtx(void);	// 頂点情報の設定
	void SetIdx(void);	// インデックス情報の設定

	void SetScrollTex	// スクロールのテクスチャ座標の設定
	( // 引数
		const float fTexU,	// テクスチャの横座標の開始位置
		const float fTexV	// テクスチャの縦座標の開始位置
	);

private:
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;	// インデックスバッファへのポインタ

	SMeshDome m_meshDome;	// メッシュドームの情報
	POSGRID2 m_part;		// 分割数
	POSGRID2 m_texPart;		// テクスチャ分割数
	int m_nNumVtx;			// 必要頂点数
	int m_nNumIdx;			// 必要インデックス数
	int m_nTextureID;		// テクスチャインデックス
};

#endif	// _OBJECT_MESHDOME_H_
