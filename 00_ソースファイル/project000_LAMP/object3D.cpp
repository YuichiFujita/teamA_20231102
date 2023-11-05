//============================================================
//
//	オブジェクト3D処理 [object3D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_VERTEX	(4)	// 頂点数

//************************************************************
//	子クラス [CObject3D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObject3D::CObject3D()
{
	// メンバ変数をクリア
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ワールドマトリックス
	m_pVtxBuff		= NULL;			// 頂点バッファへのポインタ
	m_pPosGapBuff	= NULL;			// 座標のずれバッファ
	m_pos		= VEC3_ZERO;		// 位置
	m_rot		= VEC3_ZERO;		// 向き
	m_size		= VEC3_ZERO;		// 大きさ
	m_col		= XCOL_WHITE;		// 色
	m_origin	= ORIGIN_CENTER;	// 原点
	m_cull		= D3DCULL_CCW;		// カリング状況
	m_func		= D3DCMP_ALWAYS;	// Zテスト設定
	m_bZEnable	= false;			// Zバッファの使用状況
	m_bLight	= false;			// ライティング状況
	m_nTextureID = 0;				// テクスチャインデックス
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObject3D::CObject3D(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ワールドマトリックス
	m_pVtxBuff		= NULL;			// 頂点バッファへのポインタ
	m_pPosGapBuff	= NULL;			// 座標のずれバッファ
	m_pos		= VEC3_ZERO;		// 位置
	m_rot		= VEC3_ZERO;		// 向き
	m_size		= VEC3_ZERO;		// 大きさ
	m_col		= XCOL_WHITE;		// 色
	m_origin	= ORIGIN_CENTER;	// 原点
	m_cull		= D3DCULL_CCW;		// カリング状況
	m_func		= D3DCMP_ALWAYS;	// Zテスト設定
	m_bZEnable	= false;			// Zバッファの使用状況
	m_bLight	= false;			// ライティング状況
	m_nTextureID = 0;				// テクスチャインデックス
}

//============================================================
//	デストラクタ
//============================================================
CObject3D::~CObject3D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObject3D::Init(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// メンバ変数を初期化
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ワールドマトリックス
	m_pVtxBuff		= NULL;			// 頂点バッファへのポインタ
	m_pPosGapBuff	= NULL;			// 座標のずれバッファ
	m_pos		= VEC3_ZERO;		// 位置
	m_rot		= VEC3_ZERO;		// 向き
	m_size		= VEC3_ZERO;		// 大きさ
	m_col		= XCOL_WHITE;		// 色
	m_origin	= ORIGIN_CENTER;	// 原点
	m_func		= D3DCMP_LESSEQUAL;	// Zテスト設定
	m_bZEnable	= true;				// Zバッファの使用状況
	m_bLight	= true;				// ライティング状況
	m_nTextureID = NONE_IDX;		// テクスチャインデックス

	if (m_pVtxBuff == NULL)
	{ // 非使用中の場合

		// 頂点バッファの生成
		if (FAILED(pDevice->CreateVertexBuffer
		( // 引数
			sizeof(VERTEX_3D) * MAX_VERTEX,	// 必要頂点数
			D3DUSAGE_WRITEONLY,				// 使用方法
			FVF_VERTEX_3D,					// 頂点フォーマット
			D3DPOOL_MANAGED,				// メモリの指定
			&m_pVtxBuff,					// 頂点バッファへのポインタ
			NULL
		)))
		{ // 頂点バッファの生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// 使用中

	if (m_pPosGapBuff == NULL)
	{ // 非使用中の場合

		// 座標のずれバッファのメモリ確保
		m_pPosGapBuff = new D3DXVECTOR3[MAX_VERTEX];

		if (m_pPosGapBuff != NULL)
		{ // 確保に成功した場合

			// メモリクリア
			memset(m_pPosGapBuff, 0, sizeof(D3DXVECTOR3) * MAX_VERTEX);
		}
		else { assert(false); return E_FAIL; }	// 確保失敗
	}
	else { assert(false); return E_FAIL; }	// 使用中

	// 頂点情報の設定
	SetVtx();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObject3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{ // 頂点バッファが使用中の場合

		// メモリ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 座標のずれバッファの破棄
	if (m_pPosGapBuff != NULL)
	{ // 座標のずれバッファが使用中の場合

		// メモリ開放
		delete[] m_pPosGapBuff;
		m_pPosGapBuff = NULL;
	}

	// オブジェクト3Dを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObject3D::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CObject3D::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// テクスチャへのポインタ

	// ポリゴンの表示状態を設定
	pDevice->SetRenderState(D3DRS_CULLMODE, m_cull);

	// ライティングを設定する
	pDevice->SetRenderState(D3DRS_LIGHTING, m_bLight);

	// Zテストを設定する
	pDevice->SetRenderState(D3DRS_ZFUNC, m_func);				// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, m_bZEnable);	// Zバッファ更新の有効 / 無効の設定

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ポリゴンの表面のみを表示状態にする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);		// Zバッファ更新の有効 / 無効の設定
}

//============================================================
//	生成処理
//============================================================
CObject3D *CObject3D::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rSize,	// 大きさ
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXCOLOR& rCol,		// 色
	const EOrigin origin,		// 原点
	const D3DCULL cull,			// カリング状況
	const bool bLight,			// ライティング状況
	const D3DCMPFUNC func,		// Zテスト設定
	const bool bZEnable			// Zバッファの使用状況
)
{
	// ポインタを宣言
	CObject3D *pObject3D = NULL;	// オブジェクト3D生成用

	if (pObject3D == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pObject3D = new CObject3D;	// オブジェクト3D
	}
	else { assert(false); return NULL; }	// 使用中

	if (pObject3D != NULL)
	{ // 確保に成功している場合

		// オブジェクト3Dの初期化
		if (FAILED(pObject3D->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObject3D;
			pObject3D = NULL;

			// 失敗を返す
			return NULL;
		}

		// 原点を設定
		pObject3D->SetOrigin(origin);

		// 位置を設定
		pObject3D->SetVec3Position(rPos);

		// 向きを設定
		pObject3D->SetVec3Rotation(rRot);

		// 大きさを設定
		pObject3D->SetVec3Sizing(rSize);

		// 色を設定
		pObject3D->SetColor(rCol);

		// カリングを設定
		pObject3D->SetCulling(cull);

		// ライティングを設定
		pObject3D->SetLighting(bLight);

		// Zテストを設定
		pObject3D->SetFunc(func);

		// Zバッファの使用状況を設定
		pObject3D->SetZEnable(bZEnable);

		// 確保したアドレスを返す
		return pObject3D;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	テクスチャ割当処理 (インデックス)
//============================================================
void CObject3D::BindTexture(const int nTextureID)
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
void CObject3D::BindTexture(const char *pTexturePass)
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
void CObject3D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObject3D::GetVec3Position(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	向きの設定処理
//============================================================
void CObject3D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 引数の向きを設定
	m_rot = rRot;

	// 向きの正規化
	useful::Vec3NormalizeRot(m_rot);

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CObject3D::GetVec3Rotation(void) const
{
	// 向きを返す
	return m_rot;
}

//============================================================
//	大きさの設定処理
//============================================================
void CObject3D::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// 引数の大きさを代入
	m_size = rSize;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	大きさ取得処理
//============================================================
D3DXVECTOR3 CObject3D::GetVec3Sizing(void) const
{
	// 大きさを返す
	return m_size;
}

//============================================================
//	色の設定処理
//============================================================
void CObject3D::SetColor(const D3DXCOLOR& rCol)
{
	// 引数の色を代入
	m_col = rCol;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CObject3D::GetColor(void) const
{
	// 色を返す
	return m_col;
}

//============================================================
//	原点の設定処理
//============================================================
void CObject3D::SetOrigin(const EOrigin origin)
{
	// 引数の原点を設定
	m_origin = origin;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	原点取得処理
//============================================================
CObject3D::EOrigin CObject3D::GetOrigin(void) const
{
	// 原点を返す
	return m_origin;
}

//============================================================
//	カリング設定処理
//============================================================
void CObject3D::SetCulling(const D3DCULL cull)
{
	// 引数のカリング状況を設定
	m_cull = cull;
}

//============================================================
//	カリング取得処理
//============================================================
D3DCULL CObject3D::GetCulling(void) const
{
	// カリング状況を返す
	return m_cull;
}

//============================================================
//	ライティングの設定処理
//============================================================
void CObject3D::SetLighting(const bool bLight)
{
	// 引数のライティング状況を設定
	m_bLight = bLight;
}

//============================================================
//	ライティング取得処理
//============================================================
bool CObject3D::IsLighting(void) const
{
	// ライティング状況を返す
	return m_bLight;
}

//============================================================
//	Zテストの設定処理
//============================================================
void CObject3D::SetFunc(const D3DCMPFUNC func)
{
	// 引数のZテストの設定を設定
	m_func = func;
}

//============================================================
//	Zテスト取得処理
//============================================================
D3DCMPFUNC CObject3D::GetFunc(void) const
{
	// Zテストの設定を返す
	return m_func;
}

//============================================================
//	Zバッファの使用状況の設定処理
//============================================================
void CObject3D::SetZEnable(const bool bEnable)
{
	// 引数のZバッファの使用状況を設定
	m_bZEnable = bEnable;
}

//============================================================
//	Zバッファの使用状況取得処理
//============================================================
bool CObject3D::IsZEnable(void) const
{
	// Zバッファの使用状況を返す
	return m_bZEnable;
}

//============================================================
//	頂点位置の設定処理
//============================================================
void CObject3D::SetVertexPosition(const int nID, const D3DXVECTOR3& rPos)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{ // 使用中の場合

		if (nID < MAX_VERTEX)
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

	// 法線の設定・正規化
	NormalizeNormal();
}

//============================================================
//	頂点位置取得処理
//============================================================
D3DXVECTOR3 CObject3D::GetVertexPosition(const int nID)
{
	// 変数を宣言
	D3DXVECTOR3 pos;	// 頂点座標の代入用

	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{ // 使用中の場合

		if (nID < MAX_VERTEX)
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
//	座標のずれの設定処理
//============================================================
void CObject3D::SetGapPosition(const int nID, const D3DXVECTOR3& rPos)
{
	if (m_pPosGapBuff != NULL)
	{ // 使用中の場合

		if (nID < MAX_VERTEX)
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
D3DXVECTOR3 CObject3D::GetGapPosition(const int nID)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 頂点のずれの代入用

	if (m_pPosGapBuff != NULL)
	{ // 使用中の場合

		if (nID < MAX_VERTEX)
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
//	ポリゴンの着地取得処理
//============================================================
float CObject3D::GetPositionHeight(const D3DXVECTOR3&rPos)
{
	// 変数を宣言
	D3DXVECTOR3 nor;	// 法線ベクトル

	// 変数配列を宣言
	D3DXVECTOR3 aVtxPos[MAX_VERTEX];	// ポリゴンの頂点座標 [※] 0：左上 1：右上 2：左下 3：右下

	for (int nCnt = 0; nCnt < MAX_VERTEX; nCnt++)
	{ // 頂点数分繰り返す

		// 変数を宣言
		D3DXMATRIX mtxWorld, mtxRot, mtxTrans;		// 計算用マトリックス
		D3DXVECTOR3 pos = GetVertexPosition(nCnt);	// 頂点座標
		
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// 頂点位置を反映
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// ポリゴン向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// ポリゴン位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// 計算したマトリックスから座標を設定
		aVtxPos[nCnt] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	}

	if (collision::TriangleOuterPillar(aVtxPos[0], aVtxPos[1], aVtxPos[2], rPos))
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

	// 着地範囲外の場合現在のy座標を返す
	return rPos.y;
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObject3D::SetVtx(void)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_origin)
	{ // 原点ごとの処理
	case ORIGIN_CENTER:	// 中央

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f,  m_size.y * 0.5f,  m_size.z * 0.5f);
		pVtx[1].pos = D3DXVECTOR3( m_size.x * 0.5f,  m_size.y * 0.5f,  m_size.z * 0.5f);
		pVtx[2].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y * 0.5f, -m_size.z * 0.5f);
		pVtx[3].pos = D3DXVECTOR3( m_size.x * 0.5f, -m_size.y * 0.5f, -m_size.z * 0.5f);

		break;

	case ORIGIN_DOWN:	// 下

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f, m_size.y,  m_size.z * 0.5f);
		pVtx[1].pos = D3DXVECTOR3( m_size.x * 0.5f, m_size.y,  m_size.z * 0.5f);
		pVtx[2].pos = D3DXVECTOR3(-m_size.x * 0.5f,     0.0f, -m_size.z * 0.5f);
		pVtx[3].pos = D3DXVECTOR3( m_size.x * 0.5f,     0.0f, -m_size.z * 0.5f);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 頂点からのずれ量を加算
	for (int nCntVtx = 0; nCntVtx < MAX_VERTEX; nCntVtx++)
	{ // 頂点数分繰り返す

		pVtx[nCntVtx].pos += m_pPosGapBuff[nCntVtx];
	}

	// 法線の設定・正規化
	NormalizeNormal();

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================================================
//	法線の正規化処理
//============================================================
void CObject3D::NormalizeNormal(void)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 法線の正規化
	useful::NormalizeNormal
	( // 引数
		pVtx[1].pos,	// 左位置
		pVtx[0].pos,	// 中心位置
		pVtx[2].pos,	// 右位置
		pVtx[0].nor		// 法線
	);

	// 法線の正規化
	useful::NormalizeNormal
	( // 引数
		pVtx[2].pos,	// 左位置
		pVtx[3].pos,	// 中心位置
		pVtx[1].pos,	// 右位置
		pVtx[3].nor		// 法線
	);

	// 法線ベクトルの設定
	pVtx[1].nor = (pVtx[0].nor + pVtx[3].nor) / 2;
	pVtx[2].nor = (pVtx[0].nor + pVtx[3].nor) / 2;

	// 法線を正規化
	D3DXVec3Normalize(&pVtx[1].nor, &pVtx[1].nor);
	D3DXVec3Normalize(&pVtx[2].nor, &pVtx[2].nor);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
