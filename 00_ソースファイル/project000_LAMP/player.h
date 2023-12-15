//============================================================
//
//	プレイヤーヘッダー [player.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectChara.h"
#include "scene.h"
#include "object3D.h"
//************************************************************
//	前方宣言
//************************************************************
class CStatusManager;	// ステータスマネージャークラス
class CFlail;			// フレイルクラス
class CPlayerAI;		// フレイルクラス
class CObjectBillboard;
//************************************************************
//	クラス定義
//************************************************************
// プレイヤークラス
class CPlayer : public CObjectChara
{
public:
	// モデル列挙
	enum EModel
	{
		MODEL_WAIST = 0,	// 腰
		MODEL_BODY,			// 体
		MODEL_HEAD,			// 頭
		MODEL_SHOULDER_R,	// 右肩
		MODEL_ARM_R,		// 右腕
		MODEL_HAND_R,		// 右手
		MODEL_SHOULDER_L,	// 左肩
		MODEL_ARM_L,		// 左腕
		MODEL_HAND_L,		// 左手
		MODEL_THIGH_R,		// 右もも
		MODEL_LEG_R,		// 右足
		MODEL_THIGH_L,		// 左もも
		MODEL_LEG_L,		// 左足
		MODEL_STICK,		// 棒
		MODEL_MAX			// この列挙型の総数
	};

	// モーション列挙
	enum EMotion
	{
		MOTION_IDOL = 0,	// 待機モーション
		MOTION_MOVE,		// 移動モーション
		MOTION_ATTACK,		// 攻撃モーション
		MOTION_CHARGE,		// チャージモーション
		MOTION_DASH,		// ダッシュモーション
		MOTION_PULL,		// 引きずりモーション
		MOTION_KNOCK,		// 吹っ飛びモーション
		MOTION_LAND,		// 着地モーション
		MOTION_DROWN,		// 溺れモーション
		MOTION_DEATH,		// 死亡モーション
		MOTION_EMOTE_PROUD,	// 胸を張るエモートモーション
		MOTION_EMOTE_SLEEP,	// 寝るエモートモーション
		MOTION_MAX			// この列挙型の総数
	};

	// プレイヤー列挙
	enum EPlayer
	{
		PLAYER_RED = 0,	// プレイヤー1：赤
		PLAYER_BLUE,	// プレイヤー2：青
		PLAYER_YELLOW,	// プレイヤー3：黄
		PLAYER_GREEN,	// プレイヤー4：緑
		PLAYER_MAX		// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_SPAWN,	// スポーン状態
		STATE_NORMAL,	// 通常状態
		STATE_KNOCK,	// ノック状態
		STATE_INVULN,	// 無敵状態
		STATE_DROWN,	// 溺れ状態
		STATE_DEATH,	// 死亡状態
		STATE_MAX		// この列挙型の総数
	};

	// 軸列挙
	enum EAxis
	{
		AXIS_X = 0,	// X軸
		AXIS_Y,		// Y軸
		AXIS_Z,		// Z軸
		AXIS_MAX	// この列挙型の総数
	};
	//アイテム効果
	enum EItem
	{
		ITEM_EMPTY = 0,
		ITEM_HEAL ,// 回復
		ITEM_BOOST_ATTACK,
		ITEM_BOOST_KNOCKBACK,
		ITEM_SUPERARMOR,
		ITEM_BIGFLAIL,
		ITEM_LONGFLAIL,
		ITEM_GHOSTFLAIL,
		ITEM_MULTIFLAIL,
		ITEM_BURNINGFLAIL,
		ITEM_MAX
	};

	struct SItemEffect
	{
		EItem type;	//種別
		int nLife;	//効果時間
	};
	// コンストラクタ
	explicit CPlayer(const int nPad);

	// デストラクタ
	~CPlayer();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void Hit(void) override;		// アイテムヒット

	void SetState(const int nState) override;	// 状態設定
	int GetState(void) const override;			// 状態取得
	float GetRadius(void) const override;		// 半径取得
	float GetHeight(void) const override;		// 縦幅取得

	void SetEnableUpdate(const bool bUpdate) override;	// 更新状況設定
	void SetEnableDraw(const bool bDraw) override;		// 描画状況設定
	D3DXMATRIX GetMtxWorld(void) const override;		// マトリックス取得

	// 静的メンバ関数
	static CPlayer *Create(CScene::EMode mode, const int nPad, const bool bAI = false);	// 生成

	// メンバ関数
	void HitKnockBack(const int nDmg, const D3DXVECTOR3& vecKnock, CPlayer *pAttack);	// ノックバックヒット
	void HitKillY(const int nDmg);	// キルY座標ヒット
	void SetSpawn(void);	// 出現設定
	void SetInvuln(void);	// 無敵設定

	void SetEnableDrawUI(const bool bDraw);		// UI描画状況設定
	int GetPadID(void) const;					// パッドインデックス取得
	CPlayer *GetLastAttackPlayer(void) const;	// 最後に攻撃したプレイヤー取得
	int GetCounterFlail(void) const;			// フレイルカウンター取得
	void SetCounterFlail(const int nCounterFlail);		// フレイルカウンター取得

	CFlail *GetFlail(void) const;		// フレイルカウンター取得

	void SetItemPermanent(EItem Item);
	SItemEffect GetTemporaryItem() { return m_SItemTemporary; }
protected:
	// メンバ関数
	void UpdateMotion(int nMotion);			// モーション・オブジェクトキャラクターの更新
	void SetMainMaterial(void);				// 自身のメインカラーマテリアル設定
	D3DXMATERIAL GetMainMaterial(void);		// 自身のメインカラーマテリアル取得

private:
	// メンバ関数
	void LoadSetup(void);		// セットアップ
	EMotion UpdateSpawn(void);	// スポーン状態時の更新
	EMotion UpdateNormal(void);	// 通常状態時の更新
	EMotion UpdateKnock(void);	// ノック状態時の更新
	EMotion UpdateInvuln(void);	// 無敵状態時の更新
	EMotion UpdateDrown(void);	// 溺れ状態時の更新
	void UpdateDeath(void);		// 死亡状態時の更新

	void PlayEmote(EMotion& rAnim);		// エモート操作

	void UpdateOldPosition(void);			// 過去位置の更新
	EMotion UpdateMove(D3DXVECTOR3& rPos);	// 移動量・目標向きの更新
	void UpdateDash(void);					// ダッシュの更新
	void UpdateGravity(void);				// 重力の更新
	bool UpdateLanding(D3DXVECTOR3& rPos);	// 着地状況の更新
	void UpdateRotation(D3DXVECTOR3& rRot);	// 向きの更新
	bool UpdateFadeOut(const float fAdd);	// フェードアウト状態時の更新
	bool UpdateFadeIn(const float fSub);	// フェードイン状態時の更新

	bool ResponseSingleGround(const EAxis axis, D3DXVECTOR3& rPos);	// 地盤との一軸ごとの当たり判定
	bool ResponseSingleBlock(const EAxis axis, D3DXVECTOR3& rPos);	// ブロックとの一軸ごとの当たり判定
	bool CollisionGroundBlock(D3DXVECTOR3& rPos);	// 地盤・ブロックとの当たり判定
	bool CollisionObstacle(D3DXVECTOR3& rPos);		// 障害物との当たり判定
	bool CollisionPlayer(D3DXVECTOR3& rPos);		// 障害物との当たり判定

	// 静的メンバ変数
	static const char *mc_apModelFile[];	// モデル定数

	// メンバ変数
	SItemEffect m_SItemPermanent[2];//永続的なアイテムの効果
	SItemEffect m_SItemTemporary;	//一時的なアイテムの効果
	CStatusManager *m_pStatus;		// ステータスの情報
	CFlail		*m_pFlail;			// フレイルの情報
	CPlayer		*m_pFinalAttack;	// 攻撃者の情報
	CPlayerAI	*m_pAI;				// AI情報
	D3DXVECTOR3	m_oldPos;			// 過去位置
	D3DXVECTOR3	m_move;				// 移動量
	D3DXVECTOR3	m_destRot;			// 目標向き
	D3DXVECTOR3	m_dashRot;			// ダッシュ向き
	EState		m_state;			// 状態
	CObject3D * m_pGuide;			// 投擲方向
	CObjectBillboard * m_pPlayerGuide;
	CObjectBillboard * m_pClown;
	int			m_motionOld;		// 過去モーション
	int			m_nCounterState;	// 状態管理カウンター
	int			m_nCounterFlail;	// フレイル管理カウンター
	int			m_nCounterAttack;	// 最終攻撃カウンター
	float		m_fPlusMove;		// プラス移動量
	float		m_fSinAlpha;		// 透明向き
	bool		m_bDash;			// ダッシュ状況
	bool		m_bJump;			// ジャンプ状況
	bool		m_bAI;				// AI状況
	bool		m_bHook;			// AI状況
	const int	m_nPadID;			// パッドインデックス
};

#endif	// _PLAYER_H_
