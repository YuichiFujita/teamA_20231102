//<============================================================
//
//モデルでのエフェクト処理 [ModelEffect.cpp]
//Author：kazuki watanebe
//
//<============================================================
#include "ModelEffect.h"

//<=============================================================
//モデルエフェクトのコンストラクタ
//<=============================================================
CModEffect::CModEffect() : CObjectModel(CObject::LABEL_EFFECT,1)
{
	//値のクリア
	m_rMove = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_rSize = VEC3_ZERO;
	m_rPos = VEC3_ZERO;
	m_nLife = 0;

	m_bGravity = false;
}
//<=============================================================
//モデルエフェクトの生成処理
//<=============================================================
CModEffect *CModEffect::Create(const D3DXVECTOR3 rPos, const D3DXVECTOR3 rRot, 
	const D3DXVECTOR3 rMove, const int nLife, const bool bGravity)
{
	//生成するためのオブジェクト
	CModEffect *pModelEffect = new CModEffect;

	//中身チェック
	if (pModelEffect != nullptr)
	{
		//初期化処理
		assert(pModelEffect->Init() == S_OK);

		//<*********************************
		//モデルの割り当て
		//<*********************************
		pModelEffect->SetVec3Position(rPos);	//位置
		pModelEffect->SetVec3Rotation(rRot);	//向き
		pModelEffect->m_rMove = rMove;			//移動値
		pModelEffect->m_nLife = nLife;			//寿命
		pModelEffect->m_bGravity = bGravity;	//重力を加えるかどうか

		//モデルの割り当て
		pModelEffect->BindModel("data\\MODEL\\OBSTACLE\\Jar000.x");

		//ポインタを返す
		return pModelEffect;
	}
	//なければ
	else
	{
		//nullptrを返す
		return nullptr;
	}

	//nullptrを返す
	return nullptr;
}
//<=============================================================
//モデルエフェクトの更新処理
//<=============================================================
void CModEffect::Update(void)
{
	//情報の取得
	m_rPos = GetVec3Position();
	m_rot = GetVec3Rotation();

	//寿命がまだあれば
	if (m_nLife > 0)
	{
		//寿命を減らす
		m_nLife--;
	}
	//なければ
	else
	{
		//終了処理をする
		Uninit();

		return;
	}
	//もし重力するかどうかがtrueになっていたら
	if (m_bGravity)
	{
		//地面の位置を下回りそうになったら
		if (m_rPos.y <= 0.0f)
		{
			m_rPos.y = 0.0f;

			//終了をする
			Uninit();

			return;
		}
		//それ以外の場合
		else
		{
			//重力を働かせる
			m_rMove.y -= 1.5f;
		}

	}

	//移動値を加算
	m_rPos += m_rMove;

	//それぞれの設定
	CObjectModel::SetVec3Position(m_rPos);
	CObjectModel::SetVec3Rotation(m_rot);

	//更新処理
	CObjectModel::Update();
}