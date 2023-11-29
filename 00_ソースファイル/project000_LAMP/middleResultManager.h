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

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

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
		TEXTURE_FADE = 0,	// フェードテクスチャ
		TEXTURE_MAX			// この列挙型の総数
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
	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CObject2D *m_pFade;	// フェードの情報
};

#endif	// _MIDDLE_RESULT_MANAGER_H_
