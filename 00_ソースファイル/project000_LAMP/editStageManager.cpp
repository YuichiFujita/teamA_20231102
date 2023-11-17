//============================================================
//
//	エディットステージマネージャー処理 [editStageManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editStageManager.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "editGround.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_DOUBLE		(DIK_LCONTROL)	// 二重化キー
#define NAME_DOUBLE		("LCTRL")		// 二重化表示
#define KEY_TRIGGER		(DIK_LSHIFT)	// トリガー化キー
#define NAME_TRIGGER	("LSHIFT")		// トリガー化表示

#define KEY_SAVE	(DIK_F9)	// 保存キー
#define NAME_SAVE	("F9")		// 保存表示

#define KEY_CHANGE_THING	(DIK_1)	// 配置物変更キー
#define NAME_CHANGE_THING	("1")	// 配置物変更表示

#define KEY_MOVE_UP		(DIK_UP)	// 移動量上昇キー
#define NAME_MOVE_UP	("↑")		// 移動量上昇表示
#define KEY_MOVE_DOWN	(DIK_DOWN)	// 移動量下降キー
#define NAME_MOVE_DOWN	("↓")		// 移動量下降表示

#define KEY_FAR		(DIK_W)	// 奥移動キー
#define NAME_FAR	("W")	// 奥移動表示
#define KEY_NEAR	(DIK_S)	// 手前移動キー
#define NAME_NEAR	("S")	// 手前移動表示
#define KEY_RIGHT	(DIK_D)	// 右移動キー
#define NAME_RIGHT	("D")	// 右移動表示
#define KEY_LEFT	(DIK_A)	// 左移動キー
#define NAME_LEFT	("A")	// 左移動表示
#define KEY_UP		(DIK_E)	// 上移動キー
#define NAME_UP		("E")	// 上移動表示
#define KEY_DOWN	(DIK_Q)	// 下移動キー
#define NAME_DOWN	("Q")	// 下移動表示

#define KEY_ROTA_RIGHT	(DIK_Z)	// 右回転キー
#define NAME_ROTA_RIGHT	("Z")	// 右回転表示
#define KEY_ROTA_LEFT	(DIK_C)	// 左回転キー
#define NAME_ROTA_LEFT	("C")	// 左回転表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SAVE_TXT	= "data\\TXT\\save_stage.txt";	// ステージセーブテキスト
	const float INIT_MOVE	= 40.0f;	// 配置物の初期移動量
	const float CHANGE_MOVE = 10.0f;	// 配置物の移動量の変動量
	const float MIN_MOVE	= 10.0f;	// 配置物の最小移動量
	const float MAX_MOVE	= 200.0f;	// 配置物の最大移動量
}

//************************************************************
//	親クラス [CEditStageManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditStageManager::CEditStageManager()
{
#if _DEBUG

	// メンバ変数をクリア
	m_pGround	= NULL;			// エディット地盤の情報
	m_pos	= VEC3_ZERO;		// 位置
	m_rot	= VEC3_ZERO;		// 向き
	m_fMove	= 0.0f;				// 位置移動量
	m_thing	= THING_GROUND;		// 配置物
	m_bSave	= false;			// 保存状況
	m_bEdit	= false;			// エディット状況

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditStageManager::~CEditStageManager()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditStageManager::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pGround	= NULL;			// エディット地盤の情報
	m_pos	= VEC3_ZERO;		// 位置
	m_rot	= VEC3_ZERO;		// 向き
	m_fMove	= INIT_MOVE;		// 位置移動量
	m_thing	= THING_GROUND;		// 配置物
	m_bSave	= false;			// 保存状況
	m_bEdit	= false;			// エディット状況

	// エディット地盤の生成
	m_pGround = CEditGround::Create(this);
	if (m_pGround == NULL)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
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
void CEditStageManager::Uninit(void)
{
#if _DEBUG

	if (m_pGround != NULL)
	{ // エディット地盤が使用されている場合

		// エディット地盤の破棄
		CEditGround::Release(m_pGround);
	}
	else { assert(false); }	// 非使用中

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditStageManager::Update(void)
{
#if _DEBUG

	if (!m_bEdit)
	{ // エディットモードではない場合

		// 処理を抜ける
		return;
	}

	// 配置物変更の更新
	UpdateChangeThing();

	// 移動量変更の更新
	UpdateChangeMove();

	// 位置の更新
	UpdatePosition();

	// 向きの更新
	UpdateRotation();

	switch (m_thing)
	{ // 配置物ごとの処理
	case THING_GROUND:	// 地盤

		if (m_pGround != NULL)
		{ // エディット地盤が使用されている場合

			// エディット地盤の更新
			m_pGround->Update();
		}
		else { assert(false); }	// 非使用中

		break;

	case THING_BLOCK:	// ブロック



		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// ステージ保存
	SaveStage();

	// 操作表示の描画
	DrawDebugControl();

	// 情報表示の描画
	DrawDebugInfo();

#endif	// _DEBUG
}

//============================================================
//	未保存の設定処理
//============================================================
void CEditStageManager::UnSave(void)
{
	// 保存していない状況にする
	m_bSave = false;
}

//============================================================
//	エディット状況の設定処理
//============================================================
void CEditStageManager::SetEnableEdit(const bool bEdit)
{
	// 引数のエディット状況にする
	m_bEdit = bEdit;

	switch (m_thing)
	{ // 配置物ごとの処理
	case THING_GROUND:	// 地盤

		if (m_pGround != NULL)
		{ // エディット地盤が使用されている場合

			// エディット地盤の表示の設定
			m_pGround->SetDisp(m_bEdit);
		}
		else { assert(false); }	// 非使用中

		break;

	case THING_BLOCK:	// ブロック



		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	エディット状況取得処理
//============================================================
bool CEditStageManager::IsEdit(void) const
{
	// エディット状況を返す
	return m_bEdit;
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CEditStageManager::GetVec3Position(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CEditStageManager::GetVec3Rotation(void) const
{
	// 向きを返す
	return m_rot;
}

//============================================================
//	生成処理
//============================================================
CEditStageManager *CEditStageManager::Create(void)
{
#if _DEBUG

	// ポインタを宣言
	CEditStageManager *pEditStageManager = NULL;	// エディットステージマネージャー生成用

	if (pEditStageManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pEditStageManager = new CEditStageManager;	// エディットステージマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pEditStageManager != NULL)
	{ // 使用されている場合
		
		// エディットステージマネージャーの初期化
		if (FAILED(pEditStageManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pEditStageManager;
			pEditStageManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pEditStageManager;
	}
	else { assert(false); return NULL; }	// 確保失敗

#else	// NDEBUG

	// NULLを返す
	return NULL;

#endif	// _DEBUG
}

//============================================================
//	破棄処理
//============================================================
HRESULT CEditStageManager::Release(CEditStageManager *&prEditStageManager)
{
#if _DEBUG

	if (prEditStageManager != NULL)
	{ // 使用中の場合

		// エディットステージマネージャーの終了
		prEditStageManager->Uninit();

		// メモリ開放
		delete prEditStageManager;
		prEditStageManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	配置物変更の更新処理
//============================================================
void CEditStageManager::UpdateChangeThing(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 配置物を変更
	if (!m_pKeyboard->IsPress(KEY_DOUBLE))
	{
		if (m_pKeyboard->IsTrigger(KEY_CHANGE_THING))
		{
			switch (m_thing)
			{ // 配置物ごとの処理
			case THING_GROUND:	// 地盤

				if (m_pGround != NULL)
				{ // エディット地盤が使用されている場合

					// エディット地盤の表示の設定
					m_pGround->SetDisp(false);
				}
				else { assert(false); }	// 非使用中

				break;

			case THING_BLOCK:	// ブロック



				break;

			default:	// 例外処理
				assert(false);
				break;
			}

			// 配置物の変更
			m_thing = (EThing)((m_thing + 1) % THING_MAX);

			switch (m_thing)
			{ // 配置物ごとの処理
			case THING_GROUND:	// 地盤

				if (m_pGround != NULL)
				{ // エディット地盤が使用されている場合

					// エディット地盤の表示の設定
					m_pGround->SetDisp(true);
				}
				else { assert(false); }	// 非使用中

				break;

			case THING_BLOCK:	// ブロック



				break;

			default:	// 例外処理
				assert(false);
				break;
			}
		}
	}
}

//============================================================
//	移動量変更の更新処理
//============================================================
void CEditStageManager::UpdateChangeMove(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 移動量を変更
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_MOVE_UP))
		{
			m_fMove += CHANGE_MOVE;
		}
		if (m_pKeyboard->IsPress(KEY_MOVE_DOWN))
		{
			m_fMove -= CHANGE_MOVE;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_MOVE_UP))
		{
			m_fMove += CHANGE_MOVE;
		}
		if (m_pKeyboard->IsTrigger(KEY_MOVE_DOWN))
		{
			m_fMove -= CHANGE_MOVE;
		}
	}

	// 移動量を補正
	useful::LimitNum(m_fMove, MIN_MOVE, MAX_MOVE);
}

//============================================================
//	位置の更新処理
//============================================================
void CEditStageManager::UpdatePosition(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 位置を変更
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_FAR))
		{
			m_pos.z += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_NEAR))
		{
			m_pos.z -= m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_RIGHT))
		{
			m_pos.x += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_LEFT))
		{
			m_pos.x -= m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_UP))
		{
			m_pos.y += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN))
		{
			m_pos.y -= m_fMove;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_FAR))
		{
			m_pos.z += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_NEAR))
		{
			m_pos.z -= m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_RIGHT))
		{
			m_pos.x += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_LEFT))
		{
			m_pos.x -= m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP))
		{
			m_pos.y += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN))
		{
			m_pos.y -= m_fMove;
		}
	}
}

//============================================================
//	向きの更新処理
//============================================================
void CEditStageManager::UpdateRotation(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 向きを変更
	if (m_pKeyboard->IsTrigger(KEY_ROTA_RIGHT))
	{
		m_rot.y += HALF_PI;
	}
	if (m_pKeyboard->IsTrigger(KEY_ROTA_LEFT))
	{
		m_rot.y -= HALF_PI;
	}

	// 向きを正規化
	useful::Vec3NormalizeRot(m_rot);
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditStageManager::DrawDebugControl(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "[エディット操作]　\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "ステージ保存：[%s+%s]\n", NAME_DOUBLE, NAME_SAVE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "移動：[%s/%s/%s/%s/%s/%s+%s]\n", NAME_FAR, NAME_LEFT, NAME_NEAR, NAME_RIGHT, NAME_UP, NAME_DOWN, NAME_TRIGGER);
	pDebug->Print(CDebugProc::POINT_RIGHT, "移動量変更：[%s/%s]\n", NAME_MOVE_UP, NAME_MOVE_DOWN);
	pDebug->Print(CDebugProc::POINT_RIGHT, "回転：[%s/%s]\n", NAME_ROTA_RIGHT, NAME_ROTA_LEFT);
	pDebug->Print(CDebugProc::POINT_RIGHT, "配置物変更：[%s]\n", NAME_CHANGE_THING);

	switch (m_thing)
	{ // 配置物ごとの処理
	case THING_GROUND:	// 地盤

		if (m_pGround != NULL)
		{ // エディット地盤が使用されている場合

			// エディット地盤の操作表示
			m_pGround->DrawDebugControl();
		}
		else { assert(false); }	// 非使用中

		break;

	case THING_BLOCK:	// ブロック



		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditStageManager::DrawDebugInfo(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報
	static char* apThing[] = { "地盤", "ブロック", };	// 配置物

	// 配置物数の不一致
	assert((sizeof(apThing) / sizeof(apThing[0])) == THING_MAX);

	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "[エディット情報]　\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, (m_bSave) ? "保存済：[保存状況]\n" : "未保存：[保存状況]\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "%s：[配置物]\n", apThing[m_thing]);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f %f：[位置]\n", m_pos.x, m_pos.y, m_pos.z);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f %f：[向き]\n", m_rot.x, m_rot.y, m_rot.z);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f：[移動量]\n", m_fMove);

	switch (m_thing)
	{ // 配置物ごとの処理
	case THING_GROUND:	// 地盤

		if (m_pGround != NULL)
		{ // エディット地盤が使用されている場合

			// エディット地盤の情報表示
			m_pGround->DrawDebugInfo();
		}
		else { assert(false); }	// 非使用中

		break;

	case THING_BLOCK:	// ブロック



		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	ステージ保存処理
//============================================================
void CEditStageManager::SaveStage(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// ステージを保存
	if (m_pKeyboard->IsPress(KEY_DOUBLE))
	{
		if (m_pKeyboard->IsTrigger(KEY_SAVE))
		{
			// 保存処理
			Save();

			// 保存した状態にする
			m_bSave = true;
		}
	}
}

//============================================================
//	保存処理
//============================================================
void CEditStageManager::Save(void)
{
	// ポインタを宣言
	FILE *pFile = NULL;	// ファイルポインタ

	// ファイルを書き出し形式で開く
	pFile = fopen(SAVE_TXT, "w");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		// 見出しを書き出し
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#	ステージセーブテキスト [save_stage.txt]\n");
		fprintf(pFile, "#	Author : you\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "---------->--<---------- ここから下を コピーし貼り付け ---------->--<----------\n\n");

		// 地盤の保存
		m_pGround->Save(pFile);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ステ－ジセーブファイルの書き出しに失敗！", "警告！", MB_ICONWARNING);
	}
}
