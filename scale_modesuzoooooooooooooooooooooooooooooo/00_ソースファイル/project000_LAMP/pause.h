//============================================================
//
//	ポーズヘッダー [pause.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PAUSE_H_
#define _PAUSE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// ポーズクラス
class CPause
{
public:
	// 選択列挙
	enum ESelect
	{
		SELECT_RESUME = 0,	// 再開
		SELECT_RETRY,		// リトライ
		SELECT_EXIT,		// 終了
		SELECT_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CPause();

	// デストラクタ
	~CPause();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	bool IsPause(void);	// ポーズ状況取得
	void SetEnableDraw(const bool bDraw);	// 描画状況設定

	// 静的メンバ関数
	static CPause *Create(void);				// 生成
	static HRESULT Release(CPause *&prPause);	// 破棄

private:
	// メンバ関数
	void Select(void);	// 選択操作

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CObject2D *m_apSelect[SELECT_MAX];	// 選択情報
	CObject2D *m_pBG;	// 背景情報
	int  m_nSelect;		// 現在の選択
	int  m_nOldSelect;	// 前回の選択
	bool m_bPause;		// ポーズ状況
};

#endif	// _PAUSE_H_
