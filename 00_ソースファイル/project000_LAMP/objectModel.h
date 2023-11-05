//============================================================
//
//	オブジェクトモデルヘッダー [objectModel.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECTMODEL_H_
#define _OBJECTMODEL_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"
#include "model.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトモデルクラス
class CObjectModel : public CObject
{
public:
	// コンストラクタ
	CObjectModel();
	CObjectModel(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectModel();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	void BindModel(const int nModelID) override;				// モデル割当 (インデックス)
	void BindModel(const char *pModelPass) override;			// モデル割当 (パス)
	void SetVec3Position(const D3DXVECTOR3& rPos) override;		// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;			// 位置取得
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;		// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const override;			// 向き取得
	void SetVec3Scaling(const D3DXVECTOR3& rScale) override;	// 拡大率設定
	D3DXVECTOR3 GetVec3Scaling(void) const override;			// 拡大率取得
	void SetAllMaterial(const D3DXMATERIAL& rMat) override;		// マテリアル全設定
	void ResetMaterial(void) override;							// マテリアル再設定
	D3DXMATRIX *GetPtrMtxWorld(void) override;					// マトリックスポインタ取得
	D3DXMATRIX GetMtxWorld(void) const override;				// マトリックス取得

	// 静的メンバ関数
	static CObjectModel *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR3& rScale = VEC3_ONE	// 大きさ
	);

	// メンバ関数
	void SetMaterial(const D3DXMATERIAL& rMat, const int nID);	// マテリアル設定
	D3DXMATERIAL GetMaterial(const int nID) const;		// マテリアル取得
	void SetAlpha(const float fAlpha);					// 透明度設定
	float GetAlpha(void) const;							// 透明度取得
	float GetMaxAlpha(void) const;						// 最大透明度取得
	void SetModelData(const CModel::SModel& rModel);	// モデル情報設定
	CModel::SModel GetModelData(void) const;			// モデル情報取得
	void SetMtxWorld(const D3DXMATRIX& rMtxWorld);		// マトリックス設定

private:
	// メンバ関数
	HRESULT SetOriginMaterial(const LPD3DXBUFFER pBuffMat, const int nNumMat);	// 元マテリアル設定

	// メンバ変数
	CModel::SModel	m_modelData;	// モデル情報
	D3DXMATERIAL	*m_pMat;		// マテリアルへのポインタ
	D3DXMATRIX		m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3		m_pos;			// 位置
	D3DXVECTOR3		m_rot;			// 向き
	D3DXVECTOR3		m_scale;		// 拡大率
	int				m_nModelID;		// モデルインデックス
};

#endif	// _OBJECTMODEL_H_
