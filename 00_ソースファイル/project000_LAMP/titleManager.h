//============================================================
//
//	タイトルマネージャーヘッダー [titleManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TITLEMANAGER_H_
#define _TITLEMANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// タイトルマネージャークラス
class CTitleManager
{
public:
	// ロゴ列挙
	enum ELogo
	{
		LOGO_NEVER = 0,	// NEVERロゴ
		LOGO_GIVE,		// GIVEロゴ
		LOGO_UP,		// UP!ロゴ
		LOGO_MAX		// この列挙型の総数
	};

	// 選択列挙
	enum ESelect
	{
		SELECT_GAME = 0,	// ゲーム選択
		SELECT_TUTORIAL,	// チュートリアル選択
		SELECT_MAX			// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_FADEOUT,	// フェードアウト状態
		STATE_MOVE,		// タイトル縮小状態
		STATE_WAIT,		// 遷移待機状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CTitleManager();

	// デストラクタ
	~CTitleManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新

	// 静的メンバ関数
	static CTitleManager *Create(void);	// 生成
	static HRESULT Release(CTitleManager *&prTitleManager);	// 破棄

private:
	// メンバ関数
	void UpdateFade(void);	// フェードアウト
	void UpdateMove(void);	// タイトル移動
	void UpdateStart(void);	// 遷移決定
	void ActSelect(void);	// 選択操作
	void SkipStaging(void);	// 演出スキップ

	// 静的メンバ変数
	static const char *mc_apLogoTextureFile[];		// ロゴテクスチャ定数
	static const char *mc_apSelectTextureFile[];	// 選択テクスチャ定数

	// メンバ変数
	CObject2D *m_apLogo[LOGO_MAX];		// タイトル表示の情報
	CObject2D *m_apSelect[SELECT_MAX];	// 選択表示の情報
	CObject2D *m_pFade;		// フェードの情報
	CObject2D *m_pSelectBG;	// 選択背景の情報
	EState m_state;		// 状態
	float m_fScale;		// タイトル拡大率
	int m_nSelect;		// 現在の選択
	int m_nOldSelect;	// 前回の選択
};

#endif	// _TITLEMANAGER_H_
