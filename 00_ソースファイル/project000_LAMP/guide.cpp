//============================================================
//
//	ガイド処理 [guide.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "guide.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY = 6;	// ガイドの優先順位
}

//************************************************************
//	子クラス [CGuide] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGuide::CGuide() : CObject3D(CObject::LABEL_UI, PRIORITY)
{
}

//============================================================
//	デストラクタ
//============================================================
CGuide::~CGuide()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGuide::Init(void)
{
	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	BindTexture("data\\TEXTURE\\Guide.png");
	SetEnableDraw(false);
	SetEnableDepthShadow(false);
	SetEnableZTex(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGuide::Uninit(void)
{
	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGuide::Update(void)
{
	// オブジェクト3Dの更新
	CObject3D::Update();
}

//============================================================
//	描画処理
//============================================================
void CGuide::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// Zテストを設定する
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);		// Zバッファ更新の有効 / 無効の設定

	// オブジェクト3Dの描画
	CObject3D::Draw();

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);		// Zバッファ更新の有効 / 無効の設定
}

//============================================================
//	生成処理
//============================================================
CGuide *CGuide::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rSize	// 大きさ
)
{
	// ポインタを宣言
	CGuide *pGuide = NULL;	// ガイド生成用

	if (pGuide == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pGuide = new CGuide;	// ガイド
	}
	else { assert(false); return NULL; }	// 使用中

	if (pGuide != NULL)
	{ // 確保に成功している場合

		// ガイドの初期化
		if (FAILED(pGuide->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pGuide;
			pGuide = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pGuide->SetVec3Position(rPos);

		// 大きさを設定
		pGuide->SetVec3Sizing(rSize);

		// 確保したアドレスを返す
		return pGuide;
	}
	else { assert(false); return NULL; }	// 確保失敗
}
