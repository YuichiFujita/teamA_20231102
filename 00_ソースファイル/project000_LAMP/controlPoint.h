//============================================================
//
//	操作位置ヘッダー [controlPoint.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _CONTROL_POINT_H_
#define _CONTROL_POINT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object3D.h"

//************************************************************
//	インクルードファイル
//************************************************************
class CObjectBillboard;	// オブジェクトビルボードクラス

//************************************************************
//	クラス定義
//************************************************************
// 操作位置クラス
class CControlPoint : public CObject3D
{
public:
	// テクスチャ列挙
	enum ETexture
	{
		TEXTURE_FIELD = 0,	// 地面テクスチャ
		TEXTURE_NEXT,		// 次テクスチャ
		TEXTURE_PREV,		// 前テクスチャ
		TEXTURE_MAX			// この列挙型の総数
	};

	// レッスン列挙
	enum ELesson
	{
		LESSON_YOBIKOMI = 0,	// 呼び込みテクスチャ
		LESSON_01,				// テクスチャ
		LESSON_02,				// テクスチャ
		LESSON_03,				// テクスチャ
		LESSON_MAX				// この列挙型の総数
	};

	// コンストラクタ
	CControlPoint();

	// デストラクタ
	~CControlPoint();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定

	// 静的メンバ関数
	static CControlPoint *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rSize	// 大きさ
	);

private:
	// メンバ関数
	bool Collision(void);				// プレイヤー全員との当たり判定
	void UpdateTutorial(const int nID);	// チュートリアル操作
	void UpdateLesson(void);			// レッスン更新

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数
	static const char *mc_apLessonFile[];	// レッスンテクスチャ定数

	// メンバ変数
	CObjectBillboard *m_pTutorial;	// チュートリアル表示情報
	CObjectBillboard *m_pNext;		// 次操作の表示情報
	CObjectBillboard *m_pPrev;		// 前操作の表示情報
	int m_nLesson;		// レッスン
	float m_fScale;		// 拡大率
	float m_fDestScale;	// 目標拡大率
};

#endif	// _CONTROL_POINT_H_
