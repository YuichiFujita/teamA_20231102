//============================================================
//
//	チュートリアル画面ヘッダー [sceneTutorial.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCENE_TUTORIAL_H_
#define _SCENE_TUTORIAL_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	前方宣言
//************************************************************
class CTutorialManager;	// チュートリアルマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// チュートリアル画面クラス
class CSceneTutorial : public CScene
{
public:
	// コンストラクタ
	CSceneTutorial(const EMode mode);

	// デストラクタ
	~CSceneTutorial();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	HRESULT Uninit(void) override;	// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CTutorialManager *GetTutorialManager(void);	// チュートリアルマネージャー取得

private:
	// 静的メンバ変数
	static CTutorialManager *m_pTutorialManager;	// チュートリアルマネージャー
};

#endif	// _SCENE_TUTORIAL_H_
