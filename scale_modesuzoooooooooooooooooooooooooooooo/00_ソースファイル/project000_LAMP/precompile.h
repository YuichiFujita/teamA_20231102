//============================================================
//
//	プレコンパイルヘッダー [precompile.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PRECOMPILE_H_
#define _PRECOMPILE_H_

//************************************************************
//	インクルードファイル
//************************************************************
// ビルド時の警告対処用マクロ
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#include <stdio.h>
#include <time.h>
#include <assert.h>

#include <d3dx9.h>

// ビルド時の警告対処用マクロ
#define  DIRECTINPUT_VERSION	(0x0800)
#include <dinput.h>		// 入力処理に必要

#include <Xinput.h>		// パッド使用に必要
#include <xaudio2.h>	// サウンド処理に必要

// メモリリーク出力用マクロ
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		// メモリリーク検出に必要

// メモリリークのソース表示 ON/OFF
#if 1
#ifdef _DEBUG
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif	// _DEBUG
#endif

#endif	// _PRECOMPILE_H_
