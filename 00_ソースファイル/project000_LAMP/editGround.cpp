//============================================================
//
//	エディット地盤処理 [editGround.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editGround.h"
#include "manager.h"
#include "input.h"
#include "collision.h"
#include "editStageManager.h"
#include "stage.h"
#include "effect3D.h"
#include "debugproc.h"

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

#define KEY_UP_SCALE_X		(DIK_T)	// X軸拡大キー
#define NAME_UP_SCALE_X		("T")	// X軸拡大表示
#define KEY_DOWN_SCALE_X	(DIK_G)	// X軸縮小キー
#define NAME_DOWN_SCALE_X	("G")	// X軸縮小表示
#define KEY_UP_SCALE_Y		(DIK_Y)	// Y軸拡大キー
#define NAME_UP_SCALE_Y		("Y")	// Y軸拡大表示
#define KEY_DOWN_SCALE_Y	(DIK_H)	// Y軸縮小キー
#define NAME_DOWN_SCALE_Y	("H")	// Y軸縮小表示
#define KEY_UP_SCALE_Z		(DIK_U)	// Z軸拡大キー
#define NAME_UP_SCALE_Z		("U")	// Z軸拡大表示
#define KEY_DOWN_SCALE_Z	(DIK_J)	// Z軸縮小キー
#define NAME_DOWN_SCALE_Z	("J")	// Z軸縮小表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(50.0f, 50.0f, 50.0f);	// 大きさ
	const float	MAX_SIZE	= 10000.0f;	// 最大の大きさ
	const float	INIT_ALPHA	= 0.5f;		// 配置前のα値
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CEditGround::SInfo CEditGround::m_save = {};	// 保存情報

//************************************************************
//	親クラス [CEditGround] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditGround::CEditGround()
{
#if _DEBUG

	// メンバ変数をクリア
	m_pGround = NULL;	// 地盤情報
	memset(&m_ground, 0, sizeof(m_ground));	// 地盤配置情報

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditGround::~CEditGround()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditGround::Init(void)
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
	m_pGround		= NULL;					// 地盤情報
	m_ground.type	= CGround::TYPE_GRASS;	// 地盤種類
	m_ground.size	= INIT_SIZE;			// 大きさ
	m_ground.partX	= VEC2_ONE;				// テクスチャ分割数X
	m_ground.partY	= VEC2_ONE;				// テクスチャ分割数Y
	m_ground.partZ	= VEC2_ONE;				// テクスチャ分割数Z

	// 地盤の生成
	m_pGround = CGround::Create(m_ground.type, posEdit, rotEdit, m_ground.size);
	if (m_pGround == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 色を設定
	D3DXCOLOR col = m_pGround->GetColor();	// 元の色を取得
	m_pGround->SetColor(D3DXCOLOR(col.r, col.g, col.b, INIT_ALPHA));

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
void CEditGround::Uninit(void)
{
#if _DEBUG

	if (m_pGround != NULL)
	{ // 生成に失敗した場合

		// 地盤の色の全初期化
		InitAllColorGround();

		// 地盤の終了
		m_pGround->Uninit();
	}

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditGround::Update(void)
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

	// 大きさの更新
	UpdateSizing();

	// 種類変更の更新
	UpdateChangeType();

	// テクスチャ分割の更新
	UpdateTexPart();

	// 地盤の生成
	CreateGround();

	// 地盤の破棄
	ReleaseGround();

	// 位置を反映
	m_pGround->SetVec3Position(pEdit->GetVec3Position());

	// 向きを反映
	m_pGround->SetVec3Rotation(pEdit->GetVec3Rotation());

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditGround::DrawDebugControl(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "大きさ：[%s/%s/%s/%s/%s/%s+%s]\n", NAME_UP_SCALE_X, NAME_DOWN_SCALE_X, NAME_UP_SCALE_Y, NAME_DOWN_SCALE_Y, NAME_UP_SCALE_Z, NAME_DOWN_SCALE_Z, NAME_TRIGGER);
	pDebug->Print(CDebugProc::POINT_RIGHT, "種類変更：[%s]\n", NAME_TYPE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "削除：[%s]\n", NAME_RELEASE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "設置：[%s]\n", NAME_CREATE);
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditGround::DrawDebugInfo(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d：[種類]\n", m_ground.type);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f %f：[大きさ]\n", m_ground.size.x, m_ground.size.y, m_ground.size.z);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f：[テクスチャ分割X]\n", m_ground.partX.x, m_ground.partX.y);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f：[テクスチャ分割Y]\n", m_ground.partY.x, m_ground.partY.y);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f：[テクスチャ分割Z]\n", m_ground.partZ.x, m_ground.partZ.y);
}

//============================================================
//	情報保存処理
//============================================================
void CEditGround::SaveInfo(void)
{
	// 現在の情報を保存
	m_save = m_ground;
}

//============================================================
//	情報読込処理
//============================================================
void CEditGround::LoadInfo(void)
{
	// 保存情報を設定
	m_ground = m_save;
}

//============================================================
//	保存処理
//============================================================
void CEditGround::Save(FILE *pFile)
{
#if _DEBUG

	if (pFile != NULL)
	{ // ファイルが存在する場合

		// 見出しを書き出し
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#	地盤の配置情報\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		// 情報開始地点を書き出し
		fprintf(pFile, "STAGE_GROUNDSET\n\n");

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
	
					if (pObjCheck->GetLabel() != CObject::LABEL_GROUND)
					{ // オブジェクトラベルが地盤ではない場合
	
						// 次のオブジェクトへのポインタを代入
						pObjCheck = pObjectNext;
	
						// 次の繰り返しに移行
						continue;
					}
	
					if (pObjCheck == (CObject*)m_pGround)
					{ // 同じアドレスだった場合
	
						// 次のオブジェクトへのポインタを代入
						pObjCheck = pObjectNext;
	
						// 次の繰り返しに移行
						continue;
					}

					// 地盤の情報を取得
					D3DXVECTOR3 posGround = pObjCheck->GetVec3Position();	// 位置
					D3DXVECTOR3 rotGround = pObjCheck->GetVec3Rotation();	// 向き
					D3DXVECTOR3 sizeGround = pObjCheck->GetVec3Sizing();	// 大きさ
					int nType = pObjCheck->GetType();	// 種類
	
					// 情報を書き出し
					fprintf(pFile, "	GROUNDSET\n");
					fprintf(pFile, "		TYPE = %d\n", nType);
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posGround.x, posGround.y, posGround.z);
					fprintf(pFile, "		ROT = %.2f %.2f %.2f\n", rotGround.x, rotGround.y, rotGround.z);
					fprintf(pFile, "		SIZE = %.2f %.2f %.2f\n", sizeGround.x, sizeGround.y, sizeGround.z);
					fprintf(pFile, "	END_GROUNDSET\n\n");

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;
				}
			}
		}

		// 情報終了地点を書き出し
		fprintf(pFile, "END_STAGE_GROUNDSET\n\n");
	}

#endif	// _DEBUG
}

//============================================================
//	大きさの更新処理
//============================================================
void CEditGround::UpdateSizing(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 大きさを変更
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_UP_SCALE_X))
		{
			m_ground.size.x += INIT_SIZE.x;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_SCALE_X))
		{
			m_ground.size.x -= INIT_SIZE.x;
		}
		if (m_pKeyboard->IsPress(KEY_UP_SCALE_Y))
		{
			m_ground.size.y += INIT_SIZE.y;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_SCALE_Y))
		{
			m_ground.size.y -= INIT_SIZE.y;
		}
		if (m_pKeyboard->IsPress(KEY_UP_SCALE_Z))
		{
			m_ground.size.z += INIT_SIZE.z;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_SCALE_Z))
		{
			m_ground.size.z -= INIT_SIZE.z;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_UP_SCALE_X))
		{
			m_ground.size.x += INIT_SIZE.x;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_SCALE_X))
		{
			m_ground.size.x -= INIT_SIZE.x;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP_SCALE_Y))
		{
			m_ground.size.y += INIT_SIZE.y;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_SCALE_Y))
		{
			m_ground.size.y -= INIT_SIZE.y;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP_SCALE_Z))
		{
			m_ground.size.z += INIT_SIZE.z;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_SCALE_Z))
		{
			m_ground.size.z -= INIT_SIZE.z;
		}
	}

	// 大きさを補正
	useful::LimitNum(m_ground.size.x, INIT_SIZE.x, MAX_SIZE);
	useful::LimitNum(m_ground.size.y, INIT_SIZE.y, MAX_SIZE);
	useful::LimitNum(m_ground.size.z, INIT_SIZE.z, MAX_SIZE);

	// 大きさを反映
	m_pGround->SetVec3Sizing(m_ground.size);
}

//============================================================
//	種類変更の更新処理
//============================================================
void CEditGround::UpdateChangeType(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 種類を変更
	if (m_pKeyboard->IsTrigger(KEY_TYPE))
	{
		m_ground.type = (CGround::EType)((m_ground.type + 1) % CGround::TYPE_MAX);
	}

	// 種類を反映
	m_pGround->SetType(m_ground.type);
}

//============================================================
//	テクスチャ分割の更新処理
//============================================================
void CEditGround::UpdateTexPart(void)
{
	// 変数を宣言
	D3DXVECTOR3 partTex = VEC3_ZERO;	// テクスチャ分割数

	// 分割数を設定
	partTex.x = m_ground.size.x / INIT_SIZE.x;
	partTex.y = m_ground.size.y / INIT_SIZE.y;
	partTex.z = m_ground.size.z / INIT_SIZE.z;

	// テクスチャ分割数を設定
	m_ground.partX.x = partTex.z;
	m_ground.partX.y = partTex.y;
	m_ground.partY.x = partTex.x;
	m_ground.partY.y = partTex.z;
	m_ground.partZ.x = partTex.x;
	m_ground.partZ.y = partTex.y;

	// テクスチャ分割数を割当
	m_pGround->SetTexturePatternX(m_ground.partX);
	m_pGround->SetTexturePatternY(m_ground.partY);
	m_pGround->SetTexturePatternZ(m_ground.partZ);
}

//============================================================
//	地盤の生成処理
//============================================================
void CEditGround::CreateGround(void)
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
	D3DXCOLOR colGround = XCOL_WHITE;	// 色保存用

	// 地盤を配置
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	地盤の情報を配置用に変更
		//----------------------------------------------------
		// 自動更新・自動描画をONにする
		m_pGround->SetEnableUpdate(true);
		m_pGround->SetEnableDraw(true);

		// 色を設定
		colGround = m_pGround->GetColor();	// 元の色を取得
		m_pGround->SetColor(D3DXCOLOR(colGround.r, colGround.g, colGround.b, 1.0f));

		// 未保存を設定
		pEdit->UnSave();

		//----------------------------------------------------
		//	新しい地盤の生成
		//----------------------------------------------------
		// 地盤の生成
		m_pGround = CGround::Create(m_ground.type, posEdit, rotEdit, m_ground.size);
		assert(m_pGround != NULL);

		// 色を設定
		colGround = m_pGround->GetColor();	// 元の色を取得
		m_pGround->SetColor(D3DXCOLOR(colGround.r, colGround.g, colGround.b, INIT_ALPHA));
	}
}

//============================================================
//	地盤の破棄処理
//============================================================
void CEditGround::ReleaseGround(void)
{
	// 変数を宣言
	bool bRelease = false;	// 破棄状況

	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 地盤を削除
	if (m_pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// 破棄する状態を設定
		bRelease = true;
	}

	// 地盤の削除判定
	DeleteCollisionGround(bRelease);
}

//============================================================
//	地盤の削除判定
//============================================================
void CEditGround::DeleteCollisionGround(const bool bRelase)
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
				D3DXVECTOR3 posGround = VEC3_ZERO;	// 地盤位置
				D3DXVECTOR3 sizeGround = VEC3_ZERO;	// 地盤大きさ

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_GROUND)
				{ // オブジェクトラベルが地盤ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				if (pObjCheck == (CObject*)m_pGround)
				{ // 同じアドレスだった場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 地盤の位置を取得
				posGround = pObjCheck->GetVec3Position();

				// 地盤の大きさを取得
				sizeGround = pObjCheck->GetVec3Sizing();

				// 球体の当たり判定
				if (collision::Circle3D
				( // 引数
					posEdit,	// 判定位置
					posGround,	// 判定目標位置
					(sizeGround.x + sizeGround.z) * 0.5f,		// 判定半径
					(m_ground.size.x + m_ground.size.z) * 0.5f	// 判定目標半径
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

						// 赤を設定
						pObjCheck->SetColor(XCOL_RED);
					}
				}
				else
				{ // 判定外だった場合

					// 通常色を設定
					pObjCheck->SetColor(XCOL_WHITE);
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	地盤の色の全初期化処理
//============================================================
void CEditGround::InitAllColorGround(void)
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

				if (pObjCheck->GetLabel() != CObject::LABEL_GROUND)
				{ // オブジェクトラベルが地盤ではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				if (pObjCheck == (CObject*)m_pGround)
				{ // 同じアドレスだった場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// 通常色を設定
				pObjCheck->SetColor(XCOL_WHITE);

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}
}
