//============================================================
//
//	エディット障害物処理 [editObstacle.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editObstacle.h"
#include "manager.h"
#include "input.h"
#include "collision.h"
#include "editStageManager.h"
#include "stage.h"
#include "effect3D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_TRIGGER		(DIK_LSHIFT)	// トリガー化キー
#define NAME_TRIGGER	("LSHIFT")		// トリガー化表示
#define KEY_REVERSE		(DIK_LCONTROL)	// 操作逆転化キー
#define NAME_REVERSE	("LCTRL")		// 操作逆転化表示

#define KEY_CREATE		(DIK_0)	// 生成キー
#define NAME_CREATE		("0")	// 生成表示
#define KEY_RELEASE		(DIK_9)	// 破棄キー
#define NAME_RELEASE	("9")	// 破棄表示
#define KEY_TYPE		(DIK_2)	// 種類変更キー
#define NAME_TYPE		("2")	// 種類変更表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float	INIT_ALPHA = 0.5f;	// 配置前のα値
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CEditObstacle::SInfo CEditObstacle::m_save = {};	// 保存情報

//************************************************************
//	親クラス [CEditObstacle] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditObstacle::CEditObstacle()
{
#if _DEBUG

	// メンバ変数をクリア
	m_pObstacle = NULL;	// 障害物情報
	memset(&m_obstacle, 0, sizeof(m_obstacle));	// 障害物配置情報

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditObstacle::~CEditObstacle()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditObstacle::Init(void)
{
#if _DEBUG

	// ポインタを宣言
	CEditStageManager *pEdit = GetPtrEditStage();	// エディットステージ情報
	if (pEdit == NULL)
	{ // エディットステージが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 変数を宣言
	D3DXVECTOR3 posEdit = pEdit->GetVec3Position();	// エディットの位置
	D3DXVECTOR3 rotEdit = pEdit->GetVec3Rotation();	// エディットの向き

	// メンバ変数を初期化
	m_pObstacle		= NULL;						// 障害物情報
	m_obstacle.type	= CObstacle::TYPE_CONIFER;	// 障害物種類

	// 障害物の生成
	m_pObstacle = CObstacle::Create(m_obstacle.type, posEdit, rotEdit);
	if (m_pObstacle == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 透明度を設定
	m_pObstacle->SetAlpha(INIT_ALPHA);

	// 成功を返す
	return S_OK;

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	終了処理
//============================================================
void CEditObstacle::Uninit(void)
{
#if _DEBUG

	if (m_pObstacle != NULL)
	{ // 生成に失敗した場合

		// 障害物の色の全初期化
		InitAllColorObstacle();

		// 障害物の終了
		m_pObstacle->Uninit();
	}

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditObstacle::Update(void)
{
#if _DEBUG

	// ポインタを宣言
	CEditStageManager *pEdit = GetPtrEditStage();	// エディットステージ情報
	if (pEdit == NULL)
	{ // エディットステージが存在しない場合

		// 処理を抜ける
		assert(false);
		return;
	}

	// 種類変更の更新
	UpdateChangeType();

	// 障害物の生成
	CreateObstacle();

	// 障害物の破棄
	ReleaseObstacle();

	// 位置を反映
	m_pObstacle->SetVec3Position(pEdit->GetVec3Position());

	// 向きを反映
	m_pObstacle->SetVec3Rotation(pEdit->GetVec3Rotation());

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditObstacle::DrawDebugControl(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "種類変更：[%s]\n", NAME_TYPE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "削除：[%s]\n", NAME_RELEASE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditObstacle::DrawDebugInfo(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d：[種類]\n", m_obstacle.type);
}

//============================================================
//	情報保存処理
//============================================================
void CEditObstacle::SaveInfo(void)
{
	// 現在の情報を保存
	m_save = m_obstacle;
}

//============================================================
//	情報読込処理
//============================================================
void CEditObstacle::LoadInfo(void)
{
	// 保存情報を設定
	m_obstacle = m_save;
}

//============================================================
//	保存処理
//============================================================
void CEditObstacle::Save(FILE *pFile)
{
#if _DEBUG

	if (pFile != NULL)
	{ // ファイルが存在する場合

		// 見出しを書き出し
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#	障害物の配置情報\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		// 情報開始地点を書き出し
		fprintf(pFile, "STAGE_OBSTACLESET\n\n");

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
		
					// ポインタを宣言
					CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト
	
					if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
					{ // オブジェクトラベルが障害物ではない場合
	
						// 次のオブジェクトへのポインタを代入
						pObjCheck = pObjectNext;
	
						// 次の繰り返しに移行
						continue;
					}
	
					if (pObjCheck == (CObject*)m_pObstacle)
					{ // 同じアドレスだった場合
	
						// 次のオブジェクトへのポインタを代入
						pObjCheck = pObjectNext;
	
						// 次の繰り返しに移行
						continue;
					}

					// 障害物の情報を取得
					D3DXVECTOR3 posObs = pObjCheck->GetVec3Position();	// 位置
					D3DXVECTOR3 rotObs = pObjCheck->GetVec3Rotation();	// 向き
					int nType = pObjCheck->GetType();	// 種類
	
					// 情報を書き出し
					fprintf(pFile, "	OBSTACLESET\n");
					fprintf(pFile, "		TYPE = %d\n", nType);
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posObs.x, posObs.y, posObs.z);
					fprintf(pFile, "		ROT = %.2f %.2f %.2f\n", rotObs.x, rotObs.y, rotObs.z);
					fprintf(pFile, "	END_OBSTACLESET\n\n");

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;
				}
			}
		}

		// 情報終了地点を書き出し
		fprintf(pFile, "END_STAGE_OBSTACLESET\n\n");
	}

#endif	// _DEBUG
}

//============================================================
//	種類変更の更新処理
//============================================================
void CEditObstacle::UpdateChangeType(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	if (m_pKeyboard->IsTrigger(KEY_TYPE))
	{
		// 種類を変更
		m_obstacle.type = (CObstacle::EType)((m_obstacle.type + 1) % CObstacle::TYPE_MAX);

		// 種類を反映
		m_pObstacle->SetType(m_obstacle.type);

		// 透明度を設定
		m_pObstacle->SetAlpha(INIT_ALPHA);
	}
}

//============================================================
//	障害物の生成処理
//============================================================
void CEditObstacle::CreateObstacle(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報
	CEditStageManager *pEdit = GetPtrEditStage();	// エディットステージ情報
	if (pEdit == NULL)
	{ // エディットステージが存在しない場合

		// 処理を抜ける
		assert(false);
		return;
	}

	// 変数を宣言
	D3DXVECTOR3 posEdit = pEdit->GetVec3Position();	// エディットの位置
	D3DXVECTOR3 rotEdit = pEdit->GetVec3Rotation();	// エディットの向き

	// 障害物を配置
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	障害物の情報を配置用に変更
		//----------------------------------------------------
		// 自動更新・自動描画をONにする
		m_pObstacle->SetEnableUpdate(true);
		m_pObstacle->SetEnableDraw(true);

		// 透明度を設定
		m_pObstacle->SetAlpha(1.0f);

		// 未保存を設定
		pEdit->UnSave();

		//----------------------------------------------------
		//	新しい障害物の生成
		//----------------------------------------------------
		// 障害物の生成
		m_pObstacle = CObstacle::Create(m_obstacle.type, posEdit, rotEdit);
		assert(m_pObstacle != NULL);

		// 透明度を設定
		m_pObstacle->SetAlpha(INIT_ALPHA);
	}
}

//============================================================
//	障害物の破棄処理
//============================================================
void CEditObstacle::ReleaseObstacle(void)
{
	// 変数を宣言
	bool bRelease = false;	// 破棄状況

	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 障害物を削除
	if (m_pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// 破棄する状態を設定
		bRelease = true;
	}

	// 障害物の削除判定
	DeleteCollisionObstacle(bRelease);
}

//============================================================
//	障害物の削除判定
//============================================================
void CEditObstacle::DeleteCollisionObstacle(const bool bRelase)
{
	// ポインタを宣言
	CEditStageManager *pEdit = GetPtrEditStage();	// エディットステージ情報
	if (pEdit == NULL)
	{ // エディットステージが存在しない場合

		// 処理を抜ける
		assert(false);
		return;
	}

	// 変数を宣言
	D3DXVECTOR3 posEdit = pEdit->GetVec3Position();	// エディットの位置

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
				D3DXVECTOR3 posObs = VEC3_ZERO;	// 障害物位置
				float fRadiusObs = 0.0f;		// 障害物半径

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
				{ // オブジェクトラベルが障害物ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				if (pObjCheck == (CObject*)m_pObstacle)
				{ // 同じアドレスだった場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 障害物の位置を取得
				posObs = pObjCheck->GetVec3Position();

				// 障害物の半径を取得
				fRadiusObs = pObjCheck->GetRadius();

				// 球体の当たり判定
				if (collision::Circle3D
				( // 引数
					posEdit,	// 判定位置
					posObs,		// 判定目標位置
					fRadiusObs,	// 判定半径
					200.0f		// 判定目標半径
				))
				{ // 判定内だった場合

					if (bRelase)
					{ // 破棄する場合

						// 終了処理
						pObjCheck->Uninit();

						// 未保存を設定
						pEdit->UnSave();
					}
					else
					{ // 破棄しない場合

						// 赤マテリアルを設定
						pObjCheck->SetAllMaterial(material::Red());
					}
				}
				else
				{ // 判定外だった場合

					// マテリアルを再設定
					pObjCheck->ResetMaterial();
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	障害物の色の全初期化処理
//============================================================
void CEditObstacle::InitAllColorObstacle(void)
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

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
				{ // オブジェクトラベルが障害物ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				if (pObjCheck == (CObject*)m_pObstacle)
				{ // 同じアドレスだった場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				if (!pObjCheck->IsDeath())
				{ // 死亡していない場合

					// マテリアルを再設定
					pObjCheck->ResetMaterial();
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}
}
