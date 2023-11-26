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
#include "editStage.h"
#include "ground.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditStageManager;	// エディットステージマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// エディット地盤クラス
class CEditGround : public CEditStage
{
public:
	// コンストラクタ
	CEditGround();

	// デストラクタ
	~CEditGround() override;

	// 地盤情報構造体
	struct SInfo
	{
		CGround::EType type;	// 地盤種類
		D3DXVECTOR3 size;		// 大きさ
		D3DXVECTOR2 partX, partY, partZ;	// テクスチャ分割数
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
	void UpdateSizing(void);		// 大きさ更新
	void UpdateTexPart(void);		// テクスチャ分割の更新
	void UpdateChangeType(void);	// 種類変更の更新
	void CreateGround(void);		// 地盤生成
	void ReleaseGround(void);		// 地盤破棄

	void DeleteCollisionGround(const bool bRelase);	// 地盤の削除判定
	void InitAllColorGround(void);					// 地盤の色全初期化

	// 静的メンバ変数
	static SInfo m_save;	// 保存情報

	// メンバ変数
	CGround *m_pGround;	// 地盤情報
	SInfo m_ground;		// 地盤配置情報
};

#endif	// _EDIT_GROUND_H_
