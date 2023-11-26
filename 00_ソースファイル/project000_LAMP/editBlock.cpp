//============================================================
//
//	エディットブロック処理 [editBlock.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editBlock.h"
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
CEditBlock::SInfo CEditBlock::m_save = {};	// 保存情報

//************************************************************
//	親クラス [CEditBlock] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditBlock::CEditBlock()
{
#if _DEBUG

	// メンバ変数をクリア
	m_pBlock = NULL;	// ブロック情報
	memset(&m_block, 0, sizeof(m_block));	// ブロック配置情報

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditBlock::~CEditBlock()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditBlock::Init(void)
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
	m_pBlock		= NULL;					// ブロック情報
	m_block.type	= CBlock::TYPE_STONE;	// ブロック種類
	m_block.size	= INIT_SIZE;			// 大きさ
	m_block.partX	= VEC2_ONE;				// テクスチャ分割数X
	m_block.partY	= VEC2_ONE;				// テクスチャ分割数Y
	m_block.partZ	= VEC2_ONE;				// テクスチャ分割数Z

	// ブロックの生成
	m_pBlock = CBlock::Create
	( // 引数
		m_block.type,	// 種類
		posEdit,		// 位置
		rotEdit,		// 向き
		m_block.size,	// 大きさ
		m_block.partX,	// テクスチャ分割数X
		m_block.partY,	// テクスチャ分割数Y
		m_block.partZ	// テクスチャ分割数Z
	);
	if (m_pBlock == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 色を設定
	D3DXCOLOR col = m_pBlock->GetColor();	// 元の色を取得
	m_pBlock->SetColor(D3DXCOLOR(col.r, col.g, col.b, INIT_ALPHA));

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
void CEditBlock::Uninit(void)
{
#if _DEBUG

	if (m_pBlock != NULL)
	{ // 生成に失敗した場合

		// ブロックの色の全初期化
		InitAllColorBlock();

		// ブロックの終了
		m_pBlock->Uninit();
	}

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditBlock::Update(void)
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

	// テクスチャ分割の更新
	UpdateTexPart();

	// 種類変更の更新
	UpdateChangeType();

	// ブロックの生成
	CreateBlock();

	// ブロックの破棄
	ReleaseBlock();

	// 位置を反映
	m_pBlock->SetVec3Position(pEdit->GetVec3Position());

	// 向きを反映
	m_pBlock->SetVec3Rotation(pEdit->GetVec3Rotation());

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditBlock::DrawDebugControl(void)
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
void CEditBlock::DrawDebugInfo(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d：[種類]\n", m_block.type);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f %f：[大きさ]\n", m_block.size.x, m_block.size.y, m_block.size.z);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f：[テクスチャ分割X]\n", m_block.partX.x, m_block.partX.y);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f：[テクスチャ分割Y]\n", m_block.partY.x, m_block.partY.y);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f：[テクスチャ分割Z]\n", m_block.partZ.x, m_block.partZ.y);
}

//============================================================
//	情報保存処理
//============================================================
void CEditBlock::SaveInfo(void)
{
	// 現在の情報を保存
	m_save = m_block;
}

//============================================================
//	情報読込処理
//============================================================
void CEditBlock::LoadInfo(void)
{
	// 保存情報を設定
	m_block = m_save;
}

//============================================================
//	保存処理
//============================================================
void CEditBlock::Save(FILE *pFile)
{
#if _DEBUG

	if (pFile != NULL)
	{ // ファイルが存在する場合

		// 見出しを書き出し
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#	ブロックの配置情報\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		// 情報開始地点を書き出し
		fprintf(pFile, "STAGE_BLOCKSET\n\n");

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
	
					if (pObjCheck->GetLabel() != CObject::LABEL_BLOCK)
					{ // オブジェクトラベルがブロックではない場合
	
						// 次のオブジェクトへのポインタを代入
						pObjCheck = pObjectNext;
	
						// 次の繰り返しに移行
						continue;
					}
	
					if (pObjCheck == (CObject*)m_pBlock)
					{ // 同じアドレスだった場合
	
						// 次のオブジェクトへのポインタを代入
						pObjCheck = pObjectNext;
	
						// 次の繰り返しに移行
						continue;
					}

					// ブロックの情報を取得
					D3DXVECTOR3 posBlock		= pObjCheck->GetVec3Position();		// 位置
					D3DXVECTOR3 rotBlock		= pObjCheck->GetVec3Rotation();		// 向き
					D3DXVECTOR3 sizeBlock		= pObjCheck->GetVec3Sizing();		// 大きさ
					D3DXVECTOR2 partTexXBlock	= pObjCheck->GetTexturePatternX();	// テクスチャ分割X
					D3DXVECTOR2 partTexYBlock	= pObjCheck->GetTexturePatternY();	// テクスチャ分割Y
					D3DXVECTOR2 partTexZBlock	= pObjCheck->GetTexturePatternZ();	// テクスチャ分割Z
					int nType = pObjCheck->GetType();	// 種類
	
					// 情報を書き出し
					fprintf(pFile, "	BLOCKSET\n");
					fprintf(pFile, "		TYPE = %d\n", nType);
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posBlock.x, posBlock.y, posBlock.z);
					fprintf(pFile, "		ROT = %.2f %.2f %.2f\n", rotBlock.x, rotBlock.y, rotBlock.z);
					fprintf(pFile, "		SIZE = %.2f %.2f %.2f\n", sizeBlock.x, sizeBlock.y, sizeBlock.z);
					fprintf(pFile, "		PARTX = %.2f %.2f \n", partTexXBlock.x, partTexXBlock.y);
					fprintf(pFile, "		PARTY = %.2f %.2f \n", partTexYBlock.x, partTexYBlock.y);
					fprintf(pFile, "		PARTZ = %.2f %.2f \n", partTexZBlock.x, partTexZBlock.y);
					fprintf(pFile, "	END_BLOCKSET\n\n");

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;
				}
			}
		}

		// 情報終了地点を書き出し
		fprintf(pFile, "END_STAGE_BLOCKSET\n\n");
	}

#endif	// _DEBUG
}

//============================================================
//	大きさの更新処理
//============================================================
void CEditBlock::UpdateSizing(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 大きさを変更
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_UP_SCALE_X))
		{
			m_block.size.x += INIT_SIZE.x;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_SCALE_X))
		{
			m_block.size.x -= INIT_SIZE.x;
		}
		if (m_pKeyboard->IsPress(KEY_UP_SCALE_Y))
		{
			m_block.size.y += INIT_SIZE.y;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_SCALE_Y))
		{
			m_block.size.y -= INIT_SIZE.y;
		}
		if (m_pKeyboard->IsPress(KEY_UP_SCALE_Z))
		{
			m_block.size.z += INIT_SIZE.z;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_SCALE_Z))
		{
			m_block.size.z -= INIT_SIZE.z;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_UP_SCALE_X))
		{
			m_block.size.x += INIT_SIZE.x;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_SCALE_X))
		{
			m_block.size.x -= INIT_SIZE.x;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP_SCALE_Y))
		{
			m_block.size.y += INIT_SIZE.y;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_SCALE_Y))
		{
			m_block.size.y -= INIT_SIZE.y;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP_SCALE_Z))
		{
			m_block.size.z += INIT_SIZE.z;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_SCALE_Z))
		{
			m_block.size.z -= INIT_SIZE.z;
		}
	}

	// 大きさを補正
	useful::LimitNum(m_block.size.x, INIT_SIZE.x, MAX_SIZE);
	useful::LimitNum(m_block.size.y, INIT_SIZE.y, MAX_SIZE);
	useful::LimitNum(m_block.size.z, INIT_SIZE.z, MAX_SIZE);

	// 大きさを反映
	m_pBlock->SetVec3Sizing(m_block.size);
}

//============================================================
//	テクスチャ分割の更新処理
//============================================================
void CEditBlock::UpdateTexPart(void)
{
	// 変数を宣言
	D3DXVECTOR3 partTex = VEC3_ZERO;	// テクスチャ分割数

	// 分割数を設定
	partTex.x = m_block.size.x / INIT_SIZE.x;
	partTex.y = m_block.size.y / INIT_SIZE.y;
	partTex.z = m_block.size.z / INIT_SIZE.z;

	// テクスチャ分割数を設定
	m_block.partX.x = partTex.z;
	m_block.partX.y = partTex.y;
	m_block.partY.x = partTex.x;
	m_block.partY.y = partTex.z;
	m_block.partZ.x = partTex.x;
	m_block.partZ.y = partTex.y;

	// テクスチャ分割数を割当
	m_pBlock->SetTexturePatternX(m_block.partX);
	m_pBlock->SetTexturePatternY(m_block.partY);
	m_pBlock->SetTexturePatternZ(m_block.partZ);
}

//============================================================
//	種類変更の更新処理
//============================================================
void CEditBlock::UpdateChangeType(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 種類を変更
	if (m_pKeyboard->IsTrigger(KEY_TYPE))
	{
		m_block.type = (CBlock::EType)((m_block.type + 1) % CBlock::TYPE_MAX);
	}

	// 種類を反映
	m_pBlock->SetType(m_block.type);
}

//============================================================
//	ブロックの生成処理
//============================================================
void CEditBlock::CreateBlock(void)
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
	D3DXCOLOR colBlock = XCOL_WHITE;	// 色保存用

	// ブロックを配置
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	ブロックの情報を配置用に変更
		//----------------------------------------------------
		// 自動更新・自動描画をONにする
		m_pBlock->SetEnableUpdate(true);
		m_pBlock->SetEnableDraw(true);

		// 色を設定
		colBlock = m_pBlock->GetColor();	// 元の色を取得
		m_pBlock->SetColor(D3DXCOLOR(colBlock.r, colBlock.g, colBlock.b, 1.0f));

		// 未保存を設定
		pEdit->UnSave();

		//----------------------------------------------------
		//	新しいブロックの生成
		//----------------------------------------------------
		// ブロックの生成
		m_pBlock = m_pBlock = CBlock::Create
		( // 引数
			m_block.type,	// 種類
			posEdit,		// 位置
			rotEdit,		// 向き
			m_block.size,	// 大きさ
			m_block.partX,	// テクスチャ分割数X
			m_block.partY,	// テクスチャ分割数Y
			m_block.partZ	// テクスチャ分割数Z
		);
		assert(m_pBlock != NULL);

		// 色を設定
		colBlock = m_pBlock->GetColor();	// 元の色を取得
		m_pBlock->SetColor(D3DXCOLOR(colBlock.r, colBlock.g, colBlock.b, INIT_ALPHA));
	}
}

//============================================================
//	ブロックの破棄処理
//============================================================
void CEditBlock::ReleaseBlock(void)
{
	// 変数を宣言
	bool bRelease = false;	// 破棄状況

	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// ブロックを削除
	if (m_pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// 破棄する状態を設定
		bRelease = true;
	}

	// ブロックの削除判定
	DeleteCollisionBlock(bRelease);
}

//============================================================
//	ブロックの削除判定
//============================================================
void CEditBlock::DeleteCollisionBlock(const bool bRelase)
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
				D3DXVECTOR3 posBlock = VEC3_ZERO;	// ブロック位置
				D3DXVECTOR3 sizeBlock = VEC3_ZERO;	// ブロック大きさ

				// ポインタを宣言
				CObject *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (pObjCheck->GetLabel() != CObject::LABEL_BLOCK)
				{ // オブジェクトラベルがブロックではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				if (pObjCheck == (CObject*)m_pBlock)
				{ // 同じアドレスだった場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				// ブロックの位置を取得
				posBlock = pObjCheck->GetVec3Position();

				// ブロックの大きさを取得
				sizeBlock = pObjCheck->GetVec3Sizing();

				// 球体の当たり判定
				if (collision::Circle3D
				( // 引数
					posEdit,	// 判定位置
					posBlock,	// 判定目標位置
					(sizeBlock.x + sizeBlock.z) * 0.5f,			// 判定半径
					(m_block.size.x + m_block.size.z) * 0.5f	// 判定目標半径
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
//	ブロックの色の全初期化処理
//============================================================
void CEditBlock::InitAllColorBlock(void)
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

				if (pObjCheck->GetLabel() != CObject::LABEL_BLOCK)
				{ // オブジェクトラベルがブロックではない場合

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;

					// 次の繰り返しに移行
					continue;
				}

				if (pObjCheck == (CObject*)m_pBlock)
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
