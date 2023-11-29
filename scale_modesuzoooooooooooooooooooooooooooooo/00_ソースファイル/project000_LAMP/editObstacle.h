//============================================================
//
//	エディット障害物ヘッダー [editObstacle.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_OBSTACLE_H_
#define _EDIT_OBSTACLE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "editStage.h"
#include "obstacle.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditStageManager;	// エディットステージマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// エディット障害物クラス
class CEditObstacle : public CEditStage
{
public:
	// コンストラクタ
	CEditObstacle();

	// デストラクタ
	~CEditObstacle() override;

	// 障害物情報構造体
	struct SInfo
	{
		CObstacle::EType type;	// 障害物種類
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void DrawDebugControl(void) override;	// 操作表示描画
	void DrawDebugInfo(void) override;		// 情報表示描画
	void SaveInfo(void) override;			// 情報保存
	void LoadInfo(void) override;			// 情報読込
	void Save(FILE *pFile) override;		// 保存

private:
	// メンバ関数
	void UpdateChangeType(void);	// 種類変更の更新
	void CreateObstacle(void);		// 障害物生成
	void ReleaseObstacle(void);		// 障害物破棄

	void DeleteCollisionObstacle(const bool bRelase);	// 障害物の削除判定
	void InitAllColorObstacle(void);					// 障害物の色全初期化

	// 静的メンバ変数
	static SInfo m_save;	// 保存情報

	// メンバ変数
	CObstacle *m_pObstacle;	// 障害物情報
	SInfo m_obstacle;		// 障害物配置情報
};

#endif	// _EDIT_OBSTACLE_H_
