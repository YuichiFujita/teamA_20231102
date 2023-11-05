//============================================================
//
//	パーティクル2D処理 [particle2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "particle2D.h"
#include "effect2D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define DMG_MOVE		(3.0f)		// ダメージの移動量
#define DMG_SPAWN		(35)		// ダメージの生成数
#define DMG_EFF_LIFE	(120)		// ダメージの寿命
#define DMG_SIZE		(50.0f)		// ダメージの大きさ
#define DMG_SUB_SIZE	(1.8f)		// ダメージの半径の減算量

#define ITEM_MOVE		(2.0f)		// アイテムの移動量
#define ITEM_SPAWN		(6)			// アイテムの生成数
#define ITEM_EFF_LIFE	(80)		// アイテムの寿命
#define ITEM_SIZE		(30.0f)		// アイテムの大きさ
#define ITEM_SUB_SIZE	(1.2f)		// アイテムの半径の減算量

#define GETITEM_MOVE		(3.0f)	// アイテム取得の移動量
#define GETITEM_SPAWN		(35)	// アイテム取得の生成数
#define GETITEM_EFF_LIFE	(120)	// アイテム取得の寿命
#define GETITEM_SIZE		(50.0f)	// アイテム取得の大きさ
#define GETITEM_SUB_SIZE	(1.8f)	// アイテム取得の半径の減算量

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const int CParticle2D::mc_aLife[CParticle2D::TYPE_MAX] =	// 寿命定数
{
	0,	// なし
	18,	// ダメージ
	1,	// アイテム
	2,	// アイテム取得
};

//************************************************************
//	子クラス [CParticle2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CParticle2D::CParticle2D() : CObject(CObject::LABEL_PARTICLE)
{
	// メンバ変数をクリア
	m_pos	= VEC3_ZERO;	// 位置
	m_col	= XCOL_WHITE;	// 色
	m_type	= TYPE_NONE;	// 種類
	m_nLife	= 0;			// 寿命
}

//============================================================
//	デストラクタ
//============================================================
CParticle2D::~CParticle2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CParticle2D::Init(void)
{
	// メンバ変数を初期化
	m_pos	= VEC3_ZERO;	// 位置
	m_col	= XCOL_WHITE;	// 色
	m_type	= TYPE_NONE;	// 種類
	m_nLife	= 0;			// 寿命

	// 成功を返す
	return S_OK;
}

//============================================================
//	初期化処理
//============================================================
HRESULT CParticle2D::Init(const EType type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// メンバ変数を初期化
	m_pos	= rPos;				// 位置
	m_col	= rCol;				// 色
	m_type	= type;				// 種類
	m_nLife	= mc_aLife[type];	// 寿命

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CParticle2D::Uninit(void)
{
	// パーティクル2Dオブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CParticle2D::Update(void)
{
	if (m_nLife > 0)
	{ // 寿命がある場合

		// 寿命を減算
		m_nLife--;
	}
	else
	{ // 寿命がない場合

		// パーティクル2Dオブジェクトの終了
		Uninit();

		// 関数を抜ける
		return;
	}

	switch (m_type)
	{ // 種類ごとの処理
	case TYPE_DAMAGE:

		// ダメージパーティクル2D
		Damage(m_pos, m_col);

		break;

	case TYPE_ITEM:

		// アイテムパーティクル2D
		Item(m_pos, m_col);

		break;

	case TYPE_GETITEM:

		// アイテム取得パーティクル2D
		GetItem(m_pos, m_col);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	描画処理
//============================================================
void CParticle2D::Draw(void)
{

}

//============================================================
//	ダメージパーティクル2D処理
//============================================================
void CParticle2D::Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// 変数を宣言
	D3DXVECTOR3 move = VEC3_ZERO;	// 移動量の代入用
	D3DXVECTOR3 rot  = VEC3_ZERO;	// 向きの代入用

	if ((m_nLife + 1) % 9 == 0)
	{ // 寿命が9の倍数の場合

		for (int nCntPart = 0; nCntPart < DMG_SPAWN; nCntPart++)
		{ // 生成されるエフェクト数分繰り返す

			// ベクトルをランダムに設定
			move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.z = 0.0f;

			// ベクトルを正規化
			D3DXVec3Normalize(&move, &move);

			// 移動量を設定
			move.x *= DMG_MOVE;
			move.y *= DMG_MOVE;
			move.z = 0.0f;

			// 向きを設定
			rot = VEC3_ZERO;

			// エフェクト2Dオブジェクトの生成
			CEffect2D::Create
			( // 引数
				rPos,					// 位置
				DMG_SIZE,				// 半径
				CEffect2D::TYPE_NORMAL,	// テクスチャ
				DMG_EFF_LIFE,			// 寿命
				move,					// 移動量
				rot,					// 向き
				rCol,					// 色
				DMG_SUB_SIZE,			// 半径の減算量
				true,					// 加算合成状況
				LABEL_PARTICLE			// オブジェクトラベル
			);
		}
	}
}

//============================================================
//	アイテムパーティクル2D処理
//============================================================
void CParticle2D::Item(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// 変数を宣言
	D3DXVECTOR3 move = VEC3_ZERO;	// 移動量の代入用
	D3DXVECTOR3 rot  = VEC3_ZERO;	// 向きの代入用

	for (int nCntPart = 0; nCntPart < ITEM_SPAWN; nCntPart++)
	{ // 生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = 0.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を設定
		move.x *= ITEM_MOVE;
		move.y *= ITEM_MOVE;
		move.z = 0.0f;

		// 向きを設定
		rot = VEC3_ZERO;

		// エフェクト2Dオブジェクトの生成
		CEffect2D::Create
		( // 引数
			rPos,					// 位置
			ITEM_SIZE,				// 半径
			CEffect2D::TYPE_NORMAL,	// テクスチャ
			ITEM_EFF_LIFE,			// 寿命
			move,					// 移動量
			rot,					// 向き
			rCol,					// 色
			ITEM_SUB_SIZE,			// 半径の減算量
			true,					// 加算合成状況
			LABEL_PARTICLE			// オブジェクトラベル
		);
	}
}

//============================================================
//	アイテム取得パーティクル2D処理
//============================================================
void CParticle2D::GetItem(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// 変数を宣言
	D3DXVECTOR3 move = VEC3_ZERO;	// 移動量の代入用
	D3DXVECTOR3 rot  = VEC3_ZERO;	// 向きの代入用

	for (int nCntPart = 0; nCntPart < GETITEM_SPAWN; nCntPart++)
	{ // 生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = 0.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を設定
		move.x *= GETITEM_MOVE;
		move.y *= GETITEM_MOVE;
		move.z = 0.0f;

		// 向きを設定
		rot = VEC3_ZERO;

		// エフェクト2Dオブジェクトの生成
		CEffect2D::Create
		( // 引数
			rPos,					// 位置
			GETITEM_SIZE,			// 半径
			CEffect2D::TYPE_NORMAL,	// テクスチャ
			GETITEM_EFF_LIFE,		// 寿命
			move,					// 移動量
			rot,					// 向き
			rCol,					// 色
			GETITEM_SUB_SIZE,		// 半径の減算量
			true,					// 加算合成状況
			LABEL_PARTICLE			// オブジェクトラベル
		);
	}
}

//============================================================
//	生成処理
//============================================================
CParticle2D *CParticle2D::Create(const EType type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// ポインタを宣言
	CParticle2D *pParticle2D = NULL;	// パーティクル2D生成用

	if (pParticle2D == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pParticle2D = new CParticle2D;	// パーティクル2D
	}
	else { assert(false); return NULL; }	// 使用中

	if (pParticle2D != NULL)
	{ // 確保に成功している場合

		// パーティクル2Dの初期化
		if (FAILED(pParticle2D->Init(type, rPos, rCol)))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pParticle2D;
			pParticle2D = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pParticle2D;
	}
	else { assert(false); return NULL; }	// 確保失敗
}
