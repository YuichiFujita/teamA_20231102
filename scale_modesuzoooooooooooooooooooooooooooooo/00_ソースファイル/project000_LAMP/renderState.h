//============================================================
//
//	レンダーステートヘッダー [renderState.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RENDER_STATE_H_
#define _RENDER_STATE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	クラス定義
//************************************************************
// レンダーステートクラス
class CRenderState
{
public:
	// コンストラクタ
	CRenderState();

	// デストラクタ
	~CRenderState();

	// αブレンド情報
	struct SBlendAlpha
	{
	public:
		// コンストラクタ
		SBlendAlpha() {}
		SBlendAlpha(const D3DBLENDOP Op, const D3DBLEND Scr, const D3DBLEND Dest)
		{ op = Op; scr = Scr; dest = Dest; bBlend = true; }

		// デストラクタ
		~SBlendAlpha() {}

		// メンバ変数
		D3DBLENDOP	op;		// αブレンド情報
		D3DBLEND	scr;	// SCRブレンド
		D3DBLEND	dest;	// DESTブレンド
		bool		bBlend;	// αブレンド状況
	};

	// αテスト情報
	struct STestAlpha
	{
		D3DCMPFUNC	func;	// αテスト情報
		int			nRef;	// αテスト参照値
		bool		bTest;	// αテスト状況
	};

	// Zテスト情報
	struct STestZ
	{
		D3DCMPFUNC	func;		// Zテスト情報
		bool		bUpdate;	// Zバッファ更新状況
	};

	// レンダーステート情報
	struct SInfo
	{
		SBlendAlpha	blendAlpha;	// αブレンド情報
		STestAlpha	testAlpha;	// αテスト情報
		STestZ		testZ;		// Zテスト情報
		D3DCULL		cull;		// カリング情報
		bool		bLight;		// ライティング状況
	};

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Set(void);		// 設定
	void Reset(void);	// 再設定

	// 静的メンバ関数
	static CRenderState *Create(void);	// 生成
	static void InitRenderState(const LPDIRECT3DDEVICE9 &rDevice);	// レンダーステート情報初期化

private:
	// メンバ関数
	static void SetRenderState(const LPDIRECT3DDEVICE9 &rDevice, const SInfo info);	// レンダーステート設定

	// 静的メンバ変数
	static SInfo m_save;	// 保存レンダーステート情報

	// メンバ変数
	SInfo m_info;	// レンダーステート情報
};

#endif	// _RENDER_STATE_H_
