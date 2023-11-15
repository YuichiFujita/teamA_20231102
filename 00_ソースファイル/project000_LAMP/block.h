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
		TYPE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CBlock();

	// デストラクタ
	~CBlock();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetType(const int nType) override;	// 種類設定
	int GetType(void) const override;		// 種類取得

	// 静的メンバ関数
	static CBlock *Create	// 生成
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR3& rSize	// 大きさ
	);

private:
	// 静的メンバ変数
	static const char *mc_apTextureFile[][6];	// テクスチャ定数

	// メンバ変数
	EType m_type;	// 種類
};

#endif	// _BLOCK_H_
