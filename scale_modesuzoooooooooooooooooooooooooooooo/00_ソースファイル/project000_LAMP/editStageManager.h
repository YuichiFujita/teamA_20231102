//============================================================
//
//	エディットステージマネージャーヘッダー [editStageManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_STAGEMANAGER_H_
#define _EDIT_STAGEMANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "editStage.h"

//************************************************************
//	クラス定義
//************************************************************
// エディットステージマネージャークラス
class CEditStageManager
{
public:
	// コンストラクタ
	CEditStageManager();

	// デストラクタ
	~CEditStageManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void UnSave(void);	// 未保存設定
	void SetEnableEdit(const bool bEdit);		// エディット状況設定
	bool IsEdit(void) const;					// エディット状況取得
	D3DXVECTOR3 GetVec3Position(void) const;	// 位置取得
	D3DXVECTOR3 GetVec3Rotation(void) const;	// 向き取得
	D3DXVECTOR3 GetVec3Sizing(void) const;		// 大きさ取得

	// 静的メンバ関数
	static CEditStageManager *Create(void);	// 生成
	static HRESULT Release(CEditStageManager *&prEditStageManager);	// 破棄

private:
	// メンバ関数
	void UpdateChangeThing(void);	// 配置物変更の更新
	void UpdateChangeMove(void);	// 移動量変更の更新
	void UpdatePosition(void);		// 位置更新
	void UpdateRotation(void);		// 向き更新
	void UpdateSizing(void);		// 大きさ更新
	void DrawDebugControl(void);	// 操作表示描画
	void DrawDebugInfo(void);		// 情報表示描画
	void SaveStage(void);			// ステージ保存
	void Save(void);				// 保存

	// メンバ変数
	CEditStage	*m_pStage;		// エディットステージの情報
	CEditStage::EThing m_thing;	// 配置物
	D3DXVECTOR3	m_pos;	// 位置
	D3DXVECTOR3	m_rot;	// 向き
	D3DXVECTOR3	m_size;	// 大きさ
	float m_fMove;		// 位置移動量
	bool m_bSave;		// 保存状況
	bool m_bEdit;		// エディット状況
};

#endif	// _EDIT_STAGEMANAGER_H_
