//<============================================================
//
//モデルでのエフェクト処理 [ModelEffect.h]
//Author：kazuki watanebe
//
//<============================================================
//<************************************************************
//二重インクルード防止
//<************************************************************
#ifndef _MODELEFFECT_H_
#define _MODELEFFECT_H_

//<************************************************************
//インクルードファイル
//<************************************************************
#include "main.h"
#include "ObjectModel.h"

//<************************************************************
//クラス宣言
//<************************************************************
class CModEffect : public CObjectModel
{
public:

	CModEffect();
	~CModEffect() {};

	//オーバーライド関数
	//初期化
	HRESULT Init(void) override 
	{
		CObjectModel::Init();

		return S_OK;

	}	

	//終了
	void Uninit(void) override { CObjectModel::Uninit(); }		

	//更新
	void Update(void) override;	

	//描画
	void Draw(void) override { CObjectModel::Draw(); }		

	//生成処理
	static CModEffect *Create(const D3DXVECTOR3 rPos, const D3DXVECTOR3 rRot, const D3DXVECTOR3 rMove,const int nLife,const bool bGravity);

private:

	//静的メンバ変数
	static const char *mc_apTextureFile[];	//テクスチャ定数

	D3DXVECTOR3 m_rPos;		//位置
	D3DXVECTOR3 m_rMove;	//移動量
	D3DXVECTOR3 m_rot;		//向き
	D3DXVECTOR3 m_rotMove;	//向きの移動量
	D3DXVECTOR3 m_rSize;	//サイズ
	D3DXCOLOR m_Col;		//色

	bool m_bGravity;		//重力を加えるかどうか
	int m_nLife;			//寿命
	int m_nNumLand;			//着地した回数
};

#endif
