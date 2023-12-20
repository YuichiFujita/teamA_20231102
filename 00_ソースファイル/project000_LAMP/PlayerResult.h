//============================================================
//
//	プレイヤーエントリーヘッダー [playerEntry.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_RESULT_H_
#define _PLAYER_RESULT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "player.h"

//************************************************************
//	クラス定義
//************************************************************
// プレイヤーエントリークラス
class CPlayerResult : public CPlayer
{
public:
	// コンストラクタ
	CPlayerResult(const CScene::EMode mode, const int nPad);

	// デストラクタ
	~CPlayerResult();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

private:
	// メンバ関数
	void SetEntry(void);	// エントリー設定
};

#endif	// _PLAYER_ENTRY_H_

