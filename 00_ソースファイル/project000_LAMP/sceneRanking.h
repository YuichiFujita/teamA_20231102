//============================================================
//
//	ランキング画面ヘッダー [sceneRanking.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCENE_RANKING_H_
#define _SCENE_RANKING_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	前方宣言
//************************************************************
class CRankingManager;	// ランキングマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// ランキング画面クラス
class CSceneRanking : public CScene
{
public:
	// コンストラクタ
	CSceneRanking(const EMode mode);

	// デストラクタ
	~CSceneRanking();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	HRESULT Uninit(void) override;	// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CRankingManager *GetRankingManager(void);	// ランキングマネージャー取得

private:
	// 静的メンバ変数
	static CRankingManager *m_pRankingManager;	// ランキングマネージャー
};

#endif	// _SCENE_RANKING_H_
