//============================================================
//
//	空処理 [sky.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sky.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define SKY_ROTMOVE	(0.001f)	// 向き変更量
#define SKY_PRIO	(0)			// 空の優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CSky::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\sky000.png",	// 通常テクスチャ
};

//************************************************************
//	子クラス [CSky] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSky::CSky() : CObjectMeshDome(CObject::LABEL_SKY, SKY_PRIO)
{

}

//============================================================
//	デストラクタ
//============================================================
CSky::~CSky()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSky::Init(void)
{
	// オブジェクトメッシュドームの初期化
	if (FAILED(CObjectMeshDome::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSky::Uninit(void)
{
	// オブジェクトメッシュドームの終了
	CObjectMeshDome::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSky::Update(void)
{
	// 変数を宣言
	D3DXVECTOR3 rot = GetVec3Rotation();	// 向き

	// 向きを変更
	rot.y -= SKY_ROTMOVE;

	// 向きを設定
	SetVec3Rotation(rot);

	// オブジェクトメッシュドームの更新
	CObjectMeshDome::Update();
}

//============================================================
//	描画処理
//============================================================
void CSky::Draw(void)
{
	// オブジェクトメッシュドームの描画
	CObjectMeshDome::Draw();
}

//============================================================
//	生成処理
//============================================================
CSky *CSky::Create
(
	const ETexture texture,		// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const float fRadius			// 半径
)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	CSky *pSky = NULL;	// 空生成用

	if (pSky == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pSky = new CSky;	// 空
	}
	else { assert(false); return NULL; }	// 使用中

	if (pSky != NULL)
	{ // 確保に成功している場合

		// 空の初期化
		if (FAILED(pSky->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pSky;
			pSky = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャを登録
		nTextureID = pTexture->Regist(mc_apTextureFile[texture]);

		// テクスチャを割当
		pSky->BindTexture(nTextureID);

		// 位置を設定
		pSky->SetVec3Position(rPos);

		// 向きを設定
		pSky->SetVec3Rotation(rRot);

		// 色を設定
		pSky->SetColor(rCol);

		// 半径を設定
		pSky->SetRadius(fRadius);

		// カリングを設定
		pSky->SetCulling(D3DCULL_CW);

		// ライティングを設定
		pSky->SetLighting(false);

		// 分割数を設定
		if (FAILED(pSky->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// メモリ開放
			delete pSky;
			pSky = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャ分割数の設定
		pSky->SetTexPattern(POSGRID2(rPart.x, rPart.y - 1));

		// 確保したアドレスを返す
		return pSky;
	}
	else { assert(false); return NULL; }	// 確保失敗
}
