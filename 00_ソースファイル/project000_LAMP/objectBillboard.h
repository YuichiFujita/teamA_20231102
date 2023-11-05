//============================================================
//
//	オブジェクトビルボードヘッダー [objectBillboard.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトビルボードクラス
class CObjectBillboard : public CObject
{
public:
	// 原点列挙
	enum EOrigin
	{
		ORIGIN_CENTER = 0,	// 中心原点
		ORIGIN_DOWN,		// 下原点
		ORIGIN_MAX,			// この列挙型の総数
	};

	// 回転列挙
	enum ERotate
	{
		ROTATE_NORMAL = 0,	// 通常回転
		ROTATE_LATERAL,		// 横回転
		ROTATE_MAX,			// この列挙型の総数
	};

	// コンストラクタ
	CObjectBillboard();
	CObjectBillboard(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectBillboard();

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
	D3DXMATRIX *GetPtrMtxWorld(void) override;				// マトリックスポインタ取得
	D3DXMATRIX GetMtxWorld(void) const override;			// マトリックス取得

	// 静的メンバ関数
	static CObjectBillboard *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rSize,	// 大きさ
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXCOLOR& rCol = XCOL_WHITE,		// 色
		const EOrigin origin = ORIGIN_CENTER,	// 原点
		const ERotate rotate = ROTATE_NORMAL,	// 回転
		const D3DCMPFUNC func = D3DCMP_ALWAYS,	// Zテスト設定
		const bool bZEnable = false				// Zバッファの使用状況
	);

	// メンバ関数
	void SetOrigin(const EOrigin origin);	// 原点設定
	EOrigin GetOrigin(void) const;			// 原点取得
	void SetRotate(const ERotate rotate);	// 回転設定
	ERotate GetRotate(void) const;			// 回転取得
	void SetFunc(const D3DCMPFUNC func);	// Zテスト設定
	D3DCMPFUNC GetFunc(void) const;			// Zテスト取得
	void SetZEnable(const bool bEnable);	// Zバッファの使用状況設定
	bool IsZEnable(void) const;				// Zバッファの使用状況取得

protected:
	// メンバ関数
	void SetVtx(void);	// 頂点情報の設定

private:
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX	m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3	m_pos;		// 位置
	D3DXVECTOR3	m_rot;		// 向き
	D3DXVECTOR3	m_size;		// 大きさ
	D3DXCOLOR	m_col;		// 色
	EOrigin		m_origin;	// 原点
	ERotate		m_rotate;	// 回転
	D3DCMPFUNC	m_func;		// Zテスト設定
	bool  m_bZEnable;		// Zバッファの使用状況
	float m_fAngle;			// 対角線の角度
	float m_fLength;		// 対角線の長さ
	int   m_nTextureID;		// テクスチャインデックス
};

#endif	// _OBJECTBILLBOARD_H_
