#include "BoundingCapsule.h"
#include"BoundingBox.h"
#include"BoundingSegment.h"
#include"BoundingCircle.h"

BoundingCircle::BoundingCircle(Vector2 center, float circleRadius) 
{
	enabled = (true);
	component_=Component(center, circleRadius);
	bodyenub_ = false;
}

BoundingCircle::BoundingCircle(Vector2 center, float circleRadius, bool isEnabled)
{
	enabled = (isEnabled);
	component_ = Component(center, circleRadius);
	bodyenub_ = false;
}

BoundingCircle::BoundingCircle(float radius) :
	BoundingCircle({ 0.0f, 0.0f }, radius, false) {

	bodyenub_ = false;
}

IBodyPtr BoundingCircle::translate(const Vector2& position) const {
	return std::make_shared<BoundingCircle>(component_.point[0] + position, component_.radius, enabled);
}

BoundingCircle BoundingCircle::transform(Vector2 center, float capsuleRadius) const {
	return BoundingCircle(center, capsuleRadius, enabled);
}

//bool BoundingSphere::intersects(const BoundingSphere& other) const {
//	return (component_.center_ - other.component_.center_).Length() <= component_.radius_ + other.component_.radius_
//	/*gsCollisionSphereAndSphere(&center, radius, &other.center, other.radius) == GS_TRUE*/;
//}

void BoundingCircle::draw() const {
	DrawCircle(static_cast<int>(component_.point[0].x), static_cast<int>(component_.point[0].y), static_cast<int>(component_.radius), GetColor(255, 0, 0), FALSE);

	//DrawSphere3D(Vector3::Vector3ToVECTOR(component_.center_), component_.radius_, 32, GetColor( 255,0,0 ), GetColor( 255, 255, 255 ), TRUE ) ;
}

void BoundingCircle::draw(Matrix inv) const {

	Vector3 pos0 = Vector3(component_.point[0].x, component_.point[0].y) * inv;

	//DrawCircle(pos0.x, pos0.y, component_.radius, GetColor(255, 0, 0), FALSE);

	//DrawSphere3D(Vector3::Vector3ToVECTOR(component_.center_), component_.radius_, 32, GetColor( 255,0,0 ), GetColor( 255, 255, 255 ), TRUE ) ;
}


bool BoundingCircle::intersects(BoundingBox & other)
{
	if (!enabled || !other.enabled)return false;

	int intSet[][2] = { { 0,1 },{ 0,2 },{ 1,3 },{ 2,3 } };
	//í[ì_
	float dx, dy, r;
	//ê¸ï™
	Vector2 pq, pm;
	float inner, k, pqd2, pmd2 , phd2, d2;
	//ì‡ë§
	Vector2 pp,inpm;
	float ininner, outer, theta[2];

	Vector2 AB = CreateVector(previousPosition_, position_);
	Vector2 AC, AD, CD, CA, CB;
	
	//Ç∑ÇËî≤ÇØÇΩÇ©
	for (int i = 0; i < 4; i++)
	{
		AC = CreateVector(previousPosition_, other.component_.point[intSet[i][0]]);
		AD = CreateVector(previousPosition_, other.component_.point[intSet[i][1]]);
		CD = CreateVector(other.component_.point[intSet[i][0]], other.component_.point[intSet[i][1]]);
		CA = CreateVector(other.component_.point[intSet[i][0]], previousPosition_);
		CB = CreateVector(other.component_.point[intSet[i][0]], position_);

		if (OuterProduct(AB, AC)*OuterProduct(AB, AD) <= 0.0f&&
			OuterProduct(CD, CA)*OuterProduct(CD, CB) < 0.0f)
		{
			//DrawFormatString(400, 400, GetColor(255, 255, 255), "deta");
			return true;
		}
	}


	//í[ì_Ç∆êGÇÍÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©
	for (int i = 0; i < 4; i++)
	{
		dx = other.component_.point[i].x - component_.point[0].x;
		dy = other.component_.point[i].y - component_.point[0].y;
		r = component_.radius;
		if ((dx*dx) + (dy*dy) < (r*r))
		{
			return true;
		}
	}

	//ê¸ï™Ç…êGÇÍÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©
	for (int i = 0; i < 4; i++)
	{
		pq = CreateVector(other.component_.point[intSet[i][0]], other.component_.point[intSet[i][1]]);
		pm = CreateVector(other.component_.point[intSet[i][0]], component_.point[0]);
		
		//inner = Vector2::Dot(pq, pm);
		inner = InnerProduct(pq,pm);
		pqd2 = pq.LengthSquared();
		pmd2 = pm.LengthSquared();
		
		k = inner / pqd2;
		
		if (k < 0 || 1 < k)continue;
		
		phd2 = (inner*inner) / pqd2;
		d2 = pmd2 - phd2;

		if (d2 < component_.radius*component_.radius)return true;
	}
	//éläpÇÃì‡ë§Ç©Ç«Ç§Ç©
	{
		for (int i = 0; i < 2; i++)
		{
			pp = CreateVector(other.component_.point[i * 3], other.component_.point[1 + i]);
			inpm = CreateVector(other.component_.point[i * 3], component_.point[0]);

			ininner = InnerProduct(pp, inpm);
			outer = OuterProduct(pp, inpm);

			theta[i] = (atan2(outer, ininner)*(180 / MathHelper::Pi));
		}
		if (0 <= theta[0] && theta[0] <= 90 &&
			0 <= theta[1] && theta[1] <= 90)
		{
			return true;
		}
	}
	return false;
}
//p->qÉxÉNÉgÉãÇï‘Ç∑

bool BoundingCircle::intersects(BoundingCircle& other)
{
	if (!enabled || !other.enabled)return false;

	Vector2 AB = CreateVector(previousPosition_, position_);
	Vector2 AC, AD, CD, CA, CB;

	//Ç∑ÇËî≤ÇØÇΩÇ©
	for (int i = 0; i < 4; i++)
	{
		AC = CreateVector(previousPosition_, other.previousPosition_);
		AD = CreateVector(previousPosition_, other.position_);
		CD = CreateVector(other.previousPosition_, other.position_);
		CA = CreateVector(other.previousPosition_, previousPosition_);
		CB = CreateVector(other.previousPosition_, position_);

		if (Vector2::Cross(AB, AC)*Vector2::Cross(AB, AD) <= 0.0f&&
			Vector2::Cross(CD, CA)*Vector2::Cross(CD, CB) < 0.0f)
		{
			//DrawFormatString(400, 400, GetColor(255, 255, 255), "deta");
			//OutputDebugString("stds");
			//OutputDebugString("\n");
			return true;
		}
	}


	float dx, dy, r;
	dx = other.component_.point[0].x - component_.point[0].x;
	dy = other.component_.point[0].y - component_.point[0].y;
	r = other.component_.radius + component_.radius;
	return ((dx*dx) + (dy*dy) < (r*r));
}
bool BoundingCircle::intersects(BoundingCapsule & other) {
	if (!enabled || !other.enabled)return false;

	Vector2 AB = CreateVector(previousPosition_, position_);
	Vector2 AC, AD, CD, CA, CB;

	//Ç∑ÇËî≤ÇØÇΩÇ©

	AC = CreateVector(previousPosition_, other.component_.point[0]);
	AD = CreateVector(previousPosition_, other.component_.point[1]);
	CD = CreateVector(other.component_.point[0], other.component_.point[1]);
	CA = CreateVector(other.component_.point[0], previousPosition_);
	CB = CreateVector(other.component_.point[0], position_);

	if (Vector2::Cross(AB, AC)*Vector2::Cross(AB, AD) <= 0.0f&&
		Vector2::Cross(CD, CA)*Vector2::Cross(CD, CB) < 0.0f)
	{
		//DrawFormatString(400, 400, GetColor(255, 255, 255), "deta");
		OutputDebugString("stds");
		OutputDebugString("\n");
		return true;
	}


	//2â~ÇÃäe+-Ç…ê¸Çà¯Ç´ÅAÇªÇÃê¸Ç∆ÇÃìñÇΩÇËîªíË
	float sin90, cos90;
	sin90 = 1;
	cos90 = 0;
	
	Vector2 A = CreateVector(other.component_.point[0], other.component_.point[1]);
	Vector2 AN = A.Normalize();
	Vector2 rotationA{ (AN.x*cos90) - (AN.y*sin90) , (AN.x*sin90) + (AN.y*cos90)};
	Vector2 A0P = other.component_.point[0] + rotationA * other.component_.radius;
	Vector2 A0M = other.component_.point[0] - rotationA * other.component_.radius;

	Vector2 A1P = other.component_.point[1] + rotationA * other.component_.radius;
	Vector2 A1M = other.component_.point[1] - rotationA * other.component_.radius;

	Vector2 pq, pm;
	float inner, k, pqd2, pmd2, phd2, d2;
	
	Vector2 As[][2]{ {A0P,A1P }, { A0M,A1M }};

	for (int i = 0; i < 2; i++)
	{
		pq = CreateVector(As[i][0], As[i][1]);
		pm = CreateVector(As[i][0], component_.point[0]);

		//inner = Vector2::Dot(pq, pm);
		inner = InnerProduct(pq, pm);
		pqd2 = pq.LengthSquared();
		pmd2 = pm.LengthSquared();

		k = inner / pqd2;

		if (k < 0 || 1 < k)continue;

		phd2 = (inner*inner) / pqd2;
		d2 = pmd2 - phd2;

		if (d2 < component_.radius*component_.radius)return true;
	}

	//â~ìØémÇÃîªíË
	float dx, dy, r;
	for (int i = 0; i < 2; i++)
	{
		dx = other.component_.point[i].x - component_.point[0].x;
		dy = other.component_.point[i].y - component_.point[0].y;
		r = other.component_.radius + component_.radius;
		if ((dx*dx) + (dy*dy) < (r*r)) {
			return true;
		};
	}

	return false;
	//DrawCircle(A0P.x, A0P.y,3, GetColor(255, 255, 255));
	//DrawCircle(A0M.x, A0M.y,3, GetColor(255, 255, 255));
	//DrawCircle(A1P.x, A1P.y,3, GetColor(255, 255, 255));
	//DrawCircle(A1M.x, A1M.y,3, GetColor(255, 255, 255));
}

bool BoundingCircle::intersects(BoundingSegment & other) {
	if (!enabled || !other.enabled)return false;

	Vector2 AB = CreateVector(previousPosition_, position_);
	Vector2 AC, AD, CD, CA, CB;

	//Ç∑ÇËî≤ÇØÇΩÇ©
	AC = CreateVector(previousPosition_, other.component_.point[0]);
	AD = CreateVector(previousPosition_, other.component_.point[1]);
	CD = CreateVector(other.component_.point[0], other.component_.point[1]);
	CA = CreateVector(other.component_.point[0], previousPosition_);
	CB = CreateVector(other.component_.point[0], position_);

	if (Vector2::Cross(AB, AC)*Vector2::Cross(AB, AD) <= 0.0f&&
		Vector2::Cross(CD, CA)*Vector2::Cross(CD, CB) < 0.0f)
	{
		//DrawFormatString(400, 400, GetColor(255, 255, 255), "deta");
		OutputDebugString("stds");
		OutputDebugString("\n");
		return true;
	}


	float dx, dy, r;

	for (int i = 0; i < 2; i++)
	{
		dx = other.component_.point[i].x - component_.point[0].x;
		dy = other.component_.point[i].y - component_.point[0].y;
		r = component_.radius;
		if ((dx*dx) + (dy*dy) < (r*r))
		{
			return true;
		}
	}

	Vector2 pq, pm;
	float inner, k, pqd2, pmd2, phd2, d2;

		pq = CreateVector(other.component_.point[0], other.component_.point[1]);
		pm = CreateVector(other.component_.point[0], component_.point[0]);

		//inner = Vector2::Dot(pq, pm);
		inner = InnerProduct(pq, pm);
		pqd2 = pq.LengthSquared();
		pmd2 = pm.LengthSquared();

		k = inner / pqd2;

		if (k < 0 || 1 < k) return false;

		phd2 = (inner*inner) / pqd2;
		d2 = pmd2 - phd2;

		if (d2 < component_.radius*component_.radius)return true;

		return false;
}


bool BoundingCircle::isIntersectOtherRayToThisLineSegment(BoundingSegment & other)
{
	float otherPoint1 =
		(component_.point[0].x - component_.point[1].x)
		*(other.component_.point[0].y - component_.point[0].y)
		+ (component_.point[0].y - component_.point[1].y)
		*(component_.point[0].x - other.component_.point[0].x);
	float otherPoint2 =
		(component_.point[0].x - component_.point[1].x)
		*(other.component_.point[1].y - component_.point[0].y)
		+ (component_.point[0].y - component_.point[1].y)
		*(component_.point[0].x - other.component_.point[1].x);

	if (otherPoint1*otherPoint2 < 0)
	{

		return true;
	}
	return false;
}
bool BoundingCircle::isIntersectThisRayToOtherLineSegment(BoundingSegment & other)
{
	Vector2 AB = CreateVector(previousPosition_, position_);
	Vector2 AC, AD, CD, CA, CB;

	//Ç∑ÇËî≤ÇØÇΩÇ©
	AC = CreateVector(previousPosition_, other.component_.point[0]);
	AD = CreateVector(previousPosition_, other.component_.point[1]);
	CD = CreateVector(other.component_.point[0], other.component_.point[1]);
	CA = CreateVector(other.component_.point[0], previousPosition_);
	CB = CreateVector(other.component_.point[0], position_);

	if (OuterProduct(AB, AC)*OuterProduct(AB, AD) <= 0.0f&&
		OuterProduct(CD, CA)*OuterProduct(CD, CB) < 0.0f)
	{
		//DrawFormatString(400, 400, GetColor(255, 255, 255), "deta");
		return true;
	}


	float otherPoint1 =
		(other.component_.point[0].x - other.component_.point[1].x)
		*(component_.point[0].y - other.component_.point[0].y)
		+ (other.component_.point[0].y - other.component_.point[1].y)
		*(other.component_.point[0].x - component_.point[0].x);
	float otherPoint2 =
		(other.component_.point[0].x - other.component_.point[1].x)
		*(component_.point[1].y - other.component_.point[0].y)
		+ (other.component_.point[0].y - other.component_.point[1].y)
		*(other.component_.point[0].x - component_.point[1].x);

	if (otherPoint1*otherPoint2 < 0)
	{
		return true;
	}
	return false;
}
bool BoundingCircle::isIntersectOtherRayToThisLineCapsule(BoundingCapsule & other)
{
	float otherPoint1 =
		(component_.point[0].x - component_.point[1].x)
		*(other.component_.point[0].y - component_.point[0].y)
		+ (component_.point[0].y - component_.point[1].y)
		*(component_.point[0].x - other.component_.point[0].x);
	float otherPoint2 =
		(component_.point[0].x - component_.point[1].x)
		*(other.component_.point[1].y - component_.point[0].y)
		+ (component_.point[0].y - component_.point[1].y)
		*(component_.point[0].x - other.component_.point[1].x);

	if (otherPoint1*otherPoint2 < 0)
	{

		return true;
	}
	return false;
}
bool BoundingCircle::isIntersectThisRayToOtherLineCapsule(BoundingCapsule & other)
{
	float otherPoint1 =
		(other.component_.point[0].x - other.component_.point[1].x)
		*(component_.point[0].y - other.component_.point[0].y)
		+ (other.component_.point[0].y - other.component_.point[1].y)
		*(other.component_.point[0].x - component_.point[0].x);
	float otherPoint2 =
		(other.component_.point[0].x - other.component_.point[1].x)
		*(component_.point[1].y - other.component_.point[0].y)
		+ (other.component_.point[0].y - other.component_.point[1].y)
		*(other.component_.point[0].x - component_.point[1].x);

	if (otherPoint1*otherPoint2 < 0)
	{
		return true;
	}
	return false;
}
bool BoundingCircle::isIntersectOtherRayToThisLineBox(BoundingBox & other, int point1, int point2)
{
	float otherPoint1 =
		(component_.point[0].x - component_.point[1].x)
		*(other.component_.point[point1].y - component_.point[0].y)
		+ (component_.point[0].y - component_.point[1].y)
		*(component_.point[0].x - other.component_.point[point1].x);
	float otherPoint2 =
		(component_.point[0].x - component_.point[1].x)
		*(other.component_.point[point2].y - component_.point[0].y)
		+ (component_.point[0].y - component_.point[1].y)
		*(component_.point[0].x - other.component_.point[point2].x);

	if (otherPoint1*otherPoint2 < 0)
	{

		return true;
	}
	return false;
}
bool BoundingCircle::isIntersectThisRayToOtherLineBox(BoundingBox & other, int point1, int point2)
{
	float otherPoint1 =
		(other.component_.point[point1].x - other.component_.point[point2].x)
		*(component_.point[0].y - other.component_.point[point1].y)
		+ (other.component_.point[point1].y - other.component_.point[point2].y)
		*(other.component_.point[point1].x - component_.point[0].x);
	float otherPoint2 =
		(other.component_.point[point1].x - other.component_.point[point2].x)
		*(component_.point[1].y - other.component_.point[point1].y)
		+ (other.component_.point[point1].y - other.component_.point[point2].y)
		*(other.component_.point[point1].x - component_.point[1].x);

	if (otherPoint1*otherPoint2 < 0)
	{
		//DrawFormatString(375, 200, GetColor(255, 255, 255), "ìñÇΩÇ¡ÇΩ");

		return true;
	}
	return false;
}


void BoundingCircle::update(const Vector2 & center)
{
}

void BoundingCircle::debug() const
{
}
