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
	bool *pLeft,		// 左からの判定
	bool *pRight		// 右からの判定
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
	bool *pDown,		// 下からの判定
	bool *pUp			// 上からの判定
)
{
	// 変数を宣言
	bool bHit = false;	// 衝突判定結果

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
	bool *pBefore,		// 前からの判定
	bool *pAfter		// 後からの判定
)
{
	// 変数を宣言
	bool bHit = false;	// 衝突判定結果

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
