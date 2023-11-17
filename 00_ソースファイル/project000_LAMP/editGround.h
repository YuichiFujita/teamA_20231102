//============================================================
//
//	エディット地盤ヘッダー [editGround.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_GROUND_H_
#define _EDIT_GROUND_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "ground.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditStageManager;	// エディットステージマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// エディット地盤クラス
class CEditGround
{
public:
	// コンストラクタ
	CEditGround();

	// デストラクタ
	~CEditGround();

	// 地盤情報構造体
	struct SInfo
	{
		CGround *pGround;		// 地盤情報
		CGround::EType type;	// 地盤種類
	};

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void SetDisp(const bool bDisp);	// 表示設定
	void DrawDebugControl(void);	// 操作表示描画
	void DrawDebugInfo(void);		// 情報表示描画
	void Save(FILE *pFile);			// 保存

	// 静的メンバ関数
	static CEditGround *Create(CEditStageManager *pEdit);	// 生成
	static HRESULT Release(CEditGround *&prEditGround);	// 破棄

private:
	// メンバ関数
	void UpdateChangeType(void);	// 種類変更の更新
	void CreateGround(void);		// 地盤生成
	void ReleaseGround(void);		// 地盤破棄

	void DeleteCollisionGround(const bool bRelase);	// 地盤の削除判定
	void InitAllColorGround(void);					// 地盤の色全初期化

	// メンバ変数
	CEditStageManager *m_pEdit;	// エディットステージの情報
	SInfo m_ground;	// 地盤配置情報
};

#endif	// _EDIT_GROUND_H_
