//============================================================
//
//	マスク処理 [mask.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "mask.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY = 5;	// マスクの優先順位
}

//************************************************************
//	子クラス [CMask] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CMask::CMask() : CObject2D(CObject::LABEL_UI)
{

}

//============================================================
//	デストラクタ
//============================================================
CMask::~CMask()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CMask::Init(void)
{
	// オブジェクト2Dの初期化
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 影の表示をOFFにする
	SetEnableDepthShadow(false);
	SetEnableZTex(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CMask::Uninit(void)
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CMask::Update(void)
{
	// オブジェクト2Dの更新
	CObject2D::Update();
}

//============================================================
//	描画処理
//============================================================
void CMask::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// ステンシルテストを有効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// 比較参照値を設定する
	pDevice->SetRenderState(D3DRS_STENCILREF, 2);

	// ステンシルマスクを指定する 
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	// ステンシル比較関数を指定する
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);

	// ステンシル結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS,	D3DSTENCILOP_KEEP);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL,	D3DSTENCILOP_KEEP);	// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL,	D3DSTENCILOP_KEEP);	// Zテスト失敗・ステンシルテスト成功

	// オブジェクト2Dの描画
	CObject2D::Draw();

	// ステンシルテストを無効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//============================================================
//	生成処理
//============================================================
CMask *CMask::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rSize,	// 大きさ
	const D3DXCOLOR& rCol		// 色
)
{
	// ポインタを宣言
	CMask *pMask = NULL;	// マスク生成用

	if (pMask == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pMask = new CMask;	// マスク
	}
	else { assert(false); return NULL; }	// 使用中

	if (pMask != NULL)
	{ // 確保に成功している場合

		// マスクの初期化
		if (FAILED(pMask->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pMask;
			pMask = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pMask->SetVec3Position(rPos);

		// 大きさを設定
		pMask->SetVec3Sizing(rSize);

		// 色を設定
		pMask->SetColor(rCol);

		// 確保したアドレスを返す
		return pMask;
	}
	else { assert(false); return NULL; }	// 確保失敗
}
