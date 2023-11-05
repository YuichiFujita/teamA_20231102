//============================================================
//
//	数字ヘッダー [value.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _VALUE_H_
#define _VALUE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "anim2D.h"

//************************************************************
//	クラス定義
//************************************************************
// 数字クラス
class CValue : public CAnim2D
{
public:
	// テクスチャ列挙
	enum ETexture
	{
		TEXTURE_NORMAL = 0,	// 通常テクスチャ
		TEXTURE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CValue();

	// デストラクタ
	~CValue();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CValue *Create(const ETexture texture);	// 生成
	static CValue *Create	// 生成
	( // 引数
		const ETexture texture,		// テクスチャ
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rSize = VEC3_ONE,	// 大きさ
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXCOLOR& rCol = XCOL_WHITE		// 色
	);

	// メイン関数
	void SetTexture(const ETexture texture);	// テクスチャ設定
	void SetNumber(const int nNum);	// 値設定

private:
	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	int m_nNum;	// 表示数値
};

#endif	// _VALUE_H_
