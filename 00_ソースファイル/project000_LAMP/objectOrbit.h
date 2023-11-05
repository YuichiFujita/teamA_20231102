//============================================================
//
//	オブジェクト軌跡ヘッダー [objectOrbit.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_ORBIT_H_
#define _OBJECT_ORBIT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_OFFSET		(2)		// オフセットの数
#define DEFAULT_PART	(10)	// デフォルトの分割数
#define DEFAULT_TEXPART	(1)		// デフォルトのテクスチャ分割数

//************************************************************
//	クラス定義
//************************************************************
// オブジェクト軌跡クラス
class CObjectOrbit : public CObject
{
public:
	// オフセット列挙
	enum EOffset
	{
		OFFSET_ARMDL = 0,	// 左下腕オフセット
		OFFSET_ARMDR,		// 右下腕オフセット
		OFFSET_LEGDL,		// 左脛オフセット
		OFFSET_LEGDR,		// 右脛オフセット
		OFFSET_MAX			// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_NORMAL,	// 通常状態
		STATE_VANISH,	// 消失状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CObjectOrbit();
	CObjectOrbit(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectOrbit();

	// 軌跡構造体
	struct SOrbit
	{
		D3DXVECTOR3	aOffset[MAX_OFFSET];		// 両端のオフセット
		D3DXCOLOR	aCol[MAX_OFFSET];			// 両端の基準色
		D3DXMATRIX	aMtxWorldPoint[MAX_OFFSET];	// 両端のワールドマトリックス
		D3DXVECTOR3	*pPosPoint;		// 各頂点座標
		D3DXCOLOR	*pColPoint;		// 各頂点カラー
		D3DXMATRIX	*pMtxParent;	// 親のマトリックス
		D3DXMATRIX	mtxVanish;		// 消失開始時の親のマトリックス
		int nPart;		// 分割数
		int nTexPart;	// テクスチャ分割数
		bool bAlpha;	// 透明化状況
		bool bInit;		// 初期化状況
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void BindTexture(const int nTextureID) override;		// テクスチャ割当 (インデックス)
	void BindTexture(const char *pTexturePass) override;	// テクスチャ割当 (パス)
	void SetColor(const D3DXCOLOR& rCol) override;			// 色設定
	int GetState(void) const override;	// 状態取得

	// 静的メンバ関数
	static CObjectOrbit *Create	// 生成
	( // 引数
		D3DXMATRIX *pMtxParent,	// 親マトリックス
		const D3DXCOLOR& rCol,	// 色
		const EOffset offset,	// オフセット
		const int nPart = DEFAULT_PART,			// 分割数
		const int nTexPart = DEFAULT_TEXPART,	// テクスチャ分割数
		const bool bAlpha = true				// 透明化状況
	);

	// メンバ関数
	void DeleteMatrixParent(void);					// 親マトリックス削除
	void SetState(const EState state);				// 状態設定
	void SetMatrixParent(D3DXMATRIX *pMtxParent);	// 親のマトリックス設定
	void SetOffset(const EOffset offset);			// オフセット設定
	void SetTexPart(const int nTexPart);			// テクスチャ分割数設定
	void SetEnableAlpha(const bool bAlpha);			// 透明化状況設定
	void SetEnableInit(const bool bInit);			// 初期化状況設定
	HRESULT SetLength(const int nPart);				// 長さ設定

protected:
	// メンバ関数
	void SetVtx(void);	// 頂点情報の設定

private:
	// 静的メンバ変数
	static const D3DXVECTOR3 mc_aOffset[][MAX_OFFSET];	// オフセットの位置加減量

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	SOrbit m_orbit;			// 軌跡の情報
	EState m_state;			// 状態
	int m_nCounterState;	// 状態管理カウンター
	int m_nNumVtx;			// 必要頂点数
	int m_nTextureID;		// テクスチャインデックス
};

#endif	// _OBJECT_ORBIT_H_
