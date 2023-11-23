//============================================================
//
//	深度バッファによる影 [DepthShadow.cpp]
//	Author：丹野竜之介
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "DepthShadow.h"

#include <tchar.h>



// コンストラクタ
CDepthShadow::CDepthShadow()
{
}


// デストラクタ
CDepthShadow::~CDepthShadow()
{
}


// 初期化メソッド
bool CDepthShadow::Init(IDirect3DDevice9 &cpDev )
{
	// デバイスの登録とエフェクトハンドルの取得
	if( &cpDev == NULL ) return false;
	ID3DXBuffer* pError = NULL;

	if(FAILED( D3DXCreateEffectFromFile(
		&cpDev,
		("data\\SHADER\\DepthBufShadowEffect.fx"),
		NULL,
		NULL,
		0,
		NULL,
		&m_cpEffect,
		&pError)))
		if (pError)
		{
			OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
			//デバッグコンソールに表示する
			MessageBoxA(NULL, (LPCSTR)pError->GetBufferPointer(), "Shader Error", MB_OK);
			return false;
		}


	m_hWorldMat		 = m_cpEffect->GetParameterByName( NULL, "matWorld" );
	m_hCameraViewMat = m_cpEffect->GetParameterByName( NULL, "matCameraView" );
	m_hCameraProjMat = m_cpEffect->GetParameterByName( NULL, "matCameraProj" );
	m_hLightViewMat  = m_cpEffect->GetParameterByName( NULL, "matLightView" );
	m_hLightProjMat  = m_cpEffect->GetParameterByName( NULL, "matLightProj" );
	m_hShadowMapTex  = m_cpEffect->GetParameterByName( NULL, "texShadowMap" );
	m_hTechnique = m_cpEffect->GetTechniqueByName( "DepthBufShadowTec" );

	if( !m_hWorldMat || !m_hCameraViewMat || !m_hCameraProjMat
		|| !m_hLightViewMat || !m_hLightProjMat || !m_hShadowMapTex || !m_hTechnique )
		return false;
	
	m_cpDev = &cpDev;
	return true;
}


// シャドウマップを設定
bool CDepthShadow::SetShadowMap(IDirect3DTexture9 &cpShadowMap )
{
	if( &cpShadowMap == NULL )
		return false;

	m_cpShadowMapTex = &cpShadowMap;
	return true;
}


// 描画対象オブジェクトのワールド変換行列を設定
void CDepthShadow::SetWorldMatrix( D3DXMATRIX *pMat )
{
	m_matWorld = *pMat;
}

// カメラのビュー行列を設定
void CDepthShadow::SetCameraViewMatrix( D3DXMATRIX *pMat )
{
	m_matCameraView = *pMat;
}


// カメラの射影変換行列を設定
void CDepthShadow::SetCameraProjMatrix( D3DXMATRIX *pMat )
{
	m_matCameraProj = *pMat;
}


// ライトのビュー行列を設定
void CDepthShadow::SetLightViewMatrix( D3DXMATRIX *pMat )
{
	m_matLightView = *pMat;
}


// ライトの射影変換行列を設定
void CDepthShadow::SetLightProjMatrix( D3DXMATRIX *pMat )
{
	m_matLightProj = *pMat;
}


// 描画の開始を宣言する
HRESULT CDepthShadow::Begin()
{
	// 各サーフェイスを初期化
	m_cpDev->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0 );

	// プログラマブルシェーダに切り替え
	m_cpEffect->SetTechnique( m_hTechnique );

	// 開始宣言
	UINT Pass;
	m_cpEffect->Begin(&Pass, 0);

	return S_OK;
}


// 描画の終了を宣言する
HRESULT CDepthShadow::End()
{
	m_cpEffect->End();

	// 固定機能に戻す
	m_cpDev->SetVertexShader( NULL );
	m_cpDev->SetPixelShader( NULL );

	return S_OK;
}


// パスの開始を宣言する
HRESULT CDepthShadow::BeginPass()
{
	m_cpEffect->BeginPass(0);	// パスは1つのみ
	return S_OK;
}


// パスの終了を宣言する
HRESULT CDepthShadow::EndPass()
{
	m_cpEffect->EndPass();
	return S_OK;
}


// 登録されているパラメータ情報をエフェクトにセット
bool CDepthShadow::SetParamToEffect()
{
	m_cpEffect->SetMatrix( m_hWorldMat, &m_matWorld );
	m_cpEffect->SetMatrix( m_hCameraViewMat, &m_matCameraView );
	m_cpEffect->SetMatrix( m_hCameraProjMat, &m_matCameraProj );
	m_cpEffect->SetMatrix( m_hLightViewMat, &m_matLightView );
	m_cpEffect->SetMatrix( m_hLightProjMat, &m_matLightProj );
	HRESULT hr = m_cpEffect->SetTexture( m_hShadowMapTex, m_cpShadowMapTex);

	return true;
}