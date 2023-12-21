//============================================================
//
//	遷移位置ヘッダー [nextPoint.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _NEXTPOINT_H_
#define _NEXTPOINT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object3D.h"

//************************************************************
//	インクルードファイル
//************************************************************
class CObjectGauge3D;	// オブジェクトゲージ3Dクラス
class CControlPoint;	// 操作位置クラス

//************************************************************
//	クラス定義
//************************************************************
// 遷移位置クラス
class CNextPoint : public CObject3D
{
public:
	// テクスチャ列挙
	enum ETexture
	{
		TEXTURE_NORMAL = 0,	// 通常テクスチャ
		TEXTURE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CNextPoint();

	// デストラクタ
	~CNextPoint();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定

	// 静的メンバ関数
	static CNextPoint *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rSize	// 大きさ
	);

private:
	// メンバ関数
	bool Collision(void);	// プレイヤー全員との当たり判定

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CObjectGauge3D *m_pGauge;	// 待機ゲージ情報
	CControlPoint *m_pControl;	// 操作表示の情報
};

#endif	// _NEXTPOINT_H_
