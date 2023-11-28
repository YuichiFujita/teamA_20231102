//============================================================
//
//	デバッグヘッダー [debug.h]
//	Author：藤田勇一
//
//============================================================
#ifdef _DEBUG	// デバッグ処理
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _DEBUG_H_
#define _DEBUG_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	クラス定義
//************************************************************
// デバッグクラス
class CDebug
{
public:
	// コンストラクタ
	CDebug();

	// デストラクタ
	~CDebug();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void MeasureFps(const DWORD dwCurrentTime);	// FPS測定
	void AddFrameCount(void);	// フレームカウント加算
	int  GetFps(void) const;	// FPS取得

	// 静的メンバ関数
	static CDebug *Create(void);				// 生成
	static HRESULT Release(CDebug *&pDebug);	// 破棄

private:
	// メンバ変数
	int		m_nFps;				// FPSカウンタ
	DWORD	m_dwFrameCount;		// フレームカウント
	DWORD	m_dwFPSLastTime;	// 最後にFPSを計測した時刻
};

#endif	// _DEBUG_H_
#endif	// _DEBUG
