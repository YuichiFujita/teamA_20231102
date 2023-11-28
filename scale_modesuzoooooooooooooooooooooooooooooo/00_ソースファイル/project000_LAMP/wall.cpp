//============================================================
//
//	壁処理 [wall.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "wall.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define WALL_PRIO	(0)	// 壁の優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CWall::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\wall000.png",	// 通常テクスチャ
};

//************************************************************
//	子クラス [CWall] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CWall::CWall() : CObjectMeshWall(CObject::LABEL_WALL, WALL_PRIO)
{

}

//============================================================
//	デストラクタ
//============================================================
CWall::~CWall()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CWall::Init(void)
{
	// オブジェクトメッシュウォールの初期化
	if (FAILED(CObjectMeshWall::Init()))
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
void CWall::Uninit(void)
{
	// オブジェクトメッシュウォールの終了
	CObjectMeshWall::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CWall::Update(void)
{
	// オブジェクトメッシュウォールの更新
	CObjectMeshWall::Update();
}

//============================================================
//	描画処理
//============================================================
void CWall::Draw(void)
{
	// オブジェクトメッシュウォールの描画
	CObjectMeshWall::Draw();
}

//============================================================
//	生成処理
//============================================================
CWall *CWall::Create
(
	const ETexture texture,		// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR2& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const D3DCULL cull,			// カリング状況
	const bool bLight			// ライティング状況
)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	CWall *pWall = NULL;	// 壁生成用

	if (pWall == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pWall = new CWall;	// 壁
	}
	else { assert(false); return NULL; }	// 使用中

	if (pWall != NULL)
	{ // 確保に成功している場合

		// 壁の初期化
		if (FAILED(pWall->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pWall;
			pWall = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャを登録
		nTextureID = pTexture->Regist(mc_apTextureFile[texture]);

		// テクスチャを割当
		pWall->BindTexture(nTextureID);

		// 位置を設定
		pWall->SetVec3Position(rPos);

		// 向きを設定
		pWall->SetVec3Rotation(rRot);

		// 大きさを設定
		pWall->SetVec2Sizing(rSize);

		// 色を設定
		pWall->SetColor(rCol);

		// カリングを設定
		pWall->SetCulling(cull);

		// ライティングを設定
		pWall->SetLighting(bLight);

		// 分割数を設定
		if (FAILED(pWall->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// メモリ開放
			delete pWall;
			pWall = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pWall;
	}
	else { assert(false); return NULL; }	// 確保失敗
}
