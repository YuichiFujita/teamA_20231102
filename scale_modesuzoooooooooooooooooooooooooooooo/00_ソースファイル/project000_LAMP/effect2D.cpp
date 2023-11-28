//============================================================
//
//	エフェクト2D処理 [effect2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "effect2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define EFF_PRIO	(7)	// エフェクト2Dの優先順位
#define PART_PRIO	(9)	// パーティクル2Dの優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CEffect2D::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\effect000.jpg",	// 通常テクスチャ
};

//************************************************************
//	子クラス [CEffect2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEffect2D::CEffect2D() : CObject2D(CObject::LABEL_EFFECT), m_type(TYPE_NORMAL)
{
	// メンバ変数をクリア
	m_move = VEC3_ZERO;	// 移動量
	m_nLife		= 0;	// 寿命
	m_fSubSize	= 0.0f;	// 大きさの減算量
	m_fSubAlpha	= 0.0f;	// 透明度の減算量
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CEffect2D::CEffect2D(const EType type, const CObject::ELabel label) : CObject2D(CObject::LABEL_EFFECT, (label == LABEL_EFFECT) ? EFF_PRIO : PART_PRIO), m_type(type)
{
	// メンバ変数をクリア
	m_move = VEC3_ZERO;	// 移動量
	m_nLife		= 0;	// 寿命
	m_fSubSize	= 0.0f;	// 大きさの減算量
	m_fSubAlpha	= 0.0f;	// 透明度の減算量
}

//============================================================
//	デストラクタ
//============================================================
CEffect2D::~CEffect2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEffect2D::Init(void)
{
	// メンバ変数を初期化
	m_move = VEC3_ZERO;	// 移動量
	m_nLife		= 0;	// 寿命
	m_fSubSize	= 0.0f;	// 大きさの減算量
	m_fSubAlpha	= 0.0f;	// 透明度の減算量

	// オブジェクト2Dの初期化
	if (FAILED(CObject2D::Init()))
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
void CEffect2D::Uninit(void)
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEffect2D::Update(void)
{
	// 変数を宣言
	D3DXVECTOR3 pos  = GetVec3Position();	// 位置
	D3DXVECTOR3 rot  = GetVec3Rotation();	// 向き
	D3DXVECTOR3 size = GetVec3Sizing();		// 大きさ
	D3DXCOLOR   col  = GetColor();			// 色
	float fRadius = size.x;					// 半径

	if (m_nLife <= 0		// 寿命を迎えた
	||  fRadius <= 0.0f)	// 半径が0.0f以下
	{ // 上記のどれかになった場合

		// オブジェクトを破棄
		Uninit();

		// 関数を抜ける
		return;
	}

	// 移動量を加算
	pos += m_move;

	// 寿命を減算
	m_nLife--;

	// 半径を減算
	fRadius -= m_fSubSize;
	if (fRadius < 0.0f)
	{ // 半径が0.0fより小さい場合

		// 半径を補正
		fRadius = 0.0f;
	}

	// α値を減算
	col.a -= m_fSubAlpha;
	useful::LimitNum(col.a, 0.0f, 1.0f);	// α値制限

	// 位置を設定
	CObject2D::SetVec3Position(pos);

	// 向きを設定
	CObject2D::SetVec3Rotation(rot);

	// 大きさを設定
	CObject2D::SetVec3Sizing(D3DXVECTOR3(fRadius, fRadius, 0.0f));

	// 色を設定
	CObject2D::SetColor(col);

	// オブジェクト2Dの更新
	CObject2D::Update();
}

//============================================================
//	描画処理
//============================================================
void CEffect2D::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	if (m_bAdd)
	{ // 加算合成がONの場合

		// αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// オブジェクト2Dの描画
	CObject2D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================
//	生成処理
//============================================================
CEffect2D *CEffect2D::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const float fRadius,		// 半径
	const EType type,			// テクスチャ
	const int nLife,			// 寿命
	const D3DXVECTOR3& rMove,	// 移動量
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXCOLOR& rCol,		// 色
	const float fSubSize,		// 半径の減算量
	const bool bAdd,			// 加算合成状況
	const CObject::ELabel label	// オブジェクトラベル
)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	CEffect2D *pEffect2D = NULL;	// エフェクト2D生成用

	if (pEffect2D == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pEffect2D = new CEffect2D(type, label);	// エフェクト2D
	}
	else { assert(false); return NULL; }	// 使用中

	if (pEffect2D != NULL)
	{ // 確保に成功している場合

		// エフェクト2Dの初期化
		if (FAILED(pEffect2D->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pEffect2D;
			pEffect2D = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャを登録
		nTextureID = pTexture->Regist(mc_apTextureFile[type]);

		// テクスチャを割当
		pEffect2D->BindTexture(nTextureID);

		// 位置を設定
		pEffect2D->SetVec3Position(rPos);

		// 向きを設定
		pEffect2D->SetVec3Rotation(rRot);

		// 大きさを設定
		pEffect2D->SetVec3Sizing(D3DXVECTOR3(fRadius, fRadius, 0.0f));

		// 色を設定
		pEffect2D->SetColor(rCol);

		// 引数の情報を設定
		pEffect2D->m_move		= rMove;		// 移動量
		pEffect2D->m_nLife		= nLife;		// 寿命
		pEffect2D->m_fSubSize	= fSubSize;		// 大きさの減算量
		pEffect2D->m_fSubAlpha	= 1.0f / nLife;	// 透明度の減算量
		pEffect2D->m_bAdd		= bAdd;			// 加算合成状況

		// 確保したアドレスを返す
		return pEffect2D;
	}
	else { assert(false); return NULL; }	// 確保失敗
}
