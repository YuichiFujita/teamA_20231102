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
#include "sound.h"
#include "ModelParticle.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\TXT\\obstacle.txt";		// 障害物セットアップテキスト
	const char* SAVE_TXT = "data\\TXT\\save_obstacle.txt";	// 障害物セーブテキスト
	const int	PRIORITY = 1;	// 障害物の優先順位
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CObstacle::SStatusInfo CObstacle::m_aStatusInfo[TYPE_MAX] = {};	// ステータス情報
const char *CObstacle::mc_apModelFile[] =	// モデル定数
{
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
	"data\\MODEL\\OBSTACLE\\Fountain000.x",					// 噴水モデル
	"data\\MODEL\\OBSTACLE\\Tower000.x",					// 塔モデル
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
	m_type	= TYPE_CONIFER;	// 種類
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
	m_type	= TYPE_CONIFER;	// 種類
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
//	破壊状況の設定処理
//============================================================
void CObstacle::SetState(const int nState)
{
	// 破壊状況を設定
	m_status.state = (EBreak)nState;
}

//============================================================
//	破壊状況取得処理
//============================================================
int CObstacle::GetState(void) const
{
	// 破壊状況を返す
	return m_status.state;
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
//	ステータス情報設定処理
//============================================================
void CObstacle::SetStatusInfo(const int nID, const SStatusInfo status)
{
	// 引数インデックスのステータスを設定
	m_aStatusInfo[nID] = status;
}

//============================================================
//	ステータス情報取得処理
//============================================================
CObstacle::SStatusInfo CObstacle::GetStatusInfo(const int nID)
{
	// 引数インデックスのステータスを返す
	return m_aStatusInfo[nID];
}

//============================================================
//	ステータス保存処理
//============================================================
void CObstacle::SaveStatus(void)
{
#if _DEBUG

	// ポインタを宣言
	FILE *pFile = NULL;	// ファイルポインタ

	// ファイルを書き出し形式で開く
	pFile = fopen(SAVE_TXT, "w");

	if (pFile != NULL)
	{ // ファイルが存在する場合

		// 見出しを書き出し
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#	障害物セーブテキスト [save_obstacle.txt]\n");
		fprintf(pFile, "#	Author : you\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "---------->--<---------- ここから下を コピーし貼り付け ---------->--<----------\n\n");

		// 情報開始地点を書き出し
		fprintf(pFile, "STATUSSET\n\n");

		for (int nCntObs = 0; nCntObs < MAX_PRIO; nCntObs++)
		{ // 優先順位の総数分繰り返す
	
			// 情報を書き出し
			fprintf(pFile, "	OBSTACLESET\n");
			fprintf(pFile, "		TYPE = %d\n", nCntObs);
			fprintf(pFile, "		BREAK = %d\n", m_aStatusInfo[nCntObs].state);
			fprintf(pFile, "		LIFE = %d\n", m_aStatusInfo[nCntObs].nLife);
			fprintf(pFile, "		LENGTH_CENTER = %.2f\n", m_aStatusInfo[nCntObs].fLengthCenter);
			fprintf(pFile, "		ANGLE_CENTER = %.2f\n",  m_aStatusInfo[nCntObs].fAngleCenter);
			fprintf(pFile, "		SIZE_COLL = %.2f %.2f %.2f\n", m_aStatusInfo[nCntObs].sizeColl.x, m_aStatusInfo[nCntObs].sizeColl.y, m_aStatusInfo[nCntObs].sizeColl.z);
			fprintf(pFile, "	END_OBSTACLESET\n\n");
		}

		// 情報終了地点を書き出し
		fprintf(pFile, "END_STATUSSET\n\n");

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "障害物セーブファイルの書き出しに失敗！", "警告！", MB_ICONWARNING);
	}

#endif	// _DEBUG
}
//============================================================
//	当たり判定処理
//============================================================
void CObstacle::Hit(void)
{
	if (IsDeath())
	{ // 死亡フラグが立っている場合

		return;
	}

	//破壊できるブロックだったら
	if (m_status.state == EBreak::BREAK_TRUE)
	{
		CModParticle::Create(D3DXVECTOR3(GetVec3Position().x, GetVec3Position().y + 50.0f,
			GetVec3Position().z), GetMaterial(0).MatD3D.Diffuse, CManager::GetInstance()->GetTexture()->Regist(GetMaterial(0).pTextureFilename));
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT2);	// ヒット音

		//終了処理を行う
		Uninit();
	}
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
							else if (strcmp(&aString[0], "LENGTH_CENTER") == 0)
							{ // 読み込んだ文字列が LENGTH_CENTER の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fLengthCenter);	// 判定中心位置の距離を読み込む
							}
							else if (strcmp(&aString[0], "ANGLE_CENTER") == 0)
							{ // 読み込んだ文字列が ANGLE_CENTER の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fAngleCenter);	// 判定中心位置の角度を読み込む
							}
							else if (strcmp(&aString[0], "SIZE_COLL") == 0)
							{ // 読み込んだ文字列が SIZE_COLL の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].sizeColl.x);	// 判定大きさXを読み込む
								fscanf(pFile, "%f", &m_aStatusInfo[nType].sizeColl.y);	// 判定大きさYを読み込む
								fscanf(pFile, "%f", &m_aStatusInfo[nType].sizeColl.z);	// 判定大きさZを読み込む
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
