#ifndef BOUNDING_CIRCLE_H_
#define BOUNDING_CIRCLE_H_


#include "../../Math/Math.h"
#include "Body.h"

class BoundingCircle : public Body {
public:
	//カプセルの判定を作成する(判定を行う場合) topLeft:左上の点(Vector2) topRight:右上の点(Vector2) bottomLeft:左下の点(Vector2) bottomRight:右下の点(Vector2)
	BoundingCircle(Vector2 center,float circleRadius);
	//カプセルの判定を作成する(判定を行う場合) topLeft:左上の点(Vector2) topRight:右上の点(Vector2) bottomLeft:左下の点(Vector2) bottomRight:右下の点(Vector2) isEnabled:判定をするかどうか(bool)
	BoundingCircle(Vector2 center,float circleRadius, bool isEnabled);
	//カプセルの判定を作成する(判定を行わない場合)
	BoundingCircle(float radius = -1);
	float getRadius() { return component_.radius; }
	virtual bool intersects( IBodyPtr & other)  override {
		return other->intersects(*this); 
	}
	//使用しない
	//virtual bool intersects(BoundingSphere& other) { return false; }
	////使用しない
	//virtual bool intersects(Capsule& other) { return false; }
	////使用しない
	//virtual bool intersects(Model& other) { return false; }
	// 自身(Circle)と相手(Box)の判定 other:判定したい相手(BoundingBox) return:判定結果(bool)
	virtual bool intersects(BoundingBox & other) override;
	// 自身(Circle)と相手(Capsule)の判定 other:判定したい相手(BoundingCapsule) return:判定結果(bool)
	virtual bool intersects(BoundingCapsule & other) override;
	// 自身(Circle)と相手(Segment)の判定 other:判定したい相手(BoundingSegment) return:判定結果(bool)
	virtual bool intersects(BoundingSegment & other) override;
	// 自身(Circle)と相手(Circle)の判定 other:判定したい相手(BoundingCircle) return:判定結果(bool)
	virtual bool intersects(BoundingCircle & other) override;
	//使用しない
	virtual void update(const Vector2 & center) override;
	//使用しない
	virtual void update(const Vector3& center) {}
	//使用しない
	virtual void debug() const override;
	//自身(Circle)を移動する position:移動量(Vector2) return:移動した結果(BoundingCircle)
	virtual IBodyPtr translate(const Vector2& position) const;
	//自身(Box)を変形する center:中心点(Vector2) circleRadius:半径(float) return:変形した結果(BoundingCircle)
	BoundingCircle transform(Vector2 center, float circleRadius) const;
	//判定の表示(デバッグ用)
	virtual void draw() const override;
	virtual void draw(Matrix inv) const override;
	virtual void draw(int spriteID, Matrix inv)const override {
		draw(inv);
	}
	virtual void draw(int spriteID, int rotation, Matrix inv)const override {
		draw(inv);
	}

private:
	Vector2 CreateVector(const Vector2& p, const Vector2& q){
		return q - p;// Vector2(q.x - p.x, q.y - p.y);
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
	bool isIntersectOtherRayToThisLineSegment(BoundingSegment & other);
	bool isIntersectThisRayToOtherLineSegment(BoundingSegment & other);
	bool isIntersectOtherRayToThisLineCapsule(BoundingCapsule & other);
	bool isIntersectThisRayToOtherLineCapsule(BoundingCapsule & other);
	bool isIntersectOtherRayToThisLineBox(BoundingBox & other, int point1, int point2);
	bool isIntersectThisRayToOtherLineBox(BoundingBox & other, int point1, int point2);
public:

	bool enabled;

};

#endif