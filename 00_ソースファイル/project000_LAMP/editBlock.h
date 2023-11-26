//============================================================
//
//	エディットブロックヘッダー [editBlock.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_BLOCK_H_
#define _EDIT_BLOCK_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "editStage.h"
#include "block.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditStageManager;	// エディットステージマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// エディットブロッククラス
class CEditBlock : public CEditStage
{
public:
	// コンストラクタ
	CEditBlock();

	// デストラクタ
	~CEditBlock() override;

	// ブロック情報構造体
	struct SInfo
	{
		CBlock::EType type;	// ブロック種類
		D3DXVECTOR3 size;	// 大きさ
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
	void CreateBlock(void);			// ブロック生成
	void ReleaseBlock(void);		// ブロック破棄

	void DeleteCollisionBlock(const bool bRelase);	// ブロックの削除判定
	void InitAllColorBlock(void);					// ブロックの色全初期化

	// 静的メンバ変数
	static SInfo m_save;	// 保存情報

	// メンバ変数
	CBlock *m_pBlock;	// ブロック情報
	SInfo m_block;		// ブロック配置情報
};

#endif	// _EDIT_BLOCK_H_
