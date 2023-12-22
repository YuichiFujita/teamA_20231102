//============================================================
//
//	開始表示[Ready.cpp]
//	Author：TANNO RYUNOSUKE
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "Ready.h"
#include "manager.h"
#include "fade.h"
#include "sound.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "retentionManager.h"
#include "player.h"
#include "particle2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY = 12;	// 開始の優先順位
}

//************************************************************
//	子クラス [CReady] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CReady::CReady()
{
	// メンバ変数をクリア

}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CReady::CReady(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	
}

//============================================================
//	デストラクタ
//============================================================
CReady::~CReady()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CReady::Init(void)
{
	// メンバ変数を初期化
	m_pTexReady = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, -100.0f, 0.0f), D3DXVECTOR3(405.0f, 209.0f, 0.0f));
	m_pTexReady->BindTexture("data\\TEXTURE\\Ready.png");
	
	m_pTexFight = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(355.0f, 210.0f, 0.0f));
	m_pTexFight->BindTexture("data\\TEXTURE\\Fight.png");
	m_pTexFight->SetEnableDraw(false);
	m_nStateCount = 120;
	m_pFlare = CAnim2D::Create(23, 2, SCREEN_CENT, SCREEN_SIZE);
	m_pFlare->BindTexture("data\\TEXTURE\\lensFlare.jpg");
	m_pFlare->SetCounter(2);
	m_pFlare->SetAdd(true);
	m_pFlare->SetEnableStop(true);
	m_pFlare->SetLabel(LABEL_EFFECT);

	m_pTexReady->SetPriority(PRIORITY);
	m_pTexFight->SetPriority(PRIORITY);
	m_pFlare->SetPriority(PRIORITY);

	// サウンドの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_NOISE);	// ノイズ音

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CReady::Uninit(void)
{
	// オブジェクト2Dの終了
	if (m_pTexReady != NULL)
	{
		m_pTexReady->Uninit();
		m_pTexReady = NULL;
	}
	if (m_pTexFight != NULL)
	{
		m_pTexFight->Uninit();
		m_pTexFight = NULL;
	}
	if (m_pFlare != NULL)
	{
		m_pFlare->Uninit();
		m_pFlare = NULL;
	}
	Release();
}

//============================================================
//	更新処理
//============================================================
void CReady::Update(void)
{
	if (CManager::GetInstance()->GetFade()->GetState() == CFade::FADE_NONE)
	{
		m_pTexReady->SetVec3Position(D3DXVECTOR3(m_pTexReady->GetVec3Position() + (D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f) - m_pTexReady->GetVec3Position())* 0.1f));
		m_nStateCount--;

		if (m_nStateCount <= 0)
		{
			if (m_nStateCount == 0)
			{
				m_pFlare->SetEnableStop(false);

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_FIGHT);	// 戦闘音
			}
			m_pTexReady->SetEnableDraw(false);
			m_pTexFight->SetEnableDraw(true);
			if (m_pFlare->GetLoopAnimation() > 0)
			{ 
				CSceneGame::GetGameManager()->SetState(CGameManager::STATE_NORMAL);

				for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
				{ // プレイヤー数分繰り返す

					CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);	// プレイヤー情報

					if (pPlayer != NULL)
					{ // プレイヤーが存在する場合

						// プレイヤーを出現
						pPlayer->SetSpawn();
					}
				}
				
				Uninit();
			}
		}
	}
}

//============================================================
//	描画処理
//============================================================
void CReady::Draw(void)
{

}

//============================================================
//	生成処理
//============================================================
CReady *CReady::Create()
{	// ポインタを宣言
	CReady *pReady = NULL;	// アニメーション2D生成用

	if (pReady == NULL)
	{ // 使用されていない場合

	  // メモリ確保
		pReady = new CReady(ELabel::LABEL_UI);	// アニメーション2D
	}
	else { assert(false); return NULL; }	// 使用中

	if (pReady != NULL)
	{ // 確保に成功している場合

	  //初期化
		if (FAILED(pReady->Init()))
		{ // 初期化に失敗した場合

		  // メモリ開放
			delete pReady;
			pReady = NULL;

			// 失敗を返す
			return NULL;
		}

		

		// 確保したアドレスを返す
		return pReady;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
void CReady::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}