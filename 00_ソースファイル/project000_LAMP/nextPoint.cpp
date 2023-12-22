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
#include "controlPoint.h"
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

	namespace gauge
	{
		const CObjectGauge3D::EType	FRAMEVISUAL = CObjectGauge3D::TYPE_TUTORIAL;	// フレーム見た目
		const CObject::ELabel	LABEL	= CObject::LABEL_UI;				// オブジェクトラベル
		const D3DXVECTOR3	SIZE		= D3DXVECTOR3(650.0f, 50.0f, 0.0f);	// ゲージ大きさ
		const D3DXVECTOR3	SIZE_FRAME	= D3DXVECTOR3(680.0f, 80.0f, 0.0f);	// フレーム大きさ
		const D3DXCOLOR		COL_FRONT	= XCOL_WHITE;						// 表ゲージ色
		const D3DXCOLOR		COL_BACK	= D3DXCOLOR(0.4f, 0.4f, 0.5f, 1.0f);// 裏ゲージ色
		const int	MAX_NUM			= 120;	// 最大表示値
		const int	CHANGE_FRAME	= 2;	// 表示値変動フレーム
		const float	POSY_UP			= 1.0f;	// 表示Y位置の加算量
	}

	namespace control
	{
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(800.0f, 0.0f, 800.0f);	// 大きさ
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(1400.0f, 0.0f, 0.0f);		// 空白
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CNextPoint::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\field_font000.png",	// 通常テクスチャ
};

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
	m_pControl = NULL;	// 操作表示の情報
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
	m_pControl = NULL;	// 操作表示の情報

	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	BindTexture(mc_apTextureFile[TEXTURE_NORMAL]);

	// 待機ゲージの生成
	m_pGauge = CObjectGauge3D::Create
	( // 引数
		gauge::LABEL,			// オブジェクトラベル
		this,					// 親オブジェクト
		gauge::MAX_NUM,			// 最大表示値
		gauge::CHANGE_FRAME,	// 表示値変動フレーム
		gauge::POSY_UP,			// 表示Y位置の加算量
		gauge::SIZE,			// ゲージ大きさ
		gauge::COL_FRONT,		// 表ゲージ色
		gauge::COL_BACK			// 裏ゲージ色
	);

	// 優先順位を設定
	m_pGauge->SetPriority(2);

	// 値を設定
	m_pGauge->SetNum(0);

	// 操作表示の生成
	m_pControl = CControlPoint::Create
	( // 引数
		VEC3_ZERO,		// 位置
		control::SIZE	// 大きさ
	);

	// 自身の影の描画をOFF・影の映り込みをONにする
	SetEnableDepthShadow(true);
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
	if (CManager::GetInstance()->GetMode() != CScene::MODE_GAME)
	{ // ゲームモードではない場合

		return;
	}

	if (m_pGauge->GetNum() < m_pGauge->GetMaxNum())
	{ // ゲージが最大ではない場合

		if (Collision())
		{ // プレイヤー全員が範囲内の場合

			// ゲージを加算
			m_pGauge->AddNum(1);
		}
		else
		{ // プレイヤー全員が範囲外の場合

			// ゲージを減算
			m_pGauge->AddNum(-2);
		}
	}
	else
	{ // ゲージが最大の場合

		// チュートリアルの終了を反映
		CManager::GetInstance()->GetRetentionManager()->EndTutorial();

		// ゲームに遷移
		CManager::GetInstance()->SetScene(CScene::MODE_GAME, WAIT_FRAME);
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
//	位置の設定処理
//============================================================
void CNextPoint::SetVec3Position(const D3DXVECTOR3 & rPos)
{
	// 自身の位置を設定
	CObject3D::SetVec3Position(rPos);

	// 操作表示の位置を設定
	m_pControl->SetVec3Position(rPos + control::SPACE);
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
