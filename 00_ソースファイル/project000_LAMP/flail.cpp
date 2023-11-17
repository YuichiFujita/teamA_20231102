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
#include "player.h"
#include "retentionManager.h"
#include "stage.h"
#include "liquid.h"
#include "scrollMeshField.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MODEL_UI_PRIO	(14)	// モデルUI表示の優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CFlail::mc_apModelFile[] =	// モデル定数
{
	"data\\MODEL\\PLAYER\\15_ironBall.x",	// 鉄球
	"data\\MODEL\\OBSTACLE\\obstacle018.x",	// プレハブ小屋
};

//************************************************************
//	子クラス [CFlail] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFlail::CFlail() : CObjectModel(CObject::LABEL_NONE, MODEL_UI_PRIO)
{
	m_move = 0.0f;
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

	BindModel(mc_apModelFile[CFlail::MODEL_FLAIL]);

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
}

//============================================================
//	更新処理
//============================================================
void CFlail::Update(void)
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

	// 鎖の長さに移動量代入
	m_fLengthChain += m_move;

	if (player->GetCounterFlail() < 0)
	{
		// 移動量減衰
		m_move += (0.0f - m_move) * 0.01f;
	}
	else
	{
		// 移動量減衰
		m_move += (0.0f - m_move) * 0.08f;
	}

	// 角度修正
	useful::NormalizeRot(m_fChainRot);
	useful::NormalizeRot(m_fChainRotMove);

	// 引っ張る時のみ角度調整
	if (m_move < 0.0f)
	{
		m_fChainRot += (m_fChainRotMove - m_fChainRot) * 0.008f;
	}

	if (m_move > 0.0f && m_move < 5.0f)
	{
		m_move = 0.0f;
	}
	
	// 角度修正
	useful::NormalizeRot(m_fChainRot);
	useful::NormalizeRot(m_fChainRotMove);

	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_RIGHT, "鎖目標角度 %f\n", m_fChainRotMove);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_RIGHT, "鎖角度 %f\n", m_fChainRot);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_RIGHT, "鎖長さ %f\n", m_fLengthChain);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_RIGHT, "[原点位置]：%f %f %f\n", m_posOrg.x, m_posOrg.y, m_posOrg.z);

	// 一定の長さを超えたら止める
	if (m_fLengthChain > 1000.0f)
	{
		m_fLengthChain = 1000.0f;
	}

	// 角度と長さから鉄球の位置決定
	D3DXVECTOR3 pos = GetVec3Position();

	pos.x = m_posOrg.x + (sinf(m_fChainRot) * m_fLengthChain);

	if (m_move == 0.0f)
	{
		if (player->GetCounterFlail() < 0)
		{
			if (pos.y > -13.0f)
			{
				pos.y -= 5.0f;
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

	pos.z = m_posOrg.z + (cosf(m_fChainRot) * m_fLengthChain);

	Collision();

	SetVec3Position(pos);

	// オブジェクトモデルの更新
	CObjectModel::Update();
}

//============================================================
//	描画処理
//============================================================
void CFlail::Draw(void)
{
	// 変数を宣言
	D3DVIEWPORT9 viewportDef;	// カメラのビューポート保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 現在のビューポートを取得
	pDevice->GetViewport(&viewportDef);

	// カメラの設定
	CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

	// オブジェクトモデルの描画
	CObjectModel::Draw();

	// カメラの設定を元に戻す
	CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

	// ビューポートを元に戻す
	pDevice->SetViewport(&viewportDef);
}

//============================================================
//	生成処理
//============================================================
CFlail *CFlail::Create
( // 引数
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

		// 確保したアドレスを返す
		return pModelUI;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	当たり判定処理
//============================================================
void CFlail::Collision(void)
{
	for (int nCntPlayer = 0; nCntPlayer < CManager::GetInstance()->GetRetentionManager()->GetNumPlayer(); nCntPlayer++)
	{
		CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);

		if (player != NULL && nCntPlayer != m_nPlayerID)
		{
			D3DXVECTOR3 vec;
			float length;

			vec = player->GetVec3Position() - GetVec3Position();
			vec.y = 0.0f;
			length = D3DXVec3Length(&vec);
			
			if (length < GetModelData().fRadius + player->GetRadius())
			{
				D3DXVECTOR3 pos = player->GetVec3Position();

				player->SetVec3Position(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z));
			}
		}
	}
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
void CFlail::SetMove(const float& rMove)
{
	// 引数の位置を設定
	m_move = rMove;
}

//============================================================
//	移動量の取得処理
//============================================================
float CFlail::GetMove(void)
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