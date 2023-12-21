//============================================================
//
//	遷移位置処理 [nextPoint.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "nextPoint.h"
#include "manager.h"
#include "renderer.h"
#include "objectGauge3D.h"
#include "player.h"
#include "collision.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY = 4;	// 範囲の優先順位
}

//************************************************************
//	子クラス [CNextPoint] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CNextPoint::CNextPoint() : CObject3D(CObject::LABEL_NEXT, PRIORITY)
{
	// メンバ変数をクリア
	m_pGauge = NULL;	// 待機ゲージ情報
}

//============================================================
//	デストラクタ
//============================================================
CNextPoint::~CNextPoint()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CNextPoint::Init(void)
{
	// メンバ変数を初期化
	m_pGauge = NULL;	// 待機ゲージ情報

	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 待機ゲージの生成
	m_pGauge = CObjectGauge3D::Create
	( // 引数
		LABEL_UI,
		this,
		10,
		2,
		350.0f,
		D3DXVECTOR3(220.0f, 40.0f, 0.0f)
	);

	// 影の描画をOFFにする
	SetEnableDepthShadow(false);
	SetEnableZTex(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CNextPoint::Uninit(void)
{
	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CNextPoint::Update(void)
{
	if (Collision())
	{ // プレイヤー全員が範囲内の場合


	}

	// オブジェクト3Dの更新
	CObject3D::Update();
}

//============================================================
//	描画処理
//============================================================
void CNextPoint::Draw(void)
{
	// オブジェクト3Dの描画
	CObject3D::Draw();
}

//============================================================
//	生成処理
//============================================================
CNextPoint *CNextPoint::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rSize	// 大きさ
)
{
	// ポインタを宣言
	CNextPoint *pNextPoint = NULL;	// 遷移位置生成用

	if (pNextPoint == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pNextPoint = new CNextPoint;	// 遷移位置
	}
	else { assert(false); return NULL; }	// 使用中

	if (pNextPoint != NULL)
	{ // 確保に成功している場合

		// 遷移位置の初期化
		if (FAILED(pNextPoint->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pNextPoint;
			pNextPoint = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pNextPoint->SetVec3Position(rPos);

		// 大きさを設定
		pNextPoint->SetVec3Sizing(rSize);

		// 確保したアドレスを返す
		return pNextPoint;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	プレイヤー全員との当たり判定
//============================================================
bool CNextPoint::Collision(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // プレイヤーの最大数分繰り返す

		CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);	// プレイヤー情報
		if (pPlayer != NULL)
		{ // プレイヤーが使用されている場合

			if (pPlayer->GetState() == CPlayer::STATE_DEATH)
			{ // 死亡していた場合

				break;
			}

			float fPlayerRadius = pPlayer->GetRadius();	// プレイヤー半径

			// 矩形判定
			bool bHit = collision::Box2D
			( // 引数
				pPlayer->GetVec3Position(),							// 判定位置
				GetVec3Position(),									// 判定目標位置
				D3DXVECTOR3(fPlayerRadius, 0.0f, fPlayerRadius),	// 判定サイズ(右・上・後)
				D3DXVECTOR3(fPlayerRadius, 0.0f, fPlayerRadius),	// 判定サイズ(左・下・前)
				GetVec3Sizing(),									// 判定目標サイズ(右・上・後)
				GetVec3Sizing()										// 判定目標サイズ(左・下・前)
			);

			if (!bHit)
			{ // 判定外の場合

				// 全員が範囲外ではないためfalse
				return false;
			}
		}
	}

	// ここにたどり着けば全員が範囲内 == true
	return true;
}
