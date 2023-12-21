//============================================================
//
//	ステージ処理 [stage.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "stage.h"
#include "manager.h"
#include "texture.h"
#include "collision.h"

#include "field.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "liquid.h"

#include "ground.h"
#include "block.h"
#include "obstacle.h"
#include "spawnPoint.h"
#include "ItemSpawnPoint.h"
#include "nextPoint.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT[] =	// ステージセットアップテキスト
	{
#if 0
		//"data\\TXT\\Stages\\stage0.txt",
		//"data\\TXT\\Stages\\Ground_Stage.txt",
#else
		"data\\TXT\\Stages\\stage1.txt",
		"data\\TXT\\Stages\\stage2.txt",
		"data\\TXT\\Stages\\stage3.txt",
		"data\\TXT\\Stages\\stage4.txt",
		"data\\TXT\\Stages\\stage5.txt",
		"data\\TXT\\Stages\\stage7.txt",
		"data\\TXT\\Stages\\stage8.txt",
		"data\\TXT\\Stages\\stage13.txt",
		"data\\TXT\\Stages\\stage15.txt",
		"data\\TXT\\Stages\\stage17.txt",
		"data\\TXT\\Stages\\stage18.txt",
		"data\\TXT\\Stages\\stage21.txt",
		"data\\TXT\\Stages\\stage22.txt",
		"data\\TXT\\Stages\\stage23.txt",
		"data\\TXT\\Stages\\stage24.txt",
		"data\\TXT\\Stages\\stage25.txt",
		"data\\TXT\\Stages\\stage26.txt",
		"data\\TXT\\Stages\\stage27.txt",
		"data\\TXT\\Stages\\stage28.txt",
		"data\\TXT\\Stages\\stage29.txt",
		"data\\TXT\\Stages\\stage30.txt",
		"data\\TXT\\Stages\\stage31.txt",
		"data\\TXT\\Stage_morikawa\\stage001.txt",
		"data\\TXT\\Stage_morikawa\\stage002.txt",
#endif
	};
}

//************************************************************
//	親クラス [CStage] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CStage::CStage()
{
	// メンバ変数をクリア
	memset(&m_stageLimit,	0, sizeof(m_stageLimit));	// 範囲
	memset(&m_field,		0, sizeof(m_field));		// 地面情報
	memset(&m_wall,			0, sizeof(m_wall));			// 壁情報
	memset(&m_scenery,		0, sizeof(m_scenery));		// 景色情報
	memset(&m_sky,			0, sizeof(m_sky));			// 空情報
	memset(&m_liquid,		0, sizeof(m_liquid));		// 液体情報
}

//============================================================
//	デストラクタ
//============================================================
CStage::~CStage()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CStage::Init(void)
{
	// メンバ変数を初期化
	memset(&m_stageLimit, 0, sizeof(m_stageLimit));	// 範囲

	// 地面の情報を初期化
	m_field.ppField = NULL;	// 地面の情報
	m_field.nNum = 0;		// 地面の総数

	// 壁の情報を初期化
	m_wall.ppWall = NULL;	// 壁の情報
	m_wall.nNum = 0;		// 壁の総数

	// 景色の情報を初期化
	m_scenery.ppScenery = NULL;	// 景色の情報
	m_scenery.nNum = 0;			// 景色の総数

	// 空の情報を初期化
	m_sky.ppSky = NULL;	// 空の情報
	m_sky.nNum = 0;		// 空の総数

	// 液体の情報を初期化
	m_liquid.ppLiquid = NULL;	// 液体の情報
	m_liquid.nNum = 0;			// 液体の総数

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CStage::Uninit(void)
{
	// 地面の破棄
	if (m_field.ppField != NULL)
	{ // 地面が使用されている場合

		// メモリ開放
		delete[] m_field.ppField;
		m_field.ppField = NULL;
	}

	// 壁の破棄
	if (m_wall.ppWall != NULL)
	{ // 壁が使用されている場合

		// メモリ開放
		delete[] m_wall.ppWall;
		m_wall.ppWall = NULL;
	}

	// 景色の破棄
	if (m_scenery.ppScenery != NULL)
	{ // 景色が使用されている場合

		// メモリ開放
		delete[] m_scenery.ppScenery;
		m_scenery.ppScenery = NULL;
	}

	// 空の破棄
	if (m_sky.ppSky != NULL)
	{ // 空が使用されている場合

		// メモリ開放
		delete[] m_sky.ppSky;
		m_sky.ppSky = NULL;
	}

	// 液体の破棄
	if (m_liquid.ppLiquid != NULL)
	{ // 液体が使用されている場合

		// メモリ開放
		delete[] m_liquid.ppLiquid;
		m_liquid.ppLiquid = NULL;
	}
}

//============================================================
//	更新処理
//============================================================
void CStage::Update(void)
{

}

//============================================================
//	ステージ範囲の設定処理
//============================================================
void CStage::SetStageLimit(const SStageLimit& rLimit)
{
	// 引数のステージ範囲を設定
	m_stageLimit = rLimit;
}

//============================================================
//	ステージ範囲取得処理
//============================================================
CStage::SStageLimit CStage::GetStageLimit(void) const
{
	// ステージ範囲を返す
	return m_stageLimit;
}

//============================================================
//	位置の補正処理
//============================================================
void CStage::LimitPosition(D3DXVECTOR3& rPos, const float fRadius)
{
	switch (m_stageLimit.mode)
	{ // 制限モードごとの処理
	case LIMIT_BOX:		// 矩形範囲

		// 角柱の内側制限
		collision::InBoxPillar
		( // 引数
			rPos,				// 判定位置
			VEC3_ZERO,			// 判定原点位置
			VEC3_ALL(fRadius),	// 判定サイズ(右・上・後)
			VEC3_ALL(fRadius),	// 判定サイズ(左・下・前)
			D3DXVECTOR3(fabsf(m_stageLimit.fRight), 0.0f, fabsf(m_stageLimit.fFar)),	// 判定原点サイズ(右・上・後)
			D3DXVECTOR3(fabsf(m_stageLimit.fLeft),  0.0f, fabsf(m_stageLimit.fNear))	// 判定原点サイズ(左・下・前)
		);

		break;

	case LIMIT_CIRCLE:	// 円範囲

		// 円柱の内側制限
		collision::InCirclePillar
		( // 引数
			rPos,					// 判定位置
			VEC3_ZERO,				// 判定原点位置
			fRadius,				// 判定半径
			m_stageLimit.fRadius	// 判定原点半径
		);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	キルY座標との当たり判定
//============================================================
bool CStage::CollisionKillY(const D3DXVECTOR3& rPos)
{
	if (rPos.y  < m_stageLimit.fField)
	{ // 位置が地面の制限位置より下の場合

		// 死亡している状況を返す
		return true;
	}

	// 死亡していない状況を返す
	return false;
}

//============================================================
//	地面着地の着地処理
//============================================================
bool CStage::LandFieldPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove)
{
	// 変数を宣言
	CField *pCurrentField = NULL;	// 着地予定の地面
	float fCurrentPos = m_stageLimit.fField;	// 着地予定のY座標

	for (int nCntField = 0; nCntField < m_field.nNum; nCntField++)
	{ // 地面の総数分繰り返す

		if (m_field.ppField[nCntField] != NULL)
		{ // 地面が使用されている場合

			if (m_field.ppField[nCntField]->IsPositionRange(rPos))
			{ // 地面の範囲内の場合

				// 変数を宣言
				float fPosHeight = m_field.ppField[nCntField]->GetPositionHeight(rPos);	// 着地Y座標

				if (fCurrentPos <= fPosHeight)
				{ // 現在の着地予定Y座標より高い位置にある場合

					// 着地予定の地面を更新
					pCurrentField = m_field.ppField[nCntField];

					// 着地予定のY座標を更新
					fCurrentPos = fPosHeight;
				}
			}
		}
		else { assert(false); }	// 非使用中
	}

	if (pCurrentField != NULL)
	{ // 着地予定の地面が存在する場合

		// メッシュの着地
		return pCurrentField->LandPosition(rPos, rMove);
	}
	else
	{ // 着地予定の地面が存在しない場合

		// 着地していない状態を返す
		return false;
	}
}

//============================================================
//	地面の範囲内の取得処理 (回転考慮)
//============================================================
bool CStage::IsFieldPositionRange(const D3DXVECTOR3&rPos)
{
	for (int nCntField = 0; nCntField < m_field.nNum; nCntField++)
	{ // 地面の総数分繰り返す

		if (m_field.ppField[nCntField] != NULL)
		{ // 地面が使用されている場合

			if (m_field.ppField[nCntField]->IsPositionRange(rPos))
			{ // 地面の範囲内の場合

				// 範囲内にいる状態を返す
				return true;
			}
		}
		else { assert(false); }	// 非使用中
	}

	// 範囲内にいない状態を返す
	return false;
}

//============================================================
//	地面の着地位置の取得処理 (回転考慮)
//============================================================
float CStage::GetFieldPositionHeight(const D3DXVECTOR3&rPos)
{
	// 変数を宣言
	CField *pCurrentField = NULL;	// 着地予定の地面
	float fCurrentPos = m_stageLimit.fField;	// 着地予定のY座標

	for (int nCntField = 0; nCntField < m_field.nNum; nCntField++)
	{ // 地面の総数分繰り返す

		if (m_field.ppField[nCntField] != NULL)
		{ // 地面が使用されている場合

			if (m_field.ppField[nCntField]->IsPositionRange(rPos))
			{ // 地面の範囲内の場合

				// 変数を宣言
				float fPosHeight = m_field.ppField[nCntField]->GetPositionHeight(rPos);	// 着地Y座標

				if (fCurrentPos <= fPosHeight)
				{ // 現在の着地予定Y座標より高い位置にある場合

					// 着地予定の地面を更新
					pCurrentField = m_field.ppField[nCntField];

					// 着地予定のY座標を更新
					fCurrentPos = fPosHeight;
				}
			}
		}
		else { assert(false); }	// 非使用中
	}

	if (pCurrentField != NULL)
	{ // 着地予定の地面が存在する場合

		// 着地位置を返す
		return fCurrentPos;
	}
	else
	{ // 着地予定の地面が存在しない場合

		// 引数位置を返す
		return rPos.y;
	}
}

//============================================================
//	生成処理
//============================================================
CLiquid *CStage::GetLiquid(void)
{
	// 引数位置を返す
	return *m_liquid.ppLiquid;
}

//============================================================
//	生成処理
//============================================================
CStage *CStage::Create(void)
{
	// ポインタを宣言
	CStage *pStage = NULL;		// ステージ生成用

	if (pStage == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pStage = new CStage;	// ステージ
	}
	else { assert(false); return NULL; }	// 使用中

	if (pStage != NULL)
	{ // 確保に成功している場合

		// ステージの初期化
		if (FAILED(pStage->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pStage;
			pStage = nullptr;

			// 失敗を返す
			assert(false);
			return NULL;
		}

		// セットアップの読込
		if (FAILED(LoadSetup(pStage)))
		{ // 読み込みに失敗した場合

			// メモリ開放
			delete pStage;
			pStage = nullptr;

			// 失敗を返す
			assert(false);
			return NULL;
		}

		// 確保したアドレスを返す
		return pStage;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CStage::Release(CStage *&prStage)
{
	if (prStage != NULL)
	{ // 使用中の場合

		// ステージの終了
		prStage->Uninit();

		// メモリ開放
		delete prStage;
		prStage = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CStage::LoadSetup(CStage *pStage)
{
	// 変数を宣言
	int nLoadID = rand() % (sizeof(SETUP_TXT) / sizeof(SETUP_TXT[0]));	// 読込ステージインデックス
	int nEnd = 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(SETUP_TXT[nLoadID], "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		while (1)
		{ // EOFまで無限ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// 終端の場合 EOFを返す
			if (nEnd == EOF)
			{ // ファイルが終端の場合

				// 処理を抜ける
				break;
			}

			// 範囲情報の読込
			if (FAILED(LoadLimit(&aString[0], pFile, pStage)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 地面の読込
			else if (FAILED(LoadField(&aString[0], pFile, pStage)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 壁の読込
			else if (FAILED(LoadWall(&aString[0], pFile, pStage)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 景色の読込
			else if (FAILED(LoadScenery(&aString[0], pFile, pStage)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 空の読込
			else if (FAILED(LoadSky(&aString[0], pFile, pStage)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 液体の読込
			else if (FAILED(LoadLiquid(&aString[0], pFile, pStage)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 地盤の読込
			else if (FAILED(LoadGround(&aString[0], pFile, pStage)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// ブロックの読込
			else if (FAILED(LoadBlock(&aString[0], pFile, pStage)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 障害物の読込
			else if (FAILED(LoadObstacle(&aString[0], pFile, pStage)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 生成位置の読込
			else if (FAILED(LoadSpawnPoint(&aString[0], pFile, pStage)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// アイテム生成位置の読込
			else if (FAILED(LoadItemPoint(&aString[0], pFile, pStage)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 遷移位置情報の読込
			else if (FAILED(LoadNextPoint(&aString[0], pFile, pStage)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}
		}
		
		// ファイルを閉じる
		fclose(pFile);

		// 成功を返す
		return S_OK;
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ステージセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}
}

//============================================================
//	範囲情報の読込処理
//============================================================
HRESULT CStage::LoadLimit(const char* pString, FILE *pFile, CStage *pStage)
{
	// 変数を宣言
	SStageLimit stageLimit;		// ステージ範囲の代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == NULL || pFile == NULL || pStage == NULL)
	{ // 文字列・ファイル・ステージが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ステージ範囲の設定
	if (strcmp(pString, "LIMITSET") == 0)
	{ // 読み込んだ文字列が LIMITSET の場合

		do
		{ // 読み込んだ文字列が END_LIMITSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "CENTER") == 0)
			{ // 読み込んだ文字列が CENTER の場合

				fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
				fscanf(pFile, "%f", &stageLimit.center.x);	// 中心座標Xを読み込む
				fscanf(pFile, "%f", &stageLimit.center.y);	// 中心座標Yを読み込む
				fscanf(pFile, "%f", &stageLimit.center.z);	// 中心座標Zを読み込む
			}
			else if (strcmp(&aString[0], "NEAR") == 0)
			{ // 読み込んだ文字列が NEAR の場合

				fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
				fscanf(pFile, "%f", &stageLimit.fNear);		// 前位置を読み込む

				// 制限モードを矩形範囲に設定
				stageLimit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "FAR") == 0)
			{ // 読み込んだ文字列が FAR の場合

				fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
				fscanf(pFile, "%f", &stageLimit.fFar);		// 後位置を読み込む

				// 制限モードを矩形範囲に設定
				stageLimit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "RIGHT") == 0)
			{ // 読み込んだ文字列が RIGHT の場合

				fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
				fscanf(pFile, "%f", &stageLimit.fRight);	// 右位置を読み込む

				// 制限モードを矩形範囲に設定
				stageLimit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "LEFT") == 0)
			{ // 読み込んだ文字列が LEFT の場合

				fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
				fscanf(pFile, "%f", &stageLimit.fLeft);		// 左位置を読み込む

				// 制限モードを矩形範囲に設定
				stageLimit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "RADIUS") == 0)
			{ // 読み込んだ文字列が RADIUS の場合

				fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
				fscanf(pFile, "%f", &stageLimit.fRadius);	// 半径を読み込む

				// 制限モードを円範囲に設定
				stageLimit.mode = LIMIT_CIRCLE;
			}
			else if (strcmp(&aString[0], "FIELD") == 0)
			{ // 読み込んだ文字列が FIELD の場合

				fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
				fscanf(pFile, "%f", &stageLimit.fField);	// 地面位置を読み込む
			}
		} while (strcmp(&aString[0], "END_LIMITSET") != 0);	// 読み込んだ文字列が END_LIMITSET ではない場合ループ

		// ステージ範囲の設定
		pStage->SetStageLimit(stageLimit);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	地面情報の読込処理
//============================================================
HRESULT CStage::LoadField(const char* pString, FILE *pFile, CStage *pStage)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	D3DXVECTOR2 size = VEC3_ZERO;	// 大きさの代入用
	D3DXCOLOR col = XCOL_WHITE;		// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用
	D3DCULL cull = D3DCULL_CCW;		// カリング状況の代入用

	bool bLight = false;	// ライティング状況の代入用
	int nCurrentID = 0;		// 現在の読み込み数の保存用
	int nTextureID = 0;		// テクスチャインデックスの代入用
	int nLight = 0;			// ライティング状況の変換用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == NULL || pFile == NULL || pStage == NULL)
	{ // 文字列・ファイル・ステージが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 地面の設定
	if (strcmp(pString, "STAGE_FIELDSET") == 0)
	{ // 読み込んだ文字列が STAGE_FIELDSET の場合

		// 現在の読み込み数を初期化
		nCurrentID = 0;

		do
		{ // 読み込んだ文字列が END_STAGE_FIELDSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM") == 0)
			{ // 読み込んだ文字列が NUM の場合

				fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
				fscanf(pFile, "%d", &pStage->m_field.nNum);	// 読み込み数を読み込む

				if (pStage->m_field.nNum > 0)
				{ // 読み込むものがある場合

					if (pStage->m_field.ppField == NULL)
					{ // 地面が使用されていない場合

						// 地面の読み込み数分メモリ確保
						pStage->m_field.ppField = new CField*[pStage->m_field.nNum];

						if (pStage->m_field.ppField != NULL)
						{ // 確保に成功した場合

							// メモリクリア
							memset(pStage->m_field.ppField, 0, sizeof(CField*) * pStage->m_field.nNum);
						}
						else { assert(false); return E_FAIL; }	// 確保失敗
					}
					else { assert(false); return E_FAIL; }	// 使用中
				}
				else
				{ // 読み込むものがない場合

					// 処理を抜ける
					break;
				}
			}
			else if (strcmp(&aString[0], "FIELDSET") == 0)
			{ // 読み込んだ文字列が FIELDSET の場合
	
				do
				{ // 読み込んだ文字列が END_FIELDSET ではない場合ループ
	
					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TEXTURE_ID") == 0)
					{ // 読み込んだ文字列が TEXTURE_ID の場合
	
						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nTextureID);	// テクスチャインデックスを読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &size.x);		// 大きさXを読み込む
						fscanf(pFile, "%f", &size.y);		// 大きさYを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "CULL") == 0)
					{ // 読み込んだ文字列が CULL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &cull);			// カリングを読み込む
					}
					else if (strcmp(&aString[0], "LIGHT") == 0)
					{ // 読み込んだ文字列が LIGHT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nLight);		// ライティングのON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						bLight = (nLight == 0) ? false : true;
					}
				} while (strcmp(&aString[0], "END_FIELDSET") != 0);	// 読み込んだ文字列が END_FIELDSET ではない場合ループ

				if (pStage->m_field.ppField[nCurrentID] == NULL)
				{ // 使用されていない場合

					// 地面オブジェクトの生成
					pStage->m_field.ppField[nCurrentID] = CField::Create((CField::ETexture)nTextureID, pos, D3DXToRadian(rot), size, col, part, cull, bLight);
					if (pStage->m_field.ppField[nCurrentID] == NULL)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
					}
				}
				else { assert(false); }	// 使用中

				// 読込総数オーバー
				assert(nCurrentID < pStage->m_field.nNum);

				// 現在の読み込み数を加算
				nCurrentID++;
			}
		} while (strcmp(&aString[0], "END_STAGE_FIELDSET") != 0);	// 読み込んだ文字列が END_STAGE_FIELDSET ではない場合ループ

		// 読込総数の不一致
		assert(nCurrentID == pStage->m_field.nNum);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	壁情報の読込処理
//============================================================
HRESULT CStage::LoadWall(const char* pString, FILE *pFile, CStage *pStage)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	D3DXVECTOR2 size = VEC3_ZERO;	// 大きさの代入用
	D3DXCOLOR col = XCOL_WHITE;		// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用
	D3DCULL cull = D3DCULL_CCW;		// カリング状況の代入用

	bool bLight = false;	// ライティング状況の代入用
	int nCurrentID = 0;		// 現在の読み込み数の保存用
	int nTextureID = 0;		// テクスチャインデックスの代入用
	int nLight = 0;			// ライティング状況の変換用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == NULL || pFile == NULL || pStage == NULL)
	{ // 文字列・ファイル・ステージが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 壁の設定
	if (strcmp(pString, "STAGE_WALLSET") == 0)
	{ // 読み込んだ文字列が STAGE_WALLSET の場合

		// 現在の読み込み数を初期化
		nCurrentID = 0;

		do
		{ // 読み込んだ文字列が END_STAGE_SCENERYSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM") == 0)
			{ // 読み込んだ文字列が NUM の場合

				fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
				fscanf(pFile, "%d", &pStage->m_wall.nNum);	// 読み込み数を読み込む

				if (pStage->m_wall.nNum > 0)
				{ // 読み込むものがある場合

					if (pStage->m_wall.ppWall == NULL)
					{ // 壁が使用されていない場合

						// 壁の読み込み数分メモリ確保
						pStage->m_wall.ppWall = new CWall*[pStage->m_wall.nNum];

						if (pStage->m_wall.ppWall != NULL)
						{ // 確保に成功した場合

							// メモリクリア
							memset(pStage->m_wall.ppWall, 0, sizeof(CWall*) * pStage->m_wall.nNum);
						}
						else { assert(false); return E_FAIL; }	// 確保失敗
					}
					else { assert(false); return E_FAIL; }	// 使用中
				}
				else
				{ // 読み込むものがない場合

					// 処理を抜ける
					break;
				}
			}
			else if (strcmp(&aString[0], "WALLSET") == 0)
			{ // 読み込んだ文字列が WALLSET の場合
	
				do
				{ // 読み込んだ文字列が END_WALLSET ではない場合ループ
	
					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TEXTURE_ID") == 0)
					{ // 読み込んだ文字列が TEXTURE_ID の場合
	
						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nTextureID);	// テクスチャインデックスを読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &size.x);		// 大きさXを読み込む
						fscanf(pFile, "%f", &size.y);		// 大きさYを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "CULL") == 0)
					{ // 読み込んだ文字列が CULL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &cull);			// カリングを読み込む
					}
					else if (strcmp(&aString[0], "LIGHT") == 0)
					{ // 読み込んだ文字列が LIGHT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nLight);		// ライティングのON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						bLight = (nLight == 0) ? false : true;
					}
				} while (strcmp(&aString[0], "END_WALLSET") != 0);	// 読み込んだ文字列が END_WALLSET ではない場合ループ

				if (pStage->m_wall.ppWall[nCurrentID] == NULL)
				{ // 使用されていない場合

					// 壁オブジェクトの生成
					pStage->m_wall.ppWall[nCurrentID] = CWall::Create((CWall::ETexture)nTextureID, pos, D3DXToRadian(rot), size, col, part, cull, bLight);
					if (pStage->m_wall.ppWall[nCurrentID] == NULL)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
					}
				}
				else { assert(false); }	// 使用中

				// 読込総数オーバー
				assert(nCurrentID < pStage->m_wall.nNum);

				// 現在の読み込み数を加算
				nCurrentID++;
			}
		} while (strcmp(&aString[0], "END_STAGE_WALLSET") != 0);	// 読み込んだ文字列が END_STAGE_WALLSET ではない場合ループ

		// 読込総数の不一致
		assert(nCurrentID == pStage->m_wall.nNum);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	景色情報の読込処理
//============================================================
HRESULT CStage::LoadScenery(const char* pString, FILE *pFile, CStage *pStage)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	D3DXCOLOR col = XCOL_WHITE;		// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用

	float fRadius = 0.0f;	// 半径の代入用
	float fHeight = 0.0f;	// 縦幅の代入用
	int nTextureID = 0;		// テクスチャインデックスの代入用
	int nCurrentID = 0;		// 現在の読み込み数の保存用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == NULL || pFile == NULL || pStage == NULL)
	{ // 文字列・ファイル・ステージが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 景色の設定
	if (strcmp(pString, "STAGE_SCENERYSET") == 0)
	{ // 読み込んだ文字列が STAGE_SCENERYSET の場合

		// 現在の読み込み数を初期化
		nCurrentID = 0;

		do
		{ // 読み込んだ文字列が END_STAGE_SCENERYSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM") == 0)
			{ // 読み込んだ文字列が NUM の場合

				fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
				fscanf(pFile, "%d", &pStage->m_scenery.nNum);	// 読み込み数を読み込む

				if (pStage->m_scenery.nNum > 0)
				{ // 読み込むものがある場合

					if (pStage->m_scenery.ppScenery == NULL)
					{ // 景色が使用されていない場合

						// 景色の読み込み数分メモリ確保
						pStage->m_scenery.ppScenery = new CScenery*[pStage->m_scenery.nNum];

						if (pStage->m_scenery.ppScenery != NULL)
						{ // 確保に成功した場合

							// メモリクリア
							memset(pStage->m_scenery.ppScenery, 0, sizeof(CScenery*) * pStage->m_scenery.nNum);
						}
						else { assert(false); return E_FAIL; }	// 確保失敗
					}
					else { assert(false); return E_FAIL; }	// 使用中
				}
				else
				{ // 読み込むものがない場合

					// 処理を抜ける
					break;
				}
			}
			else if (strcmp(&aString[0], "SCENERYSET") == 0)
			{ // 読み込んだ文字列が SCENERYSET の場合
	
				do
				{ // 読み込んだ文字列が END_SCENERYSET ではない場合ループ
	
					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TEXTURE_ID") == 0)
					{ // 読み込んだ文字列が TEXTURE_ID の場合
	
						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nTextureID);	// テクスチャインデックスを読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // 読み込んだ文字列が RADIUS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fRadius);		// 半径を読み込む
					}
					else if (strcmp(&aString[0], "HEIGHT") == 0)
					{ // 読み込んだ文字列が HEIGHT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fHeight);		// 縦幅を読み込む
					}
				} while (strcmp(&aString[0], "END_SCENERYSET") != 0);	// 読み込んだ文字列が END_SCENERYSET ではない場合ループ

				if (pStage->m_scenery.ppScenery[nCurrentID] == NULL)
				{ // 使用されていない場合

					// 景色オブジェクトの生成
					pStage->m_scenery.ppScenery[nCurrentID] = CScenery::Create((CScenery::ETexture)nTextureID, pos, D3DXToRadian(rot), col, part, fRadius, fHeight);
					if (pStage->m_scenery.ppScenery[nCurrentID] == NULL)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
					}
				}
				else { assert(false); }	// 使用中

				// 読込総数オーバー
				assert(nCurrentID < pStage->m_scenery.nNum);

				// 現在の読み込み数を加算
				nCurrentID++;
			}
		} while (strcmp(&aString[0], "END_STAGE_SCENERYSET") != 0);	// 読み込んだ文字列が END_STAGE_SCENERYSET ではない場合ループ

		// 読込総数の不一致
		assert(nCurrentID == pStage->m_scenery.nNum);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	空情報の読込処理
//============================================================
HRESULT CStage::LoadSky(const char* pString, FILE *pFile, CStage *pStage)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	D3DXCOLOR col = XCOL_WHITE;		// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用

	float fRadius = 0.0f;	// 半径の代入用
	int nTextureID = 0;		// テクスチャインデックスの代入用
	int nCurrentID = 0;		// 現在の読み込み数の保存用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == NULL || pFile == NULL || pStage == NULL)
	{ // 文字列・ファイル・ステージが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 空の設定
	if (strcmp(pString, "STAGE_SKYSET") == 0)
	{ // 読み込んだ文字列が STAGE_SKYSET の場合

		// 現在の読み込み数を初期化
		nCurrentID = 0;

		do
		{ // 読み込んだ文字列が END_STAGE_SKYSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM") == 0)
			{ // 読み込んだ文字列が NUM の場合

				fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
				fscanf(pFile, "%d", &pStage->m_sky.nNum);	// 読み込み数を読み込む

				if (pStage->m_sky.nNum > 0)
				{ // 読み込むものがある場合

					if (pStage->m_sky.ppSky == NULL)
					{ // 空が使用されていない場合

						// 空の読み込み数分メモリ確保
						pStage->m_sky.ppSky = new CSky*[pStage->m_sky.nNum];

						if (pStage->m_sky.ppSky != NULL)
						{ // 確保に成功した場合

							// メモリクリア
							memset(pStage->m_sky.ppSky, 0, sizeof(CSky*) * pStage->m_sky.nNum);
						}
						else { assert(false); return E_FAIL; }	// 確保失敗
					}
					else { assert(false); return E_FAIL; }	// 使用中
				}
				else
				{ // 読み込むものがない場合

					// 処理を抜ける
					break;
				}
			}
			else if (strcmp(&aString[0], "SKYSET") == 0)
			{ // 読み込んだ文字列が SKYSET の場合

				do
				{ // 読み込んだ文字列が END_SKYSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TEXTURE_ID") == 0)
					{ // 読み込んだ文字列が TEXTURE_ID の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nTextureID);	// テクスチャインデックスを読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // 読み込んだ文字列が RADIUS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fRadius);		// 半径を読み込む
					}
				} while (strcmp(&aString[0], "END_SKYSET") != 0);	// 読み込んだ文字列が END_SKYSET ではない場合ループ

				if (pStage->m_sky.ppSky[nCurrentID] == NULL)
				{ // 使用されていない場合

					// 空オブジェクトの生成
					pStage->m_sky.ppSky[nCurrentID] = CSky::Create((CSky::ETexture)nTextureID, pos, D3DXToRadian(rot), col, part, fRadius);
					if (pStage->m_sky.ppSky[nCurrentID] == NULL)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
					}
				}
				else { assert(false); }	// 使用中

				// 読込総数オーバー
				assert(nCurrentID < pStage->m_sky.nNum);

				// 現在の読み込み数を加算
				nCurrentID++;
			}
		} while (strcmp(&aString[0], "END_STAGE_SKYSET") != 0);	// 読み込んだ文字列が END_STAGE_SKYSET ではない場合ループ

		// 読込総数の不一致
		assert(nCurrentID == pStage->m_sky.nNum);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	液体情報の読込処理
//============================================================
HRESULT CStage::LoadLiquid(const char* pString, FILE *pFile, CStage *pStage)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	D3DXVECTOR2 size = VEC3_ZERO;	// 大きさの代入用
	D3DXCOLOR col = XCOL_WHITE;		// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用
	CLiquid::STexMove texMove;		// テクスチャ移動量の代入用

	float fMaxUp = 0.0f;		// 波の最高上昇量
	float fAddSinRot = 0.0f;	// 波打ち向き加算量
	float fAddVtxRot = 0.0f;	// 隣波の向き加算量
	int nTypeID = 0;			// 種類インデックスの代入用
	int nCurrentID = 0;			// 現在の読み込み数の保存用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == NULL || pFile == NULL || pStage == NULL)
	{ // 文字列・ファイル・ステージが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 液体の設定
	if (strcmp(pString, "STAGE_LIQUIDSET") == 0)
	{ // 読み込んだ文字列が STAGE_LIQUIDSET の場合

		// 現在の読み込み数を初期化
		nCurrentID = 0;

		do
		{ // 読み込んだ文字列が END_STAGE_LIQUIDSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM") == 0)
			{ // 読み込んだ文字列が NUM の場合

				fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
				fscanf(pFile, "%d", &pStage->m_liquid.nNum);	// 読み込み数を読み込む

				if (pStage->m_liquid.nNum > 0)
				{ // 読み込むものがある場合

					if (pStage->m_liquid.ppLiquid == NULL)
					{ // 液体が使用されていない場合

						// 液体の読み込み数分メモリ確保
						pStage->m_liquid.ppLiquid = new CLiquid*[pStage->m_liquid.nNum];

						if (pStage->m_liquid.ppLiquid != NULL)
						{ // 確保に成功した場合

							// メモリクリア
							memset(pStage->m_liquid.ppLiquid, 0, sizeof(CLiquid*) * pStage->m_liquid.nNum);
						}
						else { assert(false); return E_FAIL; }	// 確保失敗
					}
					else { assert(false); return E_FAIL; }	// 使用中
				}
				else
				{ // 読み込むものがない場合

					// 処理を抜ける
					break;
				}
			}
			else if (strcmp(&aString[0], "LIQUIDSET") == 0)
			{ // 読み込んだ文字列が LIQUIDSET の場合

				do
				{ // 読み込んだ文字列が END_LIQUIDSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nTypeID);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &size.x);		// 大きさXを読み込む
						fscanf(pFile, "%f", &size.y);		// 大きさYを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXMOVE_LOW") == 0)
					{ // 読み込んだ文字列が TEXMOVE_LOW の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &texMove.texMoveLow.x);	// 下液体のテクスチャ移動量を読み込む
						fscanf(pFile, "%f", &texMove.texMoveLow.y);	// 下液体のテクスチャ移動量を読み込む
					}
					else if (strcmp(&aString[0], "TEXMOVE_HIGH") == 0)
					{ // 読み込んだ文字列が TEXMOVE_HIGH の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f", &texMove.texMoveHigh.x);	// 上液体のテクスチャ移動量を読み込む
						fscanf(pFile, "%f", &texMove.texMoveHigh.y);	// 上液体のテクスチャ移動量を読み込む
					}
					else if (strcmp(&aString[0], "MAX_UP") == 0)
					{ // 読み込んだ文字列が MAX_UP の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fMaxUp);		// 波の最高上昇量を読み込む
					}
					else if (strcmp(&aString[0], "ADD_SINROT") == 0)
					{ // 読み込んだ文字列が ADD_SINROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fAddSinRot);	// 波打ち向き加算量を読み込む
					}
					else if (strcmp(&aString[0], "ADD_VTXROT") == 0)
					{ // 読み込んだ文字列が ADD_VTXROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fAddVtxRot);	// 隣波の向き加算量を読み込む
					}
				} while (strcmp(&aString[0], "END_LIQUIDSET") != 0);	// 読み込んだ文字列が END_LIQUIDSET ではない場合ループ

				if (pStage->m_liquid.ppLiquid[nCurrentID] == NULL)
				{ // 使用されていない場合

					// 液体オブジェクトの生成
					pStage->m_liquid.ppLiquid[nCurrentID] = CLiquid::Create
					( // 引数
						(CLiquid::EType)nTypeID,	// 種類
						pos,						// 位置
						D3DXToRadian(rot),			// 向き
						size,						// 大きさ
						col,						// 色
						part,						// 分割数
						texMove,					// テクスチャ移動量
						fMaxUp,						// 波の最高上昇量
						D3DXToRadian(fAddSinRot),	// 波打ち向き加算量
						D3DXToRadian(fAddVtxRot)	// 隣波の向き加算量
					);
					if (pStage->m_liquid.ppLiquid[nCurrentID] == NULL)
					{ // 確保に失敗した場合

						// 失敗を返す
						assert(false);
						return E_FAIL;
					}
				}
				else { assert(false); }	// 使用中

				// 読込総数オーバー
				assert(nCurrentID < pStage->m_liquid.nNum);

				// 現在の読み込み数を加算
				nCurrentID++;
			}
		} while (strcmp(&aString[0], "END_STAGE_LIQUIDSET") != 0);	// 読み込んだ文字列が END_STAGE_LIQUIDSET ではない場合ループ

		// 読込総数の不一致
		assert(nCurrentID == pStage->m_liquid.nNum);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	地盤情報の読込処理
//============================================================
HRESULT CStage::LoadGround(const char* pString, FILE *pFile, CStage *pStage)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;		// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;		// 向きの代入用
	D3DXVECTOR3 size = VEC3_ZERO;		// 大きさの代入用
	D3DXVECTOR2 partTexX = VEC2_ZERO;	// テクスチャ分割数Xの代入用
	D3DXVECTOR2 partTexY = VEC2_ZERO;	// テクスチャ分割数Yの代入用
	D3DXVECTOR2 partTexZ = VEC2_ZERO;	// テクスチャ分割数Zの代入用
	int nTypeID = 0;					// 種類インデックスの代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == NULL || pFile == NULL || pStage == NULL)
	{ // 文字列・ファイル・ステージが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 地盤の設定
	if (strcmp(pString, "STAGE_GROUNDSET") == 0)
	{ // 読み込んだ文字列が STAGE_GROUNDSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_GROUNDSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "GROUNDSET") == 0)
			{ // 読み込んだ文字列が GROUNDSET の場合

				do
				{ // 読み込んだ文字列が END_GROUNDSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nTypeID);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &size.x);		// 大きさXを読み込む
						fscanf(pFile, "%f", &size.y);		// 大きさYを読み込む
						fscanf(pFile, "%f", &size.z);		// 大きさZを読み込む
					}
					else if (strcmp(&aString[0], "PARTX") == 0)
					{ // 読み込んだ文字列が PARTX の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &partTexX.x);	// Xテクスチャ分割数Xを読み込む
						fscanf(pFile, "%f", &partTexX.y);	// Xテクスチャ分割数Yを読み込む
					}
					else if (strcmp(&aString[0], "PARTY") == 0)
					{ // 読み込んだ文字列が PARTY の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &partTexY.x);	// Yテクスチャ分割数Xを読み込む
						fscanf(pFile, "%f", &partTexY.y);	// Yテクスチャ分割数Yを読み込む
					}
					else if (strcmp(&aString[0], "PARTZ") == 0)
					{ // 読み込んだ文字列が PARTZ の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &partTexZ.x);	// Zテクスチャ分割数Xを読み込む
						fscanf(pFile, "%f", &partTexZ.y);	// Zテクスチャ分割数Yを読み込む
					}
				} while (strcmp(&aString[0], "END_GROUNDSET") != 0);	// 読み込んだ文字列が END_GROUNDSET ではない場合ループ

				// 地盤オブジェクトの生成
				if (CGround::Create((CGround::EType)nTypeID, pos, rot, size, partTexX, partTexY, partTexZ) == NULL)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_GROUNDSET") != 0);	// 読み込んだ文字列が END_STAGE_GROUNDSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	ブロック情報の読込処理
//============================================================
HRESULT CStage::LoadBlock(const char* pString, FILE *pFile, CStage *pStage)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;		// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;		// 向きの代入用
	D3DXVECTOR3 size = VEC3_ZERO;		// 大きさの代入用
	D3DXVECTOR2 partTexX = VEC2_ZERO;	// テクスチャ分割数Xの代入用
	D3DXVECTOR2 partTexY = VEC2_ZERO;	// テクスチャ分割数Yの代入用
	D3DXVECTOR2 partTexZ = VEC2_ZERO;	// テクスチャ分割数Zの代入用
	int nTypeID = 0;					// 種類インデックスの代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == NULL || pFile == NULL || pStage == NULL)
	{ // 文字列・ファイル・ステージが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ブロックの設定
	if (strcmp(pString, "STAGE_BLOCKSET") == 0)
	{ // 読み込んだ文字列が STAGE_BLOCKSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_BLOCKSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "BLOCKSET") == 0)
			{ // 読み込んだ文字列が BLOCKSET の場合

				do
				{ // 読み込んだ文字列が END_BLOCKSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nTypeID);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &size.x);		// 大きさXを読み込む
						fscanf(pFile, "%f", &size.y);		// 大きさYを読み込む
						fscanf(pFile, "%f", &size.z);		// 大きさZを読み込む
					}
					else if (strcmp(&aString[0], "PARTX") == 0)
					{ // 読み込んだ文字列が PARTX の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &partTexX.x);	// Xテクスチャ分割数Xを読み込む
						fscanf(pFile, "%f", &partTexX.y);	// Xテクスチャ分割数Yを読み込む
					}
					else if (strcmp(&aString[0], "PARTY") == 0)
					{ // 読み込んだ文字列が PARTY の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &partTexY.x);	// Yテクスチャ分割数Xを読み込む
						fscanf(pFile, "%f", &partTexY.y);	// Yテクスチャ分割数Yを読み込む
					}
					else if (strcmp(&aString[0], "PARTZ") == 0)
					{ // 読み込んだ文字列が PARTZ の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &partTexZ.x);	// Zテクスチャ分割数Xを読み込む
						fscanf(pFile, "%f", &partTexZ.y);	// Zテクスチャ分割数Yを読み込む
					}
				} while (strcmp(&aString[0], "END_BLOCKSET") != 0);	// 読み込んだ文字列が END_BLOCKSET ではない場合ループ

				// ブロックオブジェクトの生成
				if (CBlock::Create((CBlock::EType)nTypeID, pos, rot, size, partTexX, partTexY, partTexZ) == NULL)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_BLOCKSET") != 0);	// 読み込んだ文字列が END_STAGE_BLOCKSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	障害物情報の読込処理
//============================================================
HRESULT CStage::LoadObstacle(const char* pString, FILE *pFile, CStage *pStage)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	int nTypeID = 0;				// 種類インデックスの代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == NULL || pFile == NULL || pStage == NULL)
	{ // 文字列・ファイル・ステージが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 障害物の設定
	if (strcmp(pString, "STAGE_OBSTACLESET") == 0)
	{ // 読み込んだ文字列が STAGE_OBSTACLESET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_OBSTACLESET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "OBSTACLESET") == 0)
			{ // 読み込んだ文字列が OBSTACLESET の場合

				do
				{ // 読み込んだ文字列が END_OBSTACLESET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nTypeID);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
				} while (strcmp(&aString[0], "END_OBSTACLESET") != 0);	// 読み込んだ文字列が END_OBSTACLESET ではない場合ループ

				// 障害物オブジェクトの生成
				if (CObstacle::Create((CObstacle::EType)nTypeID, pos, rot) == NULL)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_OBSTACLESET") != 0);	// 読み込んだ文字列が END_STAGE_OBSTACLESET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	生成位置情報の読込処理
//============================================================
HRESULT CStage::LoadSpawnPoint(const char* pString, FILE *pFile, CStage *pStage)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == NULL || pFile == NULL || pStage == NULL)
	{ // 文字列・ファイル・ステージが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 生成位置の設定
	if (strcmp(pString, "STAGE_SPAWNPOINTSET") == 0)
	{ // 読み込んだ文字列が STAGE_SPAWNPOINTSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_SPAWNPOINTSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SPAWNPOINTSET") == 0)
			{ // 読み込んだ文字列が SPAWNPOINTSET の場合

				do
				{ // 読み込んだ文字列が END_SPAWNPOINTSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
				} while (strcmp(&aString[0], "END_SPAWNPOINTSET") != 0);	// 読み込んだ文字列が END_SPAWNPOINTSET ではない場合ループ

				// 生成位置オブジェクトの生成
				if (CSpawnPoint::Create(pos, rot) == NULL)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_SPAWNPOINTSET") != 0);	// 読み込んだ文字列が END_STAGE_SPAWNPOINTSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	アイテム生成位置情報の読込処理
//============================================================
HRESULT CStage::LoadItemPoint(const char * pString, FILE * pFile, CStage * pStage)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == NULL || pFile == NULL || pStage == NULL)
	{ // 文字列・ファイル・ステージが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// アイテム生成位置の設定
	if (strcmp(pString, "STAGE_ITEMPOINTSET") == 0)
	{ // 読み込んだ文字列が STAGE_ITEMPOINTSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_ITEMPOINTSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "ITEMPOINTSET") == 0)
			{ // 読み込んだ文字列が ITEMPOINTSET の場合

				do
				{ // 読み込んだ文字列が END_ITEMPOINTSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
				} while (strcmp(&aString[0], "END_ITEMPOINTSET") != 0);	// 読み込んだ文字列が END_ITEMPOINTSET ではない場合ループ

				// アイテム生成位置オブジェクトの生成
				if (CItemSpawnPoint::Create(pos) == NULL)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_ITEMPOINTSET") != 0);	// 読み込んだ文字列が END_STAGE_ITEMPOINTSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	遷移位置情報の読込処理
//============================================================
HRESULT CStage::LoadNextPoint(const char * pString, FILE * pFile, CStage * pStage)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 size = VEC3_ZERO;	// 大きさの代入用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	if (pString == NULL || pFile == NULL || pStage == NULL)
	{ // 文字列・ファイル・ステージが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 遷移位置の設定
	if (strcmp(pString, "STAGE_NEXTPOINTSET") == 0)
	{ // 読み込んだ文字列が STAGE_NEXTPOINTSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_NEXTPOINTSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NEXTPOINTSET") == 0)
			{ // 読み込んだ文字列が NEXTPOINTSET の場合

				do
				{ // 読み込んだ文字列が END_NEXTPOINTSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &size.x);		// 大きさXを読み込む
						fscanf(pFile, "%f", &size.y);		// 大きさYを読み込む
						fscanf(pFile, "%f", &size.z);		// 大きさZを読み込む
					}
				} while (strcmp(&aString[0], "END_NEXTPOINTSET") != 0);	// 読み込んだ文字列が END_NEXTPOINTSET ではない場合ループ

				// 遷移位置オブジェクトの生成
				if (CNextPoint::Create(pos, size) == NULL)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_NEXTPOINTSET") != 0);	// 読み込んだ文字列が END_STAGE_NEXTPOINTSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}
