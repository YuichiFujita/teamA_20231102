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
COrbitalEffect::COrbitalEffect() : CObject()
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
	
}

//============================================================
//	更新処理
//============================================================
void COrbitalEffect::Update(void)
{
	// 変数を宣言
	D3DXVECTOR3 pos = GetVec3Position();	// 位置
	D3DXVECTOR3 rot = GetVec3Rotation();	// 向き
	D3DXVECTOR3 size = GetVec3Sizing();		// 大きさ
	D3DXCOLOR   col = GetColor();			// 色
	float fRadius = size.x;				// 半径

	if (m_nLife <= 0		// 寿命を迎えた
		|| fRadius <= 0.0f)	// 半径が0.0f以下
	{ // 上記のどれかになった場合

	  // オブジェクトを破棄
		Uninit();

		// 関数を抜ける
		return;
	}

	// 移動量を加算
	pos += m_move;

	// 寿命を減算
	m_nLife--;


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
//
////============================================================
////	生成処理
////============================================================
//COrbitalEffect *COrbitalEffect::Create
//(
//	 D3DXVECTOR3& rPos,	// 位置
//	D3DXCOLOR col,
//	 float fOffset,		// 大きさ
//	 int nLife ,					// 寿命
//	 D3DXVECTOR3& rMove ,	// 移動量
//	 float fAttenuation ,
//	 bool bAdd ,
//	 bool bGravity ,
//	 CObject::ELabel label 	// オブジェクトラベル
//)
//{
//	// 変数を宣言
//	int nTextureID;	// テクスチャインデックス
//
//	// ポインタを宣言
//	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
//	COrbitalEffect *pOrbitalEffect = NULL;	// エフェクト3D生成用
//
//	if (pOrbitalEffect == NULL)
//	{ // 使用されていない場合
//
//	  // メモリ確保
//		pOrbitalEffect = new COrbitalEffect();	// エフェクト3D
//	}
//	else { assert(false); return NULL; }	// 使用中
//
//	if (pOrbitalEffect != NULL)
//	{ // 確保に成功している場合
//
//	  // エフェクト3Dの初期化
//		if (FAILED(pOrbitalEffect->Init()))
//		{ // 初期化に失敗した場合
//
//		  // メモリ開放
//			delete pOrbitalEffect;
//			pOrbitalEffect = NULL;
//
//			// 失敗を返す
//			return NULL;
//		}
//
//	
//		// 位置を設定
//		pOrbitalEffect->SetVec3Position(rPos);
//		// 引数の情報を設定
//		pOrbitalEffect->m_move = rMove;		// 移動量
//		pOrbitalEffect->m_nLife = nLife;		// 寿命
//		pOrbitalEffect->m_bAdd = bAdd;			// 加算合成状況
//		pOrbitalEffect->m_pOrbit = CObjectOrbit::Create(&pOrbitalEffect->m_Mtx, CObjectOrbit::SOffset(D3DXVECTOR3(rPos.x, rPos.y, rPos.z), D3DXVECTOR3(rPos.x, rPos.y, rPos.z), col, col));
//		pOrbitalEffect->m_pOrbit->SetEnableDraw(false);
//		// 確保したアドレスを返す
//		return pOrbitalEffect;
//	}
//	else { assert(false); return NULL; }	// 確保失敗
//}

//============================================================
//	破棄処理
//============================================================
void COrbitalEffect::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}