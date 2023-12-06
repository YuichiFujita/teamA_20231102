//============================================================
//
//	パーティクル3Dヘッダー [particle3D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MODELPARTICLE_H_
#define _MODELPARTICLE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "Object.h"

//<************************************************************
//クラス宣言
//<************************************************************
class CModParticle :public CObject
{
public:

	CModParticle();
	~CModParticle() {}

	// オーバーライド関数
	HRESULT Init(void) override;					// 初期化
	void Uninit(void) override { Release(); };		// 終了
	void Update(void) override;						// 更新
	void Draw(void) override {};					// 描画

	static CModParticle *Create(const D3DXVECTOR3 rPos);

private:

	// オーバーライド関数
	void Release(void) override { CObject::Release(); }	// 破棄
	void Set(void);


	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_rot;	// 向き
	D3DXCOLOR m_col;	// 色
	int  m_nLife;		// 寿命

};

#endif
