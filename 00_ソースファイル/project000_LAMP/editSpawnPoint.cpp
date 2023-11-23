//============================================================
//
//	エディット生成位置処理 [editSpawnPoint.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editSpawnPoint.h"
#include "manager.h"
#include "editStageManager.h"
#include "stage.h"
#include "effect3D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_CREATE		(DIK_0)	// 生成キー
#define NAME_CREATE		("0")	// 生成表示
#define KEY_CHANGE_IDX	(DIK_2)	// インデックス変更キー
#define NAME_CHANGE_IDX	("2")	// インデックス変更表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float	POS_EFFECT_RADIUS = 120.0f;	// 位置表示のエフェクト半径
	const float	ROT_EFFECT_RADIUS = 60.0f;	// 向き表示のエフェクト半径
	const float	EFFECT_ADDLENGTH = 100.0f;	// 向き表示のエフェクトベクトル加算量
	const int	EFFECT_LIFE = 10;			// エフェクト寿命
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
int CEditSpawnPoint::m_nSave = 0;	// 保存情報

//************************************************************
//	親クラス [CEditSpawnPoint] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditSpawnPoint::CEditSpawnPoint()
{
#if _DEBUG

	// メンバ変数をクリア
	memset(&m_apSpawnPoint[0], 0, sizeof(m_apSpawnPoint));	// 生成位置情報
	m_nIdxChange = 0;	// 変更インデックス

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditSpawnPoint::~CEditSpawnPoint()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditSpawnPoint::Init(void)
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
	memset(&m_apSpawnPoint[0], 0, sizeof(m_apSpawnPoint));	// 生成位置情報
	m_nIdxChange = 0;	// 変更インデックス

	// 生成位置の使用確認
	CheckUseSpawnPoint(MAX_PLAYER, &m_apSpawnPoint[0]);

	for (int nCntSpawn = 0; nCntSpawn < MAX_PLAYER; nCntSpawn++)
	{ // プレイヤーの最大数分繰り返す

		if (m_apSpawnPoint[nCntSpawn] == NULL)
		{ // 使用されていなかった場合

			// 生成位置の生成
			m_apSpawnPoint[nCntSpawn] = CSpawnPoint::Create(posEdit, rotEdit);
			if (m_apSpawnPoint[nCntSpawn] == NULL)
			{ // 生成に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}
		}
	}

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
void CEditSpawnPoint::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditSpawnPoint::Update(void)
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

	// 生成位置の生成
	CreateSpawnPoint();

	// インデックス変更の更新
	UpdateChangeIdx();

	// 生成位置の全表示
	LookAllSpawnPoint();

	// エディット位置の表示
	LookEffect(pEdit->GetVec3Position(), pEdit->GetVec3Rotation(), XCOL_RED);

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditSpawnPoint::DrawDebugControl(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "配置インデックス変更：[%s]\n", NAME_CHANGE_IDX);
	pDebug->Print(CDebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditSpawnPoint::DrawDebugInfo(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d：[配置インデックス]\n", m_nIdxChange);
}

//============================================================
//	情報保存処理
//============================================================
void CEditSpawnPoint::SaveInfo(void)
{
	// 現在の情報を保存
	m_nSave = m_nIdxChange;
}

//============================================================
//	情報読込処理
//============================================================
void CEditSpawnPoint::LoadInfo(void)
{
	// 保存情報を設定
	m_nIdxChange = m_nSave;
}

//============================================================
//	保存処理
//============================================================
void CEditSpawnPoint::Save(FILE *pFile)
{
#if _DEBUG

	if (pFile != NULL)
	{ // ファイルが存在する場合

		// 見出しを書き出し
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#	生成位置の配置情報\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		// 情報開始地点を書き出し
		fprintf(pFile, "STAGE_SPAWNPOINTSET\n\n");

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
	
					if (pObjCheck->GetLabel() != CObject::LABEL_SPAWNPOINT)
					{ // オブジェクトラベルが生成位置ではない場合
	
						// 次のオブジェクトへのポインタを代入
						pObjCheck = pObjectNext;
	
						// 次の繰り返しに移行
						continue;
					}

					// 生成位置の情報を取得
					D3DXVECTOR3 posSpawnPoint = pObjCheck->GetVec3Position();	// 位置
					D3DXVECTOR3 rotSpawnPoint = pObjCheck->GetVec3Rotation();	// 向き
	
					// 情報を書き出し
					fprintf(pFile, "	SPAWNPOINTSET\n");
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posSpawnPoint.x, posSpawnPoint.y, posSpawnPoint.z);
					fprintf(pFile, "		ROT = %.2f %.2f %.2f\n", rotSpawnPoint.x, rotSpawnPoint.y, rotSpawnPoint.z);
					fprintf(pFile, "	END_SPAWNPOINTSET\n\n");

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;
				}
			}
		}

		// 情報終了地点を書き出し
		fprintf(pFile, "END_STAGE_SPAWNPOINTSET\n\n");
	}

#endif	// _DEBUG
}

//============================================================
//	生成位置のエフェクト表示処理
//============================================================
void CEditSpawnPoint::LookEffect
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXCOLOR& rCol		// 色
)
{
	// 変数を宣言
	D3DXVECTOR3 vecRot = VEC3_ZERO;	// 向きベクトル

	// 向きベクトルを作成
	vecRot.x = sinf(rRot.y + D3DX_PI) * EFFECT_ADDLENGTH;
	vecRot.z = cosf(rRot.y + D3DX_PI) * EFFECT_ADDLENGTH;

	// 生成位置にエフェクトを表示
	CEffect3D::Create(rPos, POS_EFFECT_RADIUS, CEffect3D::TYPE_NORMAL, EFFECT_LIFE, VEC3_ZERO, VEC3_ZERO, rCol);

	// 生成向きにエフェクトを表示
	CEffect3D::Create(rPos + vecRot, ROT_EFFECT_RADIUS, CEffect3D::TYPE_NORMAL, EFFECT_LIFE);
}

//============================================================
//	生成位置の生成処理
//============================================================
void CEditSpawnPoint::CreateSpawnPoint(void)
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

	// 生成位置を配置
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		// 配置情報を変更
		m_apSpawnPoint[m_nIdxChange]->SetVec3Position(posEdit);
		m_apSpawnPoint[m_nIdxChange]->SetVec3Rotation(rotEdit);

		// 未保存を設定
		pEdit->UnSave();
	}
}

//============================================================
//	インデックス変更の更新処理
//============================================================
void CEditSpawnPoint::UpdateChangeIdx(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	if (m_pKeyboard->IsTrigger(KEY_CHANGE_IDX))
	{
		// インデックスを変更
		m_nIdxChange = (m_nIdxChange + 1) % MAX_PLAYER;
	}
}

//============================================================
//	生成位置の全表示
//============================================================
void CEditSpawnPoint::LookAllSpawnPoint(void)
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

				if (pObjCheck->GetLabel() != CObject::LABEL_SPAWNPOINT)
				{ // オブジェクトラベルが生成位置ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 変数を宣言
				D3DXVECTOR3 posSpawn = pObjCheck->GetVec3Position();	// スポーンポイント位置
				D3DXVECTOR3 rotSpawn = pObjCheck->GetVec3Rotation();	// スポーンポイント向き
				D3DXCOLOR colEffect = XCOL_CYAN;		// エフェクト色
				int nIdxSpawn = pObjCheck->GetIndex();	// スポーンポイントインデックス

				// 範囲外例外
				assert(nIdxSpawn > NONE_IDX && nIdxSpawn < MAX_PLAYER);

				if (nIdxSpawn == m_nIdxChange)
				{ // スポーンポイントインデックスが現在選択中のインデックスの場合

					// 色を緑に変更
					colEffect = XCOL_GREEN;
				}

				// 生成位置のエフェクト表示
				LookEffect
				( // 引数
					posSpawn,	// 位置
					rotSpawn,	// 向き
					colEffect	// 色
				);

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	生成位置の使用確認
//============================================================
void CEditSpawnPoint::CheckUseSpawnPoint(const int nMaxPoint, CObject **ppAllSpawnPoint)
{
	for (int nCntSpawn = 0; nCntSpawn < nMaxPoint; nCntSpawn++)
	{ // 生成位置の数分繰り返す

		// 非使用中にする
		ppAllSpawnPoint[nCntSpawn] = NULL;
	}

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

				if (pObjCheck->GetLabel() != CObject::LABEL_SPAWNPOINT)
				{ // オブジェクトラベルが生成位置ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 範囲外例外
				assert(pObjCheck->GetIndex() > NONE_IDX && pObjCheck->GetIndex() < MAX_PLAYER);

				for (int nCntSpawn = 0; nCntSpawn < nMaxPoint; nCntSpawn++)
				{ // 生成位置の数分繰り返す

					if (pObjCheck->GetIndex() == nCntSpawn)
					{ // 現在の値がインデックスの場合

						// 使用中にする
						ppAllSpawnPoint[nCntSpawn] = pObjCheck;
					}
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}
}
