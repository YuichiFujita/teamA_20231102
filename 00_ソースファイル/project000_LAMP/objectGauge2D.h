//============================================================
//
//	オブジェクトゲージ2Dヘッダー [objectGauge2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECTGAUGE2D_H_
#define _OBJECTGAUGE2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトゲージ2Dクラス
class CObjectGauge2D : public CObject
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 通常状態
		STATE_CHANGE,	// ゲージ変動状態
		STATE_MAX		// この列挙型の総数
	};

	// ポリゴン列挙
	enum EPolygon
	{
		POLYGON_BACK = 0,	// 背景
		POLYGON_FRONT,		// ゲージ
		POLYGON_FRAME,		// 枠
		POLYGON_MAX			// この列挙型の総数
	};

	// フレーム種類列挙
	enum EType
	{
		TYPE_NONE = 0,	// フレーム無し
		TYPE_EXP,		// 経験値フレーム
		TYPE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CObjectGauge2D();
	CObjectGauge2D(const int nFrame, const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectGauge2D();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得

	// 静的メンバ関数
	static CObjectGauge2D *Create	// 生成
	( // 引数
		const CObject::ELabel label,	// オブジェクトラベル
		const int nMax,				// 最大表示値
		const int nFrame,			// 表示値変動フレーム
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rSizeGauge = VEC3_ONE,	// ゲージ大きさ
		const D3DXCOLOR& rColFront = XCOL_WHITE,	// 表ゲージ色
		const D3DXCOLOR& rColBack = XCOL_BLACK,		// 裏ゲージ色
		const bool bDrawFrame = false,				// 枠描画状況
		const EType frameType = TYPE_NONE,			// 枠種類
		const D3DXVECTOR3& rSizeFrame = VEC3_ONE	// 枠大きさ
	);

	// メンバ関数
	void AddNum(const int nAdd);	// ゲージ加算
	void SetNum(const int nNum);	// ゲージ設定
	int  GetNum(void) const;		// ゲージ取得
	void SetMaxNum(const int nMax);	// ゲージ最大値設定
	int  GetMaxNum(void) const;		// ゲージ最大値取得

	void BindTexture(const int nPolygonID, const int nTextureID);		// テクスチャ割当 (インデックス)
	void BindTexture(const int nPolygonID, const char *pTexturePass);	// テクスチャ割当 (パス)
	void SetScalingGauge(const D3DXVECTOR3& rSize);	// ゲージ大きさ設定
	D3DXVECTOR3 GetScalingGauge(void) const;		// ゲージ大きさ取得
	void SetScalingFrame(const D3DXVECTOR3& rSize);	// 背景大きさ設定
	D3DXVECTOR3 GetScalingFrame(void) const;		// 背景大きさ取得
	void SetColorFront(const D3DXCOLOR& rCol);		// 表ゲージ色設定
	D3DXCOLOR GetColorFront(void) const;			// 表ゲージ色取得
	void SetColorBack(const D3DXCOLOR& rCol);		// 裏ゲージ色設定
	D3DXCOLOR GetColorBack(void) const;				// 裏ゲージ色取得
	void SetEnableDrawFrame(const bool bDraw);		// 枠表示状況設定
	bool IsEnableDrawFrame(void) const;				// 枠表示状況取得

private:
	// メンバ関数
	void SetVtx(void);	// 頂点情報の設定

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXVECTOR3	m_pos;					// 位置
	D3DXVECTOR3	m_sizeGauge;			// ゲージ大きさ
	D3DXVECTOR3	m_sizeFrame;			// 枠大きさ
	D3DXCOLOR	m_colFront;				// 表ゲージ色
	D3DXCOLOR	m_colBack;				// 裏ゲージ色
	EState m_state;						// 状態
	bool  m_bDrawFrame;					// 枠表示状況
	float m_fChange;					// ゲージ変動量
	float m_fCurrentNumGauge;			// 現在表示値
	float m_fAddRight;					// 横幅加算量
	int m_nCounterState;				// 状態管理カウンター
	int m_nNumGauge;					// 表示値
	int m_nMaxNumGauge;					// 表示値の最大値
	int m_aTextureID[POLYGON_MAX];		// テクスチャインデックス
	const int m_nFrame;					// 表示値の変動フレーム定数
};

#endif	// _OBJECTGAUGE2D_H_
