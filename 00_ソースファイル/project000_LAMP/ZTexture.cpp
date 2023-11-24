//============================================================
//
//	Zテクスチャ生成 [ZTexture.cpp]
//	Author：丹野竜之介
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "ZTexture.h"

// コンストラクタ
CZTexture::CZTexture()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	m_hWorldMat = NULL;
	m_hViewMat = NULL;
	m_hProjMat = NULL;
	m_hTechnique = NULL;
}

// デストラクタ
CZTexture::~CZTexture()
{
	// COMポインタを明示的に消去
	m_cpDepthBuff = NULL;	// 深度バッファ
	m_cpZTexSurf = NULL;	// Z値テクスチャサーフェイス
	m_cpZTex = NULL;		// Z値テクスチャ
	m_cpDevBuffer = NULL;	// デバイスバックバッファ
	m_cpDevDepth = NULL;	// デバイス深度バッファ
	m_cpEffect = NULL;		// Z値プロットエフェクト
	m_cpDev = NULL;			// 描画デバイス
}


// 初期化メソッド
bool CZTexture::Init(IDirect3DDevice9 &cpDev, UINT ZTexWidth, UINT ZTexHeight, D3DFORMAT ZTexFormat)
{
	if (&cpDev == NULL) return false;
	if (ZTexWidth == 0 || ZTexHeight == 0) return false;

	HRESULT hr;
	ID3DXBuffer* pError = NULL;
	// Z値プロットシェーダプログラムを読み込む
	if (FAILED(D3DXCreateEffectFromFile(
		&cpDev,
		("data\\SHADER\\ZTexCreator.fx"),
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
	

	// エフェクト内の各種パラメータハンドルを取得
	m_hWorldMat = m_cpEffect->GetParameterByName(NULL, "matWorld");
	m_hViewMat = m_cpEffect->GetParameterByName(NULL, "matView");
	m_hProjMat = m_cpEffect->GetParameterByName(NULL, "matProj");
	m_hTechnique = m_cpEffect->GetTechniqueByName("ZValuePlotTec");
	if (!m_hWorldMat || !m_hViewMat || !m_hProjMat || !m_hTechnique)
		return false;

	// 指定のZ値テクスチャを生成
	hr = D3DXCreateTexture(
		&cpDev,
		ZTexWidth, ZTexHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		ZTexFormat,
		D3DPOOL_DEFAULT,
		&m_cpZTex);
	if (FAILED(hr))
		return false;

	m_cpZTex->GetSurfaceLevel(0, &m_cpZTexSurf);	// サーフェイス取得

															// 描画デバイスに定義されているバッファの能力を取得
	IDirect3DSurface9 *pSurf;
	cpDev.GetDepthStencilSurface(&pSurf);
	D3DSURFACE_DESC Desc;
	pSurf->GetDesc(&Desc);
	pSurf->Release();

	// 独自深度バッファを作成
	hr = cpDev.CreateDepthStencilSurface(
		ZTexWidth, ZTexHeight,
		Desc.Format,
		Desc.MultiSampleType,
		Desc.MultiSampleQuality,
		FALSE,
		&m_cpDepthBuff,
		NULL
	);
	if (FAILED(hr))
		return false;

	m_cpDev = &cpDev;
	m_bPass = false;
	return true;
}


// 描画対象オブジェクトのワールド変換行列を設定
void CZTexture::SetWorldMatrix(D3DXMATRIX *pMat)
{
	m_matWorld = *pMat;
}


// ビュー行列を設定
void CZTexture::SetViewMatrix(D3DXMATRIX *pMat)
{
	m_matView = *pMat;
}


// 射影変換行列を設定
void CZTexture::SetProjMatrix(D3DXMATRIX *pMat)
{
	m_matProj = *pMat;
}


// 描画の開始を宣言する
HRESULT CZTexture::Begin()
{
	// 初期化が正しく行われているかチェック
	if (m_cpDepthBuff == NULL || m_cpZTex == NULL)
		return E_FAIL;

	// デバイスが持っているバッファを一時保存
	m_cpDev->GetRenderTarget(0, &m_cpDevBuffer);
	m_cpDev->GetDepthStencilSurface(&m_cpDevDepth);

	// デバイスにZ値テクスチャサーフェイスと深度バッファを設定
	m_cpDev->SetRenderTarget(0, m_cpZTexSurf);
	m_cpDev->SetDepthStencilSurface(m_cpDepthBuff);

	// 各サーフェイスを初期化
	m_cpDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);

	// プログラマブルシェーダのテクニックを設定
	m_cpEffect->SetTechnique(m_hTechnique);

	// シェーダの開始を宣言
	UINT Tmp;
	m_cpEffect->Begin(&Tmp, 0);
	m_bPass = true;
	return S_OK;
}


// パスの開始を宣言する
HRESULT CZTexture::BeginPass()
{
	m_cpEffect->BeginPass(0);	// Z値計算は1パス
	return S_OK;
}


// 登録されているパラメータ情報をエフェクトにセット
bool CZTexture::SetParamToEffect()
{
	if (m_cpEffect == NULL) return false;

	m_cpEffect->SetMatrix(m_hWorldMat, &m_matWorld);
	m_cpEffect->SetMatrix(m_hViewMat, &m_matView);
	m_cpEffect->SetMatrix(m_hProjMat, &m_matProj);

	return true;
}


// パスの終了を宣言する
HRESULT CZTexture::EndPass()
{
	m_cpEffect->EndPass();
	return S_OK;
}


// 描画の終了を宣言する
HRESULT CZTexture::End()
{
	m_cpEffect->End();

	// デバイスに元のサーフェイスを戻す
	m_cpDev->SetRenderTarget(0, m_cpDevBuffer);
	m_cpDev->SetDepthStencilSurface(m_cpDevDepth);

	m_cpDevBuffer = NULL;
	m_cpDevDepth = NULL;

	// 固定機能に戻す
	m_cpDev->SetVertexShader(NULL);
	m_cpDev->SetPixelShader(NULL);
	m_bPass = false;
	return S_OK;
}


// Z値テクスチャを取得する
bool CZTexture::GetZTex(IDirect3DTexture9 **cpTex)
{
	*cpTex = m_cpZTex;
	return true;
}