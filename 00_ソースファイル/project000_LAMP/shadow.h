//============================================================
//
//	影ヘッダー [shadow.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SHADOW_H_
#define _SHADOW_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object3D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MIN_ALPHA	(0.25f)	// α値の最小値
#define MAX_ALPHA	(0.65f)	// α値の最大値

//************************************************************
//	クラス定義
//************************************************************
// 影クラス
class CShadow : public CObject3D
{
public:
	// テクスチャ列挙
	enum ETexture
	{
		TEXTURE_NORMAL = 0,	// 通常テクスチャ
		TEXTURE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CShadow(const float fMinAlpha, const float fMaxAlpha);

	// デストラクタ
	~CShadow();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CShadow *Create	// 生成
	( // 引数
		const ETexture texture,				// 種類
		const D3DXVECTOR3& rSize,			// 大きさ
		CObject *pObject,					// 親オブジェクト
		const float fMinAlpha = MIN_ALPHA,	// 透明度の最小値
		const float fMaxAlpha = MAX_ALPHA	// 透明度の最大値
	);

	// メンバ関数
	void SetScalingOrigin(const D3DXVECTOR3& rSize);	// 元の大きさ設定
	HRESULT SetDrawInfo(void);			// 描画情報設定
	D3DXVECTOR3 SetDrawPosition(void);	// 描画位置設定
	void DeleteObjectParent(void);		// 親オブジェクト削除
	bool CollisionObstacle(D3DXVECTOR3& rPos, float& rDrawPos);	// 障害物との当たり判定
	bool CollisionBuilding(D3DXVECTOR3& rPos, float& rDrawPos);	// ビルとの当たり判定

private:
	// メンバ関数
	void SetParentObject(CObject *pObject);	// 親オブジェクト設定

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CObject *m_pParentObject;	// 親オブジェクト
	D3DXVECTOR3 m_sizeOrigin;	// 元の大きさ
	const float m_fMinAlpha;	// 透明度の最小値定数
	const float m_fMaxAlpha;	// 透明度の最大値定数
};

#endif	// _SHADOW_H_
