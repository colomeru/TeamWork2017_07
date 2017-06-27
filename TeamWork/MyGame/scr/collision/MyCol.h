#pragma once

#include"../math/Vector2.h"
#include"../math/Matrix.h"
#include <math.h>
#include"../collision/CollisionParameter.h"
#include"../actor/ActorParameter.h"
#include"../actor/Actor.h"
#include"../conv/DXConverter.h"
#include"../math/MathHelper.h"
#include<vector>
#include<map>
// 有向境界ボックス（２次元）
struct OBB
{
	Vector2	Pos;        // 位置
	Vector2	Dir[2];		// XY各座標軸の傾きを表す方向ベクトル
	float		Length[2];  // 各軸方向の長さの半分
};
struct Segment {
	Vector2 startPoint;
	Vector2 endPoint;
};
struct Circle {
	Vector2 pos;
	float rad;
};

class MyCol {
public:


	static bool CalcIntersectionPoint(const Vector2& pointA,
		const Vector2& pointB,
		const Vector2& pointC,
		const Vector2& pointD,
		Vector2& pointIntersection,
		double& dR,
		double& dS)
	{
		double dBunbo = (pointB.x - pointA.x)
			* (pointD.y - pointC.y)
			- (pointB.y - pointA.y)
			* (pointD.x - pointC.x);
		if (0 == dBunbo)
		{	// 平行の時はcとdの中心(これでいいの？)
			pointIntersection = (pointC + pointD) / 2;
			return false;
		}

		Vector2 vectorAC = pointC - pointA;

		dR = ((pointD.y - pointC.y) * vectorAC.x
			- (pointD.x - pointC.x) * vectorAC.y) / dBunbo;
		dS = ((pointB.y - pointA.y) * vectorAC.x
			- (pointB.x - pointA.x) * vectorAC.y) / dBunbo;

		pointIntersection = pointA + (float)dR * (pointB - pointA);

		return true;
	}

	static Vector2 CreateVector(const Vector2& p, const Vector2& q) {
		return q - p;// Vector2(q.x - p.x, q.y - p.y);
	}
	static float InnerProduct(const Vector2 &a, const Vector2 &b) {
		return (a.x*b.x + a.y*b.y);//a・b
	}
	static float OuterProduct(const Vector2 &a, const Vector2 &b) {
		return (a.x*b.y - a.y*b.x);//a×b
	}


	// OBB作成
	static void CreateOBB(OBB* pOutOBB, const Vector2& Pos, const Matrix& RotateMtx, const Vector2& Length)
	{
		pOutOBB->Pos = Pos;
		pOutOBB->Length[0] = Length.x;
		pOutOBB->Length[1] = Length.y;
		pOutOBB->Dir[0] = Vector2(RotateMtx.m[0][0], RotateMtx.m[0][1]);
		pOutOBB->Dir[1] = Vector2(RotateMtx.m[1][0], RotateMtx.m[1][1]);
	}
	static void CreateSegment(Segment* outSeg, const Vector2& pos, const Matrix& RotateMtx, const float& Length) {
		Vector3 rotSeg = Vector3(Length / 2, 0, 0);
		rotSeg = rotSeg*RotateMtx;

		outSeg->startPoint = Vector2(pos.x - rotSeg.x, (pos.y - rotSeg.y));
		outSeg->endPoint = Vector2(pos.x + rotSeg.x, (pos.y + rotSeg.y));
	}
	static void CreateSegment(Segment* outSeg, const Vector2& startPos, const Vector2& endPos) {
		outSeg->startPoint = startPos;
		outSeg->endPoint = endPos;
	}
	static void CreateCircle(Circle* outCrc, const Vector2& position, float radius) {
		outCrc->pos = position;
		outCrc->rad = radius;
	}
	static std::vector<Vector2> GetOBBPoints(const Actor& parameter) {
		auto is = Matrix::CreateRotationZ(parameter.GetAngle());
		auto pos = parameter.GetPosition();
		auto sizeVec = Vector3((parameter.GetParameter().size.x / 2), (parameter.GetParameter().size.y / 2));

		auto box1 = Vector3(-sizeVec.x, -sizeVec.y)*is;
		auto box2 = Vector3(+sizeVec.x, -sizeVec.y)*is;
		auto box3 = Vector3(-sizeVec.x, +sizeVec.y)*is;
		auto box4 = Vector3(+sizeVec.x, +sizeVec.y)*is;
		//auto seg = Vector3(+sizeVec.x, 0)*is;

		std::vector<Vector2> datas;
		//左上,右上,左下,右下
		datas.push_back(Vector2(pos.x + box1.x, pos.y + box1.y));
		datas.push_back(Vector2(pos.x + box2.x, pos.y + box2.y));
		datas.push_back(Vector2(pos.x + box3.x, pos.y + box3.y));
		datas.push_back(Vector2(pos.x + box4.x, pos.y + box4.y));
		return datas;
		//Model::GetInstance().Draw(MODEL_ID::PLAYER_MODEL, parameter_.mat);
	}
	// 分離軸に投影された軸成分から投影線分長を算出
	static float LenSegOnSeparateAxis(Vector2 *Sep, Vector2 *e1, Vector2 *e2)
	{
		// 3つの内積の絶対値の和で投影線分長を計算
		// 分離軸Sepは標準化されていること
		float r1 = fabs(Vector2::Dot(*Sep, *e1));
		float r2 = fabs(Vector2::Dot(*Sep, *e2));
		return r1 + r2;
	}

	// OBB vs OBB
	static bool TestOBBOBB(const OBB &obb1, const OBB &obb2)
	{
		// 各方向ベクトルの確保
		// （N***:標準化方向ベクトル）
		Vector2 NAe1 = obb1.Dir[0], Ae1 = NAe1 * obb1.Length[0];
		Vector2 NAe2 = obb1.Dir[1], Ae2 = NAe2 * obb1.Length[1];
		Vector2 NBe1 = obb2.Dir[0], Be1 = NBe1 * obb2.Length[0];
		Vector2 NBe2 = obb2.Dir[1], Be2 = NBe2 * obb2.Length[1];
		Vector2 Interval = obb1.Pos - obb2.Pos;

		// 分離軸 : Ae1
		float rA = Ae1.Length();
		float rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2);
		float L = fabs(Vector2::Dot(Interval, NAe1));
		if (L > rA + rB)
			return false; // 衝突していない

						  // 分離軸 : Ae2
		rA = Ae2.Length();
		rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2);
		L = fabs(Vector2::Dot(Interval, NAe2));
		if (L > rA + rB)
			return false;

		// 分離軸 : Be1
		rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2);
		rB = Be1.Length();
		L = fabs(Vector2::Dot(Interval, NBe1));
		if (L > rA + rB)
			return false;

		// 分離軸 : Be2
		rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2);
		rB = Be2.Length();
		L = fabs(Vector2::Dot(Interval, NBe2));
		if (L > rA + rB)
			return false;

		// 分離平面が存在しないので「衝突している」
		return true;
	}
	static bool Col_Segment_Segment(const Segment &seg1, const Segment &seg2, Vector2& intersectPoint = Vector2()) {

		Vector3 seg1start = Vector3(seg1.startPoint.x, seg1.startPoint.y, 0);
		Vector3 seg1end = Vector3(seg1.endPoint.x, seg1.endPoint.y, 0);
		Vector3 seg2start = Vector3(seg2.startPoint.x, seg2.startPoint.y, 0);
		Vector3 seg2end = Vector3(seg2.endPoint.x, seg2.endPoint.y, 0);

		VECTOR p1_start = DXConverter::GetInstance().ToVECTOR(seg1start);
		VECTOR p1_end = DXConverter::GetInstance().ToVECTOR(seg1end);
		VECTOR p2_start = DXConverter::GetInstance().ToVECTOR(seg2start);
		VECTOR p2_end = DXConverter::GetInstance().ToVECTOR(seg2end);

		double R = 0;
		double S = 0;
		if (Segment_Segment_MinLength_Square(p1_start, p1_end, p2_start, p2_end) <= 0.01f) {
			CalcIntersectionPoint(Vector2(seg1start.x, seg1start.y), Vector2(seg1end.x, seg1end.y),
				Vector2(seg2start.x, seg2start.y), Vector2(seg2end.x, seg2end.y), intersectPoint, R, S);

			return true;
		}
		else {
			return false;
		}
	}
	//円と円の当たり判定を行う
	static bool Col_Circle_Circle(const Circle &crc1, const Circle &crc2, Vector2& intersectPoint = Vector2()) {
		float dx, dy, r;
		dx = crc2.pos.x - crc1.pos.x;
		dy = crc2.pos.y - crc1.pos.y;
		r = crc2.rad + crc1.rad;
		return ((dx*dx) + (dy*dy) < (r*r));

	}
	static bool Col_Circle_Segment(const Circle &crc, const Segment &seg, Vector2& intersectPoint = Vector2()) {

		Vector3 segstart = Vector3(seg.startPoint.x, seg.startPoint.y, 0);
		Vector3 segend = Vector3(seg.endPoint.x, seg.endPoint.y, 0);

		VECTOR p_start = DXConverter::GetInstance().ToVECTOR(segstart);
		VECTOR p_end = DXConverter::GetInstance().ToVECTOR(segend);

		float dx, dy, r;

		Vector2 pot[2] = { seg.startPoint,seg.endPoint };
		for (int i = 0; i < 2; i++)
		{
			dx = pot[i].x - crc.pos.x;
			dy = pot[i].y - crc.pos.y;
			r = crc.rad;
			if ((dx*dx) + (dy*dy) < (r*r))
			{
				return true;
			}
		}

		Vector2 pq, pm;
		float inner, k, pqd2, pmd2, phd2, d2;

		pq = CreateVector(seg.startPoint, seg.endPoint);
		pm = CreateVector(seg.startPoint, crc.pos);

		//inner = Vector2::Dot(pq, pm);
		inner = InnerProduct(pq, pm);
		pqd2 = pq.LengthSquared();
		pmd2 = pm.LengthSquared();

		k = inner / pqd2;

		if (k < 0 || 1 < k) return false;

		phd2 = (inner*inner) / pqd2;
		d2 = pmd2 - phd2;

		if (d2 < crc.rad*crc.rad)return true;

		return false;
	}
	static bool Col_OBB_Segment(const OBB &obb, const Segment &seg, const std::vector<Vector2>& points, Vector2& intersectPoint = Vector2()) {

		int intSet[][2] = { { 0,1 },{ 0,2 },{ 1,3 },{ 2,3 } };

		Segment obbSeg;

		Vector2 sectP[4] = { Vector2(99999,99999),  Vector2(99999,99999) , Vector2(99999,99999) , Vector2(99999,99999) };
		bool sectL = false;
		for (int i = 0; i < 4; i++)
		{
			CreateSegment(&obbSeg, points[intSet[i][0]], points[intSet[i][1]]);

			if (Col_Segment_Segment(obbSeg, seg, sectP[i])) {
				sectL = true;
			}
		}
		int targetNum = 0;
		float minLength = 99999;
		for (int i = 0; i < 4; i++) {
			if (minLength > Vector2::Distance(obb.Pos, sectP[i])) {
				minLength = Vector2::Distance(obb.Pos, sectP[i]);
				targetNum = i;
			}
		}
		intersectPoint = sectP[targetNum];
		return sectL;

	}
	static bool Col_OBB_Circle(const OBB &obb, const Circle &crc, const std::vector<Vector2>& points, Vector2& intersectPoint = Vector2()) {

		int intSet[][2] = { { 0,1 },{ 0,2 },{ 1,3 },{ 2,3 } };
		//端点
		float dx, dy, r;
		//線分
		Vector2 pq, pm;
		float inner, k, pqd2, pmd2, phd2, d2;
		//内側
		Vector2 pp, inpm;
		float ininner, outer, theta[2];



		//端点と触れているかどうか
		for (int i = 0; i < 4; i++)
		{
			dx = points[i].x - crc.pos.x;
			dy = points[i].y - crc.pos.y;
			r = crc.rad;
			if ((dx*dx) + (dy*dy) < (r*r))
			{
				return true;
			}
		}

		//線分に触れているかどうか
		for (int i = 0; i < 4; i++)
		{
			pq = CreateVector(points[intSet[i][0]], points[intSet[i][1]]);
			pm = CreateVector(points[intSet[i][0]], crc.pos);

			//inner = Vector2::Dot(pq, pm);
			inner = InnerProduct(pq, pm);
			pqd2 = pq.LengthSquared();
			pmd2 = pm.LengthSquared();

			k = inner / pqd2;

			if (k < 0 || 1 < k)continue;

			phd2 = (inner*inner) / pqd2;
			d2 = pmd2 - phd2;

			if (d2 < crc.rad*crc.rad)return true;
		}
		//四角の内側かどうか
		{
			for (int i = 0; i < 2; i++)
			{
				pp = CreateVector(points[i * 3], points[1 + i]);
				inpm = CreateVector(points[i * 3], crc.pos);

				ininner = InnerProduct(pp, inpm);
				outer = OuterProduct(pp, inpm);

				theta[i] = (atan2(outer, ininner)*(180 / MathHelper::Pi));
			}
			if (0 <= theta[0] && theta[0] <= 90 &&
				0 <= theta[1] && theta[1] <= 90)
			{
				intersectPoint = obb.Pos;
				return true;
			}
		}
		return false;

	}

	/*---------------------------------------------------------------------------------------------------------------------------------------------*/


};