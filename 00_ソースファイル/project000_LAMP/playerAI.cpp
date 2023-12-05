//============================================================
//
//	AI処理 [playerAI.cpp]
//	Author：中村陸
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "playerAI.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sound.h"
#include "flail.h"
#include "collision.h"
#include "retentionManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float	DEAD_ZONE = (float)USHRT_MAX * 0.01f;	// スティックの無視する傾き量

	const float	DASH_UP = 10.0f;	// ダッシュ上昇量
	const float	DASH_SIDE = 15.0f;	// ダッシュ横移動量
	const float	DASH_REV = 0.25f;	// ダッシュの減算係数
	const float	DASH_MINMOVE = 0.06f;	// ダッシュ再度可能になる移動量

	const float STICK_LENGTH = 2.458f;
}

//============================================================
//	コンストラクタ
//============================================================
CPlayerAI::CPlayerAI()
{
	//初期値代入
	m_pFlail = NULL;
	m_pPos = NULL;
	m_pMove = NULL;
	m_pDestRot = NULL;
	m_pCounterFlail = NULL;
	m_nPadID = 0;
	fRotstickR = 0.0f;
}

//============================================================
//	デストラクタ
//============================================================
CPlayerAI::~CPlayerAI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayerAI::Init
(
	CFlail* pFlail,
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pMove,
	D3DXVECTOR3* pDestRot,
	int* pCounterFlail,
	const int nPadID
)
{
	//初期値代入
	m_pFlail = pFlail;
	m_pPos = pPos;
	m_pMove = pMove;
	m_pDestRot = pDestRot;
	m_pCounterFlail = pCounterFlail;
	m_nPadID = nPadID;

	// 成功を返す
	return S_OK;
}

//============================================================
//	AI全般をまとめる場所
//============================================================
CPlayer::EMotion CPlayerAI::playerAI
(
	const int nMotionOld
)
{
	m_nMotionOld = nMotionOld;

	AIselect();

	return CPlayer::MOTION_IDOL;
}

//============================================================
//	行動選択に関するAI
//============================================================
void CPlayerAI::AIselect()
{
	float fLengthMin = flail::FLAIL_RADIUS * (m_pFlail->GetNumChain() - 1);
	int nApproach = 0;
	int nApproachNum = 0;

	for (int nCntPlayer = 0; nCntPlayer < CManager::GetInstance()->GetRetentionManager()->GetNumPlayer(); nCntPlayer++)
	{
		CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);

		if (player != NULL && nCntPlayer != m_nPadID && player->GetState() != CPlayer::STATE_DEATH)
		{
			D3DXVECTOR3 vec;
			float length;

			// プレイヤーとフレイルのベクトルを求める
			vec = player->GetVec3Position() - *m_pPos;
			vec.y = 0.0f;	// Yは無視

			// 距離を求める
			length = D3DXVec3Length(&vec);

			if (length < flail::FLAIL_RADIUS * (m_pFlail->GetNumChain() - 1))
			{
				if (length < fLengthMin)
				{
					fLengthMin = length;
					nApproachNum = nCntPlayer;
				}

				nApproach++;
			}
		}
	}

	if (nApproach == 1)
	{
		CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(nApproachNum);
		D3DXVECTOR3 vec;

		// プレイヤーとフレイルのベクトルを求める
		vec = player->GetVec3Position() - *m_pPos;
		vec.y = 0.0f;	// Yは無視

		fRotstickR = atan2f(vec.x, vec.z);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[AI目標角度]：%f\n", fRotstickR);
	}
}

//============================================================
//	攻撃に関するAI
//============================================================
void CPlayerAI::AIatack()
{

}

//============================================================
//	移動に関するAI
//============================================================
void CPlayerAI::AImove()
{

}

//============================================================
//	ダッシュに関するAI
//============================================================
void CPlayerAI::AIDash
(
	D3DXVECTOR3& rMove,
	D3DXVECTOR3& rDashRot,
	D3DXVECTOR3& rDestRot,
	float& rPlusMove,
	bool& rDash
)
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		// カメラ

	if (pCamera == NULL) { assert(false); }	// 非使用中

	// 変数を宣言
	float fStick = D3DXVec3Length(&rMove);	// スティックの倒し量

	if (-1.0f < fStick)
	{ // デッドゾーン以上の場合

		D3DXVECTOR3 dashPos = VEC3_ZERO;
		float rotMove;
		bool bJumpSelect = false;
		rotMove = m_pDestRot->y + HALF_PI;

		// 移動量を更新
		dashPos.x += sinf(rotMove + pCamera->GetVec3Rotation().y + HALF_PI) * 700.0f;
		dashPos.y = -1.0f;
		dashPos.z += cosf(rotMove + pCamera->GetVec3Rotation().y + HALF_PI) * 700.0f;

		if (Collision(dashPos))
		{
			int nProb = rand() % 10000;

			if (nProb > 9990)
			{
				bJumpSelect = true;
			}
			else
			{
				bJumpSelect = false;
			}
		}
		else
		{
			int nProb = rand() % 1000;

			if (nProb > 9999)
			{
				bJumpSelect = true;
			}
			else
			{
				bJumpSelect = false;
			}
		}

		if (bJumpSelect)
		{ // ダッシュの操作が行われた場合

			if (!rDash)
			{ // ダッシュしていない場合

			  // 上移動量を加算
				rMove.y += DASH_UP;

				// プラス移動量を加算
				rPlusMove += DASH_SIDE;

				// ダッシュ時の向きを保存
				rDashRot.y = rotMove + pCamera->GetVec3Rotation().y + HALF_PI;

				m_pDestRot->y = rotMove + pCamera->GetVec3Rotation().y - HALF_PI;

				// ダッシュしている状態にする
				rDash = true;

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_JUMP);	// ジャンプ音
			}
		}
	}

	//if (rDash)
	//{ // ダッシュしている場合

	//  // 移動量を更新
	//	rMove.x += sinf(rDashRot.y) * rPlusMove;
	//	rMove.z += cosf(rDashRot.y) * rPlusMove;

	//	// プラス移動量を減算
	//	rPlusMove += (0.0f - rPlusMove) * DASH_REV;

	//	if (fabsf(rPlusMove) < DASH_MINMOVE)
	//	{ // 移動量が一定値より少なくなった場合

	//	  // 再ダッシュできるようにする
	//		rPlusMove = 0.0f;
	//		rDash = false;
	//	}
	//}
}

//============================================================
//	生成処理
//============================================================
CPlayerAI *CPlayerAI::Create
( // 引数
	CFlail* pFlail,
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pMove,
	D3DXVECTOR3* pDestRot,
	int* pCounterFlail,
	const int nPadID
)
{
	// ポインタを宣言
	CPlayerAI *pAI = NULL;	// モデルUI生成用

	if (pAI == NULL)
	{ // 使用されていない場合

	  // メモリ確保
		pAI = new CPlayerAI;	// モデルUI
	}
	else { assert(false); return NULL; }	// 使用中

	if (pAI != NULL)
	{ // 使用されている場合

	  // モデルUIの初期化
		if (FAILED(pAI->Init(pFlail, pPos, pMove, pDestRot, pCounterFlail, nPadID)))
		{ // 初期化に失敗した場合

		  // メモリ開放
			delete pAI;
			pAI = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pAI;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	当たり判定処理(地面、ブロック、障害物)
//============================================================
bool CPlayerAI::Collision(D3DXVECTOR3& rPos)
{
	if (CollisionGround(CPlayer::AXIS_X, rPos))
	{
		return true;
	}

	return false;
}
bool CPlayerAI::CollisionGround(const CPlayer::EAxis axis, D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(40.0f, 40.0f, 40.0f);		// プレイヤー最小大きさ
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(40.0f, 40.0f, 40.0f);		// プレイヤー最大大きさ
	bool bMin = false;	// 不の方向の判定状況
	bool bMax = false;	// 正の方向の判定状況
	bool bHit = false;	// 着地の判定情報
	bool bHitBox = false;	// 接触の判定情報
	bool bHitBoxCheck = false;	// 接触の判定情報

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
				//sizeMaxGround.y = 0.0f;		// 縦の大きさを初期化

				if (collision::SquareTrigger(posGround, rPos, rotGround, sizeMaxGround, sizeMinGround))
				{
					bHitBoxCheck = true;
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	return bHitBoxCheck;
}