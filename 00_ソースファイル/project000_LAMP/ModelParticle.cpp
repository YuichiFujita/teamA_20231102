#include "ModelParticle.h"
#include "ModelEffect.h"

//<**************************************************************
//名前宣言
//<**************************************************************
namespace
{
	const int			NUM_PARTICLE	= 1;											//使うパーティクルの数
	const D3DXVECTOR3	MOVE_VALUE		= D3DXVECTOR3(20.0f, 45.0f, 20.0f);				//移動値
	const int			NUM_LIFE		= 100;											//寿命
	const float			FLOAT_VALUE		= 100.0f;										//移動値を出すための値
}
//<=============================================================
//モデルパーティクルのコンストラクタ
//<=============================================================
CModParticle::CModParticle() : CObject(CObject::LABEL_PARTICLE, 1)
{
	// メンバ変数をクリア
	m_pos = VEC3_ZERO;	// 位置
	m_col = XCOL_WHITE;	// 色
	m_nLife = 0;		// 寿命
}
//<=============================================================
//モデルパーティクルの生成処理
//<=============================================================
CModParticle *CModParticle::Create(const D3DXVECTOR3 rPos)
{
	//最初に生成する
	CModParticle *pModParticle = new CModParticle;

	//中身チェック
	if (pModParticle != nullptr)
	{
		//初期化が失敗していないかのAssert
		assert(pModParticle->Init() == S_OK);

		//値の代入
		pModParticle->m_pos = rPos;

		//ポインタを返す
		return pModParticle;
	}
	//できなかったら
	else
	{
		//nullptrを返す
		return nullptr;
	}

	//nullptrを返す
	return nullptr;
}
//<=============================================================
//モデルパーティクルの初期化処理
//<=============================================================
HRESULT CModParticle::Init(void)
{
	// メンバ変数をクリア
	m_pos = VEC3_ZERO;	// 位置
	m_col = XCOL_WHITE;	// 色
	m_move = VEC3_ZERO;	// 移動値
	m_rot = VEC3_ZERO;	// 向き
	m_nLife = 5;		// 寿命

	return S_OK;
}
//<=============================================================
//モデルパーティクルの更新処理
//<=============================================================
void CModParticle::Update(void)
{
	//寿命がまだあれば
	if (m_nLife > 0)
	{
		//寿命を減らす
		m_nLife--;
	}
	//なければ
	else
	{
		//終了をする
		Uninit();

		return;
	}

	//設定を行う
	Set();
}
//<=============================================================
//モデルパーティクルの設定処理
//<=============================================================
void CModParticle::Set(void)
{
	// 生成されるエフェクト数分繰り返す
	for (int nCntPart = 0; nCntPart < NUM_PARTICLE; nCntPart++)
	{ 

	  // ベクトルをランダムに設定
		m_move.x = sinf((rand() % 629 - 314) / FLOAT_VALUE) * 1.0f;
		m_move.y = cosf((rand() % 629 - 314) / FLOAT_VALUE) * 1.0f;
		m_move.z = cosf((rand() % 629 - 314) / FLOAT_VALUE) * 1.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&m_move, &m_move);

		// 移動量を設定
		m_move.x *= MOVE_VALUE.x;
		m_move.y *= MOVE_VALUE.y;
		m_move.z *= MOVE_VALUE.z;

		//モデルエフェクトを生成する
		CModEffect::Create(m_pos, m_rot, m_move, NUM_LIFE,true);

	}
}