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
#include "sound.h"
#include "ModelParticle.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\TXT\\block.txt";	// ブロックセットアップテキスト

#ifdef ON_STENCIL_PLAYER

	const int	PRIORITY = 4;	// ブロックの優先順位

#else	// OFF_STENCIL_PLAYER

	const int	PRIORITY = 1;	// ブロックの優先順位

#endif	// ON_STENCIL_PLAYER
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CBlock::SStatusInfo CBlock::m_aStatusInfo[TYPE_MAX] = {};	// ステータス情報
const char *CBlock::mc_apTextureFile[][6] =	// テクスチャ定数
{
	{ // 石テクスチャ
		"data\\TEXTURE\\stone_001.png",	// 左テクスチャ
		"data\\TEXTURE\\stone_001.png",	// 右テクスチャ
		"data\\TEXTURE\\stone_001.png",	// 下テクスチャ
		"data\\TEXTURE\\stone_001.png",	// 上テクスチャ
		"data\\TEXTURE\\stone_001.png",	// 前テクスチャ
		"data\\TEXTURE\\stone_001.png",	// 後テクスチャ
	},

	{ // 箱テクスチャ
		"data\\TEXTURE\\block001.png",	// 左テクスチャ
		"data\\TEXTURE\\block001.png",	// 右テクスチャ
		"data\\TEXTURE\\block001.png",	// 下テクスチャ
		"data\\TEXTURE\\block001.png",	// 上テクスチャ
		"data\\TEXTURE\\block001.png",	// 前テクスチャ
		"data\\TEXTURE\\block001.png",	// 後テクスチャ
	},

	{ // レンガテクスチャ
		"data\\TEXTURE\\Brick.png",	// 左テクスチャ
		"data\\TEXTURE\\Brick.png",	// 右テクスチャ
		"data\\TEXTURE\\Brick.png",	// 下テクスチャ
		"data\\TEXTURE\\Brick.png",	// 上テクスチャ
		"data\\TEXTURE\\Brick.png",	// 前テクスチャ
		"data\\TEXTURE\\Brick.png",	// 後テクスチャ
	},

	{ // コンクリートテクスチャ
		"data\\TEXTURE\\Concrete000.png",	// 左テクスチャ
		"data\\TEXTURE\\Concrete000.png",	// 右テクスチャ
		"data\\TEXTURE\\Concrete000.png",	// 下テクスチャ
		"data\\TEXTURE\\Concrete000.png",	// 上テクスチャ
		"data\\TEXTURE\\Concrete000.png",	// 前テクスチャ
		"data\\TEXTURE\\Concrete000.png",	// 後テクスチャ
	},

	{ // 氷テクスチャ
		"data\\TEXTURE\\iceblock.png",	// 左テクスチャ
		"data\\TEXTURE\\iceblock.png",	// 右テクスチャ
		"data\\TEXTURE\\iceblock.png",	// 下テクスチャ
		"data\\TEXTURE\\iceblock.png",	// 上テクスチャ
		"data\\TEXTURE\\iceblock.png",	// 前テクスチャ
		"data\\TEXTURE\\iceblock.png",	// 後テクスチャ
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
	m_type	= TYPE_STONE;	// 種類
	m_nLife	= 0;			// 体力

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
	/*
		バックバッファ書き込み用の描画
	*/
	// オブジェクトメッシュキューブの描画
	CObjectMeshCube::Draw();

#ifdef ON_STENCIL_PLAYER

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	/*
		ステンシルバッファ書き込み用の描画
	*/
	// ステンシルテストを有効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// 比較参照値を設定する
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);

	// ステンシルマスクを指定する 
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	// ステンシル比較関数を指定する
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	// ステンシル結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS,	D3DSTENCILOP_INCRSAT);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL,	D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL,	D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// オブジェクトメッシュキューブの描画
	CObjectMeshCube::Draw();

	// ステンシルテストを無効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

#endif	// ON_STENCIL_PLAYER
}
//============================================================
//	当たり判定処理
//============================================================
void CBlock::Hit(void)
{
	if (IsDeath())
	{ // 死亡フラグが立っている場合

		return;
	}
	
	//破壊できるブロックだったら
	if (m_status.state == EBreak::BREAK_TRUE)
	{
		// WATANABE：ビルドすると未解決の外部参照って言われたからコメントアウトしておきました
		CModParticle::Create(D3DXVECTOR3(CObjectMeshCube::GetVec3Position().x, CObjectMeshCube::GetVec3Position().y + 50.0f,
			CObjectMeshCube::GetVec3Position().z), GetColor(), GetMeshCube().texID.Top);
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT2);	// ヒット音

		//終了処理を行う
		Uninit();
	}
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
//	破壊状況の設定処理
//============================================================
void CBlock::SetState(const int nState)
{
	// 破壊状況を設定
	m_status.state = (EBreak)nState;
}

//============================================================
//	破壊状況取得処理
//============================================================
int CBlock::GetState(void) const
{
	// 破壊状況を返す
	return m_status.state;
}

//============================================================
//	生成処理
//============================================================
CBlock *CBlock::Create
(
	const EType type,				// 種類
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rRot,		// 向き
	const D3DXVECTOR3& rSize,		// 大きさ
	const D3DXVECTOR2& rTexPartX,	// テクスチャ分割数X
	const D3DXVECTOR2& rTexPartY,	// テクスチャ分割数Y
	const D3DXVECTOR2& rTexPartZ	// テクスチャ分割数Z
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

		// テクスチャの分割数Xを設定
		pBlock->SetTexturePatternX(rTexPartX);

		// テクスチャの分割数Yを設定
		pBlock->SetTexturePatternY(rTexPartY);

		// テクスチャの分割数Zを設定
		pBlock->SetTexturePatternZ(rTexPartZ);

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
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nLife);	// 体力を読み込む
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
