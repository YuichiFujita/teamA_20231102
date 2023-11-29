//============================================================
//
//	メイン処理 [main.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "manager.h"
#include "resource.h"

#ifdef _DEBUG	// デバッグ処理

#include "debug.h"

#endif	// _DEBUG

//************************************************************
//	ライブラリリンク
//************************************************************
#pragma comment(lib, "d3d9.lib")	// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")	// [d3d9.lib] の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")	// DirectXコンポーネント (部品) 使用に必要
#pragma comment(lib, "winmm.lib")	// システム時刻取得に必要
#pragma comment(lib, "dinput8.lib")	// 入力処理に必要
#pragma comment(lib, "xinput.lib")	// パッド使用に必要

//************************************************************
//	マクロ定義
//************************************************************
#define CLASS_NAME	"WindowClass"	// ウインドウクラスの名前
#define WINDOW_NAME	"NeverGiveUp!"	// ウインドウの名前 (キャプションに表示)

#define FPS		(60)			// アプリケーションのFPS値
#define APP_FPS	(1000 / FPS)	// アプリケーションの経過時間の判定用

//************************************************************
//	プロトタイプ宣言
//************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウインドウプロシージャ

//============================================================
//	メイン関数
//============================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hInstancePrev*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	// 終了時にメモリリークを出力
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// 変数を宣言
	WNDCLASSEX wcex =
	{ // 初期値
		sizeof(WNDCLASSEX),									// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,											// ウインドウのスタイル
		WindowProc,											// ウインドウプロシージャ
		0,													// 通常は使用しない
		0,													// 通常は使用しない
		hInstance,											// インスタンスハンドル
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),	// タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),						// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),							// クライアント領域の背景色
		NULL,												// メニューバー
		CLASS_NAME,											// ウインドウクラスの名前
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1))		// ファイルのアイコン
	};
	RECT rect =		// 画面サイズ
	{ // 初期値
		0,				// ウインドウの左上 X座標
		0,				// ウインドウの左上 Y座標
		SCREEN_WIDTH,	// ウインドウの幅
		SCREEN_HEIGHT	// ウインドウの高さ
	};
	HWND  hWnd;	// ウインドウハンドル (識別子)
	MSG   msg;	// メッセージを格納する変数
	DWORD dwCurrentTime  = 0;				// 現在時刻
	DWORD dwExecLastTime = timeGetTime();	// 最後に処理した時刻
	CManager *pManager = NULL;	// マネージャーオブジェクト

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを生成
	hWnd = CreateWindowEx
	( // 引数
		0,							// 拡張ウインドウスタイル
		CLASS_NAME,					// ウインドウクラスの名前
		WINDOW_NAME,				// ウインドウの名前
		WS_OVERLAPPEDWINDOW,		// ウインドウスタイル
		CW_USEDEFAULT,				// ウインドウの左上 X 座標
		CW_USEDEFAULT,				// ウインドウの左上 Y 座標
		(rect.right - rect.left),	// ウインドウの幅
		(rect.bottom - rect.top),	// ウインドウの高さ
		NULL,						// 親ウインドウのハンドル
		NULL,						// メニューハンドルまたは子ウインドウ ID
		hInstance,					// インスタンスハンドル
		NULL						// ウインドウ作成データ
	);

	// 分解能を設定
	timeBeginPeriod(1);

	// ランダム関数の種を作成
	srand((unsigned int)time(0));

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);		// ウインドウの表示状態を設定
	UpdateWindow(hWnd);				// クライアント領域を更新

	// マネージャーの生成
	pManager = CManager::Create(hInstance, hWnd, TRUE);
	if (pManager == NULL)
	{ // 非使用中の場合

		// 例外を返す
		return -1;
	}

	// メッセージループ
	while (1)
	{ // 無限ループ

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{ // Windowsの処理

			if (msg.message == WM_QUIT)
			{ // WM_QUITメッセージを受け取った場合

				// メッセージループを抜ける
				break;
			}
			else
			{ // WM_QUITメッセージを受け取っていない場合

				// メッセージの設定
				TranslateMessage(&msg);	// 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);	// ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{ // DirectXの処理

			// 現在時刻を取得
			dwCurrentTime = timeGetTime();

#ifdef _DEBUG	// デバッグ処理

			// FPSの測定
			pManager->GetDebug()->MeasureFps(dwCurrentTime);

#endif	// _DEBUG

			if ((dwCurrentTime - dwExecLastTime) >= APP_FPS)
			{ // 60分の1秒経過

				if (pManager != NULL)
				{ // 使用中の場合

					// マネージャーの更新
					pManager->Update();
				}
				else { assert(false); return -1; }	// 非使用中

				if (pManager != NULL)
				{ // 使用中の場合

					// マネージャーの描画
					pManager->Draw();
				}
				else { assert(false); return -1; }	// 非使用中

				// 処理開始の時刻 [現在時刻] を保存
				dwExecLastTime = dwCurrentTime;

#ifdef _DEBUG	// デバッグ処理

				// フレームカウントの加算
				pManager->GetDebug()->AddFrameCount();

#endif	// _DEBUG

			}
		}
	}

	// マネージャーの破棄
	if (FAILED(CManager::Release(pManager)))
	{ // 破棄に失敗した場合

		// 例外を返す
		return -1;
	}

	// 分解能を戻す
	timeEndPeriod(1);

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了コードを返す
	return (int)msg.wParam;
}

//============================================================
//	ウインドウプロシージャ
//============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{ // メッセージコードごとの処理
	case WM_KEYDOWN:	// キー入力検知

		switch (wParam)
		{ // wパラメータごとの処理
		case VK_ESCAPE:	// [ESC] キーが押された場合

#ifdef NDEBUG	// リリース処理

			if (IDYES == MessageBox(hWnd, "本当に終了しますか？", "終了メッセージ", MB_YESNO))
			{ // YES が押された場合

				// ウインドウを破棄する
				DestroyWindow(hWnd);	// WM_DESTROYメッセージを送る
			}
			else
			{ // NO が押された場合

				// 値を返す
				return 0;
			}

#else	// NDEBUG

			// ウインドウを破棄する
			DestroyWindow(hWnd);	// WM_DESTROYメッセージを送る

#endif	// NDEBUG

			// 処理を抜ける
			break;
		}

		// 処理を抜ける
		break;

	case WM_CLOSE:		// ウインドウ終了確認

#ifdef NDEBUG	// リリース処理

		if (IDYES == MessageBox(hWnd, "本当に終了しますか？", "終了メッセージ", MB_YESNO))
		{ // YES が押された場合

			// ウインドウを破棄する
			DestroyWindow(hWnd);	// WM_DESTROYメッセージを送る
		}
		else
		{ // NO が押された場合

			// 値を返す
			return 0;
		}

#else	// NDEBUG

		// ウインドウを破棄する
		DestroyWindow(hWnd);	// WM_DESTROYメッセージを送る

#endif	// NDEBUG

		// 処理を抜ける
		break;

	case WM_DESTROY:	// ウインドウ終了

		// WM_QUITメッセージを送る
		PostQuitMessage(0);

		// 処理を抜ける
		break;
	}

	// デフォルトのウインドウプロシージャの終了コードを返す
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
