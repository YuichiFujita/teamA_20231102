//============================================================
//
//	数字処理 [value.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "value.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	マクロ定義
//************************************************************
#define VAL_PRIO	(13)	// 数字の優先順位
#define PTRN_WIDTH	(10)	// テクスチャの横の分割数
#define PTRN_HEIGHT	(1)		// テクスチャの縦の分割数

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CValue::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\number000.png",	// 通常テクスチャ
};

//************************************************************
//	子クラス [CValue] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CValue::CValue() : CAnim2D(CObject::LABEL_NONE, VAL_PRIO)
{
	// メンバ変数をクリア
	m_nNum = 0;	// 表示数値
}

//============================================================
//	デストラクタ
//============================================================
CValue::~CValue()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CValue::Init(void)
{
	// メンバ変数を初期化
	m_nNum = 0;	// 表示数値

	// アニメーション2Dの初期化
	if (FAILED(CAnim2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャ横分割数の設定
	CAnim2D::SetWidthPattern(PTRN_WIDTH);

	// テクスチャ縦分割数の設定
	CAnim2D::SetHeightPattern(PTRN_HEIGHT);

	// 停止状況の設定
	CAnim2D::SetEnableStop(true);	// アニメーションしない

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CValue::Uninit(void)
{
	// アニメーション2Dの終了
	CAnim2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CValue::Update(void)
{
	// アニメーション2Dの更新
	CAnim2D::Update();
}

//============================================================
//	描画処理
//============================================================
void CValue::Draw(void)
{
	// アニメーション2Dの描画
	CAnim2D::Draw();
}

//============================================================
//	生成処理
//============================================================
CValue *CValue::Create(const ETexture texture)
{
	// ポインタを宣言
	CValue *pValue = NULL;		// 数字生成用

	if (pValue == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pValue = new CValue;	// 数字
	}
	else { assert(false); return NULL; }	// 使用中

	if (pValue != NULL)
	{ // 確保に成功している場合

		// 数字の初期化
		if (FAILED(pValue->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pValue;
			pValue = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャを設定
		pValue->SetTexture(texture);

		// 確保したアドレスを返す
		return pValue;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	生成処理
//============================================================
CValue *CValue::Create(const ETexture texture, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXVECTOR3& rRot, const D3DXCOLOR& rCol)
{
	// ポインタを宣言
	CValue *pValue = NULL;		// 数字生成用

	if (pValue == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pValue = new CValue;	// 数字
	}
	else { assert(false); return NULL; }	// 使用中

	if (pValue != NULL)
	{ // 確保に成功している場合

		// 数字の初期化
		if (FAILED(pValue->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pValue;
			pValue = NULL;

			// 失敗を返す
			return NULL;
		}

		// テクスチャを設定
		pValue->SetTexture(texture);

		// 位置を設定
		pValue->SetVec3Position(rPos);

		// 向きを設定
		pValue->SetVec3Rotation(rRot);

		// 大きさを設定
		pValue->SetVec3Sizing(rSize);

		// 色を設定
		pValue->SetColor(rCol);

		// 確保したアドレスを返す
		return pValue;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	テクスチャの設定処理
//============================================================
void CValue::SetTexture(const ETexture texture)
{
	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ

	// テクスチャを登録・割当
	BindTexture(pTexture->Regist(mc_apTextureFile[texture]));
}

//============================================================
//	値の設定処理
//============================================================
void CValue::SetNumber(const int nNum)
{
	// 引数の値を代入
	m_nNum = nNum;

	// パターンの設定
	CAnim2D::SetPattern(m_nNum);
}
