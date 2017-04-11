#ifndef BOUNDING_CAPSULE_H_
#define BOUNDING_CAPSULE_H_

#include "../../Math/Math.h"
#include "Body.h"

class BoundingCapsule : public Body {
public:
	//カプセルの判定を作成する(判定を行う場合) topLeft:左上の点(Vector2) topRight:右上の点(Vector2) bottomLeft:左下の点(Vector2) bottomRight:右下の点(Vector2)
	BoundingCapsule(Vector2 startPoint, Vector2 endPoint, float capsuleRadius);
	//カプセルの判定を作成する(判定を行う場合) topLeft:左上の点(Vector2) topRight:右上の点(Vector2) bottomLeft:左下の点(Vector2) bottomRight:右下の点(Vector2) isEnabled:判定をするかどうか(bool)
	BoundingCapsule(Vector2 startPoint, Vector2 endPoint, float capsuleRadius,bool isEnabled);
	//カプセルの判定を作成する(判定を行わない場合)
	explicit BoundingCapsule(float radius=-1);
	float getRadius() { return component_.radius; }
	float getLength(){
		Vector2 lengthVect = CreateVector(component_.point[0], component_.point[1]);
		return lengthVect.Length();
	}
	virtual bool intersects( IBodyPtr & other)  override
	{
		return other->intersects(*this); 
	}

	//virtual bool intersects(BoundingSphere& other) { return false; }
	//virtual bool intersects(Capsule& other) { return false; }
	//virtual bool intersects(Model& other) { return false; }

	// Body を介して継承されました
	virtual bool intersects(BoundingBox & other) override;

	virtual bool intersects(BoundingCapsule & other) override;

	virtual bool intersects(BoundingSegment & other) override;

	virtual bool intersects(BoundingCircle & other) override;

	virtual void update(const Vector2 & center) override;
	virtual void update(const Vector3& center) {}

	virtual void debug() const override;

	virtual IBodyPtr translate(const Vector2& position) const;

	BoundingCapsule transform(Vector2 startPoint, Vector2 endPoint, float capsuleRadius) const;

	virtual void draw() const override;
	virtual void draw(Matrix inv) const override;
	virtual void draw(int spriteID, Matrix inv)const override {
		draw(inv);
	}
	virtual void draw(int spriteID,int rotation, Matrix inv)const override {
		draw(inv);
	}
private:
	Vector2 CreateVector(const Vector2& p, const Vector2& q) {
		return Vector2(q.x - p.x, q.y - p.y);
	}
	float InnerProduct(const Vector2 &a, const Vector2 &b) {
		return (a.x*b.x + a.y*b.y);//a・b
	}
	float OuterProduct(const Vector2 &a, const Vector2 &b) {
		return (a.x*b.y - a.y*b.x);//a×b
	}
	float VectorLength2(const Vector2 &v) {
		return InnerProduct(v, v);//v・v=|v|^2
	}
	bool isIntersectOtherRayToThisLine(Vector2 a1,Vector2 a2,Vector2 a3,Vector2 a4)
	{
		Vector2 bc = CreateVector(a1, a2);
		Vector2 bp = CreateVector(a1, a3);
		Vector2 bq = CreateVector(a1, a4);
		Vector2 pb = CreateVector(a3, a1);
		Vector2 pq = CreateVector(a3, a4);
		Vector2 pc = CreateVector(a3, a2);
		float otherPoint1 =
			(bp.x*bc.y - bp.y*bc.x)*(bq.x*bc.y - bq.y*bc.x);
			//(a1.x - a2.x)
			//*(a3.y - a1.y)
			//+ (a1.y - a2.y)
			//*(a1.x - a3.x);
		float otherPoint2 =
			(pb.x*pq.y - pb.y*pq.x)*(pc.x*pq.y - pc.y*pq.x);
		//(a1.x - a2.x)
			//*(a4.y - a1.y)
			//+ (a1.y - a2.y)
			//*(a1.x - a4.x);

		if (otherPoint1<= 0&&otherPoint2<=0)
		{
			return true;
		}
		return false;
	}

	bool isIntersectOtherRayToThisLineSegment(BoundingSegment & other);
	bool isIntersectThisRayToOtherLineSegment(BoundingSegment & other);
	bool isIntersectOtherRayToThisLineCapsule(BoundingCapsule & other);
	bool isIntersectThisRayToOtherLineCapsule(BoundingCapsule & other);
	bool isIntersectOtherRayToThisLineBox(BoundingBox & other,int point1,int point2);
	bool isIntersectThisRayToOtherLineBox(BoundingBox & other,int point1,int point2);
public:


};

#endif