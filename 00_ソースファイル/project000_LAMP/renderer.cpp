//============================================================
//
//	レンダラー処理 [renderer.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "camera.h"
#include "object2D.h"

//************************************************************
//	親クラス [CRenderer] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CRenderer::CRenderer()
{
	// メンバ変数をクリア
	m_pD3D			= NULL;	// Direct3Dオブジェクト
	m_pD3DDevice	= NULL;	// Direct3Dデバイス
	m_pRenderTexture			= NULL;	// テクスチャへのポインタ
	m_pRenderTextureSurface		= NULL;	// 描画サーフェイスへのポインタ
	m_pDepthStencilSurface		= NULL;	// Zバッファ・ステンシルバッファのサーフェイスへのポインタ
	m_pDefRenderTextureSurface	= NULL;	// 元の描画サーフェイス保存用
	m_pDefDepthStencilSurface	= NULL;	// 元のZバッファ・ステンシルバッファのサーフェイス保存用
}

//============================================================
//	デストラクタ
//============================================================
CRenderer::~CRenderer()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	// 変数を宣言
	HRESULT					hr;		// 異常終了の確認用
	D3DDISPLAYMODE			d3ddm;	// ディスプレイモード
	D3DPRESENT_PARAMETERS	d3dpp;	// プレゼンテーションパラメータ

	// メンバ変数を初期化
	m_pD3D			= NULL;	// Direct3Dオブジェクト
	m_pD3DDevice	= NULL;	// Direct3Dデバイス
	m_pRenderTexture			= NULL;	// テクスチャへのポインタ
	m_pRenderTextureSurface		= NULL;	// 描画サーフェイスへのポインタ
	m_pDepthStencilSurface		= NULL;	// Zバッファ・ステンシルバッファのサーフェイスへのポインタ
	m_pDefRenderTextureSurface	= NULL;	// 元の描画サーフェイス保存用
	m_pDefDepthStencilSurface	= NULL;	// 元のZバッファ・ステンシルバッファのサーフェイス保存用

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{ // オブジェクトの生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{ // ディスプレイモードの取得に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// パラメータのゼロクリア
	d3dpp.BackBufferWidth	= SCREEN_WIDTH;		// ゲーム画面サイズ (幅)
	d3dpp.BackBufferHeight	= SCREEN_HEIGHT;	// ゲーム画面サイズ (高さ)
	d3dpp.BackBufferFormat	= d3ddm.Format;		// バックバッファの形式
	d3dpp.BackBufferCount	= 1;				// バックバッファの数
	d3dpp.SwapEffect		= D3DSWAPEFFECT_DISCARD;	// ダブルバッファの切り替え (映像信号に同期)
	d3dpp.EnableAutoDepthStencil	= TRUE;				// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;		// デプスバッファとして 16bit を使う
	d3dpp.Windowed					= bWindow;			// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成
	if (FAILED(CreateDevice(hWnd, d3dpp)))
	{ // デバイスの生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 空のテクスチャを生成
	hr = D3DXCreateTexture
	( // 引数
		m_pD3DDevice,			// Direct3Dデバイス
		SCREEN_WIDTH,			// テクスチャ横幅
		SCREEN_HEIGHT,			// テクスチャ縦幅
		1,						// ミップマップレベル
		D3DUSAGE_RENDERTARGET,	// 性質・確保オプション
		D3DFMT_A8R8G8B8,		// ピクセルフォーマット
		D3DPOOL_DEFAULT,		// 格納メモリ
		&m_pRenderTexture		// テクスチャへのポインタ
	);
	if (FAILED(hr))
	{ // テクスチャの生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャ用サーフェイスの生成
	hr = m_pD3DDevice->CreateDepthStencilSurface
	( // 引数
		SCREEN_WIDTH,					// 深度ステンシルのサーフェス横幅
		SCREEN_HEIGHT,					// 深度ステンシルのサーフェス縦幅
		d3dpp.AutoDepthStencilFormat,	// 深度ステンシルのサーフェス形式
		D3DMULTISAMPLE_NONE,			// マルチサンプリングのバッファー型
		0,								// 品質レベル
		FALSE,							// Zバッファ破棄の有効/無効
		&m_pDepthStencilSurface,		// Zバッファ・ステンシルバッファのサーフェイスへのポインタ
		NULL							// NULL
	);
	if (FAILED(hr))
	{ // 描画先の生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 描画サーフェイスの取得
	hr = m_pRenderTexture->GetSurfaceLevel
	( // 引数
		0,							// ミップマップレベル
		&m_pRenderTextureSurface	// 描画サーフェイスへのポインタ
	);
	if (FAILED(hr))
	{ // サーフェイスの取得に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 元の描画サーフェイスを保存
	m_pD3DDevice->GetRenderTarget(0, &m_pDefRenderTextureSurface);
	if (FAILED(hr))
	{ // 描画サーフェイスの取得に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 元のZバッファ・ステンシルバッファのサーフェイスを保存
	m_pD3DDevice->GetDepthStencilSurface(&m_pDefDepthStencilSurface);
	if (FAILED(hr))
	{ // Zバッファ・ステンシルバッファの取得に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定 (テクスチャの拡縮補間の設定)
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定 (テクスチャのアルファブレンドの設定)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CRenderer::Uninit(void)
{
	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{ // Direct3Dデバイスが使用中の場合

		// メモリ開放
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{ // Direct3Dオブジェクトが使用中の場合

		// メモリ開放
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	// テクスチャの破棄
	if (m_pRenderTexture != NULL)
	{ // テクスチャが使用中の場合

		// メモリ開放
		m_pRenderTexture->Release();
		m_pRenderTexture = NULL;
	}

	// 描画サーフェイスの破棄
	if (m_pRenderTextureSurface != NULL)
	{ // 描画サーフェイスが使用中の場合

		// メモリ開放
		m_pRenderTextureSurface->Release();
		m_pRenderTextureSurface = NULL;
	}

	//  Zバッファ・ステンシルバッファのサーフェイスの破棄
	if (m_pDepthStencilSurface != NULL)
	{ // Zバッファ・ステンシルバッファのサーフェイスが使用中の場合

		// メモリ開放
		m_pDepthStencilSurface->Release();
		m_pDepthStencilSurface = NULL;
	}

	// 元の描画サーフェイスの破棄
	if (m_pDefRenderTextureSurface != NULL)
	{ // 元の描画サーフェイスが使用中の場合

		// メモリ開放
		m_pDefRenderTextureSurface->Release();
		m_pDefRenderTextureSurface = NULL;
	}

	// 元のZバッファ・ステンシルバッファのサーフェイス破棄
	if (m_pDefDepthStencilSurface != NULL)
	{ // 元のZバッファ・ステンシルバッファのサーフェイスが使用中の場合

		// メモリ開放
		m_pDefDepthStencilSurface->Release();
		m_pDefDepthStencilSurface = NULL;
	}
}

//============================================================
//	更新処理
//============================================================
void CRenderer::Update(void)
{
	// オブジェクトの全更新
	CObject::UpdateAll();
}

//============================================================
//	描画処理
//============================================================
void CRenderer::Draw(void)
{
	// 変数を宣言
	HRESULT			hr;				// 異常終了の確認用
	D3DVIEWPORT9	viewportDef;	// カメラのビューポート保存用

	//--------------------------------------------------------
	//	テクスチャ作成用の描画
	//--------------------------------------------------------
	// 描画サーフェイスを作成したものに変更
	hr = m_pD3DDevice->SetRenderTarget(0, m_pRenderTextureSurface);
	if (FAILED(hr)) { assert(false); }

	// Zバッファ・ステンシルバッファのサーフェイスを作成したものに変更
	hr = m_pD3DDevice->SetDepthStencilSurface(m_pDepthStencilSurface);
	if (FAILED(hr)) { assert(false); }

	// バックバッファとZバッファのクリア
	hr = m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	if (FAILED(hr)) { assert(false); }

	// テクスチャ作成用の描画
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{ // 描画開始が成功した場合

		// 現在のビューポートを取得
		m_pD3DDevice->GetViewport(&viewportDef);

		// カメラの設定
		CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

		// オブジェクトの全描画
		CObject::DrawAll();

		// ビューポートを元に戻す
		m_pD3DDevice->SetViewport(&viewportDef);

		// 描画終了
		hr = m_pD3DDevice->EndScene();
		if (FAILED(hr)) { assert(false); }
	}

	//--------------------------------------------------------
	//	画面用の描画
	//--------------------------------------------------------
	// 描画サーフェイスを元に戻す
	hr = m_pD3DDevice->SetRenderTarget(0, m_pDefRenderTextureSurface);
	if (FAILED(hr)) { assert(false); }

	// Zバッファ・ステンシルバッファのサーフェイスを元に戻す
	hr = m_pD3DDevice->SetDepthStencilSurface(m_pDefDepthStencilSurface);
	if (FAILED(hr)) { assert(false); }

	// バックバッファとZバッファのクリア
	hr = m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	if (FAILED(hr)) { assert(false); }

	// 画面の描画
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{ // 描画開始が成功した場合

		// 現在のビューポートを取得
		m_pD3DDevice->GetViewport(&viewportDef);

		// カメラの設定
		CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

		// オブジェクトの全描画
		CObject::DrawAll();

		// デバッグ表示の描画
		CManager::GetInstance()->GetDebugProc()->Draw();

		// ビューポートを元に戻す
		m_pD3DDevice->SetViewport(&viewportDef);

		// 描画終了
		hr = m_pD3DDevice->EndScene();
		if (FAILED(hr)) { assert(false); }
	}

	// バックバッファとフロントバッファの入れ替え
	hr = m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	if (FAILED(hr)) { assert(false); }
}

//============================================================
//	デバイス取得処理
//============================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void) const
{
	// デバイスのポインタを返す
	return m_pD3DDevice;
}

//============================================================
//	生成処理
//============================================================
CRenderer *CRenderer::Create(HWND hWnd, BOOL bWindow)
{
	// ポインタを宣言
	CRenderer *pRenderer = NULL;	// レンダラー生成用

	if (pRenderer == NULL)
	{ // 使用されていない場合

		// メモリを確保
		pRenderer = new CRenderer;	// レンダラー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pRenderer != NULL)
	{ // 確保に成功している場合

		// レンダラーの初期化
		if (FAILED(pRenderer->Init(hWnd, bWindow)))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pRenderer;
			pRenderer = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pRenderer;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CRenderer::Release(CRenderer *&prRenderer)
{
	if (prRenderer != NULL)
	{ // 使用中の場合

		// レンダラーの終了
		prRenderer->Uninit();

		// メモリ開放
		delete prRenderer;
		prRenderer = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	デバイス生成処理
//============================================================
HRESULT CRenderer::CreateDevice(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp)
{
	// 描画処理と頂点処理をハードウェアで行う設定にする
	if (FAILED(m_pD3D->CreateDevice
	( // 引数
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice
	)))
	{ // CPUの性能が低かった場合

		// 描画処理はハードウェア、頂点処理はCPUが行う設定にする
		if (FAILED(m_pD3D->CreateDevice
		( // 引数
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice
		)))
		{ // ハードウェアの性能が低かった場合

			// 描画処理と頂点処理をCPUで行う設定にする
			if (FAILED(m_pD3D->CreateDevice
			( // 引数
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice
			)))
			{ // CPUとハードウェアの性能が使い物にならなかった場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}
		}
	}

	// 成功を返す
	return S_OK;
}
