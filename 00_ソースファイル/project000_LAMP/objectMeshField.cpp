//============================================================
//
//	オブジェクトメッシュフィールド処理 [objectMeshField.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectMeshField.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//************************************************************
//	マクロ定義
//************************************************************
#define NUM_VTX_TRIANGLE	(3)	// 三角形ポリゴンの頂点数
#define COLL_RADIUS	(100.0f)	// 当たり判定を行う距離

//************************************************************
//	子クラス [CObjectMeshField] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectMeshField::CObjectMeshField()
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;		// 頂点バッファ
	m_pIdxBuff = NULL;		// インデックスバッファ
	m_pPosGapBuff = NULL;	// 座標のずれバッファ
	m_pNorBuff = NULL;		// 法線バッファ
	m_pNumNorBuff = NULL;	// 法線の使用数バッファ
	m_part = GRID2_ZERO;	// 分割数
	m_nNumVtx = 0;			// 必要頂点数
	m_nNumIdx = 0;			// 必要インデックス数
	m_nTextureID = 0;		// テクスチャインデックス
	memset(&m_meshField, 0, sizeof(m_meshField));	// メッシュフィールドの情報
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObjectMeshField::CObjectMeshField(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;		// 頂点バッファ
	m_pIdxBuff = NULL;		// インデックスバッファ
	m_pPosGapBuff = NULL;	// 座標のずれバッファ
	m_pNorBuff = NULL;		// 法線バッファ
	m_pNumNorBuff = NULL;	// 法線の使用数バッファ
	m_part = GRID2_ZERO;	// 分割数
	m_nNumVtx = 0;			// 必要頂点数
	m_nNumIdx = 0;			// 必要インデックス数
	m_nTextureID = 0;		// テクスチャインデックス
	memset(&m_meshField, 0, sizeof(m_meshField));	// メッシュフィールドの情報
}

//============================================================
//	デストラクタ
//============================================================
CObjectMeshField::~CObjectMeshField()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectMeshField::Init(void)
{
	// メンバ変数を初期化
	m_pVtxBuff = NULL;			// 頂点バッファ
	m_pIdxBuff = NULL;			// インデックスバッファ
	m_pPosGapBuff = NULL;		// 座標のずれバッファ
	m_pNorBuff = NULL;			// 法線バッファ
	m_pNumNorBuff = NULL;		// 法線の使用数バッファ
	m_part = GRID2_ZERO;		// 分割数
	m_nNumVtx = 0;				// 必要頂点数
	m_nNumIdx = 0;				// 必要インデックス数
	m_nTextureID = NONE_IDX;	// テクスチャインデックス

	m_meshField.pos = VEC3_ZERO;	// 位置
	m_meshField.rot = VEC3_ZERO;	// 向き
	m_meshField.size = VEC2_ZERO;	// 大きさ
	m_meshField.col = XCOL_WHITE;	// 色
	m_meshField.cull = D3DCULL_CCW;	// カリング状況
	m_meshField.bLight = true;		// ライティング状況

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
void CObjectMeshField::Uninit(void)
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

	// 座標のずれバッファの破棄
	if (m_pPosGapBuff != NULL)
	{ // 座標のずれバッファが使用中の場合

		// メモリ開放
		delete[] m_pPosGapBuff;
		m_pPosGapBuff = NULL;
	}

	// 法線バッファの破棄
	if (m_pNorBuff != NULL)
	{ // 法線バッファが使用中の場合

		// メモリ開放
		delete[] m_pNorBuff;
		m_pNorBuff = NULL;
	}

	// 法線の使用数バッファの破棄
	if (m_pNumNorBuff != NULL)
	{ // インデックスバッファが使用中の場合

		// メモリ開放
		delete[] m_pNumNorBuff;
		m_pNumNorBuff = NULL;
	}

	// オブジェクトメッシュフィールドを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectMeshField::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CObjectMeshField::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// テクスチャへのポインタ

	// ポリゴンの裏面のみを表示状態にする
	pDevice->SetRenderState(D3DRS_CULLMODE, m_meshField.cull);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, m_meshField.bLight);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_meshField.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshField.rot.y, m_meshField.rot.x, m_meshField.rot.z);
	D3DXMatrixMultiply(&m_meshField.mtxWorld, &m_meshField.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_meshField.pos.x, m_meshField.pos.y, m_meshField.pos.z);
	D3DXMatrixMultiply(&m_meshField.mtxWorld, &m_meshField.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_meshField.mtxWorld);

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
void CObjectMeshField::BindTexture(const int nTextureID)
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
void CObjectMeshField::BindTexture(const char *pTexturePass)
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
void CObjectMeshField::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_meshField.pos = rPos;
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObjectMeshField::GetVec3Position(void) const
{
	// 位置を返す
	return m_meshField.pos;
}

//============================================================
//	向きの設定処理
//============================================================
void CObjectMeshField::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 引数の向きを設定
	m_meshField.rot = rRot;

	// 向きの正規化
	useful::Vec3NormalizeRot(m_meshField.rot);
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CObjectMeshField::GetVec3Rotation(void) const
{
	// 向きを返す
	return m_meshField.rot;
}

//============================================================
//	大きさの設定処理
//============================================================
void CObjectMeshField::SetVec2Sizing(const D3DXVECTOR2& rSize)
{
	// 引数の大きさを設定
	m_meshField.size = rSize;

	// 頂点情報の設定
	SetVtx(false);
}

//============================================================
//	大きさ取得処理
//============================================================
D3DXVECTOR2 CObjectMeshField::GetVec2Sizing(void) const
{
	// 大きさを返す
	return m_meshField.size;
}

//============================================================
//	色の設定処理
//============================================================
void CObjectMeshField::SetColor(const D3DXCOLOR& rCol)
{
	// 引数の色を設定
	m_meshField.col = rCol;

	// 頂点情報の設定
	SetVtx(false);
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CObjectMeshField::GetColor(void) const
{
	// 色を返す
	return m_meshField.col;
}

//============================================================
//	生成処理
//============================================================
CObjectMeshField *CObjectMeshField::Create
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
	CObjectMeshField *pObjectMeshField = NULL;		// オブジェクトメッシュフィールド生成用

	if (pObjectMeshField == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pObjectMeshField = new CObjectMeshField;	// オブジェクトメッシュフィールド
	}
	else { assert(false); return NULL; }	// 使用中

	if (pObjectMeshField != NULL)
	{ // 確保に成功している場合

		// オブジェクトメッシュフィールドの初期化
		if (FAILED(pObjectMeshField->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObjectMeshField;
			pObjectMeshField = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pObjectMeshField->SetVec3Position(rPos);

		// 向きを設定
		pObjectMeshField->SetVec3Rotation(rRot);

		// 大きさを設定
		pObjectMeshField->SetVec2Sizing(rSize);

		// 色を設定
		pObjectMeshField->SetColor(rCol);

		// カリングを設定
		pObjectMeshField->SetCulling(cull);

		// ライティングを設定
		pObjectMeshField->SetLighting(bLight);

		// 分割数を設定
		if (FAILED(pObjectMeshField->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// メモリ開放
			delete pObjectMeshField;
			pObjectMeshField = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pObjectMeshField;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	カリング設定処理
//============================================================
void CObjectMeshField::SetCulling(const D3DCULL cull)
{
	// 引数のカリング状況を設定
	m_meshField.cull = cull;
}

//============================================================
//	カリング取得処理
//============================================================
D3DCULL CObjectMeshField::GetCulling(void) const
{
	// カリング状況を返す
	return m_meshField.cull;
}

//============================================================
//	ライティング設定処理
//============================================================
void CObjectMeshField::SetLighting(const bool bLight)
{
	// 引数のライティング状況を設定
	m_meshField.bLight = bLight;
}

//============================================================
//	ライティング取得処理
//============================================================
bool CObjectMeshField::GetLighting(void) const
{
	// ライティング状況を返す
	return m_meshField.bLight;
}

//============================================================
//	分割数の設定処理
//============================================================
HRESULT CObjectMeshField::SetPattern(const POSGRID2& rPart)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	//--------------------------------------------------------
	//	変更情報を設定
	//--------------------------------------------------------
	// 引数の分割数を設定
	m_part = rPart;

	// 必要頂点・インデックス数を求める
	m_nNumVtx = (m_part.x + 1) * (m_part.y + 1); // 必要頂点数
	m_nNumIdx = (m_part.x + 1) * (((m_part.y + 1) * 2) - 2) + (m_part.y * 2) - 2; // 必要インデックス数

	//--------------------------------------------------------
	//	頂点バッファの破棄・生成
	//--------------------------------------------------------
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

	//--------------------------------------------------------
	//	インデックスバッファの破棄・生成
	//--------------------------------------------------------
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

	//--------------------------------------------------------
	//	座標のずれバッファの破棄・生成
	//--------------------------------------------------------
	// 座標のずれバッファの破棄
	if (m_pPosGapBuff != NULL)
	{ // 座標のずれバッファが使用中の場合

		// メモリ開放
		delete[] m_pPosGapBuff;
		m_pPosGapBuff = NULL;
	}

	// 座標のずれバッファの情報を設定
	if (m_pPosGapBuff == NULL)
	{ // 非使用中の場合

		// 座標のずれバッファのメモリ確保
		m_pPosGapBuff = new D3DXVECTOR3[m_nNumVtx];

		if (m_pPosGapBuff != NULL)
		{ // 確保に成功した場合

			// メモリクリア
			memset(m_pPosGapBuff, 0, sizeof(D3DXVECTOR3) * m_nNumVtx);
		}
		else { assert(false); return E_FAIL; }	// 確保失敗
	}
	else { assert(false); return E_FAIL; }	// 使用中

	//--------------------------------------------------------
	//	法線バッファの破棄・生成
	//--------------------------------------------------------
	// 法線バッファの破棄
	if (m_pNorBuff != NULL)
	{ // 法線バッファが使用中の場合

		// メモリ開放
		delete[] m_pNorBuff;
		m_pNorBuff = NULL;
	}

	// 法線バッファの情報を設定
	if (m_pNorBuff == NULL)
	{ // 非使用中の場合

		// 変数を宣言
		int nNorBuff = 6 + (6 * (m_part.x - 1)) + (6 * (m_part.y - 1)) + (6 * ((m_part.x - 1) * (m_part.y - 1)));	// 法線バッファの確保数

		// 法線バッファのメモリ確保
		m_pNorBuff = new D3DXVECTOR3[nNorBuff];

		if (m_pNorBuff != NULL)
		{ // 確保に成功した場合

			// メモリクリア
			memset(m_pNorBuff, 0, sizeof(D3DXVECTOR3) * nNorBuff);
		}
		else { assert(false); return E_FAIL; }	// 確保失敗
	}
	else { assert(false); return E_FAIL; }	// 使用中

	//--------------------------------------------------------
	//	法線の使用数バッファの破棄・生成
	//--------------------------------------------------------
	// 法線の使用数バッファの破棄
	if (m_pNumNorBuff != NULL)
	{ // 法線の使用数バッファが使用中の場合

		// メモリ開放
		delete[] m_pNumNorBuff;
		m_pNumNorBuff = NULL;
	}

	// 法線の使用数バッファの情報を設定
	if (m_pNumNorBuff == NULL)
	{ // 非使用中の場合

		// 法線の使用数バッファのメモリ確保
		m_pNumNorBuff = new int[m_nNumVtx];

		if (m_pNumNorBuff != NULL)
		{ // 確保に成功した場合

			// メモリクリア
			memset(m_pNumNorBuff, 0, sizeof(int) * m_nNumVtx);
		}
		else { assert(false); return E_FAIL; }	// 確保失敗
	}
	else { assert(false); return E_FAIL; }	// 使用中

	//--------------------------------------------------------
	//	分割数を反映
	//--------------------------------------------------------
	// 頂点・インデックス情報の設定
	SetVtx(true);
	SetIdx();

	// 成功を返す
	return S_OK;
}

//============================================================
//	分割数取得処理
//============================================================
POSGRID2 CObjectMeshField::GetPattern(void) const
{
	// 分割数を返す
	return m_part;
}

//============================================================
//	座標のずれの設定処理
//============================================================
void CObjectMeshField::SetGapPosition(const int nID, const D3DXVECTOR3& rPos)
{
	if (m_pPosGapBuff != NULL)
	{ // 使用中の場合

		if (nID < (m_part.x + 1) * (m_part.y + 1))
		{ // インデックスが使用可能な場合

			// 頂点のずれを設定
			m_pPosGapBuff[nID] = rPos;
		}
		else
		{ // インデックスが使用不可な場合

			// 例外処理
			assert(false);
		}
	}
}

//============================================================
//	座標のずれ取得処理
//============================================================
D3DXVECTOR3 CObjectMeshField::GetGapPosition(const int nID)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 頂点のずれの代入用

	if (m_pPosGapBuff != NULL)
	{ // 使用中の場合

		if (nID < (m_part.x + 1) * (m_part.y + 1))
		{ // インデックスが使用可能な場合

			// 頂点のずれを設定
			pos = m_pPosGapBuff[nID];
		}
		else
		{ // インデックスが使用不可な場合

			// 例外処理
			assert(false);
		}
	}

	// 引数のインデックスの頂点のずれを返す
	return pos;
}

//============================================================
//	法線の正規化処理
//============================================================
void CObjectMeshField::NormalizeNormal(void)
{
	// 変数を宣言
	int nNumNor = 0;	// 法線データ格納用

	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	//--------------------------------------------------------
	//	法線の設定
	//--------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
	{ // 縦の分割数 +1回繰り返す

		for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
		{ // 横の分割数 +1回繰り返す

			// 変数を宣言
			int nNumVtx = nCntWidth + (nCntHeight * (m_part.x + 1));	// 現在の頂点番号

			if (nNumVtx == 0)
			{ // 頂点番号が左上の場合

				// 法線の使用数を 2に設定
				m_pNumNorBuff[nNumVtx] = 2;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalRightTop(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalRightBottom(pVtx);

				// 法線データを 2つ分進める
				nNumNor += 2;
			}
			else if (nNumVtx == m_nNumVtx - 1)
			{ // 頂点番号が右下の場合

				// 法線の使用数を 2に設定
				m_pNumNorBuff[nNumVtx] = 2;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalLeftTop(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalLeftBottom(pVtx);

				// 法線データを 2つ分進める
				nNumNor += 2;
			}
			else if (nNumVtx == m_part.x)
			{ // 頂点番号が右上の場合

				// 法線の使用数を 1に設定
				m_pNumNorBuff[nNumVtx] = 1;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalLeft(pVtx);

				// 法線データを 1つ分進める
				nNumNor += 1;
			}
			else if (nNumVtx == (m_nNumVtx - 1) - m_part.x)
			{ // 頂点番号が左下の場合

				// 法線の使用数を 1に設定
				m_pNumNorBuff[nNumVtx] = 1;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalRight(pVtx);

				// 法線データを 1つ分進める
				nNumNor += 1;
			}
			else if (nNumVtx < m_part.x)
			{ // 頂点番号が角を除いた上一行の頂点の場合

				// 法線の使用数を 3に設定
				m_pNumNorBuff[nNumVtx] = 3;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalRightTop(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalRightBottom(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalLeft(pVtx);

				// 法線データを 3つ分進める
				nNumNor += 3;
			}
			else if (nNumVtx > (m_part.x + 1) * m_part.y)
			{ // 頂点番号が角を除いた下一行の頂点の場合

				// 法線の使用数を 3に設定
				m_pNumNorBuff[nNumVtx] = 3;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalRight(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalLeftTop(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalLeftBottom(pVtx);

				// 法線データを 3つ分進める
				nNumNor += 3;
			}
			else if (nCntWidth == 0)
			{ // 頂点番号が角を除いた左一行の頂点の場合

				// 法線の使用数を 3に設定
				m_pNumNorBuff[nNumVtx] = 3;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalRight(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalRightTop(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalRightBottom(pVtx);

				// 法線データを 3つ分進める
				nNumNor += 3;
			}
			else if (nCntWidth == m_part.x)
			{ // 頂点番号が角を除いた右一行の頂点の場合

				// 法線の使用数を 3に設定
				m_pNumNorBuff[nNumVtx] = 3;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalLeft(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalLeftTop(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalLeftBottom(pVtx);

				// 法線データを 3つ分進める
				nNumNor += 3;
			}
			else
			{ // 頂点番号が外周内の頂点の場合

				// 法線の使用数を 6に設定
				m_pNumNorBuff[nNumVtx] = 6;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalRight(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalRightTop(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalRightBottom(pVtx);
				m_pNorBuff[nNumNor + 3] = GetNormalLeft(pVtx);
				m_pNorBuff[nNumNor + 4] = GetNormalLeftTop(pVtx);
				m_pNorBuff[nNumNor + 5] = GetNormalLeftBottom(pVtx);

				// 法線データを 6つ分進める
				nNumNor += 6;
			}

			// 頂点データのポインタを 1つ分進める
			pVtx += 1;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//--------------------------------------------------------
	//	法線の平均化
	//--------------------------------------------------------
	// 法線データ格納用変数を初期化
	nNumNor = 0;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{ // 頂点数分繰り返す

		// 変数を宣言
		D3DXVECTOR3 nor = VEC3_ZERO;	// 法線設定用

		for (int nCntNor = 0; nCntNor < m_pNumNorBuff[nCntVtx]; nCntNor++)
		{ // 設定されている法線数分繰り返す

			// 法線を加算
			nor += m_pNorBuff[nNumNor];

			// 法線データを 1つ分進める
			nNumNor++;
		}

		// 法線を平均化
		nor /= (float)m_pNumNorBuff[nCntVtx];

		// 法線を設定
		pVtx->nor = nor;

		// 頂点データのポインタを 1つ分進める
		pVtx += 1;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================================================
//	メッシュの着地処理
//============================================================
bool CObjectMeshField::LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove)
{
	// 変数を宣言
	float fLandHeight = GetPositionHeight(rPos);	// 着地位置

	if (rPos.y < fLandHeight)
	{ // 位置が地面より下の場合

		// 位置を補正
		rPos.y = fLandHeight;

		// 移動量を初期化
		rMove.y = 0.0f;

		// 着地している状況を返す
		return true;
	}

	// 着地していない状況を返す
	return false;
}

//============================================================
//	メッシュの頂点位置の設定処理
//============================================================
void CObjectMeshField::SetMeshVertexPosition(const int nID, const D3DXVECTOR3& rPos)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{ // 使用中の場合

		if (nID < (m_part.x + 1) * (m_part.y + 1))
		{ // インデックスが使用可能な場合

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点データのポインタを引数分進める
			pVtx += nID;

			// 頂点座標の設定
			pVtx[0].pos = rPos;

			// 頂点バッファをアンロックする
			m_pVtxBuff->Unlock();
		}
	}
}

//============================================================
//	地形の設定処理
//============================================================
void CObjectMeshField::SetTerrain(const POSGRID2& rPart, D3DXVECTOR3 *pPosGap)
{
	if (rPart != m_part)
	{ // 分割数が違う場合

		// 分割数を設定
		SetPattern(rPart);
	}

	if (m_pPosGapBuff != NULL && rPart == m_part)
	{ // 座標のずれバッファが使用中且つ、同じ分割数の場合

		for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++, pPosGap++)
		{ // 頂点数分繰り返す

			// 引数の座標のずれ量を設定
			m_pPosGapBuff[nCntVtx] = *pPosGap;
		}
	}
	else { assert(false); }	// 非使用中

	// 頂点・インデックス情報の設定
	SetVtx(true);
	SetIdx();
}

//============================================================
//	頂点数取得処理
//============================================================
int CObjectMeshField::GetNumVertex(void) const
{
	// 頂点数を返す
	return m_nNumVtx;
}

//============================================================
//	メッシュの頂点位置の設定処理
//============================================================
D3DXVECTOR3 CObjectMeshField::GetMeshVertexPosition(const int nID)
{
	// 変数を宣言
	D3DXVECTOR3 pos;	// 頂点座標の代入用

	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{ // 使用中の場合

		if (nID < (m_part.x + 1) * (m_part.y + 1))
		{ // インデックスが使用可能な場合

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点データのポインタを引数分進める
			pVtx += nID;

			// 頂点座標を代入
			pos = pVtx[0].pos;

			// 頂点バッファをアンロックする
			m_pVtxBuff->Unlock();
		}
	}

	// 引数のインデックスの頂点座標を返す
	return pos;
}

//============================================================
//	メッシュの範囲内取得処理 (回転考慮)
//============================================================
bool CObjectMeshField::IsPositionRange(const D3DXVECTOR3&rPos)
{
	// 変数配列を宣言
	D3DXVECTOR3 aVtxPos[4];		// ポリゴンの頂点座標
	D3DXVECTOR3 aVtxMtxPos[4];	// ポリゴンの位置・向き反映を行った頂点座標

	// メッシュの頂点位置を設定
	aVtxPos[0] = D3DXVECTOR3(m_meshField.pos.x - m_meshField.size.x * 0.5f, 0.0f, m_meshField.pos.z + m_meshField.size.y * 0.5f);	// 左上
	aVtxPos[1] = D3DXVECTOR3(m_meshField.pos.x - m_meshField.size.x * 0.5f, 0.0f, m_meshField.pos.z - m_meshField.size.y * 0.5f);	// 左下
	aVtxPos[2] = D3DXVECTOR3(m_meshField.pos.x + m_meshField.size.x * 0.5f, 0.0f, m_meshField.pos.z - m_meshField.size.y * 0.5f);	// 右下
	aVtxPos[3] = D3DXVECTOR3(m_meshField.pos.x + m_meshField.size.x * 0.5f, 0.0f, m_meshField.pos.z + m_meshField.size.y * 0.5f);	// 右上

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{ // 三角形ポリゴンの頂点数分繰り返す

		// 変数を宣言
		D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// 計算用マトリックス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// 頂点位置を反映
		D3DXMatrixTranslation(&mtxTrans, aVtxPos[nCntVtx].x, aVtxPos[nCntVtx].y, aVtxPos[nCntVtx].z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// ポリゴン向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshField.rot.y, m_meshField.rot.x, m_meshField.rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// ポリゴン位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_meshField.pos.x, m_meshField.pos.y, m_meshField.pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// 計算したマトリックスから座標を設定
		aVtxMtxPos[nCntVtx] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	}

	// 判定を返す
	return collision::BoxOuterPillar(aVtxMtxPos[0], aVtxMtxPos[1], aVtxMtxPos[2], aVtxMtxPos[3], rPos);
}

//============================================================
//	メッシュの着地位置の取得処理 (回転非考慮)
//============================================================
float CObjectMeshField::GetPositionHeight(const D3DXVECTOR3& rPos)
{
	// 変数を宣言
	int nWidth  = (int)(( rPos.x + m_meshField.size.x * 0.5f) / (m_meshField.size.x / m_part.x));	// 横の分割位置
	int nHeight = (int)((-rPos.z + m_meshField.size.y * 0.5f) / (m_meshField.size.y / m_part.y));	// 縦の分割位置
	int nNumVtx = nWidth + (nHeight * (m_part.x + 1));	// 分割位置の左上頂点番号
	D3DXVECTOR3 nor;	// 法線ベクトル

	// 変数配列を宣言
	D3DXVECTOR3 aVtxPos[4];	// ポリゴンの頂点座標

	for (int nCntHeight = 0; nCntHeight < 3; nCntHeight++)
	{ // 縦の判定確認数分繰り返す

		// 変数を宣言
		int nColl = -(m_part.x + 2) + (nCntHeight * (m_part.x + 1));	// 判定する頂点番号の変更用

		for (int nCntWidth = 0; nCntWidth < 3; nCntWidth++)
		{ // 横の判定確認数分繰り返す

			// 変数配列を宣言
			int aVtxNum[4] =	// 頂点番号
			{ // 初期化
				nColl + nNumVtx + m_part.x + 1, // 左下
				nColl + nNumVtx,			    // 左上
				nColl + nNumVtx + m_part.x + 2, // 右下
				nColl + nNumVtx + 1			    // 右上
			};

			if (aVtxNum[1] >= 0 && aVtxNum[2] <= (m_part.x + 1) * (m_part.y + 1) - 1)
			{ // 頂点番号が範囲内の場合

				// ポリゴンの頂点位置を取得
				aVtxPos[0] = m_meshField.pos + GetMeshVertexPosition(aVtxNum[0]);
				aVtxPos[1] = m_meshField.pos + GetMeshVertexPosition(aVtxNum[1]);
				aVtxPos[2] = m_meshField.pos + GetMeshVertexPosition(aVtxNum[2]);
				aVtxPos[3] = m_meshField.pos + GetMeshVertexPosition(aVtxNum[3]);

				if (collision::TriangleOuterPillar(aVtxPos[0], aVtxPos[2], aVtxPos[1], rPos))
				{ // ポリゴンの範囲内にいる場合

					// 法線を求める
					useful::NormalizeNormal(aVtxPos[1], aVtxPos[0], aVtxPos[2], nor);

					if (nor.y != 0.0f)
					{ // 法線が設定されている場合

						// プレイヤーの着地点を返す
						return (((rPos.x - aVtxPos[0].x) * nor.x + (-aVtxPos[0].y) * nor.y + (rPos.z - aVtxPos[0].z) * nor.z) * -1.0f) / nor.y;
					}
				}

				if (collision::TriangleOuterPillar(aVtxPos[3], aVtxPos[1], aVtxPos[2], rPos))
				{ // ポリゴンの範囲内にいる場合

					// 法線を求める
					useful::NormalizeNormal(aVtxPos[2], aVtxPos[3], aVtxPos[1], nor);

					if (nor.y != 0.0f)
					{ // 法線が設定されている場合

						// プレイヤーの着地点を返す
						return (((rPos.x - aVtxPos[3].x) * nor.x + (-aVtxPos[3].y) * nor.y + (rPos.z - aVtxPos[3].z) * nor.z) * -1.0f) / nor.y;
					}
				}
			}

			// 判定する頂点番号を隣に変更
			nColl++;
		}
	}

	// 着地範囲外の場合現在のy座標を返す
	return rPos.y;
}

//============================================================
//	メッシュの着地位置の取得処理 (回転考慮)
//============================================================
float CObjectMeshField::GetPositionRotateHeight(const D3DXVECTOR3&rPos)
{
	// 変数を宣言
	int nNumCul;		// 法線ベクトル用の頂点計算用
	int nNumVtx;		// 法線を求める頂点番号
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DXVECTOR3 pos;	// ターゲット位置

	// 変数配列を宣言
	D3DXVECTOR3 aVtxPos[NUM_VTX_TRIANGLE];		// ポリゴンの頂点座標
	D3DXVECTOR3 aVtxMtxPos[NUM_VTX_TRIANGLE];	// ポリゴンの位置・向き反映を行った頂点座標

	for (int nCntVtx = 0; nCntVtx < 2; nCntVtx++)
	{ // 四角ポリゴンに使用する三角の数分繰り返す

		// 法線ベクトル用の頂点計算用の値を設定
		nNumCul = (nCntVtx == 0) ? 1: -1;

		for (int nCntHeight = 0; nCntHeight < m_part.y; nCntHeight++)
		{ // 縦の分割数分繰り返す

			for (int nCntWidth = 0; nCntWidth < m_part.x; nCntWidth++)
			{ // 横の分割数分繰り返す

				// 法線を求める頂点番号を設定
				nNumVtx = ((nCntWidth + 1) + (nCntHeight * (m_part.x + 1))) + (nCntVtx * m_part.x);

				// ターゲット位置を設定
				pos = rPos;

				if (collision::CirclePillar(pos, GetMeshVertexPosition(nNumVtx), COLL_RADIUS, 0.0f))
				{ // 頂点座標が近くにある場合

					// ポリゴンの頂点位置を取得
					aVtxPos[0] = GetMeshVertexPosition(nNumVtx);
					aVtxPos[1] = GetMeshVertexPosition(nNumVtx - (1 * nNumCul));
					aVtxPos[2] = GetMeshVertexPosition(nNumVtx + ((m_part.x + 1) * nNumCul));

					for (int nCntTriangle = 0; nCntTriangle < NUM_VTX_TRIANGLE; nCntTriangle++)
					{ // 三角形ポリゴンの頂点数分繰り返す

						// 変数を宣言
						D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// 計算用マトリックス

						// ワールドマトリックスの初期化
						D3DXMatrixIdentity(&mtxWorld);

						// 頂点位置を反映
						D3DXMatrixTranslation(&mtxTrans, aVtxPos[nCntTriangle].x, aVtxPos[nCntTriangle].y, aVtxPos[nCntTriangle].z);
						D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

						// ポリゴン向きを反映
						D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshField.rot.y, m_meshField.rot.x, m_meshField.rot.z);
						D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

						// ポリゴン位置を反映
						D3DXMatrixTranslation(&mtxTrans, m_meshField.pos.x, m_meshField.pos.y, m_meshField.pos.z);
						D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

						// 計算したマトリックスから座標を設定
						aVtxMtxPos[nCntTriangle] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
					}

					if (collision::TriangleOuterPillar(aVtxMtxPos[0], aVtxMtxPos[1], aVtxMtxPos[2], rPos))
					{ // ポリゴンの範囲内にいる場合

						// 法線を求める
						useful::NormalizeNormal(aVtxMtxPos[1], aVtxMtxPos[0], aVtxMtxPos[2], nor);

						if (nor.y != 0.0f)
						{ // 法線が設定されている場合

							// プレイヤーの着地点を返す
							return (((rPos.x - aVtxMtxPos[0].x) * nor.x + (-aVtxMtxPos[0].y) * nor.y + (rPos.z - aVtxMtxPos[0].z) * nor.z) * -1.0f) / nor.y;
						}
					}
				}
			}
		}
	}

	// 着地範囲外の場合現在のy座標を返す
	return rPos.y;
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObjectMeshField::SetVtx(bool bNor)
{
	// 変数を宣言
	int nNumVtx = 0;	// 現在の頂点番号

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

				// 現在の頂点番号を設定
				nNumVtx = nCntWidth + (nCntHeight * (m_part.x + 1));

				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3
				( // 引数
					nCntWidth * (m_meshField.size.x / (float)m_part.x) - (m_meshField.size.x * 0.5f),		// x
					0.0f,																					// y
					-(nCntHeight * (m_meshField.size.y / (float)m_part.y) - (m_meshField.size.y * 0.5f))	// z
				);
				pVtx[0].pos += m_pPosGapBuff[nNumVtx];	// 頂点からのずれ量を加算

				// 頂点カラーの設定
				pVtx[0].col = m_meshField.col;

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntWidth, 1.0f * nCntHeight);

				// 頂点データのポインタを 1つ分進める
				pVtx += 1;
			}
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}

	if (bNor)
	{ // 法線の正規化も行う場合

		// 法線の設定・正規化
		NormalizeNormal();
	}
}

//============================================================
//	インデックス情報の設定処理
//============================================================
void CObjectMeshField::SetIdx(void)
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
void CObjectMeshField::SetScrollTex(const float fTexU, const float fTexV)
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

//============================================================
//	法線の取得処理 (左)
//============================================================
D3DXVECTOR3 CObjectMeshField::GetNormalLeft(VERTEX_3D *pVtx)
{
	// 変数を宣言
	D3DXVECTOR3 nor;	// 法線代入用

	// 法線の正規化
	useful::NormalizeNormal
	( // 引数
		pVtx[m_part.x + 1].pos,	// 左位置
		pVtx[0].pos,			// 中心位置
		pVtx[-1].pos,			// 右位置
		nor						// 法線
	);

	// 法線を返す
	return nor;
}

//============================================================
//	法線の取得処理 (左上)
//============================================================
D3DXVECTOR3 CObjectMeshField::GetNormalLeftTop(VERTEX_3D *pVtx)
{
	// 変数を宣言
	D3DXVECTOR3 nor;	// 法線代入用

	// 法線の正規化
	useful::NormalizeNormal
	( // 引数
		pVtx[-(m_part.x + 2)].pos,	// 左位置
		pVtx[0].pos,				// 中心位置
		pVtx[-(m_part.x + 1)].pos,	// 右位置
		nor							// 法線
	);

	// 法線を返す
	return nor;
}

//============================================================
//	法線の取得処理 (左下)
//============================================================
D3DXVECTOR3 CObjectMeshField::GetNormalLeftBottom(VERTEX_3D *pVtx)
{
	// 変数を宣言
	D3DXVECTOR3 nor;	// 法線代入用

	// 法線の正規化
	useful::NormalizeNormal
	( // 引数
		pVtx[-1].pos,				// 左位置
		pVtx[0].pos,				// 中心位置
		pVtx[-(m_part.x + 2)].pos,	// 右位置
		nor							// 法線
	);

	// 法線を返す
	return nor;
}

//============================================================
//	法線の取得処理 (右)
//============================================================
D3DXVECTOR3 CObjectMeshField::GetNormalRight(VERTEX_3D *pVtx)
{
	// 変数を宣言
	D3DXVECTOR3 nor;	// 法線代入用

	// 法線の正規化
	useful::NormalizeNormal
	( // 引数
		pVtx[-(m_part.x + 1)].pos,	// 左位置
		pVtx[0].pos,				// 中心位置
		pVtx[1].pos,				// 右位置
		nor							// 法線
	);

	// 法線を返す
	return nor;
}

//============================================================
//	法線の取得処理 (右上)
//============================================================
D3DXVECTOR3 CObjectMeshField::GetNormalRightTop(VERTEX_3D *pVtx)
{
	// 変数を宣言
	D3DXVECTOR3 nor;	// 法線代入用

	// 法線の正規化
	useful::NormalizeNormal
	( // 引数
		pVtx[1].pos,			// 左位置
		pVtx[0].pos,			// 中心位置
		pVtx[m_part.x + 2].pos,	// 右位置
		nor						// 法線
	);

	// 法線を返す
	return nor;
}

//============================================================
//	法線の取得処理 (右下)
//============================================================
D3DXVECTOR3 CObjectMeshField::GetNormalRightBottom(VERTEX_3D *pVtx)
{
	// 変数を宣言
	D3DXVECTOR3 nor;	// 法線代入用

	// 法線の正規化
	useful::NormalizeNormal
	( // 引数
		pVtx[m_part.x + 2].pos,	// 左位置
		pVtx[0].pos,			// 中心位置
		pVtx[m_part.x + 1].pos,	// 右位置
		nor						// 法線
	);

	// 法線を返す
	return nor;
}
