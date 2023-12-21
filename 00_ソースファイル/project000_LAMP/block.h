//============================================================
//
//	ブロックヘッダー [block.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BLOCK_H_
#define _BLOCK_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectMeshCube.h"

//************************************************************
//	クラス定義
//************************************************************
// ブロッククラス
class CBlock : public CObjectMeshCube
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_STONE = 0,	// 石テクスチャ
		TYPE_BOX,		// 箱テクスチャ
		TYPE_BRICK,		// レンガ
		TYPE_CONCRETE,	// コンクリート
		TYPE_ICE,		// 氷
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
	CBlock();

	// デストラクタ
	~CBlock();

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
	void Hit(void) override;		// ヒット
	void SetType(const int nType) override;		// 種類設定
	int GetType(void) const override;			// 種類取得
	void SetState(const int nState) override;	// 破壊状況設定
	int GetState(void) const override;			// 破壊状況取得

	// 静的メンバ関数
	static CBlock *Create	// 生成
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR3& rSize,	// 大きさ
		const D3DXVECTOR2& rTexPartX = VEC2_ONE,	// テクスチャ分割数X
		const D3DXVECTOR2& rTexPartY = VEC2_ONE,	// テクスチャ分割数Y
		const D3DXVECTOR2& rTexPartZ = VEC2_ONE		// テクスチャ分割数Z
	);
	static void LoadSetup(void);	// セットアップ

private:
	// 静的メンバ変数
	static SStatusInfo m_aStatusInfo[TYPE_MAX];	// ステータス情報
	static const char *mc_apTextureFile[][6];	// テクスチャ定数

	// メンバ変数
	SStatusInfo m_status;	// ステータス
	EType m_type;	// 種類
	int m_nLife;	// 体力
};

#endif	// _BLOCK_H_
