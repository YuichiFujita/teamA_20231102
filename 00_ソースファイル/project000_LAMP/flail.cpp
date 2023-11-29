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

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY	= 3;	// フレイルの優先順位

	const float	RADIUS		= 50.0f;	// 半径
	const int	HIT_DAMAGE	= 10;		// ダメージ量
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CFlail::mc_apModelFileFlail[] =	// モデル定数(フレイル)
{
	"data\\MODEL\\PLAYER\\ironBall_Resize.x",	// 鉄球
	"data\\MODEL\\OBSTACLE\\obstacle018.x",	// プレハブ小屋
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
	m_fChainRot = 0.0f;
	m_fLengthChain = 0.0f;
	m_fChainRotMove = 0.0f;
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

	BindModel(mc_apModelFileFlail[FLAIL_NORMAL]);

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM; nCntChain++)
	{
		// モデルの生成
		m_chain[nCntChain].multiModel = CMultiModel::Create(VEC3_ZERO, VEC3_ZERO);

		// モデルを割当
		m_chain[nCntChain].multiModel->BindModel(mc_apModelFileChain[CFlail::CHAIN_NORMAL]);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CFlail::Uninit(void)
{
	// オブジェクトモデルの終了
	CObjectModel::Uninit();

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM; nCntChain++)
	{
		// モデルの終了
		m_chain[nCntChain].multiModel->Uninit();
	}
}

//============================================================
//	更新処理
//============================================================
void CFlail::Update(void)
{
	m_oldPos = GetVec3Position();
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);
	m_fLengthChain = 0.0f;

	// 角度修正
	useful::NormalizeRot(m_fChainRot);
	useful::NormalizeRot(m_fChainRotMove);

	// 引っ張る時のみ角度調整
	if (player->GetCounterFlail() < flail::FLAIL_DROP)
	{
		m_fChainRot += (m_fChainRotMove - m_fChainRot) * 0.008f;
	}

	if (D3DXVec3Length(&m_move) > 0.0f && D3DXVec3Length(&m_move) < 5.0f)
	{
		m_move.x = 0.0f;
		m_move.z = 0.0f;
	}
	
	// 角度修正
	useful::NormalizeRot(m_fChainRot);
	useful::NormalizeRot(m_fChainRotMove);

	if (m_nPlayerID == 0)
	{
		/*CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "鎖目標角度 %f\n", m_fChainRotMove);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "鎖角度 %f\n", m_fChainRot);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "鎖長さ %f\n", m_fLengthChain);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[カウンター]：%d\n", player->GetCounterFlail());*/
	}

	// オブジェクトモデルの更新
	CObjectModel::Update();

	// 鎖の更新
	UpdateChain();
}

//============================================================
//	フレイル位置の更新処理
//============================================================
void CFlail::UpdateFlailPos(void)
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

	int partsNum = flail::FLAIL_NUM - 1;
	D3DXMATRIX partsMtx = m_chain[partsNum].multiModel->GetMtxWorld();
	D3DXVECTOR3 partsPos = D3DXVECTOR3(partsMtx._41, partsMtx._42, partsMtx._43);
	SetVec3PosOrg(partsPos);

	// 角度と長さから鉄球の位置決定
	D3DXVECTOR3 pos = GetVec3Position();

	if (D3DXVec3Length(&m_move) == 0.0f)
	{
		if (player->GetCounterFlail() < flail::FLAIL_DEF)
		{
			if (pos.y > -13.0f)
			{
				pos.y -= 0.5f;
			}
			else
			{
				pos.y = CManager::GetInstance()->GetScene()->GetStage()->GetLiquid()->GetScrollMeshField(0)->GetPositionHeight(pos);
			}
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

	pos.x = m_posOrg.x + (sinf(m_fChainRot) * 1.0f);
	pos.z = m_posOrg.z + (cosf(m_fChainRot) * 1.0f);

	if (player->GetCounterFlail() < flail::FLAIL_DEF || player->GetCounterFlail() == flail::FLAIL_THROW)
	{
		Collision(pos);
	}

	SetVec3Position(pos);
}

//============================================================
//	鎖の更新処理
//============================================================
void CFlail::UpdateChain(void)
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM; nCntChain++)
	{
		D3DXVECTOR3 pos, rot;
		m_chain[nCntChain].rotOld = m_chain[nCntChain].multiModel->GetVec3Rotation();
		pos = m_chain[nCntChain].multiModel->GetVec3Position();
		rot = m_chain[nCntChain].multiModel->GetVec3Rotation();

		if (nCntChain == 0)
		{
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

			if (nCntChain == 1)
			{
				rot.y = m_chain[IDParent].rotOld.y - m_chain[IDParent].multiModel->GetVec3Rotation().y;
			}
			else
			{
				rot.y = m_chain[IDParent].rotOld.y * 0.80f;
			}

			if (player->GetCounterFlail() == flail::FLAIL_THROW)
			{
				if ((m_chain[IDParent].multiModel->GetVec3Position().x >= 4.0f) || nCntChain == 1)
				{
					pos.x += 3.9f;

					if (pos.x > 4.0f)
					{
						pos.x = 4.0f;
					}
				}
			}

			if (player->GetCounterFlail() < flail::FLAIL_DROP)
			{
				if (m_chain[IDParent].multiModel->GetVec3Position().x <= 0.0f)
				{
					pos.x -= -0.04f * player->GetCounterFlail();
					
					if (pos.x < 0.0f)
					{
						pos.x = 0.0f;
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
}

//============================================================
//	描画処理
//============================================================
void CFlail::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

	D3DXMATRIX mtx;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	mtx._41 = player->GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_41;
	mtx._42 = player->GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_42;
	mtx._43 = player->GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_43;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM; nCntChain++)
	{
		int IDParent = nCntChain - 1;
		D3DXVECTOR3 rotOld = m_chain[nCntChain].multiModel->GetVec3Rotation();
		D3DXVECTOR3 rotNow = m_chain[nCntChain].multiModel->GetVec3Rotation();

		if (nCntChain != 0)
		{
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

		if (m_chain[nCntChain].multiModel->GetVec3Position().x == 0.0f)
		{
			// 変数を宣言
			D3DXMATRIX   mtxScale, mtxRot, mtxTrans, *mtxOrg;	// 計算用マトリックス
			D3DXMATRIX   mtxParent;	// 親のマトリックス
			D3DXVECTOR3 pos, rot, scale;

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

		if (nCntChain != 0)
		{
			m_chain[nCntChain].multiModel->SetVec3Rotation(rotOld);
		}
	}

	UpdateFlailPos();

	// オブジェクトモデルの描画
	CObjectModel::Draw();
}

//============================================================
//	生成処理
//============================================================
CFlail *CFlail::Create
( // 引数
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

		// メモリ確保
		pModelUI = new CFlail;	// モデルUI
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

		// 確保したアドレスを返す
		return pModelUI;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	当たり判定処理
//============================================================
void CFlail::Collision(D3DXVECTOR3& rPos)
{
	for (int nCntPlayer = 0; nCntPlayer < CManager::GetInstance()->GetRetentionManager()->GetNumPlayer(); nCntPlayer++)
	{
		CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);

		if (player != NULL && nCntPlayer != m_nPlayerID)
		{
			D3DXVECTOR3 vec;
			float length;

			// プレイヤーとフレイルのベクトルを求める
			vec = player->GetVec3Position() - GetVec3Position();
			vec.y = 0.0f;	// Yは無視

			// 距離を求める
			length = D3DXVec3Length(&vec);

			// 吹っ飛びベクトルを正規化
			D3DXVec3Normalize(&vec, &vec);
			
			if (length < RADIUS + player->GetRadius())
			{
				// ダメージヒット処理
				player->HitKnockBack(HIT_DAMAGE, vec);
			}
		}
	}

	CollisionGround(CPlayer::AXIS_X, rPos);
	CollisionBlock(CPlayer::AXIS_X, rPos);

	CollisionGround(CPlayer::AXIS_Y, rPos);
	CollisionBlock(CPlayer::AXIS_Y, rPos);

	CollisionGround(CPlayer::AXIS_Z, rPos);
	CollisionBlock(CPlayer::AXIS_Z, rPos);
}

void CFlail::CollisionGround(const CPlayer::EAxis axis, D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(8.0f, 8.0f, 8.0f);		// プレイヤー最小大きさ
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(8.0f, 8.0f, 8.0f);		// プレイヤー最大大きさ
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
				case CPlayer::AXIS_X:	// X軸

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

				case CPlayer::AXIS_Y:	// Y軸

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

				case CPlayer::AXIS_Z:	// Z軸

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
}

void CFlail::CollisionBlock(const CPlayer::EAxis axis, D3DXVECTOR3& rPos)
{
	// 変数を宣言
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(8.0f, 8.0f, 8.0f);		// プレイヤー最小大きさ
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(8.0f, 8.0f, 8.0f);		// プレイヤー最大大きさ
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

				if (pObjCheck->GetLabel() != CObject::LABEL_BLOCK)
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

				case CPlayer::AXIS_Y:	// Y軸

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

				case CPlayer::AXIS_Z:	// Z軸

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
}

//============================================================
//	親子付けの設定処理
//============================================================
void CFlail::BindParent(const CPlayer& rPlayer)
{
	int parentID = 0;

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM; nCntChain++)
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
	
	parentID = flail::FLAIL_NUM - 1;
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
void CFlail::SetChainRotMove(const float& rChainRotMove)
{
	// 引数の目標角度を設定
	m_fChainRotMove = rChainRotMove;
}

//============================================================
//	目標角度の取得処理
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