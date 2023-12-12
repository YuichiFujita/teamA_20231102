//============================================================
//
//	エディットアイテム生成位置ヘッダー [editItemPoint.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_ITEMPOINT_H_
#define _EDIT_ITEMPOINT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "editStage.h"
#include "ItemSpawnPoint.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditStageManager;	// エディットステージマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// エディットアイテム生成位置クラス
class CEditItemPoint : public CEditStage
{
public:
	// コンストラクタ
	CEditItemPoint();

	// デストラクタ
	~CEditItemPoint() override;

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
	void LookEffect(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);	// アイテム生成位置のエフェクト表示

	void CreateItemPoint(void);		// アイテム生成位置生成
	void LookAllItemPoint(void);	// アイテム生成位置の全表示
};

#endif	// _EDIT_ITEMPOINT_H_
