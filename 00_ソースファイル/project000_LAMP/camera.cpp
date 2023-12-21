//============================================================
//
//	カメラ処理 [camera.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "camera.h"
#include "manager.h"
#include "sceneGame.h"
#include "renderer.h"
#include "input.h"
#include "player.h"
#include "stage.h"
#include "retentionManager.h"
#include "DepthShadow.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	// カメラ基本情報
	namespace basic
	{
		const D3DXVECTOR3 INIT_VECU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 上方向ベクトルの初期値
		const D3DXVECTOR3 INIT_POSV = D3DXVECTOR3(0.0f, 0.0f, -600.0f);	// 視点の初期値
		const float VIEW_NEAR	= 10.0f;	// モデルが見えるZ軸の最小値
		const float VIEW_FAR	= 50000.0f;	// モデルが見えるZ軸の最大値

		const float VIEW_ASPECT = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;	// アスペクト比
		const float VIEW_ANGLE = D3DXToRadian(45.0f);	// 視野角
	}

	// エントリーカメラ情報
	namespace entry
	{
		const D3DXVECTOR3 INIT_VECU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトルの初期値
		const D3DXVECTOR3 INIT_POSV = D3DXVECTOR3(406.0f, 52.0f, -1200.0f);	// 視点の初期値
		const D3DXVECTOR3 INIT_POSR = D3DXVECTOR3(406.0f, 52.0f, 0.0f);		// 注視点の初期値

		const float VIEW_WIDTH	= SCREEN_WIDTH * 0.85f;		// 画面の横幅
		const float VIEW_HEIGHT	= SCREEN_HEIGHT * 0.85f;	// 画面の縦幅
	}

	// 回転カメラ情報
	namespace rotate
	{
		const D3DXVECTOR3 INIT_POSR	= D3DXVECTOR3(0.0f, 550.0f, 0.0f);	// 回転カメラの注視点の初期値
		const D3DXVECTOR3 INIT_ROT	= D3DXVECTOR2(1.2f, 0.0f);			// 回転カメラの向き初期値

		const float INIT_DIS	= -6350.0f;	// 回転カメラの距離初期値
		const float ADD_ROTY	= 0.005f;	// 回転カメラの向き加算量Y
	}

	// 見下ろしカメラ情報
	namespace lookdown
	{
		const D3DXVECTOR3 INIT_ROT	= D3DXVECTOR3(2.45f, 0.0f, 0.0f);	// 見下ろしカメラの向きX初期値
		const float INIT_DIS	= 2400.0f;	// 見下ろしカメラの距離
		const float MUL_DIS		= 0.75f;	// 加算距離の乗算量
		const float REV_POS		= 0.025f;	// カメラ位置の補正係数
		const float REV_ROT		= 0.045f;	// カメラ向きの補正係数
	}

	// 操作カメラ情報
	namespace control
	{
		const float REV_MOVE_MOUSE	= 1.0f;		// マウス操作でのカメラの移動の補正係数
		const float REV_DIS_MOUSE	= -0.2f;	// マウス操作でのカメラの距離の補正係数
		const float REV_ROT_MOUSE	= 0.0045f;	// マウス操作でのカメラの回転量の補正係数

		const float MIN_DIS	= -10000.0f;	// カメラの視点から注視点への距離の最小
		const float MAX_DIS	= -1.0f;		// カメラの視点から注視点への距離の最大

		const float LIMIT_ROT_HIGH	= D3DX_PI - 0.1f;	// X上回転の制限値
		const float LIMIT_ROT_LOW	= 0.1f;				// X下回転の制限値
	}
	
	// エントリーカメラ情報
	namespace Result
	{
		const D3DXVECTOR3 INIT_VECU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトルの初期値
		const D3DXVECTOR3 INIT_POSV = D3DXVECTOR3(240.0f, 140.0f, -1000.0f);	// 視点の初期値
		const D3DXVECTOR3 INIT_POSR = D3DXVECTOR3(240.0f, 140.0f, 0.0f);		// 注視点の初期値

		const float VIEW_WIDTH = SCREEN_WIDTH * 0.65f;		// 画面の横幅
		const float VIEW_HEIGHT = SCREEN_HEIGHT * 0.65f;	// 画面の縦幅
	}
}

//************************************************************
//	親クラス [CCamera] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CCamera::CCamera()
{
	// メンバ変数をクリア
	memset(&m_aCamera[0], 0, sizeof(m_aCamera));	// カメラの情報
	m_state = STATE_NONE;	// 状態
	m_bUpdate = false;		// 更新状況
}

//============================================================
//	デストラクタ
//============================================================
CCamera::~CCamera()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CCamera::Init(void)
{
	//--------------------------------------------------------
	//	メンバ変数を初期化
	//--------------------------------------------------------
	m_state = STATE_NONE;	// 状態
	m_bUpdate = true;	 	// 更新状況

	//--------------------------------------------------------
	//	メインカメラの初期化
	//--------------------------------------------------------
	// カメラ情報を初期化
	m_aCamera[TYPE_MAIN].posV		= VEC3_ZERO;		// 現在の視点
	m_aCamera[TYPE_MAIN].posR		= VEC3_ZERO;		// 現在の注視点
	m_aCamera[TYPE_MAIN].destPosV	= VEC3_ZERO;		// 目標の視点
	m_aCamera[TYPE_MAIN].destPosR	= VEC3_ZERO;		// 目標の注視点
	m_aCamera[TYPE_MAIN].vecU		= basic::INIT_VECU;	// 上方向ベクトル
	m_aCamera[TYPE_MAIN].rot		= VEC3_ZERO;		// 現在の向き
	m_aCamera[TYPE_MAIN].destRot	= VEC3_ZERO;		// 目標の向き
	m_aCamera[TYPE_MAIN].fDis		= 0.0f;				// 現在の視点と注視点の距離
	m_aCamera[TYPE_MAIN].fDestDis	= 0.0f;				// 目標の視点と注視点の距離

	// ビューポート情報を初期化
	m_aCamera[TYPE_MAIN].viewport.X			= 0;				// 左上隅のピクセル座標 (x)
	m_aCamera[TYPE_MAIN].viewport.Y			= 0;				// 左上隅のピクセル座標 (y)
	m_aCamera[TYPE_MAIN].viewport.Width		= SCREEN_WIDTH;		// 描画する画面の横幅
	m_aCamera[TYPE_MAIN].viewport.Height	= SCREEN_HEIGHT;	// 描画する画面の縦幅
	m_aCamera[TYPE_MAIN].viewport.MinZ		= 0.0f;
	m_aCamera[TYPE_MAIN].viewport.MaxZ		= 1.0f;

		//--------------------------------------------------------
	//	リザルト表示カメラの初期化
	//--------------------------------------------------------
	// カメラ情報を初期化
	m_aCamera[TYPE_RESULT].posV = Result::INIT_POSV;	// 現在の視点
	m_aCamera[TYPE_RESULT].posR = Result::INIT_POSR;	// 現在の注視点
	m_aCamera[TYPE_RESULT].destPosV = Result::INIT_POSV;	// 目標の視点
	m_aCamera[TYPE_RESULT].destPosR = Result::INIT_POSR;	// 目標の注視点
	m_aCamera[TYPE_RESULT].vecU = Result::INIT_VECU;	// 上方向ベクトル
	m_aCamera[TYPE_RESULT].rot = VEC3_ZERO;		// 現在の向き
	m_aCamera[TYPE_RESULT].destRot = VEC3_ZERO;		// 目標の向き
	m_aCamera[TYPE_RESULT].fDis = 0.0f;				// 現在の視点と注視点の距離
	m_aCamera[TYPE_RESULT].fDestDis = 0.0f;				// 目標の視点と注視点の距離

	// ビューポート情報を初期化
	m_aCamera[TYPE_RESULT].viewport.X = 0;				// 左上隅のピクセル座標 (x)
	m_aCamera[TYPE_RESULT].viewport.Y = 0;				// 左上隅のピクセル座標 (y)
	m_aCamera[TYPE_RESULT].viewport.Width = SCREEN_WIDTH;		// 描画する画面の横幅
	m_aCamera[TYPE_RESULT].viewport.Height = SCREEN_HEIGHT;	// 描画する画面の縦幅
	m_aCamera[TYPE_RESULT].viewport.MinZ = 0.0f;
	m_aCamera[TYPE_RESULT].viewport.MaxZ = 0.5f;

	//--------------------------------------------------------
	//	モデルUI表示カメラの初期化
	//--------------------------------------------------------
	// カメラ情報を初期化
	m_aCamera[TYPE_MODELUI].posV		= basic::INIT_POSV;	// 現在の視点
	m_aCamera[TYPE_MODELUI].posR		= VEC3_ZERO;		// 現在の注視点
	m_aCamera[TYPE_MODELUI].destPosV	= basic::INIT_POSV;	// 目標の視点
	m_aCamera[TYPE_MODELUI].destPosR	= VEC3_ZERO;		// 目標の注視点
	m_aCamera[TYPE_MODELUI].vecU		= basic::INIT_VECU;	// 上方向ベクトル
	m_aCamera[TYPE_MODELUI].rot			= VEC3_ZERO;		// 現在の向き
	m_aCamera[TYPE_MODELUI].destRot		= VEC3_ZERO;		// 目標の向き
	m_aCamera[TYPE_MODELUI].fDis		= 0.0f;				// 現在の視点と注視点の距離
	m_aCamera[TYPE_MODELUI].fDestDis	= 0.0f;				// 目標の視点と注視点の距離

	// ビューポート情報を初期化
	m_aCamera[TYPE_MODELUI].viewport.X		= 0;				// 左上隅のピクセル座標 (x)
	m_aCamera[TYPE_MODELUI].viewport.Y		= 0;				// 左上隅のピクセル座標 (y)
	m_aCamera[TYPE_MODELUI].viewport.Width	= SCREEN_WIDTH;		// 描画する画面の横幅
	m_aCamera[TYPE_MODELUI].viewport.Height	= SCREEN_HEIGHT;	// 描画する画面の縦幅
	m_aCamera[TYPE_MODELUI].viewport.MinZ	= 0.0f;
	m_aCamera[TYPE_MODELUI].viewport.MaxZ	= 0.5f;

	//--------------------------------------------------------
	//	エントリー表示カメラの初期化
	//--------------------------------------------------------
	// カメラ情報を初期化
	m_aCamera[TYPE_ENTRY].posV		= entry::INIT_POSV;	// 現在の視点
	m_aCamera[TYPE_ENTRY].posR		= entry::INIT_POSR;	// 現在の注視点
	m_aCamera[TYPE_ENTRY].destPosV	= entry::INIT_POSV;	// 目標の視点
	m_aCamera[TYPE_ENTRY].destPosR	= entry::INIT_POSR;	// 目標の注視点
	m_aCamera[TYPE_ENTRY].vecU		= entry::INIT_VECU;	// 上方向ベクトル
	m_aCamera[TYPE_ENTRY].rot		= VEC3_ZERO;		// 現在の向き
	m_aCamera[TYPE_ENTRY].destRot	= VEC3_ZERO;		// 目標の向き
	m_aCamera[TYPE_ENTRY].fDis		= 0.0f;				// 現在の視点と注視点の距離
	m_aCamera[TYPE_ENTRY].fDestDis	= 0.0f;				// 目標の視点と注視点の距離

	// ビューポート情報を初期化
	m_aCamera[TYPE_ENTRY].viewport.X		= 0;				// 左上隅のピクセル座標 (x)
	m_aCamera[TYPE_ENTRY].viewport.Y		= 0;				// 左上隅のピクセル座標 (y)
	m_aCamera[TYPE_ENTRY].viewport.Width	= SCREEN_WIDTH;		// 描画する画面の横幅
	m_aCamera[TYPE_ENTRY].viewport.Height	= SCREEN_HEIGHT;	// 描画する画面の縦幅
	m_aCamera[TYPE_ENTRY].viewport.MinZ		= 0.0f;
	m_aCamera[TYPE_ENTRY].viewport.MaxZ		= 0.5f;

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CCamera::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CCamera::Update(void)
{
	if (m_bUpdate)
	{ // 更新する状況の場合

		switch (m_state)
		{ // 状態ごとの処理
		case STATE_NONE:		// なにもしない状態

			// 無し

			break;

		case STATE_CONTROL:		// 操作状態

			// カメラの更新 (操作)
			Control();

			break;

		case STATE_ROTATE:		// 回転状態

			// カメラの更新 (回転)
			Rotate();

			break;

		case STATE_LOOKDOWN:	// 見下ろし状態

			// カメラの更新 (見下ろし)
			LookDown();

			break;

		default:	// 例外処理
			assert(false);
			break;
		}
	}

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[ 視点 ]：%f %f %f\n", m_aCamera[TYPE_MAIN].posV.x, m_aCamera[TYPE_MAIN].posV.y, m_aCamera[TYPE_MAIN].posV.z);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[注視点]：%f %f %f\n", m_aCamera[TYPE_MAIN].posR.x, m_aCamera[TYPE_MAIN].posR.y, m_aCamera[TYPE_MAIN].posR.z);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[ 向き ]：%f %f %f\n", m_aCamera[TYPE_MAIN].rot.x, m_aCamera[TYPE_MAIN].rot.y, m_aCamera[TYPE_MAIN].rot.z);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[ 距離 ]：%f\n", m_aCamera[TYPE_MAIN].fDis);
}

//============================================================
//	カメラ設定処理
//============================================================
void CCamera::SetCamera(const EType type)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// ビューポートの設定
	pDevice->SetViewport(&m_aCamera[type].viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_aCamera[type].mtxProjection);

	switch (type)
	{ // カメラの種類ごとの処理
	case TYPE_MAIN:		// メインカメラ
	case TYPE_MODELUI:	// モデルUI表示カメラ

		// プロジェクションマトリックスを透視投影で作成
		D3DXMatrixPerspectiveFovLH
		( // 引数
			&m_aCamera[type].mtxProjection,	// プロジェクションマトリックス
			basic::VIEW_ANGLE,	// 視野角
			basic::VIEW_ASPECT,	// 画面のアスペクト比
			basic::VIEW_NEAR,	// Z軸の最小値
			basic::VIEW_FAR		// Z軸の最大値
		);

		break;

	case TYPE_ENTRY:	// エントリー表示カメラ

		// プロジェクションマトリックスを平行投影で作成
		D3DXMatrixOrthoLH
		( // 引数
			&m_aCamera[type].mtxProjection,	// プロジェクションマトリックス
			entry::VIEW_WIDTH,	// 画面の縦幅
			entry::VIEW_HEIGHT,	// 画面の横幅
			basic::VIEW_NEAR,	// Z軸の最小値
			basic::VIEW_FAR		// Z軸の最大値
		);

		break;
		
	case TYPE_RESULT:	//リザルト

						// プロジェクションマトリックスを平行投影で作成
		D3DXMatrixOrthoLH
		( // 引数
			&m_aCamera[type].mtxProjection,	// プロジェクションマトリックス
			Result::VIEW_WIDTH,				// 画面の縦幅
			Result::VIEW_HEIGHT,			// 画面の横幅
			basic::VIEW_NEAR,				// Z軸の最小値
			basic::VIEW_FAR					// Z軸の最大値
		);

		break;
	}

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_aCamera[type].mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_aCamera[type].mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH
	( // 引数
		&m_aCamera[type].mtxView,	// ビューマトリックス
		&m_aCamera[type].posV,		// 視点
		&m_aCamera[type].posR,		// 注視点
		&m_aCamera[type].vecU		// 上方向ベクトル
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_aCamera[type].mtxView);
	D3DXVECTOR3 posR = m_aCamera[type].posR;
	D3DXVECTOR3 posV = m_aCamera[type].posV;
	D3DXVECTOR3 cVec = posR - posV;
	D3DXVec3Normalize(&cVec, &cVec);
	cVec *= -5000.0f;
	posR = cVec + posV;
	D3DXMATRIX CameraView;
	D3DXMatrixLookAtLH(&CameraView, &posV, &posR, &D3DXVECTOR3(0, 1, 0));

	CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetCameraViewMatrix(&m_aCamera[type].mtxView);
//	CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetParamToEffect();
}

//============================================================
//	カメラ目標位置の設定処理 (回転)
//============================================================
void CCamera::SetDestRotate(void)
{
	if (m_state != STATE_ROTATE)
	{ // カメラの回転状態ではない場合

		// 処理を抜ける
		return;
	}

	//--------------------------------------------------------
	//	向きの更新
	//--------------------------------------------------------
	// 現在向きの更新
	m_aCamera[TYPE_MAIN].rot.x = rotate::INIT_ROT.x;
	m_aCamera[TYPE_MAIN].rot.y = rotate::INIT_ROT.y;
	useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化

	//--------------------------------------------------------
	//	距離の更新
	//--------------------------------------------------------
	m_aCamera[TYPE_MAIN].fDis = rotate::INIT_DIS;

	//--------------------------------------------------------
	//	位置の更新
	//--------------------------------------------------------
	// 注視点の更新
	m_aCamera[TYPE_MAIN].posR = rotate::INIT_POSR;

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].posR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].posR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].posR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));
}

//============================================================
//	カメラの目標位置の設定処理 (見下ろし)
//============================================================
void CCamera::SetDestLookDown(void)
{
	if (m_state != STATE_LOOKDOWN)
	{ // カメラの見下ろし状態ではない場合

		// 処理を抜ける
		return;
	}

	float fDis = lookdown::INIT_DIS;	// 距離
	D3DXVECTOR3 posLook = VEC3_ZERO;	// 注視点の位置

	//----------------------------------------------------
	//	注視点の位置を計算
	//----------------------------------------------------
	{
		bool bInit = false;				// 初期化状況
		D3DXVECTOR3 posMin = VEC3_ZERO;	// 最小の位置
		D3DXVECTOR3 posMax = VEC3_ZERO;	// 最大の位置

		for (int nCntCamera = 0; nCntCamera < MAX_PLAYER; nCntCamera++)
		{ // プレイヤーの最大数分繰り返す

			CPlayer *pPlayer = CScene::GetPlayer(nCntCamera);	// プレイヤー情報

			if (pPlayer != NULL)
			{ // プレイヤーが使用されている場合

				if (pPlayer->GetState() != CPlayer::STATE_DEATH)
				{ // 死んでいない場合

					D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();	// プレイヤー位置

					if (!bInit)
					{ // 初期化されていない場合

						// 最小と最大の位置を初期化
						posMin = posPlayer;
						posMax = posPlayer;

						// 初期化済みにする
						bInit = true;
					}
					else
					{ // 初期化されている場合

						// 最小位置の更新
						if (posPlayer.x < posMin.x)
						{ // 現在の最小位置より小さい場合

							posMin.x = posPlayer.x;
						}
						if (posPlayer.z < posMin.z)
						{ // 現在の最小位置より小さい場合

							posMin.z = posPlayer.z;
						}

						// 最大位置の更新
						if (posPlayer.x > posMax.x)
						{ // 現在の最大位置より大きい場合

							posMax.x = posPlayer.x;
						}
						if (posPlayer.z > posMax.z)
						{ // 現在の最大位置より大きい場合

							posMax.z = posPlayer.z;
						}
					}
				}
			}
		}

		// 注視点の位置を平均で求める
		posLook = (posMin + posMax) / 2;

		// カメラの距離を加算
		fDis += sqrtf((posMin.x - posMax.x) * (posMin.x - posMax.x) + (posMin.z - posMax.z) * (posMin.z - posMax.z)) * lookdown::MUL_DIS;
	}

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	// 向きを設定
	m_aCamera[TYPE_MAIN].rot = m_aCamera[TYPE_MAIN].destRot = lookdown::INIT_ROT;

	// 向きを正規化
	useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].rot);
	useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].destRot);

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 目標距離を設定
	m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis = fDis;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// 注視点の更新
	m_aCamera[TYPE_MAIN].destPosR = posLook;

	// 注視点の更新
	m_aCamera[TYPE_MAIN].posR = m_aCamera[TYPE_MAIN].destPosR = posLook;

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].destPosR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].destPosR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].destPosR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));
}

//============================================================
//	更新状況の設定処理
//============================================================
void CCamera::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	m_bUpdate = bUpdate;
}

//============================================================
//	向きの設定処理
//============================================================
void CCamera::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 引数のカメラの向きを設定
	m_aCamera[TYPE_MAIN].rot = rRot;

	// 向きを正規化
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot.x);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot.y);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot.z);
}

//============================================================
//	目標向きの設定処理
//============================================================
void CCamera::SetVec3DestRotation(const D3DXVECTOR3& rRot)
{
	// 引数のカメラの目標向きを設定
	m_aCamera[TYPE_MAIN].destRot = rRot;

	// 向きを正規化
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot.x);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot.y);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot.z);
}

//============================================================
//	カメラ取得処理
//============================================================
CCamera::SCamera CCamera::GetCamera(const EType type)
{
	// カメラの情報を返す
	return m_aCamera[type];
}

//============================================================
//	カメラ状態の設定処理
//============================================================
void CCamera::SetState(const EState state)
{
	// 状態を設定
	m_state = state;
}

//============================================================
//	カメラ状態取得処理
//============================================================
CCamera::EState CCamera::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	ビューポートの設定処理
//============================================================
void CCamera::SetViewport(const EType type, const D3DVIEWPORT9& rViewport)
{
	// 引数種類のビューポートを設定
	m_aCamera[type].viewport = rViewport;
}

//============================================================
//	ビューポート取得処理
//============================================================
D3DVIEWPORT9 CCamera::GetViewport(const EType type) const
{
	// 引数種類のビューポートを返す
	return m_aCamera[type].viewport;
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CCamera::GetVec3Rotation(void) const
{
	// カメラの向きを返す
	return m_aCamera[TYPE_MAIN].rot;
}

//============================================================
//	目標向き取得処理
//============================================================
D3DXVECTOR3 CCamera::GetVec3DestRotation(void) const
{
	// カメラの目標向きを返す
	return m_aCamera[TYPE_MAIN].destRot;
}

//============================================================
//	生成処理
//============================================================
CCamera *CCamera::Create(void)
{
	// ポインタを宣言
	CCamera *pCamera = NULL;	// カメラ生成用

	if (pCamera == NULL)
	{ // 使用されていない場合

		// メモリを確保
		pCamera = new CCamera;	// カメラ
	}
	else { assert(false); return NULL; }	// 使用中

	if (pCamera != NULL)
	{ // 確保に成功している場合

		// カメラの初期化
		if (FAILED(pCamera->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pCamera;
			pCamera = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pCamera;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CCamera::Release(CCamera *&prCamera)
{
	if (prCamera != NULL)
	{ // 使用中の場合

		// カメラの終了
		prCamera->Uninit();

		// メモリ開放
		delete prCamera;
		prCamera = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	カメラの更新処理 (回転)
//============================================================
void CCamera::Rotate(void)
{
	//--------------------------------------------------------
	//	向きの更新
	//--------------------------------------------------------
	// 現在向きの更新
	m_aCamera[TYPE_MAIN].rot.x = rotate::INIT_ROT.x;
	m_aCamera[TYPE_MAIN].rot.y += rotate::ADD_ROTY;
	useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化

	//--------------------------------------------------------
	//	距離の更新
	//--------------------------------------------------------
	m_aCamera[TYPE_MAIN].fDis = rotate::INIT_DIS;

	//--------------------------------------------------------
	//	位置の更新
	//--------------------------------------------------------
	// 注視点の更新
	m_aCamera[TYPE_MAIN].posR = rotate::INIT_POSR;

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].posR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].posR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].posR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));
}

//============================================================
//	カメラの更新処理 (見下ろし)
//============================================================
void CCamera::LookDown(void)
{
	// 変数を宣言
	D3DXVECTOR3 posLook  = VEC3_ZERO;	// 注視点の位置
	D3DXVECTOR3 posMin   = VEC3_ZERO;	// 最小の位置
	D3DXVECTOR3 posMax   = VEC3_ZERO;	// 最大の位置
	D3DXVECTOR3 diffPosV = VEC3_ZERO;	// 視点の差分位置
	D3DXVECTOR3 diffPosR = VEC3_ZERO;	// 注視点の差分位置
	D3DXVECTOR3 diffRot  = VEC3_ZERO;	// 差分向き
	float fDis = lookdown::INIT_DIS;	// 距離
	bool bInit = false;					// 初期化状況

	//----------------------------------------------------
	//	注視点の位置を計算
	//----------------------------------------------------
	for (int nCntCamera = 0; nCntCamera < MAX_PLAYER; nCntCamera++)
	{ // プレイヤーの最大数分繰り返す

		CPlayer *pPlayer = CScene::GetPlayer(nCntCamera);	// プレイヤー情報

		if (pPlayer != NULL)
		{ // プレイヤーが使用されている場合

			if (pPlayer->GetState() != CPlayer::STATE_DEATH)
			{ // 死んでいない場合

				D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();	// プレイヤー位置

				if (!bInit)
				{ // 初期化されていない場合

					// 最小と最大の位置を初期化
					posMin = posPlayer;
					posMax = posPlayer;

					// 初期化済みにする
					bInit = true;
				}
				else
				{ // 初期化されている場合

					// 最小位置の更新
					if (posPlayer.x < posMin.x)
					{ // 現在の最小位置より小さい場合

						posMin.x = posPlayer.x;
					}
					if (posPlayer.z < posMin.z)
					{ // 現在の最小位置より小さい場合

						posMin.z = posPlayer.z;
					}

					// 最大位置の更新
					if (posPlayer.x > posMax.x)
					{ // 現在の最大位置より大きい場合

						posMax.x = posPlayer.x;
					}
					if (posPlayer.z > posMax.z)
					{ // 現在の最大位置より大きい場合

						posMax.z = posPlayer.z;
					}
				}
			}
		}
	}

	// 注視点の位置を平均で求める
	posLook = (posMin + posMax) / 2;

	// カメラの距離を加算
	fDis += sqrtf((posMin.x - posMax.x) * (posMin.x - posMax.x) + (posMin.z - posMax.z) * (posMin.z - posMax.z)) * lookdown::MUL_DIS;

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[最小位置]：%f %f %f\n", posMin.x, posMin.y, posMin.z);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[最大位置]：%f %f %f\n", posMax.x, posMax.y, posMax.z);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[距離]：%f\n", fDis);

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	// 目標向きを設定
	m_aCamera[TYPE_MAIN].destRot = lookdown::INIT_ROT;

	// 目標向きを正規化
	useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].destRot);

	// 差分向きを計算
	diffRot = m_aCamera[TYPE_MAIN].destRot - m_aCamera[TYPE_MAIN].rot;
	useful::Vec3NormalizeRot(diffRot);	// 差分向きを正規化

	// 現在向きの更新
	m_aCamera[TYPE_MAIN].rot += diffRot * lookdown::REV_ROT;
	useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 目標距離を設定
	m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis = fDis;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// 注視点の更新
	m_aCamera[TYPE_MAIN].destPosR = posLook;

	// 視点の更新
	m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].destPosR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].destPosR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].destPosR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 注視点の差分位置を計算
	diffPosR = m_aCamera[TYPE_MAIN].destPosR - m_aCamera[TYPE_MAIN].posR;

	// 視点の差分位置を計算
	diffPosV = m_aCamera[TYPE_MAIN].destPosV - m_aCamera[TYPE_MAIN].posV;

	// 注視点の現在位置を更新
	m_aCamera[TYPE_MAIN].posR.x += diffPosR.x * lookdown::REV_POS;
	m_aCamera[TYPE_MAIN].posR.y += diffPosR.y * lookdown::REV_POS;
	m_aCamera[TYPE_MAIN].posR.z += diffPosR.z * lookdown::REV_POS;

	// 視点の現在位置を更新
	m_aCamera[TYPE_MAIN].posV.x += diffPosV.x * lookdown::REV_POS;
	m_aCamera[TYPE_MAIN].posV.y += diffPosV.y * lookdown::REV_POS;
	m_aCamera[TYPE_MAIN].posV.z += diffPosV.z * lookdown::REV_POS;
}

//============================================================
//	カメラの更新処理 (操作)
//============================================================
void CCamera::Control(void)
{
	// 位置の更新
	Move();

	// 距離の更新
	Distance();

	// 向きの更新
	Rotation();
}

//============================================================
//	位置の更新処理 (操作)
//============================================================
void CCamera::Move(void)
{
	// ポインタを宣言
	CInputMouse	*pMouse = CManager::GetInstance()->GetMouse();	// マウスの取得

	// 変数を宣言
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// マウスの移動量

	// マウス操作の更新
	if (pMouse->IsPress(CInputMouse::KEY_LEFT) && pMouse->IsPress(CInputMouse::KEY_RIGHT))
	{ // 右クリックと左クリックが押されている場合

		// 視点を移動
		m_aCamera[TYPE_MAIN].posV.x -= sinf(m_aCamera[TYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posV.z -= cosf(m_aCamera[TYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posV.x += sinf(m_aCamera[TYPE_MAIN].rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posV.z += cosf(m_aCamera[TYPE_MAIN].rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;

		// 注視点を移動
		m_aCamera[TYPE_MAIN].posR.x -= sinf(m_aCamera[TYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posR.z -= cosf(m_aCamera[TYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posR.x += sinf(m_aCamera[TYPE_MAIN].rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posR.z += cosf(m_aCamera[TYPE_MAIN].rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;
	}
}

//============================================================
//	距離の更新処理 (操作)
//============================================================
void CCamera::Distance(void)
{
	// ポインタを宣言
	CInputMouse	*pMouse = CManager::GetInstance()->GetMouse();	// マウスの取得

	// 変数を宣言
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// マウスの移動量

	// マウス操作の更新
	if (mouseMove.z != 0.0f)
	{ // マウスホイールが操作された場合

		// 距離を加算
		m_aCamera[TYPE_MAIN].fDis += mouseMove.z * control::REV_DIS_MOUSE;
	}

	// 距離の補正
	useful::LimitNum(m_aCamera[TYPE_MAIN].fDis, control::MIN_DIS, control::MAX_DIS);
}

//============================================================
//	向きの更新処理 (操作)
//============================================================
void CCamera::Rotation(void)
{
	// ポインタを宣言
	CInputMouse	*pMouse = CManager::GetInstance()->GetMouse();	// マウスの取得

	// 変数を宣言
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// マウスの移動量

	// マウス操作の更新
	if (pMouse->IsPress(CInputMouse::KEY_LEFT) && !pMouse->IsPress(CInputMouse::KEY_RIGHT))
	{ // 左クリックだけが押されている場合

		// カメラのY軸を回転
		m_aCamera[TYPE_MAIN].rot.y += mouseMove.x * control::REV_ROT_MOUSE;

		// カメラのX軸を回転
		m_aCamera[TYPE_MAIN].rot.x += mouseMove.y * control::REV_ROT_MOUSE;
	}

	// 向きの補正
	useful::LimitNum(m_aCamera[TYPE_MAIN].rot.x, control::LIMIT_ROT_LOW, control::LIMIT_ROT_HIGH);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot.y);

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].posR.x + ((m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].posR.y + ((m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].posR.z + ((m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// マウス操作の更新
	if (pMouse->IsPress(CInputMouse::KEY_RIGHT) && !pMouse->IsPress(CInputMouse::KEY_LEFT))
	{ // 右クリックだけが押されている場合

		// カメラのY軸を回転
		m_aCamera[TYPE_MAIN].rot.y += mouseMove.x * control::REV_ROT_MOUSE;

		// カメラのX軸を回転
		m_aCamera[TYPE_MAIN].rot.x += mouseMove.y * control::REV_ROT_MOUSE;
	}

	// 向きの補正
	useful::LimitNum(m_aCamera[TYPE_MAIN].rot.x, control::LIMIT_ROT_LOW, control::LIMIT_ROT_HIGH);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot.y);

	// 注視点の更新
	m_aCamera[TYPE_MAIN].posR.x = m_aCamera[TYPE_MAIN].posV.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posR.y = m_aCamera[TYPE_MAIN].posV.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posR.z = m_aCamera[TYPE_MAIN].posV.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));
}
