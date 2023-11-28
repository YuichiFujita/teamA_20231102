//============================================================
//
//	チュートリアルマネージャーヘッダー [tutorialManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TUTORIAL_MANAGER_H_
#define _TUTORIAL_MANAGER_H_

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
// チュートリアルマネージャークラス
class CTutorialManager
{
public:
	// テクスチャ列挙
	enum ETexture
	{
		TEXTURE_CONTROL = 0,	// 操作方法テクスチャ
		TEXTURE_MAX				// この列挙型の総数
	};

	// 説明列挙
	enum EExplain
	{
		EXPLAIN_JUMP = 0,	// ジャンプ説明テクスチャ
		EXPLAIN_SLIDE,		// スライディング説明テクスチャ
		EXPLAIN_WALLDASH,	// 壁走り説明テクスチャ
		EXPLAIN_WALLJUMP,	// 壁ジャンプ説明テクスチャ
		EXPLAIN_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CTutorialManager();

	// デストラクタ
	~CTutorialManager();

	// メンバ関数
	HRESULT Init(void);		// 初期化
	HRESULT Uninit(void);	// 終了
	void Update(void);		// 更新

	// 静的メンバ関数
	static CTutorialManager *Create(void);	// 生成
	static HRESULT Release(CTutorialManager *&prTutorialManager);	// 破棄

private:
	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数
	static const char *mc_apLessonTextureFile[];	// レッスンテクスチャ定数

	// メンバ変数
	CObject2D *m_pExplain;	// 説明表示の情報
	CObject2D *m_pControl;	// 操作説明の情報
};

#endif	// _TUTORIAL_MANAGER_H_
