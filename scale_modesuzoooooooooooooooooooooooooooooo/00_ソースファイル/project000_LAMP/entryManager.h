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
//	前方宣言
//************************************************************
class CValueUI;		// 数字UIクラス
class CObject2D;	// オブジェクト2Dクラス

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
		TEXTURE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEntryManager();

	// デストラクタ
	~CEntryManager();

	// メンバ関数
	HRESULT Init(void);		// 初期化
	HRESULT Uninit(void);	// 終了
	void Update(void);		// 更新

	// 静的メンバ関数
	static CEntryManager *Create(void);	// 生成
	static HRESULT Release(CEntryManager *&prEntryManager);	// 破棄

private:
	// メンバ関数
	void UpdateEntry(void);		// エントリーの更新
	void UpdateStart(void);		// 開始の更新

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CValueUI *m_apNumber[MAX_PLAYER];	// プレイヤーナンバーの情報
	CObject2D *m_apFrame[MAX_PLAYER];	// プレイヤーフレームの情報
	CObject2D *m_pControl;	// 操作表示の情報
	CObject2D *m_pStart;	// 開始表示の情報
};

#endif	// _ENTRY_MANAGER_H_
