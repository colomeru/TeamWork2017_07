#pragma once

#include <string>
#include "../../Math/Math.h"
#include "ShapeType.h"
#include"../Collision/Collision.h"
#include "IBodyPtr.h"
#include <vector>

class BoundingBox;
class BoundingCapsule;
class BoundingSegment;
class BoundingCircle;

//enum CollisionType
//{
//	BoxCol,
//	CapsuleCol,
//	CircleCol,
//	SegmentCol,
//	NoneCol
//};

class IBody{
public:
	//IBody() {}
	virtual ~IBody() {}
	virtual bool intersects( IBodyPtr& other)  = 0;
	virtual bool intersects( BoundingBox& other)   = 0;
	virtual bool intersects( BoundingCapsule& other) = 0;
	virtual bool intersects( BoundingSegment& other)  = 0;
	virtual bool intersects( BoundingCircle& other) = 0;
	//virtual void update(float deltaTime) = 0;
	//virtual void enabled(bool enabled) = 0;
	//virtual Vector2 velocity()const = 0;
	//virtual Vector2 cur_pos()const = 0;
	//virtual Vector2 pre_pos()const = 0;
	////virtual Matrix mat()const = 0;
	//virtual float radius()const = 0;
	//virtual float length()const = 0;
	//virtual float height()const = 0;
	//virtual float width()const = 0;
	//virtual std::vector<Vector2> points()const = 0;
	virtual void draw(Matrix inv) const = 0;
	virtual void draw(int spriteID, Matrix inv) const = 0;
	virtual void draw(int spriteID, Matrix inv, int width, int height) const =0;
	virtual void draw(int spriteID, int rotation, Matrix inv) const = 0;
	//virtual void setSegment(const Vector2& start, const Vector2& end) = 0;
	virtual IBodyPtr translate(const Vector2& pos)const = 0;
	//virtual IBodyPtr transform(const Matrix& mat)const = 0;

	void setPosition(Vector2 position) {
		position_ = position;
		previousPosition_ = position;
	}


	Vector2 position_;
	Vector2 previousPosition_;

	bool enabled;

	struct Component {
		// [0]:ç∂è„ [1]:âEè„ [2]:ç∂â∫ [3]:âEâ∫
		Vector2 point[4];
		float radius;
		Component() {
			point[0] = Vector2(0,0);
			point[1] = Vector2(0, 0);
			point[2] = Vector2(0, 0);
			point[3] = Vector2(0, 0);
			radius = 0;
		}
		Component(Vector2 topLeft, Vector2 topRight, Vector2 bottomLeft, Vector2 bottomRight) {
			point[0] = topLeft;
			point[1] = topRight;
			point[2] = bottomLeft;
			point[3] = bottomRight;

			radius = 0;
		}
		Component(Vector2 center, float capsuleRadius) {
			point[0] = center;
			point[1] = center;
			point[2] = center;
			point[3] = center;

			radius = capsuleRadius;
		}

		Component(Vector2 startPoint, Vector2 endPoint) {
			point[0] = startPoint;
			point[1] = endPoint;
			point[2] = startPoint;
			point[3] = endPoint;

			radius = 0;
		}
		Component(Vector2 startPoint, Vector2 endPoint, float capsuleRadius) {
			point[0] = startPoint;
			point[1] = endPoint;
			point[2] = startPoint;
			point[3] = endPoint;

			radius = capsuleRadius;
		}

	};

	Component component_;

};