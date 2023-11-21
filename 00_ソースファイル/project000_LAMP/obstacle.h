//============================================================
//
//	障害物ヘッダー [obstacle.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	クラス定義
//************************************************************
// 障害物クラス
class CObstacle : public CObjectModel
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_WOOD = 0,	// 木モデル
		TYPE_MAX		// この列挙型の総数
	};

	// 破壊列挙
	enum EBreak
	{
		BREAK_FALSE = 0,	// 破壊OFF
		BREAK_TRUE,			// 破壊ON
		BREAK_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CObstacle();

	// デストラクタ
	~CObstacle();

	// ステータス構造体
	struct SStatusInfo
	{
		EBreak state;	// 破壊状況
		int nLife;		// 体力
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetType(const int nType) override;	// 種類設定
	int GetType(void) const override;		// 種類取得

	// 静的メンバ関数
	static CObstacle *Create	// 生成
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR3& rScale	// 拡大率
	);
	static void LoadSetup(void);	// セットアップ

private:
	// 静的メンバ変数
	static SStatusInfo m_aStatusInfo[TYPE_MAX];	// ステータス情報
	static const char *mc_apModelFile[];		// モデル定数

	// メンバ変数
	SStatusInfo m_status;	// ステータス
	EType m_type;	// 種類
	int m_nLife;	// 体力
};

#endif	// _OBSTACLE_H_
