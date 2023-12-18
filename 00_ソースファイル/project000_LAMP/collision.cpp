//============================================================
//
//	判定関数処理 [collision.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "collision.h"
#include "manager.h"
#include "renderer.h"

//============================================================
//	XZ平面の矩形の当たり判定
//============================================================
bool collision::Box2D
(
	D3DXVECTOR3 centerPos,		// 判定位置
	D3DXVECTOR3 targetPos,		// 判定目標位置
	D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
	D3DXVECTOR3 targetSizeDown	// 判定目標サイズ(左・下・前)
)
{
	if (centerPos.x + centerSizeUp.x   > targetPos.x - targetSizeDown.x
	&&  centerPos.z + centerSizeUp.z   > targetPos.z - targetSizeDown.z
	&&  centerPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x
	&&  centerPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // 判定内の場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	3軸の矩形の当たり判定
//============================================================
bool collision::Box3D
(
	D3DXVECTOR3 centerPos,		// 判定位置
	D3DXVECTOR3 targetPos,		// 判定目標位置
	D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
	D3DXVECTOR3 targetSizeDown	// 判定目標サイズ(左・下・前)
)
{
	if (centerPos.x + centerSizeUp.x   > targetPos.x - targetSizeDown.x
	&&  centerPos.y + centerSizeUp.y   > targetPos.y - targetSizeDown.y
	&&  centerPos.z + centerSizeUp.z   > targetPos.z - targetSizeDown.z
	&&  centerPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x
	&&  centerPos.y - centerSizeDown.y < targetPos.y + targetSizeUp.y
	&&  centerPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // 判定内の場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	XZ平面の円の当たり判定
//============================================================
bool collision::Circle2D
(
	D3DXVECTOR3 centerPos,	// 判定位置
	D3DXVECTOR3 targetPos,	// 判定目標位置
	float fCenterRadius,	// 判定半径
	float fTargetRadius,	// 判定目標半径
	float *pLength			// 判定目標との距離
)
{
	// 変数を宣言
	float fLength  = 0.0f;	// 判定位置と判定目標位置の間の距離

	// 判定位置と判定目標位置の距離を求める
	fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
			+ (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (pLength != NULL)
	{ // ポインタが使用されている場合

		// 判定目標との距離を代入
		*pLength = fLength;
	}

	if (fLength < ((fCenterRadius + fTargetRadius) * (fCenterRadius + fTargetRadius)))
	{ // 判定内の場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	3軸の円の当たり判定
//============================================================
bool collision::Circle3D
(
	D3DXVECTOR3 centerPos,	// 判定位置
	D3DXVECTOR3 targetPos,	// 判定目標位置
	float fCenterRadius,	// 判定半径
	float fTargetRadius		// 判定目標半径
)
{
	// 変数を宣言
	float fLength  = 0.0f;	// 判定位置と判定目標位置の間の距離

	// 判定位置と判定目標位置の距離を求める
	fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
			+ (centerPos.y - targetPos.y) * (centerPos.y - targetPos.y)
			+ (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (fLength < ((fCenterRadius + fTargetRadius) * (fCenterRadius + fTargetRadius)))
	{ // 判定内の場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	XZ平面の扇形の当たり判定
//============================================================
bool collision::Sector
(
	D3DXVECTOR3 centerPos,	// 判定位置
	D3DXVECTOR3 targetPos,	// 判定目標位置
	float fCenterRot,		// 判定向き
	float fRadius,			// 視界範囲
	float fAngle,			// 視野角
	float *pLength			// 判定目標との距離
)
{
	// 変数を宣言
	bool  bHit = false;		// 当たり判定の結果
	float fLength;			// 中心位置と目標位置の距離
	float fHalfAngle;		// 引数の角度の半分の値の代入用
	D3DXVECTOR3 vecToPos;	// 左端と位置のベクトル

	// 変数配列を宣言
	float fRotEdge[2];		// 扇形の縁の角度     [※] 0：左 1：右
	D3DXVECTOR3 posEdge[2];	// 扇形の縁の先端位置 [※] 0：左 1：右
	D3DXVECTOR3 vecEdge[2];	// 扇形の縁ベクトル   [※] 0：左 1：右

	// 中心位置と目標位置の距離求める
	fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
			+ (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (pLength != NULL)
	{ // ポインタが使用されている場合

		// 判定目標との距離を代入
		*pLength = fLength;
	}

	if (fLength < fRadius * fRadius)
	{ // 円の範囲内の場合

		// 引数の角度の半分の値を求める
		fHalfAngle = fAngle * 0.5f;

		// 扇形の左縁の角度を求める
		fRotEdge[0] = fCenterRot + fHalfAngle;	// 角度を左に傾ける
		useful::NormalizeRot(fRotEdge[0]);		// 向きを正規化

		// 扇形の右縁の角度を求める
		fRotEdge[1] = fCenterRot - fHalfAngle;	// 角度を右に傾ける
		useful::NormalizeRot(fRotEdge[1]);		// 向きを正規化

		// 扇形の左縁の先端位置を求める
		posEdge[0].x = centerPos.x + sinf(fRotEdge[0]) * 1.0f;
		posEdge[0].y = 0.0f;
		posEdge[0].z = centerPos.z + cosf(fRotEdge[0]) * 1.0f;

		// 扇形の右縁の先端位置を求める
		posEdge[1].x = centerPos.x + sinf(fRotEdge[1]) * 1.0f;
		posEdge[1].y = 0.0f;
		posEdge[1].z = centerPos.z + cosf(fRotEdge[1]) * 1.0f;

		// 扇形の左縁のベクトルを求める
		vecEdge[0] = posEdge[0] - centerPos;
		vecEdge[0].y = 0.0f;

		// 扇形の右縁のベクトルを求める
		vecEdge[1] = posEdge[1] - centerPos;
		vecEdge[1].y = 0.0f;

		// 左端と位置のベクトルを求める
		vecToPos = targetPos - centerPos;

		if ((vecEdge[0].z * vecToPos.x) - (vecEdge[0].x * vecToPos.z) < 0
		&&  (vecEdge[1].z * vecToPos.x) - (vecEdge[1].x * vecToPos.z) > 0)
		{ // 扇形の両縁の範囲内の場合

			// 当たっている状態にする
			bHit = true;
		}
	}

	// 当たり判定の結果を返す
	return bHit;
}

//============================================================
//	角柱の衝突判定
//============================================================
bool collision::BoxPillar
(
	D3DXVECTOR3& rCenterPos,	// 判定位置
	D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
	D3DXVECTOR3 targetPos,		// 判定目標位置
	D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
	D3DXVECTOR3 targetSizeDown	// 判定目標サイズ(左・下・前)
)
{
	// 変数を宣言
	bool bHit = false;	// 衝突判定結果

	// 前後の当たり判定
	if (rCenterPos.x + centerSizeUp.x > targetPos.x - targetSizeDown.x
	&&  rCenterPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x)
	{ // 左右の範囲内の場合

		if (rCenterPos.z    + centerSizeUp.z >  targetPos.z - targetSizeDown.z
		&&  rCenterPosOld.z + centerSizeUp.z <= targetPos.z - targetSizeDown.z)
		{ // 前からの当たり判定

			// 位置を補正
			rCenterPos.z = targetPos.z - targetSizeDown.z - centerSizeUp.z;

			// 衝突状態にする
			bHit = true;
		}
		else if (rCenterPos.z    - centerSizeDown.z <  targetPos.z + targetSizeUp.z
		     &&  rCenterPosOld.z - centerSizeDown.z >= targetPos.z + targetSizeUp.z)
		{ // 後からの当たり判定

			// 位置を補正
			rCenterPos.z = targetPos.z + targetSizeUp.z + centerSizeDown.z;

			// 衝突状態にする
			bHit = true;
		}
	}

	// 左右の当たり判定
	if (rCenterPos.z + centerSizeUp.z > targetPos.z - targetSizeDown.z
	&&  rCenterPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // 前後の範囲内の場合

		if (rCenterPos.x    + centerSizeUp.x >  targetPos.x - targetSizeDown.x
		&&  rCenterPosOld.x + centerSizeUp.x <= targetPos.x - targetSizeDown.x)
		{ // 左からの当たり判定

			// 位置を補正
			rCenterPos.x = targetPos.x - targetSizeDown.x - centerSizeUp.x;

			// 衝突状態にする
			bHit = true;
		}
		else if (rCenterPos.x    - centerSizeDown.x <  targetPos.x + targetSizeUp.x
		     &&  rCenterPosOld.x - centerSizeDown.x >= targetPos.x + targetSizeUp.x)
		{ // 右からの当たり判定
			
			// 位置を補正
			rCenterPos.x = targetPos.x + targetSizeUp.x + centerSizeDown.x;

			// 衝突状態にする
			bHit = true;
		}
	}

	// 衝突判定を返す
	return bHit;
}

//============================================================
//	円柱の衝突判定
//============================================================
bool collision::CirclePillar
(
	D3DXVECTOR3& rCenterPos,	// 判定位置
	D3DXVECTOR3 targetPos,		// 判定目標位置
	float fCenterRadius,		// 判定半径
	float fTargetRadius			// 判定目標半径
)
{
	// 変数を宣言
	float fLength = 0.0f;	// 判定位置と判定目標位置の間の距離

	// 判定位置と判定目標位置の距離を求める
	fLength = (rCenterPos.x - targetPos.x) * (rCenterPos.x - targetPos.x)
			+ (rCenterPos.z - targetPos.z) * (rCenterPos.z - targetPos.z);

	if (fLength < (fCenterRadius + fTargetRadius) * (fCenterRadius + fTargetRadius))
	{ // 判定内の場合

		// 変数を宣言
		float fCenterRot = atan2f(rCenterPos.x - targetPos.x, rCenterPos.z - targetPos.z);	// 判定目標から見た判定向き

		// 位置を補正
		rCenterPos.x = targetPos.x + sinf(fCenterRot) * (fCenterRadius + fTargetRadius);
		rCenterPos.z = targetPos.z + cosf(fCenterRot) * (fCenterRadius + fTargetRadius);

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	三軸の矩形の衝突判定
//============================================================
bool collision::ResponseBox3D
(
	D3DXVECTOR3& rCenterPos,	// 判定位置
	D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
	D3DXVECTOR3 targetPos,		// 判定目標位置
	D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
	D3DXVECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
	D3DXVECTOR3 *pMove,	// 移動量
	bool *pUp,			// 上からの判定
	bool *pSide,		// 横からの判定
	bool *pDown			// 下からの判定
)
{
	// 変数を宣言
	bool bHit = false;	// 衝突判定結果

	// 左右の当たり判定
	if (rCenterPos.y + centerSizeUp.y   > targetPos.y - targetSizeDown.y
	&&  rCenterPos.y - centerSizeDown.y < targetPos.y + targetSizeUp.y
	&&  rCenterPos.z + centerSizeUp.z   > targetPos.z - targetSizeDown.z
	&&  rCenterPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // 上下と前後の範囲内の場合

		if (rCenterPos.x    + centerSizeUp.x >  targetPos.x - targetSizeDown.x
		&&  rCenterPosOld.x + centerSizeUp.x <= targetPos.x - targetSizeDown.x)
		{ // 左からの当たり判定

			// 位置を補正
			rCenterPos.x = targetPos.x - targetSizeDown.x - centerSizeUp.x;

			// 衝突状態にする
			bHit = true;

			if (pMove != NULL)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->x = 0.0f;
			}

			if (pSide != NULL)
			{ // ポインタが使用されている場合

				// 横に当たっている状態を設定
				*pSide = true;
			}
		}
		else if (rCenterPos.x    - centerSizeDown.x <  targetPos.x + targetSizeUp.x
		     &&  rCenterPosOld.x - centerSizeDown.x >= targetPos.x + targetSizeUp.x)
		{ // 右からの当たり判定
			
			// 位置を補正
			rCenterPos.x = targetPos.x + targetSizeUp.x + centerSizeDown.x;

			// 衝突状態にする
			bHit = true;

			if (pMove != NULL)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->x = 0.0f;
			}

			if (pSide != NULL)
			{ // ポインタが使用されている場合

				// 横に当たっている状態を設定
				*pSide = true;
			}
		}
	}

	// 上下の当たり判定
	if (rCenterPos.x + centerSizeUp.x   > targetPos.x - targetSizeDown.x
	&&  rCenterPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x
	&&  rCenterPos.z + centerSizeUp.z   > targetPos.z - targetSizeDown.z
	&&  rCenterPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // 左右と前後の範囲内の場合

		if (rCenterPos.y    + centerSizeUp.y >  targetPos.y - targetSizeDown.y
		&&  rCenterPosOld.y + centerSizeUp.y <= targetPos.y - targetSizeDown.y)
		{ // 下からの当たり判定

			// 位置を補正
			rCenterPos.y = targetPos.y - targetSizeDown.y - centerSizeUp.y;

			// 衝突状態にする
			bHit = true;

			if (pMove != NULL)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->y = 0.0f;
			}

			if (pDown != NULL)
			{ // ポインタが使用されている場合

				// 下に当たっている状態を設定
				*pDown = true;
			}
		}
		else if (rCenterPos.y    - centerSizeDown.y <  targetPos.y + targetSizeUp.y
		     &&  rCenterPosOld.y - centerSizeDown.y >= targetPos.y + targetSizeUp.y)
		{ // 上からの当たり判定
			
			// 位置を補正
			rCenterPos.y = targetPos.y + targetSizeUp.y + centerSizeDown.y;

			// 衝突状態にする
			bHit = true;

			if (pMove != NULL)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->y = 0.0f;
			}

			if (pUp != NULL)
			{ // ポインタが使用されている場合

				// 上に当たっている状態を設定
				*pUp = true;
			}
		}
	}

	// 前後の当たり判定
	if (rCenterPos.x + centerSizeUp.x   > targetPos.x - targetSizeDown.x
	&&  rCenterPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x
	&&  rCenterPos.y + centerSizeUp.y   > targetPos.y - targetSizeDown.y
	&&  rCenterPos.y - centerSizeDown.y < targetPos.y + targetSizeUp.y)
	{ // 左右と上下の範囲内の場合

		if (rCenterPos.z    + centerSizeUp.z >  targetPos.z - targetSizeDown.z
		&&  rCenterPosOld.z + centerSizeUp.z <= targetPos.z - targetSizeDown.z)
		{ // 前からの当たり判定

			// 位置を補正
			rCenterPos.z = targetPos.z - targetSizeDown.z - centerSizeUp.z;

			// 衝突状態にする
			bHit = true;

			if (pMove != NULL)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->z = 0.0f;
			}

			if (pSide != NULL)
			{ // ポインタが使用されている場合

				// 横に当たっている状態を設定
				*pSide = true;
			}
		}
		else if (rCenterPos.z    - centerSizeDown.z <  targetPos.z + targetSizeUp.z
		     &&  rCenterPosOld.z - centerSizeDown.z >= targetPos.z + targetSizeUp.z)
		{ // 後からの当たり判定

			// 位置を補正
			rCenterPos.z = targetPos.z + targetSizeUp.z + centerSizeDown.z;

			// 衝突状態にする
			bHit = true;

			if (pMove != NULL)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->z = 0.0f;
			}

			if (pSide != NULL)
			{ // ポインタが使用されている場合

				// 横に当たっている状態を設定
				*pSide = true;
			}
		}
	}

	// 衝突判定を返す
	return bHit;
}

//============================================================
//	X軸の衝突判定
//============================================================
bool collision::ResponseSingleX
(
	D3DXVECTOR3& rCenterPos,	// 判定位置
	D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
	D3DXVECTOR3 targetPos,		// 判定目標位置
	D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
	D3DXVECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
	D3DXVECTOR3 *pMove,	// 移動量
	bool bYColl,		// Y判定
	bool bZColl,		// Z判定
	bool *pLeft,		// 左からの判定
	bool *pRight		// 右からの判定
)
{
	// 変数を宣言
	bool bColl = true;	// 判定可能状況
	bool bHit = false;	// 衝突判定結果

	if (bYColl)
	{ // Y判定を行う場合

		if (rCenterPos.y + centerSizeUp.y   <= targetPos.y - targetSizeDown.y
		||  rCenterPos.y - centerSizeDown.y >= targetPos.y + targetSizeUp.y)
		{ // 上下の範囲外の場合

			// 判定不可能状態にする
			bColl = false;
		}
	}

	if (bZColl)
	{ // Z判定を行う場合

		if (rCenterPos.z + centerSizeUp.z   <= targetPos.z - targetSizeDown.z
		||  rCenterPos.z - centerSizeDown.z >= targetPos.z + targetSizeUp.z)
		{ // 前後の範囲外の場合

			// 判定不可能状態にする
			bColl = false;
		}
	}

	// 左右の当たり判定
	if (bColl)
	{ // 判定可能な場合

		if (rCenterPos.x    + centerSizeUp.x >  targetPos.x - targetSizeDown.x
		&&  rCenterPosOld.x + centerSizeUp.x <= targetPos.x - targetSizeDown.x)
		{ // 左からの当たり判定

			// 位置を補正
			rCenterPos.x = targetPos.x - targetSizeDown.x - centerSizeUp.x;

			// 衝突状態にする
			bHit = true;

			if (pMove != NULL)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->x = 0.0f;
			}

			if (pLeft != NULL)
			{ // ポインタが使用されている場合

				// 左に当たっている状態を設定
				*pLeft = true;
			}
		}
		else if (rCenterPos.x    - centerSizeDown.x <  targetPos.x + targetSizeUp.x
		     &&  rCenterPosOld.x - centerSizeDown.x >= targetPos.x + targetSizeUp.x)
		{ // 右からの当たり判定
			
			// 位置を補正
			rCenterPos.x = targetPos.x + targetSizeUp.x + centerSizeDown.x;

			// 衝突状態にする
			bHit = true;

			if (pMove != NULL)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->x = 0.0f;
			}

			if (pRight != NULL)
			{ // ポインタが使用されている場合

				// 右に当たっている状態を設定
				*pRight = true;
			}
		}
	}

	// 衝突判定を返す
	return bHit;
}

//============================================================
//	Y軸の衝突判定
//============================================================
bool collision::ResponseSingleY
(
	D3DXVECTOR3& rCenterPos,	// 判定位置
	D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
	D3DXVECTOR3 targetPos,		// 判定目標位置
	D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
	D3DXVECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
	D3DXVECTOR3 *pMove,	// 移動量
	bool bXColl,		// X判定
	bool bZColl,		// Z判定
	bool *pDown,		// 下からの判定
	bool *pUp			// 上からの判定
)
{
	// 変数を宣言
	bool bColl = true;	// 判定可能状況
	bool bHit = false;	// 衝突判定結果

	if (bXColl)
	{ // X判定を行う場合

		if (rCenterPos.x + centerSizeUp.x   <= targetPos.x - targetSizeDown.x
		||  rCenterPos.x - centerSizeDown.x >= targetPos.x + targetSizeUp.x)
		{ // 左右の範囲外の場合

			// 判定不可能状態にする
			bColl = false;
		}
	}

	if (bZColl)
	{ // Z判定を行う場合

		if (rCenterPos.z + centerSizeUp.z   <= targetPos.z - targetSizeDown.z
		||  rCenterPos.z - centerSizeDown.z >= targetPos.z + targetSizeUp.z)
		{ // 前後の範囲外の場合

			// 判定不可能状態にする
			bColl = false;
		}
	}

	// 上下の当たり判定
	if (bColl)
	{ // 判定可能な場合

		if (rCenterPos.y    + centerSizeUp.y >  targetPos.y - targetSizeDown.y
		&&  rCenterPosOld.y + centerSizeUp.y <= targetPos.y - targetSizeDown.y)
		{ // 下からの当たり判定

			// 位置を補正
			rCenterPos.y = targetPos.y - targetSizeDown.y - centerSizeUp.y;

			// 衝突状態にする
			bHit = true;

			if (pMove != NULL)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->y = 0.0f;
			}

			if (pDown != NULL)
			{ // ポインタが使用されている場合

				// 下に当たっている状態を設定
				*pDown = true;
			}
		}
		else if (rCenterPos.y    - centerSizeDown.y <  targetPos.y + targetSizeUp.y
		     &&  rCenterPosOld.y - centerSizeDown.y >= targetPos.y + targetSizeUp.y)
		{ // 上からの当たり判定
			
			// 位置を補正
			rCenterPos.y = targetPos.y + targetSizeUp.y + centerSizeDown.y;

			// 衝突状態にする
			bHit = true;

			if (pMove != NULL)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->y = 0.0f;
			}

			if (pUp != NULL)
			{ // ポインタが使用されている場合

				// 上に当たっている状態を設定
				*pUp = true;
			}
		}
	}

	// 衝突判定を返す
	return bHit;
}

//============================================================
//	Z軸の衝突判定
//============================================================
bool collision::ResponseSingleZ
(
	D3DXVECTOR3& rCenterPos,	// 判定位置
	D3DXVECTOR3& rCenterPosOld,	// 判定過去位置
	D3DXVECTOR3 targetPos,		// 判定目標位置
	D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	D3DXVECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
	D3DXVECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
	D3DXVECTOR3 *pMove,	// 移動量
	bool bXColl,		// X判定
	bool bYColl,		// Y判定
	bool *pBefore,		// 前からの判定
	bool *pAfter		// 後からの判定
)
{
	// 変数を宣言
	bool bColl = true;	// 判定可能状況
	bool bHit = false;	// 衝突判定結果

	if (bXColl)
	{ // X判定を行う場合

		if (rCenterPos.x + centerSizeUp.x   <= targetPos.x - targetSizeDown.x
		||  rCenterPos.x - centerSizeDown.x >= targetPos.x + targetSizeUp.x)
		{ // 左右の範囲外の場合

			// 判定不可能状態にする
			bColl = false;
		}
	}

	if (bYColl)
	{ // Y判定を行う場合

		if (rCenterPos.y + centerSizeUp.y   <= targetPos.y - targetSizeDown.y
		||  rCenterPos.y - centerSizeDown.y >= targetPos.y + targetSizeUp.y)
		{ // 上下の範囲外の場合

			// 判定不可能状態にする
			bColl = false;
		}
	}

	// 前後の当たり判定
	if (bColl)
	{ // 判定可能な場合

		if (rCenterPos.z    + centerSizeUp.z >  targetPos.z - targetSizeDown.z
		&&  rCenterPosOld.z + centerSizeUp.z <= targetPos.z - targetSizeDown.z)
		{ // 前からの当たり判定

			// 位置を補正
			rCenterPos.z = targetPos.z - targetSizeDown.z - centerSizeUp.z;

			// 衝突状態にする
			bHit = true;

			if (pMove != NULL)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->z = 0.0f;
			}

			if (pBefore != NULL)
			{ // ポインタが使用されている場合

				// 前に当たっている状態を設定
				*pBefore = true;
			}
		}
		else if (rCenterPos.z    - centerSizeDown.z <  targetPos.z + targetSizeUp.z
		     &&  rCenterPosOld.z - centerSizeDown.z >= targetPos.z + targetSizeUp.z)
		{ // 後からの当たり判定

			// 位置を補正
			rCenterPos.z = targetPos.z + targetSizeUp.z + centerSizeDown.z;

			// 衝突状態にする
			bHit = true;

			if (pMove != NULL)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->z = 0.0f;
			}

			if (pAfter != NULL)
			{ // ポインタが使用されている場合

				// 後に当たっている状態を設定
				*pAfter = true;
			}
		}
	}

	// 衝突判定を返す
	return bHit;
}

//============================================================
//	角柱の内側制限
//============================================================
bool collision::InBoxPillar
(
	D3DXVECTOR3& rCenterPos,	// 判定位置
	D3DXVECTOR3 originPos,		// 判定原点位置
	D3DXVECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	D3DXVECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	D3DXVECTOR3 originSizeUp,	// 判定原点サイズ(右・上・後)
	D3DXVECTOR3 originSizeDown	// 判定原点サイズ(左・下・前)
)
{
	// 変数を宣言
	bool bHit = false;	// 判定結果

	// 左右の補正
	if (useful::LimitNum(rCenterPos.x, originPos.x - originSizeDown.x + centerSizeUp.x, originPos.x + originSizeUp.x - centerSizeDown.x))
	{ // 補正が行われた場合

		// 判定した状態にする
		bHit = true;
	}

	// 前後の補正
	if (useful::LimitNum(rCenterPos.z, originPos.z - originSizeDown.z + centerSizeUp.z, originPos.z + originSizeUp.z - centerSizeDown.z))
	{ // 補正が行われた場合

		// 判定した状態にする
		bHit = true;
	}

	// 判定結果を返す
	return bHit;
}

//============================================================
//	円柱の内側制限
//============================================================
bool collision::InCirclePillar
(
	D3DXVECTOR3& rCenterPos,	// 判定位置
	D3DXVECTOR3 originPos,		// 判定原点位置
	float fCenterRadius,		// 判定半径
	float fOriginRadius			// 判定原点半径
)
{
	// 変数を宣言
	float fLength = 0.0f;	// 判定位置と判定目標位置の間の距離

	// 判定位置と判定目標位置の距離を求める
	fLength = (rCenterPos.x - originPos.x) * (rCenterPos.x - originPos.x)
			+ (rCenterPos.z - originPos.z) * (rCenterPos.z - originPos.z);

	if (fLength > (fOriginRadius - fCenterRadius) * (fOriginRadius - fCenterRadius))
	{ // 判定外の場合

		// 変数を宣言
		float fCenterRot = atan2f(rCenterPos.x - originPos.x, rCenterPos.z - originPos.z);	// 判定目標から見た判定向き

		// 位置を補正
		rCenterPos.x = originPos.x + sinf(fCenterRot) * (fOriginRadius - fCenterRadius);
		rCenterPos.z = originPos.z + cosf(fCenterRot) * (fOriginRadius - fCenterRadius);

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	外積の左右判定
//============================================================
//	境界線から見て左右どちらにいるかの判定に使用
//============================================================
float collision::LineOuterProduct
(
	D3DXVECTOR3 posLeft,	// 境界線左座標
	D3DXVECTOR3 posRight,	// 境界線右座標
	D3DXVECTOR3 pos			// 判定位置
)
{
	// 変数を宣言
	D3DXVECTOR3 vecLine;	// 境界線ベクトル
	D3DXVECTOR3 vecToPos;	// 左端と位置のベクトル

	// 境界線ベクトルを求める
	vecLine = posRight - posLeft;

	// 左端と位置のベクトルを求める
	vecToPos = pos - posLeft;

	// 外積の計算結果を返す
	return (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
}

//============================================================
//	外積の上下判定
//============================================================
//	境界線から見て上下どちらにいるかの判定に使用
//============================================================
float collision::LineHeightOuterProduct
(
	D3DXVECTOR3 posLeft,	// 境界線左座標
	D3DXVECTOR3 posRight,	// 境界線右座標
	D3DXVECTOR3 pos			// 判定位置
)
{
	// 変数を宣言
	D3DXVECTOR3 vecLine;	// 境界線ベクトル
	D3DXVECTOR3 vecToPos;	// 左端と位置のベクトル

	// 境界線ベクトルを求める
	vecLine = posRight - posLeft;

	// 左端と位置のベクトルを求める
	vecToPos = pos - posLeft;

	// 外積の計算結果を返す
	return (vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y);
}

//============================================================
//	三角柱の当たり判定
//============================================================
bool collision::TriangleOuterPillar
(
	D3DXVECTOR3 pos0,	// 三角の各頂点
	D3DXVECTOR3 pos1,	// 三角の各頂点
	D3DXVECTOR3 pos2,	// 三角の各頂点
	D3DXVECTOR3 pos		// 判定位置
)
{
	if (collision::LineOuterProduct(pos0, pos1, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos1, pos2, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos2, pos0, pos) <= 0.0f)
	{ // 判定内の場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	四角柱の当たり判定
//============================================================
bool collision::BoxOuterPillar
(
	D3DXVECTOR3 pos0,	// 四角の各頂点
	D3DXVECTOR3 pos1,	// 四角の各頂点
	D3DXVECTOR3 pos2,	// 四角の各頂点
	D3DXVECTOR3 pos3,	// 四角の各頂点
	D3DXVECTOR3 pos		// 判定位置
)
{
	if (collision::LineOuterProduct(pos0, pos1, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos1, pos2, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos2, pos3, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos3, pos0, pos) <= 0.0f)
	{ // 判定内の場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
// 箱の当たり判定
//============================================================
bool collision::Square
( // 引数
	D3DXVECTOR3 posSquare,			//箱の位置
	D3DXVECTOR3 *posTarget,			//衝突した物の位置
	D3DXVECTOR3 posTargetOld,		//衝突した物の過去位置
	D3DXVECTOR3 rotSquare,			//箱の角度
	D3DXVECTOR3 offsetMax,			//箱の最大位置オフセット
	D3DXVECTOR3 offsetMin			//箱の最小位置オフセット
)
{
	D3DXVECTOR3 posCorner[8] = {};									//箱の角
	D3DXVECTOR3 posPlaneCenter[6] = {};								//箱の各面の中心
	D3DXVECTOR3 vecIntersect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//箱との衝突点
	D3DXVECTOR3 vecNorPlaneCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//平面の法線ベクトル
	D3DXPLANE plane = D3DXPLANE(0.0f, 0.0f, 0.0f, 0.0f);			//各面の平面
	float lengthX, lengthY, lengthZ;								//各方向軸の大きさ

	//各方向軸で大きさを図る
	lengthX = fabsf(offsetMax.x - offsetMin.x) * 0.5f;
	lengthY = fabsf(offsetMax.y - offsetMin.y) * 0.5f;
	lengthZ = fabsf(offsetMax.z - offsetMin.z) * 0.5f;

	//箱の各面の中心を求める
	posPlaneCenter[0] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(lengthX, 0.0f, 0.0f));
	posPlaneCenter[1] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(-lengthX, 0.0f, 0.0f));
	posPlaneCenter[2] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, lengthY, 0.0f));
	posPlaneCenter[3] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, -lengthY, 0.0f));
	posPlaneCenter[4] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, 0.0f, lengthZ));
	posPlaneCenter[5] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, 0.0f, -lengthZ));

	//posが箱の中に入っている
	if (SquareTrigger(posSquare, *posTarget, rotSquare, offsetMax, offsetMin) == true)
	{
		//6面全てで確かめる
		for (int nCnt = 0; nCnt < 6; nCnt++)
		{
			//各面の法線ベクトルを計算する
			vecNorPlaneCenter = posSquare - posPlaneCenter[nCnt];
			D3DXVec3Normalize(&vecNorPlaneCenter, &vecNorPlaneCenter);

			//法線ベクトルから平面の式を計算する
			D3DXPlaneFromPointNormal(&plane, &posPlaneCenter[nCnt], &vecNorPlaneCenter);

			//平面の式と点から
			if (D3DXPlaneDotCoord(&plane, posTarget) > 0.0f && D3DXPlaneDotCoord(&plane, &posTargetOld) <= 0.0f)
			{
				//平面と位置過去位置から衝突点を算出
				D3DXPlaneIntersectLine(&vecIntersect, &plane, posTarget, &posTargetOld);

				//衝突点を修正
				if (posPlaneCenter[nCnt].x == 0.0f)
				{
					vecIntersect.x = 0.0f;
				}

				if (posPlaneCenter[nCnt].y == 0.0f)
				{
					vecIntersect.y = 0.0f;
				}

				if (posPlaneCenter[nCnt].z == 0.0f)
				{
					vecIntersect.z = 0.0f;
				}

				//位置修正
				*posTarget = vecIntersect - (vecNorPlaneCenter * 7.0f);

				return true;
			}
		}
	}

	return false;
}

//============================================================
//箱の当たり判定(内外判定のみ)
//============================================================
bool collision::SquareTrigger
(// 引数
	D3DXVECTOR3 posSquare,		//箱の位置
	D3DXVECTOR3 posTarget,		//衝突した物の位置
	D3DXVECTOR3 rotSquare,		//箱の角度
	D3DXVECTOR3 offsetMax,		//箱の最大位置オフセット
	D3DXVECTOR3 offsetMin		//箱の最小位置オフセット
)						  
{
	bool bSquare;							//判定用bool
	bSquare = false;

	D3DXVECTOR3 posCorner[8];			//箱の角
	D3DXVECTOR3 posPlaneCenter[6];		//箱の各面の中心
	D3DXVECTOR3 vecNorPlaneCenter[6];	//平面の法線ベクトル
	D3DXPLANE plane[6];					//各面の平面
	int nCheckCollision = 0;			//面で当たったかどうか確認用変数

	//箱の各面の中心を求める
	posPlaneCenter[0] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(offsetMax.x, 0.0f, 0.0f));
	posPlaneCenter[1] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(offsetMin.x, 0.0f, 0.0f));
	posPlaneCenter[2] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, offsetMax.y, 0.0f));
	posPlaneCenter[3] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, offsetMin.y, 0.0f));
	posPlaneCenter[4] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, 0.0f, offsetMax.z));
	posPlaneCenter[5] = PosRelativeMtx(posSquare, rotSquare, D3DXVECTOR3(0.0f, 0.0f, offsetMin.z));

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		//各面の法線ベクトルを計算する
		vecNorPlaneCenter[nCnt] = posSquare - posPlaneCenter[nCnt];
		D3DXVec3Normalize(&vecNorPlaneCenter[nCnt], &vecNorPlaneCenter[nCnt]);

		//法線ベクトルから平面の式を計算する
		D3DXPlaneFromPointNormal(&plane[nCnt], &posPlaneCenter[nCnt], &vecNorPlaneCenter[nCnt]);

		//平面の式と点から
		if (D3DXPlaneDotCoord(&plane[nCnt], &posTarget) > 0.0f)
		{
			nCheckCollision++;
		}
	}

	//全ての面で内側だったら
	if (nCheckCollision == 6)
	{
		bSquare = true;
	}

	return bSquare;
}

//========================================
//3次元空間での行列による回転座標変換関数
//(任意の点からのオフセット位置を角度と距離で変換)
//========================================
D3DXVECTOR3 collision::PosRelativeMtx
(
	D3DXVECTOR3 posO,		//回転中心点
	D3DXVECTOR3 rot,		//角度
	D3DXVECTOR3 offset		//オフセット位置
)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	D3DXVECTOR3 posAnswer;																//返答用変数
	D3DXMATRIX mtxO, mtxAnswer;															//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;														//計算用マトリックス
	D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;									//計算用マトリックス

	//パーツのワールドマトリックス初期化
	D3DXMatrixIdentity(&mtxO);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel,
		posO.x, posO.y, posO.z);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxTransModel);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxO);

	mtxPalent = mtxO;

	//パーツのワールドマトリックス初期化
	D3DXMatrixIdentity(&mtxAnswer);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		3.14f, 3.14f, 3.14f);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel,
		offset.x, offset.y, offset.z);
	D3DXMatrixMultiply(&mtxAnswer, &mtxAnswer, &mtxTransModel);

	//算出したパーツのワールドマトリックスと親のマトリックスをかけ合わせる
	D3DXMatrixMultiply(&mtxAnswer,
		&mtxAnswer,
		&mtxPalent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxAnswer);

	//変数用変数に代入
	posAnswer.x = mtxAnswer._41;
	posAnswer.y = mtxAnswer._42;
	posAnswer.z = mtxAnswer._43;

	return posAnswer;
}