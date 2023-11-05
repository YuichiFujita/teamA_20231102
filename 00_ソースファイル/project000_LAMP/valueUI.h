//============================================================
//
//	数字UIヘッダー [valueUI.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _VALUE_UI_H_
#define _VALUE_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"
#include "value.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス
class CMultiValue;	// マルチ数字クラス

//************************************************************
//	クラス定義
//************************************************************
// 数字UIクラス
class CValueUI : public CObject
{
public:
	// コンストラクタ
	CValueUI();

	// デストラクタ
	~CValueUI();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetEnableDraw(const bool bDraw) override;	// 描画状況設定
	void SetPriority(const int nPriority) override;	// 優先順位設定
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得

	// 静的メンバ関数
	static CValueUI *Create	// 生成
	( // 引数
		const char *pPassTex,			// タイトルテクスチャパス
		const CValue::ETexture texture,	// 数字テクスチャ
		const int nDigit,				// 桁数
		const D3DXVECTOR3& rPos,		// 位置
		const D3DXVECTOR3& rSpace,		// 行間
		const D3DXVECTOR3& rSpaceValue,	// 数字行間
		const D3DXVECTOR3& rSizeTitle,	// タイトル大きさ
		const D3DXVECTOR3& rSizeValue,	// 数字大きさ
		const D3DXVECTOR3& rRotTitle = VEC3_ZERO,	// タイトル向き
		const D3DXVECTOR3& rRotValue = VEC3_ZERO,	// 数字向き
		const D3DXCOLOR& rColTitle = XCOL_WHITE,	// タイトル色
		const D3DXCOLOR& rColValue = XCOL_WHITE		// 数字色
	);

	// メンバ関数
	void SetSpace(const D3DXVECTOR3& rSpace);		// 行間設定
	D3DXVECTOR3 GetSpace(void) const;				// 行間取得
	void SetRotationTitle(const D3DXVECTOR3& rRot);	// タイトル向き設定
	D3DXVECTOR3 GetRotationTitle(void) const;		// タイトル向き取得
	void SetScalingTitle(const D3DXVECTOR3& rSize);	// タイトル大きさ設定
	D3DXVECTOR3 GetScalingTitle(void) const;		// タイトル大きさ取得
	void SetColorTitle(const D3DXCOLOR& rCol);		// タイトル色設定
	D3DXCOLOR GetColorTitle(void) const;			// タイトル色取得
	void SetTextureTitle(const char *pPassTex);		// タイトルテクスチャ設定
	CMultiValue *GetMultiValue(void) const;			// 数字情報取得

private:
	// メンバ関数
	void SetPositionRelative(void);	// 相対位置設定

	// メンバ変数
	CObject2D *m_pTitle;	// タイトル情報
	CMultiValue *m_pValue;	// 数字情報
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_space;	// 行間
};

#endif	// _VALUE_UI_H_
