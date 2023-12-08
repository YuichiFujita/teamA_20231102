//============================================================
//
//	地盤処理 [ground.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "ground.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define GROUND_PRIO	(1)	// 地盤の優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CGround::mc_apTextureFile[][6] =	// テクスチャ定数
{
	{ // 草原テクスチャ
		"data\\TEXTURE\\soil2.png",	// 左テクスチャ
		"data\\TEXTURE\\soil2.png",	// 右テクスチャ
		"data\\TEXTURE\\soil2.png",	// 下テクスチャ
		"data\\TEXTURE\\soil.png",	// 上テクスチャ
		"data\\TEXTURE\\soil2.png",	// 前テクスチャ
		"data\\TEXTURE\\soil2.png",	// 後テクスチャ
	},

	{ // 石テクスチャ
		"data\\TEXTURE\\block000.png",	// 左テクスチャ
		"data\\TEXTURE\\block000.png",	// 右テクスチャ
		"data\\TEXTURE\\block000.png",	// 下テクスチャ
		"data\\TEXTURE\\block000.png",	// 上テクスチャ
		"data\\TEXTURE\\block000.png",	// 前テクスチャ
		"data\\TEXTURE\\block000.png",	// 後テクスチャ
	},
};

//************************************************************
//	子クラス [CGround] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGround::CGround() : CObjectMeshCube(CObject::LABEL_GROUND, GROUND_PRIO)
{
	// メンバ変数をクリア
	m_type = TYPE_GRASS;	// 種類
}

//============================================================
//	デストラクタ
//============================================================
CGround::~CGround()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGround::Init(void)
{
	// メンバ変数を初期化
	m_type = TYPE_GRASS;	// 種類

	// オブジェクトメッシュキューブの初期化
	if (FAILED(CObjectMeshCube::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 原点を設定
	SetOrigin(CObjectMeshCube::ORIGIN_UP);

	// キューブ色を設定
	SetColor(XCOL_WHITE);

	// 縁取り色を設定
	SetBorderColor(XCOL_WHITE);

	// 縁取りの状態を設定
	if (FAILED(SetBorderState(CObjectMeshCube::BORDER_OFF)))
	{ // 分割数の設定に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 縁取りの太さを設定
	SetBorderThick(0.0f);

	// テクスチャの状態を設定
	SetTextureState(CObjectMeshCube::TEXSTATE_SELECT);

	// カリングを設定
	SetCulling(D3DCULL_CCW);

	// ライティングを設定
	SetLighting(true);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGround::Uninit(void)
{
	// オブジェクトメッシュキューブの終了
	CObjectMeshCube::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGround::Update(void)
{
	// オブジェクトメッシュキューブの更新
	CObjectMeshCube::Update();
}

//============================================================
//	描画処理
//============================================================
void CGround::Draw(void)
{
	// オブジェクトメッシュキューブの描画
	CObjectMeshCube::Draw();
}

//============================================================
//	種類の設定処理
//============================================================
void CGround::SetType(const int nType)
{
	// 変数を宣言
	SFaceTex faceTex;	// テクスチャインデックス設定用

	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	if (pTexture == NULL)
	{ // 使用されていない場合

		// 処理を抜ける
		assert(false);
		return;
	}

	if (nType > NONE_IDX && nType < TYPE_MAX)
	{ // 種類が範囲内の場合

		// 引数の種類を設定
		m_type = (EType)nType;

		// 引数の種類のテクスチャを登録
		faceTex = SFaceTex
		( // 引数
			pTexture->Regist(mc_apTextureFile[nType][0]),	// 左
			pTexture->Regist(mc_apTextureFile[nType][1]),	// 右
			pTexture->Regist(mc_apTextureFile[nType][2]),	// 下
			pTexture->Regist(mc_apTextureFile[nType][3]),	// 上
			pTexture->Regist(mc_apTextureFile[nType][4]),	// 前
			pTexture->Regist(mc_apTextureFile[nType][5])	// 後
		);
		BindTexture(faceTex);	// テクスチャを割当
	}
	else { assert(false); }	// 種類オーバー
}

//============================================================
//	種類取得処理
//============================================================
int CGround::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//============================================================
//	生成処理
//============================================================
CGround *CGround::Create
(
	const EType type,				// 種類
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rRot,		// 向き
	const D3DXVECTOR3& rSize,		// 大きさ
	const D3DXVECTOR2& rTexPartX,	// テクスチャ分割数X
	const D3DXVECTOR2& rTexPartY,	// テクスチャ分割数Y
	const D3DXVECTOR2& rTexPartZ	// テクスチャ分割数Z
)
{
	// ポインタを宣言
	CGround *pGround = NULL;	// 地盤生成用

	if (pGround == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pGround = new CGround;	// 地盤
	}
	else { assert(false); return NULL; }	// 使用中

	if (pGround != NULL)
	{ // 確保に成功している場合

		// 地盤の初期化
		if (FAILED(pGround->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pGround;
			pGround = NULL;

			// 失敗を返す
			return NULL;
		}

		// 種類を設定
		pGround->SetType(type);

		// 位置を設定
		pGround->SetVec3Position(rPos);

		// 向きを設定
		pGround->SetVec3Rotation(rRot);

		// 大きさを設定
		pGround->SetVec3Sizing(rSize);
		int nSprit;
		// テクスチャの分割数Xを設定
		if (type == TYPE_GRASS)
		{
			nSprit = 5;
		}
		else
		{
			nSprit = 2;
		}
		pGround->SetTexturePatternX(rTexPartX / nSprit);

		// テクスチャの分割数Yを設定
		pGround->SetTexturePatternY(rTexPartY / nSprit);

		// テクスチャの分割数Zを設定
		pGround->SetTexturePatternZ(rTexPartZ / nSprit);

		// 確保したアドレスを返す
		return pGround;
	}
	else { assert(false); return NULL; }	// 確保失敗
}
