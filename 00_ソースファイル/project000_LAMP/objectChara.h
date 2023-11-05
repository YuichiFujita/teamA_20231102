//============================================================
//
//	オブジェクトキャラクターヘッダー [objectChara.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECTCHARA_H_
#define _OBJECTCHARA_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"
#include "motion.h"

//************************************************************
//	前方宣言
//************************************************************
class CMultiModel;	// マルチモデルクラス
class CMotion;		// モーションクラス

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトキャラクタークラス
class CObjectChara : public CObject
{
public:
	// コンストラクタ
	CObjectChara();
	CObjectChara(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectChara();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// 向き取得
	D3DXMATRIX *GetPtrMtxWorld(void) override;				// マトリックスポインタ取得

	// メンバ関数
	void SetPartsInfo	// パーツ情報設定
	( // 引数
		const int nID,				// パーツインデックス
		const int nParentID,		// 親インデックス
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const char *pFileName		// ファイル名
	);
	void SetModelInfo(void);						// モデル情報設定
	void SetMotionInfo(CMotion::SMotionInfo info);	// モーション情報設定
	void SetMotion(const int nType);				// モーション設定
	void SetEnableMotionUpdate(const bool bUpdate);	// 更新状況設定

	void SetPartsPosition(const int nID, const D3DXVECTOR3& rPos);	// パーツ位置設定
	D3DXVECTOR3 GetPartsPosition(const int nID) const;				// パーツ位置取得
	void SetPartsRotation(const int nID, const D3DXVECTOR3& rRot);	// パーツ向き設定
	D3DXVECTOR3 GetPartsRotation(const int nID) const;				// パーツ向き取得
	CMultiModel *GetMultiModel(const int nID) const;				// マルチモデル取得

	int  GetMotionType(void) const;				// モーション種類取得
	int  GetMotionPose(void) const;				// モーションポーズ番号取得
	int  GetMotionCounter(void) const;			// モーションカウンター取得
	bool IsMotionFinish(void) const;			// モーション終了取得
	bool IsMotionLoop(const int nType) const;	// モーションループ取得
	void SetMaterial(const D3DXMATERIAL& rMat);	// マテリアル設定
	void ResetMaterial(void);					// マテリアル再設定
	void SetAlpha(const float fAlpha);			// 透明度設定
	float GetAlpha(void) const;					// 透明度取得
	float GetMaxAlpha(void) const;				// 最大透明度取得

private:
	// メンバ変数
	CMultiModel	*m_apMultiModel[MAX_PARTS];	// モデルの情報
	CMotion		*m_pMotion;		// モーションの情報
	D3DXMATRIX	m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3	m_pos;			// 位置
	D3DXVECTOR3	m_rot;			// 向き
	int			m_nNumModel;	// パーツの総数
};

#endif	// _OBJECTCHARA_H_
