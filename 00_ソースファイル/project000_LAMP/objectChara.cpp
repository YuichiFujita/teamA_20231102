//============================================================
//
//	オブジェクトキャラクター処理 [objectChara.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectChara.h"
#include "manager.h"
#include "renderer.h"
#include "multiModel.h"
#include "motion.h"

//************************************************************
//	子クラス [CObjectChara] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectChara::CObjectChara()
{
	// メンバ変数をクリア
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// モデルの情報
	m_pMotion = NULL;	// モーションの情報
	m_pos = VEC3_ZERO;	// 位置
	m_rot = VEC3_ZERO;	// 向き
	m_nNumModel = 0;	// パーツの総数
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObjectChara::CObjectChara(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// モデルの情報
	m_pMotion = NULL;	// モーションの情報
	m_pos = VEC3_ZERO;	// 位置
	m_rot = VEC3_ZERO;	// 向き
	m_nNumModel = 0;	// パーツの総数
}

//============================================================
//	デストラクタ
//============================================================
CObjectChara::~CObjectChara()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectChara::Init(void)
{
	// メンバ変数を初期化
	m_pos = VEC3_ZERO;	// 位置
	m_rot = VEC3_ZERO;	// 向き
	m_nNumModel = 0;	// パーツの総数

	// モーションオブジェクトの生成
	m_pMotion = CMotion::Create();
	if (m_pMotion == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectChara::Uninit(void)
{
	for (int nCntObjectChara = 0; nCntObjectChara < MAX_PARTS; nCntObjectChara++)
	{ // パーツの最大数分繰り返す

		if (m_apMultiModel[nCntObjectChara] != NULL)
		{ // パーツが使用中の場合

			// マルチモデルの終了
			m_apMultiModel[nCntObjectChara]->Uninit();
		}
	}

	if (m_pMotion != NULL)
	{ // モーションが使われている場合

		// モーションを破棄
		if (FAILED(m_pMotion->Release(m_pMotion)))
		{ // 破棄に失敗した場合

			// 例外処理
			assert(false);
		}
	}
	else { assert(false); }	// 非使用中

	// オブジェクトキャラクターを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectChara::Update(void)
{
	// モーションの更新
	m_pMotion->Update();
}

//============================================================
//	描画処理
//============================================================
void CObjectChara::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntObjectChara = 0; nCntObjectChara < m_nNumModel; nCntObjectChara++)
	{ // パーツの総数分繰り返す

		// パーツの描画
		m_apMultiModel[nCntObjectChara]->Draw();
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CObjectChara::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObjectChara::GetVec3Position(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	向きの設定処理
//============================================================
void CObjectChara::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 引数の向きを設定
	m_rot = rRot;

	// 向きの正規化
	useful::Vec3NormalizeRot(m_rot);
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CObjectChara::GetVec3Rotation(void) const
{
	// 向きを返す
	return m_rot;
}

//============================================================
//	マトリックスポインタ取得処理
//============================================================
D3DXMATRIX *CObjectChara::GetPtrMtxWorld(void)
{
	// マトリックスのポインタを返す
	return &m_mtxWorld;
}

//============================================================
//	パーツ情報の設定処理
//============================================================
void CObjectChara::SetPartsInfo
(
	const int nID,				// パーツインデックス
	const int nParentID,		// 親インデックス
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const char *pFileName		// ファイル名
)
{
	if (nID < MAX_PARTS && nParentID < MAX_PARTS && pFileName != NULL)
	{ // パーツ・親インデックスが配列範囲内且つ、ファイル名が存在する場合

		// モデルの生成
		m_apMultiModel[nID] = CMultiModel::Create(rPos, rRot);

		// モデルを割当
		m_apMultiModel[nID]->BindModel(pFileName);

		// 親モデルの設定
		if (nParentID == NONE_IDX)
		{ // 親がない場合

			// NULLを設定
			m_apMultiModel[nID]->SetParentModel(NULL);
		}
		else
		{ // 親がいる場合

			// 親のアドレスを設定
			m_apMultiModel[nID]->SetParentModel(m_apMultiModel[nParentID]);
		}

		// パーツの総数を加算
		m_nNumModel++;
	}
}

//============================================================
//	モデル情報の設定処理
//============================================================
void CObjectChara::SetModelInfo(void)
{
	// モデル情報の設定
	m_pMotion->SetModel(&m_apMultiModel[0], m_nNumModel);
}

//============================================================
//	モーション情報の設定処理
//============================================================
void CObjectChara::SetMotionInfo(CMotion::SMotionInfo info)
{
	// モーション情報の設定
	m_pMotion->SetInfo(info);
}

//============================================================
//	モーションの設定処理
//============================================================
void CObjectChara::SetMotion(const int nType)
{
	// モーションの設定
	m_pMotion->Set(nType);
}

//============================================================
//	モーションの更新状況の設定処理
//============================================================
void CObjectChara::SetEnableMotionUpdate(const bool bUpdate)
{
	// 引数の更新状況をモーションの更新状況に設定
	m_pMotion->SetEnableUpdate(bUpdate);
}

//============================================================
//	パーツ位置の設定処理
//============================================================
void CObjectChara::SetPartsPosition(const int nID, const D3DXVECTOR3& rPos)
{
	// 引数のインデックスの位置を設定
	m_apMultiModel[nID]->SetVec3Position(rPos);
}

//============================================================
//	パーツ位置取得処理
//============================================================
D3DXVECTOR3 CObjectChara::GetPartsPosition(const int nID) const
{
	// 引数インデックスのパーツの位置を返す
	return m_apMultiModel[nID]->GetVec3Position();
}

//============================================================
//	パーツ向きの設定処理
//============================================================
void CObjectChara::SetPartsRotation(const int nID, const D3DXVECTOR3& rRot)
{
	// 引数のインデックスの向きを設定
	m_apMultiModel[nID]->SetVec3Rotation(rRot);
}

//============================================================
//	パーツ向き取得処理
//============================================================
D3DXVECTOR3 CObjectChara::GetPartsRotation(const int nID) const
{
	// 引数インデックスのパーツの向きを返す
	return m_apMultiModel[nID]->GetVec3Rotation();
}

//============================================================
//	マルチモデル取得処理
//============================================================
CMultiModel *CObjectChara::GetMultiModel(const int nID) const
{
	if (nID < m_nNumModel)
	{ // 使用可能なインデックスの場合

		// マルチモデルの情報を返す
		return m_apMultiModel[nID];
	}
	else { assert(false); return m_apMultiModel[0]; }
}

//============================================================
//	モーション種類取得処理
//============================================================
int CObjectChara::GetMotionType(void) const
{
	// モーションの種類を返す
	return m_pMotion->GetType();
}

//============================================================
//	モーションポーズ番号取得処理
//============================================================
int CObjectChara::GetMotionPose(void) const
{
	// モーションのポーズ番号を返す
	return m_pMotion->GetPose();
}

//============================================================
//	モーションカウンター取得処理
//============================================================
int CObjectChara::GetMotionCounter(void) const
{
	// モーションのカウンターを返す
	return m_pMotion->GetCounter();
}

//============================================================
//	モーション終了取得処理
//============================================================
bool CObjectChara::IsMotionFinish(void) const
{
	// モーションの終了状況を返す
	return m_pMotion->IsFinish();
}

//============================================================
//	モーションループ取得処理
//============================================================
bool CObjectChara::IsMotionLoop(const int nType) const
{
	// 引数の種類のモーションループ状況を返す
	return m_pMotion->IsLoop(nType);
}

//============================================================
//	マテリアルの設定処理
//============================================================
void CObjectChara::SetMaterial(const D3DXMATERIAL& rMat)
{
	for (int nCntObjectChara = 0; nCntObjectChara < m_nNumModel; nCntObjectChara++)
	{ // パーツの総数分繰り返す

		// 引数のマテリアルを全マテリアルに設定
		m_apMultiModel[nCntObjectChara]->SetAllMaterial(rMat);
	}
}

//============================================================
//	マテリアルの再設定処理
//============================================================
void CObjectChara::ResetMaterial(void)
{
	for (int nCntObjectChara = 0; nCntObjectChara < m_nNumModel; nCntObjectChara++)
	{ // パーツの総数分繰り返す

		// 全マテリアルに初期マテリアルを再設定
		m_apMultiModel[nCntObjectChara]->ResetMaterial();
	}
}

//============================================================
//	透明度の設定処理
//============================================================
void CObjectChara::SetAlpha(const float fAlpha)
{
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // パーツの最大数分繰り返す

		// 引数の透明度を設定
		m_apMultiModel[nCntParts]->SetAlpha(fAlpha);
	}
}

//============================================================
//	透明度取得処理
//============================================================
float CObjectChara::GetAlpha(void) const
{
	// 変数を宣言
	float fAlpha = 0.0f;	// 最も不透明なマテリアルの透明度

	// 最も不透明な透明度を探す
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // パーツの最大数分繰り返す

		if (m_apMultiModel[nCntParts]->GetAlpha() > fAlpha)
		{ // マテリアルの透明度がより不透明だった場合

			// 現在のマテリアルの透明度を保存
			fAlpha = m_apMultiModel[nCntParts]->GetAlpha();
		}
	}

	// 全パーツ内で最も不透明だったマテリアルの透明度を返す
	return fAlpha;
}

//============================================================
//	最大透明度取得処理
//============================================================
float CObjectChara::GetMaxAlpha(void) const
{
	// 変数を宣言
	float fAlpha = 0.0f;	// 最も不透明なマテリアルの透明度

	// 最も不透明な透明度を探す
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // パーツの最大数分繰り返す

		if (m_apMultiModel[nCntParts]->GetMaxAlpha() > fAlpha)
		{ // マテリアルの透明度がより不透明だった場合

			// 現在のマテリアルの透明度を保存
			fAlpha = m_apMultiModel[nCntParts]->GetMaxAlpha();
		}
	}

	// 全パーツ内で最も不透明だったマテリアルの透明度を返す
	return fAlpha;
}
