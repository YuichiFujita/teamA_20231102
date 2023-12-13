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
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 0;	// 液体の優先順位
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CLiquid::mc_apTextureFile[][LIQUID_MAX] =	// テクスチャ定数
{
	{ // 海テクスチャ
		"data\\TEXTURE\\sea000.jpg",	// 海(下)
		"data\\TEXTURE\\sea000.png",	// 海(上)
	},

	{ // 溶岩テクスチャ
		"data\\TEXTURE\\Lava002.png",	// 海(下)
		"data\\TEXTURE\\Lava003.png",	// 海(上)
	},
};

//************************************************************
//	子クラス [CLiquid] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CLiquid::CLiquid() : CObject(CObject::LABEL_LIQUID, PRIORITY)
{
	// メンバ変数をクリア
	memset(&m_apLiquid[0], 0, sizeof(m_apLiquid));	// 液体の情報
	m_type			= TYPE_SEA;	// 種類
	m_fMaxUp		= 0.0f;		// 波の最高上昇量
	m_fSinRot		= 0.0f;		// 波打ち向き
	m_fAddSinRot	= 0.0f;		// 波打ち向き加算量
	m_fAddVtxRot	= 0.0f;		// 隣波の向き加算量
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
	// メンバ変数を初期化
	memset(&m_apLiquid[0], 0, sizeof(m_apLiquid));	// 液体の情報
	m_type			= TYPE_SEA;	// 種類
	m_fMaxUp		= 0.0f;		// 波の最高上昇量
	m_fSinRot		= 0.0f;		// 波打ち向き
	m_fAddSinRot	= 0.0f;		// 波打ち向き加算量
	m_fAddVtxRot	= 0.0f;		// 隣波の向き加算量

	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // 液体の最大数分繰り返す

		// 液体の生成
		m_apLiquid[nCntLiquid] = CScrollMeshField::Create
		( // 引数
			0.0f,			// 横座標の移動量
			0.0f,			// 縦座標の移動量
			VEC3_ZERO,		// 位置
			VEC3_ZERO,		// 向き
			VEC2_ZERO,		// 大きさ
			XCOL_WHITE,		// 色
			GRID2_ONE,		// 分割数
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
		m_apLiquid[nCntLiquid]->BindTexture(mc_apTextureFile[m_type][nCntLiquid]);
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
	POSGRID2 part = GetPattern();	// 分割数
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
			nLine = nCntVtx / (part.x + 1);				// 頂点の行
			fSinRot = m_fSinRot + nLine * m_fAddVtxRot;	// サインカーブに使用する角度

			// サインカーブ用の向きを補正
			useful::NormalizeRot(fSinRot);

			// 頂点座標を設定
			m_apLiquid[nCntLiquid]->SetMeshVertexPosition(nCntVtx, D3DXVECTOR3(pos.x, (sinf(fSinRot) * m_fMaxUp) + m_fMaxUp, pos.z));
		}
	}

	// 波打ち向きを加算
	m_fSinRot += m_fAddSinRot;

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
//	位置の設定処理
//============================================================
void CLiquid::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 液体の位置を設定
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // 液体の最大数分繰り返す

		// 引数の位置を設定
		m_apLiquid[nCntLiquid]->SetVec3Position(rPos);
	}
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CLiquid::GetVec3Position(void) const
{
	// 下の液体の位置を返す
	return m_apLiquid[LIQUID_LOW]->GetVec3Position();
}

//============================================================
//	向きの設定処理
//============================================================
void CLiquid::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 液体の向きを設定
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // 液体の最大数分繰り返す

		// 引数の向きを設定
		m_apLiquid[nCntLiquid]->SetVec3Rotation(rRot);
	}
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CLiquid::GetVec3Rotation(void) const
{
	// 下の液体の向きを返す
	return m_apLiquid[LIQUID_LOW]->GetVec3Rotation();
}

//============================================================
//	大きさの設定処理
//============================================================
void CLiquid::SetVec2Sizing(const D3DXVECTOR2& rSize)
{
	// 液体の大きさを設定
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // 液体の最大数分繰り返す

		// 引数の大きさを設定
		m_apLiquid[nCntLiquid]->SetVec2Sizing(rSize);
	}
}

//============================================================
//	大きさ取得処理
//============================================================
D3DXVECTOR2 CLiquid::GetVec2Sizing(void) const
{
	// 下の液体の大きさを返す
	return m_apLiquid[LIQUID_LOW]->GetVec2Sizing();
}

//============================================================
//	色の設定処理
//============================================================
void CLiquid::SetColor(const D3DXCOLOR& rCol)
{
	// 液体の色を設定
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // 液体の最大数分繰り返す

		// 引数の色を設定
		m_apLiquid[nCntLiquid]->SetColor(rCol);
	}
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CLiquid::GetColor(void) const
{
	// 下の液体の色を返す
	return m_apLiquid[LIQUID_LOW]->GetColor();
}

//============================================================
//	種類の設定処理
//============================================================
void CLiquid::SetType(const int nType)
{
	if (nType > NONE_IDX && nType < TYPE_MAX)
	{ // 種類が範囲内の場合

		// 引数の種類を設定
		m_type = (EType)nType;

		for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
		{ // 液体の最大数分繰り返す

			// テクスチャを登録・割当
			m_apLiquid[nCntLiquid]->BindTexture(mc_apTextureFile[m_type][nCntLiquid]);
		}
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	種類取得処理
//============================================================
int CLiquid::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//============================================================
//	波のメッシュフィールド取得処理
//============================================================
CScrollMeshField *CLiquid::GetScrollMeshField(int num) const
{
	// 波のメッシュフィールドを返す
	return m_apLiquid[num];
}

//============================================================
//	生成処理
//============================================================
CLiquid *CLiquid::Create
(
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR2& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const STexMove& rTexMove,	// テクスチャ移動量
	const float fMaxUp,			// 波の最高上昇量
	const float fAddSinRot,		// 波打ち向き加算量
	const float fAddVtxRot		// 隣波の向き加算量
)
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

		// 種類を設定
		pLiquid->SetType(type);

		// 位置を設定
		pLiquid->SetVec3Position(rPos);

		// 向きを設定
		pLiquid->SetVec3Rotation(rRot);

		// 大きさを設定
		pLiquid->SetVec2Sizing(rSize);

		// 色を設定
		pLiquid->SetColor(rCol);

		// テクスチャ移動量を設定
		pLiquid->SetTexMove(rTexMove);

		// 波の最高上昇量を設定
		pLiquid->SetMaxUp(fMaxUp);

		// 波打ち向き加算量を設定
		pLiquid->SetAddSinRot(fAddSinRot);

		// 隣波の向き加算量を設定
		pLiquid->SetAddVtxRot(fAddVtxRot);

		// 分割数を設定
		if (FAILED(pLiquid->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// メモリ開放
			delete pLiquid;
			pLiquid = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pLiquid;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	分割数の設定処理
//============================================================
HRESULT CLiquid::SetPattern(const POSGRID2& rPart)
{
	// 液体の分割数を設定
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // 液体の最大数分繰り返す

		// 引数の分割数を設定
		if (FAILED(m_apLiquid[nCntLiquid]->SetPattern(rPart)))
		{ // 設定に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	分割数取得処理
//============================================================
POSGRID2 CLiquid::GetPattern(void) const
{
	// 下の液体の分割数を返す
	return m_apLiquid[LIQUID_LOW]->GetPattern();
}

//============================================================
//	テクスチャ移動量の設定処理
//============================================================
void CLiquid::SetTexMove(const STexMove texMove)
{
	// 液体のテクスチャ移動量を設定
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // 液体の最大数分繰り返す

		// 引数のテクスチャ移動量を設定
		m_apLiquid[nCntLiquid]->SetMoveU(texMove.aTexMove[nCntLiquid].x);
		m_apLiquid[nCntLiquid]->SetMoveV(texMove.aTexMove[nCntLiquid].y);
	}
}

//============================================================
//	テクスチャ移動量取得処理
//============================================================
CLiquid::STexMove CLiquid::GetTexMove(const ELiquid liquid) const
{
	// 変数を宣言
	STexMove texMove;	// テクスチャ移動量

	// 下液体のテクスチャ移動量を設定
	texMove.texMoveLow.x = m_apLiquid[LIQUID_LOW]->GetMoveU();
	texMove.texMoveLow.y = m_apLiquid[LIQUID_LOW]->GetMoveV();

	// 上液体のテクスチャ移動量を設定
	texMove.texMoveHigh.x = m_apLiquid[LIQUID_HIGH]->GetMoveU();
	texMove.texMoveHigh.y = m_apLiquid[LIQUID_HIGH]->GetMoveV();

	// テクスチャ移動量を返す
	return texMove;
}

//============================================================
//	波の最高上昇量の設定処理
//============================================================
void CLiquid::SetMaxUp(const float fMaxUp)
{
	// 引数の波の最高上昇量を設定
	m_fMaxUp = fMaxUp;
}

//============================================================
//	波の最高上昇量の取得処理
//============================================================
float CLiquid::GetMaxUp(void) const
{
	// 波の最高上昇量を返す
	return m_fMaxUp;
}

//============================================================
//	波打ち向き加算量の設定処理
//============================================================
void CLiquid::SetAddSinRot(const float fAddSinRot)
{
	// 引数の波打ち向き加算量を設定
	m_fAddSinRot = fAddSinRot;
}

//============================================================
//	波打ち向き加算量の取得処理
//============================================================
float CLiquid::GetAddSinRot(void) const
{
	// 波打ち向き加算量を返す
	return m_fAddSinRot;
}

//============================================================
//	隣波の向き加算量の設定処理
//============================================================
void CLiquid::SetAddVtxRot(const float fAddVtxRot)
{
	// 引数の隣波の向き加算量を設定
	m_fAddVtxRot = fAddVtxRot;
}

//============================================================
//	隣波の向き加算量の取得処理
//============================================================
float CLiquid::GetAddVtxRot(void) const
{
	// 隣波の向き加算量を返す
	return m_fAddVtxRot;
}

//============================================================
//	破棄処理
//============================================================
void CLiquid::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}
