#include "BoundingBox.h"
#include"BoundingSegment.h"
#include"BoundingCircle.h"
#include"BoundingCapsule.h"
#include"../../Define.h"

BoundingBox::BoundingBox(const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomLeft, const Vector2& bottomRight)
{
	enabled = true;
	component_ = Component(topLeft, topRight, bottomLeft, bottomRight);
	bodyenub_ = false;
}
BoundingBox::BoundingBox(const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomLeft, const Vector2& bottomRight,bool isEnabled)
{
	enabled = isEnabled;
	component_ = Component(topLeft, topRight, bottomLeft, bottomRight);
	bodyenub_ = false;

}
BoundingBox::BoundingBox() 
{
	enabled = false;
	component_ = Component(0, 0, 0, 0);
	bodyenub_ = false;
}

IBodyPtr BoundingBox::translate(const Vector2& position) const {

	return std::make_shared<BoundingBox>( component_.point[0] + position,
						component_.point[1] + position,
						component_.point[2] + position,
						component_.point[3] + position,
						enabled);
}

BoundingBox BoundingBox::transform(const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomLeft, const Vector2& bottomRight) const {
	return BoundingBox(topLeft, topRight, bottomLeft, bottomRight,enabled);
}

//bool BoundingSphere::intersects(const BoundingSphere& other) const {
//	return (component_.center_ - other.component_.center_).Length() <= component_.radius_ + other.component_.radius_
//	/*gsCollisionSphereAndSphere(&center, radius, &other.center, other.radius) == GS_TRUE*/;
//}

void BoundingBox::draw() const {
	//if (!enabled)return;

	DrawLine(static_cast<int>(component_.point[0].x), static_cast<int>(component_.point[0].y), static_cast<int>(component_.point[1].x), static_cast<int>(component_.point[1].y), GetColor(255, 0, 0));
	DrawLine(static_cast<int>(component_.point[1].x), static_cast<int>(component_.point[1].y), static_cast<int>(component_.point[3].x), static_cast<int>(component_.point[3].y), GetColor(255, 0, 0));
	DrawLine(static_cast<int>(component_.point[0].x), static_cast<int>(component_.point[0].y), static_cast<int>(component_.point[2].x), static_cast<int>(component_.point[2].y), GetColor(255, 0, 0));
	DrawLine(static_cast<int>(component_.point[2].x), static_cast<int>(component_.point[2].y), static_cast<int>(component_.point[3].x), static_cast<int>(component_.point[3].y), GetColor(255, 0, 0));
	

	//DrawBox(component_.point[0].x, component_.point[0].y,
	//	component_.point[3].x, component_.point[3].y, GetColor(255, 0, 0), FALSE);
	//DrawSphere3D(Vector3::Vector3ToVECTOR(component_.center_), component_.radius_, 32, GetColor( 255,0,0 ), GetColor( 255, 255, 255 ), TRUE ) ;
}

void BoundingBox::draw(Matrix inv) const {
	//if (!enabled)return;

	Vector3 pos0 = Vector3(component_.point[0].x, component_.point[0].y) * inv;
	Vector3 pos1 = Vector3(component_.point[1].x, component_.point[1].y) * inv;
	Vector3 pos2 = Vector3(component_.point[2].x, component_.point[2].y) * inv;
	Vector3 pos3 = Vector3(component_.point[3].x, component_.point[3].y) * inv;

	//DrawBox(static_cast<int>(pos0.x), static_cast<int>(pos0.y), static_cast<int>(pos3.x), static_cast<int>(pos3.y), GetColor(255, 255, 255), TRUE);

	DrawLine((int)pos0.x, (int)pos0.y, (int)pos1.x, (int)pos1.y, GetColor(255, 0, 0));
	DrawLine((int)pos1.x, (int)pos1.y, (int)pos3.x, (int)pos3.y, GetColor(255, 0, 0));
	DrawLine((int)pos0.x, (int)pos0.y, (int)pos2.x, (int)pos2.y, GetColor(255, 0, 0));
	DrawLine((int)pos2.x, (int)pos2.y, (int)pos3.x, (int)pos3.y, GetColor(255, 0, 0));

	//DrawBox(component_.point[0].x, component_.point[0].y,
	//	component_.point[3].x, component_.point[3].y, GetColor(255, 0, 0), FALSE);
	//DrawSphere3D(Vector3::Vector3ToVECTOR(component_.center_), component_.radius_, 32, GetColor( 255,0,0 ), GetColor( 255, 255, 255 ), TRUE ) ;
}

void BoundingBox::draw(int spriteID, Matrix inv) const
{
	Vector3 pos0 = Vector3(component_.point[0].x, component_.point[0].y) * inv;
	Vector3 pos1 = Vector3(component_.point[1].x, component_.point[1].y) * inv;
	Vector3 pos2 = Vector3(component_.point[2].x, component_.point[2].y) * inv;
	Vector3 pos3 = Vector3(component_.point[3].x, component_.point[3].y) * inv;

	DrawGraph(static_cast<int>(pos0.x), static_cast<int>(pos0.y), spriteID, TRUE);
}

void BoundingBox::draw(int spriteID, int rotation, Matrix inv) const
{
	Vector3 pos0 = Vector3(component_.point[0].x, component_.point[0].y) * inv;
	Vector3 pos1 = Vector3(component_.point[1].x, component_.point[1].y) * inv;
	Vector3 pos2 = Vector3(component_.point[2].x, component_.point[2].y) * inv;
	Vector3 pos3 = Vector3(component_.point[3].x, component_.point[3].y) * inv;

	Vector2 centerMath1, centerMath2, center;

	centerMath1.x = (pos0.x + pos1.x) / 2;
	centerMath1.y = (pos0.y + pos1.y) / 2;
	centerMath2.x = (pos2.x + pos3.x) / 2;
	centerMath2.y = (pos2.y + pos3.y) / 2;
	center = (centerMath1 + centerMath2) / 2;

	DrawRotaGraph(static_cast<int>(center.x), static_cast<int>(center.y), 1, static_cast<int>((rotation*MathHelper::Pi / 180)), spriteID, TRUE);

	//DrawLine(pos0.x, pos0.y, pos1.x, pos1.y, GetColor(255, 0, 0));
	//DrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 0, 0));
	//DrawLine(pos0.x, pos0.y, pos2.x, pos2.y, GetColor(255, 0, 0));
	//DrawLine(pos2.x, pos2.y, pos3.x, pos3.y, GetColor(255, 0, 0));
}
void BoundingBox::draw(int spriteID, Matrix inv, int width, int height) const
{
	for (int i = 0; i < width; i++) {
		for (int x = 0; x < height; x++) {

			Vector3 pos0 = Vector3(component_.point[0].x + (i*CHIPSIZE), component_.point[0].y + (x*CHIPSIZE)) * inv;
			Vector3 pos1 = Vector3(component_.point[1].x + (i*CHIPSIZE), component_.point[1].y + (x*CHIPSIZE)) * inv;
			Vector3 pos2 = Vector3(component_.point[2].x + (i*CHIPSIZE), component_.point[2].y + (x*CHIPSIZE)) * inv;
			Vector3 pos3 = Vector3(component_.point[3].x + (i*CHIPSIZE), component_.point[3].y + (x*CHIPSIZE)) * inv;

			DrawGraph(static_cast<int>(pos0.x), static_cast<int>(pos0.y), spriteID, TRUE);
		}
	}

	//Vector3 pos0 = Vector3(component_.point[0].x, component_.point[0].y) * inv;
	//Vector3 pos1 = Vector3(component_.point[1].x, component_.point[1].y) * inv;
	//Vector3 pos2 = Vector3(component_.point[2].x, component_.point[2].y) * inv;
	//Vector3 pos3 = Vector3(component_.point[3].x, component_.point[3].y) * inv;

	//DrawLine(pos0.x, pos0.y, pos1.x, pos1.y, GetColor(255, 0, 0));
	//DrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 0, 0));
	//DrawLine(pos0.x, pos0.y, pos2.x, pos2.y, GetColor(255, 0, 0));
	//DrawLine(pos2.x, pos2.y, pos3.x, pos3.y, GetColor(255, 0, 0));
}

bool BoundingBox::intersects(BoundingBox & other)
{
	if (!enabled || !other.enabled)return false;

	int intSet[][2] = { { 0,1 },{ 0,2 },{ 1,3 },{ 2,3 } };

	Vector2 AB = CreateVector(previousPosition_, position_);
	Vector2 AC, AD, CD, CA, CB;

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



	for (int i = 0; i < 4; i++)
	{
		for (int x = 0; x < 4; x++)
		{
			//判定の大きさ、位置が完全に一致した場合の例外
			if(component_.point[intSet[i][0]].x==other.component_.point[intSet[x][0]].x&&component_.point[intSet[i][0]].y == other.component_.point[intSet[x][0]].y
				&&component_.point[intSet[i][1]].x == other.component_.point[intSet[x][1]].x&&component_.point[intSet[i][1]].y == other.component_.point[intSet[x][1]].y)
			{
				return true;
			}
			float otherPoint1 =
				(other.component_.point[intSet[x][0]].x - other.component_.point[intSet[x][1]].x)
				*(component_.point[intSet[i][0]].y - other.component_.point[intSet[x][0]].y)
				+ (other.component_.point[intSet[x][0]].y - other.component_.point[intSet[x][1]].y)
				*(other.component_.point[intSet[x][0]].x - component_.point[intSet[i][0]].x);
			float otherPoint2 =
				(other.component_.point[intSet[x][0]].x - other.component_.point[intSet[x][1]].x)
				*(component_.point[intSet[i][1]].y - other.component_.point[intSet[x][0]].y)
				+ (other.component_.point[intSet[x][0]].y - other.component_.point[intSet[x][1]].y)
				*(other.component_.point[intSet[x][0]].x - component_.point[intSet[i][1]].x);

			//2直線が交差しているかを調べる
			if (otherPoint1*otherPoint2 < 0)
			{
				float otherPoint1 =
					(component_.point[intSet[i][0]].x - component_.point[intSet[i][1]].x)
					*(other.component_.point[intSet[x][0]].y - component_.point[intSet[i][0]].y)
					+ (component_.point[intSet[i][0]].y - component_.point[intSet[i][1]].y)
					*(component_.point[intSet[i][0]].x - other.component_.point[intSet[x][0]].x);
				float otherPoint2 =
					(component_.point[intSet[i][0]].x - component_.point[intSet[i][1]].x)
					*(other.component_.point[intSet[x][1]].y - component_.point[intSet[i][0]].y)
					+ (component_.point[intSet[i][0]].y - component_.point[intSet[i][1]].y)
					*(component_.point[intSet[i][0]].x - other.component_.point[intSet[x][1]].x);

				if (otherPoint1*otherPoint2 <= 0)
				{

					return true;
				}
			}
		}
	}
	return false;
}
//bool BoundingBox::intersects(BoundingBox & other)
//{
//	if (!enabled)return false;
//
//	if (component_.point[1].x >= other.component_.point[0].x&&
//		component_.point[0].x <= other.component_.point[1].x)
//	{
//		if (component_.point[2].y >= other.component_.point[0].y&&
//			component_.point[0].y <= other.component_.point[2].y)
//		{
//			return true;
//		}
//	}
//	return false;
//}

bool BoundingBox::intersects(BoundingCapsule & other) {
	if (!enabled || !other.enabled)return false;

	int intSet[][2] = { { 0,1 },{ 0,2 },{ 1,3 },{ 2,3 } };

	Vector2 AB = CreateVector(other.previousPosition_, other.position_);
	Vector2 AC, AD, CD, CA, CB;

	for (int i = 0; i < 4; i++)
	{
		//すり抜けたか
		AC = CreateVector(other.previousPosition_, component_.point[intSet[i][0]]);
		AD = CreateVector(other.previousPosition_, component_.point[intSet[i][1]]);
		CD = CreateVector(component_.point[intSet[i][0]], component_.point[intSet[i][1]]);
		CA = CreateVector(component_.point[intSet[i][0]], other.previousPosition_);
		CB = CreateVector(component_.point[intSet[i][0]], other.position_);

		if (OuterProduct(AB, AC)*OuterProduct(AB, AD) <= 0.0f&&
			OuterProduct(CD, CA)*OuterProduct(CD, CB) < 0.0f)
		{
			//DrawFormatString(400, 400, GetColor(255, 255, 255), "deta");
			return true;
		}
	}


	for (int i = 0; i < 4; i++)
	{
			float xmin = min(other.component_.point[0].x, other.component_.point[1].x);
			float xmax = max(other.component_.point[0].x, other.component_.point[1].x);
			float ymin = min(other.component_.point[0].y, other.component_.point[1].y);
			float ymax = max(other.component_.point[0].y, other.component_.point[1].y);
			float oxmin = min(component_.point[intSet[i][0]].x, component_.point[intSet[i][1]].x);
			float oxmax = max(component_.point[intSet[i][0]].x, component_.point[intSet[i][1]].x);
			float oymin = min(component_.point[intSet[i][0]].y, component_.point[intSet[i][1]].y);
			float oymax = max(component_.point[intSet[i][0]].y, component_.point[intSet[i][1]].y);
					
		if (isIntersectOtherRayToThisLine(other, intSet[i][0], intSet[i][1]))
		{
			if (isIntersectThisRayToOtherLine(other, intSet[i][0], intSet[i][1]))
			{
				//DrawFormatString(375, 50*i, GetColor(255, 255, 255), "当たった");
				//交差している
				return true;
			}
			else if (xmax + other.component_.radius >= oxmin &&
				xmin - other.component_.radius <= oxmax &&
				ymax + other.component_.radius >= oymin &&
				ymin - other.component_.radius <= oymax)
			{
				float a1 = other.component_.point[0].y - other.component_.point[1].y;
				float b1 = -(other.component_.point[0].x - other.component_.point[1].x);
				float c1 = (other.component_.point[0].x*other.component_.point[1].y)
					- (other.component_.point[1].x*other.component_.point[0].y);
				//自分が斜め、Otherが横(自分が点)
				float perpendicular1 =
					(abs(a1*component_.point[intSet[i][0]].x + b1*component_.point[intSet[i][0]].y + c1)) / sqrtf(a1*a1 + b1*b1);
				float perpendicular2 =
					(abs(a1*component_.point[intSet[i][1]].x + b1*component_.point[intSet[i][1]].y + c1)) / sqrtf(a1*a1 + b1*b1);
				float perpendicularMain = min(perpendicular1, perpendicular2);
				if (other.component_.radius >= perpendicularMain)
				{

					return true;
				}
			}
		}
		
		//else if (isIntersectThisRayToOtherLine(other, intSet[i][0], intSet[i][1]))
		//{
		//	//DrawFormatString(200 * i, 100, GetColor(255, 255, 255), "kurikaesi");
		//	float a1 = component_.point[intSet[i][0]].y - component_.point[intSet[i][1]].y;
		//	float b1 = -(component_.point[intSet[i][0]].x - component_.point[intSet[i][1]].x);
		//	float c1 = (component_.point[intSet[i][0]].x*component_.point[intSet[i][1]].y)
		//		- (component_.point[intSet[i][1]].x*component_.point[intSet[i][0]].y);
		//	//自分が横、Otherが斜め(Otherが点)
		//	float perpendicular1 =
		//		(abs(a1*other.component_.point[0].x + b1*other.component_.point[0].y + c1)) / sqrtf(a1*a1 + b1*b1);
		//	float perpendicular2 =
		//		(abs(a1*other.component_.point[1].x + b1*other.component_.point[1].y + c1)) / sqrtf(a1*a1 + b1*b1);
		//	float perpendicularMain = min(perpendicular1, perpendicular2);
		//	if (other.component_.radius >= perpendicularMain)
		//	{
		//		return true;
		//	}
		//}

		{
			float otherPoint1 =
				(component_.point[intSet[i][0]].x - component_.point[intSet[i][1]].x)
				*(other.component_.point[0].y - component_.point[intSet[i][0]].y)
				+ (component_.point[intSet[i][0]].y - component_.point[intSet[i][1]].y)
				*(component_.point[intSet[i][0]].x - other.component_.point[0].x);
			float otherPoint2 =
				(component_.point[intSet[i][0]].x - component_.point[intSet[i][1]].x)
				*(other.component_.point[1].y - component_.point[intSet[i][0]].y)
				+ (component_.point[intSet[i][0]].y - component_.point[intSet[i][1]].y)
				*(component_.point[intSet[i][0]].x - other.component_.point[1].x);
			if (otherPoint1 == otherPoint2)
			{
				//if (xmax == xmin || ymax == ymin)
				//{
				//	if (xmax + other.component_.radius >= oxmin &&
				//		xmin - other.component_.radius <= oxmax &&
				//		ymax + other.component_.radius >= oymin &&
				//		ymin - other.component_.radius <= oymax)
				//	{
				//		float a1 = component_.point[intSet[i][1]].y - component_.point[intSet[i][0]].y;
				//		float b1 = -(component_.point[intSet[i][1]].x - component_.point[intSet[i][0]].x);
				//		float c1 = component_.point[intSet[i][1]].y*(component_.point[intSet[i][1]].x - component_.point[intSet[i][0]].x)
				//			- component_.point[intSet[i][1]].x*(component_.point[intSet[i][1]].y - component_.point[intSet[i][0]].y);
				//		//自分が斜め、Otherが横(自分が点)
				//		float perpendicular1 =
				//			(abs(a1*other.component_.point[0].x + b1*other.component_.point[0].y + c1)) / sqrtf(a1*a1 + b1*b1);
				//		float perpendicular2 =
				//			(abs(a1*other.component_.point[1].x + b1*other.component_.point[1].y + c1)) / sqrtf(a1*a1 + b1*b1);
				//		float perpendicularMain = min(perpendicular1, perpendicular2);
				//		if (other.component_.radius >= perpendicularMain)
				//		{
				//			return true;
				//		}
				//	}
				//}
				if (xmax >= oxmin&& xmin <= oxmax)
				{
					if (ymax >= oymin&&ymin <= oymax)
					{
						float a1 = component_.point[intSet[i][1]].y - component_.point[intSet[i][0]].y;
						float b1 = -(component_.point[intSet[i][1]].x - component_.point[intSet[i][0]].x);
						float c1 = component_.point[intSet[i][1]].y*(component_.point[intSet[i][1]].x - component_.point[intSet[i][0]].x)
							- component_.point[intSet[i][1]].x*(component_.point[intSet[i][1]].y - component_.point[intSet[i][0]].y);
						//自分が斜め、Otherが横(自分が点)
						float perpendicular1 =
							(abs(a1*other.component_.point[0].x + b1*other.component_.point[0].y + c1)) / sqrtf(a1*a1 + b1*b1);
						float perpendicular2 =
							(abs(a1*other.component_.point[1].x + b1*other.component_.point[1].y + c1)) / sqrtf(a1*a1 + b1*b1);
						float perpendicularMain = min(perpendicular1, perpendicular2);
						if (other.component_.radius >= perpendicularMain)
						{
							return true;
						}
					}
				}
			}
			//2線分が交わらない
			float perpendicular1 =
				sqrtf(
					(other.component_.point[0].x - component_.point[intSet[i][0]].x)*
					(other.component_.point[0].x - component_.point[intSet[i][0]].x) +
					(other.component_.point[0].y - component_.point[intSet[i][0]].y)*
					(other.component_.point[0].y - component_.point[intSet[i][0]].y)
					);
			float perpendicular2 =
				sqrtf(
					(other.component_.point[0].x - component_.point[intSet[i][1]].x)*
					(other.component_.point[0].x - component_.point[intSet[i][1]].x) +
					(other.component_.point[0].y - component_.point[intSet[i][1]].y)*
					(other.component_.point[0].y - component_.point[intSet[i][1]].y)
					);
			float perpendicular3 =
				sqrtf(
					(other.component_.point[1].x - component_.point[intSet[i][0]].x)*
					(other.component_.point[1].x - component_.point[intSet[i][0]].x) +
					(other.component_.point[1].y - component_.point[intSet[i][0]].y)*
					(other.component_.point[1].y - component_.point[intSet[i][0]].y)
					);
			float perpendicular4 =
				sqrtf(
					(other.component_.point[1].x - component_.point[intSet[i][1]].x)*
					(other.component_.point[1].x - component_.point[intSet[i][1]].x) +
					(other.component_.point[1].y - component_.point[intSet[i][1]].y)*
					(other.component_.point[1].y - component_.point[intSet[i][1]].y)
					);
			float perpendicularMin1 = min(perpendicular1, perpendicular2);
			float perpendicularMin2 = min(perpendicular3, perpendicular4);
			float perpendicularMain = min(perpendicularMin1, perpendicularMin2);
			if (other.component_.radius >= perpendicularMain)
			{
				return true;
			}
		}
	}
	return false;
}
bool BoundingBox::intersects(BoundingSegment & other)
{
	if (!enabled || !other.enabled)return false;

	int intSet[][2] = { { 0,1 },{ 0,2 },{ 1,3 },{ 2,3 } };

	Vector2 AB = CreateVector(other.previousPosition_, other.position_);
	Vector2 AC, AD, CD, CA, CB;

	for (int i = 0; i < 4; i++)
	{
		AC = CreateVector(other.previousPosition_, component_.point[intSet[i][0]]);
		AD = CreateVector(other.previousPosition_, component_.point[intSet[i][1]]);
		CD = CreateVector(component_.point[intSet[i][0]], component_.point[intSet[i][1]]);
		CA = CreateVector(component_.point[intSet[i][0]], other.previousPosition_);
		CB = CreateVector(component_.point[intSet[i][0]], other.position_);

		if (OuterProduct(AB, AC)*OuterProduct(AB, AD) <= 0.0f&&
			OuterProduct(CD, CA)*OuterProduct(CD, CB) < 0.0f)
		{
			//DrawFormatString(400, 400, GetColor(255, 255, 255), "deta");
			return true;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		float otherPoint1 =
			(other.component_.point[0].x - other.component_.point[1].x)
			*(component_.point[intSet[i][0]].y - other.component_.point[0].y)
			+ (other.component_.point[0].y - other.component_.point[1].y)
			*(other.component_.point[0].x - component_.point[intSet[i][0]].x);
		float otherPoint2 =
			(other.component_.point[0].x - other.component_.point[1].x)
			*(component_.point[intSet[i][1]].y - other.component_.point[0].y)
			+ (other.component_.point[0].y - other.component_.point[1].y)
			*(other.component_.point[0].x - component_.point[intSet[i][1]].x);

		if (otherPoint1*otherPoint2 < 0)
		{
			float otherPoint1 =
				(component_.point[intSet[i][0]].x - component_.point[intSet[i][1]].x)
				*(other.component_.point[0].y - component_.point[intSet[i][0]].y)
				+ (component_.point[intSet[i][0]].y - component_.point[intSet[i][1]].y)
				*(component_.point[intSet[i][0]].x - other.component_.point[0].x);
			float otherPoint2 =
				(component_.point[intSet[i][0]].x - component_.point[intSet[i][1]].x)
				*(other.component_.point[1].y - component_.point[intSet[i][0]].y)
				+ (component_.point[intSet[i][0]].y - component_.point[intSet[i][1]].y)
				*(component_.point[intSet[i][0]].x - other.component_.point[1].x);

			if (otherPoint1*otherPoint2 < 0)
			{

				return true;
			}

		}
	}
	return false;


	//if (component_.point[1].x >= other.component_.point[0].x&&
	//	component_.point[0].x <= other.component_.point[1].x)
	//{
	//	if (component_.point[2].y >= other.component_.point[0].y&&
	//		component_.point[0].y <= other.component_.point[2].y)
	//	{
	//		return true;
	//	}
	//}
	//return false;
}

bool BoundingBox::intersects(BoundingCircle & other)
{
	if (!enabled || !other.enabled)return false;

	int intSet[][2] = { { 0,1 },{ 0,2 },{ 1,3 },{ 2,3 } };
	//端点
	float dx, dy, r;
	//線分
	Vector2 pq, pm;
	float inner, k, pqd2, pmd2, phd2, d2;
	//内側
	Vector2 pp, inpm;
	float ininner, outer, theta[2];

	Vector2 AB = CreateVector(other.previousPosition_, other.position_);
	Vector2 AC, AD, CD, CA, CB;
	
	//すり抜けたか
	for (int i = 0; i < 4; i++)
	{
		AC = CreateVector(other.previousPosition_, component_.point[intSet[i][0]]);
		AD = CreateVector(other.previousPosition_, component_.point[intSet[i][1]]);
		CD = CreateVector(component_.point[intSet[i][0]], component_.point[intSet[i][1]]);
		CA = CreateVector(component_.point[intSet[i][0]], other.previousPosition_);
		CB = CreateVector(component_.point[intSet[i][0]], other.position_);

		if (OuterProduct(AB, AC)*OuterProduct(AB, AD) <= 0.0f&&
			OuterProduct(CD, CA)*OuterProduct(CD, CB) < 0.0f)
		{
			//DrawFormatString(400, 400, GetColor(255, 255, 255), "deta");
			return true;
		}
	}


	//端点と触れているかどうか
	for (int i = 0; i < 4; i++)
	{
		dx = component_.point[i].x - other.component_.point[0].x;
		dy = component_.point[i].y - other.component_.point[0].y;
		r = other.component_.radius;
		if ((dx*dx) + (dy*dy) < (r*r))
		{
			return true;
		}
	}

	//線分に触れているかどうか
	for (int i = 0; i < 4; i++)
	{
		pq = CreateVector(component_.point[intSet[i][0]], component_.point[intSet[i][1]]);
		pm = CreateVector(component_.point[intSet[i][0]], other.component_.point[0]);

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
	//四角の内側かどうか
	{
		for (int i = 0; i < 2; i++)
		{
			pp = CreateVector(component_.point[i * 3], component_.point[1 + i]);
			inpm = CreateVector(component_.point[i * 3], other.component_.point[0]);

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

//bool BoundingBox::isIntersectOtherRayToThisLine(BoundingBox & other, int point1, int point2)
//{
//	float otherPoint1 =
//		(component_.point[0].x - component_.point[1].x)
//		*(other.component_.point[point1].y - component_.point[0].y)
//		+ (component_.point[0].y - component_.point[1].y)
//		*(component_.point[0].x - other.component_.point[point1].x);
//	float otherPoint2 =
//		(component_.point[0].x - component_.point[1].x)
//		*(other.component_.point[point2].y - component_.point[0].y)
//		+ (component_.point[0].y - component_.point[1].y)
//		*(component_.point[0].x - other.component_.point[point2].x);
//
//	if (otherPoint1*otherPoint2 < 0)
//	{
//		return true;
//	}
//	return false;
//}
bool BoundingBox::isIntersectOtherRayToThisLine(BoundingCapsule & other,int point1,int point2)
{
	float otherPoint1 =
		(component_.point[point1].x - component_.point[point2].x)
		*(other.component_.point[0].y - component_.point[point1].y)
		+ (component_.point[point1].y - component_.point[point2].y)
		*(component_.point[point1].x - other.component_.point[0].x);
	float otherPoint2 =
		(component_.point[point1].x - component_.point[point2].x)
		*(other.component_.point[1].y - component_.point[point1].y)
		+ (component_.point[point1].y - component_.point[point2].y)
		*(component_.point[point1].x - other.component_.point[1].x);

	if (otherPoint1*otherPoint2 < 0)
	{

		return true;
	}
	return false;
}

bool BoundingBox::isIntersectThisRayToOtherLine(BoundingCapsule & other, int point1, int point2)
{
	float otherPoint1 =
		(other.component_.point[0].x - other.component_.point[1].x)
		*(component_.point[point1].y - other.component_.point[0].y)
		+ (other.component_.point[0].y - other.component_.point[1].y)
		*(other.component_.point[0].x - component_.point[point1].x);
	float otherPoint2 =
		(other.component_.point[0].x - other.component_.point[1].x)
		*(component_.point[point2].y - other.component_.point[0].y)
		+ (other.component_.point[0].y - other.component_.point[1].y)
		*(other.component_.point[0].x - component_.point[point2].x);

	if (otherPoint1*otherPoint2 < 0)
	{

		return true;
	}
	return false;
}

void BoundingBox::update(const Vector2 & center)
{
}

void BoundingBox::debug() const
{
}
