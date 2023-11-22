//============================================================
//
//	エディットステージヘッダー [editStage.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_STAGE_H_
#define _EDIT_STAGE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditStageManager;	// エディットステージマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// エディットステージクラス
class CEditStage
{
public:
	// 配置物列挙
	enum EThing
	{
		THING_GROUND = 0,	// 地盤
		THING_BLOCK,		// ブロック
		THING_OBSTACLE,		// 障害物
		THING_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEditStage();

	// デストラクタ
	virtual ~CEditStage();

	// 純粋仮想関数
	virtual HRESULT Init(void)	= 0;	// 初期化
	virtual void Uninit(void)	= 0;	// 終了
	virtual void Update(void)	= 0;	// 更新
	virtual void DrawDebugControl(void) = 0;	// 操作表示描画
	virtual void DrawDebugInfo(void) = 0;		// 情報表示描画
	virtual void SaveInfo(void) = 0;			// 情報保存
	virtual void LoadInfo(void) = 0;			// 情報読込
	virtual void Save(FILE *pFile) = 0;			// 保存

	// 静的メンバ関数
	static CEditStage *Create(CEditStageManager *pEdit, const EThing thing);	// 生成
	static HRESULT Release(CEditStage *&prEditGround);	// 破棄

	// メンバ関数
	CEditStageManager *GetPtrEditStage(void) const;	// エディットステージ情報取得

private:
	// メンバ変数
	CEditStageManager *m_pEdit;	// エディットステージの情報
};

#endif	// _EDIT_STAGE_H_
