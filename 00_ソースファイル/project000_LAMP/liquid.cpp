//============================================================
//
//	液体処理 [liquid.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "liquid.h"
#include "manager.h"
#include "renderer.h"
#include "scrollMeshField.h"

//************************************************************
//	マクロ定義
//************************************************************
#define LIQUID_PRIO	(0)		// 液体の優先順位
#define ADD_ROT		(80)	// 頂点ごとのカーブ加算量
#define ADD_UP		(10.0f)	// 波の最高点

#define ADD_SINROT	(D3DXToRadian(0.5f))	// 毎フレームの波打ち向き加算量

#define LIQUID_DOWN	(D3DXVECTOR2(-0.0f, 0.002f))	// 液体(下)の移動量
#define LIQUID_UP	(D3DXVECTOR2(-0.0f, 0.0001f))	// 液体(上)の移動量

#define LIQUID_ROT	(D3DXVECTOR3(0.0f, D3DXToRadian(45), 0.0f))	// 向き
#define LIQUID_SIZE	(D3DXVECTOR2(24000.0f, 24000.0f))			// 大きさ

#define LIQUID_COL	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// 色
#define LIQUID_PART	(POSGRID2(128, 128))				// 分割数

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CLiquid::mc_apTextureFile[][LIQUID_MAX] =	// テクスチャ定数
{
	{ // 海テクスチャ
		"data\\TEXTURE\\sea000.jpg",	// 海(下)
		"data\\TEXTURE\\sea000.png",	// 海(上)
	},
};

//************************************************************
//	子クラス [CLiquid] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CLiquid::CLiquid() : CObject(CObject::LABEL_LIQUID, LIQUID_PRIO)
{
	// メンバ変数をクリア
	memset(&m_apLiquid[0], 0, sizeof(m_apLiquid));	// 液体の情報
	m_fSinRot = 0.0f;	// 波打ち向き
}

//============================================================
//	デストラクタ
//============================================================
CLiquid::~CLiquid()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CLiquid::Init(void)
{
	// 変数配列を宣言
	D3DXVECTOR2 aMoveTexU[LIQUID_MAX] =	// テクスチャの横移動量の設定用
	{ // 初期値
		LIQUID_UP,		// 液体(下)の移動量
		LIQUID_DOWN,	// 液体(上)の移動量
	};

	D3DXCOLOR aCol[LIQUID_MAX] =	// 色の設定用
	{ // 初期値
		XCOL_WHITE,	// 液体(下)の色
		LIQUID_COL,	// 液体(上)の色
	};

	// メンバ変数を初期化
	memset(&m_apLiquid[0], 0, sizeof(m_apLiquid));	// 液体の情報
	m_fSinRot = 0.0f;	// 波打ち向き

	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // 液体の最大数分繰り返す

		// 液体の生成
		m_apLiquid[nCntLiquid] = CScrollMeshField::Create
		( // 引数
			aMoveTexU[nCntLiquid].x,	// 横座標の移動量
			aMoveTexU[nCntLiquid].y,	// 縦座標の移動量
			VEC3_ZERO,		// 位置
			LIQUID_ROT,		// 向き
			LIQUID_SIZE,		// 大きさ
			aCol[nCntLiquid],	// 色
			LIQUID_PART,		// 分割数
			D3DCULL_CCW,	// カリング状況
			false			// ライティング状況
		);
		if (m_apLiquid[nCntLiquid] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを登録・割当
		m_apLiquid[nCntLiquid]->BindTexture(mc_apTextureFile[0][nCntLiquid]);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CLiquid::Uninit(void)
{
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // 液体の最大数分繰り返す

		// 液体の終了
		m_apLiquid[nCntLiquid]->Uninit();
	}

	// 自身のオブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CLiquid::Update(void)
{
	// 変数を宣言
	POSGRID2 part = m_apLiquid[0]->GetPattern();	// 分割数
	D3DXVECTOR3 pos;	// 頂点座標取得用
	int nLine;			// 頂点の行
	float fSinRot;		// サインカーブに使用する角度

	// メッシュの頂点座標の更新
	for (int nCntVtx = 0; nCntVtx < (part.x + 1) * (part.y + 1); nCntVtx++)
	{ // 使用頂点数分繰り返す

		for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
		{ // 液体の最大数分繰り返す

			// 頂点座標を取得
			pos = m_apLiquid[nCntLiquid]->GetMeshVertexPosition(nCntVtx);

			// 波打ち用の角度を求める
			nLine = nCntVtx / (part.x + 1);							// 頂点の行
			fSinRot = m_fSinRot + D3DXToRadian(nLine * ADD_ROT);	// サインカーブに使用する角度

			// サインカーブ用の向きを補正
			useful::NormalizeRot(fSinRot);

			// 頂点座標を設定
			m_apLiquid[nCntLiquid]->SetMeshVertexPosition(nCntVtx, D3DXVECTOR3(pos.x, (sinf(fSinRot) * ADD_UP) + ADD_UP, pos.z));
		}
	}

	// 波打ち向きを加算
	m_fSinRot += ADD_SINROT;

	// 向きを補正
	useful::NormalizeRot(m_fSinRot);

	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // 液体の最大数分繰り返す

		// 液体の更新
		m_apLiquid[nCntLiquid]->Update();
	}
}

//============================================================
//	描画処理
//============================================================
void CLiquid::Draw(void)
{

}

//============================================================
//	生成処理
//============================================================
CLiquid *CLiquid::Create(void)
{
	// ポインタを宣言
	CLiquid *pLiquid = NULL;	// 液体生成用

	if (pLiquid == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pLiquid = new CLiquid;	// 液体
	}
	else { assert(false); return NULL; }	// 使用中

	if (pLiquid != NULL)
	{ // 確保に成功している場合

		// 液体の初期化
		if (FAILED(pLiquid->Init()))
		{ // 初期化に失敗した場合

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pLiquid;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
void CLiquid::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}
