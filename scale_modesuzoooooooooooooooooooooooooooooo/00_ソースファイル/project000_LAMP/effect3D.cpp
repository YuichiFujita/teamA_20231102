//============================================================
//
//	エフェクト3D処理 [effect3D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "effect3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define EFF_PRIO	(9)	// エフェクト3Dの優先順位
#define PART_PRIO	(9)	// パーティクル3Dの優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CEffect3D::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\effect000.jpg",	// 通常テクスチャ
	"data\\TEXTURE\\effect001.png",	// バブルテクスチャ
	"data\\TEXTURE\\effect002.tga",	// 煙テクスチャ
	"data\\TEXTURE\\effect003.jpg",	// 回復テクスチャ
	"data\\TEXTURE\\effect004.png",	// 葉っぱテクスチャ
	"data\\TEXTURE\\effect005.jpg",	// かけらテクスチャ (小)
	"data\\TEXTURE\\effect006.jpg",	// かけらテクスチャ (中)
	"data\\TEXTURE\\effect007.jpg",	// かけらテクスチャ (大)
};

//************************************************************
//	子クラス [CEffect3D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEffect3D::CEffect3D() : CObjectBillboard(CObject::LABEL_EFFECT), m_type(TYPE_NORMAL)
{
	// メンバ変数をクリア
	m_move = VEC3_ZERO;		// 移動量
	m_nLife		= 0;		// 寿命
	m_fSubSize	= 0.0f;		// 大きさの減算量
	m_fSubAlpha	= 0.0f;		// 透明度の減算量
	m_bAdd		= false;	// 加算合成状況
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CEffect3D::CEffect3D(const EType type, const CObject::ELabel label) : CObjectBillboard(CObject::LABEL_EFFECT, (label == LABEL_EFFECT) ? EFF_PRIO : PART_PRIO), m_type(type)
{
	// メンバ変数をクリア
	m_move = VEC3_ZERO;		// 移動量
	m_nLife		= 0;		// 寿命
	m_fSubSize	= 0.0f;		// 大きさの減算量
	m_fSubAlpha	= 0.0f;		// 透明度の減算量
	m_bAdd		= false;	// 加算合成状況
}

//============================================================
//	デストラクタ
//============================================================
CEffect3D::~CEffect3D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEffect3D::Init(void)
{
	// メンバ変数を初期化
	m_move = VEC3_ZERO;	// 移動量
	m_nLife		= 0;	// 寿命
	m_fSubSize	= 0.0f;	// 大きさの減算量
	m_fSubAlpha	= 0.0f;	// 透明度の減算量
	m_bAdd		= true;	// 加算合成状況

	// オブジェクトビルボードの初期化
	if (FAILED(CObjectBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// Zテストを設定
	SetFunc(D3DCMP_ALWAYS);

	// Zバッファの使用状況を設定
	SetZEnable(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEffect3D::Uninit(void)
{
	// オブジェクトビルボードの終了
	CObjectBillboard::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEffect3D::Update(void)
{
	// 変数を宣言
	D3DXVECTOR3 pos  = GetVec3Position();	// 位置
	D3DXVECTOR3 rot  = GetVec3Rotation();	// 向き
	D3DXVECTOR3 size = GetVec3Sizing();		// 大きさ
	D3DXCOLOR   col  = GetColor();			// 色
	float fRadius    = size.x;				// 半径

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
	CObjectBillboard::SetVec3Position(pos);

	// 向きを設定
	CObjectBillboard::SetVec3Rotation(rot);

	// 大きさを設定
	CObjectBillboard::SetVec3Sizing(D3DXVECTOR3(fRadius, fRadius, 0.0f));

	// 色を設定
	CObjectBillboard::SetColor(col);

	// オブジェクトビルボードの更新
	CObjectBillboard::Update();
}

//============================================================
//	描画処理
//============================================================
void CEffect3D::Draw(void)
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

	// オブジェクトビルボードの描画
	CObjectBillboard::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================
//	生成処理
//============================================================
CEffect3D *CEffect3D::Create
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
	CEffect3D *pEffect3D = NULL;	// エフェクト3D生成用

	if (pEffect3D == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pEffect3D = new CEffect3D(type, label);	// エフェクト3D
	}
	else { assert(false); return NULL; }	// 使用中

	if (pEffect3D != NULL)
	{ // 確保に成功している場合

		// エフェクト3Dの初期化
		if (FAILED(pEffect3D->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pEffect3D;
			pEffect3D = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャを登録
		nTextureID = pTexture->Regist(mc_apTextureFile[type]);

		// テクスチャを割当
		pEffect3D->BindTexture(nTextureID);

		// 位置を設定
		pEffect3D->SetVec3Position(rPos);

		// 向きを設定
		pEffect3D->SetVec3Rotation(rRot);

		// 大きさを設定
		pEffect3D->SetVec3Sizing(D3DXVECTOR3(fRadius, fRadius, 0.0f));

		// 色を設定
		pEffect3D->SetColor(rCol);

		// 引数の情報を設定
		pEffect3D->m_move		= rMove;		// 移動量
		pEffect3D->m_nLife		= nLife;		// 寿命
		pEffect3D->m_fSubSize	= fSubSize;		// 大きさの減算量
		pEffect3D->m_fSubAlpha	= 1.0f / nLife;	// 透明度の減算量
		pEffect3D->m_bAdd		= bAdd;			// 加算合成状況

		// 確保したアドレスを返す
		return pEffect3D;
	}
	else { assert(false); return NULL; }	// 確保失敗
}
