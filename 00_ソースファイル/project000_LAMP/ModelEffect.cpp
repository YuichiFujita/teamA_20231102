//<============================================================
//
//モデルでのエフェクト処理 [ModelEffect.cpp]
//Author：kazuki watanebe
//
//<============================================================
#include "ModelEffect.h"
#include "useful.h"

//<************************************************************
//静的メンバ変数宣言
//<************************************************************
const char *CModEffect::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\MODEL\\PIECE\\PIECE000.x",		//かけら１
	"data\\MODEL\\PIECE\\PIECE001.x",		//かけら２
	"data\\MODEL\\PIECE\\PIECE002.x",		//かけら３
};

//<**************************************************************
//名前宣言
//<**************************************************************
namespace
{
	const int		MAX_PIECE	= 3;		//かけらの最大個数
	const float		VALUE_GRAV	= 1.5f;		//重力を加える値
	const float		VALUE_UP	= 50.0f;	//上昇する値
	const int		MAX_LAND	= 4;		//着地数の最大数
	const float		VALUE_ALPHA = 0.1f;		//透明度を下げる値

}
//<=============================================================
//モデルエフェクトのコンストラクタ
//<=============================================================
CModEffect::CModEffect() : CObjectModel(CObject::LABEL_EFFECT,1)
{
	//値のクリア
	m_rMove = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_rotMove = VEC3_ZERO;
	m_rSize = VEC3_ZERO;
	m_rPos = VEC3_ZERO;
	m_nLife = 0;
	m_nNumLand = 0;
	m_Col = XCOL_WHITE;

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

		//ランダムで出すかけらを設定
		pModelEffect->BindModel(mc_apTextureFile[rand() % MAX_PIECE]);

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
	//向きの移動量の設定
	m_rotMove = D3DXVECTOR3(0.05f, 0.05f, 0.05f);

	//情報の取得
	m_rPos = GetVec3Position();
	m_rot = GetVec3Rotation();

	//もし重力するかどうかがfalseになっていたら
	if (!m_bGravity)
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
			//終了処理をする
			Uninit();

			return;
		}
	}

	//もし重力するかどうかがtrueになっていたら
	if (m_bGravity)
	{
		//α値制限
		useful::LimitNum(m_Col.a, 0.0f, 1.0f);

		//地面の位置を下回りそうになったら
		if (m_rPos.y <= VEC3_ZERO.y)
		{
			//着地した回数を増やす
			m_nNumLand++;

			//地面一個手前の値にする
			m_rPos.y = 0.1f;

			//上に移動させる
			m_rMove.y += VALUE_UP / m_nNumLand;

			//透明度を減らす
			m_Col.a = m_Col.a / m_nNumLand;

			//着地した回数が最大値になっていたら
			if (m_nNumLand >= MAX_LAND)
			{
				//念のための初期化
				m_nNumLand = 0;

				//終了をする
				Uninit();

				//処理を抜ける
				return;
			}
			//ここでreturnをすると跳ねなくなってしまう
		}
		//それ以外の場合
		else
		{
			//重力を働かせる
			m_rMove.y -= VALUE_GRAV;
		}

	}

	//移動値を加算
	m_rPos += m_rMove;

	//向きの移動をする
	m_rot += m_rotMove;

	//円周率を超えないようにする
	useful::NormalizeRot(m_rot.y);

	//それぞれの設定
	CObjectModel::SetVec3Position(m_rPos);
	CObjectModel::SetVec3Rotation(m_rot);
	CObjectModel::SetAlpha(m_Col.a);

	//更新処理
	CObjectModel::Update();
}