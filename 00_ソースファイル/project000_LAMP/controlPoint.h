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
		TEXTURE_NORMAL = 0,	// 通常テクスチャ
		TEXTURE_MAX			// この列挙型の総数
	};

	// レッスン列挙
	enum ELesson
	{
		LESSON_YOBIKOMI = 0,	// 呼び込みテクスチャ
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
	void Collision(void);	// プレイヤー全員との当たり判定
	void UpdateTutorial(const int nID);	// チュートリアル操作

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数
	static const char *mc_apLessonFile[];	// レッスンテクスチャ定数

	// メンバ変数
	CObjectBillboard *m_pTutorial;	// チュートリアル表示情報
};

#endif	// _CONTROL_POINT_H_
