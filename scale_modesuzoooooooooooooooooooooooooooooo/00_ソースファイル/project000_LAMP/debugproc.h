//==========================================================
//
//デバッグ表示処理 [debugproc.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _DEBUGPROC_H_		//このマクロが定義されていない場合
#define _DEBUGPROC_H_

#include "main.h"

//**********************************************************
//マクロ定義
//**********************************************************
#define MAX_DEBUGSTRING	(2048)	//デバッグ表示の最大文字数

//**********************************************************
//クラスの定義
//**********************************************************
class CDebugProc
{
public:
	// デバッグ位置列挙
	enum EPoint
	{
		POINT_CENTER = 0,	// 中央
		POINT_LEFT,			// 左
		POINT_RIGHT,		// 右
		POINT_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CDebugProc();

	// デストラクタ
	~CDebugProc();

	// メンバ関数
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Print(const EPoint point, const char *fmt, ...);

	// 静的メンバ関数
	static CDebugProc *Create(void);	// 生成
	static HRESULT Release(CDebugProc *&pDebugProc);	// 破棄

private:
	static LPD3DXFONT m_pFont;	// フォントへのポインタ
	bool m_bDisp;				// デバッグ表示のON/OFF
	char m_aStr[POINT_MAX][MAX_DEBUGSTRING];	// デバッグ表示用の文字列
};

#endif	// _DEBUGPROC_H_
