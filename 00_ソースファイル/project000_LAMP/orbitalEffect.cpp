//============================================================
//
//	エフェクト3D処理 [OrbitalEffect.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "OrbitalEffect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define EFF_PRIO	(9)	// エフェクト3Dの優先順位
#define PART_PRIO	(9)	// パーティクル3Dの優先順位



//************************************************************
//	子クラス [COrbitalEffect] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
COrbitalEffect::COrbitalEffect() : CObject(LABEL_PARTICLE, PART_PRIO)
{
	// メンバ変数をクリア
	m_move = VEC3_ZERO;		// 移動量
	m_nLife = 0;		// 寿命
	m_bGravity = false;		// 大きさの減算量
	m_fOffset = 0.0f;		// 透明度の減算量
	m_bAdd = false;	// 加算合成状況
}



//============================================================
//	デストラクタ
//============================================================
COrbitalEffect::~COrbitalEffect()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT COrbitalEffect::Init(void)
{
	// メンバ変数を初期化
	m_move = VEC3_ZERO;	// 移動量
	m_nLife = 0;	// 寿命

	m_bAdd = true;	// 加算合成状況




	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void COrbitalEffect::Uninit(void)
{
	// オブジェクトビルボードの終了
	if (m_pOrbit != NULL)
	{
		m_pOrbit->Uninit();
		m_pOrbit = NULL;
	}
	Release();
}

//============================================================
//	更新処理
//============================================================
void COrbitalEffect::Update(void)
{
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリクス
	D3DXMATRIX mtxParent;

	D3DXVECTOR3 angles;

	// Yaw（ヨー）を計算
	angles.y = atan2(m_move.x, m_move.z);

	// Pitch（ピッチ）を計算
	angles.x = atan2(m_move.y, sqrt(m_move.x * m_move.x + m_move.z * m_move.z));

	// Roll（ロール）は0度に設定
	angles.z = 0.0f;



	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_Mtx);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, angles.y, angles.x, angles.z);
	D3DXMatrixMultiply(&m_Mtx, &m_Mtx, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_Mtx, &m_Mtx, &mtxTrans);


	// 移動量を加算
	m_pos += m_move;
	m_move *= m_fAttenuation;
	
	if (m_bGravity)
	{
		m_move.y -= 5.0f;
	}
	// 寿命を減算
	m_nLife--;

	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//============================================================
//	描画処理
//============================================================
void COrbitalEffect::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	if (m_bAdd)
	{ // 加算合成がONの場合

	  // αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	m_pOrbit->Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================
//	生成処理
//============================================================
COrbitalEffect *COrbitalEffect::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& Offset,	// 大きさ
	const D3DXCOLOR col,
	const int nLife,					// 寿命
	const D3DXVECTOR3& rMove,	// 移動量
	const float fAttenuation,
	const bool bAdd,
	const bool bGravity,
	const CObject::ELabel label	// オブジェクトラベル
)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	COrbitalEffect *pOrbitalEffect = NULL;	// エフェクト3D生成用

	if (pOrbitalEffect == NULL)
	{ // 使用されていない場合

	  // メモリ確保
		pOrbitalEffect = new COrbitalEffect();	// エフェクト3D
	}
	else { assert(false); return NULL; }	// 使用中

	if (pOrbitalEffect != NULL)
	{ // 確保に成功している場合

	  // エフェクト3Dの初期化
		if (FAILED(pOrbitalEffect->Init()))
		{ // 初期化に失敗した場合

		  // メモリ開放
			delete pOrbitalEffect;
			pOrbitalEffect = NULL;

			// 失敗を返す
			return NULL;
		}

	
		// 位置を設定
		pOrbitalEffect->m_pos = rPos;
		pOrbitalEffect->m_fAttenuation = fAttenuation;
		// 引数の情報を設定
		pOrbitalEffect->m_move = rMove;		// 移動量
		pOrbitalEffect->m_nLife = nLife;		// 寿命
		pOrbitalEffect->m_bAdd = bAdd;			// 加算合成状況
		pOrbitalEffect->m_bGravity = bGravity;
												//ワールドマトリクスの初期化
		D3DXMATRIX mtxRot, mtxTrans; //計算用マトリクス
		D3DXMATRIX mtxParent;

		D3DXVECTOR3 angles;


		//ワールドマトリクスの初期化
		D3DXMatrixIdentity(&pOrbitalEffect->m_Mtx);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, pOrbitalEffect->m_pos.x, pOrbitalEffect->m_pos.y, pOrbitalEffect->m_pos.z);
		D3DXMatrixMultiply(&pOrbitalEffect->m_Mtx, &pOrbitalEffect->m_Mtx, &mtxTrans);

		pOrbitalEffect->m_pOrbit = CObjectOrbit::Create(&pOrbitalEffect->m_Mtx, CObjectOrbit::SOffset(Offset, -Offset, col),100);
		pOrbitalEffect->m_pOrbit->SetEnableDraw(false);
		// 確保したアドレスを返す
		return pOrbitalEffect;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
void COrbitalEffect::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}