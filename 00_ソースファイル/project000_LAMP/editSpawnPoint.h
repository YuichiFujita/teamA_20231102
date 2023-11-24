//============================================================
//
//	エディット生成位置ヘッダー [editSpawnPoint.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_SPAWNPOINT_H_
#define _EDIT_SPAWNPOINT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "editStage.h"
#include "spawnPoint.h"
#include "input.h"

//************************************************************
//	前方宣言
//************************************************************
class CEditStageManager;	// エディットステージマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// エディット生成位置クラス
class CEditSpawnPoint : public CEditStage
{
public:
	// コンストラクタ
	CEditSpawnPoint();

	// デストラクタ
	~CEditSpawnPoint() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void DrawDebugControl(void) override;	// 操作表示描画
	void DrawDebugInfo(void) override;		// 情報表示描画
	void SaveInfo(void) override;			// 情報保存
	void LoadInfo(void) override;			// 情報読込
	void Save(FILE *pFile) override;		// 保存

private:
	// メンバ関数
	void LookEffect	// 生成位置のエフェクト表示
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXCOLOR& rCol		// 色
	);

	void CreateSpawnPoint(void);	// 生成位置生成
	void UpdateChangeIdx(void);		// インデックス変更の更新
	void LookAllSpawnPoint(void);	// 生成位置の全表示
	void CheckUseSpawnPoint(const int nMaxPoint, CObject **ppAllSpawnPoint);	// 生成位置の使用確認

	// 静的メンバ変数
	static int m_nSave;	// 保存情報

	// メンバ変数
	CObject *m_apSpawnPoint[MAX_PLAYER];	// 生成位置情報
	int m_nIdxChange;	// 変更インデックス
};

#endif	// _EDIT_SPAWNPOINT_H_
