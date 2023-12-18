//============================================================
//
//	アニメーション2Dヘッダー [Ready.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _Ready_H_
#define _Ready_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object2D.h"
#include "anim2D.h"
//************************************************************
//	クラス定義
//************************************************************
// アニメーション2Dクラス
class CReady : public CObject
{
public:
	// コンストラクタ
	CReady();
	explicit CReady(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CReady();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	

	// 静的メンバ関数
	static CReady *Create( 	);

	// メンバ関数


private:
	void Release();
	// メンバ変数
	int m_nStateCount;
	CObject2D * m_pTexReady;
	CObject2D * m_pTexFight;
	CAnim2D * m_pFlare;
};

#endif	// _Ready_H_
