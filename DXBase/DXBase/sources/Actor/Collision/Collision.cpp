#include "Collision.h"
//#include "Collision.h"
//
//bool CollisionUtil::Intersects_Segment_Segment(Vector2 thisstart, Vector2 thisend, Vector2 otherstart, Vector2 otherend)
//{
//		float otherPoint1 =
//			(thisstart.x - thisend.x)
//			*(otherstart.y - thisstart.y)
//			+ (thisstart.y - thisend.y)
//			*(thisstart.x - otherstart.x);
//		float otherPoint2 =
//			(thisstart.x - thisend.x)
//			*(otherend.y - thisstart.y)
//			+ (thisstart.y - thisend.y)
//			*(thisstart.x - otherend.x);
//
//		if (otherPoint1*otherPoint2 < 0)
//		{
//			float otherPoint1 =
//				(otherstart.x - otherend.x)
//				*(thisstart.y - otherstart.y)
//				+ (otherstart.y - otherend.y)
//				*(otherstart.x - thisstart.x);
//			float otherPoint2 =
//				(otherstart.x - otherend.x)
//				*(thisend.y - otherstart.y)
//				+ (otherstart.y - otherend.y)
//				*(otherstart.x - thisend.x);
//
//			if (otherPoint1*otherPoint2 < 0)
//			{
//
//				return true;
//			}
//
//		}
//	
//	return false;
//
//}
//
//bool CollisionUtil::Intersects_Segment_Box(Vector2 thisstart, Vector2 thisend, Vector2 othertopLeft, Vector2 othertopRight, Vector2 otherbottomLeft, Vector2 otherbottomRight)
//{
//	if (
//		Intersects_Segment_Segment(thisstart, thisend, othertopLeft, othertopRight)
//		|| Intersects_Segment_Segment(thisstart, thisend, othertopLeft, otherbottomLeft)
//		|| Intersects_Segment_Segment(thisstart, thisend, othertopRight, otherbottomRight)
//		|| Intersects_Segment_Segment(thisstart, thisend, otherbottomLeft, otherbottomRight)
//		) {
//		return true;
//	}
//	return false;
//
//}
//
//bool CollisionUtil::Intersects_Segment_Circle(Vector2 thisstart, Vector2 thisend, Vector2 othercenter, float otherradius)
//{
//	float dx, dy, r;
//
//	Vector2 component_[2] = { thisstart,thisend };
//
//	for (int i = 0; i < 2; i++)
//	{
//		dx = component_[i].x - othercenter.x;
//		dy = component_[i].y - othercenter.y;
//		r = otherradius;
//		if ((dx*dx) + (dy*dy) < (r*r))
//		{
//			return true;
//		}
//	}
//
//
//	Vector2 pq, pm;
//	float inner, k, pqd2, pmd2, phd2, d2;
//
//	pq = thisend - thisstart;
//	pm = othercenter - thisstart;
//	//pq = CreateVector(component_.point[0], component_.point[1]);
//	//pm = CreateVector(component_.point[0], other.component_.point[0]);
//
//	//inner = Vector2::Dot(pq, pm);
//	inner = Vector2::Dot(pq, pm);
//	//inner = InnerProduct(pq, pm);
//	pqd2 = pq.LengthSquared();
//	pmd2 = pm.LengthSquared();
//
//	k = inner / pqd2;
//
//	if (k < 0 || 1 < k) return false;
//
//	phd2 = (inner*inner) / pqd2;
//	d2 = pmd2 - phd2;
//
//	if (d2 < otherradius*otherradius)return true;
//
//	return false;
//
//}
//
//bool CollisionUtil::Intersects_Segment_Capsule(Vector2 thisstart, Vector2 thisend, Vector2 otherstart, Vector2 otherend, float otherradius)
//{
//	if (Intersects_Segment_Circle(thisstart, thisend, otherstart, otherradius)
//		|| Intersects_Segment_Circle(thisstart, thisend, otherend, otherradius))
//	{
//		return true;
//	}
//	float sin90, cos90;
//	sin90 = 1;
//	cos90 = 0;
//
//	Vector2 A = otherend - otherstart;
//	//Vector2 A = CreateVector(other.component_.point[0], other.component_.point[1]);
//	Vector2 AN = A.Normalize();
//	Vector2 rotationA{ (AN.x*cos90) - (AN.y*sin90) , (AN.x*sin90) + (AN.y*cos90) };
//	Vector2 A0P = otherstart + rotationA * otherradius;
//	Vector2 A0M = otherstart - rotationA * otherradius;
//
//	Vector2 A1P = otherend + rotationA * otherradius;
//	Vector2 A1M = otherend - rotationA * otherradius;
//
//	Vector2 As[][2]{ { A0P,A1P },{ A0M,A1M } };
//
//	for (int i = 0; i < 2; i++) {
//		if (Intersects_Segment_Segment(thisstart, thisend, As[i][0], As[i][1])) return true;
//	}
//
//	return false;
//}
//
//bool CollisionUtil::Intersects_Box_Box(Vector2 thistopLeft, Vector2 thistopRight, Vector2 thisbottomLeft, Vector2 thisbottomRight, Vector2 othertopLeft, Vector2 othertopRight, Vector2 otherbottomLeft, Vector2 otherbottomRight)
//{
//	Vector2 thisbox[4][2] = { { thistopLeft,thistopRight },{ thistopLeft,thisbottomLeft },{ thistopRight,thisbottomRight },{ thisbottomLeft,thisbottomRight } };
//	Vector2 otherbox[4][2] = { { othertopLeft,othertopRight },{ othertopLeft,otherbottomLeft },{ othertopRight,otherbottomRight },{ otherbottomLeft,otherbottomRight } };
//	for (int i = 0; i < 4; i++) {
//		for (int x = 0; x < 4; x++) {
//			if(Intersects_Segment_Segment(thisbox[i][0], thisbox[i][1], otherbox[x][0], otherbox[x][1])) return true;
//		}
//	}
//	return false;
//}
//
//bool CollisionUtil::Intersects_Box_Circle(Vector2 thistopLeft, Vector2 thistopRight, Vector2 thisbottomLeft, Vector2 thisbottomRight, Vector2 othercenter, float otherradius)
//{
//	Vector2 thisbox[4][2] = { { thistopLeft,thistopRight },{ thistopLeft,thisbottomLeft },{ thistopRight,thisbottomRight },{ thisbottomLeft,thisbottomRight } };
//	for (int i = 0; i < 4; i++) {
//		if(Intersects_Segment_Circle(thisbox[i][0], thisbox[i][1], othercenter, otherradius)) return true;
//	}
//	return false;
//}
//
//bool CollisionUtil::Intersects_Box_Capsule(Vector2 thistopLeft, Vector2 thistopRight, Vector2 thisbottomLeft, Vector2 thisbottomRight, Vector2 otherstart, Vector2 otherend, float otherradius)
//{
//	Vector2 thisbox[4][2] = { { thistopLeft,thistopRight },{ thistopLeft,thisbottomLeft },{ thistopRight,thisbottomRight },{ thisbottomLeft,thisbottomRight } };
//	for (int i = 0; i < 4; i++) {
//		if (Intersects_Segment_Capsule(thisbox[i][0], thisbox[i][1], otherstart, otherend, otherradius)) return true;
//	}
//	return false;
//}
//
//bool CollisionUtil::Intersects_Circle_Circle(Vector2 thiscenter, float thisradius, Vector2 othercenter, float otherradius)
//{
//	float dx, dy, r;
//	dx = othercenter.x - thiscenter.x;
//	dy = othercenter.y - thiscenter.y;
//	r = otherradius + thisradius;
//	return ((dx*dx) + (dy*dy) < (r*r));
//}
//
//bool CollisionUtil::Intersects_Circle_Capsule(Vector2 thiscenter, float thisradius, Vector2 otherstart, Vector2 otherend, float otherradius)
//{
//	Intersects_Circle_Circle(thiscenter, thisradius, otherstart, otherradius);
//	Intersects_Circle_Circle(thiscenter, thisradius, otherend, otherradius);
//
//	float sin90, cos90;
//	sin90 = 1;
//	cos90 = 0;
//
//	Vector2 A = otherend - otherstart;
//	Vector2 AN = A.Normalize();
//	Vector2 rotationA{ (AN.x*cos90) - (AN.y*sin90) , (AN.x*sin90) + (AN.y*cos90) };
//	Vector2 A0P = otherstart + rotationA * otherradius;
//	Vector2 A0M = otherstart - rotationA * otherradius;
//
//	Vector2 A1P = otherend + rotationA * otherradius;
//	Vector2 A1M = otherend - rotationA * otherradius;
//
//	Vector2 As[][2]{ { A0P,A1P },{ A0M,A1M } };
//
//	for (int i = 0; i < 2; i++) {
//		if (Intersects_Segment_Circle(As[i][0], As[i][1],thiscenter,thisradius)) return true;
//	}
//
//	return false;
//}
//
//bool CollisionUtil::Intersects_Capsule_Capsule(Vector2 thisstart, Vector2 thisend, float thisradius, Vector2 otherstart, Vector2 otherend, float otherradius)
//{
//	//2‰~‚ÌŠe+-‚Éü‚ðˆø‚«A‚»‚Ìü‚Æ‚Ì“–‚½‚è”»’è
//	float sin90, cos90;
//	sin90 = 1;
//	cos90 = 0;
//
//	Vector2 A = thisend - thisstart;
//	Vector2 AN = A.Normalize();
//	Vector2 rotationA{ (AN.x*cos90) - (AN.y*sin90) , (AN.x*sin90) + (AN.y*cos90) };
//	Vector2 A0P = thisstart + rotationA * thisradius;
//	Vector2 A0M = thisstart - rotationA * thisradius;
//	Vector2 A1P = thisend + rotationA * thisradius;
//	Vector2 A1M = thisend - rotationA * thisradius;
//
//	Vector2 B = otherend - otherstart;
//	Vector2 BN = B.Normalize();
//	Vector2 rotationB{ (BN.x*cos90) - (BN.y*sin90) , (BN.x*sin90) + (BN.y*cos90) };
//	Vector2 B0P = otherstart + rotationB * otherradius;
//	Vector2 B0M = otherstart - rotationB * otherradius;
//	Vector2 B1P = otherend + rotationB * otherradius;
//	Vector2 B1M = otherend - rotationB * otherradius;
//
//	Vector2 As[][2]{ { A0P,A1P },{ A0M,A1M } };
//	Vector2 Bs[][2]{ { B0P,B1P },{ B0M,B1M } };
//
//	//ü•ª“¯Žm‚Ì”»’è
//	for (int i = 0; i < 2; i++)
//	{
//		for (int x = 0; x < 2; x++)
//		{
//			if (Intersects_Segment_Segment(As[i][0], As[i][1], Bs[x][0], Bs[x][1]))
//			{
//				return true;
//			}
//		}
//	}
//	//‰~‚Æü•ª‚Ì”»’è
//	{
//		for (int i = 0; i < 2; i++)
//		{
//
//			if (Intersects_Segment_Circle(As[i][0], As[i][1], otherstart, otherradius)
//				|| Intersects_Segment_Circle(As[i][0], As[i][1], otherend, otherradius)
//				|| Intersects_Segment_Circle(Bs[i][0], Bs[i][1], thisstart, thisradius)
//				|| Intersects_Segment_Circle(Bs[i][0], Bs[i][1], thisend, thisradius)) {
//				return true;
//			}
//		}
//	}
//	//‰~“¯Žm‚Ì”»’è
//	if (Intersects_Circle_Circle(thisstart, thisradius, otherstart, otherradius)
//		||Intersects_Circle_Circle(thisstart, thisradius, otherend, otherradius)
//		||Intersects_Circle_Circle(thisend, thisradius, otherstart, otherradius)
//		||Intersects_Circle_Circle(thisend, thisradius, otherend, otherradius))
//	{
//		return true;
//	}
//	return false;
//}
//
//bool CollisionUtil::IsCross(Vector2 thisprevPos, Vector2 thisPos, Vector2 otherStart, Vector2 otherEnd) {
//	Vector2 AB = thisPos - thisprevPos;
//	Vector2 AC, AD, CD, CA, CB;
//
//	//‚·‚è”²‚¯‚½‚©
//	AC = otherStart - thisprevPos;
//	AD = otherEnd - thisprevPos;
//	CD = otherEnd - otherStart;
//	CA = thisprevPos - otherStart;
//	CB = thisPos - otherStart;
//
//	if (Vector2::Cross(AB, AC)*Vector2::Cross(AB, AD) <= 0.0f&&
//		Vector2::Cross(CD, CA)*Vector2::Cross(CD, CB) < 0.0f)
//	{
//		return true;
//	}
//	return false;
//}

bool Collision::seg_seg(const Vector2 & v1p1, const Vector2 & v1p2, const Vector2 & v2p1, const Vector2 & v2p2){

	Vector2 AB = v1p2 - v1p1;
	Vector2 AC, AD, CD, CA, CB;

	AC = v1p2 - v2p1;
	AD = v1p2 - v2p2;
	CD = v2p1 - v2p2;
	CA = v2p1 - v1p2;
	CB = v2p1 - v1p1;

	if (Vector2::Cross(AB, AC) * Vector2::Cross(AB, AD) <= 0.0f &&
		Vector2::Cross(CD, CA) * Vector2::Cross(CD, CB) <= 0.0f){
		DrawFormatString(400, 400, GetColor(255, 255, 255), "deta");
		return true;
	}
	return false;
}
