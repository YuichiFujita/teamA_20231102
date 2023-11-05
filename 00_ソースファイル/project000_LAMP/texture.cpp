//============================================================
//
//	テクスチャ処理 [texture.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	マクロ定義
//************************************************************
#define TEXTURE_SETUP_TXT	"data\\TXT\\texture.txt"	// セットアップテキスト相対パス

//************************************************************
//	静的メンバ変数宣言
//************************************************************
int CTexture::m_nNumAll = 0;	// テクスチャの総数

//************************************************************
//	親クラス [CTexture] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTexture::CTexture()
{
	// テクスチャへのポインタをクリア
	memset(&m_apTexture[0], 0, sizeof(m_apTexture));

	// 読み込んだテクスチャファイル名をクリア
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{ // テクスチャの最大数分繰り返す

		// NULL文字をコピー
		strcpy(&m_pFileName[nCntTexture][0], NONE_STRING);
	}
}

//============================================================
//	デストラクタ
//============================================================
CTexture::~CTexture()
{

}

//============================================================
//	テクスチャ生成処理
//============================================================
HRESULT CTexture::Load(void)
{
	// テクスチャへのポインタを初期化
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{ // テクスチャの最大数分繰り返す

		// NULLを代入
		m_apTexture[nCntTexture] = NULL;
	}

	// 読み込んだテクスチャファイル名を初期化
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{ // テクスチャの最大数分繰り返す

		// NULL文字をコピー
		strcpy(&m_pFileName[nCntTexture][0], NONE_STRING);
	}

	// セットアップの読込
	LoadSetup();

	// 成功を返す
	return S_OK;
}

//============================================================
//	テクスチャ破棄処理
//============================================================
void CTexture::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{ // テクスチャの最大数分繰り返す

		// テクスチャの破棄
		if (m_apTexture[nCntTexture] != NULL)
		{ // テクスチャが使用中の場合

			// メモリ開放
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}
}

//============================================================
//	テクスチャ登録処理
//============================================================
int CTexture::Regist(const char *pFileName)
{
	// 変数を宣言
	int nID = m_nNumAll;	// テクスチャ読込番号

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	if (pFileName != NULL)
	{ // ポインタが使用されている場合

		for (int nCntTexture = 0; nCntTexture < m_nNumAll; nCntTexture++)
		{ // テクスチャの総数分繰り返す

			if (strcmp(&m_pFileName[nCntTexture][0], pFileName) == 0)
			{ // 文字列が一致した場合

				// すでに読み込んでいるテクスチャの配列番号を返す
				return nCntTexture;
			}
		}

		if (m_nNumAll >= MAX_TEXTURE)
		{ // テクスチャオーバーの場合

			// 失敗を返す
			assert(false);
			return NONE_IDX;
		}

		// テクスチャの読み込み
		if (FAILED(D3DXCreateTextureFromFile(pDevice, pFileName, &m_apTexture[nID])))
		{ // テクスチャの読み込みに失敗した場合

			// 失敗を返す
			assert(false);
			return NONE_IDX;
		}

		// テクスチャのファイル名を保存
		strcpy(&m_pFileName[nID][0], pFileName);

		// テクスチャの総数を加算
		m_nNumAll++;

		// 読み込んだテクスチャの配列番号を返す
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
//	テクスチャ情報の取得処理
//============================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(int nID)
{
	if (nID >= 0 && nID < m_nNumAll)
	{ // 引数のインデックスが範囲内の場合

		// 引数のテクスチャポインタを返す
		return m_apTexture[nID];
	}
	else if (nID == NONE_IDX)
	{ // 引数のインデックスが -1の場合

		// NULLを返す
		return NULL;
	}
	else
	{ // 引数のインデックスが使用不可の場合

		// NULLを返す
		assert(false);
		return NULL;
	}
}

//============================================================
//	生成処理
//============================================================
CTexture *CTexture::Create(void)
{
	// ポインタを宣言
	CTexture *pTexture = NULL;	// テクスチャ生成用

	if (pTexture == NULL)
	{ // 使用されていない場合

		// メモリを確保
		pTexture = new CTexture;	// テクスチャ
	}
	else { assert(false); return NULL; }	// 使用中

	if (pTexture != NULL)
	{ // 確保に成功している場合

		// テクスチャの読込
		if (FAILED(pTexture->Load()))
		{ // テクスチャ読み込みに失敗した場合

			// メモリ開放
			delete pTexture;
			pTexture = NULL;

			// 失敗を返す
			assert(false);
			return NULL;
		}

		// 確保したアドレスを返す
		return pTexture;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CTexture::Release(CTexture *&prTexture)
{
	if (prTexture != NULL)
	{ // 使用中の場合

		// テクスチャの破棄
		prTexture->Unload();

		// メモリ開放
		delete prTexture;
		prTexture = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	セットアップ処理
//============================================================
void CTexture::LoadSetup(void)
{
	// 変数を宣言
	int nEnd = 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(TEXTURE_SETUP_TXT, "r");

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

				// テクスチャを登録
				Regist(&aString[0]);
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "テクスチャセットアップファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
