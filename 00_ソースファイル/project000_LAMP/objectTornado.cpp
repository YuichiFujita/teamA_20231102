//============================================================
//
//	オブジェクト竜巻処理 [objectTornado.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectTornado.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_OFFSET	(2)	// オフセットの数

//************************************************************
//	子クラス [CObjectTornado] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectTornado::CObjectTornado()
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;	// 頂点バッファ
	m_nNumVtx = 0;		// 必要頂点数
	m_nNumAround = 0;	// 渦の周回数
	m_nPattern = 0;		// 渦の分割数
	m_nTextureID = 0;	// テクスチャインデックス
	memset(&m_tornado, 0, sizeof(m_tornado));	// 竜巻の情報
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObjectTornado::CObjectTornado(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;	// 頂点バッファ
	m_nNumVtx = 0;		// 必要頂点数
	m_nNumAround = 0;	// 渦の周回数
	m_nPattern = 0;		// 渦の分割数
	m_nTextureID = 0;	// テクスチャインデックス
	memset(&m_tornado, 0, sizeof(m_tornado));	// 竜巻の情報
}

//============================================================
//	デストラクタ
//============================================================
CObjectTornado::~CObjectTornado()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectTornado::Init(void)
{
	// メンバ変数を初期化
	m_pVtxBuff = NULL;	// 頂点バッファ
	m_nNumVtx = 0;		// 必要頂点数
	m_nNumAround = 0;	// 渦の周回数
	m_nPattern = 0;		// 渦の分割数
	m_nTextureID = NONE_IDX;	// テクスチャインデックス

	// 竜巻の情報を初期化
	m_tornado.pos     = VEC3_ZERO;		// 位置
	m_tornado.rot     = VEC3_ZERO;		// 向き
	m_tornado.direRot = VEC3_ZERO;		// 成長向き
	m_tornado.col     = XCOL_WHITE;		// 色
	m_tornado.pMtxParent  = NULL;		// 親のマトリックス
	m_tornado.fMoveRot    = 0.0f;		// 向きの変更量
	m_tornado.fThickness  = 0.0f;		// ポリゴンの太さ
	m_tornado.fOuterPlus  = 0.0f;		// ポリゴン外周の y座標加算量
	m_tornado.fSetWidth   = 0.0f;		// 生成時の横ずれ量
	m_tornado.fSetAlpha   = 0.0f;		// 生成時の透明度
	m_tornado.fAddWidth   = 0.0f;		// 横ずれの加算量
	m_tornado.fAddHeight  = 0.0f;		// 縦ずれの加算量
	m_tornado.fSubAlpha   = 0.0f;		// 透明度の減算量
	m_tornado.fGrowWidth  = 0.0f;		// 横ずれの成長量
	m_tornado.fGrowHeight = 0.0f;		// 縦ずれの成長量
	m_tornado.fGrowAlpha  = 0.0f;		// 透明度の成長量
	m_tornado.cull   = D3DCULL_NONE;	// カリング状況
	m_tornado.bLight = false;			// ライティング状況

	// 渦を設定
	if (FAILED(SetVortex(1, 1)))
	{ // 渦の設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectTornado::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{ // 頂点バッファが使用中の場合

		// メモリ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクト竜巻を破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectTornado::Update(void)
{
	// 竜巻の向きを変更
	m_tornado.rot.y -= m_tornado.fMoveRot;

	// 向きを正規化
	useful::NormalizeRot(m_tornado.rot.y);

	// 竜巻を成長させる
	m_tornado.fSetWidth  += m_tornado.fGrowWidth * m_nNumVtx;	// 原点からの横ずれ量を加算
	m_tornado.fAddWidth  += m_tornado.fGrowWidth;	// 竜巻の横の広がりを加算
	m_tornado.fAddHeight += m_tornado.fGrowHeight;	// 竜巻の縦の広がりを加算
	m_tornado.fSetAlpha  -= m_tornado.fGrowAlpha;	// 竜巻の透明度を減算 (透明にしていく)

	// 頂点情報の設定
	SetVtx();

	if (m_tornado.fSetAlpha <= 0.0f)
	{ // 竜巻が透明になった場合

		// 竜巻オブジェクトの終了
		Uninit();

		// 処理を抜ける
		return;
	}
}

//============================================================
//	描画処理
//============================================================
void CObjectTornado::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxOrigin;			// 発生源のマトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// テクスチャへのポインタ

	// ポリゴンの表示状態を設定
	pDevice->SetRenderState(D3DRS_CULLMODE, m_tornado.cull);

	// ライティングを設定
	pDevice->SetRenderState(D3DRS_LIGHTING, m_tornado.bLight);

	// αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストの有効 / 無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// αテストの設定

	//--------------------------------------------------------
	//	発生源のマトリックスを求める
	//--------------------------------------------------------
	// 発生源のマトリックスの初期化
	D3DXMatrixIdentity(&mtxOrigin);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_tornado.direRot.y, m_tornado.direRot.x, m_tornado.direRot.z);
	D3DXMatrixMultiply(&mtxOrigin, &mtxOrigin, &mtxRot);	// 成長向き

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_tornado.pos.x, m_tornado.pos.y, m_tornado.pos.z);
	D3DXMatrixMultiply(&mtxOrigin, &mtxOrigin, &mtxTrans);	// 発生位置

	//--------------------------------------------------------
	//	ワールドマトリックスを求める
	//--------------------------------------------------------
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_tornado.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_tornado.rot.y, m_tornado.rot.x, m_tornado.rot.z);
	D3DXMatrixMultiply(&m_tornado.mtxWorld, &m_tornado.mtxWorld, &mtxRot);	// 回転量

	//--------------------------------------------------------
	//	マトリックスを掛け合わせる
	//--------------------------------------------------------
	if (m_tornado.pMtxParent != NULL)	// TODO：親マトリックスがNULLじゃなくなったときに判断できない
	{ // 親のマトリックスが存在する場合

		// 親のマトリックスと掛け合わせる
		D3DXMatrixMultiply(&mtxOrigin, &mtxOrigin, m_tornado.pMtxParent);
	}

	// 発生源のマトリックスと掛け合わせる
	D3DXMatrixMultiply(&m_tornado.mtxWorld, &m_tornado.mtxWorld, &mtxOrigin);

	//--------------------------------------------------------
	//	竜巻を描画
	//--------------------------------------------------------
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_tornado.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);

	// ポリゴンの表面のみを表示状態にする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストの有効 / 無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// αテストの設定
}

//============================================================
//	位置の設定処理
//============================================================
void CObjectTornado::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_tornado.pos = rPos;
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObjectTornado::GetVec3Position(void) const
{
	// 位置を返す
	return m_tornado.pos;
}

//============================================================
//	色の設定処理
//============================================================
void CObjectTornado::SetColor(const D3DXCOLOR& rCol)
{
	// 引数の色を設定
	m_tornado.col = rCol;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CObjectTornado::GetColor(void) const
{
	// 色を返す
	return m_tornado.col;
}

//============================================================
//	生成処理
//============================================================
CObjectTornado *CObjectTornado::Create
(
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rDireRot,	// 成長向き
	const D3DXCOLOR& rCol,			// 色
	D3DXMATRIX *pMtxParent,			// 親のマトリックス
	const int   nNumAround,			// 渦の周回数
	const int   nPattern,			// 渦の分割数
	const float fMoveRot,			// 向きの変更量
	const float fThickness,			// ポリゴンの太さ
	const float fOuterPlus,			// ポリゴン外周の y座標加算量
	const float fSetWidth,			// 生成時の横ずれ量
	const float fSetAlpha,			// 生成時の透明度
	const float fAddWidth,			// 横ずれの加算量
	const float fAddHeight,			// 縦ずれの加算量
	const float fSubAlpha,			// 透明度の減算量
	const float fGrowWidth,			// 横ずれの成長量
	const float fGrowHeight,		// 縦ずれの成長量
	const float fGrowAlpha,			// 透明度の成長量
	const D3DCULL cull,				// カリング状況
	const bool bLight				// ライティング状況
)
{
	// ポインタを宣言
	CObjectTornado *pObjectTornado = NULL;	// オブジェクト竜巻生成用

	if (pObjectTornado == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pObjectTornado = new CObjectTornado;	// オブジェクト竜巻
	}
	else { assert(false); return NULL; }	// 使用中

	if (pObjectTornado != NULL)
	{ // 確保に成功している場合

		// オブジェクト竜巻の初期化
		if (FAILED(pObjectTornado->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObjectTornado;
			pObjectTornado = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pObjectTornado->SetVec3Position(rPos);

		// 向きを設定
		pObjectTornado->SetDirectionRotation(rDireRot);

		// 色を設定
		pObjectTornado->SetColor(rCol);

		// 親のマトリックスを設定
		pObjectTornado->SetMatrixParent(pMtxParent);

		// ポリゴンの太さを設定
		pObjectTornado->SetThickness(fThickness);

		// ポリゴン外周のy座標加算量を設定
		pObjectTornado->SetOuterPlus(fOuterPlus);

		// 生成時の横ずれ量を設定
		pObjectTornado->SetWidth(fSetWidth);

		// 生成時の透明度を設定
		pObjectTornado->SetAlpha(fSetAlpha);

		// 横ずれの加算量を設定
		pObjectTornado->SetAddWidth(fAddWidth);

		// 縦ずれの加算量を設定
		pObjectTornado->SetAddHeight(fAddHeight);

		// 透明度の減算量を設定
		pObjectTornado->SetSubAlpha(fSubAlpha);

		// カリングを設定
		pObjectTornado->SetCulling(cull);

		// ライティングを設定
		pObjectTornado->SetLighting(bLight);

		// 成長性を設定
		pObjectTornado->SetGrow(fMoveRot, fGrowWidth, fGrowHeight, fGrowAlpha);

		// 渦を設定
		if (FAILED(pObjectTornado->SetVortex(nNumAround, nPattern)))
		{ // 渦の設定に失敗した場合

			// メモリ開放
			delete pObjectTornado;
			pObjectTornado = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pObjectTornado;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	成長の設定処理
//============================================================
void CObjectTornado::SetGrow(float fMoveRot, float fGrowWidth, float fGrowHeight, float fGrowAlpha)
{
	// 引数の情報を設定
	m_tornado.fMoveRot		= fMoveRot;		// 向きの変更量
	m_tornado.fGrowWidth	= fGrowWidth;	// 横ずれの成長量
	m_tornado.fGrowHeight	= fGrowHeight;	// 縦ずれの成長量
	m_tornado.fGrowAlpha	= fGrowAlpha;	// 透明度の成長量
}

//============================================================
//	渦の設定処理
//============================================================
HRESULT CObjectTornado::SetVortex(const int nNumAround, const int nPattern)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 引数の渦を設定
	m_nNumAround = nNumAround;
	m_nPattern = nPattern;

	// 必要頂点数を求める
	m_nNumVtx = m_nNumAround * m_nPattern * MAX_OFFSET;

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

	// 頂点情報の設定
	SetVtx();

	// 成功を返す
	return S_OK;
}

//============================================================
//	親マトリックス削除処理
//============================================================
void CObjectTornado::DeleteMatrixParent(void)
{
	// 親マトリックスをNULLにする
	m_tornado.pMtxParent = NULL;
}

//============================================================
//	親のマトリックスの設定処理
//============================================================
void CObjectTornado::SetMatrixParent(D3DXMATRIX *pMtxParent)
{
	// 引数の親マトリックスを設定
	m_tornado.pMtxParent = pMtxParent;
}

//============================================================
//	成長向きの設定処理
//============================================================
void CObjectTornado::SetDirectionRotation(const D3DXVECTOR3& rRot)
{
	// 引数の成長向きを設定
	m_tornado.direRot = rRot;

	// 成長向きの正規化
	useful::Vec3NormalizeRot(m_tornado.direRot);
}

//============================================================
//	成長向き取得処理
//============================================================
D3DXVECTOR3 CObjectTornado::GetDirectionRotation(void) const
{
	// 成長向きを返す
	return m_tornado.direRot;
}

//============================================================
//	ポリゴンの太さの設定処理
//============================================================
void CObjectTornado::SetThickness(const float fThickness)
{
	// 引数のポリゴンの太さを設定
	m_tornado.fThickness = fThickness;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	ポリゴンの太さ取得処理
//============================================================
float CObjectTornado::GetThickness(void) const
{
	// ポリゴンの太さを返す
	return m_tornado.fThickness;
}

//============================================================
//	ポリゴン外周のy座標加算量の設定処理
//============================================================
void CObjectTornado::SetOuterPlus(const float fOuterPlus)
{
	// 引数のポリゴン外周のy座標加算量を設定
	m_tornado.fOuterPlus = fOuterPlus;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	ポリゴン外周のy座標加算量の取得処理
//============================================================
float CObjectTornado::GetOuterPlus(void) const
{
	// ポリゴン外周のy座標加算量を返す
	return m_tornado.fOuterPlus;
}

//============================================================
//	生成時の横ずれ量の設定処理
//============================================================
void CObjectTornado::SetWidth(const float fSetWidth)
{
	// 引数の生成時の横ずれ量を設定
	m_tornado.fSetWidth = fSetWidth;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	生成時の横ずれ量取得処理
//============================================================
float CObjectTornado::GetWidth(void) const
{
	// 生成時の横ずれ量を返す
	return m_tornado.fSetWidth;
}

//============================================================
//	生成時の透明度の設定処理
//============================================================
void CObjectTornado::SetAlpha(const float fSetAlpha)
{
	// 引数の生成時の透明度を設定
	m_tornado.fSetAlpha = fSetAlpha;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	生成時の透明度取得処理
//============================================================
float CObjectTornado::GetAlpha(void) const
{
	// 生成時の透明度を返す
	return m_tornado.fSetAlpha;
}

//============================================================
//	横ずれの加算量の設定処理
//============================================================
void CObjectTornado::SetAddWidth(const float fAddWidth)
{
	// 引数の横ずれの加算量を設定
	m_tornado.fAddWidth = fAddWidth;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	横ずれの加算量取得処理
//============================================================
float CObjectTornado::GetAddWidth(void) const
{
	// 横ずれの加算量を返す
	return m_tornado.fAddWidth;
}

//============================================================
//	縦ずれの加算量の設定処理
//============================================================
void CObjectTornado::SetAddHeight(const float fAddHeight)
{
	// 引数の縦ずれの加算量を設定
	m_tornado.fAddHeight = fAddHeight;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	縦ずれの加算量取得処理
//============================================================
float CObjectTornado::GetAddHeight(void) const
{
	// 縦ずれの加算量を返す
	return m_tornado.fAddHeight;
}

//============================================================
//	透明度の減算量の設定処理
//============================================================
void CObjectTornado::SetSubAlpha(const float fSubAlpha)
{
	// 引数の透明度の減算量を設定
	m_tornado.fSubAlpha = fSubAlpha;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	透明度の減算量取得処理
//============================================================
float CObjectTornado::GetSubAlpha(void) const
{
	// 透明度の減算量を返す
	return m_tornado.fSubAlpha;
}

//============================================================
//	カリング設定処理
//============================================================
void CObjectTornado::SetCulling(const D3DCULL cull)
{
	// 引数のカリング状況を設定
	m_tornado.cull = cull;
}

//============================================================
//	カリング取得処理
//============================================================
D3DCULL CObjectTornado::GetCulling(void) const
{
	// カリング状況を返す
	return m_tornado.cull;
}

//============================================================
//	ライティング設定処理
//============================================================
void CObjectTornado::SetLighting(const bool bLight)
{
	// 引数のライティング状況を設定
	m_tornado.bLight = bLight;
}

//============================================================
//	ライティング取得処理
//============================================================
bool CObjectTornado::GetLighting(void) const
{
	// ライティング状況を返す
	return m_tornado.bLight;
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObjectTornado::SetVtx(void)
{
	// 変数を宣言
	D3DXVECTOR3 vecPos;	// 竜巻の頂点方向ベクトル
	float fWidth  = m_tornado.fSetWidth;	// 頂点方向の横ずれ量
	float fHeight = 0.0f;					// 頂点方向の縦ずれ量
	float fAlpha  = m_tornado.fSetAlpha;	// 頂点カラーの透明度

	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{ // 竜巻の頂点バッファが使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntWidth = 0; nCntWidth < m_nNumAround; nCntWidth++)
		{ // 渦の周回の総数分繰り返す

			for (int nCntHeight = 0; nCntHeight < m_nPattern; nCntHeight++)
			{ // 渦の分割数分繰り返す

				// 頂点方向ベクトルを求める
				vecPos.x = sinf(nCntHeight * ((D3DX_PI * 2.0f) / m_nPattern)) * 1.0f;
				vecPos.y = 1.0f;
				vecPos.z = cosf(nCntHeight * ((D3DX_PI * 2.0f) / m_nPattern)) * 1.0f;

				for (int nCntSet = 0; nCntSet < MAX_OFFSET; nCntSet++)
				{ // オフセットの総数分繰り返す

					// 頂点座標の設定
					pVtx[0].pos.x = 0.0f + vecPos.x * (fWidth + (nCntSet * m_tornado.fThickness));	// x
					pVtx[0].pos.y = 0.0f + vecPos.y * (fHeight + (nCntSet * m_tornado.fOuterPlus));	// y
					pVtx[0].pos.z = 0.0f + vecPos.z * (fWidth + (nCntSet * m_tornado.fThickness));	// z

					// 法線ベクトルの設定
					pVtx[0].nor = VEC3_ZERO;

					if (fAlpha < 0.0f)
					{ // 透明度が範囲外の場合

						// 透明度を補正
						fAlpha = 0.0f;
					}

					// 頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(m_tornado.col.r, m_tornado.col.g, m_tornado.col.b, fAlpha);

					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2
					( // 引数
						1.0f,			// u
						1.0f * nCntSet	// v
					);

					// 頂点データのポインタを進める
					pVtx += 1;
				}

				// 縦横を広げる
				fWidth  += m_tornado.fAddWidth;
				fHeight += m_tornado.fAddHeight;

				// α値を減算
				fAlpha -= m_tornado.fSubAlpha;
			}
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
	else { assert(false); }	// 非使用中
}
