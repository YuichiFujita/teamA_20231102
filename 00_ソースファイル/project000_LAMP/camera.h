//============================================================
//
//	カメラヘッダー [camera.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	クラス定義
//************************************************************
// カメラクラス
class CCamera
{
public:
	// カメラ列挙
	enum EType
	{
		TYPE_MAIN = 0,	// メインカメラ
		TYPE_MODELUI,	// モデルUI表示カメラ
		TYPE_MAX		// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// なにもしない状態
		STATE_CONTROL,	// 操作状態
		STATE_ROTATE,	// 回転状態
		STATE_FOLLOW,	// 追従状態
		STATE_ZOOM,		// ズーム状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CCamera();

	// デストラクタ
	~CCamera();

	// カメラ構造体
	struct SCamera
	{
		D3DXVECTOR3		posV;			// 現在の視点
		D3DXVECTOR3		posR;			// 現在の注視点
		D3DXVECTOR3		destPosV;		// 目標の視点
		D3DXVECTOR3		destPosR;		// 目標の注視点
		D3DXVECTOR3		vecU;			// 上方向ベクトル
		D3DXVECTOR3		rot;			// 現在の向き
		D3DXVECTOR3		destRot;		// 目標の向き
		float			fDis;			// 現在の視点と注視点の距離
		float			fDestDis;		// 目標の視点と注視点の距離
		D3DXMATRIX		mtxProjection;	// プロジェクションマトリックス
		D3DXMATRIX		mtxView;		// ビューマトリックス
		D3DVIEWPORT9	viewport;		// ビューポート
	};

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void SetCamera(const EType type);		// カメラ設定
	SCamera GetCamera(const EType type);	// カメラ取得
	void SetState(const EState state);		// カメラ状態設定
	EState GetState(void) const;			// カメラ状態取得

	void SetDestRotate(void);	// カメラ目標位置設定 (回転)
	void SetDestFollow(void);	// カメラ目標位置設定 (追従)
	void SetEnableUpdate(const bool bUpdate);			// 更新状況設定
	void SetVec3Rotation(const D3DXVECTOR3& rRot);		// 現在向き設定
	void SetVec3DestRotation(const D3DXVECTOR3& rRot);	// 目標向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const;			// 現在向き取得
	D3DXVECTOR3 GetVec3DestRotation(void) const;		// 目標向き取得

	// 静的メンバ関数
	static CCamera *Create(void);				// 生成
	static HRESULT Release(CCamera *&prCamera);	// 破棄

private:
	// メンバ関数
	void Rotate(void);		// カメラの更新 (回転)
	void Follow(void);		// カメラの更新 (追従)
	void Zoom(void);		// カメラの更新 (ズーム)
	void Control(void);		// カメラの更新 (操作)
	void Move(void);		// 位置の更新 (操作)
	void Distance(void);	// 距離の更新 (操作)
	void Rotation(void);	// 向きの更新 (操作)

	// メンバ変数
	SCamera m_aCamera[TYPE_MAX];	// カメラの情報
	EState m_state;	// 状態
	bool m_bUpdate;	// 更新状況
};

#endif	// _CAMERA_H_
