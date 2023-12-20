//============================================================
//
//	エントリーマネージャーヘッダー [entryManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENTRY_MANAGER_H_
#define _ENTRY_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "input.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_ENTRY_ARROW	(2)	// 矢印の総数

//************************************************************
//	前方宣言
//************************************************************
class CValueUI;				// 数字UIクラス
class CAnim2D;				// アニメーション2Dクラス
class CObject2D;			// オブジェクト2Dクラス
class CEntryRuleManager;	// エントリールールマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// エントリーマネージャークラス
class CEntryManager
{
public:
	// テクスチャ列挙
	enum ETexture
	{
		TEXTURE_PLAYER = 0,	// PLAYERテクスチャ
		TEXTURE_FRAME,		// フレームテクスチャ
		TEXTURE_CONTROL,	// 操作表示テクスチャ
		TEXTURE_START,		// 開始表示テクスチャ
		TEXTURE_JOIN,		// 参加状況テクスチャ
		TEXTURE_ARROW,		// 矢印テクスチャ
		TEXTURE_CPU,		// CPUテクスチャ
		TEXTURE_NUMCPU,		// CPU数テクスチャ
		TEXTURE_FLAILARROW,	// フレイル矢印テクスチャ
		TEXTURE_MAX			// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_ENTRY = 0,	// エントリー受付状態
		STATE_RULE,			// ルール設定状態
		STATE_END,			// 終了状態
		STATE_MAX			// この列挙型の総数
	};

	// エントリー状態列挙
	enum EStateEntry
	{
		STATE_ENTRY_NONE_JOIN = 0,	// 準備の未完了状態
		STATE_ENTRY_CPU_BG,			// CPU数背景の演出状態
		STATE_ENTRY_CPU_UI,			// CPU数UIの演出状態
		STATE_ENTRY_NUMCPU,			// CPU数の変更状態
		STATE_ENTRY_MAX				// この列挙型の総数
	};

	// 参加列挙
	enum EJoin
	{
		JOIN_OFF = 0,	// 参加OFF
		JOIN_ON,		// 参加ON
		JOIN_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CEntryManager();

	// デストラクタ
	~CEntryManager();

	// メンバ関数
	HRESULT Init(void);		// 初期化
	HRESULT Uninit(void);	// 終了
	void Update(void);		// 更新
	HRESULT SetState(const EState state);	// 状態設定

	// 静的メンバ関数
	static CEntryManager *Create(void);	// 生成
	static HRESULT Release(CEntryManager *&prEntryManager);	// 破棄

private:
	// メンバ関数
	void UpdateEntry(void);			// エントリーの更新
	void UpdateFlail(void);			// フレイルの更新
	void UpdateCpu(void);			// CPUの更新
	void UpdateAddCpu(void);		// CPUの加減算の更新
	void UpdatePlayerName(void);	// プレイヤー名の更新
	void UpdateControlUI(void);		// 操作UIの更新
	void UpdateStartUI(void);		// 開始UIの更新
	void UpdateStart(void);			// 開始の更新
	void UpdateArrow(void);			// 矢印の更新
	void UpdateCpuBG(void);			// CPU数背景の演出の更新
	void UpdateCpuUI(void);			// CPU数UIの演出の更新
	void UpdateUIAll(void);			// UIオブジェクトの全更新
	void SetCpuObject(const bool bStart);		// CPU演出の設定
	bool IsReadyOK(const int nNumEntry) const;	// 準備済みかの判定取得

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CEntryRuleManager *m_pRuleManager;			// エントリールールの情報
	CValueUI	*m_apNumber[MAX_PLAYER];		// プレイヤーナンバーの情報
	CObject2D	*m_apCpu[MAX_PLAYER];			// プレイヤーCPUの情報
	CAnim2D		*m_apJoin[MAX_PLAYER];			// プレイヤー参加の情報
	CObject2D	*m_apFrame[MAX_PLAYER];			// プレイヤーフレームの情報
	CAnim2D		*m_apArrow[MAX_ENTRY_ARROW];	// 矢印の情報
	CAnim2D		*m_apFlailArrow[MAX_PLAYER][MAX_ENTRY_ARROW];	// フレイル矢印の情報

	CObject2D	*m_pControl;	// 操作表示の情報
	CObject2D	*m_pBG;			// 背景の情報
	CObject2D	*m_pStart;		// 開始表示の情報
	CValueUI	*m_pNumCpu;		// CPU数表示の情報
	EStateEntry	m_stateEntry;	// エントリー状態
	EState	m_state;			// 状態
	float	m_fScale;			// 拡大率
	float	m_fSinStartAlpha;	// 開始表示の透明向き
	float	m_fSinControlAlpha;	// 操作表示の透明向き
	float	m_fSinArrowAlpha;	// 矢印表示の透明向き
};

#endif	// _ENTRY_MANAGER_H_
