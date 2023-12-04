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
#include "objectMeshCube.h"
#include "stage.h"
#include "effect3D.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_DOUBLE		(DIK_LCONTROL)	// 二重化キー
#define NAME_DOUBLE		("LCTRL")		// 二重化表示
#define KEY_TRIGGER		(DIK_LSHIFT)	// トリガー化キー
#define NAME_TRIGGER	("LSHIFT")		// トリガー化表示
#define KEY_REVERSE		(DIK_LCONTROL)	// 操作逆転化キー
#define NAME_REVERSE	("LCTRL")		// 操作逆転化表示

#define KEY_SAVE	(DIK_F8)	// 保存キー
#define NAME_SAVE	("F8")		// 保存表示

#define KEY_CREATE		(DIK_0)	// 生成キー
#define NAME_CREATE		("0")	// 生成表示
#define KEY_RELEASE		(DIK_9)	// 破棄キー
#define NAME_RELEASE	("9")	// 破棄表示
#define KEY_TYPE		(DIK_2)	// 種類変更キー
#define NAME_TYPE		("2")	// 種類変更表示
#define KEY_BREAK		(DIK_3)	// 破壊変更キー
#define NAME_BREAK		("3")	// 破壊変更表示
#define KEY_LIFE		(DIK_4)	// 体力変更キー
#define NAME_LIFE		("4")	// 体力変更表示

#define KEY_MOVE_RIGHT	(DIK_RIGHT)	// 右移動キー
#define NAME_MOVE_RIGHT	("→")		// 右移動表示
#define KEY_MOVE_LEFT	(DIK_LEFT)	// 左移動キー
#define NAME_MOVE_LEFT	("←")		// 左移動表示
#define KEY_MOVE_FAR	(DIK_UP)	// 奥移動キー
#define NAME_MOVE_FAR	("↑")		// 奥移動表示
#define KEY_MOVE_NEAR	(DIK_DOWN)	// 手前移動キー
#define NAME_MOVE_NEAR	("↓")		// 手前移動表示

#define KEY_UP_SCALE_X		(DIK_T)	// X軸拡大キー
#define NAME_UP_SCALE_X		("T")	// X軸拡大表示
#define KEY_DOWN_SCALE_X	(DIK_G)	// X軸縮小キー
#define NAME_DOWN_SCALE_X	("G")	// X軸縮小表示
#define KEY_UP_SCALE_Z		(DIK_U)	// Z軸拡大キー
#define NAME_UP_SCALE_Z		("U")	// Z軸拡大表示
#define KEY_DOWN_SCALE_Z	(DIK_J)	// Z軸縮小キー
#define NAME_DOWN_SCALE_Z	("J")	// Z軸縮小表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float	INIT_ALPHA		= 0.5f;		// 配置前のα値
	const float	MOVE_POS		= 10.0f;	// 位置移動量
	const float	SIZE_COLLY		= 300.0f;	// 判定の大きさY
	const float	MIN_SIZE		= 10.0f;	// 最小の大きさ
	const float	MAX_SIZE		= 10000.0f;	// 最大の大きさ
	const float	MOVE_SIZE		= 10.0f;	// 大きさ変動量
	const int	MIN_LIFE		= 1;		// 最小の体力
	const int	MAX_LIFE		= 6;		// 最大の体力
	const D3DXCOLOR	COLL_COL	= D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.4f);	// 当たり判定色
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
	D3DXVECTOR3 posEdit		= pEdit->GetVec3Position();	// エディットの位置
	D3DXVECTOR3 rotEdit		= pEdit->GetVec3Rotation();	// エディットの向き
	D3DXVECTOR3 posColl		= VEC3_ZERO;				// 判定位置
	D3DXVECTOR3 sizeColl	= CObstacle::GetStatusInfo(m_obstacle.type).sizeColl;		// 判定大きさ
	float fLengthColl		= CObstacle::GetStatusInfo(m_obstacle.type).fLengthCenter;	// 判定中心位置の距離
	float fAngleColl		= CObstacle::GetStatusInfo(m_obstacle.type).fAngleCenter;	// 判定中心位置の角度

	// 判定位置を求める
	posColl = posEdit;
	posColl.x += sinf(rotEdit.y + fAngleColl) * fLengthColl;
	posColl.y = 0.0f;
	posColl.z += cosf(rotEdit.y + fAngleColl) * fLengthColl;

	// メンバ変数を初期化
	m_pObstacle		= NULL;						// 障害物情報
	m_pVisual		= NULL;						// 当たり判定視認キューブ
	m_obstacle.type	= CObstacle::TYPE_CONIFER;	// 障害物種類
	m_obstacle.posCollCenter = posColl;			// 判定の中心位置

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

	// 判定視認キューブの生成
	m_pVisual = CObjectMeshCube::Create
	( // 引数
		posColl,	// 位置
		rotEdit,	// 向き
		sizeColl,	// 大きさ
		COLL_COL	// キューブ色
	);

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
	{ // 使用中の場合

		// 障害物の色の全初期化
		InitAllColorObstacle();

		// 障害物の終了
		m_pObstacle->Uninit();
	}

	if (m_pVisual != NULL)
	{ // 使用中の場合

		// 判定視認キューブの終了
		m_pVisual->Uninit();
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

	// 変数を宣言
	D3DXVECTOR3 posEdit = pEdit->GetVec3Position();	// エディットの位置
	D3DXVECTOR3 rotEdit = pEdit->GetVec3Rotation();	// エディットの向き

	// 種類変更の更新
	UpdateChangeType();

	// 判定位置の更新
	UpdateCollPosition();

	// 判定大きさの更新
	UpdateCollSizing();

	// 破壊変更の更新
	UpdateChangeBreak();

	// 体力変更の更新
	UpdateChangeLife();

	// 障害物の生成
	CreateObstacle();

	// 障害物の破棄
	ReleaseObstacle();

	// ステータス情報の保存
	SaveStatusInfo();

	// 位置を反映
	m_pObstacle->SetVec3Position(posEdit);
	m_pVisual->SetVec3Position(m_obstacle.posCollCenter);

	// 向きを反映
	m_pObstacle->SetVec3Rotation(rotEdit);
	m_pVisual->SetVec3Rotation(rotEdit);

	// 判定視認キューブの更新
	m_pVisual->Update();

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditObstacle::DrawDebugControl(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグプロックの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "障害物ステータス保存：[%s+%s]\n", NAME_DOUBLE, NAME_SAVE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "判定位置：[%s/%s/%s/%s]\n", NAME_MOVE_RIGHT, NAME_MOVE_LEFT, NAME_MOVE_FAR, NAME_MOVE_NEAR);
	pDebug->Print(CDebugProc::POINT_RIGHT, "判定大きさ：[%s/%s/%s/%s+%s]\n", NAME_UP_SCALE_X, NAME_DOWN_SCALE_X, NAME_UP_SCALE_Z, NAME_DOWN_SCALE_Z, NAME_TRIGGER);
	pDebug->Print(CDebugProc::POINT_RIGHT, "種類変更：[%s+%s]\n", NAME_TYPE, NAME_REVERSE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "破壊変更：[%s]\n", NAME_BREAK);
	pDebug->Print(CDebugProc::POINT_RIGHT, "体力変更：[%s]\n", NAME_LIFE);
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
	static char* apBreak[] = { "不可能", "可能" };	// 破壊状況

	// 破壊状況数の不一致
	assert((sizeof(apBreak) / sizeof(apBreak[0])) == CObstacle::BREAK_MAX);

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d：[種類]\n", m_obstacle.type);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f %f：[判定位置]\n", m_obstacle.posCollCenter.x, m_obstacle.posCollCenter.y, m_obstacle.posCollCenter.z);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f %f：[判定大きさ]\n", m_pVisual->GetVec3Sizing().x, m_pVisual->GetVec3Sizing().y, m_pVisual->GetVec3Sizing().z);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%s：[破壊]\n", apBreak[CObstacle::GetStatusInfo(m_obstacle.type).state]);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%d：[体力]\n", CObstacle::GetStatusInfo(m_obstacle.type).nLife);
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

	// 種類を変更
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
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
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_TYPE))
		{
			// 種類を変更
			m_obstacle.type = (CObstacle::EType)((m_obstacle.type + (CObstacle::TYPE_MAX - 1)) % CObstacle::TYPE_MAX);

			// 種類を反映
			m_pObstacle->SetType(m_obstacle.type);

			// 透明度を設定
			m_pObstacle->SetAlpha(INIT_ALPHA);
		}
	}
}

//============================================================
//	判定位置の更新処理
//============================================================
void CEditObstacle::UpdateCollPosition(void)
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
	CObstacle::SStatusInfo status = CObstacle::GetStatusInfo(m_obstacle.type);	// ステータス情報
	D3DXVECTOR3 posEdit	= pEdit->GetVec3Position();	// エディットの位置
	D3DXVECTOR3 rotEdit	= pEdit->GetVec3Rotation();	// エディットの向き
	float fLengthColl	= status.fLengthCenter;		// 判定中心位置の距離
	float fAngleColl	= status.fAngleCenter;		// 判定中心位置の角度

	// 判定位置を求める
	m_obstacle.posCollCenter = posEdit;
	m_obstacle.posCollCenter.x += sinf(rotEdit.y + fAngleColl) * fLengthColl;
	m_obstacle.posCollCenter.y = 0.0f;
	m_obstacle.posCollCenter.z += cosf(rotEdit.y + fAngleColl) * fLengthColl;

	// 位置を変更
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_MOVE_RIGHT))
		{
			m_obstacle.posCollCenter.x += MOVE_POS;
		}
		if (m_pKeyboard->IsPress(KEY_MOVE_LEFT))
		{
			m_obstacle.posCollCenter.x -= MOVE_POS;
		}
		if (m_pKeyboard->IsPress(KEY_MOVE_FAR))
		{
			m_obstacle.posCollCenter.z += MOVE_POS;
		}
		if (m_pKeyboard->IsPress(KEY_MOVE_NEAR))
		{
			m_obstacle.posCollCenter.z -= MOVE_POS;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_MOVE_RIGHT))
		{
			m_obstacle.posCollCenter.x += MOVE_POS;
		}
		if (m_pKeyboard->IsTrigger(KEY_MOVE_LEFT))
		{
			m_obstacle.posCollCenter.x -= MOVE_POS;
		}
		if (m_pKeyboard->IsTrigger(KEY_MOVE_FAR))
		{
			m_obstacle.posCollCenter.z += MOVE_POS;
		}
		if (m_pKeyboard->IsTrigger(KEY_MOVE_NEAR))
		{
			m_obstacle.posCollCenter.z -= MOVE_POS;
		}
	}

	// 判定中心位置の距離を設定
	status.fLengthCenter = sqrtf((m_obstacle.posCollCenter.x - posEdit.x) * (m_obstacle.posCollCenter.x - posEdit.x)
						 + (m_obstacle.posCollCenter.z - posEdit.z) * (m_obstacle.posCollCenter.z - posEdit.z));

	// 判定中心位置の角度を設定
	status.fAngleCenter = atan2f((m_obstacle.posCollCenter.x - posEdit.x), (m_obstacle.posCollCenter.z - posEdit.z)) - rotEdit.y;
	
	// ステータス情報を反映
	CObstacle::SetStatusInfo(m_obstacle.type, status);
}

//============================================================
//	判定大きさの更新処理
//============================================================
void CEditObstacle::UpdateCollSizing(void)
{
	// 変数を宣言
	CObstacle::SStatusInfo status = CObstacle::GetStatusInfo(m_obstacle.type);	// ステータス情報

	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 大きさを変更
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_UP_SCALE_X))
		{
			status.sizeColl.x += MOVE_SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_SCALE_X))
		{
			status.sizeColl.x -= MOVE_SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_UP_SCALE_Z))
		{
			status.sizeColl.z += MOVE_SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_SCALE_Z))
		{
			status.sizeColl.z -= MOVE_SIZE;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_UP_SCALE_X))
		{
			status.sizeColl.x += MOVE_SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_SCALE_X))
		{
			status.sizeColl.x -= MOVE_SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP_SCALE_Z))
		{
			status.sizeColl.z += MOVE_SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_SCALE_Z))
		{
			status.sizeColl.z -= MOVE_SIZE;
		}
	}

	// 大きさを補正
	status.sizeColl.y = SIZE_COLLY;	// 縦の大きさは固定
	useful::LimitNum(status.sizeColl.x, MIN_SIZE, MAX_SIZE);
	useful::LimitNum(status.sizeColl.z, MIN_SIZE, MAX_SIZE);

	// ステータス情報を反映
	CObstacle::SetStatusInfo(m_obstacle.type, status);

	// 大きさを反映
	m_pVisual->SetVec3Sizing(status.sizeColl);
}

//============================================================
//	破壊変更の更新処理
//============================================================
void CEditObstacle::UpdateChangeBreak(void)
{
	// 変数を宣言
	CObstacle::SStatusInfo status = CObstacle::GetStatusInfo(m_obstacle.type);	// ステータス情報

	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 破壊状況を変更
	if (m_pKeyboard->IsTrigger(KEY_BREAK))
	{
		// 破壊状況を変更
		status.state = (CObstacle::EBreak)((status.state + 1) % CObstacle::BREAK_MAX);
	}

	// ステータス情報を反映
	CObstacle::SetStatusInfo(m_obstacle.type, status);
}

//============================================================
//	体力変更の更新処理
//============================================================
void CEditObstacle::UpdateChangeLife(void)
{
	// 変数を宣言
	CObstacle::SStatusInfo status = CObstacle::GetStatusInfo(m_obstacle.type);	// ステータス情報

	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 体力を変更
	if (!m_pKeyboard->IsPress(KEY_REVERSE))
	{
		if (m_pKeyboard->IsTrigger(KEY_LIFE))
		{
			// 体力を加算
			status.nLife++;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_LIFE))
		{
			// 体力を減算
			status.nLife--;
		}
	}

	// 体力を補正
	useful::LimitNum(status.nLife, MIN_LIFE, MAX_LIFE);

	// ステータス情報を反映
	CObstacle::SetStatusInfo(m_obstacle.type, status);
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
//	ステータス情報保存処理
//============================================================
void CEditObstacle::SaveStatusInfo(void)
{
	// ポインタを宣言
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報

	// 障害物を保存
	if (m_pKeyboard->IsPress(KEY_DOUBLE))
	{
		if (m_pKeyboard->IsTrigger(KEY_SAVE))
		{
			// 障害物のステータス保存処理
			CObstacle::SaveStatus();
		}
	}
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
