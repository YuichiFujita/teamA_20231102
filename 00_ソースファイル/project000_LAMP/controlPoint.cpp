//============================================================
//
//	操作位置処理 [controlPoint.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "controlPoint.h"
#include "manager.h"
#include "renderer.h"
#include "objectBillboard.h"
#include "player.h"
#include "collision.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "retentionManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY = 4;	// 範囲の優先順位
	const int	WAIT_FRAME = 8;	// 遷移の余韻フレーム

	namespace tutorial
	{
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(800.0f, 500.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(0.0f, 800.0f, 0.0f);		// 空白
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CControlPoint::mc_apTextureFile[] =	// テクスチャ定数
{
	NULL,	// 通常テクスチャ
};

const char *CControlPoint::mc_apLessonFile[] =	// レッスンテクスチャ定数
{
	"data\\TEXTURE\\lesson000.png",	// 呼び込みテクスチャ
};

//************************************************************
//	子クラス [CControlPoint] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CControlPoint::CControlPoint() : CObject3D(CObject::LABEL_NEXT, PRIORITY)
{
	// メンバ変数をクリア
	m_pTutorial = NULL;	// チュートリアル表示情報
}

//============================================================
//	デストラクタ
//============================================================
CControlPoint::~CControlPoint()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CControlPoint::Init(void)
{
	// メンバ変数を初期化
	m_pTutorial = NULL;	// チュートリアル表示情報

	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	BindTexture(mc_apTextureFile[TEXTURE_NORMAL]);

	// チュートリアル表示の生成
	m_pTutorial = CObjectBillboard::Create
	( // 引数
		VEC3_ZERO,		// 位置
		tutorial::SIZE	// 大きさ
	);

	// テクスチャを登録・割当
	m_pTutorial->BindTexture(mc_apLessonFile[LESSON_YOBIKOMI]);

	// ラベルを設定
	m_pTutorial->SetLabel(LABEL_UI);

	// 自身の影の描画をOFF・影の映り込みをONにする
	SetEnableDepthShadow(true);
	SetEnableZTex(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CControlPoint::Uninit(void)
{
	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CControlPoint::Update(void)
{
	if (CManager::GetInstance()->GetMode() != CScene::MODE_GAME)
	{ // ゲームモードではない場合

		return;
	}

	// チュートリアルの操作判定
	Collision();

	// オブジェクト3Dの更新
	CObject3D::Update();
}

//============================================================
//	描画処理
//============================================================
void CControlPoint::Draw(void)
{
	// オブジェクト3Dの描画
	CObject3D::Draw();
}

//============================================================
//	位置の設定処理
//============================================================
void CControlPoint::SetVec3Position(const D3DXVECTOR3 & rPos)
{
	// 自身の位置を設定
	CObject3D::SetVec3Position(rPos);

	// チュートリアル表示の位置を設定
	m_pTutorial->SetVec3Position(rPos + tutorial::SPACE);
}

//============================================================
//	生成処理
//============================================================
CControlPoint *CControlPoint::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rSize	// 大きさ
)
{
	// ポインタを宣言
	CControlPoint *pControlPoint = NULL;	// 操作位置生成用

	if (pControlPoint == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pControlPoint = new CControlPoint;	// 操作位置
	}
	else { assert(false); return NULL; }	// 使用中

	if (pControlPoint != NULL)
	{ // 確保に成功している場合

		// 操作位置の初期化
		if (FAILED(pControlPoint->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pControlPoint;
			pControlPoint = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pControlPoint->SetVec3Position(rPos);

		// 大きさを設定
		pControlPoint->SetVec3Sizing(rSize);

		// 確保したアドレスを返す
		return pControlPoint;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	プレイヤー全員との当たり判定
//============================================================
void CControlPoint::Collision(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // プレイヤーの最大数分繰り返す

		CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);	// プレイヤー情報
		if (pPlayer != NULL)
		{ // プレイヤーが使用されている場合

			if (pPlayer->GetState() == CPlayer::STATE_DEATH)
			{ // 死亡していた場合

				continue;
			}

			float fPlayerRadius = pPlayer->GetRadius();	// プレイヤー半径

			// 矩形判定
			bool bHit = collision::Box2D
			( // 引数
				pPlayer->GetVec3Position(),								// 判定位置
				GetVec3Position(),										// 判定目標位置
				D3DXVECTOR3(fPlayerRadius, 0.0f, fPlayerRadius) * 0.5f,	// 判定サイズ(右・上・後)
				D3DXVECTOR3(fPlayerRadius, 0.0f, fPlayerRadius) * 0.5f,	// 判定サイズ(左・下・前)
				GetVec3Sizing() * 0.5f,									// 判定目標サイズ(右・上・後)
				GetVec3Sizing() * 0.5f									// 判定目標サイズ(左・下・前)
			);
			if (bHit)
			{ // 判定内の場合

				// チュートリアルの操作
				UpdateTutorial(nCntPlayer);
			}
		}
	}
}

//============================================================
//	チュートリアルの操作処理
//============================================================
void CControlPoint::UpdateTutorial(const int nID)
{

}
