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
		TEXTURE_ = 0,	// テクスチャ
		TEXTURE_MAX		// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_FADEIN = 0,	// フェードイン状態
		STATE_WAIT,			// 待機状態
		STATE_FADEOUT,		// フェードアウト状態
		STATE_ENTRYBACK,	// エントリー戻し状態
		STATE_MAX			// この列挙型の総数
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

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CObject2D *m_pFade;	// フェードの情報
	EState m_state;		// 状態
};

#endif	// _ENTRYRULE_MANAGER_H_
