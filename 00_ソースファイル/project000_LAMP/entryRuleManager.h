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
		TEXTURE_PLAYER = 0,	// PLAYERテクスチャ
		TEXTURE_FRAME,		// フレームテクスチャ
		TEXTURE_CONTROL,	// 操作表示テクスチャ
		TEXTURE_START,		// 開始表示テクスチャ
		TEXTURE_MAX			// この列挙型の総数
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


	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CObject2D *m_pControl;	// 操作表示の情報
	CObject2D *m_pStart;	// 開始表示の情報
};

#endif	// _ENTRYRULE_MANAGER_H_
