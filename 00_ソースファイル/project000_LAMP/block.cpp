//============================================================
//
//	ブロック処理 [block.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "block.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\TXT\\player.txt";	// プレイヤーセットアップテキスト
	const int	PRIORITY = 1;	// プレイヤーの優先順位
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CBlock::SStatusInfo CBlock::m_aStatusInfo[TYPE_MAX] = {};	// ステータス情報
const char *CBlock::mc_apTextureFile[][6] =	// テクスチャ定数
{
	{ // 石テクスチャ
		"data\\TEXTURE\\block000.png",	// 左テクスチャ
		"data\\TEXTURE\\block000.png",	// 右テクスチャ
		"data\\TEXTURE\\block000.png",	// 下テクスチャ
		"data\\TEXTURE\\block000.png",	// 上テクスチャ
		"data\\TEXTURE\\block000.png",	// 前テクスチャ
		"data\\TEXTURE\\block000.png",	// 後テクスチャ
	},

	{ // 箱テクスチャ
		"data\\TEXTURE\\block001.png",	// 左テクスチャ
		"data\\TEXTURE\\block001.png",	// 右テクスチャ
		"data\\TEXTURE\\block001.png",	// 下テクスチャ
		"data\\TEXTURE\\block001.png",	// 上テクスチャ
		"data\\TEXTURE\\block001.png",	// 前テクスチャ
		"data\\TEXTURE\\block001.png",	// 後テクスチャ
	},
};

//************************************************************
//	子クラス [CBlock] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBlock::CBlock() : CObjectMeshCube(CObject::LABEL_BLOCK, PRIORITY)
{
	// メンバ変数をクリア
	memset(&m_status, 0, sizeof(m_status));	// ステータス
	m_type	= TYPE_STONE;	// 種類
	m_nLife	= 0;			// 体力
}

//============================================================
//	デストラクタ
//============================================================
CBlock::~CBlock()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBlock::Init(void)
{
	// メンバ変数を初期化
	memset(&m_status, 0, sizeof(m_status));	// ステータス
	m_type = TYPE_STONE;	// 種類
	m_nLife = 0;			// 体力

	// オブジェクトメッシュキューブの初期化
	if (FAILED(CObjectMeshCube::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 原点を設定
	SetOrigin(CObjectMeshCube::ORIGIN_DOWN);

	// キューブ色を設定
	SetColor(XCOL_WHITE);

	// 縁取り色を設定
	SetBorderColor(XCOL_WHITE);

	// 縁取りの状態を設定
	if (FAILED(SetBorderState(CObjectMeshCube::BORDER_OFF)))
	{ // 分割数の設定に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 縁取りの太さを設定
	SetBorderThick(0.0f);

	// テクスチャの状態を設定
	SetTextureState(CObjectMeshCube::TEXSTATE_SELECT);

	// テクスチャの分割数Xを設定
	SetTexturePatternX(VEC2_ONE);

	// テクスチャの分割数Yを設定
	SetTexturePatternY(VEC2_ONE);

	// テクスチャの分割数Zを設定
	SetTexturePatternZ(VEC2_ONE);

	// カリングを設定
	SetCulling(D3DCULL_CCW);

	// ライティングを設定
	SetLighting(true);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CBlock::Uninit(void)
{
	// オブジェクトメッシュキューブの終了
	CObjectMeshCube::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CBlock::Update(void)
{
	// オブジェクトメッシュキューブの更新
	CObjectMeshCube::Update();
}

//============================================================
//	描画処理
//============================================================
void CBlock::Draw(void)
{
	// オブジェクトメッシュキューブの描画
	CObjectMeshCube::Draw();
}

//============================================================
//	種類の設定処理
//============================================================
void CBlock::SetType(const int nType)
{
	// 変数を宣言
	SFaceTex faceTex;	// テクスチャインデックス設定用

	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	if (pTexture == NULL)
	{ // 使用されていない場合

		// 処理を抜ける
		assert(false);
		return;
	}

	if (nType > NONE_IDX && nType < TYPE_MAX)
	{ // 種類が範囲内の場合

		// 引数の種類を設定
		m_type = (EType)nType;

		// 引数の種類のステータス情報を設定
		m_status = m_aStatusInfo[nType];

		// 引数の種類の体力を設定
		m_nLife = m_status.nLife;

		// 引数の種類のテクスチャを登録
		faceTex = SFaceTex
		( // 引数
			pTexture->Regist(mc_apTextureFile[nType][0]),	// 左
			pTexture->Regist(mc_apTextureFile[nType][1]),	// 右
			pTexture->Regist(mc_apTextureFile[nType][2]),	// 下
			pTexture->Regist(mc_apTextureFile[nType][3]),	// 上
			pTexture->Regist(mc_apTextureFile[nType][4]),	// 前
			pTexture->Regist(mc_apTextureFile[nType][5])	// 後
		);
		BindTexture(faceTex);	// テクスチャを割当
	}
	else { assert(false); }	// 種類オーバー
}

//============================================================
//	種類取得処理
//============================================================
int CBlock::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//============================================================
//	生成処理
//============================================================
CBlock *CBlock::Create
(
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR3& rSize	// 大きさ
)
{
	// ポインタを宣言
	CBlock *pBlock = NULL;	// ブロック生成用

	if (pBlock == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pBlock = new CBlock;	// ブロック
	}
	else { assert(false); return NULL; }	// 使用中

	if (pBlock != NULL)
	{ // 確保に成功している場合

		// ブロックの初期化
		if (FAILED(pBlock->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pBlock;
			pBlock = NULL;

			// 失敗を返す
			return NULL;
		}

		// 種類を設定
		pBlock->SetType(type);

		// 位置を設定
		pBlock->SetVec3Position(rPos);

		// 向きを設定
		pBlock->SetVec3Rotation(rRot);

		// 大きさを設定
		pBlock->SetVec3Sizing(rSize);

		// 確保したアドレスを返す
		return pBlock;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	セットアップ処理
//============================================================
void CBlock::LoadSetup(void)
{
	// 変数を宣言
	int nType = 0;	// 種類の代入用
	int nEnd = 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ステータス情報の静的メンバ変数を初期化
	memset(&m_aStatusInfo[0], 0, sizeof(m_aStatusInfo));

	// ファイルを読み込み形式で開く
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// ステータスの設定
			if (strcmp(&aString[0], "STATUSSET") == 0)
			{ // 読み込んだ文字列が STATUSSET の場合

				do
				{ // 読み込んだ文字列が END_STATUSSET ではない場合ループ

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
								fscanf(pFile, "%d", &nType);		// 種類を読み込む
							}
							else if (strcmp(&aString[0], "BREAK") == 0)
							{ // 読み込んだ文字列が BREAK の場合

								fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].state);	// 破壊状況を読み込む
							}
							else if (strcmp(&aString[0], "LIFE") == 0)
							{ // 読み込んだ文字列が LIFE の場合

								fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nLife);	// 破壊状況を読み込む
							}
						} while (strcmp(&aString[0], "END_BLOCKSET") != 0);	// 読み込んだ文字列が END_BLOCKSET ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_STATUSSET") != 0);	// 読み込んだ文字列が END_STATUSSET ではない場合ループ
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ブロックセットアップファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
