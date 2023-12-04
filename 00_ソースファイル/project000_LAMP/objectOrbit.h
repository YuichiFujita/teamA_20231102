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
	explicit CObjectOrbit(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectOrbit();

	// オフセット構造体
	struct SOffset
	{
	public:
		// コンストラクタ
		SOffset() {}

		SOffset(const D3DXVECTOR3& rOffset0, const D3DXVECTOR3& rOffset1, const D3DXCOLOR& rCol)
		{ aOffset[0] = rOffset0; aOffset[1] = rOffset1; aCol[0] = aCol[1] = rCol; }

		SOffset(const D3DXVECTOR3& rOffset0, const D3DXVECTOR3& rOffset1, const D3DXCOLOR& rCol0, const D3DXCOLOR& rCol1)
		{ aOffset[0] = rOffset0; aOffset[1] = rOffset1; aCol[0] = rCol0; aCol[1] = rCol1; }

		// デストラクタ
		~SOffset() {}

		// メンバ変数
		D3DXVECTOR3	aOffset[MAX_OFFSET];	// 両端のオフセット
		D3DXCOLOR	aCol[MAX_OFFSET];		// 両端の基準色
	};

	// 軌跡構造体
	struct SOrbit
	{
		D3DXMATRIX	aMtxWorldPoint[MAX_OFFSET];	// 両端のワールドマトリックス
		SOffset		offset;			// オフセット情報
		D3DXVECTOR3	*pPosPoint;		// 各頂点座標
		D3DXCOLOR	*pColPoint;		// 各頂点カラー
		D3DXMATRIX	*pMtxParent;	// 親のマトリックス
		D3DXMATRIX	mtxVanish;		// 消失開始時の親のマトリックス
		int nPart;		// 分割数
		int nTexPart;	// テクスチャ分割数
		bool bAdd;		// 加算合成状況
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
	int GetState(void) const override;	// 状態取得

	// 静的メンバ関数
	static CObjectOrbit *Create	// 生成
	( // 引数
		D3DXMATRIX *pMtxParent,	// 親マトリックス
		const SOffset offset,	// オフセット情報
		const int nPart		= DEFAULT_PART,		// 分割数
		const int nTexPart	= DEFAULT_TEXPART,	// テクスチャ分割数
		const bool bAdd		= true,				// 加算合成状況
		const bool bAlpha	= true				// 透明化状況
	);

	// メンバ関数
	void DeleteMatrixParent(void);					// 親マトリックス削除
	void SetState(const EState state);				// 状態設定
	void SetMatrixParent(D3DXMATRIX *pMtxParent);	// 親のマトリックス設定
	void SetOffset(const SOffset offset);			// オフセット情報設定
	void SetTexPart(const int nTexPart);			// テクスチャ分割数設定
	void SetEnableAdd(const bool bAdd);				// 加算合成状況設定
	void SetEnableAlpha(const bool bAlpha);			// 透明化状況設定
	void SetEnableInit(const bool bInit);			// 初期化状況設定
	HRESULT SetLength(const int nPart);				// 長さ設定

protected:
	// メンバ関数
	void SetVtx(void);	// 頂点情報の設定

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	SOrbit m_orbit;			// 軌跡の情報
	EState m_state;			// 状態
	int m_nCounterState;	// 状態管理カウンター
	int m_nNumVtx;			// 必要頂点数
	int m_nTextureID;		// テクスチャインデックス
};

#endif	// _OBJECT_ORBIT_H_
