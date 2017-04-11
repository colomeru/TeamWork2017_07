#include "Collision.h"
#include "../dxlib/DXConverter.h"

// 球とOBBの衝突判定
CollisionParameter Collision::Sphere_vs_OBB(CollisionParameter & colpara, Sphere & sphere, Box & box)
{
	ClosestPtPointOBB(colpara, sphere.centerPos, box);
	Vector3 v = colpara.colPos - sphere.centerPos;
	
	if (Vector3::Dot(v, v) <= sphere.radius * sphere.radius)
		colpara.colFrag = true;
	
	return colpara;
}

// カプセルとカプセルの衝突判定
CollisionParameter Collision::Capsule_vs_Capsule(CollisionParameter & colpara, Capsule & c1, Capsule & c2)
{
	int result = HitCheck_Capsule_Capsule(
		DXConverter::GetInstance().ToVECTOR(c1.startPos),
		DXConverter::GetInstance().ToVECTOR(c1.endPos),
		c1.radius,
		DXConverter::GetInstance().ToVECTOR(c2.startPos),
		DXConverter::GetInstance().ToVECTOR(c2.endPos),
		c2.radius);

	if (result > 0)
		colpara.colFrag = true;
	else
		colpara.colFrag = false;

	return colpara;
}

// OBBと点の最接近点を求める
void Collision::ClosestPtPointOBB(CollisionParameter & colpara, Vector3 & point, Box & box)
{
	Vector3 d = point - box.centerPos;
	colpara.colPos = box.centerPos;

	for (int i = 0; i < 3; i++)
	{
		float dist = Vector3::Dot(d, box.rotate[i]);
	
		switch (i)
		{
		case 0:
		{
			if (dist > box.halfSize.x)
				dist = box.halfSize.x;
			if (dist < -box.halfSize.x)
				dist = -box.halfSize.x;
			colpara.colPos += dist * box.rotate[i];
			break;
		}
		case 1:
		{
			if (dist > box.halfSize.y)
				dist = box.halfSize.y;
			if (dist < -box.halfSize.y)
				dist = -box.halfSize.y;
			colpara.colPos += dist * box.rotate[i];
			break;
		}
		case 2:
		{
			if (dist > box.halfSize.z)
				dist = box.halfSize.z;
			if (dist < -box.halfSize.z)
				dist = -box.halfSize.z;
			colpara.colPos += dist * box.rotate[i];
			break;
		}}
	}
}

