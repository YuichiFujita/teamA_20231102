//============================================================
//
//	ライト処理 [light.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "light.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	親クラス [CLight] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CLight::CLight()
{
	// メンバ変数をクリア
	memset(&m_aLight[0], 0, sizeof(m_aLight));	// ライトの情報
}

//============================================================
//	デストラクタ
//============================================================
CLight::~CLight()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CLight::Init(void)
{
	// 変数配列を宣言
	D3DXCOLOR	aDiffuse[MAX_LIGHT];	// 設定用拡散光カラー
	D3DXVECTOR3	aVecDir[MAX_LIGHT];		// 設定用方向ベクトル

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 拡散光カラーを設定
	aDiffuse[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	aDiffuse[1] = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	aDiffuse[2] = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	aDiffuse[3] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);

	// 方向ベクトルを設定
	aVecDir[0] = D3DXVECTOR3( 0.22f, -0.97f,  0.54f);
	aVecDir[1] = D3DXVECTOR3(-0.38f,  0.78f, -0.54f);
	aVecDir[2] = D3DXVECTOR3( 0.89f, -0.21f,  0.44f);
	aVecDir[3] = D3DXVECTOR3(-0.96f,  0.15f, -0.44f);

	// メンバ変数を初期化
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{ // 使用するライト数分繰り返す

		// 構造体の要素をクリア
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ライトの拡散光を設定
		m_aLight[nCntLight].Diffuse = aDiffuse[nCntLight];

		// ライトの方向を設定
		D3DXVec3Normalize(&aVecDir[nCntLight], &aVecDir[nCntLight]);	// ベクトルを正規化
		m_aLight[nCntLight].Direction = aVecDir[nCntLight];

		// ライトを設定する
		pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CLight::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CLight::Update(void)
{

}

//============================================================
//	生成処理
//============================================================
CLight *CLight::Create(void)
{
	// ポインタを宣言
	CLight *pLight = NULL;	// ライト生成用

	if (pLight == NULL)
	{ // 使用されていない場合

		// メモリを確保
		pLight = new CLight;	// ライト
	}
	else { assert(false); return NULL; }	// 使用中

	if (pLight != NULL)
	{ // 確保に成功している場合

		// ライトの初期化
		if (FAILED(pLight->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pLight;
			pLight = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pLight;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CLight::Release(CLight *&prLight)
{
	if (prLight != NULL)
	{ // 使用中の場合

		// ライトの終了
		prLight->Uninit();

		// メモリ開放
		delete prLight;
		prLight = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
