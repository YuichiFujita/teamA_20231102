//============================================================
//
//	オブジェクトビルボード処理 [objectBillboard.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectBillboard.h"
#include "manager.h"
#include "renderer.h"
#include "object3D.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_VERTEX	(4)	// 頂点数

//************************************************************
//	子クラス [CObjectBillboard] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectBillboard::CObjectBillboard()
{
	// メンバ変数をクリア
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ワールドマトリックス
	m_pVtxBuff = NULL;			// 頂点バッファへのポインタ
	m_pos      = VEC3_ZERO;		// 位置
	m_rot      = VEC3_ZERO;		// 向き
	m_size     = VEC3_ZERO;		// 大きさ
	m_col      = XCOL_WHITE;	// 色
	m_origin   = ORIGIN_CENTER;	// 原点
	m_rotate   = ROTATE_NORMAL;	// 回転
	m_func     = D3DCMP_ALWAYS;	// Zテスト設定
	m_bZEnable = false;			// Zバッファの使用状況
	m_fAngle   = 0.0f;			// 対角線の角度
	m_fLength  = 0.0f;			// 対角線の長さ
	m_nTextureID = 0;			// テクスチャインデックス
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObjectBillboard::CObjectBillboard(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ワールドマトリックス
	m_pVtxBuff = NULL;			// 頂点バッファへのポインタ
	m_pos      = VEC3_ZERO;		// 位置
	m_rot      = VEC3_ZERO;		// 向き
	m_size     = VEC3_ZERO;		// 大きさ
	m_col      = XCOL_WHITE;	// 色
	m_origin   = ORIGIN_CENTER;	// 原点
	m_rotate   = ROTATE_NORMAL;	// 回転
	m_func     = D3DCMP_ALWAYS;	// Zテスト設定
	m_bZEnable = false;			// Zバッファの使用状況
	m_fAngle   = 0.0f;			// 対角線の角度
	m_fLength  = 0.0f;			// 対角線の長さ
	m_nTextureID = 0;			// テクスチャインデックス
}

//============================================================
//	デストラクタ
//============================================================
CObjectBillboard::~CObjectBillboard()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectBillboard::Init(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// メンバ変数を初期化
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ワールドマトリックス
	m_pVtxBuff = NULL;			// 頂点バッファへのポインタ
	m_pos      = VEC3_ZERO;		// 位置
	m_rot      = VEC3_ZERO;		// 向き
	m_size     = VEC3_ZERO;		// 大きさ
	m_col      = XCOL_WHITE;	// 色
	m_origin   = ORIGIN_CENTER;	// 原点
	m_rotate   = ROTATE_NORMAL;	// 回転
	m_func     = D3DCMP_ALWAYS;	// Zテスト設定
	m_bZEnable = false;			// Zバッファの使用状況
	m_fAngle   = 0.0f;			// 対角線の角度
	m_fLength  = 0.0f;			// 対角線の長さ
	m_nTextureID = NONE_IDX;	// テクスチャインデックス

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

	// 頂点情報の設定
	SetVtx();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectBillboard::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{ // 頂点バッファが使用中の場合

		// メモリ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクトビルボードを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectBillboard::Update(void)
{
	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	描画処理
//============================================================
void CObjectBillboard::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxView;		// ビューマトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// テクスチャへのポインタ

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテストを設定する
	pDevice->SetRenderState(D3DRS_ZFUNC, m_func);				// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, m_bZEnable);	// Zバッファ更新の有効 / 無効の設定

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);	// 逆行列を求める

	// マトリックスのワールド座標を原点にする
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	switch (m_rotate)
	{ // 回転ごとの処理
	case ROTATE_NORMAL:		// 通常回転

		// 無し

		break;

	case ROTATE_LATERAL:	// 横回転

		m_mtxWorld._21 = 0.0f;
		m_mtxWorld._23 = 0.0f;
		m_mtxWorld._24 = 0.0f;

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

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

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);		// Zバッファ更新の有効 / 無効の設定
}

//============================================================
//	生成処理
//============================================================
CObjectBillboard *CObjectBillboard::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rSize,	// 大きさ
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXCOLOR& rCol,		// 色
	const EOrigin origin,		// 原点
	const ERotate rotate,		// 回転
	const D3DCMPFUNC func,		// Zテスト設定
	const bool bZEnable			// Zバッファの使用状況
)
{
	// ポインタを宣言
	CObjectBillboard *pObjectBillboard = NULL;	// オブジェクトビルボード生成用

	if (pObjectBillboard == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pObjectBillboard = new CObjectBillboard;	// オブジェクトビルボード
	}
	else { assert(false); return NULL; }	// 使用中

	if (pObjectBillboard != NULL)
	{ // 確保に成功している場合

		// オブジェクトビルボードの初期化
		if (FAILED(pObjectBillboard->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObjectBillboard;
			pObjectBillboard = NULL;

			// 失敗を返す
			return NULL;
		}

		// 原点を設定
		pObjectBillboard->SetOrigin(origin);

		// 位置を設定
		pObjectBillboard->SetVec3Position(rPos);

		// 向きを設定
		pObjectBillboard->SetVec3Rotation(rRot);

		// 大きさを設定
		pObjectBillboard->SetVec3Sizing(rSize);

		// 色を設定
		pObjectBillboard->SetColor(rCol);

		// 回転を設定
		pObjectBillboard->SetRotate(rotate);

		// Zテストを設定
		pObjectBillboard->SetFunc(func);

		// Zバッファの使用状況を設定
		pObjectBillboard->SetZEnable(bZEnable);

		// 確保したアドレスを返す
		return pObjectBillboard;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	テクスチャ割当処理 (インデックス)
//============================================================
void CObjectBillboard::BindTexture(const int nTextureID)
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
void CObjectBillboard::BindTexture(const char *pTexturePass)
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
void CObjectBillboard::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObjectBillboard::GetVec3Position(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	向きの設定処理
//============================================================
void CObjectBillboard::SetVec3Rotation(const D3DXVECTOR3& rRot)
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
D3DXVECTOR3 CObjectBillboard::GetVec3Rotation(void) const
{
	// 向きを返す
	return m_rot;
}

//============================================================
//	大きさの設定処理
//============================================================
void CObjectBillboard::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// 引数の大きさを代入
	m_size = rSize;

	switch (m_origin)
	{ // 原点ごとの処理
	case ORIGIN_CENTER:	// 中央

		// 対角線の角度を求める
		m_fAngle = atan2f(m_size.x, m_size.y);

		// 対角線の長さを求める
		m_fLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) * 0.5f;

		break;

	case ORIGIN_DOWN:	// 下

		// 対角線の角度を求める
		m_fAngle = atan2f(m_size.x, m_size.y);

		// 対角線の長さを求める
		m_fLength = sqrtf(m_size.x * m_size.x + (m_size.y * 2.0f) * (m_size.y * 2.0f)) * 0.5f;

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	大きさ取得処理
//============================================================
D3DXVECTOR3 CObjectBillboard::GetVec3Sizing(void) const
{
	// 大きさを返す
	return m_size;
}

//============================================================
//	色の設定処理
//============================================================
void CObjectBillboard::SetColor(const D3DXCOLOR& rCol)
{
	// 引数の色を代入
	m_col = rCol;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CObjectBillboard::GetColor(void) const
{
	// 色を返す
	return m_col;
}

//============================================================
//	マトリックスポインタ取得処理
//============================================================
D3DXMATRIX *CObjectBillboard::GetPtrMtxWorld(void)
{
	// ワールドマトリックスのポインタを返す
	return &m_mtxWorld;
}

//============================================================
//	マトリックス取得処理
//============================================================
D3DXMATRIX CObjectBillboard::GetMtxWorld(void) const
{
	// ワールドマトリックスを返す
	return m_mtxWorld;
}

//============================================================
//	原点の設定処理
//============================================================
void CObjectBillboard::SetOrigin(const EOrigin origin)
{
	// 引数の原点を設定
	m_origin = origin;

	switch (m_origin)
	{ // 原点ごとの処理
	case ORIGIN_CENTER:	// 中央

		// 対角線の角度を求める
		m_fAngle = atan2f(m_size.x, m_size.y);

		// 対角線の長さを求める
		m_fLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) * 0.5f;

		break;

	case ORIGIN_DOWN:	// 下

		// 対角線の角度を求める
		m_fAngle = atan2f(m_size.x, m_size.y);

		// 対角線の長さを求める
		m_fLength = sqrtf(m_size.x * m_size.x + (m_size.y * 2.0f) * (m_size.y * 2.0f)) * 0.5f;

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	原点取得処理
//============================================================
CObjectBillboard::EOrigin CObjectBillboard::GetOrigin(void) const
{
	// 原点を返す
	return m_origin;
}

//============================================================
//	回転の設定処理
//============================================================
void CObjectBillboard::SetRotate(const ERotate rotate)
{
	// 引数の回転を設定
	m_rotate = rotate;
}

//============================================================
//	回転取得処理
//============================================================
CObjectBillboard::ERotate CObjectBillboard::GetRotate(void) const
{
	// 回転を返す
	return m_rotate;
}

//============================================================
//	Zテストの設定処理
//============================================================
void CObjectBillboard::SetFunc(const D3DCMPFUNC func)
{
	// 引数のZテストの設定を設定
	m_func = func;
}

//============================================================
//	Zテスト取得処理
//============================================================
D3DCMPFUNC CObjectBillboard::GetFunc(void) const
{
	// Zテストの設定を返す
	return m_func;
}

//============================================================
//	Zバッファの使用状況の設定処理
//============================================================
void CObjectBillboard::SetZEnable(const bool bEnable)
{
	// 引数のZバッファの使用状況を設定
	m_bZEnable = bEnable;
}

//============================================================
//	Zバッファの使用状況取得処理
//============================================================
bool CObjectBillboard::IsZEnable(void) const
{
	// Zバッファの使用状況を返す
	return m_bZEnable;
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObjectBillboard::SetVtx(void)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_origin)
	{ // 原点ごとの処理
	case ORIGIN_CENTER:	// 中央

		// 頂点座標の設定
		pVtx[0].pos.x = 0.0f + sinf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
		pVtx[0].pos.y = 0.0f - cosf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = 0.0f + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.y = 0.0f - cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = 0.0f + sinf(m_rot.z - m_fAngle) * m_fLength;
		pVtx[2].pos.y = 0.0f - cosf(m_rot.z - m_fAngle) * m_fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = 0.0f + sinf(m_rot.z + m_fAngle) * m_fLength;
		pVtx[3].pos.y = 0.0f - cosf(m_rot.z + m_fAngle) * m_fLength;
		pVtx[3].pos.z = 0.0f;

		break;

	case ORIGIN_DOWN:	// 下

		// 頂点座標の設定
		pVtx[0].pos.x = 0.0f + sinf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
		pVtx[0].pos.y = 0.0f - cosf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = 0.0f + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.y = 0.0f - cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = 0.0f + sinf(m_rot.z - (D3DX_PI * 0.5f)) * m_size.x;
		pVtx[2].pos.y = 0.0f - cosf(m_rot.z - (D3DX_PI * 0.5f)) * m_size.x;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = 0.0f + sinf(m_rot.z + (D3DX_PI * 0.5f)) * m_size.x;
		pVtx[3].pos.y = 0.0f - cosf(m_rot.z + (D3DX_PI * 0.5f)) * m_size.x;
		pVtx[3].pos.z = 0.0f;

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
