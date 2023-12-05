//============================================================
//
//	中間リザルトマネージャーヘッダー [middleResultManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MIDDLE_RESULT_MANAGER_H_
#define _MIDDLE_RESULT_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "input.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス
class CValueUI;		// 数字UIクラス

//************************************************************
//	クラス定義
//************************************************************
// 中間リザルトマネージャークラス
class CMiddleResultManager
{
public:
	// テクスチャ列挙
	enum ETexture
	{
		TEXTURE_FADE = 0,		// フェードテクスチャ
		TEXTURE_TITLE,			// ランキングタイトルテクスチャ
		TEXTURE_WIN_BG,			// 勝利ポイント背景テクスチャ
		TEXTURE_WIN,			// 勝利ポイントタイトルテクスチャ
		TEXTURE_PLAYER,			// プレイヤーテクスチャ
		TEXTURE_PLAYER_POINT,	// 勝利ポイントテクスチャ
		TEXTURE_FRAME,			// フレームテクスチャ
		TEXTURE_MAX				// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_FADEIN = 0,		// フェードイン状態
		STATE_FADEIN_WAIT,		// フェードイン待機状態
		STATE_FADEIN_ACCEL,		// フェードイン加速状態
		STATE_RANK_TITLE_WAIT,	// ランキングタイトル待機状態
		STATE_RANK_TITLE,		// ランキングタイトル表示状態
		STATE_WINPOINT_WAIT,	// 勝利ポイント待機状態
		STATE_WINPOINT,			// 勝利ポイント表示状態
		STATE_WAIT,				// 待機状態
		STATE_FADEOUT,			// フェードアウト状態
		STATE_FADEOUT_WAIT,		// フェードアウト待機状態
		STATE_FADEOUT_ACCEL,	// フェードアウト加速状態
		STATE_END,				// 終了状態
		STATE_MAX				// この列挙型の総数
	};

	// コンストラクタ
	CMiddleResultManager();

	// デストラクタ
	~CMiddleResultManager();

	// メンバ関数
	HRESULT Init(void);		// 初期化
	HRESULT Uninit(void);	// 終了
	void Update(void);		// 更新

	// 静的メンバ関数
	static CMiddleResultManager *Create(void);	// 生成
	static HRESULT Release(CMiddleResultManager *&prMiddleResultManager);	// 破棄

private:
	// メンバ変数
	void UpdateFadeIn(void);		// フェードイン更新
	void UpdateFadeInWait(void);	// フェードイン待機更新
	void UpdateFadeInAccel(void);	// フェードイン加速更新
	void UpdateRankTitleWait(void);	// ランキングタイトル待機更新
	void UpdateRankTitle(void);		// ランキングタイトル表示更新
	void UpdateWinPointWait(void);	// 勝利ポイント待機更新
	void UpdateWinPoint(void);		// 勝利ポイント表示更新
	void UpdateFadeOut(void);		// フェードアウト更新
	void UpdateFadeOutWait(void);	// フェードアウト待機更新
	void UpdateFadeOutAccel(void);	// フェードアウト加速更新

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CObject2D	*m_pFade;		// フェードの情報
	CObject2D	*m_pTitle;		// タイトルの情報
	CObject2D	*m_pWinPointBG;	// 勝利ポイントの背景情報
	CValueUI	*m_pWinPoint;	// 勝利ポイントの情報
	CValueUI	*m_apNumber[MAX_PLAYER];			// プレイヤーナンバーの情報
	CValueUI	*m_apPlayerWinPoint[MAX_PLAYER];	// プレイヤー勝利ポイントの情報
	CObject2D	*m_apFrame[MAX_PLAYER];				// プレイヤーフレームの情報

	EState	m_state;			// 状態
	int		m_nCounterState;	// 状態管理カウンター
	float	m_fMoveY;			// 縦移動量
	float	m_fScale;			// 拡大率
};

#endif	// _MIDDLE_RESULT_MANAGER_H_
