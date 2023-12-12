//============================================================
//
//	エントリールールマネージャーヘッダー [entryRuleManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENTRYRULE_MANAGER_H_
#define _ENTRYRULE_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_ARROW	(2)	// 矢印の総数

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス
class CAnim2D;		// アニメーション2Dクラス
class CMultiValue;	// マルチ数字クラス

//************************************************************
//	クラス定義
//************************************************************
// エントリールールマネージャークラス
class CEntryRuleManager
{
public:
	// テクスチャ列挙
	enum ETexture
	{
		TEXTURE_RULE_TITLE = 0,	// ルールタイトルテクスチャ
		TEXTURE_KILL,			// 撃破条件テクスチャ
		TEXTURE_WIN,			// 勝利条件テクスチャ
		TEXTURE_START,			// 開始ボタンテクスチャ
		TEXTURE_CONTROL,		// 操作表示テクスチャ
		TEXTURE_ARROW,			// 矢印テクスチャ
		TEXTURE_MAX				// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_INIT = 0,		// 初期化状態
		STATE_FADEIN,		// フェードイン状態
		STATE_RULESET,		// ルール設定状態
		STATE_FADEOUT,		// フェードアウト状態
		STATE_ENTRYBACK,	// エントリー戻し状態
		STATE_MAX			// この列挙型の総数
	};

	// ルール列挙
	enum ERule
	{
		RULE_WINPOINT = 0,	// 勝利ポイント
		RULE_KILL,			// 撃破条件
		RULE_WIN,			// 勝利条件
		RULE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEntryRuleManager();

	// デストラクタ
	~CEntryRuleManager();

	// メンバ関数
	HRESULT Init(void);		// 初期化
	HRESULT Uninit(void);	// 終了
	void Update(void);		// 更新

	// 静的メンバ関数
	static CEntryRuleManager *Create(void);	// 生成
	static HRESULT Release(CEntryRuleManager *&prEntryRuleManager);	// 破棄

private:
	// メンバ関数
	void UpdateFadeIn(void);	// フェードイン更新
	void UpdateFadeOut(void);	// フェードアウト更新
	void UpdateControl(void);	// 操作更新
	void UpdateArrow(void);		// 矢印更新

	void Select(void);		// 選択操作
	void Decide(void);		// 決定操作
	void ChangeRule(void);	// ルール変更操作
	void SetRule(void);		// ルール変更反映
	void SetEnableUI(const bool bDraw);	// UI描画設定
	void SetAlphaUI(const float fAlpha, const bool bFadeOut);	// UI透明度設定

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CAnim2D *m_apRuleTitle[RULE_MAX];	// ルールタイトルの情報
	CAnim2D *m_apArrow[MAX_ARROW];		// 矢印の情報
	CMultiValue	*m_pWinPoint;	// 勝利ポイント数の情報
	CAnim2D		*m_pKill;		// 撃破条件の情報
	CAnim2D		*m_pWin;		// 勝利条件の情報
	CObject2D	*m_pSelect;		// 選択の情報
	CObject2D	*m_pStart;		// 開始ボタンの情報
	CObject2D	*m_pControl;	// 操作表示の情報
	CObject2D	*m_pFade;		// フェードの情報
	EState	m_state;			// 状態
	int		m_nSelect;			// 現在の選択
	int		m_nOldSelect;		// 前回の選択
	float	m_fSinControlAlpha;	// 操作表示の透明向き
	float	m_fSinArrowAlpha;	// 矢印表示の透明向き
};

#endif	// _ENTRYRULE_MANAGER_H_
