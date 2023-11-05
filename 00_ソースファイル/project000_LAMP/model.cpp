//============================================================
//
//	モデル処理 [model.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MODEL_SETUP_TXT	"data\\TXT\\model.txt"	// セットアップテキスト相対パス

#define INIT_VTXMIN	(D3DXVECTOR3( 9999.0f,  9999.0f,  9999.0f))	// モデルの最小の頂点座標の初期値
#define INIT_VTXMAX	(D3DXVECTOR3(-9999.0f, -9999.0f, -9999.0f))	// モデルの最大の頂点座標の初期値

//************************************************************
//	静的メンバ変数宣言
//************************************************************
int CModel::m_nNumAll = 0;	// モデルの総数

//************************************************************
//	親クラス [CModel] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CModel::CModel()
{
	// モデルへのポインタをクリア
	memset(&m_aModel[0], 0, sizeof(m_aModel));

	// 読み込んだモデルファイル名をクリア
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{ // モデルの最大数分繰り返す

		// NULL文字をコピー
		strcpy(&m_pFileName[nCntModel][0], NONE_STRING);
	}
}

//============================================================
//	デストラクタ
//============================================================
CModel::~CModel()
{

}

//============================================================
//	モデル生成処理
//============================================================
HRESULT CModel::Load(void)
{
	// モデルへのポインタを初期化
	memset(&m_aModel[0], 0, sizeof(m_aModel));

	// 読み込んだモデルファイル名を初期化
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{ // モデルの最大数分繰り返す

		// NULL文字をコピー
		strcpy(&m_pFileName[nCntModel][0], NONE_STRING);
	}

	// セットアップの読込
	LoadSetup();

	// 成功を返す
	return S_OK;
}

//============================================================
//	モデル破棄処理
//============================================================
void CModel::Unload(void)
{
	// モデルの破棄
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{ // モデルの最大数分繰り返す

		// テクスチャインデックスの破棄
		if (m_aModel[nCntModel].pTextureID != NULL)
		{ // テクスチャインデックスが使用中の場合

			// メモリ開放
			free(m_aModel[nCntModel].pTextureID);
			m_aModel[nCntModel].pTextureID = NULL;
		}

		// メッシュの破棄
		if (m_aModel[nCntModel].pMesh != NULL)
		{ // メッシュが使用中の場合

			// メモリ開放
			m_aModel[nCntModel].pMesh->Release();
			m_aModel[nCntModel].pMesh = NULL;
		}

		// マテリアルの破棄
		if (m_aModel[nCntModel].pBuffMat != NULL)
		{ // マテリアルが使用中の場合

			// メモリ開放
			m_aModel[nCntModel].pBuffMat->Release();
			m_aModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//============================================================
//	モデル登録処理
//============================================================
int CModel::Regist(const char *pFileName)
{
	// 変数を宣言
	int nID = m_nNumAll;	// モデル読込番号

	if (pFileName != NULL)
	{ // ポインタが使用されている場合

		for (int nCntModel = 0; nCntModel < m_nNumAll; nCntModel++)
		{ // モデルの総数分繰り返す

			if (strcmp(&m_pFileName[nCntModel][0], pFileName) == 0)
			{ // 文字列が一致した場合

				// すでに読み込んでいるモデルの配列番号を返す
				return nCntModel;
			}
		}

		if (m_nNumAll >= MAX_MODEL)
		{ // モデルオーバーの場合

			// 失敗を返す
			assert(false);
			return NONE_IDX;
		}

		// xファイルの読み込み
		if (FAILED(LoadXFileModel(nID, pFileName)))
		{ // xファイルの読み込みに失敗した場合

			// 失敗を返す
			assert(false);
			return NONE_IDX;
		}

		// テクスチャの読み込み
		if (FAILED(LoadTextureModel(nID)))
		{ // テクスチャの読み込みに失敗した場合

			// 失敗を返す
			assert(false);
			return NONE_IDX;
		}

		// 当たり判定の作成
		SetCollisionModel(nID);

		// モデルのファイル名を保存
		strcpy(&m_pFileName[nID][0], pFileName);

		// モデルの総数を加算
		m_nNumAll++;

		// 読み込んだモデルの配列番号を返す
		return nID;
	}
	else
	{ // ポインタが使用されていない場合

		// 失敗を返す
		assert(false);
		return NONE_IDX;
	}
}

//============================================================
//	モデル情報取得処理
//============================================================
CModel::SModel *CModel::GetModel(const int nID)
{
	if (nID > NONE_IDX && nID < m_nNumAll)
	{ // 引数のインデックスが範囲内の場合

		// 引数のモデルアドレスを返す
		return &m_aModel[nID];
	}
	else { assert(false); return NULL; }	// 範囲外
}

//============================================================
//	生成処理
//============================================================
CModel *CModel::Create(void)
{
	// ポインタを宣言
	CModel *pModel = NULL;	// モデル生成用

	if (pModel == NULL)
	{ // 使用されていない場合

		// メモリを確保
		pModel = new CModel;	// モデル
	}
	else { assert(false); return NULL; }	// 使用中

	if (pModel != NULL)
	{ // 確保に成功している場合

		// モデルの読込
		if (FAILED(pModel->Load()))
		{ // モデル読み込みに失敗した場合

			// メモリ開放
			delete pModel;
			pModel = NULL;

			// 失敗を返す
			assert(false);
			return NULL;
		}

		// 確保したアドレスを返す
		return pModel;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CModel::Release(CModel *&prModel)
{
	if (prModel != NULL)
	{ // 使用中の場合

		// モデルの破棄
		prModel->Unload();

		// メモリ開放
		delete prModel;
		prModel = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	xファイルの読み込み
//============================================================
HRESULT CModel::LoadXFileModel(const int nID, const char *pFileName)
{
	// 変数を宣言
	HRESULT hr;	// 異常終了の確認用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// xファイルの読み込み
	hr = D3DXLoadMeshFromX
	( // 引数
		pFileName,					// モデルの相対パス
		D3DXMESH_SYSTEMMEM,			// メッシュ作成用オプション
		pDevice,					// デバイスへのポインタ
		NULL,						// 隣接性データ
		&m_aModel[nID].pBuffMat,	// マテリアルへのポインタ
		NULL,						// エフェクトデータ
		&m_aModel[nID].dwNumMat,	// マテリアルの数
		&m_aModel[nID].pMesh		// メッシュ (頂点情報) へのポインタ
	);

	if (FAILED(hr))
	{ // xファイルの読み込みに失敗した場合

		// エラーメッセージボックス
		MessageBox(NULL, "xファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_aModel[nID].pTextureID == NULL)
	{ // 使用されていない場合

		// 確保したメモリのアドレスを取得
		m_aModel[nID].pTextureID = (int*)malloc(m_aModel[nID].dwNumMat * sizeof(int*));
	}

	if (m_aModel[nID].pTextureID == NULL)
	{ // 動的確保に失敗した場合

		// エラーメッセージボックス
		MessageBox(NULL, "動的確保に失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	テクスチャの読み込み
//============================================================
HRESULT CModel::LoadTextureModel(const int nID)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	D3DXMATERIAL *pMat;		// マテリアルへのポインタ

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_aModel[nID].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_aModel[nID].dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		if (pMat[nCntMat].pTextureFilename != NULL)
		{ // テクスチャファイルが存在する場合

			// テクスチャを登録
			m_aModel[nID].pTextureID[nCntMat] = pTexture->Regist(pMat[nCntMat].pTextureFilename);
		}
		else
		{ // テクスチャファイルが存在しない場合

			// テクスチャを登録
			m_aModel[nID].pTextureID[nCntMat] = NONE_IDX;	// テクスチャなし
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	当たり判定の作成
//============================================================
void CModel::SetCollisionModel(const int nID)
{
	// 変数を宣言
	int			nNumVtx;	// モデルの頂点数
	DWORD		dwSizeFVF;	// モデルの頂点フォーマットのサイズ
	BYTE		*pVtxBuff;	// モデルの頂点バッファへのポインタ
	D3DXVECTOR3	vtx;		// モデルの頂点座標

	// モデルの頂点数を取得
	nNumVtx = m_aModel[nID].pMesh->GetNumVertices();

	// モデルの頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(m_aModel[nID].pMesh->GetFVF());

	// モデルの頂点バッファをロック
	m_aModel[nID].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{ // モデルの頂点数分繰り返す

		// モデルの頂点座標を代入
		vtx = *(D3DXVECTOR3*)pVtxBuff;

		// x頂点座標の設定
		if (vtx.x < m_aModel[nID].vtxMin.x)
		{ // 現状の x頂点座標よりも小さい場合

			// x頂点情報を代入
			m_aModel[nID].vtxMin.x = vtx.x;
		}
		else if (vtx.x > m_aModel[nID].vtxMax.x)
		{ // 現状の x頂点座標よりも大きい場合

			// x頂点情報を代入
			m_aModel[nID].vtxMax.x = vtx.x;
		}

		// y頂点座標の設定
		if (vtx.y < m_aModel[nID].vtxMin.y)
		{ // 現状の y頂点座標よりも小さい場合

			// y頂点情報を代入
			m_aModel[nID].vtxMin.y = vtx.y;
		}
		else if (vtx.y > m_aModel[nID].vtxMax.y)
		{ // 現状の y頂点座標よりも大きい場合

			// y頂点情報を代入
			m_aModel[nID].vtxMax.y = vtx.y;
		}

		// z頂点座標の設定
		if (vtx.z < m_aModel[nID].vtxMin.z)
		{ // 現状の z頂点座標よりも小さい場合

			// z頂点情報を代入
			m_aModel[nID].vtxMin.z = vtx.z;
		}
		else if (vtx.z > m_aModel[nID].vtxMax.z)
		{ // 現状の z頂点座標よりも大きい場合

			// z頂点情報を代入
			m_aModel[nID].vtxMax.z = vtx.z;
		}

		// 頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += dwSizeFVF;
	}

	// モデルの頂点バッファをアンロック
	m_aModel[nID].pMesh->UnlockVertexBuffer();

	// モデルサイズを求める
	m_aModel[nID].size = m_aModel[nID].vtxMax - m_aModel[nID].vtxMin;

	// モデルの円の当たり判定を作成
	m_aModel[nID].fRadius = ((m_aModel[nID].size.x * 0.5f) + (m_aModel[nID].size.z * 0.5f)) * 0.5f;
}

//============================================================
//	セットアップ処理
//============================================================
void CModel::LoadSetup(void)
{
	// 変数を宣言
	int nEnd = 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(MODEL_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "FILENAME") == 0)
			{ // 読み込んだ文字列が FILENAME の場合

				// = を読み込む (不要)
				fscanf(pFile, "%s", &aString[0]);

				// ファイルパスを読み込む
				fscanf(pFile, "%s", &aString[0]);

				// モデルを登録
				Regist(&aString[0]);
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "モデルセットアップファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
