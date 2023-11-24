//============================================================
//
//	Zテクスチャ生成ヘッダー [ZTexture.h]
//	Author：丹野 竜之介
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ZTexture_H_
#define _ZTexture_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"


//************************************************************
//	クラス定義
//************************************************************
// アニメーション2Dクラス
class CZTexture
{
public:
	// コンストラクタ
	CZTexture();
	// デストラクタ
	~CZTexture();
	// 初期化メソッド
	bool Init(IDirect3DDevice9 &cpDev, UINT ZTexWidth, UINT ZTexHeight, D3DFORMAT ZTexFormat = D3DFMT_A8R8G8B8);

	// 描画対象オブジェクトのワールド変換行列を設定
	void SetWorldMatrix(D3DXMATRIX *pMat);

	// ビュー行列を設定
	void SetViewMatrix(D3DXMATRIX *pMat);

	// 射影変換行列を設定
	void SetProjMatrix(D3DXMATRIX *pMat);

	// 描画の開始を宣言する
	HRESULT Begin();

	// 描画の終了を宣言する
	HRESULT End();

	// パスの開始を宣言する
	HRESULT BeginPass();

	// パスの終了を宣言する
	HRESULT EndPass();

	// 登録されているパラメータ情報をエフェクトにセット
	bool SetParamToEffect();

	// Z値テクスチャを取得する
	bool GetZTex(IDirect3DTexture9 **cpTex);

	bool GetbPass(void) { return m_bPass; }
private:
	// メンバ変数
	IDirect3DDevice9 * m_cpDev;			// 描画デバイス
	IDirect3DTexture9 * m_cpZTex;		// Z値テクスチャ
	IDirect3DSurface9 * m_cpZTexSurf;	// Z値テクスチャサーフェイス
	IDirect3DSurface9 * m_cpDepthBuff;	// 深度バッファ
	IDirect3DSurface9 * m_cpDevBuffer;	// デバイスバックバッファ
	IDirect3DSurface9 * m_cpDevDepth;	// デバイス深度バッファ
	ID3DXEffect * m_cpEffect;			// Z値プロットエフェクト
	D3DXMATRIX m_matWorld;						// ワールド変換行列
	D3DXMATRIX m_matView;						// ビュー変換行列
	D3DXMATRIX m_matProj;						// 射影変換行列
	D3DXHANDLE m_hWorldMat;						// ワールド変換行列ハンドル
	D3DXHANDLE m_hViewMat;						// ビュー変換行列ハンドル
	D3DXHANDLE m_hProjMat;						// 射影変換行列ハンドル
	D3DXHANDLE m_hTechnique;					// テクニックへのハンドル
	bool m_bPass;
};

#endif	// _ZTexture_H_
