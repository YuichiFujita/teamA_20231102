//============================================================
//
//	オブジェクトゲージ2D処理 [objectGauge2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectGauge2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_VERTEX	(12)	// 頂点数
#define GAUGE_PRIO	(13)	// ゲージ2Dの優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CObjectGauge2D::mc_apTextureFile[] =	// テクスチャ定数
{
	NULL,	// フレーム無し
	"data\\TEXTURE\\lifeGauge2D000.png",	// 経験値フレーム
};

//************************************************************
//	子クラス [CObjectGauge2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectGauge2D::CObjectGauge2D() : m_nFrame(0)
{
	// メンバ変数をクリア
	m_pVtxBuff	= NULL;			// 頂点バッファへのポインタ
	m_pos		= VEC3_ZERO;	// 位置
	m_sizeGauge	= VEC3_ZERO;	// ゲージ大きさ
	m_sizeFrame	= VEC3_ZERO;	// 枠大きさ
	m_colFront	= XCOL_WHITE;	// 表ゲージ色
	m_colBack	= XCOL_WHITE;	// 裏ゲージ色
	m_state = STATE_NONE;		// 状態
	m_bDrawFrame = false;		// 枠表示状況
	m_fChange = 0.0f;			// ゲージ変動量
	m_fCurrentNumGauge = 0.0f;	// 現在表示値
	m_fAddRight = 0.0f;			// 横幅加算量
	m_nCounterState = 0;		// 状態管理カウンター
	m_nNumGauge = 0;			// 表示値
	m_nMaxNumGauge = 0;			// 表示値の最大値

	for (int nCntGauge = 0; nCntGauge < POLYGON_MAX; nCntGauge++)
	{ // 使用する四角形ポリゴン数分繰り返す

		// テクスチャインデックスをクリア
		m_aTextureID[nCntGauge] = 0;
	}
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObjectGauge2D::CObjectGauge2D(const int nFrame, const CObject::ELabel label, const int nPriority) : CObject(label, nPriority), m_nFrame(nFrame)
{
	// メンバ変数をクリア
	m_pVtxBuff	= NULL;			// 頂点バッファへのポインタ
	m_pos		= VEC3_ZERO;	// 位置
	m_sizeGauge = VEC3_ZERO;	// ゲージ大きさ
	m_sizeFrame = VEC3_ZERO;	// 枠大きさ
	m_colFront	= XCOL_WHITE;	// 表ゲージ色
	m_colBack	= XCOL_WHITE;	// 裏ゲージ色
	m_state = STATE_NONE;		// 状態
	m_bDrawFrame = false;		// 枠表示状況
	m_fChange = 0.0f;			// ゲージ変動量
	m_fCurrentNumGauge = 0.0f;	// 現在表示値
	m_fAddRight = 0.0f;			// 横幅加算量
	m_nCounterState = 0;		// 状態管理カウンター
	m_nNumGauge = 0;			// 表示値
	m_nMaxNumGauge = 0;			// 表示値の最大値

	for (int nCntGauge = 0; nCntGauge < POLYGON_MAX; nCntGauge++)
	{ // 使用する四角形ポリゴン数分繰り返す

		// テクスチャインデックスをクリア
		m_aTextureID[nCntGauge] = 0;
	}
}

//============================================================
//	デストラクタ
//============================================================
CObjectGauge2D::~CObjectGauge2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectGauge2D::Init(void)
{
	// メンバ変数を初期化
	m_pVtxBuff	= NULL;				// 頂点バッファへのポインタ
	m_pos		= VEC3_ZERO;		// 位置
	m_sizeGauge = VEC3_ZERO;		// ゲージ大きさ
	m_sizeFrame = VEC3_ZERO;		// 枠大きさ
	m_colFront	= XCOL_WHITE;		// 表ゲージ色
	m_colBack	= XCOL_WHITE;		// 裏ゲージ色
	m_state = STATE_NONE;			// 状態
	m_bDrawFrame = false;			// 枠表示状況
	m_fChange = 0.0f;				// ゲージ変動量
	m_fCurrentNumGauge = 0.0f;		// 現在表示値
	m_fAddRight = 0.0f;				// 横幅加算量
	m_nCounterState = 0;			// 状態管理カウンター
	m_nNumGauge = 0;				// 表示値
	m_nMaxNumGauge = 0;				// 表示値の最大値

	for (int nCntTexture = 0; nCntTexture < POLYGON_MAX; nCntTexture++)
	{ // 使用する四角形ポリゴン数分繰り返す

		// テクスチャインデックスをクリア
		m_aTextureID[nCntTexture] = NONE_IDX;
	}

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	if (m_pVtxBuff == NULL)
	{ // 非使用中の場合

		// 頂点バッファの生成
		if (FAILED(pDevice->CreateVertexBuffer
		( // 引数
			sizeof(VERTEX_2D) * MAX_VERTEX,	// 必要頂点数
			D3DUSAGE_WRITEONLY,				// 使用方法
			FVF_VERTEX_2D,					// 頂点フォーマット
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
void CObjectGauge2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{ // 頂点バッファが使用中の場合

		// メモリ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクトゲージ2Dを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectGauge2D::Update(void)
{
	// ゲージの設定
	if (m_state == STATE_CHANGE)
	{ // 体力が変動中の場合

		// カウンターを減算
		m_nCounterState--;

		// 現在のゲージに変動量を加算
		m_fCurrentNumGauge += m_fChange;

		// 現在の表示値を補正
		useful::LimitNum(m_fCurrentNumGauge, 0.0f, (float)m_nMaxNumGauge);

		// ゲージの横幅加算量を設定
		m_fAddRight = (m_fCurrentNumGauge * ((m_sizeGauge.x * 2.0f) / (float)m_nMaxNumGauge)) - m_sizeGauge.x;

		// 頂点情報の設定
		SetVtx();

		if (m_nCounterState <= 0)
		{ // カウンターが 0以下になった場合

			// 通常状態にする
			m_state = STATE_NONE;
		}
	}
}

//============================================================
//	描画処理
//============================================================
void CObjectGauge2D::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// テクスチャへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGauge = 0; nCntGauge < POLYGON_MAX; nCntGauge++)
	{ // 使用する四角形ポリゴン数分繰り返す

		if (nCntGauge == POLYGON_FRAME)
		{ // 描画する四角形ポリゴンが枠の場合

			if (m_bDrawFrame)
			{ // 枠を表示する場合

				// テクスチャの設定
				pDevice->SetTexture(0, pTexture->GetTexture(m_aTextureID[nCntGauge]));

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4, 2);
			}
		}
		else
		{ // 描画する四角形ポリゴンが枠以外の場合

			// テクスチャの設定
			pDevice->SetTexture(0, pTexture->GetTexture(m_aTextureID[nCntGauge]));

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4, 2);
		}
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CObjectGauge2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObjectGauge2D::GetVec3Position(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	生成処理
//============================================================
CObjectGauge2D *CObjectGauge2D::Create
(
	const CObject::ELabel label,		// オブジェクトラベル
	const int nMax,					// 最大表示値
	const int nFrame,				// 表示値変動フレーム
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rSizeGauge,	// ゲージ大きさ
	const D3DXCOLOR& rColFront,		// 表ゲージ色
	const D3DXCOLOR& rColBack,		// 裏ゲージ色
	const bool bDrawFrame,			// 枠描画状況
	const EType frameType,			// 枠種類
	const D3DXVECTOR3& rSizeFrame	// 枠大きさ
)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	CObjectGauge2D *pObjectGauge2D = NULL;			// オブジェクトゲージ2D生成用

	if (pObjectGauge2D == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pObjectGauge2D = new CObjectGauge2D(nFrame, label, GAUGE_PRIO);	// オブジェクトゲージ2D
	}
	else { assert(false); return NULL; }	// 使用中

	if (pObjectGauge2D != NULL)
	{ // 確保に成功している場合

		// オブジェクトゲージ2Dの初期化
		if (FAILED(pObjectGauge2D->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObjectGauge2D;
			pObjectGauge2D = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャを登録・割当
		pObjectGauge2D->BindTexture(POLYGON_FRAME, pTexture->Regist(mc_apTextureFile[frameType]));

		// ゲージ最大値を設定
		pObjectGauge2D->SetMaxNum(nMax);

		// 位置を設定
		pObjectGauge2D->SetVec3Position(rPos);

		// 大きさを設定
		pObjectGauge2D->SetScalingGauge(rSizeGauge);	// ゲージ大きさ
		pObjectGauge2D->SetScalingFrame(rSizeFrame);	// 枠大きさ

		// 色を設定
		pObjectGauge2D->SetColorFront(rColFront);	// 表ゲージ色
		pObjectGauge2D->SetColorBack(rColBack);		// 裏ゲージ色

		// 枠の表示状況を設定
		pObjectGauge2D->SetEnableDrawFrame(bDrawFrame);

		// 確保したアドレスを返す
		return pObjectGauge2D;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	ゲージの加算処理
//============================================================
void CObjectGauge2D::AddNum(const int nAdd)
{
	// 現在の表示値を設定
	m_fCurrentNumGauge = (float)m_nNumGauge;

	// 表示値の変動量を求める
	m_fChange = ((float)nAdd / (float)m_nFrame);

	// 情報を設定
	m_state = STATE_CHANGE;		// ゲージ変動状態
	m_nCounterState = m_nFrame;	// 状態管理カウンター

	// 表示値を引数分加算
	m_nNumGauge += nAdd;

	// 表示値の制限
	useful::LimitNum(m_nNumGauge, 0, m_nMaxNumGauge);
}

//============================================================
//	ゲージの設定処理
//============================================================
void CObjectGauge2D::SetNum(const int nNum)
{
	// 引数の表示値を設定
	m_nNumGauge = nNum;
	useful::LimitNum(m_nNumGauge, 0, m_nMaxNumGauge);	// 表示値の制限

	// 現在の表示値を設定
	m_fCurrentNumGauge = (float)m_nNumGauge;
	useful::LimitNum(m_fCurrentNumGauge, 0.0f, (float)m_nMaxNumGauge);	// 現在の表示値の制限

	// 情報を設定
	m_state = STATE_NONE;	// ゲージ変動状態
	m_nCounterState = 0;	// 状態管理カウンター

	// ゲージの横幅加算量を設定
	m_fAddRight = (m_fCurrentNumGauge * ((m_sizeGauge.x * 2.0f) / (float)m_nMaxNumGauge)) - m_sizeGauge.x;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	ゲージ取得処理
//============================================================
int CObjectGauge2D::GetNum(void) const
{
	// 表示値を返す
	return m_nNumGauge;
}

//============================================================
//	ゲージ最大値の設定処理
//============================================================
void CObjectGauge2D::SetMaxNum(const int nMax)
{
	// 引数の表示最大値を設定
	m_nMaxNumGauge = nMax;

	// ゲージの設定
	SetNum(m_nMaxNumGauge);
}

//============================================================
//	ゲージ最大値取得処理
//============================================================
int CObjectGauge2D::GetMaxNum(void) const
{
	// 表示最大値を返す
	return m_nMaxNumGauge;
}

//============================================================
//	テクスチャ割当処理 (インデックス)
//============================================================
void CObjectGauge2D::BindTexture(const int nPolygonID, const int nTextureID)
{
	if (nPolygonID < POLYGON_MAX)
	{ // インデックスが使用する四角形ポリゴン数より小さい場合

		if (nTextureID > NONE_IDX)
		{ // テクスチャインデックスが使用可能な場合

			// テクスチャインデックスを代入
			m_aTextureID[nPolygonID] = nTextureID;
		}
		else { assert(false); }	// 範囲外
	}
}

//============================================================
//	テクスチャ割当処理 (パス)
//============================================================
void CObjectGauge2D::BindTexture(const int nPolygonID, const char *pTexturePass)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	if (pTexture == NULL)
	{ // テクスチャポインタが存在しない場合

		// 関数を抜ける
		assert(false);
		return;
	}

	if (nPolygonID < POLYGON_MAX)
	{ // インデックスが使用する四角形ポリゴン数より小さい場合

		if (pTexturePass != NULL)
		{ // 割り当てるテクスチャパスが存在する場合
	
			// テクスチャインデックスを代入
			m_aTextureID[nPolygonID] = pTexture->Regist(pTexturePass);
		}
		else { assert(false); }	// テクスチャパス無し
	}
}

//============================================================
//	ゲージ大きさの設定処理
//============================================================
void CObjectGauge2D::SetScalingGauge(const D3DXVECTOR3& rSize)
{
	// 引数のゲージ大きさを代入
	m_sizeGauge = rSize;

	// ゲージの設定
	SetNum(m_nNumGauge);

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	ゲージ大きさ取得処理
//============================================================
D3DXVECTOR3 CObjectGauge2D::GetScalingGauge(void) const
{
	// ゲージ大きさを返す
	return m_sizeGauge;
}

//============================================================
//	背景大きさの設定処理
//============================================================
void CObjectGauge2D::SetScalingFrame(const D3DXVECTOR3& rSize)
{
	// 引数の背景大きさを代入
	m_sizeFrame = rSize;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	背景大きさ取得処理
//============================================================
D3DXVECTOR3 CObjectGauge2D::GetScalingFrame(void) const
{
	// 背景大きさを返す
	return m_sizeFrame;
}

//============================================================
//	表ゲージ色の設定処理
//============================================================
void CObjectGauge2D::SetColorFront(const D3DXCOLOR& rCol)
{
	// 引数の表ゲージ色を代入
	m_colFront = rCol;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	表ゲージ色取得処理
//============================================================
D3DXCOLOR CObjectGauge2D::GetColorFront(void) const
{
	// 表ゲージ色を返す
	return m_colFront;
}

//============================================================
//	裏ゲージ色の設定処理
//============================================================
void CObjectGauge2D::SetColorBack(const D3DXCOLOR& rCol)
{
	// 引数の裏ゲージ色を代入
	m_colBack = rCol;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	裏ゲージ色取得処理
//============================================================
D3DXCOLOR CObjectGauge2D::GetColorBack(void) const
{
	// 裏ゲージ色を返す
	return m_colBack;
}

//============================================================
//	枠表示状況設定処理
//============================================================
void CObjectGauge2D::SetEnableDrawFrame(const bool bDraw)
{
	// 引数の枠の表示状況を設定
	m_bDrawFrame = bDraw;
}

//============================================================
//	枠表示状況取得処理
//============================================================
bool CObjectGauge2D::IsEnableDrawFrame(void) const
{
	// 枠表示状況を返す
	return m_bDrawFrame;
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObjectGauge2D::SetVtx(void)
{
	// ポインタを宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntGauge = 0; nCntGauge < POLYGON_MAX; nCntGauge++)
		{ // 使用する四角形ポリゴン数分繰り返す

			switch (nCntGauge)
			{ // 四角形ポリゴンごとの処理
			case POLYGON_BACK:	// 背景

				// 頂点座標を設定
				pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_sizeGauge.x, m_pos.y - m_sizeGauge.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_sizeGauge.x, m_pos.y - m_sizeGauge.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_sizeGauge.x, m_pos.y + m_sizeGauge.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_sizeGauge.x, m_pos.y + m_sizeGauge.y, 0.0f);

				// 頂点カラーの設定
				pVtx[0].col = m_colBack;
				pVtx[1].col = m_colBack;
				pVtx[2].col = m_colBack;
				pVtx[3].col = m_colBack;

				break;

			case POLYGON_FRONT:	// ゲージ

				// 頂点座標を設定
				pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_sizeGauge.x, m_pos.y - m_sizeGauge.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fAddRight,   m_pos.y - m_sizeGauge.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_sizeGauge.x, m_pos.y + m_sizeGauge.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fAddRight,   m_pos.y + m_sizeGauge.y, 0.0f);

				// 頂点カラーの設定
				pVtx[0].col = m_colFront;
				pVtx[1].col = m_colFront;
				pVtx[2].col = m_colFront;
				pVtx[3].col = m_colFront;

				break;

			case POLYGON_FRAME:	// 枠

				// 頂点座標を設定
				pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_sizeFrame.x, m_pos.y - m_sizeFrame.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_sizeFrame.x, m_pos.y - m_sizeFrame.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_sizeFrame.x, m_pos.y + m_sizeFrame.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_sizeFrame.x, m_pos.y + m_sizeFrame.y, 0.0f);

				// 頂点カラーの設定
				pVtx[0].col = XCOL_WHITE;
				pVtx[1].col = XCOL_WHITE;
				pVtx[2].col = XCOL_WHITE;
				pVtx[3].col = XCOL_WHITE;

				break;

			default:	// 例外処理
				assert(false);
				break;
			}

			// rhw の設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点データのポインタを 4つ分進める
			pVtx += 4;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}
