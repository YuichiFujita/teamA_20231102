//============================================================
//
//	便利関数ヘッダー [useful.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _USEFUL_H_
#define _USEFUL_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	マクロ定義
//************************************************************
// 便利マクロ
#define SCREEN_WIDTH	(1280)	// ウインドウの横幅
#define SCREEN_HEIGHT	(720)	// ウインドウの縦幅
#define MAX_STRING		(128)	// 文字列の最大文字数
#define MAX_FILENAME	(128)	// ファイル名の最大文字数
#define NONE_IDX		(-1)	// インデックス非使用
#define NONE_STRING		("\0")	// 文字列非使用

#define PI_TWO_DP		(3.14f)	// 小数点第二までの円周率
#define HALF_PI_TWO_DP	(1.57f)	// 小数点第二までの二分の一の円周率 (π/２)

#define HALF_PI	(D3DX_PI * 0.5f)	// 二分の一の円周率 (π/２)
#define QRTR_PI	(D3DX_PI * 0.25f)	// 四分の一の円周率 (π/４)

// D3DXVECTOR2関係
#define VEC2_ZERO	(D3DXVECTOR2(0.0f, 0.0f))	// 0クリア
#define VEC2_ONE	(D3DXVECTOR2(1.0f, 1.0f))	// 1クリア
#define VEC2_ALL(f)	(D3DXVECTOR2((f), (f))		// 同値クリア

// D3DXVECTOR3関係
#define VEC3_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// 0クリア
#define VEC3_ONE	(D3DXVECTOR3(1.0f, 1.0f, 1.0f))	// 1クリア
#define VEC3_ALL(f)	(D3DXVECTOR3((f), (f), (f)))	// 同値クリア

#define SCREEN_CENT	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))	// ウインドウの中央座標
#define SCREEN_SIZE	(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))				// ウインドウの画面サイズ

// POSGRID2関係
#define GRID2_ZERO	(POSGRID2(0, 0))	// 0クリア
#define GRID2_ONE	(POSGRID2(1, 1))	// 1クリア

// POSGRID3関係
#define GRID3_ZERO	(POSGRID3(0, 0, 0))	// 0クリア
#define GRID3_ONE	(POSGRID3(1, 1, 1))	// 1クリア

// D3DXCOLOR関係
#define XCOL_ABLACK	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))	// 黒(透明)
#define XCOL_BLACK	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	// 黒(不透明)
#define XCOL_AWHITE	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f))	// 白(透明)
#define XCOL_WHITE	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// 白(不透明)
#define XCOL_ARED	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f))	// 赤(透明)
#define XCOL_RED	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	// 赤(不透明)
#define XCOL_AGREEN	(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f))	// 緑(透明)
#define XCOL_GREEN	(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	// 緑(不透明)
#define XCOL_ABLUE	(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f))	// 青(透明)
#define XCOL_BLUE	(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))	// 青(不透明)

//************************************************************
//	構造体定義
//************************************************************
// 2整数座標
struct POSGRID2
{
	// コンストラクタ
	POSGRID2() {}
	POSGRID2(const int nx, const int ny) { x = nx; y = ny; }	// 代入

	// デストラクタ
	~POSGRID2() {}

	// メンバ関数
	bool operator==(const POSGRID2& grid) const;
	bool operator!=(const POSGRID2& grid) const;

	// メンバ変数
	int x;	// 方眼座標 (x)
	int y;	// 方眼座標 (y)
};

// 3整数座標
struct POSGRID3
{
	// コンストラクタ
	POSGRID3() {}
	POSGRID3(const int nx, const int ny, const int nz) { x = nx; y = ny; z = nz; }	// 代入

	// デストラクタ
	~POSGRID3() {}

	// メンバ関数
	bool operator==(const POSGRID3& grid) const;
	bool operator!=(const POSGRID3& grid) const;

	// メンバ変数
	int x;	// 方眼座標 (x)
	int y;	// 方眼座標 (y)
	int z;	// 方眼座標 (z)
};

//************************************************************
//	名前空間宣言
//************************************************************
// 便利関数空間
namespace useful
{
	// 通常関数
	void DivideDigitNum		// 数値の桁数ごとの分解
	( // 引数
		int *pNumDivide,	// 分解結果の格納配列
		const int nNum,		// 分解する数値
		const int nMaxDigit	// 分解する数字の桁数
	);
	void NormalizeNormal	// 法線の正規化
	( // 引数
		const D3DXVECTOR3& rPosLeft,	// 左位置
		const D3DXVECTOR3& rPosCenter,	// 中心位置
		const D3DXVECTOR3& rPosRight,	// 右位置
		D3DXVECTOR3& rNor				// 法線
	);
	void NormalizeRot(float& rRot);				// 向きの正規化
	void Vec3NormalizeRot(D3DXVECTOR3& rRot);	// 三軸向きの正規化

	// テンプレート関数
	template<class T> bool LimitNum	// 値の範囲内制限
	( // 引数
		T& rNum,		// 制限数値
		const T min,	// 最小範囲
		const T max		// 最大範囲
	);
	template<class T> void SortNum	// 値のソート
	( // 引数
		T *pSortNum,		// ソート配列
		const int nMaxKeep	// 配列サイズ
	);
	template<class T> void Shuffle	// シャッフル
	(
		T *pShuffleData,		// シャッフル配列
		const int nMaxShuffle	// 配列サイズ
	);
}

// マテリアル空間
namespace material
{
	// 通常関数
	D3DXMATERIAL Red(void);			// 赤マテリアル取得
	D3DXMATERIAL GlowGreen(void);	// 発光緑マテリアル取得
}

//************************************************************
//	テンプレート関数
//************************************************************
//	便利関数空間
//************************************************************
//============================================================
//	値の範囲内制限処理
//============================================================
template<class T> bool useful::LimitNum
(
	T& rNum,		// 制限数値
	const T min,	// 最小範囲
	const T max		// 最大範囲
)
{
	if (rNum < min)
	{ // 制限数値が最小範囲を超えていた場合

		// 範囲内に補正
		rNum = min;

		// 真を返す
		return true;
	}
	else if (rNum > max)
	{ // 制限数値が最大範囲を超えていた場合

		// 範囲内に補正
		rNum = max;

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	ソート処理
//============================================================
template<class T> void useful::SortNum
(
	T *pSortNum,		// ソート配列
	const int nMaxKeep	// 配列サイズ
)
{
	// 変数を宣言
	T	keepNum;		// ソート用
	int	nCurrentMaxID;	// 最大値のインデックス

	for (int nCntKeep = 0; nCntKeep < (nMaxKeep - 1); nCntKeep++)
	{ // 入れ替える値の総数 -1回分繰り返す

		// 現在の繰り返し数を代入 (要素1とする)
		nCurrentMaxID = nCntKeep;

		for (int nCntSort = nCntKeep + 1; nCntSort < nMaxKeep; nCntSort++)
		{ // 入れ替える値の総数 -nCntKeep分繰り返す

			if (pSortNum[nCurrentMaxID] < pSortNum[nCntSort])
			{ // 最大値に設定されている値より、現在の値のほうが大きい場合

				// 現在の要素番号を最大値に設定
				nCurrentMaxID = nCntSort;
			}
		}

		if (nCntKeep != nCurrentMaxID)
		{ // 最大値の要素番号に変動があった場合

			// 要素の入れ替え
			keepNum					= pSortNum[nCntKeep];
			pSortNum[nCntKeep]		= pSortNum[nCurrentMaxID];
			pSortNum[nCurrentMaxID]	= keepNum;
		}
	}
}

//============================================================
//	シャッフル処理
//============================================================
template<class T> void useful::Shuffle
(
	T *pShuffleData,		// シャッフル配列
	const int nMaxShuffle	// 配列サイズ
)
{
	for (int nCntShu = nMaxShuffle - 1; nCntShu > 0; nCntShu--)
	{ // 引数の総数分繰り返す

		// 変数を宣言
		T swap;		// 要素の入れ替え用
		int nRandom = rand() % nCntShu;	// 要素を入れ替えるインデックス

		// 要素の入れ替え
		swap					= pShuffleData[nRandom];
		pShuffleData[nRandom]	= pShuffleData[nCntShu];
		pShuffleData[nCntShu]	= swap;
	}
}

#endif	// _USEFUL_H_
