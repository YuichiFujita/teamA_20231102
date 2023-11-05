//============================================================
//
//	パーティクル3D処理 [particle3D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "particle3D.h"
#include "effect3D.h"

//************************************************************
//	マクロ定義
//************************************************************
// ダメージ・バブル爆発
#define DMG_MOVE		(5.0f)	// ダメージの移動量
#define DMG_SPAWN		(35)	// ダメージの生成数
#define DMG_EFF_LIFE	(120)	// ダメージの寿命
#define DMG_SIZE		(80.0f)	// ダメージの大きさ
#define DMG_SUB_SIZE	(2.4f)	// ダメージの半径の減算量

// 回復
#define HEAL_POSGAP		(24.0f)	// 回復の位置ずれ量
#define HEAL_MOVE		(1.2f)	// 回復の移動量
#define HEAL_SPAWN		(6)		// 回復の生成数
#define HEAL_EFF_LIFE	(50)	// 回復の寿命
#define HEAL_SIZE		(55.0f)	// 回復の大きさ
#define HEAL_SUB_SIZE	(0.5f)	// 回復の半径の減算量

// 植物踏みつぶし
#define STOMP_MOVE		(2.5f)	// 植物踏みつぶしの移動量
#define STOMP_SPAWN		(8)		// 植物踏みつぶしの生成数
#define STOMP_EFF_LIFE	(28)	// 植物踏みつぶしの寿命
#define STOMP_SIZE		(36.0f)	// 植物踏みつぶしの大きさ
#define STOMP_SUB_SIZE	(0.05f)	// 植物踏みつぶしの半径の減算量

// 小爆発
#define SMALL_EXP_FIRE_COL			(D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f))	// 爆発の炎の色
#define SMALL_EXP_FIRE_POSGAP		(5.0f)	// 爆発の炎の位置ずれ量
#define SMALL_EXP_FIRE_MOVE			(2.0f)	// 爆発の炎の移動量
#define SMALL_EXP_FIRE_SPAWN		(48)	// 爆発の炎の生成数
#define SMALL_EXP_FIRE_EFF_LIFE		(18)	// 爆発の炎の寿命
#define SMALL_EXP_FIRE_RAND_LIFE	(8)		// 爆発の炎のランダム寿命加算量の最大値
#define SMALL_EXP_FIRE_SIZE			(3.27f)	// 爆発の炎の大きさ
#define SMALL_EXP_FIRE_SUB_SIZE		(-8.5f)	// 爆発の炎の半径の減算量

#define SMALL_EXP_SMOKE_COL			(D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f))	// 爆発の煙の色
#define SMALL_EXP_SMOKE_POSGAP		(3.0f)	// 爆発の煙の位置ずれ量
#define SMALL_EXP_SMOKE_MOVE		(1.2f)	// 爆発の煙の移動量
#define SMALL_EXP_SMOKE_SPAWN		(66)	// 爆発の煙の生成数
#define SMALL_EXP_SMOKE_EFF_LIFE	(44)	// 爆発の煙の寿命
#define SMALL_EXP_SMOKE_RAND_LIFE	(12)	// 爆発の煙のランダム寿命加算量の最大値
#define SMALL_EXP_SMOKE_SIZE		(70.0f)	// 爆発の煙の大きさ
#define SMALL_EXP_SMOKE_SUB_SIZE	(-1.0f)	// 爆発の煙の半径の減算量

// 大爆発
#define BIG_EXP_FIRE_COL		(D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f))	// 爆発の炎の色
#define BIG_EXP_FIRE_POSGAP		(10.0f)	// 爆発の炎の位置ずれ量
#define BIG_EXP_FIRE_MOVE		(2.54f)	// 爆発の炎の移動量
#define BIG_EXP_FIRE_SPAWN		(52)	// 爆発の炎の生成数
#define BIG_EXP_FIRE_EFF_LIFE	(36)	// 爆発の炎の寿命
#define BIG_EXP_FIRE_RAND_LIFE	(12)	// 爆発の炎のランダム寿命加算量の最大値
#define BIG_EXP_FIRE_SIZE		(3.57f)	// 爆発の炎の大きさ
#define BIG_EXP_FIRE_SUB_SIZE	(-7.0f)	// 爆発の炎の半径の減算量

#define BIG_EXP_SMOKE_COL		(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f))	// 爆発の煙の色
#define BIG_EXP_SMOKE_POSGAP	(8.0f)	// 爆発の煙の位置ずれ量
#define BIG_EXP_SMOKE_MOVE		(0.9f)	// 爆発の煙の移動量
#define BIG_EXP_SMOKE_SPAWN		(72)	// 爆発の煙の生成数
#define BIG_EXP_SMOKE_EFF_LIFE	(96)	// 爆発の煙の寿命
#define BIG_EXP_SMOKE_RAND_LIFE	(18)	// 爆発の煙のランダム寿命加算量の最大値
#define BIG_EXP_SMOKE_SIZE		(70.0f)	// 爆発の煙の大きさ
#define BIG_EXP_SMOKE_SUB_SIZE	(-2.5f)	// 爆発の煙の半径の減算量

// プレイヤーダメージ
#define PLAY_DMG_POSGAP		(12.0f)	// プレイヤーダメージの位置ずれ量
#define PLAY_DMG_MOVE_S		(6.6f)	// プレイヤーダメージの移動量 (小)
#define PLAY_DMG_MOVE_M		(5.4f)	// プレイヤーダメージの移動量 (中)
#define PLAY_DMG_MOVE_L		(4.2f)	// プレイヤーダメージの移動量 (大)
#define PLAY_DMG_EFF_SPAWN	(4)		// プレイヤーダメージの生成数
#define PLAY_DMG_RAND_SPAWN	(6)		// プレイヤーダメージのランダム生成数加算量の最大値
#define PLAY_DMG_EFF_LIFE	(28)	// プレイヤーダメージの寿命
#define PLAY_DMG_RAND_LIFE	(12)	// プレイヤーダメージのランダム寿命加算量の最大値
#define PLAY_DMG_SIZE_S		(32.0f)	// プレイヤーダメージの大きさ (小)
#define PLAY_DMG_SIZE_M		(42.0f)	// プレイヤーダメージの大きさ (中)
#define PLAY_DMG_SIZE_L		(52.0f)	// プレイヤーダメージの大きさ (大)
#define PLAY_DMG_SUB_SIZE	(0.25f)	// プレイヤーダメージの半径の減算量

// マズルフラッシュ
#define MUZZLEFLASH_COL			(D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f))	// マズルフラッシュの色
#define MUZZLEFLASH_MOVE		(0.64f)		// マズルフラッシュの移動量
#define MUZZLEFLASH_SPAWN		(48)		// マズルフラッシュの生成数
#define MUZZLEFLASH_EFF_LIFE	(8)			// マズルフラッシュの寿命
#define MUZZLEFLASH_SIZE		(0.57f)		// マズルフラッシュの大きさ
#define MUZZLEFLASH_SUB_SIZE	(-1.42f)	// マズルフラッシュの半径の減算量

// タンクファイア
#define TANKFIRE_COL		(D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f))	// タンクファイアの色
#define TANKFIRE_MOVE		(2.64f)		// タンクファイアの移動量
#define TANKFIRE_SPAWN		(58)		// タンクファイアの生成数
#define TANKFIRE_EFF_LIFE	(22)		// タンクファイアの寿命
#define TANKFIRE_SIZE		(2.57f)		// タンクファイアの大きさ
#define TANKFIRE_SUB_SIZE	(-3.42f)	// タンクファイアの半径の減算量

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const int CParticle3D::mc_aLife[CParticle3D::TYPE_MAX] =	// 寿命定数
{
	0,	// なし
	24,	// ダメージ
	1,	// 回復
	1,	// 植物踏みつぶし
	1,	// 小爆発
	1,	// 大爆発
	1,	// プレイヤーダメージ
	1,	// マズルフラッシュ
	1,	// タンクファイア
	24,	// バブル爆発
};

//************************************************************
//	子クラス [CParticle3D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CParticle3D::CParticle3D() : CObject(CObject::LABEL_PARTICLE)
{
	// メンバ変数をクリア
	m_pos	= VEC3_ZERO;	// 位置
	m_col	= XCOL_WHITE;	// 色
	m_type	= TYPE_NONE;	// 種類
	m_nLife	= 0;			// 寿命
}

//============================================================
//	デストラクタ
//============================================================
CParticle3D::~CParticle3D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CParticle3D::Init(void)
{
	// メンバ変数を初期化
	m_pos	= VEC3_ZERO;	// 位置
	m_col	= XCOL_WHITE;	// 色
	m_type	= TYPE_NONE;	// 種類
	m_nLife	= 0;			// 寿命

	// 成功を返す
	return S_OK;
}

//============================================================
//	初期化処理
//============================================================
HRESULT CParticle3D::Init(const EType type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// メンバ変数を初期化
	m_pos	= rPos;				// 位置
	m_col	= rCol;				// 色
	m_type	= type;				// 種類
	m_nLife	= mc_aLife[type];	// 寿命

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CParticle3D::Uninit(void)
{
	// パーティクル3Dオブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CParticle3D::Update(void)
{
	if (m_nLife > 0)
	{ // 寿命がある場合

		// 寿命を減算
		m_nLife--;
	}
	else
	{ // 寿命がない場合

		// パーティクル3Dオブジェクトの終了
		Uninit();

		// 関数を抜ける
		return;
	}

	switch (m_type)
	{ // 種類ごとの処理
	case TYPE_DAMAGE:

		// ダメージ
		Damage(m_pos, m_col);

		break;

	case TYPE_HEAL:

		// 回復
		Heal(m_pos, m_col);

		break;

	case TYPE_STOMP_PLANT:

		// 植物踏みつぶし
		StompPlant(m_pos, m_col);

		break;

	case TYPE_SMALL_EXPLOSION:

		// 小爆発
		SmallExplosion(m_pos);

		break;

	case TYPE_BIG_EXPLOSION:

		// 大爆発
		BigExplosion(m_pos);

		break;

	case TYPE_PLAYER_DAMAGE:

		// プレイヤーダメージ
		PlayerDamage(m_pos);

		break;

	case TYPE_MUZZLE_FLASH:

		// マズルフラッシュ
		MuzzleFlash(m_pos);

		break;

	case TYPE_TANK_FIRE:

		// タンクファイア
		TankFire(m_pos);

		break;

	case TYPE_BUBBLE_EXPLOSION:

		// バブル爆発
		BubbleExplosion(m_pos);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	描画処理
//============================================================
void CParticle3D::Draw(void)
{

}

//============================================================
//	ダメージ
//============================================================
void CParticle3D::Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// 変数を宣言
	D3DXVECTOR3 move = VEC3_ZERO;	// 移動量の代入用
	D3DXVECTOR3 rot  = VEC3_ZERO;	// 向きの代入用

	if ((m_nLife + 1) % 12 == 0)
	{ // 寿命が12の倍数の場合

		for (int nCntPart = 0; nCntPart < DMG_SPAWN; nCntPart++)
		{ // 生成されるエフェクト数分繰り返す

			// ベクトルをランダムに設定
			move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

			// ベクトルを正規化
			D3DXVec3Normalize(&move, &move);

			// 移動量を設定
			move.x *= DMG_MOVE;
			move.y *= DMG_MOVE;
			move.z *= DMG_MOVE;

			// 向きを設定
			rot = VEC3_ZERO;

			// エフェクト3Dオブジェクトの生成
			CEffect3D::Create
			( // 引数
				rPos,					// 位置
				DMG_SIZE,				// 半径
				CEffect3D::TYPE_NORMAL,	// テクスチャ
				DMG_EFF_LIFE,			// 寿命
				move,					// 移動量
				rot,					// 向き
				rCol,					// 色
				DMG_SUB_SIZE,			// 半径の減算量
				true,					// 加算合成状況
				LABEL_PARTICLE			// オブジェクトラベル
			);
		}
	}
}

//============================================================
//	回復
//============================================================
void CParticle3D::Heal(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// 変数を宣言
	D3DXVECTOR3 vec  = VEC3_ZERO;	// ベクトルの設定用
	D3DXVECTOR3 pos  = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 move = VEC3_ZERO;	// 移動量の代入用
	D3DXVECTOR3 rot  = VEC3_ZERO;	// 向きの代入用

	for (int nCntPart = 0; nCntPart < HEAL_SPAWN; nCntPart++)
	{ // 生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&vec, &vec);

		// 位置を設定
		pos = rPos + vec * HEAL_POSGAP;

		// 移動量を設定
		move = vec * HEAL_MOVE;

		// 向きを設定
		rot = VEC3_ZERO;

		// エフェクト3Dオブジェクトの生成
		CEffect3D::Create
		( // 引数
			pos,					// 位置
			HEAL_SIZE,				// 半径
			CEffect3D::TYPE_HEAL,	// テクスチャ
			HEAL_EFF_LIFE,			// 寿命
			move,					// 移動量
			rot,					// 向き
			rCol,					// 色
			HEAL_SUB_SIZE,			// 半径の減算量
			true,					// 加算合成状況
			LABEL_PARTICLE			// オブジェクトラベル
		);
	}
}

//============================================================
//	植物踏みつぶし
//============================================================
void CParticle3D::StompPlant(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// 変数を宣言
	D3DXVECTOR3 move = VEC3_ZERO;	// 移動量の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用

	for (int nCntPart = 0; nCntPart < STOMP_SPAWN; nCntPart++)
	{ // 生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を設定
		move.x *= STOMP_MOVE;
		move.y *= STOMP_MOVE;
		move.z *= STOMP_MOVE;

		// 向きを設定
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// エフェクト3Dオブジェクトの生成
		CEffect3D::Create
		( // 引数
			rPos,					// 位置
			STOMP_SIZE,				// 半径
			CEffect3D::TYPE_LEAF,	// テクスチャ
			STOMP_EFF_LIFE,			// 寿命
			move,					// 移動量
			rot,					// 向き
			rCol,					// 色
			STOMP_SUB_SIZE,			// 半径の減算量
			false,					// 加算合成状況
			LABEL_PARTICLE			// オブジェクトラベル
		);
	}
}

//============================================================
//	小爆発
//============================================================
void CParticle3D::SmallExplosion(const D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 vec  = VEC3_ZERO;	// ベクトルの設定用
	D3DXVECTOR3 pos  = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 move = VEC3_ZERO;	// 移動量の代入用
	D3DXVECTOR3 rot  = VEC3_ZERO;	// 向きの代入用
	int nLife = 0;	// 寿命の代入用

	for (int nCntPart = 0; nCntPart < SMALL_EXP_SMOKE_SPAWN; nCntPart++)
	{ // 生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&vec, &vec);

		// 位置を設定
		pos = rPos + vec * SMALL_EXP_SMOKE_POSGAP;

		// 移動量を設定
		move = vec * SMALL_EXP_SMOKE_MOVE;

		// 向きを設定
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// 寿命を設定
		nLife = (rand() % SMALL_EXP_SMOKE_RAND_LIFE) + SMALL_EXP_SMOKE_EFF_LIFE;

		// エフェクト3Dオブジェクトの生成
		CEffect3D::Create
		( // 引数
			pos,						// 位置
			SMALL_EXP_SMOKE_SIZE,		// 半径
			CEffect3D::TYPE_SMOKE,		// テクスチャ
			nLife,						// 寿命
			move,						// 移動量
			rot,						// 向き
			SMALL_EXP_SMOKE_COL,		// 色
			SMALL_EXP_SMOKE_SUB_SIZE,	// 半径の減算量
			false,						// 加算合成状況
			LABEL_PARTICLE				// オブジェクトラベル
		);
	}

	for (int nCntPart = 0; nCntPart < SMALL_EXP_FIRE_SPAWN; nCntPart++)
	{ // 生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&vec, &vec);

		// 位置を設定
		pos = rPos + vec * SMALL_EXP_FIRE_POSGAP;

		// 移動量を設定
		move = vec * SMALL_EXP_FIRE_MOVE;

		// 向きを設定
		rot = VEC3_ZERO;

		// 寿命を設定
		nLife = (rand() % SMALL_EXP_FIRE_RAND_LIFE) + SMALL_EXP_FIRE_EFF_LIFE;

		// エフェクト3Dオブジェクトの生成
		CEffect3D::Create
		( // 引数
			pos,						// 位置
			SMALL_EXP_FIRE_SIZE,		// 半径
			CEffect3D::TYPE_NORMAL,		// テクスチャ
			nLife,						// 寿命
			move,						// 移動量
			rot,						// 向き
			SMALL_EXP_FIRE_COL,			// 色
			SMALL_EXP_FIRE_SUB_SIZE,	// 半径の減算量
			true,						// 加算合成状況
			LABEL_PARTICLE				// オブジェクトラベル
		);
	}
}

//============================================================
//	大爆発
//============================================================
void CParticle3D::BigExplosion(const D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 vec  = VEC3_ZERO;	// ベクトルの設定用
	D3DXVECTOR3 pos  = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 move = VEC3_ZERO;	// 移動量の代入用
	D3DXVECTOR3 rot  = VEC3_ZERO;	// 向きの代入用
	int nLife = 0;	// 寿命の代入用

	for (int nCntPart = 0; nCntPart < BIG_EXP_SMOKE_SPAWN; nCntPart++)
	{ // 生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&vec, &vec);

		// 位置を設定
		pos = rPos + vec * BIG_EXP_SMOKE_POSGAP;

		// 移動量を設定
		move = vec * BIG_EXP_SMOKE_MOVE;

		// 向きを設定
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// 寿命を設定
		nLife = (rand() % BIG_EXP_SMOKE_RAND_LIFE) + BIG_EXP_SMOKE_EFF_LIFE;

		// エフェクト3Dオブジェクトの生成
		CEffect3D::Create
		( // 引数
			pos,					// 位置
			BIG_EXP_SMOKE_SIZE,		// 半径
			CEffect3D::TYPE_SMOKE,	// テクスチャ
			nLife,					// 寿命
			move,					// 移動量
			rot,					// 向き
			BIG_EXP_SMOKE_COL,		// 色
			BIG_EXP_SMOKE_SUB_SIZE,	// 半径の減算量
			false,					// 加算合成状況
			LABEL_PARTICLE			// オブジェクトラベル
		);
	}

	for (int nCntPart = 0; nCntPart < BIG_EXP_FIRE_SPAWN; nCntPart++)
	{ // 生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&vec, &vec);

		// 位置を設定
		pos = rPos + vec * BIG_EXP_FIRE_POSGAP;

		// 移動量を設定
		move = vec * BIG_EXP_FIRE_MOVE;

		// 向きを設定
		rot = VEC3_ZERO;

		// 寿命を設定
		nLife = (rand() % BIG_EXP_FIRE_RAND_LIFE) + BIG_EXP_FIRE_EFF_LIFE;

		// エフェクト3Dオブジェクトの生成
		CEffect3D::Create
		( // 引数
			pos,					// 位置
			BIG_EXP_FIRE_SIZE,		// 半径
			CEffect3D::TYPE_NORMAL,	// テクスチャ
			nLife,					// 寿命
			move,					// 移動量
			rot,					// 向き
			BIG_EXP_FIRE_COL,		// 色
			BIG_EXP_FIRE_SUB_SIZE,	// 半径の減算量
			true,					// 加算合成状況
			LABEL_PARTICLE			// オブジェクトラベル
		);
	}
}

//============================================================
//	プレイヤーダメージ
//============================================================
void CParticle3D::PlayerDamage(const D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 vec  = VEC3_ZERO;	// ベクトルの設定用
	D3DXVECTOR3 pos  = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 move = VEC3_ZERO;	// 移動量の代入用
	D3DXVECTOR3 rot  = VEC3_ZERO;	// 向きの代入用
	D3DXCOLOR   col  = XCOL_WHITE;	// 色の代入用
	int nSpawn = 0;	// 生成数の代入用
	int nLife = 0;	// 寿命の代入用

	// 生成数を設定
	nSpawn = (rand() % PLAY_DMG_EFF_SPAWN) + PLAY_DMG_RAND_SPAWN;

	for (int nCntPart = 0; nCntPart < nSpawn; nCntPart++)
	{ // 生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&vec, &vec);

		// 位置を設定
		pos = rPos + vec * PLAY_DMG_POSGAP;

		// 移動量を設定
		move = vec * PLAY_DMG_MOVE_S;

		// 向きを設定
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// 色を設定
		col.r = (float)(rand() % 80 + 20) / 100.0f;
		col.g = (float)(rand() % 20 + 80) / 100.0f;
		col.b = (float)(rand() % 80 + 20) / 100.0f;
		col.a = 1.0f;

		// 寿命を設定
		nLife = (rand() % PLAY_DMG_RAND_LIFE) + PLAY_DMG_EFF_LIFE;

		// エフェクト3Dオブジェクトの生成
		CEffect3D::Create
		( // 引数
			pos,						// 位置
			PLAY_DMG_SIZE_S,			// 半径
			CEffect3D::TYPE_PIECE_S,	// テクスチャ
			nLife,						// 寿命
			move,						// 移動量
			rot,						// 向き
			col,						// 色
			PLAY_DMG_SUB_SIZE,			// 半径の減算量
			true,						// 加算合成状況
			LABEL_PARTICLE				// オブジェクトラベル
		);
	}

	// 生成数を設定
	nSpawn = (rand() % PLAY_DMG_EFF_SPAWN) + PLAY_DMG_RAND_SPAWN;

	for (int nCntPart = 0; nCntPart < nSpawn; nCntPart++)
	{ // 生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&vec, &vec);

		// 位置を設定
		pos = rPos + vec * PLAY_DMG_POSGAP;

		// 移動量を設定
		move = vec * PLAY_DMG_MOVE_M;

		// 向きを設定
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// 色を設定
		col.r = (float)(rand() % 20 + 80) / 100.0f;
		col.g = (float)(rand() % 80 + 20) / 100.0f;
		col.b = (float)(rand() % 80 + 20) / 100.0f;
		col.a = 1.0f;

		// 寿命を設定
		nLife = (rand() % PLAY_DMG_RAND_LIFE) + PLAY_DMG_EFF_LIFE;

		// エフェクト3Dオブジェクトの生成
		CEffect3D::Create
		( // 引数
			pos,						// 位置
			PLAY_DMG_SIZE_M,			// 半径
			CEffect3D::TYPE_PIECE_M,	// テクスチャ
			nLife,						// 寿命
			move,						// 移動量
			rot,						// 向き
			col,						// 色
			PLAY_DMG_SUB_SIZE,			// 半径の減算量
			true,						// 加算合成状況
			LABEL_PARTICLE				// オブジェクトラベル
		);
	}

	// 生成数を設定
	nSpawn = (rand() % PLAY_DMG_EFF_SPAWN) + PLAY_DMG_RAND_SPAWN;

	for (int nCntPart = 0; nCntPart < nSpawn; nCntPart++)
	{ // 生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&vec, &vec);

		// 位置を設定
		pos = rPos + vec * PLAY_DMG_POSGAP;

		// 移動量を設定
		move = vec * PLAY_DMG_MOVE_L;

		// 向きを設定
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// 色を設定
		col.r = (float)(rand() % 80 + 20) / 100.0f;
		col.g = (float)(rand() % 80 + 20) / 100.0f;
		col.b = (float)(rand() % 20 + 80) / 100.0f;
		col.a = 1.0f;

		// 寿命を設定
		nLife = (rand() % PLAY_DMG_RAND_LIFE) + PLAY_DMG_EFF_LIFE;

		// エフェクト3Dオブジェクトの生成
		CEffect3D::Create
		( // 引数
			pos,						// 位置
			PLAY_DMG_SIZE_L,			// 半径
			CEffect3D::TYPE_PIECE_L,	// テクスチャ
			nLife,						// 寿命
			move,						// 移動量
			rot,						// 向き
			col,						// 色
			PLAY_DMG_SUB_SIZE,			// 半径の減算量
			true,						// 加算合成状況
			LABEL_PARTICLE				// オブジェクトラベル
		);
	}
}

//============================================================
//	マズルフラッシュ
//============================================================
void CParticle3D::MuzzleFlash(const D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 move = VEC3_ZERO;	// 移動量の代入用
	D3DXVECTOR3 rot  = VEC3_ZERO;	// 向きの代入用
	int nLife = 0;	// 寿命の代入用

	for (int nCntPart = 0; nCntPart < MUZZLEFLASH_SPAWN; nCntPart++)
	{ // 生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を設定
		move.x *= MUZZLEFLASH_MOVE;
		move.y *= MUZZLEFLASH_MOVE;
		move.z *= MUZZLEFLASH_MOVE;

		// 向きを設定
		rot = VEC3_ZERO;

		// 寿命を設定
		nLife = (rand() % 6) + MUZZLEFLASH_EFF_LIFE;

		// エフェクト3Dオブジェクトの生成
		CEffect3D::Create
		( // 引数
			rPos,					// 位置
			MUZZLEFLASH_SIZE,		// 半径
			CEffect3D::TYPE_NORMAL,	// テクスチャ
			nLife,					// 寿命
			move,					// 移動量
			rot,					// 向き
			MUZZLEFLASH_COL,		// 色
			MUZZLEFLASH_SUB_SIZE,	// 半径の減算量
			true,					// 加算合成状況
			LABEL_PARTICLE			// オブジェクトラベル
		);
	}
}

//============================================================
//	タンクファイア
//============================================================
void CParticle3D::TankFire(const D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 move = VEC3_ZERO;	// 移動量の代入用
	D3DXVECTOR3 rot  = VEC3_ZERO;	// 向きの代入用
	int nLife = 0;	// 寿命の代入用

	for (int nCntPart = 0; nCntPart < TANKFIRE_SPAWN; nCntPart++)
	{ // 生成されるエフェクト数分繰り返す

		// ベクトルをランダムに設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// ベクトルを正規化
		D3DXVec3Normalize(&move, &move);

		// 移動量を設定
		move.x *= TANKFIRE_MOVE;
		move.y *= TANKFIRE_MOVE;
		move.z *= TANKFIRE_MOVE;

		// 向きを設定
		rot = VEC3_ZERO;

		// 寿命を設定
		nLife = (rand() % 6) + TANKFIRE_EFF_LIFE;

		// エフェクト3Dオブジェクトの生成
		CEffect3D::Create
		( // 引数
			rPos,					// 位置
			TANKFIRE_SIZE,			// 半径
			CEffect3D::TYPE_NORMAL,	// テクスチャ
			nLife,					// 寿命
			move,					// 移動量
			rot,					// 向き
			TANKFIRE_COL,			// 色
			TANKFIRE_SUB_SIZE,		// 半径の減算量
			true,					// 加算合成状況
			LABEL_PARTICLE			// オブジェクトラベル
		);
	}
}

//============================================================
//	バブル爆発
//============================================================
void CParticle3D::BubbleExplosion(const D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 move = VEC3_ZERO;	// 移動量の代入用
	D3DXVECTOR3 rot  = VEC3_ZERO;	// 向きの代入用

	if ((m_nLife + 1) % 12 == 0)
	{ // 寿命が12の倍数の場合

		for (int nCntPart = 0; nCntPart < DMG_SPAWN; nCntPart++)
		{ // 生成されるエフェクト数分繰り返す

			// ベクトルをランダムに設定
			move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

			// ベクトルを正規化
			D3DXVec3Normalize(&move, &move);

			// 移動量を設定
			move.x *= DMG_MOVE;
			move.y *= DMG_MOVE;
			move.z *= DMG_MOVE;

			// 向きを設定
			rot = VEC3_ZERO;

			// エフェクト3Dオブジェクトの生成
			CEffect3D::Create
			( // 引数
				rPos,					// 位置
				DMG_SIZE,				// 半径
				CEffect3D::TYPE_BUBBLE,	// テクスチャ
				DMG_EFF_LIFE,			// 寿命
				move,					// 移動量
				rot,					// 向き
				XCOL_WHITE,				// 色
				DMG_SUB_SIZE,			// 半径の減算量
				true,					// 加算合成状況
				LABEL_PARTICLE			// オブジェクトラベル
			);
		}
	}
}

//============================================================
//	生成処理
//============================================================
CParticle3D *CParticle3D::Create(const EType type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// ポインタを宣言
	CParticle3D *pParticle3D = NULL;	// パーティクル3D生成用

	if (pParticle3D == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pParticle3D = new CParticle3D;	// パーティクル3D
	}
	else { assert(false); return NULL; }	// 使用中

	if (pParticle3D != NULL)
	{ // 確保に成功している場合

		// パーティクル3Dの初期化
		if (FAILED(pParticle3D->Init(type, rPos, rCol)))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pParticle3D;
			pParticle3D = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pParticle3D;
	}
	else { assert(false); return NULL; }	// 確保失敗
}
