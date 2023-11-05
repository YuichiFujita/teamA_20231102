//============================================================
//
//	パーティクル3Dヘッダー [particle3D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PARTICLE3D_H_
#define _PARTICLE3D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// パーティクル3Dクラス
class CParticle3D : CObject
{
public:
	// コンストラクタ
	CParticle3D();

	// デストラクタ
	~CParticle3D();

	// パーティクル3D種類列挙
	enum EType
	{
		TYPE_NONE = 0,			// なし
		TYPE_DAMAGE,			// ダメージ
		TYPE_HEAL,				// 回復
		TYPE_STOMP_PLANT,		// 植物踏みつぶし
		TYPE_SMALL_EXPLOSION,	// 小爆発
		TYPE_BIG_EXPLOSION,		// 大爆発
		TYPE_PLAYER_DAMAGE,		// プレイヤーダメージ
		TYPE_MUZZLE_FLASH,		// マズルフラッシュ
		TYPE_TANK_FIRE,			// タンクファイア
		TYPE_BUBBLE_EXPLOSION,	// バブル爆発
		TYPE_MAX				// この列挙型の総数
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// オーバーロードしたオーバーライド関数
	HRESULT Init	// 初期化
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXCOLOR& rCol		// 色
	);

	// 静的メンバ関数
	static CParticle3D *Create	// 生成
	( // 引数
		const EType type,					// 種類
		const D3DXVECTOR3& rPos,			// 位置
		const D3DXCOLOR& rCol = XCOL_WHITE	// 色
	);

private:
	// メンバ関数
	void Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);		// ダメージ
	void Heal(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);			// 回復
	void StompPlant(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);	// 植物踏みつぶし

	void SmallExplosion(const D3DXVECTOR3& rPos);	// 小爆発
	void BigExplosion(const D3DXVECTOR3& rPos);		// 大爆発
	void PlayerDamage(const D3DXVECTOR3& rPos);		// プレイヤーダメージ
	void MuzzleFlash(const D3DXVECTOR3& rPos);		// マズルフラッシュ
	void TankFire(const D3DXVECTOR3& rPos);			// タンクファイア
	void BubbleExplosion(const D3DXVECTOR3& rPos);	// バブル爆発

	// 静的メンバ変数
	static const int mc_aLife[TYPE_MAX];	// 寿命定数

	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXCOLOR m_col;	// 色
	EType m_type;		// 種類
	int  m_nLife;		// 寿命
};

#endif	// _PARTICLE3D_H_
