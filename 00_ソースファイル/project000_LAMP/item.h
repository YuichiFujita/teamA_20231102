//============================================================
//
//	アイテムスポーンヘッダー [itemspawn.h]
//	Author：森川駿弥
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ITEMSPAWN_H_
#define _ITEMSPAWN_H_
//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectmodel.h"

//************************************************************
//	クラス定義
//************************************************************
// アイテムクラス
class CItem : public CObjectModel
{
public:
	// コンストラクタ
	CItem();

	// デストラクタ
	~CItem();

	//<*********************************
	//アイテムタイプの列挙型
	//<*********************************
	enum TYPE
	{
		TYPE_HEAL = 0,		//回復アイテム
		TYPE_ATTACKUP,		//攻撃力アップ
		TYPE_KNOCKBACKUP,	//ノックバックアップ
		TYPE_SUPER_ARMOR,	//スーパーアーマー
		TYPE_BIG_IRON,		//巨大鉄球
		TYPE_SLIP_FRAIL,	//すり抜け鎖
		TYPE_DOUBLE_FRAIL,	//ダブル鎖
		TYPE_BURNING_FRAIL,	//燃える鎖
		TYPE_MAX
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	//メンバ関数
	void Collision();

	static CItem *Create(D3DXVECTOR3 pos);

private:

	//静的メンバ変数
	static const char *mc_apModelFile[TYPE::TYPE_MAX];	//テクスチャ定数

	int m_nType;

	// メンバ変数
};

#endif
