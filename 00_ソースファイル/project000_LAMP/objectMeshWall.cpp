//============================================================
//
//	オブジェクトメッシュウォール処理 [objectMeshWall.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectMeshWall.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	子クラス [CObjectMeshWall] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectMeshWall::CObjectMeshWall()
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;		// 頂点バッファ
	m_pIdxBuff = NULL;		// インデックスバッファ
	m_part = GRID2_ZERO;	// 分割数
	m_nNumVtx = 0;			// 必要頂点数
	m_nNumIdx = 0;			// 必要インデックス数
	m_nTextureID = 0;		// テクスチャインデックス
	memset(&m_meshWall, 0, sizeof(m_meshWall));	// メッシュウォールの情報
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObjectMeshWall::CObjectMeshWall(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;		// 頂点バッファ
	m_pIdxBuff = NULL;		// インデックスバッファ
	m_part = GRID2_ZERO;	// 分割数
	m_nNumVtx = 0;			// 必要頂点数
	m_nNumIdx = 0;			// 必要インデックス数
	m_nTextureID = 0;		// テクスチャインデックス
	memset(&m_meshWall, 0, sizeof(m_meshWall));	// メッシュウォールの情報
}

//============================================================
//	デストラクタ
//============================================================
CObjectMeshWall::~CObjectMeshWall()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectMeshWall::Init(void)
{
	// メンバ変数を初期化
	m_pVtxBuff = NULL;		// 頂点バッファ
	m_pIdxBuff = NULL;		// インデックスバッファ
	m_part = GRID2_ZERO;	// 分割数
	m_nNumVtx = 0;			// 必要頂点数
	m_nNumIdx = 0;			// 必要インデックス数
	m_nTextureID = -1;		// テクスチャインデックス

	m_meshWall.pos = VEC3_ZERO;		// 位置
	m_meshWall.rot = VEC3_ZERO;		// 向き
	m_meshWall.size = VEC2_ZERO;	// 大きさ
	m_meshWall.col = XCOL_WHITE;	// 色
	m_meshWall.cull = D3DCULL_CCW;	// カリング状況
	m_meshWall.bLight = true;		// ライティング状況

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
void CObjectMeshWall::Uninit(void)
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

	// オブジェクトメッシュウォールを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectMeshWall::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CObjectMeshWall::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// テクスチャへのポインタ

	// ポリゴンの裏面のみを表示状態にする
	pDevice->SetRenderState(D3DRS_CULLMODE, m_meshWall.cull);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, m_meshWall.bLight);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_meshWall.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshWall.rot.y, m_meshWall.rot.x, m_meshWall.rot.z);
	D3DXMatrixMultiply(&m_meshWall.mtxWorld, &m_meshWall.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_meshWall.pos.x, m_meshWall.pos.y, m_meshWall.pos.z);
	D3DXMatrixMultiply(&m_meshWall.mtxWorld, &m_meshWall.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_meshWall.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive
	( // 引数
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,
		0,
		m_nNumVtx,		// 使用する頂点数
		0,				// インデックスバッファの開始地点
		m_nNumIdx - 2	// プリミティブ (ポリゴン) 数
	);

	// ポリゴンの表面のみを表示状態にする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//============================================================
//	テクスチャ割当処理 (インデックス)
//============================================================
void CObjectMeshWall::BindTexture(const int nTextureID)
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
void CObjectMeshWall::BindTexture(const char *pTexturePass)
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
void CObjectMeshWall::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_meshWall.pos = rPos;
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObjectMeshWall::GetVec3Position(void) const
{
	// 位置を返す
	return m_meshWall.pos;
}

//============================================================
//	向きの設定処理
//============================================================
void CObjectMeshWall::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 引数の向きを設定
	m_meshWall.rot = rRot;

	// 向きの正規化
	useful::Vec3NormalizeRot(m_meshWall.rot);
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CObjectMeshWall::GetVec3Rotation(void) const
{
	// 向きを返す
	return m_meshWall.rot;
}

//============================================================
//	大きさの設定処理
//============================================================
void CObjectMeshWall::SetVec2Sizing(const D3DXVECTOR2& rSize)
{
	// 引数の大きさを設定
	m_meshWall.size = rSize;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	大きさ取得処理
//============================================================
D3DXVECTOR2 CObjectMeshWall::GetVec2Sizing(void) const
{
	// 大きさを返す
	return m_meshWall.size;
}

//============================================================
//	色の設定処理
//============================================================
void CObjectMeshWall::SetColor(const D3DXCOLOR& rCol)
{
	// 引数の色を設定
	m_meshWall.col = rCol;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CObjectMeshWall::GetColor(void) const
{
	// 色を返す
	return m_meshWall.col;
}

//============================================================
//	生成処理
//============================================================
CObjectMeshWall *CObjectMeshWall::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR2& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const D3DCULL cull,			// カリング状況
	const bool bLight			// ライティング状況
)
{
	// ポインタを宣言
	CObjectMeshWall *pObjectMeshWall = NULL;		// オブジェクトメッシュウォール生成用

	if (pObjectMeshWall == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pObjectMeshWall = new CObjectMeshWall;	// オブジェクトメッシュウォール
	}
	else { assert(false); return NULL; }	// 使用中

	if (pObjectMeshWall != NULL)
	{ // 確保に成功している場合

		// オブジェクトメッシュウォールの初期化
		if (FAILED(pObjectMeshWall->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObjectMeshWall;
			pObjectMeshWall = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pObjectMeshWall->SetVec3Position(rPos);

		// 向きを設定
		pObjectMeshWall->SetVec3Rotation(rRot);

		// 大きさを設定
		pObjectMeshWall->SetVec2Sizing(rSize);

		// 色を設定
		pObjectMeshWall->SetColor(rCol);

		// カリングを設定
		pObjectMeshWall->SetCulling(cull);

		// ライティングを設定
		pObjectMeshWall->SetLighting(bLight);

		// 分割数を設定
		if (FAILED(pObjectMeshWall->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// メモリ開放
			delete pObjectMeshWall;
			pObjectMeshWall = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pObjectMeshWall;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	カリング設定処理
//============================================================
void CObjectMeshWall::SetCulling(const D3DCULL cull)
{
	// 引数のカリング状況を設定
	m_meshWall.cull = cull;
}

//============================================================
//	カリング取得処理
//============================================================
D3DCULL CObjectMeshWall::GetCulling(void) const
{
	// カリング状況を返す
	return m_meshWall.cull;
}

//============================================================
//	ライティング設定処理
//============================================================
void CObjectMeshWall::SetLighting(const bool bLight)
{
	// 引数のライティング状況を設定
	m_meshWall.bLight = bLight;
}

//============================================================
//	ライティング取得処理
//============================================================
bool CObjectMeshWall::GetLighting(void) const
{
	// ライティング状況を返す
	return m_meshWall.bLight;
}

//============================================================
//	分割数の設定処理
//============================================================
HRESULT CObjectMeshWall::SetPattern(const POSGRID2& rPart)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 引数の分割数を設定
	m_part = rPart;

	// 必要頂点・インデックス数を求める
	m_nNumVtx = (m_part.x + 1) * (m_part.y + 1); // 必要頂点数
	m_nNumIdx = (m_part.x + 1) * (((m_part.y + 1) * 2) - 2) + (m_part.y * 2) - 2; // 必要インデックス数

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
POSGRID2 CObjectMeshWall::GetPattern(void) const
{
	// 分割数を返す
	return m_part;
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObjectMeshWall::SetVtx(void)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // 縦の分割数 +1回繰り返す

			for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // 横の分割数 +1回繰り返す

				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3
				( // 引数
					nCntWidth * (m_meshWall.size.x / (float)m_part.x) - (m_meshWall.size.x * 0.5f),	// x
					-(nCntHeight * (m_meshWall.size.y / (float)m_part.y)) + m_meshWall.size.y,		// y
					0.0f																			// z
				);

				// 法線ベクトルの設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// 頂点カラーの設定
				pVtx[0].col = m_meshWall.col;

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntWidth, 1.0f * nCntHeight);

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
void CObjectMeshWall::SetIdx(void)
{
	// ポインタを宣言
	WORD *pIdx;	// インデックス情報へのポインタ

	if (m_pIdxBuff != NULL)
	{ // 使用中の場合

		// インデックスバッファをロックし、頂点番号データへのポインタを取得
		m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < m_part.y; nCntHeight++)
		{ // 縦の分割数 +1回繰り返す

			for (nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // 横の分割数 +1回繰り返す

				pIdx[0] = (WORD)((m_part.x + 1) * (nCntHeight + 1) + nCntWidth);
				pIdx[1] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth);

				// インデックスデータのポインタを 2つ分進める
				pIdx += 2;
			}

			if (nCntHeight != m_part.y - 1)
			{ // 一番手前の分割場所ではない場合

				pIdx[0] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth - 1);
				pIdx[1] = (WORD)((m_part.x + 1) * (nCntHeight + 2));

				// インデックスデータのポインタを 2つ分進める
				pIdx += 2;
			}
		}

		// インデックスバッファをアンロックする
		m_pIdxBuff->Unlock();
	}
}

//============================================================
//	スクロールのテクスチャ座標の設定処理
//============================================================
void CObjectMeshWall::SetScrollTex(const float fTexU, const float fTexV)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // 縦の分割数 +1回繰り返す

			for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // 横の分割数 +1回繰り返す

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((float)nCntWidth + fTexU, (float)nCntHeight + fTexV);

				// 頂点データのポインタを 1つ分進める
				pVtx += 1;
			}
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}
