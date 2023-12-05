//============================================================
//
//	エフェクト3Dヘッダー [OrbitalEffect.h]
//	Author：TANNO RYUNOSUKE
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OrbitalEffect_H_
#define _OrbitalEffect_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectOrbit.h"

//************************************************************
//	クラス定義
//************************************************************
// エフェクト3Dクラス
class COrbitalEffect : public CObject
{
public:
	

	// コンストラクタ
	COrbitalEffect();

	// デストラクタ
	~COrbitalEffect();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

									// 静的メンバ関数
	static COrbitalEffect *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& Offset,	// 大きさ
		const D3DXCOLOR col,

		const int nLife = 1,					// 寿命
		const D3DXVECTOR3& rMove = VEC3_ZERO,	// 移動量
		const float fAttenuation = 0.9f,
		const bool bAdd = true,
		const bool bGravity = false,
		const CObject::ELabel label = LABEL_EFFECT	// オブジェクトラベル
	);

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄
	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数
	CObjectOrbit * m_pOrbit;	//オービット
	D3DXMATRIX m_Mtx;
							// メンバ変数
	D3DXVECTOR3 m_pos;		//座標
	D3DXVECTOR3 m_move;		// 移動量
	int		m_nLife;		// 寿命
	float	m_fOffset;		//サイズ
	float	m_fAttenuation; //移動量減衰率
	bool	m_bAdd;			// 加算合成状況
	bool	m_bGravity;		//重力
};

#endif	// _OrbitalEffect_H_
