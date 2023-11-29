//============================================================
//
//	マネージャーヘッダー [manager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	前方宣言
//************************************************************
class CRenderer;			// レンダラークラス
class CInputKeyboard;		// キーボードクラス
class CInputMouse;			// マウスクラス
class CInputPad;			// パッドクラス
class CSound;				// サウンドクラス
class CCamera;				// カメラクラス
class CLight;				// ライトクラス
class CTexture;				// テクスチャクラス
class CModel;				// モデルクラス
class CFade;				// フェードクラス
class CRetentionManager;	// データ保存マネージャークラス
class CDebugProc;			// デバッグ表示クラス

#ifdef _DEBUG	// デバッグ処理

class CDebug;	// デバッグクラス

#endif	// _DEBUG

//************************************************************
//	クラス定義
//************************************************************
// マネージャークラス
class CManager
{
public:
	// コンストラクタ
	CManager();

	// デストラクタ
	~CManager();

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// 初期化
	HRESULT Uninit(void);	// 終了
	void Update(void);		// 更新
	void Draw(void);		// 描画

	// 静的メンバ関数
	static CManager *Create(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// 生成
	static CManager *GetInstance(void);				// 取得
	static HRESULT Release(CManager *&prManager);	// 破棄

	// メンバ関数
	void SetScene(const CScene::EMode mode, const int nWait = 0);	// シーン設定
	HRESULT SetMode(const CScene::EMode mode);		// モード設定
	CScene::EMode GetMode(void);					// モード取得

	CRenderer			*GetRenderer(void);			// レンダラー取得
	CInputKeyboard		*GetKeyboard(void);			// キーボード取得
	CInputMouse			*GetMouse(void);			// マウス取得
	CInputPad			*GetPad(void);				// パッド取得
	CSound				*GetSound(void);			// サウンド取得
	CCamera				*GetCamera(void);			// カメラ取得
	CLight				*GetLight(void);			// ライト取得
	CTexture			*GetTexture(void);			// テクスチャ取得
	CModel				*GetModel(void);			// モデル取得
	CFade				*GetFade(void);				// フェード取得
	CScene				*GetScene(void);			// シーン取得
	CRetentionManager	*GetRetentionManager(void);	// データ保存マネージャー取得
	CDebugProc			*GetDebugProc(void);		// デバッグ表示取得

#ifdef _DEBUG	// デバッグ処理

	// メンバ関数
	CDebug *GetDebug(void);	// デバッグ取得

#endif	// _DEBUG

private:
	// 静的メンバ変数
	static CManager *m_pManager;	// マネージャー

	// メンバ変数
	CRenderer			*m_pRenderer;			// レンダラーオブジェクト
	CInputKeyboard		*m_pKeyboard;			// キーボードオブジェクト
	CInputMouse			*m_pMouse;				// マウスオブジェクト
	CInputPad			*m_pPad;				// パッドオブジェクト
	CSound				*m_pSound;				// サウンドオブジェクト
	CCamera				*m_pCamera;				// カメラオブジェクト
	CLight				*m_pLight;				// ライトオブジェクト
	CTexture			*m_pTexture;			// テクスチャオブジェクト
	CModel				*m_pModel;				// モデルオブジェクト
	CFade				*m_pFade;				// フェードオブジェクト
	CScene				*m_pScene;				// シーンオブジェクト
	CRetentionManager	*m_pRetentionManager;	// データ保存マネージャー
	CDebugProc			*m_pDebugProc;			// デバッグ表示

#ifdef _DEBUG	// デバッグ処理

	// メンバ変数
	CDebug *m_pDebug;	// デバッグ

#endif	// _DEBUG

};

#endif	// _MANAGER_H_
