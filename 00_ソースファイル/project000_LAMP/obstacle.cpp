//============================================================
//
//	障害物処理 [obstacle.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "obstacle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\TXT\\obstacle.txt";	// 障害物セットアップテキスト
	const int	PRIORITY = 1;	// 障害物の優先順位
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CObstacle::SStatusInfo CObstacle::m_aStatusInfo[TYPE_MAX] = {};	// ステータス情報
const char *CObstacle::mc_apModelFile[] =	// モデル定数
{
	"data\\MODEL\\OBSTACLE\\obstacle000.x",					// 木モデル
	"data\\MODEL\\OBSTACLE\\Conifer000.x",					// 針葉樹モデル
	"data\\MODEL\\OBSTACLE\\Hardwood000.x",					// 広葉樹モデル
	"data\\MODEL\\OBSTACLE\\Jar000.x",						// 壺モデル(エルデンリング風)
	"data\\MODEL\\OBSTACLE\\Jar001.x",						// 壺モデル(ゼルダ風)
	"data\\MODEL\\OBSTACLE\\Rock000-V2.x",					// 岩モデル
	"data\\MODEL\\OBSTACLE\\Rock001-V2.x",					// ゆがんだ岩モデル
	"data\\MODEL\\OBSTACLE\\Rock(Cracked)000-V2.x",			// ひび割れた岩モデル
	"data\\MODEL\\OBSTACLE\\StoneBrickPiller000(Other2).x",	// 石レンガの柱
	"data\\MODEL\\OBSTACLE\\well000.x",						// 井戸モデル
	"data\\MODEL\\OBSTACLE\\Wall000(Other).x",				// 丸屋根の壁モデル
	"data\\MODEL\\OBSTACLE\\Wall000(Other)[Flimsy].x",		// もろい丸屋根の壁モデル
	"data\\MODEL\\OBSTACLE\\Wall001(Other).x",				// 三角屋根の壁モデル
	"data\\MODEL\\OBSTACLE\\Wall001(Other)[Flimsy].x",		// もろい三角屋根の壁モデル
	"data\\MODEL\\OBSTACLE\\Castle000.x",					// 城モデル
	"data\\MODEL\\OBSTACLE\\Castle001.x",					// カステル・デル・モンテ風の城モデル
	"data\\MODEL\\OBSTACLE\\Castle-Fence000.x",				// 城の柵モデル1
	"data\\MODEL\\OBSTACLE\\Castle-Fence001.x",				// 城の柵モデル2
};															   

//************************************************************
//	子クラス [CObstacle] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObstacle::CObstacle() : CObjectModel(CObject::LABEL_OBSTACLE, PRIORITY)
{
	// メンバ変数をクリア
	memset(&m_status, 0, sizeof(m_status));	// ステータス
	m_type	= TYPE_WOOD;	// 種類
	m_nLife	= 0;			// 体力
}

//============================================================
//	デストラクタ
//============================================================
CObstacle::~CObstacle()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObstacle::Init(void)
{
	// メンバ変数を初期化
	memset(&m_status, 0, sizeof(m_status));	// ステータス
	m_type	= TYPE_WOOD;	// 種類
	m_nLife	= 0;			// 体力

	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObstacle::Uninit(void)
{
	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CObstacle::Update(void)
{
	// オブジェクトモデルの更新
	CObjectModel::Update();
}

//============================================================
//	描画処理
//============================================================
void CObstacle::Draw(void)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw();
}

//============================================================
//	種類の設定処理
//============================================================
void CObstacle::SetType(const int nType)
{
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

		// 引数の種類のモデルを登録・割当
		BindModel(mc_apModelFile[nType]);
	}
	else { assert(false); }	// 種類オーバー
}

//============================================================
//	種類取得処理
//============================================================
int CObstacle::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//============================================================
//	生成処理
//============================================================
CObstacle *CObstacle::Create
(
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot		// 向き
)
{
	// ポインタを宣言
	CObstacle *pObstacle = NULL;	// 障害物生成用

	if (pObstacle == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pObstacle = new CObstacle;	// 障害物
	}
	else { assert(false); return NULL; }	// 使用中

	if (pObstacle != NULL)
	{ // 確保に成功している場合

		// 障害物の初期化
		if (FAILED(pObstacle->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObstacle;
			pObstacle = NULL;

			// 失敗を返す
			return NULL;
		}

		// 種類を設定
		pObstacle->SetType(type);

		// 位置を設定
		pObstacle->SetVec3Position(rPos);

		// 向きを設定
		pObstacle->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pObstacle;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	セットアップ処理
//============================================================
void CObstacle::LoadSetup(void)
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

					if (strcmp(&aString[0], "OBSTACLESET") == 0)
					{ // 読み込んだ文字列が OBSTACLESET の場合

						do
						{ // 読み込んだ文字列が END_OBSTACLESET ではない場合ループ

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
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nLife);	// 体力を読み込む
							}
						} while (strcmp(&aString[0], "END_OBSTACLESET") != 0);	// 読み込んだ文字列が END_OBSTACLESET ではない場合ループ
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
		MessageBox(NULL, "障害物セットアップファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
