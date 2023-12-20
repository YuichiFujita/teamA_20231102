//============================================================
//
//	パーティクル3Dヘッダー [orbitalParticle.h]
//	Author：丹野竜之介
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _orbitalParticle_H_
#define _orbitalParticle_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// パーティクル3Dクラス
class CorbitalParticle : CObject
{
public:
	// コンストラクタ
	CorbitalParticle();

	// デストラクタ
	~CorbitalParticle();



	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	static CorbitalParticle * Create(D3DXVECTOR3 pos, D3DXVECTOR3 Offset, D3DXCOLOR col, D3DXVECTOR3 range, D3DXVECTOR3 rot, D3DXVECTOR3 forth, int nLife, int nSpeed,int nEffectLife, int nLength, int nNumSpawn, float fAngle = 1.0f, float fAttenuation = 0.9f);
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄


	// メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_range;	// 範囲
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_Offset;	// 大きさ
	D3DXVECTOR3 m_Forth;	// 位置
	D3DXCOLOR m_col;		// 色
	int  m_nLife;			// 寿命
	int m_nEffectLife;
	int m_nLength;
	int m_nSpeed;			// 速度
	int m_nNumSpawn;		// 出現数
	float m_fAngle;			// 角度
	float m_fAttenuation; // 減衰率
	int m_nCount;
};

#endif	// _orbitalParticle_H_
