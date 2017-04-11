#pragma once

#include <string>
#include "../../Math/Math.h"
#include "ShapeType.h"
#include<memory>
#include"IBody.h"

class BoundingSphere;
class Capsule;
class Model;
class BoundingBox;
class BoundingCapsule;
class BoundingSegment;
class BoundingCircle;
class BezierCurve;
//class Body;
//using BodyPtr = std::shared_ptr<Body>;


struct BodyData {
	std::string name;
	Vector2 position;
	ShapeType type;
};

class Body:public IBody
{
public:
	//Body() { /*bodyenub_ = true;*/ }
	virtual ~Body() {}
	//virtual bool intersects(Body& other) {
	//	if (bodyenub_)return false;
	//	return other.intersects(*this); }
	//virtual bool intersects(BoundingSphere& other) { return false; }
	//virtual bool intersects(Capsule& other) { return false; }
	//virtual bool intersects(Model& other) { return false; }
	//virtual bool intersects(BoundingBox& other) { return false; }
	//virtual bool intersects(BoundingCapsule& other) { return false; }
	//virtual bool intersects(BoundingSegment& other) { return false; }
	//virtual bool intersects(BoundingCircle& other) { return false; }
	//virtual bool intersects(BezierCurve& other) { return false; }
	virtual void update(const Vector3& center) {}
	virtual void update(const Vector2& center) {}
	//virtual void update(Vector2 position) {}
	virtual void draw() const {}
	virtual void draw(Matrix inv) const {}
	virtual void draw(int spriteID, Matrix inv) const {}
	virtual void draw(int spriteID, Matrix inv, int width, int height) const override{};
	virtual void draw(int spriteID,int rotation,Matrix inv) const {}
	virtual void debug() const {}
	//virtual IBodyPtr translate(const Vector2& pos)const { return std::make_shared<Body>(); }

	//virtual IBodyPtr Getthis() { return *this; }
public:
	bool bodyenub_;

};

