#include "BoundingCapsule.h"
#include"BoundingBox.h"
#include"BoundingSegment.h"
#include"BoundingCircle.h"

BoundingCapsule::BoundingCapsule(Vector2 startPoint, Vector2 endPoint, float capsuleRadius) 
{
	enabled = true;
	component_=Component(startPoint, endPoint, capsuleRadius);
	bodyenub_ = false;
}

BoundingCapsule::BoundingCapsule(Vector2 startPoint, Vector2 endPoint, float capsuleRadius, bool isEnabled)
{
	enabled = isEnabled;
	component_ = Component(startPoint, endPoint, capsuleRadius);
	bodyenub_ = false;
}

BoundingCapsule::BoundingCapsule(float radius) :
	BoundingCapsule({ 0.0f, 0.0f }, { 0.0f, 0.0f }, radius,false) {
	bodyenub_ = false;
}

IBodyPtr BoundingCapsule::translate(const Vector2& position) const {
	return std::make_shared<BoundingCapsule>(component_.point[0] + position,
		component_.point[1] + position,component_.radius,enabled);
}

BoundingCapsule BoundingCapsule::transform(Vector2 startPoint, Vector2 endPoint, float capsuleRadius) const {
	return BoundingCapsule(startPoint, endPoint, capsuleRadius,enabled);
}

//bool BoundingSphere::intersects(const BoundingSphere& other) const {
//	return (component_.center_ - other.component_.center_).Length() <= component_.radius_ + other.component_.radius_
//	/*gsCollisionSphereAndSphere(&center, radius, &other.center, other.radius) == GS_TRUE*/;
//}

void BoundingCapsule::draw() const {
	DrawCircle(static_cast<int>(component_.point[0].x), static_cast<int>(component_.point[0].y), static_cast<int>(component_.radius), GetColor(255, 0, 0), FALSE);
	DrawLine(static_cast<int>(component_.point[0].x), static_cast<int>(component_.point[0].y), static_cast<int>(component_.point[1].x), static_cast<int>(component_.point[1].y), GetColor(255, 0, 0), FALSE);
	DrawLine(static_cast<int>(component_.point[0].x), static_cast<int>(component_.point[0].y), static_cast<int>(component_.point[1].x), static_cast<int>(component_.point[1].y), GetColor(255, 0, 0), FALSE);
	DrawCircle(static_cast<int>(component_.point[1].x), static_cast<int>(component_.point[1].y), static_cast<int>(component_.radius), GetColor(255, 0, 0), FALSE);

	//DrawSphere3D(Vector3::Vector3ToVECTOR(component_.center_), component_.radius_, 32, GetColor( 255,0,0 ), GetColor( 255, 255, 255 ), TRUE ) ;
}

void BoundingCapsule::draw(Matrix inv) const {
	Vector3 pos0 = Vector3(component_.point[0].x, component_.point[0].y) * inv;
	Vector3 pos1 = Vector3(component_.point[1].x, component_.point[1].y) * inv;

	//DrawCircle(pos0.x, pos0.y, component_.radius, GetColor(255, 0, 0), FALSE);
	//DrawLine(pos0.x, pos0.y, pos1.x, pos1.y, GetColor(255, 0, 0), FALSE);
	//DrawLine(pos0.x, pos0.y, pos1.x, pos1.y, GetColor(255, 0, 0), FALSE);
	//DrawCircle(pos1.x, pos1.y, component_.radius, GetColor(255, 0, 0), FALSE);

	//DrawSphere3D(Vector3::Vector3ToVECTOR(component_.center_), component_.radius_, 32, GetColor( 255,0,0 ), GetColor( 255, 255, 255 ), TRUE ) ;
}

bool BoundingCapsule::intersects(BoundingBox & other)
{
	if (!other.enabled||!enabled)return false;

	int intSet[][2] = { { 0,1 },{ 0,2 },{ 1,3 },{ 2,3 } };

	Vector2 AB = CreateVector(previousPosition_, position_);
	Vector2 AC, AD, CD, CA, CB;

	//‚·‚è”²‚¯‚½‚©
	for (int i = 0; i < 4; i++) {
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


	for (int i = 0; i < 4; i++)
	{
		float xmin = min(component_.point[0].x, component_.point[1].x);
		float xmax = max(component_.point[0].x, component_.point[1].x);
		float ymin = min(component_.point[0].y, component_.point[1].y);
		float ymax = max(component_.point[0].y, component_.point[1].y);
		float oxmin = min(other.component_.point[intSet[i][0]].x, other.component_.point[intSet[i][1]].x);
		float oxmax = max(other.component_.point[intSet[i][0]].x, other.component_.point[intSet[i][1]].x);
		float oymin = min(other.component_.point[intSet[i][0]].y, other.component_.point[intSet[i][1]].y);
		float oymax = max(other.component_.point[intSet[i][0]].y, other.component_.point[intSet[i][1]].y);


		//DrawFormatString(200 * i, 100, GetColor(255, 255, 255), "kurikaesi");
		if (isIntersectThisRayToOtherLineBox(other, intSet[i][0], intSet[i][1]))
		{
			if (isIntersectOtherRayToThisLineBox(other, intSet[i][0], intSet[i][1]))
			{

				//Œð·‚µ‚Ä‚¢‚é
				return true;
			}
			else if (xmax + component_.radius >= oxmin &&
					xmin - component_.radius <= oxmax &&
					ymax + component_.radius >= oymin &&
					ymin - component_.radius <= oymax)
			{
				float a1 = component_.point[0].y - component_.point[1].y;
				float b1 = -(component_.point[0].x - component_.point[1].x);
				float c1 = component_.point[0].x*component_.point[1].y
					- component_.point[1].x*component_.point[0].y;

				float perpendicular1 =
					(abs(a1*other.component_.point[intSet[i][0]].x + b1*other.component_.point[intSet[i][0]].y + c1)) / sqrtf(a1*a1 + b1*b1);
				float perpendicular2 =
					(abs(a1*other.component_.point[intSet[i][1]].x + b1*other.component_.point[intSet[i][1]].y + c1)) / sqrtf(a1*a1 + b1*b1);
				float perpendicularMain = min(perpendicular1, perpendicular2);
				if (component_.radius >= perpendicularMain)
				{

					return true;
				}
			}
		}
		//else if (isIntersectOtherRayToThisLineBox(other, intSet[i][0], intSet[i][1]))
		//{

		//	float a1 = other.component_.point[intSet[i][0]].y - other.component_.point[intSet[i][1]].y;
		//	float b1 = -(other.component_.point[intSet[i][0]].x - other.component_.point[intSet[i][1]].x);
		//	float c1 = other.component_.point[intSet[i][0]].x*other.component_.point[intSet[i][1]].y
		//		- other.component_.point[intSet[i][1]].x*other.component_.point[intSet[i][0]].y;
		//	//Ž©•ª‚ª‰¡AOther‚ªŽÎ‚ß(Other‚ª“_)
		//	float perpendicular1 =
		//		(abs(a1*component_.point[0].x + b1*component_.point[0].y + c1)) / sqrtf(a1*a1 + b1*b1);
		//	float perpendicular2 =
		//		(abs(a1*component_.point[1].x + b1*component_.point[1].y + c1)) / sqrtf(a1*a1 + b1*b1);
		//	float perpendicularMain = min(perpendicular1, perpendicular2);
		//	if (component_.radius >= perpendicularMain)
		//	{
		//		return true;
		//	}
		//}

		//else
		{
			float otherPoint1 =
				(other.component_.point[intSet[i][0]].x - other.component_.point[intSet[i][1]].x)
				*(component_.point[0].y - other.component_.point[intSet[i][0]].y)
				+ (other.component_.point[intSet[i][0]].y - other.component_.point[intSet[i][1]].y)
				*(other.component_.point[intSet[i][0]].x - component_.point[0].x);
			float otherPoint2 =
				(other.component_.point[intSet[i][0]].x - other.component_.point[intSet[i][1]].x)
				*(component_.point[1].y - other.component_.point[intSet[i][0]].y)
				+ (other.component_.point[intSet[i][0]].y - other.component_.point[intSet[i][1]].y)
				*(other.component_.point[intSet[i][0]].x - component_.point[1].x);

			if (otherPoint1 == otherPoint2)
			{
				//if (xmax == xmin || ymax == ymin)
				//{
				//	if (xmax + component_.radius >= oxmin &&
				//		xmin - component_.radius <= oxmax &&
				//		ymax + component_.radius >= oymin &&
				//		ymin - component_.radius <= oymax)
				//	{

				//		float a1 = other.component_.point[intSet[i][1]].y - other.component_.point[intSet[i][0]].y;
				//		float b1 = -(other.component_.point[intSet[i][1]].x - other.component_.point[intSet[i][0]].x);
				//		float c1 = other.component_.point[intSet[i][1]].y*(other.component_.point[intSet[i][1]].x - other.component_.point[intSet[i][0]].x)
				//			- other.component_.point[intSet[i][1]].x*(other.component_.point[intSet[i][1]].y - other.component_.point[intSet[i][0]].y);
				//		//Ž©•ª‚ªŽÎ‚ßAOther‚ª‰¡(Ž©•ª‚ª“_)
				//		float perpendicular1 =
				//			(abs(a1*component_.point[0].x + b1*component_.point[0].y + c1)) / sqrtf(a1*a1 + b1*b1);
				//		float perpendicular2 =
				//			(abs(a1*component_.point[1].x + b1*component_.point[1].y + c1)) / sqrtf(a1*a1 + b1*b1);
				//		float perpendicularMain = min(perpendicular1, perpendicular2);
				//		if (component_.radius >= perpendicularMain)
				//		{
				//			return true;
				//		}


				//	}
				//}
				if (xmax >= oxmin&& xmin <= oxmax)
				{
					if (ymax >= oymin&&ymin <= oymax)
					{
						float a1 = other.component_.point[intSet[i][1]].y - other.component_.point[intSet[i][0]].y;
						float b1 = -(other.component_.point[intSet[i][1]].x - other.component_.point[intSet[i][0]].x);
						float c1 = other.component_.point[intSet[i][1]].y*(other.component_.point[intSet[i][1]].x - other.component_.point[intSet[i][0]].x)
							- other.component_.point[intSet[i][1]].x*(other.component_.point[intSet[i][1]].y - other.component_.point[intSet[i][0]].y);
						//Ž©•ª‚ªŽÎ‚ßAOther‚ª‰¡(Ž©•ª‚ª“_)
						float perpendicular1 =
							(abs(a1*component_.point[0].x + b1*component_.point[0].y + c1)) / sqrtf(a1*a1 + b1*b1);
						float perpendicular2 =
							(abs(a1*component_.point[1].x + b1*component_.point[1].y + c1)) / sqrtf(a1*a1 + b1*b1);
						float perpendicularMain = min(perpendicular1, perpendicular2);
						if (component_.radius >= perpendicularMain)
						{

							return true;
						}

					}
				}
			}
			//2ü•ª‚ªŒð‚í‚ç‚È‚¢
			float perpendicular1 =
				sqrtf(
					(other.component_.point[intSet[i][0]].x - component_.point[0].x)*
					(other.component_.point[intSet[i][0]].x - component_.point[0].x) +
					(other.component_.point[intSet[i][0]].y - component_.point[0].y)*
					(other.component_.point[intSet[i][0]].y - component_.point[0].y)
					);
			float perpendicular2 =
				sqrtf(
					(other.component_.point[intSet[i][0]].x - component_.point[1].x)*
					(other.component_.point[intSet[i][0]].x - component_.point[1].x) +
					(other.component_.point[intSet[i][0]].y - component_.point[1].y)*
					(other.component_.point[intSet[i][0]].y - component_.point[1].y)
					);
			float perpendicular3 =
				sqrtf(
					(other.component_.point[intSet[i][1]].x - component_.point[0].x)*
					(other.component_.point[intSet[i][1]].x - component_.point[0].x) +
					(other.component_.point[intSet[i][1]].y - component_.point[0].y)*
					(other.component_.point[intSet[i][1]].y - component_.point[0].y)
					);
			float perpendicular4 =
				sqrtf(
					(other.component_.point[intSet[i][1]].x - component_.point[1].x)*
					(other.component_.point[intSet[i][1]].x - component_.point[1].x) +
					(other.component_.point[intSet[i][1]].y - component_.point[1].y)*
					(other.component_.point[intSet[i][1]].y - component_.point[1].y)
					);
			float perpendicularMin1 = min(perpendicular1, perpendicular2);
			float perpendicularMin2 = min(perpendicular3, perpendicular4);
			float perpendicularMain = min(perpendicularMin1, perpendicularMin2);
			if (component_.radius >= perpendicularMain)
			{

				return true;
			}
		}
	}
	return false;


}
bool BoundingCapsule::intersects(BoundingCapsule & other){
	if (!other.enabled || !enabled)return false;

	Vector2 AB = CreateVector(previousPosition_, position_);
	Vector2 AC, AD, CD, CA, CB;

	//‚·‚è”²‚¯‚½‚©
	for (int i = 0; i < 4; i++) {
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
	}


	//2‰~‚ÌŠe+-‚Éü‚ðˆø‚«A‚»‚Ìü‚Æ‚Ì“–‚½‚è”»’è
	float sin90, cos90;
	sin90 = 1;
	cos90 = 0;

	Vector2 A = CreateVector(component_.point[0], component_.point[1]);
	Vector2 AN = A.Normalize();
	Vector2 rotationA{ (AN.x*cos90) - (AN.y*sin90) , (AN.x*sin90) + (AN.y*cos90) };
	Vector2 A0P = component_.point[0] + rotationA * component_.radius;
	Vector2 A0M = component_.point[0] - rotationA * component_.radius;
	Vector2 A1P = component_.point[1] + rotationA * component_.radius;
	Vector2 A1M = component_.point[1] - rotationA * component_.radius;
	
	Vector2 B = CreateVector(other.component_.point[0], other.component_.point[1]);
	Vector2 BN = B.Normalize();
	Vector2 rotationB{ (BN.x*cos90) - (BN.y*sin90) , (BN.x*sin90) + (BN.y*cos90) };
	Vector2 B0P = other.component_.point[0] + rotationB * other.component_.radius;
	Vector2 B0M = other.component_.point[0] - rotationB * other.component_.radius;
	Vector2 B1P = other.component_.point[1] + rotationB * other.component_.radius;
	Vector2 B1M = other.component_.point[1] - rotationB * other.component_.radius;

	Vector2 pqi, pmi,pqx,pmx;
	float inneri, ki, pqd2i, pmd2i, phd2i, d2i;
	float innerx, kx, pqd2x, pmd2x, phd2x, d2x;

	Vector2 As[][2]{ { A0P,A1P },{ A0M,A1M } };
	Vector2 Bs[][2]{ { B0P,B1P },{ B0M,B1M } };
	DrawLine(static_cast<int>(A0P.x), static_cast<int>(A0P.y), static_cast<int>(A1P.x), static_cast<int>(A1P.y), GetColor(255, 255, 255));
	DrawLine(static_cast<int>(A0M.x), static_cast<int>(A0M.y), static_cast<int>(A1M.x), static_cast<int>(A1M.y), GetColor(255, 255, 255));
	DrawLine(static_cast<int>(B0P.x), static_cast<int>(B0P.y), static_cast<int>(B1P.x), static_cast<int>(B1P.y), GetColor(255, 255, 255));
	DrawLine(static_cast<int>(B0M.x), static_cast<int>(B0M.y), static_cast<int>(B1M.x), static_cast<int>(B1M.y), GetColor(255, 255, 255));

	for (int i = 0; i < 2; i++)
	{
		for (int x = 0; x < 2; x++)
		{
			if (isIntersectOtherRayToThisLine(As[i][0],As[i][1], Bs[x][0], Bs[x][1]))
			{
				return true;
			}
		}
	}
	//‰~‚Æü•ª‚Ì”»’è
	{
		for (int i = 0; i < 2; i++)
		{
			for (int x = 0; x < 2; x++)
			{
				pqi = CreateVector(As[i][0], As[i][1]);
				pmi = CreateVector(As[i][0], other.component_.point[x]);
				inneri = InnerProduct(pqi, pmi);
				pqd2i = pqi.LengthSquared();
				pmd2i = pmi.LengthSquared();
				ki = inneri / pqd2i;

				if (ki < 0 || 1 < ki)continue;

				phd2i = (inneri*inneri) / pqd2i;
				d2i = pmd2i - phd2i;

				if (d2i < other.component_.radius*other.component_.radius)return true;
			}
		}
		for (int x = 0; x < 2; x++)
		{
			for (int i = 0; i < 2; i++)
			{
				pqx = CreateVector(Bs[x][0], Bs[x][1]);
				pmx = CreateVector(Bs[x][0], component_.point[i]);
				innerx = InnerProduct(pqx, pmx);
				pqd2x = pqx.LengthSquared();
				pmd2x = pmx.LengthSquared();
				kx = innerx / pqd2x;

				if (kx < 0 || 1 < kx)continue;

				phd2x = (innerx*innerx) / pqd2x;
				d2x = pmd2x - phd2x;

				if (d2x < component_.radius*component_.radius)return true;
			}
		}
	}
	//‰~“¯Žm‚Ì”»’è
	float dx, dy, r;
	for (int i = 0; i < 2; i++)
	{
		for (int x = 0; x < 2; x++)
		{
			dx = component_.point[i].x - other.component_.point[x].x;
			dy = component_.point[i].y - other.component_.point[x].y;
			r = component_.radius + other.component_.radius;
			if ((dx*dx) + (dy*dy) < (r*r)) {
				return true;
			};
		}
	}
	return false;
}
bool BoundingCapsule::intersects(BoundingSegment & other) {
	if (!enabled || !other.enabled)return false;

	Vector2 AB = CreateVector(previousPosition_, position_);
	Vector2 AC, AD, CD, CA, CB;

	//‚·‚è”²‚¯‚½‚©
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


	//2‰~‚ÌŠe+-‚Éü‚ðˆø‚«A‚»‚Ìü‚Æ‚Ì“–‚½‚è”»’è
	float sin90, cos90;
	sin90 = 1;
	cos90 = 0;

	Vector2 A = CreateVector(component_.point[0], component_.point[1]);
	Vector2 AN = A.Normalize();
	Vector2 rotationA{ (AN.x*cos90) - (AN.y*sin90) , (AN.x*sin90) + (AN.y*cos90) };
	Vector2 A0P = component_.point[0] + rotationA * component_.radius;
	Vector2 A0M = component_.point[0] - rotationA * component_.radius;

	Vector2 A1P = component_.point[1] + rotationA * component_.radius;
	Vector2 A1M = component_.point[1] - rotationA * component_.radius;

	Vector2 pq, pm;
	float inner, k, pqd2, pmd2, phd2, d2;

	Vector2 As[][2]{ { A0P,A1P },{ A0M,A1M } };

	//ü•ª‚Æ‚Ì“–‚½‚è”»’è
	for (int i = 0; i < 2; i++)
	{
		float otherPoint1 =
			(other.component_.point[0].x - other.component_.point[1].x)
			*(As[i][0].y - other.component_.point[0].y)
			+ (other.component_.point[0].y - other.component_.point[1].y)
			*(other.component_.point[0].x - As[i][0].x);
		float otherPoint2 =
			(other.component_.point[0].x - other.component_.point[1].x)
			*(As[i][1].y - other.component_.point[0].y)
			+ (other.component_.point[0].y - other.component_.point[1].y)
			*(other.component_.point[0].x - As[i][1].x);

		if (otherPoint1*otherPoint2 < 0)
		{
			float otherPoint1 =
				(As[i][0].x - As[i][1].x)
				*(other.component_.point[0].y - As[i][0].y)
				+ (As[i][0].y - As[i][1].y)
				*(As[i][0].x - other.component_.point[0].x);
			float otherPoint2 =
				(As[i][0].x - As[i][1].x)
				*(other.component_.point[1].y - As[i][0].y)
				+ (As[i][0].y - As[i][1].y)
				*(As[i][0].x - other.component_.point[1].x);

			if (otherPoint1*otherPoint2 < 0)
			{

				return true;
			}
		}
	}
	//‰~‚Æ‚Ì“–‚½‚è”»’è
	float dx, dy, r;

	for (int i = 0; i < 2; i++)
	{
		for (int x = 0; x < 2; x++)
		{
			dx = other.component_.point[i].x - component_.point[x].x;
			dy = other.component_.point[i].y - component_.point[x].y;
			r = component_.radius;
			if ((dx*dx) + (dy*dy) < (r*r))
			{
				return true;
			}
		}
	}

	for (int i = 0; i < 2; i++)
	{
		pq = CreateVector(other.component_.point[0], other.component_.point[1]);
		pm = CreateVector(other.component_.point[0], component_.point[i]);

		inner = InnerProduct(pq, pm);
		pqd2 = pq.LengthSquared();
		pmd2 = pm.LengthSquared();

		k = inner / pqd2;

		if (k < 0 || 1 < k) continue;

		phd2 = (inner*inner) / pqd2;
		d2 = pmd2 - phd2;

		if (d2 < component_.radius*component_.radius)return true;
	}
	return false;
}

bool BoundingCapsule::intersects(BoundingCircle & other)
{
	if (!enabled || !other.enabled)return false;

	Vector2 AB = CreateVector(other.previousPosition_, other.position_);
	Vector2 AC, AD, CD, CA, CB;

	//‚·‚è”²‚¯‚½‚©
	AC = CreateVector(other.previousPosition_, component_.point[0]);
	AD = CreateVector(other.previousPosition_, component_.point[1]);
	CD = CreateVector(component_.point[0], component_.point[1]);
	CA = CreateVector(component_.point[0], other.previousPosition_);
	CB = CreateVector(component_.point[0], other.position_);

	if (OuterProduct(AB, AC)*OuterProduct(AB, AD) <= 0.0f&&
		OuterProduct(CD, CA)*OuterProduct(CD, CB) < 0.0f)
	{
		//DrawFormatString(400, 400, GetColor(255, 255, 255), "deta");
		return true;
	}

	


	//2‰~‚ÌŠe+-‚Éü‚ðˆø‚«A‚»‚Ìü‚Æ‚Ì“–‚½‚è”»’è
	float sin90, cos90;
	sin90 = 1;
	cos90 = 0;

	Vector2 A = CreateVector(component_.point[0], component_.point[1]);
	Vector2 AN = A.Normalize();
	Vector2 rotationA{ (AN.x*cos90) - (AN.y*sin90) , (AN.x*sin90) + (AN.y*cos90) };
	Vector2 A0P = component_.point[0] + rotationA * component_.radius;
	Vector2 A0M = component_.point[0] - rotationA * component_.radius;

	Vector2 A1P = component_.point[1] + rotationA * component_.radius;
	Vector2 A1M = component_.point[1] - rotationA * component_.radius;

	Vector2 pq, pm;
	float inner, k, pqd2, pmd2, phd2, d2;

	Vector2 As[][2]{ { A0P,A1P },{ A0M,A1M } };

	for (int i = 0; i < 2; i++)
	{
		pq = CreateVector(As[i][0], As[i][1]);
		pm = CreateVector(As[i][0], other.component_.point[0]);

		//inner = Vector2::Dot(pq, pm);
		inner = InnerProduct(pq, pm);
		pqd2 = pq.LengthSquared();
		pmd2 = pm.LengthSquared();

		k = inner / pqd2;

		if (k < 0 || 1 < k)continue;

		phd2 = (inner*inner) / pqd2;
		d2 = pmd2 - phd2;

		if (d2 < other.component_.radius*other.component_.radius)return true;
	}

	//‰~“¯Žm‚Ì”»’è
	float dx, dy, r;
	for (int i = 0; i < 2; i++)
	{
		dx = component_.point[i].x - other.component_.point[0].x;
		dy = component_.point[i].y - other.component_.point[0].y;
		r = component_.radius + other.component_.radius;
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



bool BoundingCapsule::isIntersectOtherRayToThisLineSegment(BoundingSegment & other)
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
bool BoundingCapsule::isIntersectThisRayToOtherLineSegment(BoundingSegment & other)
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
bool BoundingCapsule::isIntersectOtherRayToThisLineCapsule(BoundingCapsule & other)
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
bool BoundingCapsule::isIntersectThisRayToOtherLineCapsule(BoundingCapsule & other)
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
bool BoundingCapsule::isIntersectOtherRayToThisLineBox(BoundingBox & other,int point1,int point2)
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
bool BoundingCapsule::isIntersectThisRayToOtherLineBox(BoundingBox & other,int point1,int point2)
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
		//DrawFormatString(375, 200, GetColor(255, 255, 255), "“–‚½‚Á‚½");

		return true;
	}
	return false;
}


void BoundingCapsule::update(const Vector2 & center)
{
}

void BoundingCapsule::debug() const
{
}
