//============================================================
//
//	マルチ数字処理 [multiValue.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "multiValue.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	子クラス [CMultiValue] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CMultiValue::CMultiValue() : CObject(CObject::LABEL_NUMBER, DEFAULT_PRIO)
{
	// メンバ変数をクリア
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	m_pos	= VEC3_ZERO;	// 位置
	m_space	= VEC3_ZERO;	// 数値の行間
	m_nNum		= 0;		// 数字
	m_nDigit	= 0;		// 桁数
	m_nMin		= 0;		// 最小値
	m_nMax		= 0;		// 最大値
}

//============================================================
//	デストラクタ
//============================================================
CMultiValue::~CMultiValue()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CMultiValue::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	m_pos	= VEC3_ZERO;	// 位置
	m_space	= VEC3_ZERO;	// 数値の行間
	m_nNum		= 0;		// 数字
	m_nDigit	= 0;		// 桁数
	m_nMin		= 0;		// 最小値
	m_nMax		= 0;		// 最大値

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CMultiValue::Uninit(void)
{
	// 数字オブジェクトを破棄
	for (int nCntValue = 0; nCntValue < MAX_DIGIT; nCntValue++)
	{ // 桁数の最大数分繰り返す

		if (m_apValue[nCntValue] != NULL)
		{ // 使用されている場合

			// 数字の終了
			m_apValue[nCntValue]->Uninit();
		}
	}

	// 自身のオブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CMultiValue::Update(void)
{
	for (int nCntValue = 0; nCntValue < MAX_DIGIT; nCntValue++)
	{ // 桁数の最大数分繰り返す

		if (m_apValue[nCntValue] != NULL)
		{ // 使用されている場合

			// 数字の更新
			m_apValue[nCntValue]->Update();
		}
	}
}

//============================================================
//	描画処理
//============================================================
void CMultiValue::Draw(void)
{

}

//============================================================
//	描画状況の設定処理
//============================================================
void CMultiValue::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);	// 自身

	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // 桁数分繰り返す

		// 引数の描画状況を設定
		m_apValue[nCntValue]->SetEnableDraw(bDraw);	// 数字
	}
}

//============================================================
//	優先順位の設定処理
//============================================================
void CMultiValue::SetPriority(const int nPriority)
{
	// 引数の優先順位を設定
	CObject::SetPriority(nPriority);	// 自身

	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // 桁数分繰り返す

		// 引数の優先順位を設定
		m_apValue[nCntValue]->SetPriority(nPriority);	// 数字
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CMultiValue::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;

	// 数字位置の設定
	SetNumPosition();
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CMultiValue::GetVec3Position(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	向きの設定処理
//============================================================
void CMultiValue::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // 桁数分繰り返す

		// 引数の向きを全ての数字に設定
		m_apValue[nCntValue]->SetVec3Rotation(rRot);
	}
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CMultiValue::GetVec3Rotation(void) const
{
	// 例外処理
	assert(m_apValue[0] != NULL);	// 非使用チェック

	// 先頭数字の向きを返す
	return m_apValue[0]->GetVec3Rotation();
}

//============================================================
//	大きさの設定処理
//============================================================
void CMultiValue::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // 桁数分繰り返す

		// 引数の大きさを全ての数字に設定
		m_apValue[nCntValue]->SetVec3Sizing(rSize);
	}
}

//============================================================
//	大きさ取得処理
//============================================================
D3DXVECTOR3 CMultiValue::GetVec3Sizing(void) const
{
	// 例外処理
	assert(m_apValue[0] != NULL);	// 非使用チェック

	// 先頭数字の大きさを返す
	return m_apValue[0]->GetVec3Sizing();
}

//============================================================
//	色の設定処理
//============================================================
void CMultiValue::SetColor(const D3DXCOLOR& rCol)
{
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // 桁数分繰り返す

		// 引数の色を全ての数字に設定
		m_apValue[nCntValue]->SetColor(rCol);
	}
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CMultiValue::GetColor(void) const
{
	// 例外処理
	assert(m_apValue[0] != NULL);	// 非使用チェック

	// 先頭数字の色を返す
	return m_apValue[0]->GetColor();
}

//============================================================
//	生成処理
//============================================================
CMultiValue *CMultiValue::Create
(
	const CValue::ETexture texture,	// テクスチャ
	const int nNum,					// 数字
	const int nDigit,				// 桁数
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rSize,		// 大きさ
	const D3DXVECTOR3& rSpace,		// 行間
	const D3DXVECTOR3& rRot,		// 向き
	const D3DXCOLOR& rCol			// 色
)
{
	// ポインタを宣言
	CMultiValue *pMultiValue = NULL;	// マルチ数字生成用

	if (pMultiValue == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pMultiValue = new CMultiValue;	// マルチ数字
	}
	else { assert(false); return NULL; }	// 使用中

	if (pMultiValue != NULL)
	{ // 確保に成功している場合

		// マルチ数字の初期化
		if (FAILED(pMultiValue->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pMultiValue;
			pMultiValue = NULL;

			// 失敗を返す
			return NULL;
		}

		// 桁数を設定
		pMultiValue->SetDigit(nDigit);

		// 数字を設定
		pMultiValue->SetNum(nNum);

		// テクスチャを設定
		pMultiValue->SetTexture(texture);

		// 向きを設定
		pMultiValue->SetVec3Rotation(rRot);

		// 大きさを設定
		pMultiValue->SetVec3Sizing(rSize);

		// 色を設定
		pMultiValue->SetColor(rCol);

		// 行間を設定
		pMultiValue->SetSpace(rSpace);

		// 位置を設定
		pMultiValue->SetVec3Position(rPos);

		// 確保したアドレスを返す
		return pMultiValue;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	数値の加算処理
//============================================================
void CMultiValue::AddNum(const int nNum)
{
	// 数字を引数分加算
	m_nNum += nNum;

	// 数字の補正
	useful::LimitNum(m_nNum, m_nMin, m_nMax);

	// 数字のテクスチャ座標の設定
	SetTexNum();
}

//============================================================
//	数値の設定処理
//============================================================
void CMultiValue::SetNum(const int nNum)
{
	// 数字を引数の値に設定
	m_nNum = nNum;

	// 数字の補正
	useful::LimitNum(m_nNum, m_nMin, m_nMax);

	// 数字のテクスチャ座標の設定
	SetTexNum();
}

//============================================================
//	桁数の設定処理
//============================================================
HRESULT CMultiValue::SetDigit(const int nDigit)
{
	// 変数を宣言
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向き
	D3DXVECTOR3 size = VEC3_ONE;	// 大きさ
	D3DXCOLOR col = XCOL_WHITE;		// 色
	int nLimit = 1;					// 最大値の計算用

	// 例外処理
	assert(nDigit > 0);	// 桁数違反

	// 引数の桁数を設定
	m_nDigit = nDigit;

	if (m_apValue[0] != NULL)
	{ // 先頭が使用されている場合

		// 現在の設定を取得
		rot = GetVec3Rotation();	// 向き
		size = GetVec3Sizing();		// 大きさ
		col = GetColor();			// 色
	}

	//--------------------------------------------------------
	//	数字オブジェクトの破棄・生成
	//--------------------------------------------------------
	// 数字オブジェクトの破棄
	for (int nCntValue = 0; nCntValue < MAX_DIGIT; nCntValue++)
	{ // 桁数の最大数分繰り返す

		if (m_apValue[nCntValue] != NULL)
		{ // 使用されている場合

			// 数字の終了
			m_apValue[nCntValue]->Uninit();
		}
	}

	// 数字オブジェクトの生成
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // 桁数分繰り返す

		// 数字の生成
		m_apValue[nCntValue] = CValue::Create(CValue::TEXTURE_NORMAL);
		if (m_apValue[nCntValue] == NULL)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	//--------------------------------------------------------
	//	基本情報の再設定
	//--------------------------------------------------------
	// 向きを設定
	SetVec3Rotation(rot);

	// 大きさを設定
	SetVec3Sizing(size);

	// 色を設定
	SetColor(col);

	// 数字位置の設定
	SetNumPosition();

	//--------------------------------------------------------
	//	制限値の設定
	//--------------------------------------------------------
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // 桁数分繰り返す

		// 桁数を増やす
		nLimit *= 10;
	}

	// 最小値の設定
	SetMin(0);

	// 最大値の設定
	SetMax(nLimit - 1);

	// 成功を返す
	return S_OK;
}

//============================================================
//	最小値の設定処理
//============================================================
void CMultiValue::SetMin(const int nMin)
{
	// 例外処理
	assert(nMin >= 0 && nMin <= m_nMax);

	// 引数の最小値を設定
	m_nMin = nMin;

	// 数字の補正
	useful::LimitNum(m_nNum, m_nMin, m_nMax);
}

//============================================================
//	最大値の設定処理
//============================================================
void CMultiValue::SetMax(const int nMax)
{
	// 変数を宣言
	int nLimit = 1;	// 例外処理の最大値計算用

	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // 桁数分繰り返す

		// 桁数を増やす
		nLimit *= 10;
	}

	// 例外処理
	assert(nMax <= nLimit - 1 && nMax >= m_nMin);

	// 引数の最大値を設定
	m_nMax = nMax;

	// 数字の補正
	useful::LimitNum(m_nNum, m_nMin, m_nMax);
}

//============================================================
//	数値取得処理
//============================================================
int CMultiValue::GetNum(void) const
{
	// 数字の値を返す
	return m_nNum;
}

//============================================================
//	桁数取得処理
//============================================================
int CMultiValue::GetDigit(void) const
{
	// 桁数を返す
	return m_nDigit;
}

//============================================================
//	最小値取得処理
//============================================================
int CMultiValue::GetMin(void) const
{
	// 最小値を返す
	return m_nMin;
}

//============================================================
//	最大値取得処理
//============================================================
int CMultiValue::GetMax(void) const
{
	// 最大値を返す
	return m_nMax;
}

//============================================================
//	行間の設定処理
//============================================================
void CMultiValue::SetSpace(const D3DXVECTOR3& rSpace)
{
	// 引数の行間を設定
	m_space = rSpace;
}

//============================================================
//	行間取得処理
//============================================================
D3DXVECTOR3 CMultiValue::GetSpace(void) const
{
	// 行間を返す
	return m_space;
}

//============================================================
//	テクスチャの設定処理
//============================================================
void CMultiValue::SetTexture(const CValue::ETexture texture)
{
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // 桁数分繰り返す

		// 引数のテクスチャを全ての数字に設定
		m_apValue[nCntValue]->SetTexture(texture);
	}
}

//============================================================
//	数字位置の設定処理
//============================================================
void CMultiValue::SetNumPosition(void)
{
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // 桁数分繰り返す

		// 原点の位置から数字の位置を設定
		m_apValue[nCntValue]->SetVec3Position(m_pos + (m_space * (float)nCntValue));
	}
}

//============================================================
//	数字のテクスチャ座標の設定処理
//============================================================
void CMultiValue::SetTexNum(void)
{
	// 変数を宣言
	int aNumDivide[MAX_DIGIT];	// 桁数ごとの分解用

	// マルチ数字を桁数ごとに分解
	useful::DivideDigitNum
	( // 引数
		&aNumDivide[0],	// 分解結果の格納配列
		m_nNum,			// 分解する数値
		m_nDigit		// 分解する数字の桁数
	);

	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // 桁数分繰り返す

		if (m_apValue[nCntValue] != NULL)
		{ // 使用されている場合

			// 数字の設定
			m_apValue[nCntValue]->SetNumber(aNumDivide[nCntValue]);
		}
	}
}
