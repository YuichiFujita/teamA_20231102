//============================================================
//
//	フレイル処理 [flail.cpp]
//	Author：中村陸
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "flail.h"
#include "flailEntry.h"
#include "frailResult.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "model.h"
#include "useful.h"
#include "retentionManager.h"
#include "stage.h"
#include "liquid.h"
#include "scrollMeshField.h"
#include "collision.h"
#include "obstacle.h"
#include "block.h"
#include "sound.h"
#include "orbitalParticle.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY	= 3;		// フレイルの優先順位

	const float	RADIUS		= 50.0f;	// 半径
	const int	HIT_DAMAGE	= 10;		// ダメージ量
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CFlail::mc_apModelFileFlail[] =	// モデル定数(フレイル)
{
	"data\\MODEL\\FLAIL\\ironBall.x",	// 鉄球
	"data\\MODEL\\FLAIL\\head.x",		// 頭
	"data\\MODEL\\FLAIL\\escapekun.x",	// エスケープ君
	"data\\MODEL\\FLAIL\\apple.x",		// リンゴ
	"data\\MODEL\\FLAIL\\green_apple.x",// 青リンゴ
	"data\\MODEL\\FLAIL\\Duck.x",		// アヒル
	"data\\MODEL\\FLAIL\\bomb.x",		// 爆弾
	"data\\MODEL\\FLAIL\\tomato.x",		// 爆弾
};

const char *CFlail::mc_apModelFileChain[] =	// モデル定数(鎖)
{
	"data\\MODEL\\PLAYER\\14_chain.x",	// 鎖
};

//************************************************************
//	子クラス [CFlail] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFlail::CFlail() : CObjectModel(CObject::LABEL_NONE, PRIORITY)
{
	memset(&m_chain[0], 0, sizeof(m_chain));	// モデルの情報
	m_oldPos = VEC3_ZERO;
	m_move = VEC3_ZERO;
	m_typeFlail = FLAIL_NORMAL;
	m_nNumChain = 0;
	m_nfulChainF = 0;
	m_nfulChainP = 0;
	m_nHitCount = 0;
	m_fChainRot = 0.0f;
	m_fLengthChain = 0.0f;
	m_fLengthTarget = 0.0f;
	m_fChainRotTarget = 0.0f;
	m_fChainRotMove = 0.0f;
	m_bHit = false;
}

//============================================================
//	デストラクタ
//============================================================
CFlail::~CFlail()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CFlail::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	m_nNumChain = flail::FLAIL_NUM_MAX;

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM_MAX; nCntChain++)
	{
		// モデルの生成
		m_chain[nCntChain].multiModel = CMultiModel::Create(VEC3_ZERO, VEC3_ZERO);

		// モデルを割当
		m_chain[nCntChain].multiModel->BindModel(mc_apModelFileChain[CFlail::CHAIN_NORMAL]);
	}

	m_pOrbit = CObjectOrbit::Create(GetPtrMtxWorld(), CObjectOrbit::SOffset(D3DXVECTOR3(0.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, -50.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)), 60,60);
	m_pOrbit->BindTexture("data\\TEXTURE\\orbitLine.png");

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CFlail::Uninit(void)
{
	if (m_pOrbit != NULL)
	{
		m_pOrbit->Uninit();
		m_pOrbit = NULL;
	}
	
	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM_MAX; nCntChain++)
	{
		// モデルの終了
		m_chain[nCntChain].multiModel->Uninit();
	}

	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CFlail::Update(void)
{
	m_oldPos = GetVec3Position();
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);
	m_fLengthChain = 0.0f;
	
	m_fChainRotOld = m_fChainRot;

	// 角度修正
	useful::NormalizeRot(m_fChainRot);
	useful::NormalizeRot(m_fChainRotTarget);

	// 引っ張る時のみ角度調整
	m_fChainRot += m_fChainRotMove;

	if (D3DXVec3Length(&m_move) > 0.0f && D3DXVec3Length(&m_move) < 5.0f)
	{
		m_move.x = 0.0f;
		m_move.z = 0.0f;
	}

	else
	{
		m_nDamage = HIT_DAMAGE;
	}
	// 角度修正
	useful::NormalizeRot(m_fChainRot);
	useful::NormalizeRot(m_fChainRotTarget);

	if (m_nPlayerID == 0)
	{
		/*CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "鎖目標角度 %f\n", m_fChainRotTarget);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "鎖角度 %f\n", m_fChainRot);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "鎖長さ %f\n", m_fLengthChain);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[カウンター]：%d\n", player->GetCounterFlail());*/
		//CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[角速度]：%f\n", m_fChainRotMove);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[フレイル位置]：%f %f %f\n", GetVec3Position().x, GetVec3Position().y, GetVec3Position().z);
		//CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[フレイル角度]：%f\n", m_fChainRot);
	}

	// オブジェクトモデルの更新
	CObjectModel::Update();

	// 鎖の更新
	UpdateChain();

	// フレイル同士の当たり判定カウント
	if (m_bHit)
	{
		m_nHitCount++;

		if (m_nHitCount > 60)
		{
			m_bHit = false;
		}
	}
	else
	{
		m_nHitCount = 0;
	}

	// 回転速度の慣性軽減
	if (player->GetCounterFlail() == flail::FLAIL_DROP)
	{
		m_fChainRotMove += (0.0f - m_fChainRotMove) * 0.1f;
	}
}

//============================================================
//	フレイル位置の更新処理
//============================================================
void CFlail::UpdateFlailPos(void)
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

	int partsNum = m_nNumChain - 1;
	D3DXMATRIX partsMtx = m_chain[partsNum].multiModel->GetMtxWorld();
	D3DXVECTOR3 partsPos = D3DXVECTOR3(partsMtx._41, partsMtx._42, partsMtx._43);
	SetVec3PosOrg(partsPos);

	// 角度と長さから鉄球の位置決定
	D3DXVECTOR3 pos = GetVec3Position();
	D3DXVECTOR3 rot = GetVec3Rotation();

	if (D3DXVec3Length(&m_move) == 0.0f)
	{
		// フレイルの状態によって高さを変更
		if (player->GetCounterFlail() == flail::FLAIL_DROP)
		{
			if (pos.y > -64.0f)
			{
				pos.y -= 8.0f;
			}
			else
			{
				if (m_fLengthTarget <= flail::FLAIL_RADIUS * (m_nNumChain - 1))
				{
					pos.y = CManager::GetInstance()->GetScene()->GetStage()->GetLiquid()->GetScrollMeshField(0)->GetPositionHeight(pos);
				}
			}
		}
		else if (player->GetCounterFlail() < flail::FLAIL_DROP)
		{
			D3DXMATRIX chainMtx = m_chain[0].multiModel->GetMtxWorld();
			D3DXVECTOR3 chainPos = D3DXVECTOR3(chainMtx._41, chainMtx._42, chainMtx._43);

			pos.y = chainPos.y;
		}
		else
		{
			pos.y = m_posOrg.y;
		}
	}

	if (player->GetCounterFlail() < flail::FLAIL_DEF)
	{
		// 移動量減衰
		m_move.x += (0.0f - m_move.x) * 0.15f;
		m_move.z += (0.0f - m_move.z) * 0.15f;
	}
	else
	{
		// 移動量減衰
		m_move.x += (0.0f - m_move.x) * 0.15f;
		m_move.z += (0.0f - m_move.z) * 0.15f;
	}

	// 落下状態じゃなければフレイルの先端にフレイルを移動
	if (player->GetCounterFlail() != flail::FLAIL_DROP)
	{
		pos.x = m_posOrg.x + (sinf(m_fChainRot) * 1.0f);
		pos.z = m_posOrg.z + (cosf(m_fChainRot) * 1.0f);
	}
	else
	{
		if (m_fChainRotMove < -0.01f || m_fChainRotMove > 0.01f)
		{
			pos.x = m_posOrg.x + (sinf(m_fChainRot) * 1.0f);
			pos.z = m_posOrg.z + (cosf(m_fChainRot) * 1.0f);
		}
	}

	if (player->GetCounterFlail() > flail::FLAIL_DEF)
	{
		D3DXVECTOR3 stickPos;

		stickPos.x = player->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld()._41;
		stickPos.y = player->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld()._42;
		stickPos.z = player->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld()._43;

		rot.y = atan2f(stickPos.x - pos.x, stickPos.z - pos.z);
	}
	else if (player->GetCounterFlail() <= flail::FLAIL_DROP)
	{
		D3DXVECTOR3 stickPos;

		stickPos.x = player->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld()._41;
		stickPos.y = player->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld()._42;
		stickPos.z = player->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld()._43;

		rot.y = atan2f(pos.x - stickPos.x, pos.z - stickPos.z);
	}
	else
	{
		D3DXVECTOR3 stickPos;

		stickPos.x = player->GetMtxWorld()._41;
		stickPos.y = player->GetMtxWorld()._42;
		stickPos.z = player->GetMtxWorld()._43;

		rot.y = atan2f(stickPos.x - pos.x, stickPos.z - pos.z);
	}
	// 当たり判定
	Collision(pos);
	
	SetVec3Position(pos);
	SetVec3Rotation(rot);
}

//============================================================
//	落下フレイル位置の更新処理
//============================================================
void CFlail::UpdateDropFlailPos(float& rRot)
{
	D3DXVECTOR3 vecPtoF = VEC3_ZERO;
	D3DXVECTOR3 vecPtoFTarget = VEC3_ZERO;
	D3DXVECTOR3 flailPos = VEC3_ZERO;
	float lengthPtoF = 0.0f;
	float lengthPtoFTarget = 0.0f;

	// フレイルとプレイヤー間のベクトルを計算
	vecPtoFTarget.x = m_chain[m_nfulChainF].multiModel->GetPtrMtxWorld()->_41 - GetVec3Position().x;
	vecPtoFTarget.y = m_chain[m_nfulChainF].multiModel->GetPtrMtxWorld()->_42 - GetVec3Position().y;
	vecPtoFTarget.z = m_chain[m_nfulChainF].multiModel->GetPtrMtxWorld()->_43 - GetVec3Position().z;

	vecPtoF.x = m_chain[m_nfulChainF].multiModel->GetPtrMtxWorld()->_41 - GetVec3Position().x;
	vecPtoF.z = m_chain[m_nfulChainF].multiModel->GetPtrMtxWorld()->_43 - GetVec3Position().z;

	// フレイルとプレイヤー間のベクトルから角度計算
	rRot = atan2f(vecPtoFTarget.x, vecPtoFTarget.z) + D3DX_PI * 0.5f;

	lengthPtoF = D3DXVec3Length(&vecPtoF);
	lengthPtoFTarget = D3DXVec3Length(&vecPtoFTarget);

	// フレイルとプレイヤー間の距離が最大値を超えてたら補正
	if (lengthPtoF > flail::FLAIL_RADIUS * (m_nNumChain - 1))
	{
		lengthPtoF = flail::FLAIL_RADIUS * (m_nNumChain - 1);
	}
	else if (lengthPtoF < 0.0f)
	{
		lengthPtoF = 0.0f;
	}

	if (lengthPtoFTarget < 0.0f)
	{
		lengthPtoFTarget = 0.0f;
	}

	m_fLengthTarget = lengthPtoFTarget;

	// フレイルの位置を設定
	if (lengthPtoFTarget >= flail::FLAIL_RADIUS * (m_nNumChain - m_nfulChainF - 1))
	{
		flailPos = GetVec3Position();

		flailPos.x = m_chain[m_nfulChainF].multiModel->GetPtrMtxWorld()->_41 + (sinf(rRot + D3DX_PI * 0.5f) * lengthPtoF);
		flailPos.z = m_chain[m_nfulChainF].multiModel->GetPtrMtxWorld()->_43 + (cosf(rRot + D3DX_PI * 0.5f) * lengthPtoF);

		SetVec3Position(flailPos);
	}
}

//============================================================
//	鎖の更新処理
//============================================================
void CFlail::UpdateChain(void)
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM_MAX; nCntChain++)
	{
		D3DXVECTOR3 pos, rot;
		m_chain[nCntChain].rotOld = m_chain[nCntChain].multiModel->GetVec3Rotation();
		pos = m_chain[nCntChain].multiModel->GetVec3Position();
		m_chain[nCntChain].posOld = m_chain[nCntChain].multiModel->GetVec3Position();
		rot = m_chain[nCntChain].multiModel->GetVec3Rotation();

		// 現在のフレイルの端を超えている鎖の設定
		if (m_nNumChain < nCntChain)
		{
			rot.x = 0.0f;
			rot.y = 0.0f;
			rot.z = 0.0f;

			pos.x = 0.0f;
			pos.y = 0.0f;
			pos.z = 0.0f;

			m_chain[nCntChain].multiModel->SetVec3Position(pos);
			m_chain[nCntChain].multiModel->SetVec3Rotation(rot);

			// モデルの更新
			m_chain[nCntChain].multiModel->Update();

			continue;
		}

		// 鎖によって処理を変える
		if (nCntChain == 0)
		{
			// 回転させる
			rot.x = 0.0f;
			rot.y = m_fChainRot;
			rot.z = 0.0f;

			pos.x = -1.0f;
			pos.y = 0.0f;
			pos.z = 0.0f;
		}
		else
		{
			int IDParent = nCntChain - 1;

			// 前の鎖の角度に追従させる
			if (nCntChain == 1)
			{
				rot.y = m_chain[IDParent].rotOld.y - m_chain[IDParent].multiModel->GetVec3Rotation().y;
			}
			else
			{
				rot.y = m_chain[IDParent].rotOld.y * 0.7f;
			}

			// チャージ中
			if (player->GetCounterFlail() <= flail::FLAIL_CHARGE && player->GetCounterFlail() > flail::FLAIL_DEF)
			{
				// ひとつ前の鎖が伸びきっていたら
				if ((m_chain[IDParent].multiModel->GetVec3Position().x >= flail::FLAIL_RADIUS && m_fLengthChain < m_fLengthTarget) || nCntChain == 1)
				{
					//フレイルを伸ばす
					pos.x += 10.0f;

					// 距離補正
					if (pos.x > flail::FLAIL_RADIUS)
					{
						pos.x = flail::FLAIL_RADIUS;
					}
				}
			}

			// 投擲中
			if (player->GetCounterFlail() == flail::FLAIL_THROW)
			{
				// 回転速度によって挙動変更
				if (m_fChainRotMove == 0.0f)
				{
					// 回転していないなら鎖を同時に伸ばす
					if ((m_fLengthChain < m_fLengthTarget) || nCntChain == 1)
					{
						pos.x += 0.4f + ((m_fLengthTarget - 700.0f) * 0.0006f);

						if (pos.x > flail::FLAIL_RADIUS)
						{
							pos.x = flail::FLAIL_RADIUS;
						}
					}
				}
				else
				{
					// 回転してるなら順番に伸ばす
					if ((m_chain[IDParent].multiModel->GetVec3Position().x >= flail::FLAIL_RADIUS && m_fLengthChain < m_fLengthTarget) || nCntChain == 1)
					{
						pos.x += 10.0f;

						if (pos.x > flail::FLAIL_RADIUS)
						{
							pos.x = flail::FLAIL_RADIUS;
						}
					}
				}
			}

			// 落下状態
			if (player->GetCounterFlail() == flail::FLAIL_DROP)
			{
				// 一時的にひっこめる
				pos.x = 0.0f;

				// 目標距離に届いていなければそこまで伸ばす
				if ((m_chain[IDParent].multiModel->GetVec3Position().x >= flail::FLAIL_RADIUS && m_fLengthChain < m_fLengthTarget) || nCntChain == 1)
				{
					pos.x = 20.0f;

					if (pos.x > flail::FLAIL_RADIUS)
					{
						pos.x = flail::FLAIL_RADIUS;
					}
				}
			}

			// 引き戻し中
			if (player->GetCounterFlail() < flail::FLAIL_DROP)
			{
				// 先端から引き戻す
				if (nCntChain < flail::FLAIL_NUM_MAX - 2)
				{
					if (m_chain[nCntChain + 1].multiModel->GetVec3Position().x <= 0.0f || m_chain[nCntChain + 2].multiModel->GetVec3Position().x <= 0.0f)
					{
						float speed = 10.0f;

						if (speed > 20.0f)
						{
							speed = 20.0f;
						}

						pos.x -= speed;

						if (pos.x < 0.0f)
						{
							pos.x = 0.0f;
						}
					}
				}
				else
				{
					if (m_chain[0].multiModel->GetVec3Position().x <= 0.0f)
					{
						float speed = 10.0f;

						if (speed > 20.0f)
						{
							speed = 20.0f;
						}

						pos.x -= speed;

						if (pos.x < 0.0f)
						{
							pos.x = 0.0f;
						}
					}
				}
			}

			if (m_nPlayerID == 0)
			{
				//CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "鎖位置[%d] %f %f %f\n", nCntChain, rot.x, rot.y, rot.z);
			}
		}

		m_chain[nCntChain].multiModel->SetVec3Position(pos);
		m_chain[nCntChain].multiModel->SetVec3Rotation(rot);

		if (nCntChain != 0)
		{
			m_fLengthChain += pos.x;
		}

		// モデルの更新
		m_chain[nCntChain].multiModel->Update();
	}

	D3DXVECTOR3 posCol = VEC3_ZERO;

	CollisionChain(posCol);
}

//============================================================
//	描画処理
//============================================================
void CFlail::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

	D3DXMATRIX mtx, mtxChain;

	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans, *mtxOrg;	// 計算用マトリックス
	D3DXMATRIX   mtxParent;	// 親のマトリックス
	D3DXVECTOR3 pos, rot, scale;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	mtx._41 = player->GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_41;
	mtx._42 = player->GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_42;
	mtx._43 = player->GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_43;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

#ifdef ON_STENCIL_PLAYER

	// ステンシルテストを有効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	
	// 比較参照値を設定する
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);
	
	// ステンシルマスクを指定する 
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	// ステンシル比較関数を指定する
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	// ステンシル結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS,	D3DSTENCILOP_REPLACE);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL,	D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL,	D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

#endif	// ON_STENCIL_PLAYER

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM_MAX; nCntChain++)
	{
		int IDParent = nCntChain - 1;
		D3DXVECTOR3 rotOld = m_chain[nCntChain].multiModel->GetVec3Rotation();
		D3DXVECTOR3 rotNow = m_chain[nCntChain].multiModel->GetVec3Rotation();

		if (nCntChain != 0)
		{
			// 鎖を互い違いにする
			if (nCntChain % 2 == 0)
			{
				rotNow.x = -D3DX_PI * 0.5f;
				rotNow.y = 0.0f;
				rotNow.z = 0.0f;
			}
			else if (nCntChain % 2 == 1)
			{
				rotNow.x = D3DX_PI * 0.5f;
				rotNow.y = rotOld.y;
				rotNow.z = 0.0f;
			}

			m_chain[nCntChain].multiModel->SetVec3Rotation(rotNow);
		}
		else
		{
			if (m_fLengthChain != 0)
			{
				// フレイルの高さに合わせて鎖の先端を下げる
				float cosRot1, cosRot2, acosRot;

				cosRot1 = m_chain[nCntChain].multiModel->GetMtxWorld()._42 - GetVec3Position().y;
				rotNow.z = 0.0f;

				if (cosRot1 < m_fLengthChain)
				{
					cosRot2 = cosRot1 / m_fLengthChain;
					acosRot = acosf(cosRot2);

					rotNow.z = -(D3DX_PI * 0.5f) + acosRot;
				}
			}

			m_chain[nCntChain].multiModel->SetVec3Rotation(rotNow);
		}

		// 鎖が縮みきっているなら描画しない
		if (m_chain[nCntChain].multiModel->GetVec3Position().x == 0.0f)
		{
			pos = m_chain[nCntChain].multiModel->GetVec3Position();
			rot = m_chain[nCntChain].multiModel->GetVec3Rotation();
			scale = m_chain[nCntChain].multiModel->GetVec3Scaling();
			mtxOrg = m_chain[nCntChain].multiModel->GetPtrMtxWorld();

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(mtxOrg);

			// 拡大率を反映
			D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
			D3DXMatrixMultiply(mtxOrg, mtxOrg, &mtxScale);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(mtxOrg, mtxOrg, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(mtxOrg, mtxOrg, &mtxTrans);

			// 親マトリックスを設定
			if (nCntChain == 0)
			{ // 親が存在しない場合

				// 現在のマトリックスを取得
				pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	// 設定された最新のマトリックス (実体のマトリックス)
			}
			else
			{ // 親が存在する場合

				// 親のマトリックスを設定
				mtxParent = m_chain[IDParent].multiModel->GetMtxWorld();
			}

			// ワールドマトリックスと親マトリックスを掛け合わせる
			D3DXMatrixMultiply(mtxOrg, mtxOrg, &mtxParent);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, mtxOrg);
		}
		else
		{
			// モデルの描画
			m_chain[nCntChain].multiModel->Draw();
		}

		m_chain[nCntChain].multiModel->SetVec3Rotation(rotOld);
	}

	// 落下状態フレイルの位置更新
	if (player->GetCounterFlail() == flail::FLAIL_DROP && m_fChainRotMove > -0.01f && m_fChainRotMove < 0.01f)
	{
		UpdateDropFlailPos(m_fChainRot);
	}

	// フレイルの位置更新
	UpdateFlailPos();

	// オブジェクトモデルの描画
	CObjectModel::Draw();

#ifdef ON_STENCIL_PLAYER

	// ステンシルテストを無効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

#endif	// ON_STENCIL_PLAYER
}

//============================================================
//	種類の設定処理
//============================================================
void CFlail::SetType(const int nType)
{
	if (nType > NONE_IDX && nType < FLAIL_MAX)
	{
		// 種類を設定
		m_typeFlail = (EModelFlail)nType;

		// モデルを割当
		BindModel(mc_apModelFileFlail[m_typeFlail]);
	}
	else { assert(false); }
}

//============================================================
//	種類取得処理
//============================================================
int CFlail::GetType(void) const
{
	return (int)m_typeFlail;
}

//============================================================
//	生成処理
//============================================================
CFlail *CFlail::Create
( // 引数
	CScene::EMode mode,			// モード
	const CPlayer& rPlayer,		// プレイヤー
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR3& rScale	// 大きさ
)
{
	// ポインタを宣言
	CFlail *pModelUI = NULL;	// モデルUI生成用

	if (pModelUI == NULL)
	{ // 使用されていない場合

		// モードオーバー
		assert(mode > NONE_IDX && mode < CScene::MODE_MAX);

		switch (mode)
		{ // モードごとの処理
		case CScene::MODE_ENTRY:

			// メモリ確保
			pModelUI = new CFlailEntry;	// フレイルエントリー

			break;

		case CScene::MODE_GAME:

			// メモリ確保
			pModelUI = new CFlail;	// フレイル

			break;

		case CScene::MODE_RESULT:

			// メモリ確保
			pModelUI = new CFlailResult;	// フレイル

			break;
		}
	}
	else { assert(false); return NULL; }	// 使用中

	if (pModelUI != NULL)
	{ // 使用されている場合
		
		// モデルUIの初期化
		if (FAILED(pModelUI->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pModelUI;
			pModelUI = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pModelUI->SetVec3Position(rPos);

		// 向きを設定
		pModelUI->SetVec3Rotation(rRot);

		// 拡大率を設定
		pModelUI->SetVec3Scaling(rScale);

		// 鎖の親を設定
		pModelUI->BindParent(rPlayer);

		// フレイルの種類を設定
		pModelUI->SetType(CManager::GetInstance()->GetRetentionManager()->GetFlail(rPlayer.GetPadID()));

		// 確保したアドレスを返す
		return pModelUI;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	当たり判定処理(地面、ブロック、障害物)
//============================================================
void CFlail::Collision(D3DXVECTOR3& rPos)
{
	CPlayer *playerthis = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);
	
	if (playerthis->GetCounterFlail() < flail::FLAIL_DEF || playerthis->GetCounterFlail() == flail::FLAIL_THROW)
	{
		for (int nCntPlayer = 0; nCntPlayer < 4; nCntPlayer++)
		{
			CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);

			if (player != NULL && nCntPlayer != m_nPlayerID && player->GetState() != CPlayer::STATE_DEATH)
			{
				D3DXVECTOR3 vec;
				float length, colLength;

				// プレイヤーとフレイルのベクトルを求める
				vec = player->GetVec3Position() - GetVec3Position();
				vec.y = 0.0f;	// Yは無視

				// 距離を求める
				length = D3DXVec3Length(&vec);

				//フレイルの当たり判定用半径
				if (playerthis->GetCounterFlail() == flail::FLAIL_DROP)
				{
					colLength = 40.0f + RADIUS + player->GetRadius();
				}
				else
				{
					colLength = 40.0f + (RADIUS + player->GetRadius()) * 0.0015f * m_fLengthChain;
				}

				// 吹っ飛びベクトルを正規化
				D3DXVec3Normalize(&vec, &vec);

				// 距離で判定
				if (length < colLength)
				{
					float length = D3DXVec3Length(&(rPos - m_oldPos)) + 1.0f;
					int nAddDamage;
					float bonus = 1.0f;
					if (length > 80.0f)
					{
						nAddDamage = 30;
					}
					else if (length > 10.0f)
					{
						nAddDamage = (int)(length * 0.1f) * 3;
					}
					else
					{
						nAddDamage = 0;
					}

					if (playerthis->GetTemporaryItem().type == CPlayer::ITEM_BOOST_KNOCKBACK)
					{
						vec *= 2.0f;
						// ポインタを宣言
						CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// データ保存情報

						if (pRetention->GetKillState() == CRetentionManager::KILL_LIFE)
						{
							vec *= 2.0f;
						}
						else
						{
							vec *= 1.5f;
						}
						
					}

					if (playerthis->GetTemporaryItem().type == CPlayer::ITEM_BOOST_ATTACK)
					{
						bonus = 2.0f;
					}

					// ダメージヒット処理
					player->HitKnockBack((m_nDamage + nAddDamage) * bonus, vec, playerthis);
				}

				D3DXVECTOR3 posFlail;
				float flailLength;
				posFlail = GetVec3Position() - player->GetFlail()->GetVec3Position();
				posFlail.y = 0;

				flailLength = D3DXVec3Length(&posFlail);

				//フレイル同士の当たり判定
				if (flailLength < RADIUS * 3.0f && (player->GetCounterFlail() < flail::FLAIL_DROP || player->GetCounterFlail() == flail::FLAIL_THROW))
				{
					if (m_bHit == false)
					{
						CorbitalParticle::Create(GetVec3Position(), D3DXVECTOR3(2.5f, 0.0f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f), VEC3_ZERO, VEC3_ZERO, VEC3_ZERO, 6, 600, 20, 20, 300, 1.0f, 0.99f);
					}

					// 回転を移し替える
					float rotMove1, rotMove2;
					rotMove1 = GetChainRotMove();
					rotMove2 = player->GetFlail()->GetChainRotMove();

					SetChainRotMove(rotMove2 * 0.8f);
					player->GetFlail()->SetChainRotMove(rotMove1 * 0.8f);

					// 投擲状態なら落下状態にする
					if (player->GetCounterFlail() == flail::FLAIL_THROW)
					{
						player->SetCounterFlail(flail::FLAIL_DROP);
						player->SetMotion(CPlayer::MOTION_IDOL);
					}

					m_bHit = true;
				}
			}
		}
	}

	if (playerthis->GetCounterFlail() < flail::FLAIL_DEF || playerthis->GetCounterFlail() == flail::FLAIL_THROW)
	{
		// 障害物との当たり判定
		if (CollisionObstacle(rPos) ||
			CollisionBlock(CPlayer::AXIS_X, rPos) ||
			CollisionBlock(CPlayer::AXIS_Z, rPos))
		{
			int nParticle = 30;
			
			CorbitalParticle::Create(GetVec3Position(), D3DXVECTOR3(2.5f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), VEC3_ZERO, VEC3_ZERO, VEC3_ZERO, 6, 300, 20, 20, nParticle, 1.0f, 0.99f);

			for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM_MAX; nCntChain++)
			{
				int IDParent = nCntChain - 1;
				D3DXVECTOR3 rot = m_chain[nCntChain].multiModel->GetVec3Rotation();

				// 衝突したら角度をまっすぐに調整
				if (nCntChain == 0)
				{
					
				}
				else if (nCntChain == 1)
				{
					rot.y = m_chain[IDParent].rotOld.y - m_chain[IDParent].multiModel->GetVec3Rotation().y;
				}
				else
				{
					rot.y = m_chain[IDParent].rotOld.y * 0.0f;
				}

				m_chain[nCntChain].multiModel->SetVec3Rotation(rot);
			}

			// ブロックに引っ掛かっているなら少しずつ高さをあげる
			if (m_fLengthTarget > flail::FLAIL_RADIUS * (m_nNumChain - 1) && m_fLengthChain == flail::FLAIL_RADIUS * (m_nNumChain - 1))
			{
				rPos.y += 10.0f;
			}
		}

		// 地面のYでの当たり判定
		if (CollisionGround(CPlayer::AXIS_Y, rPos))
		{
			
		}

		// 地面の側面との当たり判定
		if (CollisionGround(CPlayer::AXIS_X, rPos) ||
			CollisionGround(CPlayer::AXIS_Z, rPos))
		{
			// ブロックに引っ掛かっているなら少しずつ高さをあげる
			if (m_fLengthTarget > flail::FLAIL_RADIUS * (m_nNumChain - 1) && m_fLengthChain == flail::FLAIL_RADIUS * (m_nNumChain - 1))
			{
				rPos.y += 10.0f;
			}

			// 回転を止める
			m_fChainRotMove *= 0.0f;

			for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM_MAX; nCntChain++)
			{
				int IDParent = nCntChain - 1;
				D3DXVECTOR3 rot = m_chain[nCntChain].multiModel->GetVec3Rotation();

				// 衝突したら角度をまっすぐに調整
				if (nCntChain == 0)
				{

				}
				else if (nCntChain == 1)
				{
					rot.y = m_chain[IDParent].rotOld.y - m_chain[IDParent].multiModel->GetVec3Rotation().y;
				}
				else
				{
					rot.y = m_chain[IDParent].rotOld.y * 0.0f;
				}
				m_chain[nCntChain].multiModel->SetVec3Rotation(rot);
			}
		}
	}
}
bool CFlail::CollisionChain(D3DXVECTOR3& rPos)
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);
	bool bCol = false;

	while (bCol)
	{
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
					D3DXVECTOR3 pos = VEC3_ZERO;		// 地盤位置
					D3DXVECTOR3 rot = VEC3_ZERO;		// 地盤向き
					D3DXVECTOR3 sizeMin = VEC3_ZERO;	// 地盤最小大きさ
					D3DXVECTOR3 sizeMax = VEC3_ZERO;	// 地盤最大大きさ

					// ポインタを宣言
					CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

					if (pObjCheck->GetLabel() == CObject::LABEL_BLOCK)
					{
						// 変数を宣言
						pos = pObjCheck->GetVec3Position();

						// 地盤の向きを設定
						rot = pObjCheck->GetVec3Rotation();

						// 地盤の最小の大きさを設定
						sizeMin = pObjCheck->GetVec3Sizing();
						sizeMin.y *= 2.0f;	// 縦の大きさを倍にする

											// 地盤の最大の大きさを設定
						sizeMax = pObjCheck->GetVec3Sizing();
						sizeMax.y = 0.0f;		// 縦の大きさを初期化
					}
					else if (pObjCheck->GetLabel() == CObject::LABEL_OBSTACLE)
					{
						// 変数を宣言
						CObstacle::SStatusInfo status;		// 障害物ステータス

															// 障害物のステータスを設定
						status = CObstacle::GetStatusInfo(pObjCheck->GetType());

						// 障害物の向きを設定
						rot = pObjCheck->GetVec3Rotation();

						// 障害物の位置を設定
						pos = pObjCheck->GetVec3Position();
						pos.x += sinf(rot.y + status.fAngleCenter) * status.fLengthCenter;
						pos.y = 0.0f;
						pos.z += cosf(rot.y + status.fAngleCenter) * status.fLengthCenter;

						// 障害物の大きさを設定
						sizeMax = status.sizeColl;
						sizeMin = sizeMax * -1.0f;
					}
					else
					{
						// 次のオブジェクトへのポインタを代入
						pObjCheck = pObjectNext;

						continue;
					}

					D3DXVECTOR3 posCorner[4] = {};									//箱の角
					D3DXVECTOR3 posStick = VEC3_ZERO;

					posCorner[0] = collision::PosRelativeMtx(pos, rot, D3DXVECTOR3(sizeMax.x, 0.0f, sizeMax.z));
					posCorner[1] = collision::PosRelativeMtx(pos, rot, D3DXVECTOR3(sizeMin.x, 0.0f, sizeMax.z));
					posCorner[2] = collision::PosRelativeMtx(pos, rot, D3DXVECTOR3(sizeMax.x, 0.0f, sizeMin.z));
					posCorner[3] = collision::PosRelativeMtx(pos, rot, D3DXVECTOR3(sizeMin.x, 0.0f, sizeMin.z));

					posStick.x = player->GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_41;
					posStick.y = player->GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_42;
					posStick.z = player->GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_43;

					for (int nCntCorner = 0; nCntCorner < 4; nCntCorner++)
					{
						D3DXVECTOR3 posDef;
						posDef = posCorner[nCntCorner] - posStick;
						posDef.y = 0.0f;

						if (D3DXVec3Length(&posDef) < m_fLengthChain)
						{
							D3DXVECTOR3 vecFlail, vecFlailOld;
							float rotPoint, rotFlail, rotFlailOld, rotFlailDef;

							vecFlail = GetVec3Position() - posStick;
							vecFlailOld = m_oldPos - posStick;

							rotPoint = atan2f(posDef.x, posDef.z);
							rotFlail = atan2f(vecFlail.x, vecFlail.z);
							rotFlailOld = atan2f(vecFlailOld.x, vecFlailOld.z);

							if (rotFlail > rotFlailOld)
							{
								if (rotFlail < rotPoint && rotFlail >rotFlailOld)
								{

								}
							}
							else
							{
								if (rotFlail < rotPoint && rotFlail >rotFlailOld)
								{

								}
							}

							bCol = true;
						}
					}

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;
				}
			}
		}
	}

	return false;
}
bool CFlail::CollisionGround(const CPlayer::EAxis axis, D3DXVECTOR3& rPos)
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
				sizeMaxGround.y = 0.0f;		// 縦の大きさを初期化

				switch (axis)
				{ // 判定軸ごとの処理
				case CPlayer::AXIS_X:	// X軸

								// X軸の衝突判定
					bHitBox = collision::ResponseSingleX
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

				case CPlayer::AXIS_Y:	// Y軸

								// Y軸の衝突判定
					bHitBox = collision::ResponseSingleY
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

				case CPlayer::AXIS_Z:	// Z軸

								// Z軸の衝突判定
					bHitBox = collision::ResponseSingleZ
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

				if (bHitBox)
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
bool CFlail::CollisionBlock(const CPlayer::EAxis axis, D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(RADIUS, 0.0f, RADIUS);		// プレイヤー最小大きさ
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(RADIUS, 100.0f, RADIUS);	// プレイヤー最大大きさ
	bool bHit = false;	// 着地の判定情報
	bool bHitCheck = false;	// 着地の判定情報

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

				switch (axis)
				{ // 判定軸ごとの処理
				case CPlayer::AXIS_X:	// X軸

								// X軸の衝突判定
					bHit = collision::ResponseSingleX
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

				case CPlayer::AXIS_Z:	// Z軸

								// Z軸の衝突判定
					bHit = collision::ResponseSingleZ
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

				// 障害物との判定を実行
				if (bHit)
				{
					// Hit処理
					pObjCheck->Hit();

					if (pObjCheck->GetState() == CBlock::BREAK_TRUE)
					{
						m_fChainRotMove *= 0.7f;
					}
					else
					{
						CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

						m_fChainRotMove *= 0.0f;

						if (player->GetCounterFlail() == flail::FLAIL_THROW)
						{
							player->SetCounterFlail(flail::FLAIL_DROP);
							player->SetMotion(CPlayer::MOTION_IDOL);
						}
					}

					bHitCheck = true;
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	// 各軸の判定情報を返す
	return bHitCheck;
}
bool CFlail::CollisionObstacle(D3DXVECTOR3& rPos)
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
				if (collision::Square(posObs, &rPos, m_oldPos, rotObs, sizeObsMax, sizeObsMin))
				{
					// HIT処理
					pObjCheck->Hit();

					if (pObjCheck->GetState() == CObstacle::BREAK_TRUE)
					{
						m_fChainRotMove *= 0.7f;
					}
					else
					{
						CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

						m_fChainRotMove *= 0.0f;

						if (player->GetCounterFlail() == flail::FLAIL_THROW)
						{
							player->SetCounterFlail(flail::FLAIL_DROP);
							player->SetMotion(CPlayer::MOTION_IDOL);
						}
					}

					bHitBoxCheck = true;
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	// 判定情報を返す
	return bHitBoxCheck;
}

//============================================================
//	親子付けの設定処理
//============================================================
void CFlail::BindParent(const CPlayer& rPlayer)
{
	int parentID = 0;

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM_MAX; nCntChain++)
	{
		if (nCntChain == 0)
		{
			// 親を設定
			m_chain[nCntChain].multiModel->SetParentModel(NULL);
		}
		else
		{
			parentID = nCntChain - 1;

			// 親を設定
			m_chain[nCntChain].multiModel->SetParentModel(m_chain[parentID].multiModel);
		}
	}
	
	parentID = flail::FLAIL_NUM_MAX - 1;
}

//============================================================
//	移動量の設定処理
//============================================================
void CFlail::SetVec3PosOrg(const D3DXVECTOR3& rPosOrg)
{
	// 引数の位置を設定
	m_posOrg = rPosOrg;
}

//============================================================
//	移動量の設定処理
//============================================================
D3DXVECTOR3 CFlail::GetVec3PosOrg(void)
{
	// 位置を返す
	return m_posOrg;
}

//============================================================
//	プレイヤー番号の設定処理
//============================================================
void CFlail::SetPlayerID(const int& rPlayerID)
{
	// 引数のプレイヤー番号を設定
	m_nPlayerID = rPlayerID;
}

//============================================================
//	プレイヤー番号の設定処理
//============================================================
int CFlail::GetPlayerID(void)
{
	// プレイヤー番号を返す
	return m_nPlayerID;
}

//============================================================
//	鎖の数の設定処理
//============================================================
void CFlail::SetNumChain(const int& rNumChain)
{
	// 引数の鎖の数を設定
	m_nNumChain = rNumChain;

	if (m_nNumChain > flail::FLAIL_NUM_MAX)
	{
		m_nNumChain = flail::FLAIL_NUM_MAX;
	}
	else if (m_nNumChain < flail::FLAIL_NUM_MIN)
	{
		m_nNumChain = flail::FLAIL_NUM_MIN;
	}
}

//============================================================
//	鎖の数の取得処理
//============================================================
int CFlail::GetNumChain(void)
{
	// 鎖の数を返す
	return m_nNumChain;
}

//============================================================
//	移動量の設定処理
//============================================================
void CFlail::SetMove(const D3DXVECTOR3& rMove)
{
	// 引数の位置を設定
	m_move = rMove;
}

//============================================================
//	移動量の取得処理
//============================================================
D3DXVECTOR3 CFlail::GetMove(void)
{
	// 位置を返す
	return m_move;
}

//============================================================
//	角度の設定処理
//============================================================
void CFlail::SetChainRot(const float& rChainRot)
{
	// 引数の角度を設定
	m_fChainRot = rChainRot;
}

//============================================================
//	角度の取得処理
//============================================================
float CFlail::GetChainRot(void)
{
	// 角度を返す
	return m_fChainRot;
}

//============================================================
//	目標角度の設定処理
//============================================================
void CFlail::SetChainRotTarget(const float& rChainRotTarget)
{
	// 引数の目標角度を設定
	m_fChainRotTarget = rChainRotTarget;
}

//============================================================
//	目標角度の取得処理
//============================================================
float CFlail::GetChainRotTarget(void)
{
	// 目標角度を返す
	return m_fChainRotTarget;
}

//============================================================
//	角速度の設定処理
//============================================================
void CFlail::SetChainRotMove(const float& rChainRotMove)
{
	// 引数の目標角度を設定
	m_fChainRotMove = rChainRotMove;
}

//============================================================
//	角速度の取得処理
//============================================================
float CFlail::GetChainRotMove(void)
{
	// 目標角度を返す
	return m_fChainRotMove;
}

//============================================================
//	長さの設定処理
//============================================================
void CFlail::SetLengthChain(const float& rLengthChain)
{
	// 引数の長さを設定
	m_fLengthChain = rLengthChain;
}

//============================================================
//	長さの取得処理
//============================================================
float CFlail::GetLengthChain(void)
{
	// 長さを返す
	return m_fLengthChain;
}

//============================================================
//	目標長さの設定処理
//============================================================
void CFlail::SetLengthTarget(const float& rLengthTarget)
{
	// 引数の長さを設定
	m_fLengthTarget = rLengthTarget;
}

//============================================================
//	目標長さの取得処理
//============================================================
float CFlail::GetLengthTarget(void)
{
	// 長さを返す
	return m_fLengthTarget;
}

//============================================================
//	当たり判定状況の設定処理
//============================================================
void CFlail::SetHit(const bool& Hit)
{
	// 引数の状況を設定
	m_bHit = Hit;
}

//============================================================
//	当たり判定状況の取得処理
//============================================================
bool CFlail::GetHit(void)
{
	// 状況を返す
	return m_bHit;
}

//============================================================
//	フレイル強制取得処理
//============================================================
void CFlail::CatchFlail()
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM_MAX; nCntChain++)
	{
		D3DXVECTOR3 pos, rot;
		pos = m_chain[nCntChain].multiModel->GetVec3Position();
		rot = m_chain[nCntChain].multiModel->GetVec3Rotation();
		m_chain[nCntChain].rotOld = m_chain[nCntChain].multiModel->GetVec3Rotation();
		m_chain[nCntChain].posOld = m_chain[nCntChain].multiModel->GetVec3Position();

		pos = VEC3_ZERO;

		if (nCntChain == 0)
		{
			rot = VEC3_ZERO;
		}
		else
		{
			int IDParent = nCntChain - 1;

			if (nCntChain == 1)
			{
				rot.y = m_chain[IDParent].rotOld.y - m_chain[IDParent].multiModel->GetVec3Rotation().y;
			}
			else
			{
				rot.y = m_chain[IDParent].rotOld.y * 1.0f;
			}
		}

		m_chain[nCntChain].multiModel->SetVec3Position(pos);
		m_chain[nCntChain].multiModel->SetVec3Rotation(rot);

		// モデルの更新
		m_chain[nCntChain].multiModel->Update();
	}

	m_fChainRot = 0.0f;
	//m_fLengthChain = 0.0f;
	//m_fLengthTarget = 0.0f;
	//m_fChainRotTarget = 0.0f;
	m_fChainRotMove = 0.0f;

	//player->SetCounterFlail(flail::FLAIL_DEF);
}

//============================================================
//	フレイル投げ出し処理
//============================================================
void CFlail::ShotFlail(const float rot)
{
	m_fChainRot = rot;
	m_fChainRotMove = 0.0f;

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM_MAX; nCntChain++)
	{
		D3DXVECTOR3 pos, rotChain;
		pos = m_chain[nCntChain].multiModel->GetVec3Position();
		rotChain = m_chain[nCntChain].multiModel->GetVec3Rotation();
		m_chain[nCntChain].rotOld = m_chain[nCntChain].multiModel->GetVec3Rotation();
		m_chain[nCntChain].posOld = m_chain[nCntChain].multiModel->GetVec3Position();

		if (nCntChain == 0)
		{
			pos = VEC3_ZERO;
			rotChain = VEC3_ZERO;

			rotChain.y = m_fChainRot;
		}
		else
		{
			int IDParent = nCntChain - 1;

			if (nCntChain == 1)
			{
				rotChain.y = 0.0f;
			}
			else
			{
				rotChain.y = 0.0f;
			}
		}

		m_chain[nCntChain].multiModel->SetVec3Position(pos);
		m_chain[nCntChain].multiModel->SetVec3Rotation(rotChain);

		// モデルの更新
		m_chain[nCntChain].multiModel->Update();
	}
}

//============================================================
//	軌跡の初期化
//============================================================
void CFlail::InitOrbit(void)
{
	// 軌跡を通常状態にする
	m_pOrbit->SetState(CObjectOrbit::STATE_NORMAL);
}

//============================================================
//	軌跡の消失設定
//============================================================
void CFlail::VanishOrbit(void)
{
	// 軌跡を消失状態にする
	m_pOrbit->SetState(CObjectOrbit::STATE_VANISH);
}
