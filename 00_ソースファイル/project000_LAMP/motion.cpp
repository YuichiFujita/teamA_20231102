//============================================================
//
//	モーション処理 [motion.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "motion.h"
#include "multiModel.h"

//************************************************************
//	マクロ定義
//************************************************************
#define SUB_STOP	(2)	// ループしないモーションの停止カウントの減算用

//************************************************************
//	親クラス [CMotion] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CMotion::CMotion()
{
	// メンバ変数をクリア
	memset(&m_info, 0, sizeof(m_info));	// モーション情報
	m_ppModel = NULL;	// モデル情報
	m_nNumModel = 0;	// モデルのパーツ数
	m_bUpdate = true;	// 更新状況
}

//============================================================
//	デストラクタ
//============================================================
CMotion::~CMotion()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CMotion::Init(void)
{
	// メンバ変数をクリア
	memset(&m_info, 0, sizeof(m_info));	// モーション情報
	m_ppModel = NULL;	// モデル情報
	m_nNumModel = 0;	// モデルのパーツ数
	m_bUpdate = true;	// 更新状況

	// モーションを終了状態にする
	m_info.bFinish = true;

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CMotion::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CMotion::Update(void)
{
	// 変数を宣言
	D3DXVECTOR3 diffPos;		// 次ポーズまでの差分位置
	D3DXVECTOR3 diffRot;		// 次ポーズまでの差分向き
	D3DXVECTOR3 currentPos;		// 現在フレームの位置
	D3DXVECTOR3 currentRot;		// 現在フレームの向き
	int nType = m_info.nType;	// モーション種類
	int nPose = m_info.nPose;	// モーションポーズ番号
	int nNextPose;				// 次のモーションポーズ番号

	if (m_bUpdate)
	{ // 更新する状況の場合

		if (m_info.aMotionInfo[nType].nNumKey > 0)
		{ // キーが設定されている場合

			// 次のモーションポーズ番号を求める
			nNextPose = (nPose + 1) % m_info.aMotionInfo[nType].nNumKey;

			// パーツの位置の更新
			for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
			{ // モデルのパーツ数分繰り返す

				// 位置・向きの差分を求める
				diffPos = m_info.aMotionInfo[nType].aKeyInfo[nNextPose].aKey[nCntKey].pos - m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].pos;
				diffRot = m_info.aMotionInfo[nType].aKeyInfo[nNextPose].aKey[nCntKey].rot - m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].rot;

				// 差分向きの正規化
				useful::NormalizeRot(diffRot.x);
				useful::NormalizeRot(diffRot.y);
				useful::NormalizeRot(diffRot.z);

				// 現在のパーツの位置・向きを更新
				m_ppModel[nCntKey]->SetVec3Position(m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].pos + diffPos * ((float)m_info.nCounter / (float)m_info.aMotionInfo[nType].aKeyInfo[nPose].nFrame));
				m_ppModel[nCntKey]->SetVec3Rotation(m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].rot + diffRot * ((float)m_info.nCounter / (float)m_info.aMotionInfo[nType].aKeyInfo[nPose].nFrame));
			}

			// モーションの遷移の更新
			if (m_info.nCounter >= m_info.aMotionInfo[nType].aKeyInfo[nPose].nFrame)
			{ // 現在のモーションカウンターが現在のポーズの再生フレーム数を超えている場合

				// 次のポーズに移行
				if (m_info.aMotionInfo[nType].bLoop == true)
				{ // モーションがループする設定の場合

					// モーションカウンターを初期化
					m_info.nCounter = 0;

					// ポーズカウントを加算 (総数に達した場合 0に戻す)
					m_info.nPose = (m_info.nPose + 1) % m_info.aMotionInfo[nType].nNumKey;
				}
				else
				{ // モーションがループしない設定の場合

					if (m_info.nPose < m_info.aMotionInfo[nType].nNumKey - SUB_STOP)
					{ // 現在のポーズが最終のポーズではない場合

						// モーションカウンターを初期化
						m_info.nCounter = 0;

						// ポーズカウントを加算
						m_info.nPose++;
					}
					else
					{ // 現在のポーズが最終のポーズの場合

						// モーションを終了状態にする
						m_info.bFinish = true;
					}
				}
			}
			else
			{ // 現在のモーションカウンターが現在のポーズの再生フレーム数を超えていない場合

				// モーションカウンターを加算
				m_info.nCounter++;
			}
		}
	}
}

//============================================================
//	設定処理
//============================================================
void CMotion::Set(const int nType)
{
	// モーション情報を初期化
	m_info.nPose	= 0;		// モーションポーズ番号
	m_info.nCounter	= 0;		// モーションカウンター
	m_info.bFinish	= false;	// モーション終了状況

	// 引数のモーションの種類を設定
	m_info.nType = nType;

	// モーションを再生状態にする
	m_info.bFinish = false;

	// パーツの位置の初期化
	for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
	{ // モデルのパーツ数分繰り返す

		// 初期位置と初期向きを設定
		m_ppModel[nCntKey]->SetVec3Position(m_info.aMotionInfo[nType].aKeyInfo[m_info.nPose].aKey[nCntKey].pos);
		m_ppModel[nCntKey]->SetVec3Rotation(m_info.aMotionInfo[nType].aKeyInfo[m_info.nPose].aKey[nCntKey].rot);
	}
}

//============================================================
//	モーション情報の設定処理
//============================================================
void CMotion::SetInfo(const SMotionInfo info)
{
	// 引数のモーション情報を設定
	m_info.aMotionInfo[m_info.nNumMotion] = info;

	// モーションの情報数を加算
	m_info.nNumMotion++;

	// 例外処理
	assert(m_info.nNumMotion <= MAX_MOTION);	// モーション数オーバー
}

//============================================================
//	更新状況の設定処理
//============================================================
void CMotion::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	m_bUpdate = bUpdate;
}

//============================================================
//	モデル情報の設定処理
//============================================================
void CMotion::SetModel(CMultiModel **ppModel, const int nNum)
{
	// 引数のモデル情報を設定
	m_ppModel = ppModel;

	// 引数のモデルのパーツ数を設定
	m_nNumModel = nNum;

	// 例外処理
	assert(m_nNumModel <= MAX_PARTS);	// パーツ数オーバー
}

//============================================================
//	種類取得処理
//============================================================
int CMotion::GetType(void) const
{
	// 現在のモーションの種類を返す
	return m_info.nType;
}

//============================================================
//	ポーズ番号取得処理
//============================================================
int CMotion::GetPose(void) const
{
	// 現在のポーズ番号を返す
	return m_info.nPose;
}

//============================================================
//	モーションカウンター取得処理
//============================================================
int CMotion::GetCounter(void) const
{
	// 現在のモーションカウンターを返す
	return m_info.nCounter;
}

//============================================================
//	終了取得処理
//============================================================
bool CMotion::IsFinish(void) const
{
	// 現在のモーションの終了状況を返す
	return m_info.bFinish;
}

//============================================================
//	ループ取得処理
//============================================================
bool CMotion::IsLoop(const int nType) const
{
	// 現在のループのON/OFF状況を返す
	return m_info.aMotionInfo[nType].bLoop;
}

//============================================================
//	生成処理
//============================================================
CMotion *CMotion::Create(void)
{
	// ポインタを宣言
	CMotion *pMotion = NULL;	// モーション生成用

	if (pMotion == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pMotion = new CMotion;	// モーション
	}
	else { assert(false); return NULL; }	// 使用中

	if (pMotion != NULL)
	{ // 確保に成功している場合

		// モーションの初期化
		if (FAILED(pMotion->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pMotion;
			pMotion = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pMotion;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CMotion::Release(CMotion *&prMotion)
{
	if (prMotion != NULL)
	{ // 使用中の場合

		// モーションの終了
		prMotion->Uninit();

		// メモリ開放
		delete prMotion;
		prMotion = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
