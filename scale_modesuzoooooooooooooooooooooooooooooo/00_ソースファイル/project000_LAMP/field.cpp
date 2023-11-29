//============================================================
//
//	地面処理 [field.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define FIELD_SETUP_TXT	"data\\TXT\\field.txt"	// セットアップテキスト相対パス

#define FIELD_PRIO	(0)	// 地面の優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CField::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\road000.png",		// 直線テクスチャ
	"data\\TEXTURE\\road001.png",		// 曲がり角テクスチャ
	"data\\TEXTURE\\road002.png",		// 交差点テクスチャ
	"data\\TEXTURE\\road003.png",		// 丁字路テクスチャ
	"data\\TEXTURE\\sidewalk000.png",	// 薄い歩道テクスチャ
	"data\\TEXTURE\\sidewalk001.png",	// 濃い歩道テクスチャ
};
CField::STerrainInfo CField::m_aTerrainInfo[TERRAIN_MAX] = {};	// 地形情報

//************************************************************
//	子クラス [CField] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CField::CField() : CObjectMeshField(CObject::LABEL_FIELD, FIELD_PRIO)
{

}

//============================================================
//	デストラクタ
//============================================================
CField::~CField()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CField::Init(void)
{
	// オブジェクトメッシュフィールドの初期化
	if (FAILED(CObjectMeshField::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// セットアップの読込
	LoadSetup();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CField::Uninit(void)
{
	// オブジェクトメッシュフィールドの終了
	CObjectMeshField::Uninit();

	// 地形情報の破棄
	for (int nCntField = 0; nCntField < TERRAIN_MAX; nCntField++)
	{ // 地形情報の最大数分繰り返す

		if (m_aTerrainInfo[nCntField].pPosGap != NULL)
		{ // 頂点座標のずれ量情報が使用中の場合

			// メモリ開放
			delete[] m_aTerrainInfo[nCntField].pPosGap;
			m_aTerrainInfo[nCntField].pPosGap = NULL;
		}
	}
}

//============================================================
//	更新処理
//============================================================
void CField::Update(void)
{
	// オブジェクトメッシュフィールドの更新
	CObjectMeshField::Update();
}

//============================================================
//	描画処理
//============================================================
void CField::Draw(void)
{
	// オブジェクトメッシュフィールドの描画
	CObjectMeshField::Draw();
}

//============================================================
//	生成処理
//============================================================
CField *CField::Create
(
	const ETexture texture,		// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR2& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const D3DCULL cull,			// カリング状況
	const bool bLight			// ライティング状況
)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	CField *pField = NULL;	// 地面生成用

	if (pField == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pField = new CField;	// 地面
	}
	else { assert(false); return NULL; }	// 使用中

	if (pField != NULL)
	{ // 確保に成功している場合

		// 地面の初期化
		if (FAILED(pField->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pField;
			pField = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャを登録
		nTextureID = pTexture->Regist(mc_apTextureFile[texture]);

		// テクスチャを割当
		pField->BindTexture(nTextureID);

		// 位置を設定
		pField->SetVec3Position(rPos);

		// 向きを設定
		pField->SetVec3Rotation(rRot);

		// 大きさを設定
		pField->SetVec2Sizing(rSize);

		// 色を設定
		pField->SetColor(rCol);

		// カリングを設定
		pField->SetCulling(cull);

		// ライティングを設定
		pField->SetLighting(bLight);

		// 分割数を設定
		if (FAILED(pField->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// メモリ開放
			delete pField;
			pField = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pField;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	地形の設定処理
//============================================================
void CField::SetTerrain(const ETerrain terrain)
{
	if (terrain < TERRAIN_MAX)
	{ // 値が範囲内の場合

		// 地形を設定
		CObjectMeshField::SetTerrain(m_aTerrainInfo[terrain].part, m_aTerrainInfo[terrain].pPosGap);
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	セットアップ処理
//============================================================
void CField::LoadSetup(void)
{
	// 変数を宣言
	int nID = 0;		// インデックスの代入用
	int nNumVtx = 0;	// 頂点数の代入用
	int nEnd = 0;		// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// 地形情報の破棄
	for (int nCntField = 0; nCntField < TERRAIN_MAX; nCntField++)
	{ // 地形情報の最大数分繰り返す

		if (m_aTerrainInfo[nCntField].pPosGap != NULL)
		{ // 頂点座標のずれ量情報が使用中の場合

			// メモリ開放
			delete[] m_aTerrainInfo[nCntField].pPosGap;
			m_aTerrainInfo[nCntField].pPosGap = NULL;
		}
	}

	// 静的メンバ変数の情報をクリア
	memset(&m_aTerrainInfo[0], 0, sizeof(m_aTerrainInfo));	// 地形情報

	// ファイルを読み込み形式で開く
	pFile = fopen(FIELD_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// 地形の設定
			if (strcmp(&aString[0], "TERRAINSET") == 0)
			{ // 読み込んだ文字列が TERRAINSET の場合

				do
				{ // 読み込んだ文字列が END_TERRAINSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "FIELDSET") == 0)
					{ // 読み込んだ文字列が FIELDSET の場合

						do
						{ // 読み込んだ文字列が END_FIELDSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "PART") == 0)
							{ // 読み込んだ文字列が PART の場合

								fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
								fscanf(pFile, "%d", &m_aTerrainInfo[nID].part.x);	// 分割数Xを読み込む
								fscanf(pFile, "%d", &m_aTerrainInfo[nID].part.y);	// 分割数Yを読み込む

								// 頂点数を設定
								nNumVtx = (m_aTerrainInfo[nID].part.x + 1) * (m_aTerrainInfo[nID].part.y + 1);

								if (m_aTerrainInfo[nID].pPosGap == NULL)
								{ // ポインタが使用されていない場合

									// 頂点数分メモリ確保
									m_aTerrainInfo[nID].pPosGap = new D3DXVECTOR3[nNumVtx];

									// 例外処理
									assert(m_aTerrainInfo[nID].pPosGap != NULL);	// 非使用中
								}
								else { assert(false); }	// 使用中
							}
							else if (strcmp(&aString[0], "GAPSET") == 0)
							{ // 読み込んだ文字列が GAPSET の場合

								for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
								{ // 頂点数分繰り返す

									fscanf(pFile, "%f", &m_aTerrainInfo[nID].pPosGap[nCntVtx].x);	// 頂点座標のずれ量Xを読み込む
									fscanf(pFile, "%f", &m_aTerrainInfo[nID].pPosGap[nCntVtx].y);	// 頂点座標のずれ量Yを読み込む
									fscanf(pFile, "%f", &m_aTerrainInfo[nID].pPosGap[nCntVtx].z);	// 頂点座標のずれ量Zを読み込む
									fscanf(pFile, "%s", &aString[0]);								// , を読み込む (不要)
								}
							}
						} while (strcmp(&aString[0], "END_FIELDSET") != 0);	// 読み込んだ文字列が END_FIELDSET ではない場合ループ

						// インデックスを加算
						nID++;
					}
				} while (strcmp(&aString[0], "END_TERRAINSET") != 0);	// 読み込んだ文字列が END_TERRAINSET ではない場合ループ
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "地面セットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
