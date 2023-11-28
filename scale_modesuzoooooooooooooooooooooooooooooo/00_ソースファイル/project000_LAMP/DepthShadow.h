//============================================================
//
//	Zテクスチャ生成ヘッダー [DepthShadow.h]
//	Author：丹野 竜之介
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _DepthShadow_H_
#define _DepthShadow_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"


//************************************************************
//	クラス定義
//************************************************************
// アニメーション2Dクラス
class CDepthShadow
{
public:
	// コンストラクタ
	CDepthShadow();
	// デストラクタ
	~CDepthShadow();
	// 初期化メソッド
	bool Init(IDirect3DDevice9 &cpDev);

	// シャドウマップを設定
	bool SetShadowMap(IDirect3DTexture9 **cpShadowMap);

	// 描画対象オブジェクトのワールド変換行列を設定
	void SetWorldMatrix(D3DXMATRIX *pMat);

	// カメラのビュー行列を設定
	void SetCameraViewMatrix(D3DXMATRIX *pMat);

	// カメラの射影変換行列を設定
	void SetCameraProjMatrix(D3DXMATRIX *pMat);

	// ライトのビュー行列を設定
	void SetLightViewMatrix(D3DXMATRIX *pMat);

	// ライトの射影変換行列を設定
	void SetLightProjMatrix(D3DXMATRIX *pMat);

	void SetAmbient(D3DXCOLOR * pCol) { m_AmbientCol = D3DXVECTOR4(pCol->r, pCol->g, pCol->b, pCol->a); }

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

	bool GetbPass(void) { return m_bPass; }
private:
	// メンバ変数
	IDirect3DDevice9 * m_cpDev;			// 描画デバイス
	IDirect3DTexture9 * m_cpShadowMapTex;	// シャドウマップテクスチャ
	ID3DXEffect * m_cpEffect;			// 深度バッファシャドウエフェクト
	D3DXVECTOR4 m_AmbientCol;
	D3DXMATRIX m_matWorld;						// ワールド変換行列
	D3DXMATRIX m_matCameraView;					// カメラのビュー変換行列
	D3DXMATRIX m_matCameraProj;					// カメラの射影変換行列
	D3DXMATRIX m_matLightView;					// ライトのビュー変換行列
	D3DXMATRIX m_matLightProj;					// ライトの射影変換行列
	D3DXHANDLE m_hWorldMat;						// ワールド変換行列ハンドル
	D3DXHANDLE m_hCameraViewMat;				// ビュー変換行列ハンドル
	D3DXHANDLE m_hCameraProjMat;				// 射影変換行列ハンドル
	D3DXHANDLE m_hLightViewMat;					// ビュー変換行列ハンドル
	D3DXHANDLE m_hLightProjMat;					// 射影変換行列ハンドル
	D3DXHANDLE m_hShadowMapTex;					// シャドウマップテクスチャハンドル
	D3DXHANDLE m_hAmbient;						// 色ハンドル
	D3DXHANDLE m_hTechnique;					// テクニックへのハンドル
	bool m_bPass;
};

#endif	// _DepthShadow_H_
