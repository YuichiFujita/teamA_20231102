//============================================================
//
//	オブジェクトメッシュドーム処理 [objectMeshDome.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectMeshDome.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	子クラス [CObjectMeshDome] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectMeshDome::CObjectMeshDome()
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;		// 頂点バッファ
	m_pIdxBuff = NULL;		// インデックスバッファ
	m_part = GRID2_ZERO;	// 分割数
	m_texPart = GRID2_ZERO;	// テクスチャ分割数
	m_nNumVtx = 0;			// 必要頂点数
	m_nNumIdx = 0;			// 必要インデックス数
	m_nTextureID = 0;		// テクスチャインデックス
	memset(&m_meshDome, 0, sizeof(m_meshDome));	// メッシュドームの情報
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObjectMeshDome::CObjectMeshDome(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;		// 頂点バッファ
	m_pIdxBuff = NULL;		// インデックスバッファ
	m_part = GRID2_ZERO;	// 分割数
	m_texPart = GRID2_ZERO;	// テクスチャ分割数
	m_nNumVtx = 0;			// 必要頂点数
	m_nNumIdx = 0;			// 必要インデックス数
	m_nTextureID = 0;		// テクスチャインデックス
	memset(&m_meshDome, 0, sizeof(m_meshDome));	// メッシュドームの情報
}

//============================================================
//	デストラクタ
//============================================================
CObjectMeshDome::~CObjectMeshDome()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectMeshDome::Init(void)
{
	// メンバ変数を初期化
	m_pVtxBuff = NULL;		// 頂点バッファ
	m_pIdxBuff = NULL;		// インデックスバッファ
	m_part = GRID2_ZERO;	// 分割数
	m_texPart = GRID2_ONE;	// テクスチャ分割数
	m_nNumVtx = 0;			// 必要頂点数
	m_nNumIdx = 0;			// 必要インデックス数
	m_nTextureID = -1;		// テクスチャインデックス

	m_meshDome.pos = VEC3_ZERO;		// 位置
	m_meshDome.rot = VEC3_ZERO;		// 向き
	m_meshDome.col = XCOL_WHITE;	// 色
	m_meshDome.fRadius = 0.0f;		// 半径
	m_meshDome.cull = D3DCULL_CCW;	// カリング状況
	m_meshDome.bLight = true;		// ライティング状況

	// 分割数を設定
	if (FAILED(SetPattern(GRID2_ONE)))
	{ // 分割数の設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectMeshDome::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{ // 頂点バッファが使用中の場合

		// メモリ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{ // インデックスバッファが使用中の場合

		// メモリ開放
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// オブジェクトメッシュドームを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectMeshDome::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CObjectMeshDome::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// テクスチャへのポインタ

	// ポリゴンの裏面のみを表示状態にする
	pDevice->SetRenderState(D3DRS_CULLMODE, m_meshDome.cull);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, m_meshDome.bLight);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_meshDome.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshDome.rot.y, m_meshDome.rot.x, m_meshDome.rot.z);
	D3DXMatrixMultiply(&m_meshDome.mtxWorld, &m_meshDome.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_meshDome.pos.x, m_meshDome.pos.y, m_meshDome.pos.z);
	D3DXMatrixMultiply(&m_meshDome.mtxWorld, &m_meshDome.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_meshDome.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// 半球ポリゴンの描画
	pDevice->DrawIndexedPrimitive
	( // 引数
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,
		0,
		m_nNumVtx - 1,	// 使用する頂点数
		0,				// インデックスバッファの開始地点
		(m_nNumIdx - (m_part.x + 1)) - 3	// プリミティブ (ポリゴン) 数
	);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// 上蓋ポリゴンの描画
	pDevice->DrawIndexedPrimitive
	( // 引数
		D3DPT_TRIANGLEFAN,	// プリミティブの種類
		0,
		0,
		m_part.x + 1,				// 使用する頂点数
		m_nNumIdx - (m_part.x + 2),	// インデックスバッファの開始地点
		m_part.x					// プリミティブ (ポリゴン) 数
	);

	// ポリゴンの表面のみを表示状態にする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//============================================================
//	テクスチャ割当処理 (インデックス)
//============================================================
void CObjectMeshDome::BindTexture(const int nTextureID)
{
	if (nTextureID > NONE_IDX)
	{ // テクスチャインデックスが使用可能な場合

		// テクスチャインデックスを代入
		m_nTextureID = nTextureID;
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	テクスチャ割当処理 (パス)
//============================================================
void CObjectMeshDome::BindTexture(const char *pTexturePass)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	if (pTexture == NULL)
	{ // テクスチャポインタが存在しない場合

		// 関数を抜ける
		assert(false);
		return;
	}

	if (pTexturePass != NULL)
	{ // 割り当てるテクスチャパスが存在する場合

		// テクスチャインデックスを代入
		m_nTextureID = pTexture->Regist(pTexturePass);
	}
	else { assert(false); }	// テクスチャパス無し
}

//============================================================
//	位置の設定処理
//============================================================
void CObjectMeshDome::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_meshDome.pos = rPos;
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObjectMeshDome::GetVec3Position(void) const
{
	// 位置を返す
	return m_meshDome.pos;
}

//============================================================
//	向きの設定処理
//============================================================
void CObjectMeshDome::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 引数の向きを設定
	m_meshDome.rot = rRot;

	// 向きの正規化
	useful::NormalizeRot(m_meshDome.rot.z);
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CObjectMeshDome::GetVec3Rotation(void) const
{
	// 向きを返す
	return m_meshDome.rot;
}

//============================================================
//	色の設定処理
//============================================================
void CObjectMeshDome::SetColor(const D3DXCOLOR& rCol)
{
	// 引数の色を設定
	m_meshDome.col = rCol;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CObjectMeshDome::GetColor(void) const
{
	// 色を返す
	return m_meshDome.col;
}

//============================================================
//	半径の設定処理
//============================================================
void CObjectMeshDome::SetRadius(const float fRadius)
{
	// 引数の半径を設定
	m_meshDome.fRadius = fRadius;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	半径取得処理
//============================================================
float CObjectMeshDome::GetRadius(void) const
{
	// 半径を返す
	return m_meshDome.fRadius;
}

//============================================================
//	生成処理
//============================================================
CObjectMeshDome *CObjectMeshDome::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const float fRadius,		// 半径
	const D3DCULL cull,			// カリング状況
	const bool bLight			// ライティング状況
)
{
	// ポインタを宣言
	CObjectMeshDome *pObjectMeshDome = NULL;	// オブジェクトメッシュドーム生成用

	if (pObjectMeshDome == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pObjectMeshDome = new CObjectMeshDome;	// オブジェクトメッシュドーム
	}
	else { assert(false); return NULL; }	// 使用中

	if (pObjectMeshDome != NULL)
	{ // 確保に成功している場合

		// オブジェクトメッシュドームの初期化
		if (FAILED(pObjectMeshDome->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObjectMeshDome;
			pObjectMeshDome = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pObjectMeshDome->SetVec3Position(rPos);

		// 向きを設定
		pObjectMeshDome->SetVec3Rotation(rRot);

		// 色を設定
		pObjectMeshDome->SetColor(rCol);

		// 半径を設定
		pObjectMeshDome->SetRadius(fRadius);

		// カリングを設定
		pObjectMeshDome->SetCulling(cull);

		// ライティングを設定
		pObjectMeshDome->SetLighting(bLight);

		// 分割数を設定
		if (FAILED(pObjectMeshDome->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// メモリ開放
			delete pObjectMeshDome;
			pObjectMeshDome = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pObjectMeshDome;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	カリング設定処理
//============================================================
void CObjectMeshDome::SetCulling(const D3DCULL cull)
{
	// 引数のカリング状況を設定
	m_meshDome.cull = cull;
}

//============================================================
//	カリング取得処理
//============================================================
D3DCULL CObjectMeshDome::GetCulling(void) const
{
	// カリング状況を返す
	return m_meshDome.cull;
}

//============================================================
//	ライティング設定処理
//============================================================
void CObjectMeshDome::SetLighting(const bool bLight)
{
	// 引数のライティング状況を設定
	m_meshDome.bLight = bLight;
}

//============================================================
//	ライティング取得処理
//============================================================
bool CObjectMeshDome::GetLighting(void) const
{
	// ライティング状況を返す
	return m_meshDome.bLight;
}

//============================================================
//	分割数の設定処理
//============================================================
HRESULT CObjectMeshDome::SetPattern(const POSGRID2& rPart)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 引数の分割数を設定
	m_part = rPart;

	// 必要頂点・インデックス数を求める
	m_nNumVtx = ((m_part.x + 1) * (m_part.y + 1)) - m_part.x; // 必要頂点数
	m_nNumIdx = ((m_part.x + 1) * (((m_part.y + 1) * 2) - 2) + (m_part.y * 2) - 2) - (m_part.x + 2); // 必要インデックス数

	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{ // 頂点バッファが使用中の場合

		// メモリ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 頂点バッファの情報を設定
	if (m_pVtxBuff == NULL)
	{ // 非使用中の場合

		// 頂点バッファの生成
		if (FAILED(pDevice->CreateVertexBuffer
		( // 引数
			sizeof(VERTEX_3D) * m_nNumVtx,	// 必要頂点数
			D3DUSAGE_WRITEONLY,	// 使用方法
			FVF_VERTEX_3D,		// 頂点フォーマット
			D3DPOOL_MANAGED,	// メモリの指定
			&m_pVtxBuff,		// 頂点バッファへのポインタ
			NULL
		)))
		{ // 頂点バッファの生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// 使用中

	// インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{ // インデックスバッファが使用中の場合

		// メモリ開放
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// インデックスバッファの情報を設定
	if (m_pIdxBuff == NULL)
	{ // 非使用中の場合

		// インデックスバッファの生成
		if (FAILED(pDevice->CreateIndexBuffer
		( // 引数
			sizeof(WORD) * m_nNumIdx,	// 必要インデックス数
			D3DUSAGE_WRITEONLY,	// 使用方法
			D3DFMT_INDEX16,		// インデックスバッファのフォーマット
			D3DPOOL_MANAGED,	// メモリの指定
			&m_pIdxBuff,		// インデックスバッファへのポインタ
			NULL
		)))
		{ // インデックスバッファの生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// 使用中

	// 頂点・インデックス情報の設定
	SetVtx();
	SetIdx();

	// 成功を返す
	return S_OK;
}

//============================================================
//	分割数取得処理
//============================================================
POSGRID2 CObjectMeshDome::GetPattern(void) const
{
	// 分割数を返す
	return m_part;
}

//============================================================
//	テクスチャ分割数の設定処理
//============================================================
void CObjectMeshDome::SetTexPattern(const POSGRID2& rTexPart)
{
	// 引数のテクスチャ分割数を設定
	m_texPart = rTexPart;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	テクスチャ分割数取得処理
//============================================================
POSGRID2 CObjectMeshDome::GetTexPattern(void) const
{
	// テクスチャ分割数を返す
	return m_texPart;
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObjectMeshDome::SetVtx(void)
{
	// 変数を宣言
	D3DXVECTOR3 vecPos;					// 頂点位置の計算用
	D3DXVECTOR3 vecNor;					// 法線ベクトルの計算用
	float fRotWidth, fRotHeight;		// 頂点位置の方向の計算用
	float fMaxTexU = m_part.x * 1.0f;	// テクスチャ座標の u最大値
	float fMaxTexV = m_part.y * 1.0f;	// テクスチャ座標の v最大値

	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // 縦の分割数 +1回繰り返す

			for (int nCntWidth = 0; nCntWidth < (m_part.x * (1 - (nCntHeight / m_part.y))) + 1; nCntWidth++)
			{ // 横の分割数 +1回繰り返す (天辺の頂点時は繰り返しを 1回にする)

				// 頂点の向きを計算
				fRotWidth  = nCntWidth  * ((D3DX_PI * 2.0f) / m_part.x);
				fRotHeight = (m_part.y - nCntHeight) * ((D3DX_PI * 0.5f) / m_part.y);

				// 頂点座標の方向を設定
				vecPos = D3DXVECTOR3
				( // 引数
					m_meshDome.fRadius * sinf(fRotHeight) * sinf(fRotWidth),	// x
					m_meshDome.fRadius * cosf(fRotHeight),						// y
					m_meshDome.fRadius * sinf(fRotHeight) * cosf(fRotWidth)		// z
				);

				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f) + vecPos;

				// 法線ベクトルの方向を設定
				vecNor = vecPos;

				// ベクトルを正規化
				D3DXVec3Normalize(&vecNor, &vecNor);

				// 法線ベクトルの設定
				pVtx[0].nor = vecNor;

				// 頂点カラーの設定
				pVtx[0].col = m_meshDome.col;

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2
				( // 引数
					fMaxTexU - (nCntWidth  * 1.0f / m_texPart.x),	// u
					fMaxTexV - (nCntHeight * 1.0f / m_texPart.y)	// v
				);

				// 頂点データのポインタを 1つ分進める
				pVtx += 1;
			}
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	インデックス情報の設定処理
//============================================================
void CObjectMeshDome::SetIdx(void)
{
	// ポインタを宣言
	WORD *pIdx;	// インデックス情報へのポインタ

	if (m_pIdxBuff != NULL)
	{ // 使用中の場合

		// インデックスバッファをロックし、頂点番号データへのポインタを取得
		m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < m_part.y - 1; nCntHeight++)
		{ // 縦の分割数 +1回繰り返す

			for (nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // 横の分割数 +1回繰り返す

				pIdx[0] = (WORD)((m_part.x + 1) * (nCntHeight + 1) + nCntWidth);
				pIdx[1] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth);

				// インデックスデータのポインタを 2つ分進める
				pIdx += 2;
			}

			if (nCntHeight != m_part.y - 2)
			{ // 一番手前の分割場所ではない場合

				pIdx[0] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth - 1);
				pIdx[1] = (WORD)((m_part.x + 1) * (nCntHeight + 2));

				// インデックスデータのポインタを 2つ分進める
				pIdx += 2;
			}
		}

		// 半球の天辺の頂点を指定
		pIdx[0] = (WORD)(m_nNumVtx - 1);

		// インデックスデータのポインタを 1つ分進める
		pIdx += 1;

		for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
		{ // 横の分割数 +1回繰り返す

			pIdx[0] = (WORD)(((m_part.x + 1) * (m_part.y - 1)) + nCntWidth);

			// インデックスデータのポインタを 1つ分進める
			pIdx += 1;
		}

		// インデックスバッファをアンロックする
		m_pIdxBuff->Unlock();
	}
}

//============================================================
//	スクロールのテクスチャ座標の設定処理
//============================================================
void CObjectMeshDome::SetScrollTex(const float fTexU, const float fTexV)
{
	// 変数を宣言
	float fMaxTexU = m_part.x * 1.0f;	// テクスチャ座標の u最大値
	float fMaxTexV = m_part.y * 1.0f;	// テクスチャ座標の v最大値

	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // 縦の分割数 +1回繰り返す

			for (int nCntWidth = 0; nCntWidth < (m_part.x * (1 - (nCntHeight / m_part.y))) + 1; nCntWidth++)
			{ // 横の分割数 +1回繰り返す (天辺の頂点時は繰り返しを 1回にする)

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2
				( // 引数
					fMaxTexU - fTexU - (nCntWidth  * 1.0f / m_texPart.x),	// u
					fMaxTexV - fTexV - (nCntHeight * 1.0f / m_texPart.y)	// v
				);

				// 頂点データのポインタを 1つ分進める
				pVtx += 1;
			}
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

// TODO：テクスチャ座標動かすやつ検証
