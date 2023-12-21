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
#include "sound.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY = 4;	// 範囲の優先順位
	const int	WAIT_FRAME = 8;	// 遷移の余韻フレーム

	namespace tutorial
	{
		const int	PRIORITY	= 9;		// レッスン表示の優先順位
		const float	MIN_SCALE	= 1.0f;		// 最小の拡大率
		const float	MAX_SCALE	= 1.9f;		// 最大の拡大率
		const float	REV_SCALE	= 0.45f;	// 拡大率の補正係数

		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(800.0f, 500.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(0.0f, 300.0f, 0.0f);		// 空白

		const D3DXCOLOR	COL_NORMAL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 通常カラー
		const D3DXCOLOR	COL_LAST	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// 最終カラー
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CControlPoint::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\field_font001.png",	// 地面テクスチャ
	"data\\TEXTURE\\tutorial_next.png",	// 次テクスチャ
	"data\\TEXTURE\\tutorial_prev.png",	// 前テクスチャ
};

const char *CControlPoint::mc_apLessonFile[] =	// レッスンテクスチャ定数
{
	"data\\TEXTURE\\lesson000.png",	// 移動テクスチャ
	"data\\TEXTURE\\lesson001.png",	// ダッシュテクスチャ
	"data\\TEXTURE\\lesson002.png",	// 攻撃テクスチャ
	"data\\TEXTURE\\lesson003.png",	// 攻撃種類テクスチャ
	"data\\TEXTURE\\lesson004.png",	// フックショットテクスチャ
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
	m_pTutorial = NULL;		// チュートリアル表示情報
	m_pNext = NULL;			// 次操作の表示情報
	m_pPrev = NULL;			// 前操作の表示情報
	m_nLesson = LESSON_00;	// レッスン
	m_fScale = 0.0f;		// 拡大率
	m_fDestScale = 0.0f;	// 目標拡大率
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
	m_pTutorial = NULL;		// チュートリアル表示情報
	m_pNext = NULL;			// 次操作の表示情報
	m_pPrev = NULL;			// 前操作の表示情報
	m_nLesson = LESSON_00;	// レッスン
	m_fScale = 1.0f;		// 拡大率
	m_fDestScale = 1.0f;	// 目標拡大率

	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	BindTexture(mc_apTextureFile[TEXTURE_FIELD]);

	// チュートリアル表示の生成
	m_pTutorial = CObjectBillboard::Create
	( // 引数
		VEC3_ZERO,		// 位置
		tutorial::SIZE	// 大きさ
	);

	// テクスチャを登録・割当
	m_pTutorial->BindTexture(mc_apLessonFile[m_nLesson]);

	// ラベルを設定
	m_pTutorial->SetLabel(LABEL_UI);

	// 優先順位を設定
	m_pTutorial->SetPriority(tutorial::PRIORITY);

	// 前操作表示の生成
	m_pPrev = CObjectBillboard::Create
	( // 引数
		VEC3_ZERO,		// 位置
		tutorial::SIZE	// 大きさ
	);

	// テクスチャを登録・割当
	m_pPrev->BindTexture(mc_apTextureFile[TEXTURE_PREV]);

	// ラベルを設定
	m_pPrev->SetLabel(LABEL_UI);

	// 優先順位を設定
	m_pPrev->SetPriority(tutorial::PRIORITY);

	// 次操作表示の生成
	m_pNext = CObjectBillboard::Create
	( // 引数
		VEC3_ZERO,		// 位置
		tutorial::SIZE	// 大きさ
	);

	// テクスチャを登録・割当
	m_pNext->BindTexture(mc_apTextureFile[TEXTURE_NEXT]);

	// ラベルを設定
	m_pNext->SetLabel(LABEL_UI);

	// 優先順位を設定
	m_pNext->SetPriority(tutorial::PRIORITY);

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
	if (Collision())
	{ // 誰かが操作の範囲内にいた場合

		// 表示を拡大
		m_fDestScale = tutorial::MAX_SCALE;

		// 操作の描画をONにする
		m_pPrev->SetEnableDraw(true);	// 前操作表示
		m_pNext->SetEnableDraw(true);	// 次操作表示
	}
	else
	{ // 誰もいなかった場合

		// 表示を縮小
		m_fDestScale = tutorial::MIN_SCALE;

		// 操作の描画をOFFにする
		m_pPrev->SetEnableDraw(false);	// 前操作表示
		m_pNext->SetEnableDraw(false);	// 次操作表示
	}

	// レッスンの更新
	UpdateLesson();

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

	// 前操作表示の位置を設定
	m_pPrev->SetVec3Position(rPos + tutorial::SPACE);

	// 次操作表示の位置を設定
	m_pNext->SetVec3Position(rPos + tutorial::SPACE);
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
bool CControlPoint::Collision(void)
{
	// 変数を宣言
	bool bColl = false;	// 判定状況

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

				// 判定があった状態にする
				bColl = true;
			}
		}
	}

	// 判定状況を返す
	return bColl;
}

//============================================================
//	チュートリアルの操作処理
//============================================================
void CControlPoint::UpdateTutorial(const int nID)
{
	// ポインタを宣言
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// データ保存情報
	CInputPad *pPad = CManager::GetInstance()->GetPad();	// パッド

	if (pPad->IsTrigger(CInputPad::KEY_Y, nID))
	{ // 右移動の操作が行われた場合

		// レッスンを進める
		m_nLesson++;

		if (m_nLesson >= LESSON_MAX)
		{ // レッスンが最大値を超えた場合

			// レッスンを補正 (戻す)
			m_nLesson--;
		}
		else
		{ // レッスンが超えていない場合

			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
		}

		// テクスチャを登録・割当
		m_pTutorial->BindTexture(mc_apLessonFile[m_nLesson]);
	}
	if (pPad->IsTrigger(CInputPad::KEY_X, nID))
	{ // 左移動の操作が行われた場合

		// レッスンを戻す
		m_nLesson--;

		if (m_nLesson <= NONE_IDX)
		{ // レッスンが最小値を下回った場合

			// レッスンを補正 (進める)
			m_nLesson++;
		}
		else
		{ // レッスンが下回っていない場合

			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// 選択操作音00
		}

		// テクスチャを登録・割当
		m_pTutorial->BindTexture(mc_apLessonFile[m_nLesson]);
	}
}

//============================================================
//	レッスンの更新処理
//============================================================
void CControlPoint::UpdateLesson(void)
{
	// 変数を宣言
	float fDiffScale = 0.0f;	// 差分の拡大率

	// 拡大率の差分を計算
	fDiffScale = m_fDestScale - m_fScale;

	// 拡大率を計算
	m_fScale += fDiffScale * tutorial::REV_SCALE;

	// チュートリアル表示の大きさを設定
	m_pTutorial->SetVec3Sizing(tutorial::SIZE * m_fScale);

	// 前操作表示の大きさを設定
	m_pPrev->SetVec3Sizing(tutorial::SIZE * m_fScale);

	// 次操作表示の大きさを設定
	m_pNext->SetVec3Sizing(tutorial::SIZE * m_fScale);

	// 前操作表示の色を設定
	if (m_nLesson <= 0)
	{ // 前がこれ以上ない場合

		m_pPrev->SetColor(tutorial::COL_LAST);
	}
	else
	{ // 前がある場合

		m_pPrev->SetColor(tutorial::COL_NORMAL);
	}

	// 次操作表示の色を設定
	if (m_nLesson >= LESSON_MAX - 1)
	{ // 次がこれ以上ない場合

		m_pNext->SetColor(tutorial::COL_LAST);
	}
	else
	{ // 次がある場合

		m_pNext->SetColor(tutorial::COL_NORMAL);
	}
}
