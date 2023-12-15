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
#include "obstacle.h"

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
	const float	STICK_REV = 0.00015f;	// スティックの傾き量の補正係数
}

//============================================================
//	コンストラクタ
//============================================================
CPlayerAI::CPlayerAI()
{
	//初期値代入
	m_pFlail = NULL;
	m_pDashRot = VEC3_ZERO;
	m_stateAI = STATEAI_MOVE;
	m_currentMotion = CPlayer::MOTION_IDOL;
	m_nCounterFlail = 0;
	m_nPadID = 0;
	m_nCountAI = 0;
	m_fRotstickR = 0.0f;
	m_bDash = false;
	m_bAttack = false;
	m_bMove = false;
	m_bEmote = false;
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
HRESULT CPlayerAI::Init(const int nPadID)
{
	m_nPadID = nPadID;

	m_fRotstickL = 0.0f;

	// 成功を返す
	return S_OK;
}

//============================================================
//	AI全般をまとめる場所
//============================================================
CPlayer::EMotion CPlayerAI::playerAI
(
	CFlail* pFlail,
	D3DXVECTOR3& pPos,
	D3DXVECTOR3& rMove,
	D3DXVECTOR3& pDestRot,
	int pCounterFlail,
	const int nMotionOld
)
{
	m_currentMotion = CPlayer::MOTION_IDOL;		// 現在のモーション
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPadID);

	//値代入
	m_pFlail = pFlail;
	m_nCounterFlail = pCounterFlail;
	m_nMotionOld = nMotionOld;

	m_currentMotion = AIselect(pFlail,pPos, rMove, pDestRot, pCounterFlail, nMotionOld);

	player->SetCounterFlail(m_nCounterFlail);

	return m_currentMotion;
}

//============================================================
//	行動選択に関するAI
//============================================================
CPlayer::EMotion CPlayerAI::AIselect
(
	CFlail* pFlail,
	D3DXVECTOR3& rPos,
	D3DXVECTOR3& rMove,
	D3DXVECTOR3& rDestRot,
	int pCounterFlail,
	const int nMotionOld
)
{
	float fLengthMin = flail::FLAIL_RADIUS * (m_pFlail->GetNumChain() - 1);
	int nApproach = 0;
	m_nApproachNum = 0;

	for (int nCntPlayer = 0; nCntPlayer < 4; nCntPlayer++)
	{
		CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);

		if (player != NULL)
		{
			if (nCntPlayer != m_nPadID && player->GetState() != CPlayer::STATE_DEATH)
			{
				D3DXVECTOR3 vec;
				float length;

				// プレイヤーとフレイルのベクトルを求める
				vec = player->GetVec3Position() - rPos;
				vec.y = 0.0f;	// Yは無視

				// 距離を求める
				length = D3DXVec3Length(&vec);

				if (length < flail::FLAIL_RADIUS * (m_pFlail->GetNumChain() - 1))
				{
					if (length < fLengthMin)
					{
						fLengthMin = length;
						m_nApproachNum = nCntPlayer;
					}

					nApproach++;
				}
			}
		}
	}

	m_nCountAI++;

	if (m_nCountAI % 180 == 0)
	{
		m_nCountAI = 0;

		int nProb = rand() % 10000;

		switch (m_stateAI)
		{
		case CPlayerAI::STATEAI_NONE:

			if (m_nCounterFlail == flail::FLAIL_DROP)
			{
				if (nProb > 9500)
				{
					m_stateAI = STATEAI_NONE;
				}
				else if (nProb > 9000)
				{
					m_stateAI = STATEAI_MOVE;
				}
				else
				{
					m_stateAI = STATEAI_ATTACK;
				}
			}
			else
			{
				if (nProb > 9500)
				{
					m_stateAI = STATEAI_NONE;
				}
				else if (nProb > 2000)
				{
					m_stateAI = STATEAI_MOVE;
				}
				else
				{
					m_stateAI = STATEAI_ATTACK;
				}
			}

			break;

		case CPlayerAI::STATEAI_MOVE:

			if (nProb > 9000)
			{
				m_stateAI = STATEAI_NONE;
			}
			else if (nProb > 8000)
			{
				m_stateAI = STATEAI_MOVE;

				nProb = rand() % 10000;

				if (nProb > 5000)
				{
					m_fRotstickL = (float)((rand() % 629) - 314);
				}
				else
				{

				}
			}
			else
			{
				m_stateAI = STATEAI_ATTACK;
			}

			break;

		case CPlayerAI::STATEAI_ATTACK:

			if (nApproach > 0 && (m_nCounterFlail <= flail::FLAIL_THROW && m_nCounterFlail > flail::FLAIL_DEF))
			{
				m_stateAI = STATEAI_ATTACK;
			}
			else
			{
				if (nProb > 6000)
				{
					m_stateAI = STATEAI_NONE;
				}
				else if (nProb > 2000)
				{
					m_stateAI = STATEAI_MOVE;
				}
				else
				{
					m_stateAI = STATEAI_ATTACK;
				}
			}

			break;

		case CPlayerAI::STATEAI_EMOTE:

			m_stateAI = STATEAI_EMOTE;

		case CPlayerAI::STATEAI_MAX:

			m_stateAI = STATEAI_NONE;

			break;

		default:

			break;
		}
	}

	if (CManager::GetInstance()->GetRetentionManager()->GetNumSurvival() == 0 && m_stateAI != STATEAI_EMOTE)
	{ // 残り人数が1人の場合

		m_stateAI = STATEAI_EMOTE;
		m_bEmote = true;
	}
	
	switch (m_stateAI)
	{
	case CPlayerAI::STATEAI_NONE:

		m_bMove = false;
		m_bAttack = false;

		break;

	case CPlayerAI::STATEAI_MOVE:

		if (m_nCounterFlail == flail::FLAIL_CHARGE)
		{
			m_bAttack = true;
		}
		else
		{
			m_bAttack = false;
		}

		m_bMove = true;

		break;

	case CPlayerAI::STATEAI_ATTACK:

		if (nApproach == 1)
		{
			CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nApproachNum);
			D3DXVECTOR3 vec, rodPos;

			rodPos.x = player->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld()._41;
			rodPos.y = player->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld()._42;
			rodPos.z = player->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld()._43;

			// プレイヤーとフレイルのベクトルを求める
			vec = rodPos - rPos;
			vec.y = 0.0f;	// Yは無視

			m_fRotstickR = atan2f(vec.x, vec.z);

			useful::NormalizeRot(m_fRotstickR);

			m_pFlail->SetChainRotTarget(m_fRotstickR);

			m_fRotstickL = m_fRotstickR - (D3DX_PI * 1.0f);

			if (D3DXVec3Length(&vec) < flail::FLAIL_RADIUS * (float)(((float)m_nCounterFlail / (float)flail::FLAIL_CHARGE) * (m_pFlail->GetNumChain() - 1)))
			{
				m_bAttack = false;
			}
			else
			{
				m_bAttack = true;
			}

			if (D3DXVec3Length(&vec) < 1200.0f)
			{
				m_bMove = false;
			}
			else
			{
				m_bMove = true;
			}
		}
		else if (nApproach > 1)
		{
			m_fRotstickR = 0.0f;
			m_nApproachNum = -1;

			int nProb = rand() % 10000;

			if (nProb > 3000 && m_nCounterFlail == flail::FLAIL_CHARGE)
			{
				m_bAttack = false;
			}
			else
			{
				m_bAttack = true;
			}

			m_bMove = true;
		}
		else
		{
			m_bAttack = false;
			m_bMove = true;
		}

		break;

	case CPlayerAI::STATEAI_EMOTE:

		m_bAttack = false;
		m_bMove = false;

		break;

	case CPlayerAI::STATEAI_MAX:

		break;

	default:
		break;
	}

	//CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[近くの敵]：%d\n", nApproach);

	m_currentMotion = AImove(pFlail, rPos, rMove, rDestRot, pCounterFlail, nMotionOld);
	m_currentMotion = AIattack(pFlail,rPos, rMove, rDestRot, pCounterFlail, nMotionOld);
	m_currentMotion = AIemote(rDestRot);

	return m_currentMotion;
}

//============================================================
//	攻撃に関するAI
//============================================================
CPlayer::EMotion CPlayerAI::AIattack
(
	CFlail* pFlail,
	D3DXVECTOR3& rPos,
	D3DXVECTOR3& rMove,
	D3DXVECTOR3& rDestRot,
	int pCounterFlail,
	const int nMotionOld
)
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPadID);

	// カウンターの値によって挙動を変更
	if (m_nCounterFlail > flail::FLAIL_DEF)
	{// 0より大きい時

		if (m_bAttack && m_nCounterFlail <= flail::FLAIL_CHARGE)
		{// 投げるボタンが押されている時
		 // カウンターアップ
			m_nCounterFlail++;

			// 一定値でカウンターを止める
			if (m_nCounterFlail > flail::FLAIL_CHARGE)
			{
				m_nCounterFlail = flail::FLAIL_CHARGE;
			}

			// 溜めてる間鉄球を振り回す
			m_pFlail->SetChainRotMove((-0.002f * m_nCounterFlail) - 0.12f);

			m_pFlail->SetLengthTarget(flail::FLAIL_RADIUS * 5.0f);

			// 移動量を更新
			rMove.x *= 1.0f - (0.0042f * m_nCounterFlail);
			rMove.z *= 1.0f - (0.0042f * m_nCounterFlail);

			// 目標向きを設定
			rDestRot.y = m_pFlail->GetChainRotTarget() + D3DX_PI;

			// チャージモーションを設定
			m_currentMotion = CPlayer::MOTION_CHARGE;
		}

		// 投擲
		if (m_bAttack == false && m_nCounterFlail != flail::FLAIL_THROW)
		{
			// 溜めた時間に応じて飛距離増加
			D3DXVECTOR3 move = VEC3_ZERO;
			float lengthTarget = flail::FLAIL_RADIUS * (float)(((float)m_nCounterFlail / (float)flail::FLAIL_CHARGE) * (m_pFlail->GetNumChain() - 1));
			move.x = (sinf(m_pFlail->GetChainRotTarget()) * 5.0f * m_nCounterFlail);
			move.z = (cosf(m_pFlail->GetChainRotTarget()) * 5.0f * m_nCounterFlail);
			m_pFlail->SetMove(move);
			m_pFlail->SetChainRotMove(m_pFlail->GetChainRotMove());

			D3DXVECTOR3 posFlail = m_pFlail->GetVec3Position();
			if (posFlail.y < 0.0f)
			{
				posFlail.y = 0.0f;
				m_pFlail->SetVec3Position(posFlail);
			}

			if (m_nApproachNum != -1)
			{
				lengthTarget = flail::FLAIL_RADIUS * (float)(((float)m_nCounterFlail / (float)flail::FLAIL_CHARGE) * (m_pFlail->GetNumChain() - 1));

				if (lengthTarget < flail::FLAIL_RADIUS * flail::FLAIL_NUM_MIN)
				{
					lengthTarget = flail::FLAIL_RADIUS * flail::FLAIL_NUM_MIN;
				}

				// 目標角度に合わせる
				m_pFlail->ShotFlail(m_pFlail->GetChainRotTarget() - D3DX_PI * 0.5f);
				m_pFlail->SetChainRotMove(0.0f);
			}
			else
			{
				lengthTarget = flail::FLAIL_RADIUS * (float)(((float)m_nCounterFlail / (float)flail::FLAIL_CHARGE) * (m_pFlail->GetNumChain() - 1)) * 0.6f;

				if (lengthTarget < flail::FLAIL_RADIUS * flail::FLAIL_NUM_MIN * 0.5f)
				{
					lengthTarget = flail::FLAIL_RADIUS * flail::FLAIL_NUM_MIN * 0.5f;
				}
			}

			m_pFlail->SetLengthTarget(lengthTarget);

			// カウンターの設定
			m_nCounterFlail = flail::FLAIL_THROW;

			// 攻撃モーションを設定
			m_currentMotion = CPlayer::MOTION_ATTACK;

			// AIも状態を設定
			m_stateAI = STATEAI_NONE;

			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SWING);
		}

		if (m_nCounterFlail == flail::FLAIL_THROW)
		{
			// 移動量を更新
			rMove.x = 0.0f;
			rMove.z = 0.0f;

			// フレイルが止まったらカウンターを次の段階へ
			if (m_pFlail->GetLengthChain() >= m_pFlail->GetLengthTarget())
			{
				// モーションを攻撃から変更
				player->SetMotion(m_currentMotion);

				m_nCounterFlail = flail::FLAIL_DROP;

				// AIも状態を設定
				m_stateAI = STATEAI_NONE;
			}
		}
	}
	else if (m_nCounterFlail == flail::FLAIL_DEF)
	{
		// フレイルを強制的に所持
		m_pFlail->CatchFlail();

		// カウンターアップ開始
		if (m_bAttack)
		{
			m_nCounterFlail++;
		}
	}
	else
	{
		// 鉄球とプレイヤーの距離が一定未満の時プレイヤー位置に鉄球固定
		if (m_pFlail->GetLengthChain() <= flail::FLAIL_RADIUS * 3.0f)
		{
			m_nCounterFlail = flail::FLAIL_DEF;
			m_pFlail->SetMove(VEC3_ZERO);
			m_pFlail->CatchFlail();
		}
		else if (m_pFlail->GetLengthTarget() <= flail::FLAIL_RADIUS * 6.0f && m_nCounterFlail == flail::FLAIL_DROP)
		{
			m_nCounterFlail = flail::FLAIL_DEF;
			m_pFlail->SetMove(VEC3_ZERO);
			m_pFlail->CatchFlail();
		}
		else
		{
			// 引き戻す
			if (m_bAttack)
			{
				m_nCounterFlail -= 1;

				if (m_nCounterFlail < -60)
				{
					m_nCounterFlail = -60;
				}

				if (m_nCounterFlail == -2)
				{
					float rot1 = m_fRotstickR;
					float rot2 = rDestRot.y;
					float rot3 = rot2 - rot1;

					useful::NormalizeRot(rot3);

					if (rot3 > 1.0f)
					{
						// 溜めてる間鉄球を振り回す
						m_pFlail->SetChainRotMove(0.03f);
					}
					else if (rot3 < -1.0f)
					{
						// 溜めてる間鉄球を振り回す
						m_pFlail->SetChainRotMove(-0.03f);
					}
					else
					{
						// 溜めてる間鉄球を振り回す
						m_pFlail->SetChainRotMove(0.0f);
					}
				}

				// 移動量を更新
				rMove.x = 0.0f;
				rMove.z = 0.0f;
			}
			else
			{
				m_nCounterFlail = flail::FLAIL_DROP;
			}
		}
	}

	return m_currentMotion;
}

//============================================================
//	移動に関するAI
//============================================================
CPlayer::EMotion CPlayerAI::AImove
(
	CFlail* pFlail,
	D3DXVECTOR3& rPos,
	D3DXVECTOR3& rMove,
	D3DXVECTOR3& rDestRot,
	int pCounterFlail,
	const int nMotionOld
)
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPadID);
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		// カメラ

	if (m_bMove)
	{ // デッドゾーン以上の場合

		// 変数を宣言
		float fMove = 0.0f;	// プレイヤー移動量
		float fRotDiff = 0.0f;

		D3DXVECTOR3 movePos = VEC3_ZERO;

		// 移動量を更新
		movePos.x = rPos.x + sinf(m_fRotstickL + D3DX_PI) * 75.0f;
		movePos.y = -1.0f;
		movePos.z = rPos.z + cosf(m_fRotstickL + D3DX_PI) * 75.0f;

		/*CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[移動位置]：%f %f %f\n", rPos.x, rPos.y, rPos.z);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[移動予測位置]：%f %f %f\n", movePos.x, movePos.y, movePos.z);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[移動予測位置差分]：%f %f %f\n", movePos.x - rPos.x, movePos.y - rPos.y, movePos.z - rPos.z);*/

		if (CollisionGround(CPlayer::AXIS_X, movePos))
		{
			// 移動量を更新
			movePos.x = rPos.x + sinf(m_fRotstickL + D3DX_PI) * 75.0f;
			movePos.y = 1.0f;
			movePos.z = rPos.z + cosf(m_fRotstickL + D3DX_PI) * 75.0f;

			if (CollisionBlock(CPlayer::AXIS_X, movePos))
			{
				float fRotDiff = 0.0f;

				while (fRotDiff < 3.14f)
				{
					fRotDiff = (float)((rand() % 629) - 314);
					fMove = 1.2f;

					float colRot = m_fRotstickL + fRotDiff;
					useful::NormalizeRot(colRot);

					// 移動量を更新
					movePos.x = rPos.x + sinf(colRot + D3DX_PI) * 75.0f;
					movePos.y = -1.0f;
					movePos.z = rPos.z + cosf(colRot + D3DX_PI) * 75.0f;

					if (Collision(movePos))
					{
						m_fRotstickL = colRot;

						break;
					}
				}
			}
			else
			{
				if (CollisionObstacle(movePos))
				{
					float fRotDiff = 0.0f;

					while (fRotDiff < 3.14f)
					{
						fRotDiff = (float)((rand() % 629) - 314);
						fMove = 1.2f;

						float colRot = m_fRotstickL + fRotDiff;
						useful::NormalizeRot(colRot);

						// 移動量を更新
						movePos.x = rPos.x + sinf(colRot + D3DX_PI) * 75.0f;
						movePos.y = -1.0f;
						movePos.z = rPos.z + cosf(colRot + D3DX_PI) * 75.0f;

						if (Collision(movePos))
						{
							m_fRotstickL = colRot;

							break;
						}
					}
				}
				else
				{
					fMove = 1.2f;
				}
			}
		}
		else
		{
			float fRotDiff = 0.0f;

			while (fRotDiff < 3.14f)
			{
				fRotDiff = (float)((rand() % 629) - 314);
				fMove = 1.2f;

				float colRot = m_fRotstickL + fRotDiff;
				useful::NormalizeRot(colRot);

				// 移動量を更新
				movePos.x = rPos.x + sinf(colRot + D3DX_PI) * 75.0f;
				movePos.y = -1.0f;
				movePos.z = rPos.z + cosf(colRot + D3DX_PI) * 75.0f;

				if (Collision(movePos))
				{
					m_fRotstickL = colRot;

					break;
				}
			}
		}

		if (!m_bDash)
		{ // ダッシュ中ではない場合

		  // 移動量を更新
			rMove.x += sinf(m_fRotstickL + D3DX_PI) * fMove;
			rMove.z += cosf(m_fRotstickL + D3DX_PI) * fMove;

			// 移動モーションを設定
			m_currentMotion = CPlayer::MOTION_MOVE;
		}
		else
		{ // ダッシュ中の場合

		  // 移動量を更新
			rMove.x += sinf(m_pDashRot.y) * fMove;
			rMove.z += cosf(m_pDashRot.y) * fMove;

			// ダッシュモーションを設定
			m_currentMotion = CPlayer::MOTION_DASH;
		}

		if (m_pFlail->GetLengthChain() >= flail::FLAIL_RADIUS * (m_pFlail->GetNumChain() - 1) || m_nMotionOld == CPlayer::MOTION_PULL)
		{ // 引きずり距離の場合

		  // 移動量を更新
			rMove.x *= 0.7f;
			rMove.z *= 0.7f;

			D3DXVECTOR3 vecFlail;
			float rotMove, rotFlail, rotDiff;

			vecFlail = m_pFlail->GetVec3Position() - player->GetVec3Position();

			rotMove = atan2f(rMove.x, rMove.z);
			rotFlail = atan2f(vecFlail.x, vecFlail.z);

			rotDiff = rotMove - rotFlail;
			useful::NormalizeRot(rotDiff);

			if (rotDiff > D3DX_PI * 0.5f || rotDiff < D3DX_PI * -0.5f)
			{
				// 引きずりモーションを設定
				m_currentMotion = CPlayer::MOTION_PULL;
			}
		}

		// 目標向きを設定
		rDestRot.y = atan2f(-rMove.x, -rMove.z);
		m_pFlail->SetChainRotTarget(rDestRot.y + D3DX_PI);
	}

	return m_currentMotion;
}

//============================================================
//	ダッシュに関するAI
//============================================================
void CPlayerAI::AIDash
(
	D3DXVECTOR3& rPos,
	D3DXVECTOR3& rMove,
	D3DXVECTOR3& rDashRot,
	D3DXVECTOR3& rDestRot,
	float& rPlusMove,
	int& rCounterFlail,
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
		bool bJumpSelect = false;

		// 移動量を更新
		dashPos.x = rPos.x + sinf(m_fRotstickL + D3DX_PI) * 700.0f;
		dashPos.y = -1.0f;
		dashPos.z = rPos.z + cosf(m_fRotstickL + D3DX_PI) * 700.0f;

		if (Collision(dashPos))
		{
			int nProb = rand() % 10000;
			
			if (nProb > 9920)
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
			int nProb = rand() % 10000;

			if (nProb > 9999)
			{
				bJumpSelect = true;
			}
			else
			{
				bJumpSelect = false;
			}
		}

		if (m_stateAI != STATEAI_EMOTE)
		{
			bJumpSelect = false;
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
				rDashRot.y = m_fRotstickL + D3DX_PI;

				// ダッシュしている状態にする
				rDash = true;

				if (rCounterFlail > flail::FLAIL_DEF && rCounterFlail <= flail::FLAIL_CHARGE)
				{
					rCounterFlail = flail::FLAIL_DEF;
					m_stateAI = STATEAI_MOVE;
				}

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_JUMP);	// ジャンプ音
			}
		}
	}

	m_pDashRot = rDashRot;
	m_bDash = rDash;

	if (m_bDash)
	{ // ダッシュしている場合

	  // 移動量を更新
		rMove.x += sinf(rDashRot.y) * rPlusMove;
		rMove.z += cosf(rDashRot.y) * rPlusMove;

		// プラス移動量を減算
		rPlusMove += (0.0f - rPlusMove) * DASH_REV;

		if (fabsf(rPlusMove) < DASH_MINMOVE)
		{ // 移動量が一定値より少なくなった場合

		  // 再ダッシュできるようにする
			rPlusMove = 0.0f;
			m_bDash = false;
			rDash = false;
			m_stateAI = STATEAI_MOVE;
		}
	}
}

//============================================================
//	エモートに関するAI
//============================================================
CPlayer::EMotion CPlayerAI::AIemote(D3DXVECTOR3& rDestRot)
{
	if (m_bEmote)
	{
		int nProb = rand() % 4;

		if (nProb == 0)
		{
			m_currentMotion = CPlayer::MOTION_EMOTE_PROUD;
		}
		else if (nProb == 1)
		{
			m_currentMotion = CPlayer::MOTION_EMOTE_PROUD;
		}
		else if (nProb == 2)
		{
			m_currentMotion = CPlayer::MOTION_EMOTE_PROUD;
		}
		else if (nProb == 3)
		{
			m_currentMotion = CPlayer::MOTION_EMOTE_PROUD;
		}

		rDestRot.y = 0.0f;

		m_bEmote = false;
	}

	return m_currentMotion;
}

//============================================================
//	生成処理
//============================================================
CPlayerAI *CPlayerAI::Create(const int nPadID)
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
		if (FAILED(pAI->Init(nPadID)))
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
	if(CollisionGround(CPlayer::AXIS_X, rPos))
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

											// 地盤の最大の大きさを設定
				sizeMaxGround = pObjCheck->GetVec3Sizing();
				//sizeMaxGround.y = 0.0f;		// 縦の大きさを初期化

				if (collision::SquareTrigger(posGround, rPos, rotGround, sizeMaxGround, -sizeMinGround))
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
bool CPlayerAI::CollisionBlock(const CPlayer::EAxis axis, D3DXVECTOR3& rPos)
{
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

				// ブロックの大きさを設定
				sizeBlock = pObjCheck->GetVec3Sizing();

				if (collision::SquareTrigger(posBlock, rPos, rotBlock, sizeBlock, -sizeBlock))
				{
					bHit = true;
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	// 各軸の判定情報を返す
	return bHit;
}
bool CPlayerAI::CollisionObstacle(D3DXVECTOR3& rPos)
{
	// 変数を宣言
	bool bHit = false;	// 着地の判定情報
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
				CObstacle::SStatusInfo status;		// 障害物ステータス
				D3DXVECTOR3 posObs = VEC3_ZERO;		// 障害物位置
				D3DXVECTOR3 rotObs = VEC3_ZERO;		// 障害物向き
				D3DXVECTOR3 sizeObsMin = VEC3_ZERO;	// 障害物大きさ
				D3DXVECTOR3 sizeObsMax = VEC3_ZERO;	// 障害物大きさ

													// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
				{ // オブジェクトラベルが障害物ではない場合

				  // 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 障害物のステータスを設定
				status = CObstacle::GetStatusInfo(pObjCheck->GetType());

				// 障害物の向きを設定
				rotObs = pObjCheck->GetVec3Rotation();

				// 障害物の位置を設定
				posObs = pObjCheck->GetVec3Position();
				posObs.x += sinf(rotObs.y + status.fAngleCenter) * status.fLengthCenter;
				posObs.y = 0.0f;
				posObs.z += cosf(rotObs.y + status.fAngleCenter) * status.fLengthCenter;

				// 障害物の大きさを設定
				sizeObsMax = status.sizeColl;
				sizeObsMin = sizeObsMax * -1.0f;

				// 障害物との判定を実行
				if (collision::SquareTrigger(posObs, rPos, rotObs, sizeObsMax, sizeObsMin))
				{
					bHit = true;
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	// 判定情報を返す
	return bHit;
}

void CPlayerAI::SetRotstickL(const float RotstickL)
{
	m_fRotstickL = RotstickL;
}