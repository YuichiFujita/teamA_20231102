//============================================================
//
//	オブジェクトメッシュキューブ処理 [objectMeshCube.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectMeshCube.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define NUM_SET_CUBE	(2)		// 一つのキューブに使用するキューブの数
#define NUM_VTX_CUBE	(8)		// 四角形の頂点数
#define NEED_VTX_CUBE	(24)	// メッシュキューブに必要な頂点・インデックス数
#define NUM_CUBE_FACE	(6)		// 四角形の面の数
#define NUM_VTX_FACE	(4)		// 四角形の一面の頂点数

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const D3DXVECTOR3 CObjectMeshCube::aPosMeshCube[] =	// 頂点座標設定用
{
	D3DXVECTOR3(-1.0f, +1.0f, -1.0f), D3DXVECTOR3(-1.0f, -1.0f, -1.0f),
	D3DXVECTOR3(+1.0f, +1.0f, -1.0f), D3DXVECTOR3(+1.0f, -1.0f, -1.0f),
	D3DXVECTOR3(+1.0f, +1.0f, +1.0f), D3DXVECTOR3(+1.0f, -1.0f, +1.0f),
	D3DXVECTOR3(-1.0f, +1.0f, +1.0f), D3DXVECTOR3(-1.0f, -1.0f, +1.0f),
};

const D3DXVECTOR3 CObjectMeshCube::aNorMeshCube[] =	// 法線ベクトル設定用
{
	D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(+1.0f, 0.0f, 0.0f), D3DXVECTOR3(+1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(+1.0f, 0.0f, 0.0f), D3DXVECTOR3(+1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, +1.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
	D3DXVECTOR3(0.0f, +1.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
	D3DXVECTOR3(0.0f, +1.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
	D3DXVECTOR3(0.0f, +1.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f),
	D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f),
	D3DXVECTOR3(0.0f, 0.0f, +1.0f), D3DXVECTOR3(0.0f, 0.0f, +1.0f),
	D3DXVECTOR3(0.0f, 0.0f, +1.0f), D3DXVECTOR3(0.0f, 0.0f, +1.0f),
};

const D3DXVECTOR2 CObjectMeshCube::aTexMeshCube[] =	// テクスチャ座標設定用
{
	D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f),
	D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f),
	D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f),
	D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f),
	D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f),
	D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f),
};

const int CObjectMeshCube::aIdxMeshCube[] =			// インデックス設定用
{
	8, 14, 10, 12, 4, 5, 2, 3, 18, 19, 16, 17, 0, 1, 6, 7, 22, 23, 20, 21, 13, 15, 11, 9,
};

//************************************************************
//	子クラス [CObjectMeshCube] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectMeshCube::CObjectMeshCube()
{
	// メンバ変数をクリア
	m_pVtxBuff	= NULL;	// 頂点バッファ
	m_pIdxBuff	= NULL;	// インデックスバッファ
	m_nNumVtx	= 0;	// 必要頂点数
	m_nNumIdx	= 0;	// 必要インデックス数
	m_origin	= ORIGIN_CENTER;	// 原点
	memset(&m_meshCube, 0, sizeof(m_meshCube));	// メッシュキューブの情報
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObjectMeshCube::CObjectMeshCube(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	m_pVtxBuff	= NULL;	// 頂点バッファ
	m_pIdxBuff	= NULL;	// インデックスバッファ
	m_nNumVtx	= 0;	// 必要頂点数
	m_nNumIdx	= 0;	// 必要インデックス数
	m_origin	= ORIGIN_CENTER;	// 原点
	memset(&m_meshCube, 0, sizeof(m_meshCube));	// メッシュキューブの情報
}

//============================================================
//	デストラクタ
//============================================================
CObjectMeshCube::~CObjectMeshCube()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectMeshCube::Init(void)
{
	// メンバ変数を初期化
	m_pVtxBuff = NULL;	// 頂点バッファ
	m_pIdxBuff = NULL;	// インデックスバッファ
	m_nNumVtx = 0;		// 必要頂点数
	m_nNumIdx = 0;		// 必要インデックス数
	m_origin = ORIGIN_CENTER;	// 原点

	// 基本情報の初期化
	m_meshCube.pos  = VEC3_ZERO;			// 位置
	m_meshCube.rot  = VEC3_ZERO;			// 向き
	m_meshCube.size = VEC3_ZERO;			// 大きさ
	m_meshCube.aCol[0] = XCOL_WHITE;		// キューブ色
	m_meshCube.aCol[1] = XCOL_BLACK;		// 縁取り色
	m_meshCube.bordState  = BORDER_OFF;		// 縁取り使用状態
	m_meshCube.fBordThick = 0.0f;			// 縁取り太さ
	m_meshCube.texState   = TEXSTATE_ONE;	// テクスチャの使用状態

	// テクスチャインデックスの初期化
	m_meshCube.texID.All	= NONE_IDX;	// 全面のテクスチャインデックス
	m_meshCube.texID.Left	= NONE_IDX;	// 左のテクスチャインデックス
	m_meshCube.texID.Right	= NONE_IDX;	// 右のテクスチャインデックス
	m_meshCube.texID.Bottom	= NONE_IDX;	// 下のテクスチャインデックス
	m_meshCube.texID.Top	= NONE_IDX;	// 上のテクスチャインデックス
	m_meshCube.texID.Near	= NONE_IDX;	// 前のテクスチャインデックス
	m_meshCube.texID.Far	= NONE_IDX;	// 後のテクスチャインデックス

	// テクスチャ分割数の初期化
	m_meshCube.aTexPart[0] = VEC2_ONE;	// 分割数 x
	m_meshCube.aTexPart[1] = VEC2_ONE;	// 分割数 y
	m_meshCube.aTexPart[2] = VEC2_ONE;	// 分割数 z

	// 描画情報の初期化
	m_meshCube.cull   = D3DCULL_CCW;	// カリング状況
	m_meshCube.bLight = true;			// ライティング状況

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectMeshCube::Uninit(void)
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

	// オブジェクトメッシュキューブを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectMeshCube::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CObjectMeshCube::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// 変数配列を宣言
	int aTexType[NUM_CUBE_FACE];	// テクスチャ設定用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_meshCube.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshCube.rot.y, m_meshCube.rot.x, m_meshCube.rot.z);
	D3DXMatrixMultiply(&m_meshCube.mtxWorld, &m_meshCube.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_meshCube.pos.x, m_meshCube.pos.y, m_meshCube.pos.z);
	D3DXMatrixMultiply(&m_meshCube.mtxWorld, &m_meshCube.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_meshCube.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの裏面のみを表示状態にする
	pDevice->SetRenderState(D3DRS_CULLMODE, m_meshCube.cull);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, m_meshCube.bLight);

	// キューブの描画
	switch (m_meshCube.texState)
	{ // テクスチャ使用状態ごとの処理
	case TEXSTATE_ONE:		// 同一テクスチャ

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetTexture(m_meshCube.texID.All));

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive
		( // 引数
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			0,
			0,
			NEED_VTX_CUBE,			// 使用する頂点数
			0,						// インデックスバッファの開始地点
			NEED_VTX_CUBE - 2		// プリミティブ (ポリゴン) 数
		);

		// 処理を抜ける
		break;

	case TEXSTATE_SELECT:	// 全選択テクスチャ

		// テクスチャの種類を設定
		aTexType[0] = m_meshCube.texID.Top;		// 上のテクスチャ
		aTexType[1] = m_meshCube.texID.Right;	// 右のテクスチャ
		aTexType[2] = m_meshCube.texID.Near;	// 前のテクスチャ
		aTexType[3] = m_meshCube.texID.Left;	// 左のテクスチャ
		aTexType[4] = m_meshCube.texID.Far;		// 後のテクスチャ
		aTexType[5] = m_meshCube.texID.Bottom;	// 下のテクスチャ
		
		for (int nCntFace = 0; nCntFace < NUM_CUBE_FACE; nCntFace++)
		{ // 面の総数分繰り返す

			// テクスチャの設定
			pDevice->SetTexture(0, pTexture->GetTexture(aTexType[nCntFace]));

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive
			( // 引数
				D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				0,
				0,
				NUM_VTX_FACE,				// 使用する頂点数
				nCntFace * NUM_VTX_FACE,	// インデックスバッファの開始地点
				NUM_VTX_FACE - 2			// プリミティブ (ポリゴン) 数
			);
		}

		// 処理を抜ける
		break;
	}

	// ポリゴンの表面のみを表示状態にする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// 縁取りの描画
	if (m_meshCube.bordState == BORDER_ON)
	{ // 縁取りがONの場合

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの裏面のみを表示状態にする
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		// ライティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, false);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive
		( // 引数
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			0,
			0,
			NEED_VTX_CUBE,			// 使用する頂点数
			NEED_VTX_CUBE,			// インデックスバッファの開始地点
			NEED_VTX_CUBE - 2		// プリミティブ (ポリゴン) 数
		);

		// ポリゴンの表面のみを表示状態にする
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// ライティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, true);
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CObjectMeshCube::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_meshCube.pos = rPos;
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObjectMeshCube::GetVec3Position(void) const
{
	// 位置を返す
	return m_meshCube.pos;
}

//============================================================
//	向きの設定処理
//============================================================
void CObjectMeshCube::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 引数の向きを設定
	m_meshCube.rot = rRot;

	// 向きの正規化
	useful::Vec3NormalizeRot(m_meshCube.rot);
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CObjectMeshCube::GetVec3Rotation(void) const
{
	// 向きを返す
	return m_meshCube.rot;
}

//============================================================
//	大きさの設定処理
//============================================================
void CObjectMeshCube::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// 引数の大きさを設定
	m_meshCube.size = rSize;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	大きさ取得処理
//============================================================
D3DXVECTOR3 CObjectMeshCube::GetVec3Sizing(void) const
{
	// 大きさを返す
	return m_meshCube.size;
}

//============================================================
//	キューブ色の設定処理
//============================================================
void CObjectMeshCube::SetColor(const D3DXCOLOR& rCol)
{
	// 引数のキューブ色を設定
	m_meshCube.aCol[CUBECOL_CUBE] = rCol;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	キューブ色取得処理
//============================================================
D3DXCOLOR CObjectMeshCube::GetColor(void) const
{
	// キューブ色を返す
	return m_meshCube.aCol[CUBECOL_CUBE];
}

//============================================================
//	マトリックスポインタ取得処理
//============================================================
D3DXMATRIX *CObjectMeshCube::GetPtrMtxWorld(void)
{
	// ワールドマトリックスのポインタを返す
	return &m_meshCube.mtxWorld;
}

//============================================================
//	生成処理
//============================================================
CObjectMeshCube *CObjectMeshCube::Create
(
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rRot,		// 向き
	const D3DXVECTOR3& rSize,		// 大きさ
	const D3DXCOLOR& rCubeCol,		// キューブ色
	const D3DXCOLOR& rBorderCol,	// 縁取り色
	const EBorder bordState,		// 縁取り状態
	const float fBordThick,			// 縁取り太さ
	const ETexState texState,		// テクスチャ状態
	const SFaceTex& rTexID,			// テクスチャ種類
	const EOrigin origin,			// 原点
	const D3DXVECTOR2& rTexPartX,	// テクスチャ分割数 x
	const D3DXVECTOR2& rTexPartY,	// テクスチャ分割数 y
	const D3DXVECTOR2& rTexPartZ,	// テクスチャ分割数 z
	const D3DCULL cull,				// カリング状況
	const bool bLight				// ライティング状況
)
{
	// ポインタを宣言
	CObjectMeshCube *pObjectMeshCube = NULL;	// オブジェクトメッシュキューブ生成用

	if (pObjectMeshCube == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pObjectMeshCube = new CObjectMeshCube;	// オブジェクトメッシュキューブ
	}
	else { assert(false); return NULL; }	// 使用中

	if (pObjectMeshCube != NULL)
	{ // 確保に成功している場合

		// オブジェクトメッシュキューブの初期化
		if (FAILED(pObjectMeshCube->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObjectMeshCube;
			pObjectMeshCube = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャを割当
		pObjectMeshCube->BindTexture(rTexID);

		// 原点を設定
		pObjectMeshCube->SetOrigin(origin);

		// 位置を設定
		pObjectMeshCube->SetVec3Position(rPos);

		// 向きを設定
		pObjectMeshCube->SetVec3Rotation(rRot);

		// 大きさを設定
		pObjectMeshCube->SetVec3Sizing(rSize);

		// キューブ色を設定
		pObjectMeshCube->SetColor(rCubeCol);

		// 縁取り色を設定
		pObjectMeshCube->SetBorderColor(rBorderCol);

		// 縁取りの状態を設定
		if (FAILED(pObjectMeshCube->SetBorderState(bordState)))
		{ // 分割数の設定に失敗した場合

			// メモリ開放
			delete pObjectMeshCube;
			pObjectMeshCube = NULL;

			// 失敗を返す
			return NULL;
		}

		// 縁取りの太さを設定
		pObjectMeshCube->SetBorderThick(fBordThick);

		// テクスチャの状態を設定
		pObjectMeshCube->SetTextureState(texState);

		// テクスチャの分割数Xを設定
		pObjectMeshCube->SetTexturePatternX(rTexPartX);

		// テクスチャの分割数Yを設定
		pObjectMeshCube->SetTexturePatternY(rTexPartY);

		// テクスチャの分割数Zを設定
		pObjectMeshCube->SetTexturePatternZ(rTexPartZ);

		// カリングを設定
		pObjectMeshCube->SetCulling(cull);

		// ライティングを設定
		pObjectMeshCube->SetLighting(bLight);

		// 確保したアドレスを返す
		return pObjectMeshCube;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	テクスチャ割当処理
//============================================================
void CObjectMeshCube::BindTexture(const SFaceTex textureID)
{
	// テクスチャインデックスを代入
	m_meshCube.texID = textureID;
}

//============================================================
//	縁取り色の設定処理
//============================================================
void CObjectMeshCube::SetBorderColor(const D3DXCOLOR& rCol)
{
	// 引数の縁取り色を設定
	m_meshCube.aCol[CUBECOL_BORDER] = rCol;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	縁取り色取得処理
//============================================================
D3DXCOLOR CObjectMeshCube::GetBorderColor(void) const
{
	// 縁取り色を返す
	return m_meshCube.aCol[CUBECOL_BORDER];
}

//============================================================
//	縁取りの状態の設定処理
//============================================================
HRESULT CObjectMeshCube::SetBorderState(const EBorder bordState)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 引数の縁取りの状態を設定
	m_meshCube.bordState = bordState;

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
			sizeof(VERTEX_3D) * (NEED_VTX_CUBE + ((int)bordState * NEED_VTX_CUBE)),	// 必要頂点数
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
			sizeof(WORD) * (NEED_VTX_CUBE + ((int)bordState * NEED_VTX_CUBE)),	// 必要インデックス数
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
//	縁取り状態取得処理
//============================================================
CObjectMeshCube::EBorder CObjectMeshCube::GetBorderState(void) const
{
	// 縁取りの状態を返す
	return m_meshCube.bordState;
}

//============================================================
//	縁取りの太さの設定処理
//============================================================
void CObjectMeshCube::SetBorderThick(const float fBordThick)
{
	// 引数の縁取りの太さを設定
	m_meshCube.fBordThick = fBordThick;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	縁取り太さ取得処理
//============================================================
float CObjectMeshCube::GetBorderThick(void) const
{
	// 縁取りの太さを返す
	return m_meshCube.fBordThick;
}

//============================================================
//	テクスチャの状態の設定処理
//============================================================
void CObjectMeshCube::SetTextureState(const ETexState texState)
{
	// 引数のテクスチャの状態を設定
	m_meshCube.texState = texState;
}

//============================================================
//	テクスチャ状態取得処理
//============================================================
CObjectMeshCube::ETexState CObjectMeshCube::GetTextureState(void) const
{
	// テクスチャの状態を返す
	return m_meshCube.texState;
}

//============================================================
//	テクスチャの分割数Xの設定処理
//============================================================
void CObjectMeshCube::SetTexturePatternX(const D3DXVECTOR2& rTexPart)
{
	// 引数のテクスチャの分割数Xを設定
	m_meshCube.aTexPart[CUBEPART_X] = rTexPart;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	テクスチャ分割数X取得処理
//============================================================
D3DXVECTOR2 CObjectMeshCube::GetTexturePatternX(void) const
{
	// テクスチャの分割数Xを返す
	return m_meshCube.aTexPart[CUBEPART_X];
}

//============================================================
//	テクスチャの分割数Yの設定処理
//============================================================
void CObjectMeshCube::SetTexturePatternY(const D3DXVECTOR2& rTexPart)
{
	// 引数のテクスチャの分割数Yを設定
	m_meshCube.aTexPart[CUBEPART_Y] = rTexPart;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	テクスチャ分割数Y取得処理
//============================================================
D3DXVECTOR2 CObjectMeshCube::GetTexturePatternY(void) const
{
	// テクスチャの分割数Yを返す
	return m_meshCube.aTexPart[CUBEPART_Y];
}

//============================================================
//	テクスチャの分割数Zの設定処理
//============================================================
void CObjectMeshCube::SetTexturePatternZ(const D3DXVECTOR2& rTexPart)
{
	// 引数のテクスチャの分割数Zを設定
	m_meshCube.aTexPart[CUBEPART_Z] = rTexPart;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	テクスチャ分割数Z取得処理
//============================================================
D3DXVECTOR2 CObjectMeshCube::GetTexturePatternZ(void) const
{
	// テクスチャの分割数Zを返す
	return m_meshCube.aTexPart[CUBEPART_Z];
}

//============================================================
//	原点設定処理
//============================================================
void CObjectMeshCube::SetOrigin(const EOrigin origin)
{
	// 引数の原点を設定
	m_origin = origin;
}

//============================================================
//	原点取得処理
//============================================================
CObjectMeshCube::EOrigin CObjectMeshCube::GetOrigin(void) const
{
	// 原点を返す
	return m_origin;
}

//============================================================
//	カリング設定処理
//============================================================
void CObjectMeshCube::SetCulling(const D3DCULL cull)
{
	// 引数のカリング状況を設定
	m_meshCube.cull = cull;
}

//============================================================
//	カリング取得処理
//============================================================
D3DCULL CObjectMeshCube::GetCulling(void) const
{
	// カリング状況を返す
	return m_meshCube.cull;
}

//============================================================
//	ライティング設定処理
//============================================================
void CObjectMeshCube::SetLighting(const bool bLight)
{
	// 引数のライティング状況を設定
	m_meshCube.bLight = bLight;
}

//============================================================
//	ライティング取得処理
//============================================================
bool CObjectMeshCube::GetLighting(void) const
{
	// ライティング状況を返す
	return m_meshCube.bLight;
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObjectMeshCube::SetVtx(void)
{
	// 変数を宣言
	int nLoop = 0;	// キューブの描画数
	float fSetBord = 0.0f, fUseBord = 0.0f;	// 頂点座標計算用
	D3DXVECTOR2 texPart = VEC2_ZERO;		// テクスチャ分割数の計算用

	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 縁取りの有無で描画数を変更
		nLoop = (int)m_meshCube.bordState + 1;

		for (int nCntBorder = 0; nCntBorder < nLoop; nCntBorder++)
		{ // 縁取りがONの場合二回繰り返す

			// 頂点座標の計算用の値を設定
			fSetBord = (float)(nCntBorder - 1) * m_meshCube.fBordThick;
			fUseBord = (float)m_meshCube.bordState;

			for (int nCntSet = 0; nCntSet < NEED_VTX_CUBE; nCntSet++)
			{ // 必要頂点・インデックス数分繰り返す

				// 変数を宣言
				int nVtxID = nCntSet % NUM_VTX_CUBE;	// 現在の箱の頂点番号
				int nBoxID = nCntSet / NUM_VTX_CUBE;	// 現在の箱番号

				if (nVtxID == 0)
				{ // 頂点の設定が一巡した場合

					// テクスチャ分割数を変更
					texPart = D3DXVECTOR2
					( // 引数
						m_meshCube.aTexPart[nBoxID].x,	// x
						m_meshCube.aTexPart[nBoxID].y	// y
					);
				}

				switch (m_origin)
				{ // 原点ごとの処理
				case ORIGIN_CENTER:	// 中央
			
					// 頂点座標の設定
					pVtx[0].pos.x = (m_meshCube.size.x * aPosMeshCube[nVtxID].x) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].x);
					pVtx[0].pos.y = (m_meshCube.size.y * aPosMeshCube[nVtxID].y) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].y);
					pVtx[0].pos.z = (m_meshCube.size.z * aPosMeshCube[nVtxID].z) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].z);
			
					break;
			
				case ORIGIN_DOWN:	// 下
			
					// 頂点座標の設定
					pVtx[0].pos.x = (m_meshCube.size.x * aPosMeshCube[nVtxID].x) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].x);
					pVtx[0].pos.y = (m_meshCube.size.y * aPosMeshCube[nVtxID].y * 2.0f) * -((nVtxID % 2) - 1) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].y);
					pVtx[0].pos.z = (m_meshCube.size.z * aPosMeshCube[nVtxID].z) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].z);
			
					break;
			
				default:	// 例外処理
					assert(false);
					break;
				}

				// 法線ベクトルの設定
				pVtx[0].nor = aNorMeshCube[nCntSet];

				// 頂点カラーの設定
				pVtx[0].col = m_meshCube.aCol[nCntBorder];

				// テクスチャ座標の設定
				pVtx[0].tex.x = texPart.x * aTexMeshCube[nCntSet].x;
				pVtx[0].tex.y = texPart.y * aTexMeshCube[nCntSet].y;

				// 頂点データのポインタを移動
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
void CObjectMeshCube::SetIdx(void)
{
	// 変数を宣言
	int nNumVtx = 0;	// 頂点数の計測用
	int nLoop = 0;		// キューブの描画数

	// ポインタを宣言
	WORD *pIdx;	// インデックス情報へのポインタ

	if (m_pIdxBuff != NULL)
	{ // 使用中の場合

		// インデックスバッファをロックし、頂点番号データへのポインタを取得
		m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		// 縁取りの有無で描画数を変更
		nLoop = (int)m_meshCube.bordState + 1;

		for (int nCntBorder = 0; nCntBorder < nLoop; nCntBorder++)
		{ // 縁取りがONの場合二回繰り返す

			for (int nCntSet = 0; nCntSet < NEED_VTX_CUBE; nCntSet++)
			{ // 必要頂点・インデックス数分繰り返す

				// インデックスの設定
				pIdx[0] = (WORD)(aIdxMeshCube[nCntSet] + nNumVtx);

				// インデックスデータのポインタを移動
				pIdx += 1;
			}

			// 頂点バッファの開始地点を必要数分ずらす
			nNumVtx += NEED_VTX_CUBE;
		}

		// インデックスバッファをアンロックする
		m_pIdxBuff->Unlock();
	}
}
