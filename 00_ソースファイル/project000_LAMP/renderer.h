//============================================================
//
//	レンダラーヘッダー [renderer.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// レンダラークラス
class CRenderer
{
public:
	// コンストラクタ
	CRenderer();

	// デストラクタ
	~CRenderer();

	// メンバ関数
	HRESULT Init(HWND hWnd, BOOL bWindow);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	LPDIRECT3DDEVICE9 GetDevice(void) const;	// デバイス取得

	// 静的メンバ関数
	static CRenderer *Create(HWND hWnd, BOOL bWindow);	// 生成
	static HRESULT Release(CRenderer *&prRenderer);		// 破棄

private:
	// メンバ関数
	HRESULT CRenderer::CreateDevice(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp);	// デバイス生成

	// メンバ変数
	LPDIRECT3D9			m_pD3D;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9	m_pD3DDevice;	// Direct3Dデバイス
	LPDIRECT3DTEXTURE9	m_pRenderTexture;			// テクスチャへのポインタ
	LPDIRECT3DSURFACE9	m_pRenderTextureSurface;	// 描画サーフェイスへのポインタ
	LPDIRECT3DSURFACE9	m_pDepthStencilSurface;		// Zバッファ・ステンシルバッファのサーフェイスへのポインタ
	LPDIRECT3DSURFACE9	m_pDefRenderTextureSurface;	// 元の描画サーフェイス保存用
	LPDIRECT3DSURFACE9	m_pDefDepthStencilSurface;	// 元のZバッファ・ステンシルバッファのサーフェイス保存用
};

#endif	// _RENDERER_H_
