//============================================================
//
//	プレイヤー処理 [player.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "player.h"
#include "playerEntry.h"
#include "manager.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "collision.h"
#include "fade.h"

#include "multiModel.h"
#include "objectOrbit.h"
#include "object2D.h"
#include "timerManager.h"
#include "retentionManager.h"
#include "rankingManager.h"
#include "stage.h"
#include "field.h"

#include "effect3D.h"
#include "particle3D.h"

#include "statusManager.h"
#include "flail.h"
#include "spawnpoint.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\TXT\\player.txt";	// プレイヤーセットアップテキスト

	const int	PRIORITY	= 3;		// プレイヤーの優先順位
	const float	GRAVITY		= 1.0f;		// 重力
	const float	RADIUS		= 50.0f;	// 半径
	const float	HEIGHT		= 100.0f;	// 縦幅

	const float	DASH_UP			= 10.0f;	// ダッシュ上昇量
	const float	DASH_SIDE		= 15.0f;	// ダッシュ横移動量
	const float	DASH_REV		= 0.25f;	// ダッシュの減算係数
	const float	DASH_MINMOVE	= 0.06f;	// ダッシュ再度可能になる移動量

	const float	KNOCK_UP	= 15.0f;	// ノック上昇量
	const float	KNOCK_SIDE	= 35.0f;	// ノック横移動量
	const int	DMG_KILLY	= 50;		// キルY座標のダメージ量

	const float MAX_KNOCK_RATE	= 4.0f;		// 最大吹っ飛び倍率
	const float	INVULN_ALPHA	= 0.5f;		// 無敵状態の透明度
	const int	INVULN_CNT		= 60;		// 無敵時間のフレーム数
	const float	ADD_SINROT		= 0.25f;	// 透明度ふわふわさせる際のサインカーブ向き加算量
	const float	MAX_ADD_ALPHA	= 0.25f;	// 透明度の最大加算量

	const float	NORMAL_JUMP_REV	= 0.16f;	// 通常状態時の空中の移動量の減衰係数
	const float	NORMAL_LAND_REV	= 0.16f;	// 通常状態時の地上の移動量の減衰係数

	const float	KNOCK_REV	= 0.08f;	// ノック状態時の移動量の減衰係数
	const float	REV_ROTA	= 0.15f;	// 向き変更の補正係数
	const float	STICK_REV	= 0.00015f;	// スティックの傾き量の補正係数

	const float	DEAD_ZONE	= (float)USHRT_MAX * 0.01f;	// スティックの無視する傾き量
	const float	SPAWN_ADD_ALPHA		= 0.0075f;			// スポーン状態時の透明度の加算量
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CPlayer::mc_apModelFile[] =	// モデル定数
{
	"data\\MODEL\\PLAYER\\00_waist.x",		// 腰
	"data\\MODEL\\PLAYER\\01_body.x",		// 体
	"data\\MODEL\\PLAYER\\02_head.x",		// 頭
	"data\\MODEL\\PLAYER\\03_shoulder_R.x",	// 左上腕
	"data\\MODEL\\PLAYER\\04_arm_R.x",		// 右上腕
	"data\\MODEL\\PLAYER\\05_hand_R.x",		// 左下腕
	"data\\MODEL\\PLAYER\\06_shoulder_L.x",	// 右下腕
	"data\\MODEL\\PLAYER\\07_arm_L.x",		// 左手
	"data\\MODEL\\PLAYER\\08_hand_L.x",		// 右手
	"data\\MODEL\\PLAYER\\09_thigh_R.x",	// 左太もも
	"data\\MODEL\\PLAYER\\10_leg_R.x",		// 右太もも
	"data\\MODEL\\PLAYER\\11_thigh_L.x",	// 左脛
	"data\\MODEL\\PLAYER\\12_leg_L.x",		// 右脛
	"data\\MODEL\\PLAYER\\13_stick.x",		// フレイル持ち手
};

//************************************************************
//	子クラス [CPlayer] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayer::CPlayer(const int nPad) : CObjectChara(CObject::LABEL_PLAYER, PRIORITY), m_nPadID(nPad)
{
	// メンバ変数をクリア
	m_pStatus		= NULL;			// ステータスの情報
	m_pFlail		= NULL;			// フレイルの情報
	m_oldPos		= VEC3_ZERO;	// 過去位置
	m_move			= VEC3_ZERO;	// 移動量
	m_destRot		= VEC3_ZERO;	// 目標向き
	m_dashRot		= VEC3_ZERO;	// ダッシュ向き
	m_state			= STATE_NONE;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nCounterFlail	= 0;			// フレイル管理カウンター
	m_fPlusMove		= 0.0f;			// プラス移動量
	m_fSinAlpha		= 0.0f;			// 透明向き
	m_bDash			= false;		// ダッシュ状況
	m_bJump			= false;		// ジャンプ状況
}

//============================================================
//	デストラクタ
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayer::Init(void)
{
	// メンバ変数を初期化
	m_pStatus		= NULL;			// ステータスの情報
	m_pFlail		= NULL;			// フレイルの情報
	m_oldPos		= VEC3_ZERO;	// 過去位置
	m_move			= VEC3_ZERO;	// 移動量
	m_destRot		= VEC3_ZERO;	// 目標向き
	m_dashRot		= VEC3_ZERO;	// ダッシュ向き
	m_state			= STATE_NONE;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nCounterFlail	= 0;			// フレイル管理カウンター
	m_fPlusMove		= 0.0f;			// プラス移動量
	m_fSinAlpha		= 0.0f;			// 透明向き
	m_bDash			= false;		// ダッシュ状況
	m_bJump			= true;			// ジャンプ状況

	// オブジェクトキャラクターの初期化
	if (FAILED(CObjectChara::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// セットアップの読み込み
	LoadSetup();

	// モデル情報の設定
	SetModelInfo();

	// ステータス情報の生成
	m_pStatus = CStatusManager::Create(m_nPadID);
	if (m_pStatus == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// フレイルの生成
	m_pFlail = CFlail::Create(*this,VEC3_ZERO);
	if (m_pFlail == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}
	m_pFlail->SetPlayerID(m_nPadID);

	// メインカラーを設定
	SetMainMaterial();
	SetEnableDepthShadow(true);
	SetEnableZTex(true);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayer::Uninit(void)
{
	// ステータス情報の破棄
	if (FAILED(m_pStatus->Release(m_pStatus)))
	{ assert(false); }	// 破棄失敗

	// フレイルの終了
	m_pFlail->Uninit();

	// オブジェクトキャラクターの終了
	CObjectChara::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayer::Update(void)
{
	// 変数を宣言
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション

	// 過去位置の更新
	UpdateOldPosition();

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない状態
		break;

	case STATE_SPAWN:	// スポーン状態

		// スポーン状態時の更新
		currentMotion = UpdateSpawn();

		break;

	case STATE_NORMAL:	// 通常状態

		// 通常状態の更新
		currentMotion = UpdateNormal();

		break;

	case STATE_KNOCK:	// ノック状態

		// ノック状態の更新
		currentMotion = UpdateKnock();

		break;

	case STATE_INVULN:	// 無敵状態

		// 無敵状態時の更新
		currentMotion = UpdateInvuln();

		break;

	case STATE_DEATH:	// 死亡状態
		break;

	default:
		assert(false);
		break;
	}

	// フレイルの更新
	if (D3DXVec3Length(&m_move) > 1.0f && m_nCounterFlail < 0)
	{
		D3DXVECTOR3 vecFlail = m_pFlail->GetVec3Position() - (m_pFlail->GetVec3PosOrg());
		float length, chainRot;

		vecFlail.y = 0.0f;
		length = D3DXVec3Length(&vecFlail);
		chainRot = atan2f(vecFlail.x, vecFlail.z);

		m_pFlail->SetChainRot(chainRot);
		m_pFlail->SetChainRotMove(chainRot);
		m_pFlail->SetLengthChain(length);
		m_pFlail->SetMove(VEC3_ZERO);
	}

	if (CManager::GetInstance()->GetRetentionManager()->GetNumSurvival() == 1)
	{ // 残り人数が1人の場合

		// 生存ランキングを更新 (一位を設定)
		CManager::GetInstance()->GetRetentionManager()->SetSurvivalRank(m_nPadID);
	}

	// フレイルの更新
	m_pFlail->Update();

	// ステータス情報の更新
	m_pStatus->Update();

	// モーション・オブジェクトキャラクターの更新
	UpdateMotion(currentMotion);
}

//============================================================
//	描画処理
//============================================================
void CPlayer::Draw(void)
{
	// ステータス情報の描画
	m_pStatus->Draw();

	// オブジェクトキャラクターの描画
	CObjectChara::Draw();
}

//============================================================
//	ノックバックヒット処理
//============================================================
void CPlayer::HitKnockBack(const int nDmg, const D3DXVECTOR3& vecKnock)
{
	if (IsDeath())
	{ // 死亡フラグが立っている場合

		return;
	}

	if (m_state != STATE_NORMAL)
	{ // 通常状態ではない場合

		return;
	}

	// 変数を宣言
	bool bDeath = false;	// 死亡状況

	// 死亡状況の設定
	{
		// ポインタを宣言
		CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// データ保存情報

		switch (pRetention->GetKillState())
		{ // 討伐条件ごとの処理
		case CRetentionManager::KILL_LIFE:	// 体力制

			if (m_pStatus->GetNumLife() <= 0)
			{ // 体力がすでにない場合

				return;
			}

			// 体力にダメージを与える
			m_pStatus->AddNumLife(-nDmg);

			if (m_pStatus->GetNumLife() <= 0)
			{ // 体力がなくなった場合

				// 死亡状態にする
				bDeath = true;
			}

			break;

		case CRetentionManager::KILL_KNOCK:	// 吹っ飛ばし制
			break;

		default:
			assert(false);
			break;
		}
	}

	// 状態の設定
	{
		// 変数を宣言
		D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置

		// カウンターを初期化
		m_nCounterState = 0;

		if (bDeath)
		{ // 死亡している場合

			// 死亡状態を設定
			SetState(STATE_DEATH);

			// 死亡モーションを設定
			SetMotion(MOTION_DEATH);

			// 爆発パーティクルを生成
			CParticle3D::Create(CParticle3D::TYPE_SMALL_EXPLOSION, D3DXVECTOR3(posPlayer.x, posPlayer.y + HEIGHT * 0.5f, posPlayer.z));
		}
		else
		{ // 死亡していない場合

			// NAKAMURA：ふっとび量の決め方きもければ変えて
			// 変数を宣言
			float fKnockRate = ((MAX_KNOCK_RATE - 1.0f) / (float)m_pStatus->GetNumMaxLife()) * m_pStatus->GetNumRate() + 1.0f;	// 吹っ飛ばし率

			// ノックバック移動量を設定
			m_move.x = fKnockRate * vecKnock.x * KNOCK_SIDE;
			m_move.y = KNOCK_UP;
			m_move.z = fKnockRate * vecKnock.z * KNOCK_SIDE;

			// 空中状態にする
			m_bJump = true;

			// ノック状態を設定
			SetState(STATE_KNOCK);

			// 吹っ飛びモーションを設定
			SetMotion(MOTION_KNOCK);

			// 爆発パーティクルを生成
			CParticle3D::Create(CParticle3D::TYPE_BIG_EXPLOSION, D3DXVECTOR3(posPlayer.x, posPlayer.y + HEIGHT * 0.5f, posPlayer.z));
		}

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT);	// ヒット音
	}

	// NAKAMURA：吹っ飛び率の決め方は任せます
	// 吹っ飛び率を加算
	m_pStatus->AddNumRate(100);
}

//============================================================
//	状態の設定処理
//============================================================
void CPlayer::SetState(const int nState)
{
	if (nState > NONE_IDX && nState < STATE_MAX)
	{ // 範囲内の場合

		if (m_state != STATE_DEATH)
		{ // 死亡状態じゃない場合

			// 引数の状態を設定
			m_state = (EState)nState;

			if (m_state == STATE_DEATH)
			{ // 死亡状態の場合

				// 生存ランキングを更新
				CManager::GetInstance()->GetRetentionManager()->SetSurvivalRank(m_nPadID);
			}
		}
	}
	else { assert(false); }
}

//============================================================
//	状態取得処理
//============================================================
int CPlayer::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	半径取得処理
//============================================================
float CPlayer::GetRadius(void) const
{
	// 半径を返す
	return RADIUS;
}

//============================================================
//	縦幅取得処理
//============================================================
float CPlayer::GetHeight(void) const
{
	// 縦幅を返す
	return HEIGHT;
}

//============================================================
//	更新状況の設定処理
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);	// 自身
	m_pFlail->SetEnableUpdate(bUpdate);	// フレイル
}

//============================================================
//	描画状況の設定処理
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);	// 自身
	m_pFlail->SetEnableDraw(bDraw);	// フレイル
}

//============================================================
//	UI描画状況の設定処理
//============================================================
void CPlayer::SetEnableDrawUI(const bool bDraw)
{
	// 引数の描画状況を設定
	m_pStatus->SetEnableDrawLife(bDraw);	// 体力
	m_pStatus->SetEnableDrawRate(bDraw);	// 吹っ飛び率
}

//============================================================
//	マトリックス取得処理
//============================================================
D3DXMATRIX CPlayer::GetMtxWorld(void) const
{
	// 変数を宣言
	D3DXMATRIX  mtxRot, mtxTrans, mtxWorld;		// 計算用マトリックス
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotPlayer.y, rotPlayer.x, rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, posPlayer.x, posPlayer.y, posPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスを返す
	return mtxWorld;
}

//============================================================
//	生成処理
//============================================================
CPlayer *CPlayer::Create(CScene::EMode mode, const int nPad)
{
	// ポインタを宣言
	CPlayer *pPlayer = NULL;	// プレイヤー生成用

	if (pPlayer == NULL)
	{ // 使用されていない場合

		// モードオーバー
		assert(mode > NONE_IDX && mode < CScene::MODE_MAX);

		switch (mode)
		{ // モードごとの処理
		case CScene::MODE_ENTRY:

			// メモリ確保
			pPlayer = new CPlayerEntry(nPad);	// プレイヤーエントリー

			break;

		case CScene::MODE_GAME:

			// メモリ確保
			pPlayer = new CPlayer(nPad);	// プレイヤー

			break;
		}
	}
	else { assert(false); return NULL; }	// 使用中

	if (pPlayer != NULL)
	{ // 使用されている場合

		// プレイヤーの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pPlayer;
			pPlayer = NULL;

			// 失敗を返す
			return NULL;
		}
	}

	// 確保したアドレスを返す
	return pPlayer;
}

//============================================================
//	出現の設定処理
//============================================================
void CPlayer::SetSpawn(void)
{
	// 変数を宣言
	CObject *pSpawnPoint = CSpawnPoint::GetSavePoint(m_nPadID);
	if (pSpawnPoint != NULL)
	{ // スポーンポイントがある場合

		// 位置を設定
		SetVec3Position(pSpawnPoint->GetVec3Position());

		// 向きを設定
		SetVec3Rotation(pSpawnPoint->GetVec3Rotation());
		m_destRot = pSpawnPoint->GetVec3Rotation();
	}
	else
	{ // スポーンポイントがない場合

		// 位置を設定
		SetVec3Position(VEC3_ZERO);

		// 向きを設定
		SetVec3Rotation(VEC3_ZERO);
		m_destRot = VEC3_ZERO;
	}

	// 情報を初期化
	SetState(STATE_SPAWN);	// スポーン状態の設定
	SetMotion(MOTION_IDOL);	// 待機モーションを設定

	// カウンターを初期化
	m_nCounterState = 0;	// 状態管理カウンター

	// 移動量を初期化
	m_move = VEC3_ZERO;

	// NAKAMURA：ここにフレイルを強制的に所持状態にする処理お願い

	// マテリアルを再設定
	ResetMaterial();
	
	// メインカラーを設定
	SetMainMaterial();

	// 透明度を透明に再設定
	SetAlpha(0.0f);

	// 自動描画をONにする
	SetEnableDraw(true);

	// サウンドの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SPAWN);	// 生成音
}

//============================================================
//	無敵の設定処理
//============================================================
void CPlayer::SetInvuln(void)
{
	// 情報を初期化
	SetState(STATE_INVULN);	// 無敵状態の設定

	// カウンターを初期化
	m_nCounterState = 0;	// 状態管理カウンター

	// マテリアルを再設定
	ResetMaterial();
	
	// メインカラーを設定
	SetMainMaterial();

	// 透明度を透明に再設定
	SetAlpha(INVULN_ALPHA);

	// 自動描画をONにする
	SetEnableDraw(true);

	// サウンドの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SPAWN);	// 生成音
}

//============================================================
//	キルY座標ヒット処理
//============================================================
void CPlayer::HitKillY(const int nDmg)
{
	if (IsDeath())
	{ // 死亡フラグが立っている場合

		return;
	}

	if (m_state != STATE_NORMAL
	&&  m_state != STATE_KNOCK
	&&  m_state != STATE_INVULN)
	{ // 通常・ノック・無敵状態ではない場合

		return;
	}

	// 変数を宣言
	bool bDeath = false;	// 死亡状況

	// 死亡状況の設定
	{
		// ポインタを宣言
		CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// データ保存情報

		switch (pRetention->GetKillState())
		{ // 討伐条件ごとの処理
		case CRetentionManager::KILL_LIFE:	// 体力制

			if (m_pStatus->GetNumLife() <= 0)
			{ // 体力がすでにない場合

				return;
			}

			// 体力にダメージを与える
			m_pStatus->AddNumLife(-nDmg);

			if (m_pStatus->GetNumLife() <= 0)
			{ // 体力がなくなった場合

				// 死亡状態にする
				bDeath = true;
			}

			break;

		case CRetentionManager::KILL_KNOCK:	// 吹っ飛ばし制

			// 死亡状態にする
			bDeath = true;

			break;

		default:
			assert(false);
			break;
		}
	}

	// 状態の設定
	{
		// 変数を宣言
		D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置

		// カウンターを初期化
		m_nCounterState = 0;

		if (bDeath)
		{ // 死亡している場合

			// 死亡状態を設定
			SetState(STATE_DEATH);

			// 死亡モーションを設定
			SetMotion(MOTION_DEATH);

			// 爆発パーティクルを生成
			CParticle3D::Create(CParticle3D::TYPE_SMALL_EXPLOSION, D3DXVECTOR3(posPlayer.x, posPlayer.y + HEIGHT * 0.5f, posPlayer.z));
		}
		else
		{ // 死亡していない場合

			// 再出現させる
			SetSpawn();

			// 爆発パーティクルを生成
			CParticle3D::Create(CParticle3D::TYPE_BIG_EXPLOSION, D3DXVECTOR3(posPlayer.x, posPlayer.y + HEIGHT * 0.5f, posPlayer.z));
		}

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT);	// ヒット音
	}
}

//============================================================
//	パッドインデックス取得処理
//============================================================
int CPlayer::GetPadID(void) const
{
	// パッドインデックスを返す
	return m_nPadID;
}

//============================================================
//	フレイルカウンター取得処理
//============================================================
int CPlayer::GetCounterFlail(void) const
{
	// フレイルカウンターを返す
	return m_nCounterFlail;
}

//============================================================
//	自身のメインカラーマテリアル設定処理
//============================================================
void CPlayer::SetMainMaterial(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MODEL_MAX; nCntPlayer++)
	{ // パーツの最大数分繰り返す

		// モデルの先頭マテリアルを設定
		SetMaterial(GetMainMaterial(), nCntPlayer, 0);
	}
}

//============================================================
//	自身のメインカラーマテリアル取得処理
//============================================================
D3DXMATERIAL CPlayer::GetMainMaterial(void)
{
	switch (m_nPadID)
	{ // パッドインデックスごとの処理
	case PLAYER_RED:	// プレイヤー1：赤

		// 赤マテリアルを返す
		return material::Red();

	case PLAYER_BLUE:	// プレイヤー2：青

		// 青マテリアルを返す
		return material::Blue();

	case PLAYER_YELLOW:	// プレイヤー3：黄

		// 黄マテリアルを返す
		return material::Yellow();

	case PLAYER_GREEN:	// プレイヤー4：緑

		// 緑マテリアルを返す
		return material::Green();

	default:	// 例外処理
		assert(false);
		return material::White();
	}
}

//============================================================
//	モーション・オブジェクトキャラクターの更新処理
//============================================================
void CPlayer::UpdateMotion(int nMotion)
{
	// 変数を宣言
	int nAnimMotion = GetMotionType();	// 現在再生中のモーション

	if (nMotion != NONE_IDX)
	{ // モーションが設定されている場合

		if (IsMotionLoop(nAnimMotion))
		{ // ループするモーションだった場合

			if (nAnimMotion != nMotion)
			{ // 現在のモーションが再生中のモーションと一致しない場合

				// 現在のモーションの設定
				SetMotion(nMotion);
			}
		}
	}

	// オブジェクトキャラクターの更新
	CObjectChara::Update();
}

//============================================================
//	スポーン状態時の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateSpawn(void)
{
	// 変数を宣言
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション

	// フェードアウト状態時の更新
	UpdateFadeOut(SPAWN_ADD_ALPHA);

	if (GetAlpha() >= INVULN_ALPHA)
	{ // 無敵時の透明度以上の場合

		// 無敵の人にする
		SetInvuln();
	}

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	通常状態時の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateNormal(void)
{
	// 変数を宣言
	EMotion currentMotion = MOTION_IDOL;		// 現在のモーション
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// ポインタを宣言
	CStage *pStage = CScene::GetStage();	// ステージ情報
	if (pStage == NULL)
	{ // ステージが使用されていない場合

		// 処理を抜ける
		assert(false);
		return MOTION_IDOL;
	}

	// 移動操作
	currentMotion = UpdateMove(posPlayer);

	// 重力の更新
	UpdateGravity();

	// 着地判定
	UpdateLanding(posPlayer);

	// ダッシュ更新
	UpdateDash();

	// 向き更新
	UpdateRotation(rotPlayer);

	// ステージ範囲外の補正
	pStage->LimitPosition(posPlayer, RADIUS);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// キルY座標との当たり判定
	if (pStage->CollisionKillY(posPlayer))
	{ // 死亡座標に到達していた場合

		// キルY座標ヒット処理
		HitKillY(DMG_KILLY);
	}

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	ノック状態時の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateKnock(void)
{
	// 変数を宣言
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// ポインタを宣言
	CStage *pStage = CScene::GetStage();	// ステージ情報
	if (pStage == NULL)
	{ // ステージが使用されていない場合

		// 処理を抜ける
		assert(false);
		return MOTION_KNOCK;
	}

	// 重力の更新
	UpdateGravity();

	// 着地判定
	if (UpdateLanding(posPlayer))
	{ // 着地した場合

		// 無敵の人にする
		SetInvuln();
	}

	// 向き更新
	UpdateRotation(rotPlayer);

	// ステージ範囲外の補正
	pStage->LimitPosition(posPlayer, RADIUS);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// キルY座標との当たり判定
	if (pStage->CollisionKillY(posPlayer))
	{ // 死亡座標に到達していた場合

		// キルY座標ヒット処理
		HitKillY(DMG_KILLY);
	}

	// 吹っ飛びモーションを返す
	return MOTION_KNOCK;
}

//============================================================
//	無敵状態時の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateInvuln(void)
{
	// 変数を宣言
	float fAddAlpha = 0.0f;	// 透明度の加算量

	// 透明度を上げる
	m_fSinAlpha += ADD_SINROT;
	useful::NormalizeRot(m_fSinAlpha);	// 向き正規化

	// 透明度加算量を求める
	fAddAlpha = (MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinAlpha) - 1.0f);

	// 透明度を設定
	SetAlpha(INVULN_ALPHA + fAddAlpha);

	// カウンターを加算
	m_nCounterState++;

	if (m_nCounterState > INVULN_CNT)
	{ // 無敵時間の終了カウントになった場合

		// 通常状態を設定
		SetState(STATE_NORMAL);

		// 透明度を不透明に再設定
		SetAlpha(1.0f);
	}

	// 通常状態の処理を行い、その返り値のモーションを返す
	return UpdateNormal();
}

//============================================================
//	過去位置の更新処理
//============================================================
void CPlayer::UpdateOldPosition(void)
{
	// 過去位置を更新
	m_oldPos = GetVec3Position();
}

//============================================================
//	移動量・目標向きの更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateMove(D3DXVECTOR3& rPos)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド
	CCamera			*pCamera	= CManager::GetInstance()->GetCamera();		// カメラ

	if (pCamera == NULL) { assert(false); return MOTION_IDOL; }	// 非使用中

	// PC操作
#if 0
	if (pKeyboard->IsPress(DIK_W))
	{
		if (pKeyboard->IsPress(DIK_A))
		{
			// 移動量を更新
			m_move.x += sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;
			m_move.z += cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;
		}
		else if (pKeyboard->IsPress(DIK_D))
		{
			// 移動量を更新
			m_move.x -= sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;
			m_move.z -= cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;
		}
		else
		{
			// 移動量を更新
			m_move.x += sinf(pCamera->rot.y) * MOVE_PLAYER;
			m_move.z += cosf(pCamera->rot.y) * MOVE_PLAYER;
		}
	}
	else if (pKeyboard->IsPress(DIK_S))
	{
		if (pKeyboard->IsPress(DIK_A))
		{
			// 移動量を更新
			m_move.x += sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;
			m_move.z += cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;
		}
		else if (pKeyboard->IsPress(DIK_D))
		{
			// 移動量を更新
			m_move.x -= sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;
			m_move.z -= cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;
		}
		else
		{
			// 移動量を更新
			m_move.x -= sinf(pCamera->rot.y) * MOVE_PLAYER;
			m_move.z -= cosf(pCamera->rot.y) * MOVE_PLAYER;
		}
	}
	else if (pKeyboard->IsPress(DIK_A))
	{
		// 移動量を更新
		m_move.x += sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;
		m_move.z += cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;
	}
	else if (pKeyboard->IsPress(DIK_D))
	{
		// 移動量を更新
		m_move.x -= sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;
		m_move.z -= cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;
	}
#endif

	// 変数を宣言
	D3DXVECTOR3 vecStick = D3DXVECTOR3((float)pPad->GetPressLStickX(m_nPadID), (float)pPad->GetPressLStickY(m_nPadID), 0.0f);	// スティック各軸の倒し量
	float fStick = sqrtf(vecStick.x * vecStick.x + vecStick.y * vecStick.y) * 0.5f;	// スティックの倒し量

	if (DEAD_ZONE < fStick)
	{ // デッドゾーン以上の場合

		// 変数を宣言
		float fMove = fStick * STICK_REV;	// プレイヤー移動量

		if (!m_bDash)
		{ // ダッシュ中ではない場合

			// 移動量を更新
			m_move.x += sinf(pPad->GetPressLStickRot(m_nPadID) + pCamera->GetVec3Rotation().y + HALF_PI) * fMove;
			m_move.z += cosf(pPad->GetPressLStickRot(m_nPadID) + pCamera->GetVec3Rotation().y + HALF_PI) * fMove;
		}
		else
		{ // ダッシュ中の場合

			// 移動量を更新
			m_move.x += sinf(m_dashRot.y) * fMove;
			m_move.z += cosf(m_dashRot.y) * fMove;
		}

		if (m_pFlail->GetLengthChain() >= 1000.0f)
		{
			// 移動量を更新
			m_move.x *= 0.7f;
			m_move.z *= 0.7f;
		}

		// 目標向きを設定
		m_destRot.y = atan2f(-m_move.x, -m_move.z);
	}

	// カウンターの値によって挙動を変更
	if (m_nCounterFlail > flail::FLAIL_DEF)
	{// 0より大きい時

		if ((CManager::GetInstance()->GetKeyboard()->IsPress(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsPress(CInputPad::KEY_R1, m_nPadID) == TRUE) && m_nCounterFlail <= flail::FLAIL_CHARGE)
		{// 投げるボタンが押されている時
		 // カウンターアップ
			m_nCounterFlail++;

			// 一定値でカウンターを止める
			if (m_nCounterFlail > flail::FLAIL_CHARGE)
			{
				m_nCounterFlail = flail::FLAIL_CHARGE;
			}

			// 溜めてる間鉄球を振り回す
			m_pFlail->SetChainRot(m_pFlail->GetChainRot() - (0.003f * m_nCounterFlail));

			// 移動量を更新
			m_move.x *= 0.5f;
			m_move.z *= 0.5f;

			// 目標向きを設定
			m_destRot.y = m_pFlail->GetChainRotMove() + D3DX_PI * 0.5f;
		}

		// 投擲
		if ((CManager::GetInstance()->GetKeyboard()->IsRelease(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsRelease(CInputPad::KEY_R1, m_nPadID) == TRUE) && m_nCounterFlail != flail::FLAIL_THROW)
		{
			// 溜めた時間に応じて飛距離増加
			D3DXVECTOR3 move = VEC3_ZERO;
			move.x = (sinf(m_pFlail->GetChainRotMove()) * 5.0f * m_nCounterFlail);
			move.z = (cosf(m_pFlail->GetChainRotMove()) * 5.0f * m_nCounterFlail);
			m_pFlail->SetMove(move);

			// 目標角度に合わせる
			//m_pFlail->SetChainRot(m_destRot.y);

			// カウンターの設定
			m_nCounterFlail = flail::FLAIL_THROW;
		}

		if (m_nCounterFlail == flail::FLAIL_THROW)
		{
			// 移動量を更新
			m_move.x = 0.0f;
			m_move.z = 0.0f;

			// フレイルが止まったらカウンターを次の段階へ
			if (m_pFlail->GetLengthChain() == 20.0f * (flail::FLAIL_NUM - 1))
			{
				m_nCounterFlail = flail::FLAIL_DROP;
			}
		}
	}
	else if (m_nCounterFlail == flail::FLAIL_DEF)
	{
		// カウンターアップ開始
		if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_R1, m_nPadID) == TRUE)
		{
			m_nCounterFlail++;
		}
	}
	else
	{
		// 鉄球とプレイヤーの距離が一定未満の時プレイヤー位置に鉄球固定
		if (m_pFlail->GetLengthChain() == 0.0f)
		{
			m_nCounterFlail = 1;
			m_pFlail->SetMove(VEC3_ZERO);
		}

		// 引き戻す
		if (CManager::GetInstance()->GetKeyboard()->IsPress(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsPress(CInputPad::KEY_R1, m_nPadID) == TRUE)
		{
			m_nCounterFlail -= 1;

			if (m_nCounterFlail < -60)
			{
				m_nCounterFlail = -60;
			}
			
			// 溜めた時間に応じて飛距離増加
			D3DXVECTOR3 move = VEC3_ZERO;
			move.x = (sinf(m_pFlail->GetChainRotMove()) * (m_nCounterFlail * -m_nCounterFlail));
			move.z = (cosf(m_pFlail->GetChainRotMove()) * (m_nCounterFlail * -m_nCounterFlail));
			m_pFlail->SetMove(move);

			// 移動量を更新
			m_move.x = 0.0f;
			m_move.z = 0.0f;
		}

		// 投擲
		if ((CManager::GetInstance()->GetKeyboard()->IsRelease(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsRelease(CInputPad::KEY_R1, m_nPadID) == TRUE))
		{
			m_nCounterFlail = flail::FLAIL_DROP;
		}
	}

	vecStick = D3DXVECTOR3((float)pPad->GetPressRStickX(m_nPadID), (float)pPad->GetPressRStickY(m_nPadID), 0.0f);	// スティック各軸の倒し量
	fStick = sqrtf(vecStick.x * vecStick.x + vecStick.y * vecStick.y) * 0.5f;	// スティックの倒し量

	if (DEAD_ZONE < fStick)
	{ // デッドゾーン以上の場合
		m_pFlail->SetChainRotMove(CManager::GetInstance()->GetPad()->GetPressRStickRot(m_nPadID) + 1.57f);
	}

	// 目標角度変更処理
	if (CManager::GetInstance()->GetKeyboard()->IsPress(DIK_A) == TRUE)
	{
		m_pFlail->SetChainRotMove(m_pFlail->GetChainRotMove() - 0.015f);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsPress(DIK_D) == TRUE)
	{
		m_pFlail->SetChainRotMove(m_pFlail->GetChainRotMove() + 0.015f);
	}

	// 位置を表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[位置]：%f %f %f\n", rPos.x, rPos.y, rPos.z);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[カウンター]：%d\n", m_nCounterFlail);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[鎖長さ]：%f\n", m_pFlail->GetLengthChain());

	// 待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
//	ダッシュの更新処理
//============================================================
void CPlayer::UpdateDash(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// キーボード
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// パッド
	CCamera			*pCamera	= CManager::GetInstance()->GetCamera();		// カメラ

	if (pCamera == NULL) { assert(false); }	// 非使用中

	// 変数を宣言
	D3DXVECTOR3 vecStick = D3DXVECTOR3((float)pPad->GetPressLStickX(m_nPadID), (float)pPad->GetPressLStickY(m_nPadID), 0.0f);	// スティック各軸の倒し量
	float fStick = sqrtf(vecStick.x * vecStick.x + vecStick.y * vecStick.y) * 0.5f;	// スティックの倒し量

	if (DEAD_ZONE < fStick)
	{ // デッドゾーン以上の場合

		if (pKeyboard->IsTrigger(DIK_SPACE) || pPad->IsTrigger(CInputPad::KEY_A, m_nPadID))
		{ // ダッシュの操作が行われた場合

			if (!m_bDash)
			{ // ダッシュしていない場合

				// 上移動量を加算
				m_move.y += DASH_UP;

				// プラス移動量を加算
				m_fPlusMove += DASH_SIDE;

				// ダッシュ時の向きを保存
				m_dashRot.y = pPad->GetPressLStickRot(m_nPadID) + pCamera->GetVec3Rotation().y + HALF_PI;

				// ダッシュしている状態にする
				m_bDash = true;

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_JUMP);	// ジャンプ音
			}
		}
	}

	if (m_bDash)
	{ // ダッシュしている場合

		// 移動量を更新
		m_move.x += sinf(m_dashRot.y) * m_fPlusMove;
		m_move.z += cosf(m_dashRot.y) * m_fPlusMove;

		// プラス移動量を減算
		m_fPlusMove += (0.0f - m_fPlusMove) * DASH_REV;

		if (fabsf(m_fPlusMove) < DASH_MINMOVE)
		{ // 移動量が一定値より少なくなった場合

			// 再ダッシュできるようにする
			m_fPlusMove = 0.0f;
			m_bDash = false;
		}
	}
}

//============================================================
//	重力の更新処理
//============================================================
void CPlayer::UpdateGravity(void)
{
	// 重力を加算
	m_move.y -= GRAVITY;
}

//============================================================
//	着地状況の更新処理
//============================================================
bool CPlayer::UpdateLanding(D3DXVECTOR3& rPos)
{
	// 変数を宣言
	bool bLand = false;	// 着地状況

	// ジャンプしている状態にする
	m_bJump = true;

	// 位置の更新
	// 地盤の当たり判定・着地判定
	if (CollisionGroundBlock(rPos))
	{ // 地盤に着地していた場合

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		m_bJump = false;
	}

	// 地面の着地判定
	if (CScene::GetStage()->LandFieldPosition(rPos, m_move))
	{ // プレイヤーが着地していた場合

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		m_bJump = false;
	}

	// 着地状況を返す
	return bLand;
}

//============================================================
//	向きの更新処理
//============================================================
void CPlayer::UpdateRotation(D3DXVECTOR3& rRot)
{
	// 変数を宣言
	float fDiffRot = 0.0f;	// 差分向き

	// 目標向きの正規化
	useful::NormalizeRot(m_destRot.y);

	// 目標向きまでの差分を計算
	fDiffRot = m_destRot.y - rRot.y;

	// 差分向きの正規化
	useful::NormalizeRot(fDiffRot);

	// 向きの更新
	rRot.y += fDiffRot * REV_ROTA;

	// 向きの正規化
	useful::NormalizeRot(rRot.y);
}

//============================================================
//	フェードアウト状態時の更新処理
//============================================================
bool CPlayer::UpdateFadeOut(const float fAdd)
{
	// 変数を宣言
	bool bAlpha = false;		// 透明状況
	float fAlpha = GetAlpha();	// 透明度

	// プレイヤー自身の描画を再開
	CObject::SetEnableDraw(true);

	// 透明度を上げる
	fAlpha += fAdd;

	if (fAlpha >= GetMaxAlpha())
	{ // 透明度が上がり切った場合

		// 透明度を補正
		fAlpha = GetMaxAlpha();

		// 不透明になり切った状態にする
		bAlpha = true;
	}

	// 透明度を設定
	SetAlpha(fAlpha);

	// 透明状況を返す
	return bAlpha;
}

//============================================================
//	フェードイン状態時の更新処理
//============================================================
bool CPlayer::UpdateFadeIn(const float fSub)
{
	// 変数を宣言
	bool bAlpha = false;		// 透明状況
	float fAlpha = GetAlpha();	// 透明度

	// 透明度を下げる
	fAlpha -= fSub;

	if (fAlpha <= 0.0f)
	{ // 透明度が下がり切った場合

		// 透明度を補正
		fAlpha = 0.0f;

		// 透明になり切った状態にする
		bAlpha = true;

		// プレイヤー自身の描画を停止
		CObject::SetEnableDraw(false);
	}

	// 透明度を設定
	SetAlpha(fAlpha);

	// 透明状況を返す
	return bAlpha;
}

//============================================================
//	地盤との一軸ごとの当たり判定
//============================================================
bool CPlayer::ResponseSingleGround(const EAxis axis, D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(RADIUS, 0.0f, RADIUS);		// プレイヤー最小大きさ
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(RADIUS, HEIGHT, RADIUS);	// プレイヤー最大大きさ
	bool bMin = false;	// 不の方向の判定状況
	bool bMax = false;	// 正の方向の判定状況
	bool bHit = false;	// 着地の判定情報

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		// ポインタを宣言
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// 先頭オブジェクト

		if (pObjectTop != NULL)
		{ // 先頭が存在する場合

			// ポインタを宣言
			CObject *pObjCheck = pObjectTop;	// オブジェクト確認用

			while (pObjCheck != NULL)
			{ // オブジェクトが使用されている場合繰り返す

				// 変数を宣言
				D3DXVECTOR3 posGround = VEC3_ZERO;		// 地盤位置
				D3DXVECTOR3 rotGround = VEC3_ZERO;		// 地盤向き
				D3DXVECTOR3 sizeMinGround = VEC3_ZERO;	// 地盤最小大きさ
				D3DXVECTOR3 sizeMaxGround = VEC3_ZERO;	// 地盤最大大きさ

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_GROUND)
				{ // オブジェクトラベルが地盤ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 地盤の位置を設定
				posGround = pObjCheck->GetVec3Position();

				// 地盤の向きを設定
				rotGround = pObjCheck->GetVec3Rotation();

				// 地盤の最小の大きさを設定
				sizeMinGround = pObjCheck->GetVec3Sizing();
				sizeMinGround.y *= 2.0f;	// 縦の大きさを倍にする

				// 地盤の最大の大きさを設定
				sizeMaxGround = pObjCheck->GetVec3Sizing();
				sizeMaxGround.y = 0.0f;		// 縦の大きさを初期化

				switch (axis)
				{ // 判定軸ごとの処理
				case AXIS_X:	// X軸

					// X軸の衝突判定
					collision::ResponseSingleX
					( // 引数
						rPos,			// 判定位置
						m_oldPos,		// 判定過去位置
						posGround,		// 判定目標位置
						sizeMaxPlayer,	// 判定サイズ(右・上・後)
						sizeMinPlayer,	// 判定サイズ(左・下・前)
						sizeMaxGround,	// 判定目標サイズ(右・上・後)
						sizeMinGround,	// 判定目標サイズ(左・下・前)
						&m_move			// 移動量
					);

					break;

				case AXIS_Y:	// Y軸

					// Y軸の衝突判定
					collision::ResponseSingleY
					( // 引数
						rPos,			// 判定位置
						m_oldPos,		// 判定過去位置
						posGround,		// 判定目標位置
						sizeMaxPlayer,	// 判定サイズ(右・上・後)
						sizeMinPlayer,	// 判定サイズ(左・下・前)
						sizeMaxGround,	// 判定目標サイズ(右・上・後)
						sizeMinGround,	// 判定目標サイズ(左・下・前)
						&m_move,		// 移動量
						true,			// X判定
						true,			// Z判定
						&bMin,			// 下からの判定
						&bMax			// 上からの判定
					);

					if (bMax)
					{ // 上から当たっていた場合

						// 着地している状況にする
						bHit = true;
					}

					break;

				case AXIS_Z:	// Z軸

					// Z軸の衝突判定
					collision::ResponseSingleZ
					( // 引数
						rPos,			// 判定位置
						m_oldPos,		// 判定過去位置
						posGround,		// 判定目標位置
						sizeMaxPlayer,	// 判定サイズ(右・上・後)
						sizeMinPlayer,	// 判定サイズ(左・下・前)
						sizeMaxGround,	// 判定目標サイズ(右・上・後)
						sizeMinGround,	// 判定目標サイズ(左・下・前)
						&m_move			// 移動量
					);

					break;

				default:	// 例外処理
					assert(false);
					break;
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	// 各軸の判定情報を返す
	return bHit;
}

//============================================================
//	ブロックとの一軸ごとの当たり判定
//============================================================
bool CPlayer::ResponseSingleBlock(const EAxis axis, D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(RADIUS, 0.0f, RADIUS);		// プレイヤー最小大きさ
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(RADIUS, HEIGHT, RADIUS);	// プレイヤー最大大きさ
	bool bHit = false;	// 着地の判定情報

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // 優先順位の総数分繰り返す

		// ポインタを宣言
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// 先頭オブジェクト

		if (pObjectTop != NULL)
		{ // 先頭が存在する場合

			// ポインタを宣言
			CObject *pObjCheck = pObjectTop;	// オブジェクト確認用

			while (pObjCheck != NULL)
			{ // オブジェクトが使用されている場合繰り返す

				// 変数を宣言
				D3DXVECTOR3 posBlock = VEC3_ZERO;	// ブロック位置
				D3DXVECTOR3 rotBlock = VEC3_ZERO;	// ブロック向き
				D3DXVECTOR3 sizeBlock = VEC3_ZERO;	// ブロック大きさ

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_BLOCK)
				{ // オブジェクトラベルがブロックではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// ブロックの位置を設定
				posBlock = pObjCheck->GetVec3Position();

				// ブロックの向きを設定
				rotBlock = pObjCheck->GetVec3Rotation();

				// ブロックの最小の大きさを設定
				sizeBlock = pObjCheck->GetVec3Sizing();

				switch (axis)
				{ // 判定軸ごとの処理
				case AXIS_X:	// X軸

					// X軸の衝突判定
					collision::ResponseSingleX
					( // 引数
						rPos,			// 判定位置
						m_oldPos,		// 判定過去位置
						posBlock,		// 判定目標位置
						sizeMaxPlayer,	// 判定サイズ(右・上・後)
						sizeMinPlayer,	// 判定サイズ(左・下・前)
						sizeBlock,		// 判定目標サイズ(右・上・後)
						sizeBlock,		// 判定目標サイズ(左・下・前)
						&m_move,		// 移動量
						false			// Y判定
					);

					break;

				case AXIS_Z:	// Z軸

					// Z軸の衝突判定
					collision::ResponseSingleZ
					( // 引数
						rPos,			// 判定位置
						m_oldPos,		// 判定過去位置
						posBlock,		// 判定目標位置
						sizeMaxPlayer,	// 判定サイズ(右・上・後)
						sizeMinPlayer,	// 判定サイズ(左・下・前)
						sizeBlock,		// 判定目標サイズ(右・上・後)
						sizeBlock,		// 判定目標サイズ(左・下・前)
						&m_move,		// 移動量
						true,			// X判定
						false			// Y判定
					);

					break;

				default:	// 例外処理
					assert(false);
					break;
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	// 各軸の判定情報を返す
	return bHit;
}

//============================================================
//	地盤・ブロックとの当たり判定
//============================================================
bool CPlayer::CollisionGroundBlock(D3DXVECTOR3& rPos)
{
	// 変数を宣言
	bool bLand = false;	// 着地状況

	// 移動量を加算
	rPos.x += m_move.x;

	// X軸の当たり判定
	ResponseSingleGround(AXIS_X, rPos);
	ResponseSingleBlock(AXIS_X, rPos);

	// 移動量を加算
	rPos.y += m_move.y;

	// Y軸の当たり判定
	bLand = ResponseSingleGround(AXIS_Y, rPos);

	// 移動量を加算
	rPos.z += m_move.z;

	// Z軸の当たり判定
	ResponseSingleGround(AXIS_Z, rPos);
	ResponseSingleBlock(AXIS_Z, rPos);

	if (m_state == STATE_KNOCK)
	{ // ノック状態の場合

		m_move.x += (0.0f - m_move.x) * KNOCK_REV;
		m_move.z += (0.0f - m_move.z) * KNOCK_REV;
	}
	else
	{ // それ以外の状態の場合

		// 移動量を減衰
		if (m_bJump)
		{ // 空中の場合

			m_move.x += (0.0f - m_move.x) * NORMAL_JUMP_REV;
			m_move.z += (0.0f - m_move.z) * NORMAL_JUMP_REV;
		}
		else
		{ // 地上の場合

			m_move.x += (0.0f - m_move.x) * NORMAL_LAND_REV;
			m_move.z += (0.0f - m_move.z) * NORMAL_LAND_REV;
		}
	}

	// 着地状況を返す
	return bLand;
}

//============================================================
//	セットアップ処理
//============================================================
void CPlayer::LoadSetup(void)
{
	// 変数を宣言
	CMotion::SMotionInfo info;		// ポーズの代入用
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	int nID			= 0;	// インデックスの代入用
	int nParentID	= 0;	// 親インデックスの代入用
	int nNowPose	= 0;	// 現在のポーズ番号
	int nNowKey		= 0;	// 現在のキー番号
	int nLoop		= 0;	// ループのON/OFFの変換用
	int nEnd		= 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ポーズ代入用の変数を初期化
	memset(&info, 0, sizeof(info));

	// ファイルを読み込み形式で開く
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// キャラクターの設定
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // 読み込んだ文字列が CHARACTERSET の場合

				do
				{ // 読み込んだ文字列が END_CHARACTERSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // 読み込んだ文字列が PARTSSET の場合

						do
						{ // 読み込んだ文字列が END_PARTSSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // 読み込んだ文字列が INDEX の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nID);			// モデルのインデックスを読み込む
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // 読み込んだ文字列が PARENT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nParentID);	// モデルの親のインデックスを読み込む
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &pos.x);		// X座標を読み込む
								fscanf(pFile, "%f", &pos.y);		// Y座標を読み込む
								fscanf(pFile, "%f", &pos.z);		// Z座標を読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &rot.x);		// X向きを読み込む
								fscanf(pFile, "%f", &rot.y);		// Y向きを読み込む
								fscanf(pFile, "%f", &rot.z);		// Z向きを読み込む
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// 読み込んだ文字列が END_PARTSSET ではない場合ループ

						// パーツ情報の設定
						CObjectChara::SetPartsInfo(nID, nParentID, pos, rot, mc_apModelFile[nID]);
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// 読み込んだ文字列が END_CHARACTERSET ではない場合ループ
			}

			// モーションの設定
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // 読み込んだ文字列が MOTIONSET の場合

				// 現在のポーズ番号を初期化
				nNowPose = 0;

				do
				{ // 読み込んだ文字列が END_MOTIONSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LOOP") == 0)
					{ // 読み込んだ文字列が LOOP の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nLoop);		// ループのON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						info.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // 読み込んだ文字列が NUM_KEY の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &info.nNumKey);	// キーの総数を読み込む
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // 読み込んだ文字列が KEYSET の場合

						// 現在のキー番号を初期化
						nNowKey = 0;

						do
						{ // 読み込んだ文字列が END_KEYSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // 読み込んだ文字列が FRAME の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%d", &info.aKeyInfo[nNowPose].nFrame);	// キーが切り替わるまでのフレーム数を読み込む
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // 読み込んだ文字列が KEY の場合

								do
								{ // 読み込んだ文字列が END_KEY ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // 読み込んだ文字列が POS の場合

										fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X位置を読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y位置を読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z位置を読み込む

										// 読み込んだ位置にパーツの初期位置を加算
										info.aKeyInfo[nNowPose].aKey[nNowKey].pos += GetPartsPosition(nNowKey);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // 読み込んだ文字列が ROT の場合

										fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X向きを読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y向きを読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z向きを読み込む

										// 読み込んだ向きにパーツの初期向きを加算
										info.aKeyInfo[nNowPose].aKey[nNowKey].rot += GetPartsRotation(nNowKey);

										// 初期向きを正規化
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// 読み込んだ文字列が END_KEY ではない場合ループ

								// 現在のキー番号を加算
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// 読み込んだ文字列が END_KEYSET ではない場合ループ

						// 現在のポーズ番号を加算
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// 読み込んだ文字列が END_MOTIONSET ではない場合ループ

				// モーション情報の設定
				CObjectChara::SetMotionInfo(info);
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "プレイヤーセットアップファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
