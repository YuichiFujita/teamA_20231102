//============================================================
//
//	オブジェクト軌跡処理 [objectOrbit.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectOrbit.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "scene.h"
#include "sceneGame.h"
#include "pause.h"

//************************************************************
//	子クラス [CObjectOrbit] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectOrbit::CObjectOrbit()
{
	// メンバ変数をクリア
	memset(&m_orbit, 0, sizeof(m_orbit));	// 軌跡の情報
	m_pVtxBuff	= NULL;			// 頂点バッファ
	m_state		= STATE_NORMAL;	// 状態
	m_nNumVtx	= 0;			// 必要頂点数
	m_nTextureID	= 0;		// テクスチャインデックス
	m_nCounterState	= 0;		// 状態管理カウンター
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObjectOrbit::CObjectOrbit(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	memset(&m_orbit, 0, sizeof(m_orbit));	// 軌跡の情報
	m_pVtxBuff	= NULL;			// 頂点バッファ
	m_state		= STATE_NORMAL;	// 状態
	m_nNumVtx	= 0;			// 必要頂点数
	m_nTextureID	= 0;		// テクスチャインデックス
	m_nCounterState	= 0;		// 状態管理カウンター
}

//============================================================
//	デストラクタ
//============================================================
CObjectOrbit::~CObjectOrbit()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectOrbit::Init(void)
{
	// メンバ変数を初期化
	m_pVtxBuff	= NULL;			// 頂点バッファ
	m_state		= STATE_NORMAL;	// 状態
	m_nNumVtx	= 0;			// 必要頂点数
	m_nTextureID	= NONE_IDX;	// テクスチャインデックス
	m_nCounterState	= 0;		// 状態管理カウンター

	// 軌跡の情報を初期化
	memset(&m_orbit.mtxVanish, 0, sizeof(m_orbit.mtxVanish));	// 消失開始時の親のマトリックス
	m_orbit.offset = SOffset(VEC3_ZERO, VEC3_ZERO, XCOL_WHITE);	// オフセット情報
	D3DXMatrixIdentity(&m_orbit.aMtxWorldPoint[0]);	// 両端のワールドマトリックス
	D3DXMatrixIdentity(&m_orbit.aMtxWorldPoint[1]);	// 両端のワールドマトリックス
	m_orbit.pMtxParent	= NULL;		// 親のマトリックス
	m_orbit.pPosPoint	= NULL;		// 各頂点座標
	m_orbit.pColPoint	= NULL;		// 各頂点カラー
	m_orbit.nPart		= 1;		// 分割数
	m_orbit.nTexPart	= 1;		// テクスチャ分割数
	m_orbit.bAlpha		= false;	// 透明化状況
	m_orbit.bInit		= false;	// 初期化状況

	// 長さを設定
	if (FAILED(SetLength(1)))
	{ // 長さの設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectOrbit::Uninit(void)
{
	// 各頂点座標の破棄
	if (m_orbit.pPosPoint != NULL)
	{ // 各頂点座標が使用中の場合

		// メモリ開放
		delete[] m_orbit.pPosPoint;
		m_orbit.pPosPoint = NULL;
	}

	// 各頂点カラーの破棄
	if (m_orbit.pColPoint != NULL)
	{ // 各頂点カラーが使用中の場合

		// メモリ開放
		delete[] m_orbit.pColPoint;
		m_orbit.pColPoint = NULL;
	}

	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{ // 頂点バッファが使用中の場合

		// メモリ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクト軌跡を破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectOrbit::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CObjectOrbit::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX mtxIdent;	// 単位マトリックス設定用
	D3DXMATRIX mtxParent;	// 親のマトリックス
	bool bUpdate = true;	// 更新状況

	// 単位マトリックスの初期化
	D3DXMatrixIdentity(&mtxIdent);

	// ポインタを宣言
	LPDIRECT3DDEVICE9	pDevice		= CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	CTexture			*pTexture	= CManager::GetInstance()->GetTexture();				// テクスチャへのポインタ

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{ // モードがゲームの場合

		if (CSceneGame::GetPause()->IsPause())
		{ // ポーズ中の場合

			// 更新しない状況にする
			bUpdate = false;
		}
	}
	
	if (m_state != STATE_NONE)
	{ // 何もしない状態ではない場合

		//----------------------------------------------------
		//	レンダーステートを変更
		//----------------------------------------------------
		if (m_orbit.bAdd)
		{ // 加算合成がONの場合

			// αブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		// ライティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ポリゴンの両面を表示状態にする
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		if (bUpdate)
		{ // 更新する状況の場合

			//------------------------------------------------
			//	状態ごとの設定
			//------------------------------------------------
			switch (m_state)
			{ // 状態ごとの処理
			case STATE_NORMAL:	// 通常状態

				// 親マトリックスを設定
				mtxParent = *m_orbit.pMtxParent;

				break;

			case STATE_VANISH:	// 消失状態

				// 親マトリックスを設定
				mtxParent = m_orbit.mtxVanish;

				// カウンターを加算
				if (m_nCounterState < (m_nNumVtx / MAX_OFFSET) + 1)
				{ // カウンターが軌跡が伸び切る時間より小さい場合

					// カウンターを加算
					m_nCounterState++;
				}
				else
				{ // カウンターが軌跡が伸び切る時間以上の場合

					// カウンターを補正
					m_nCounterState = 0;

					// 状態を設定
					m_state = STATE_NONE;	// 何もしない状態
				}

				break;

			default:	// 例外状態
				assert(false);
				break;
			}

			//------------------------------------------------
			//	オフセットの初期化
			//------------------------------------------------
			for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
			{ // オフセットの数分繰り返す

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&m_orbit.aMtxWorldPoint[nCntOff]);

				// 位置を反映
				D3DXMatrixTranslation(&m_orbit.aMtxWorldPoint[nCntOff], m_orbit.offset.aOffset[nCntOff].x, m_orbit.offset.aOffset[nCntOff].y, m_orbit.offset.aOffset[nCntOff].z);

				// 親のマトリックスと掛け合わせる
				D3DXMatrixMultiply(&m_orbit.aMtxWorldPoint[nCntOff], &m_orbit.aMtxWorldPoint[nCntOff], &mtxParent);
			}

			//------------------------------------------------
			//	頂点座標と頂点カラーの情報をずらす
			//------------------------------------------------
			for (int nCntVtx = m_nNumVtx - 1; nCntVtx >= MAX_OFFSET; nCntVtx--)
			{ // 維持する頂点の最大数分繰り返す (オフセット分は含まない)

				// 頂点情報をずらす
				m_orbit.pPosPoint[nCntVtx] = m_orbit.pPosPoint[nCntVtx - MAX_OFFSET];
				m_orbit.pColPoint[nCntVtx] = m_orbit.pColPoint[nCntVtx - MAX_OFFSET];
			}

			//------------------------------------------------
			//	最新の頂点座標と頂点カラーの情報を設定
			//------------------------------------------------
			for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
			{ // オフセットの数分繰り返す

				// 頂点座標の設定
				m_orbit.pPosPoint[nCntOff] = D3DXVECTOR3
				( // 引数
					m_orbit.aMtxWorldPoint[nCntOff]._41,	// x
					m_orbit.aMtxWorldPoint[nCntOff]._42,	// y
					m_orbit.aMtxWorldPoint[nCntOff]._43		// z
				);

				// 頂点カラーの設定
				m_orbit.pColPoint[nCntOff] = m_orbit.offset.aCol[nCntOff];
			}
		}

		//----------------------------------------------------
		//	頂点座標と頂点カラーの情報を初期化
		//----------------------------------------------------
		if (!m_orbit.bInit)
		{ // 初期化済みではない場合

			for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
			{ // 維持する頂点の最大数分繰り返す

				// 頂点座標の設定
				m_orbit.pPosPoint[nCntVtx] = D3DXVECTOR3
				( // 引数
					m_orbit.aMtxWorldPoint[nCntVtx % MAX_OFFSET]._41,	// x
					m_orbit.aMtxWorldPoint[nCntVtx % MAX_OFFSET]._42,	// y
					m_orbit.aMtxWorldPoint[nCntVtx % MAX_OFFSET]._43	// z
				);

				// 頂点カラーの設定
				m_orbit.pColPoint[nCntVtx] = m_orbit.offset.aCol[nCntVtx % MAX_OFFSET];
			}

			// 初期化済みにする
			m_orbit.bInit = true;
		}

		//----------------------------------------------------
		//	ポリゴンの描画
		//----------------------------------------------------
		// 頂点情報の設定
		SetVtx();

		// 単位マトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxIdent);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);

		//----------------------------------------------------
		//	レンダーステートを元に戻す
		//----------------------------------------------------
		// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// ライティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// ポリゴンの表面のみを表示状態にする
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

//============================================================
//	テクスチャ割当処理 (インデックス)
//============================================================
void CObjectOrbit::BindTexture(const int nTextureID)
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
void CObjectOrbit::BindTexture(const char *pTexturePass)
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
//	状態取得処理
//============================================================
int CObjectOrbit::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	生成処理
//============================================================
CObjectOrbit *CObjectOrbit::Create
(
	D3DXMATRIX *pMtxParent,	// 親マトリックス
	const SOffset offset,	// オフセット情報
	const int nPart,		// 分割数
	const int nTexPart,		// テクスチャ分割数
	const bool bAdd,		// 加算合成状況
	const bool bAlpha		// 透明化状況
)
{
	// ポインタを宣言
	CObjectOrbit *pObjectOrbit = NULL;		// オブジェクト軌跡生成用

	if (pObjectOrbit == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pObjectOrbit = new CObjectOrbit;	// オブジェクト軌跡
	}
	else { assert(false); return NULL; }	// 使用中

	if (pObjectOrbit != NULL)
	{ // 確保に成功している場合

		// オブジェクト軌跡の初期化
		if (FAILED(pObjectOrbit->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObjectOrbit;
			pObjectOrbit = NULL;

			// 失敗を返す
			return NULL;
		}

		// 親のマトリックスを設定
		pObjectOrbit->SetMatrixParent(pMtxParent);

		// オフセット情報を設定
		pObjectOrbit->SetOffset(offset);

		// テクスチャ分割数を設定
		pObjectOrbit->SetTexPart(nTexPart);

		// 加算合成状況を設定
		pObjectOrbit->SetEnableAdd(bAdd);

		// 透明化状況を設定
		pObjectOrbit->SetEnableAlpha(bAlpha);

		// 長さを設定
		if (FAILED(pObjectOrbit->SetLength(nPart)))
		{ // 長さの設定に失敗した場合

			// メモリ開放
			delete pObjectOrbit;
			pObjectOrbit = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pObjectOrbit;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	親マトリックス削除処理
//============================================================
void CObjectOrbit::DeleteMatrixParent(void)
{
	// 消失状態にする
	SetState(STATE_VANISH);

	// 親マトリックスをNULLにする
	m_orbit.pMtxParent = NULL;
}

//============================================================
//	状態の設定処理
//============================================================
void CObjectOrbit::SetState(const EState state)
{
	if (state == m_state && state != STATE_NORMAL)
	{ // 設定する状態が現在の状態且つ、設定する状態が通常状態の場合

		// 処理を抜ける
		return;
	}

	// 引数の状態を設定
	m_state = state;

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない状態

		// 無し

		break;

	case STATE_NORMAL:	// 通常状態

		// 初期化していない状態にする
		m_orbit.bInit = false;

		break;

	case STATE_VANISH:	// 消失状態

		// 現在の親マトリックスを消失するマトリックスに設定
		m_orbit.mtxVanish = *m_orbit.pMtxParent;

		break;

	default:	// 例外状態
		assert(false);
		break;
	}
}

//============================================================
//	親のマトリックスの設定処理
//============================================================
void CObjectOrbit::SetMatrixParent(D3DXMATRIX *pMtxParent)
{
	// 引数の親マトリックスを設定
	m_orbit.pMtxParent = pMtxParent;
}

//============================================================
//	オフセット情報の設定処理
//============================================================
void CObjectOrbit::SetOffset(const SOffset offset)
{
	// 引数のオフセット情報を設定
	m_orbit.offset = offset;
}

//============================================================
//	テクスチャ分割数の設定処理
//============================================================
void CObjectOrbit::SetTexPart(const int nTexPart)
{
	// 例外処理
	assert(nTexPart > 0);	// 0以下エラー

	// 引数のテクスチャ分割数を設定
	m_orbit.nTexPart = nTexPart;
}

//============================================================
//	加算合成状況の設定処理
//============================================================
void CObjectOrbit::SetEnableAdd(const bool bAdd)
{
	// 引数の加算合成状況を設定
	m_orbit.bAdd = bAdd;
}

//============================================================
//	透明化状況の設定処理
//============================================================
void CObjectOrbit::SetEnableAlpha(const bool bAlpha)
{
	// 引数の透明化状況を設定
	m_orbit.bAlpha = bAlpha;
}

//============================================================
//	初期化状況の設定処理
//============================================================
void CObjectOrbit::SetEnableInit(const bool bInit)
{
	// 引数の初期化状況を設定
	m_orbit.bInit = bInit;
}

//============================================================
//	長さの設定処理
//============================================================
HRESULT CObjectOrbit::SetLength(const int nPart)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 必要頂点数を求める
	m_nNumVtx = nPart * MAX_OFFSET;

	//--------------------------------------------------------
	//	各頂点座標の破棄・生成
	//--------------------------------------------------------
	// 各頂点座標の破棄
	if (m_orbit.pPosPoint != NULL)
	{ // 各頂点座標が使用中の場合

		// メモリ開放
		delete[] m_orbit.pPosPoint;
		m_orbit.pPosPoint = NULL;
	}

	// 各頂点座標の情報を設定
	if (m_orbit.pPosPoint == NULL)
	{ // 非使用中の場合

		// 各頂点座標のメモリ確保
		m_orbit.pPosPoint = new D3DXVECTOR3[m_nNumVtx];

		if (m_orbit.pPosPoint != NULL)
		{ // 確保に成功した場合

			// メモリクリア
			memset(m_orbit.pPosPoint, 0, sizeof(D3DXVECTOR3) * m_nNumVtx);
		}
		else { assert(false); return E_FAIL; }	// 確保失敗
	}
	else { assert(false); return E_FAIL; }	// 使用中

	//--------------------------------------------------------
	//	各頂点カラーの破棄・生成
	//--------------------------------------------------------
	// 各頂点カラーの破棄
	if (m_orbit.pColPoint != NULL)
	{ // 各頂点カラーが使用中の場合

		// メモリ開放
		delete[] m_orbit.pColPoint;
		m_orbit.pColPoint = NULL;
	}

	// 各頂点カラーの情報を設定
	if (m_orbit.pColPoint == NULL)
	{ // 非使用中の場合

		// 各頂点カラーのメモリ確保
		m_orbit.pColPoint = new D3DXCOLOR[m_nNumVtx];

		if (m_orbit.pColPoint != NULL)
		{ // 確保に成功した場合

			// メモリクリア
			memset(m_orbit.pColPoint, 0, sizeof(D3DXCOLOR) * m_nNumVtx);
		}
		else { assert(false); return E_FAIL; }	// 確保失敗
	}
	else { assert(false); return E_FAIL; }	// 使用中

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

	// 頂点情報の設定
	SetVtx();

	// 成功を返す
	return S_OK;
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObjectOrbit::SetVtx(void)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{ // 軌跡の頂点バッファが使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
		{ // 維持する頂点の最大数分繰り返す

			// 頂点座標の設定
			pVtx[0].pos = m_orbit.pPosPoint[nCntVtx];

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 頂点カラーの設定
			if (m_orbit.bAlpha)
			{ // 透明にしていく場合

				pVtx[0].col = D3DXCOLOR
				( // 引数
					m_orbit.pColPoint[nCntVtx].r,	// r
					m_orbit.pColPoint[nCntVtx].g,	// g
					m_orbit.pColPoint[nCntVtx].b,	// b
					m_orbit.pColPoint[nCntVtx].a - (m_orbit.pColPoint[nCntVtx].a / (m_nNumVtx * 0.5f)) * (nCntVtx / 2)	// a
				);
			}
			else
			{ // 透明度を変えない場合

				pVtx[0].col = m_orbit.pColPoint[nCntVtx];
			}

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2
			( // 引数
				(1.0f / (float)m_orbit.nTexPart) * (nCntVtx / 2),	// u
				1.0f * -((nCntVtx % 2) - 1)							// v
			);

			// 頂点データのポインタを進める
			pVtx += 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
	else { assert(false); }	// 非使用中
}

//============================================================
//	破棄処理
//============================================================
void CObjectOrbit::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}
