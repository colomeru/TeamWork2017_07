#ifndef BOUNDING_SEGMENT_H_
#define BOUNDING_SEGMENT_H_

#include "../../Math/Math.h"
#include "Body.h"


class BoundingSegment : public Body {
public:
	//四角の判定を作成する(判定を行う場合) topLeft:左上の点(Vector2) topRight:右上の点(Vector2) bottomLeft:左下の点(Vector2) bottomRight:右下の点(Vector2)
	BoundingSegment(const Vector2& startPoint, const Vector2& endPoint);
	//四角の判定を作成する(判定を行うかどうかを指定する場合) (trueなら行い、falseなら行わない) topLeft:左上の点(Vector2) topRight:右上の点(Vector2) bottomLeft:左下の点(Vector2) bottomRight:右下の点(Vector2) isEnabled:判定をするかどうか(bool)
	BoundingSegment(const Vector2& startPoint, const Vector2& endPoint, bool isEnabled);
	//四角の判定を作成する(判定を行わない場合)
	explicit BoundingSegment();
	float getLength() {
		Vector2 lengthVect = CreateVector(component_.point[0], component_.point[1]);
		return lengthVect.Length();
	}
	virtual bool intersects( IBodyPtr & other)  override {
		return other->intersects(*this); 
	}
	//使用しない
	//virtual bool intersects(BoundingSphere& other) override { return false; }
	////使用しない
	//virtual bool intersects(Capsule& other) override { return false; }
	////使用しない
	//virtual bool intersects(Model& other) override { return false; }
	// 自身(Box)と相手(Box)の判定 other:判定したい相手(BoundingBox) return:判定結果(bool)
	virtual bool intersects(BoundingBox & other) override;
	// 自身(Box)と相手(Capsule)の判定 other:判定したい相手(BoundingCapsule) return:判定結果(bool)
	virtual bool intersects(BoundingCapsule & other) override;
	// 自身(Box)と相手(Segment)の判定 other:判定したい相手(BoundingSegment) return:判定結果(bool)
	virtual bool intersects(BoundingSegment & other) override;
	// 自身(Box)と相手(Circle)の判定 other:判定したい相手(BoundingCircle) return:判定結果(bool)
	virtual bool intersects(BoundingCircle & other) override;
	//自身の判定の更新を行う(現在未使用)
	virtual void update(const Vector2 & center) override;
	//使用しない
	virtual void update(const Vector3& center) override {}
	//使用しない
	virtual void debug() const override;
	//判定の表示(デバッグ用)
	virtual void draw() const override;
	virtual void draw(Matrix inv) const override;
	virtual void draw(int spriteID,Matrix inv) const override;
	virtual void draw(int spriteID, int rotation, Matrix inv)const override {
		draw(spriteID,inv);
	}

	//自身(Box)を移動する position:移動量(Vector2) return:移動した結果(BoundingBox)
	virtual IBodyPtr translate(const Vector2& position) const;
	//自身(Box)を変形する topLeft:左上の点(Vector2) topRight:右上の点(Vector2) bottomLeft:左下の点(Vector2) bottomRight:右下の点(Vector2) return:変形した結果(BoundingBox)
	BoundingSegment transform(const Vector2& startPoint, const Vector2& endPoint) const;
private:
	//ベクトルの作成
	Vector2 CreateVector(const Vector2& p, const Vector2& q) {
		return Vector2(q.x - p.x, q.y - p.y);
	}
	//Dotの計算
	float InnerProduct(const Vector2 &a, const Vector2 &b) {
		return (a.x*b.x + a.y*b.y);//a・b
	}
	//Crossの計算
	float OuterProduct(const Vector2 &a, const Vector2 &b) {
		return (a.x*b.y - a.y*b.x);//a×b
	}
	//長さの2乗の計算
	float VectorLength2(const Vector2 &v) {
		return InnerProduct(v, v);//v・v=|v|^2
	}
	//自身(Box)の直線と相手(Capsule)の線分が当たっているかを調べる、pointはComponentの物を利用する other:相手の判定(BoundingCapsule) point1:自身のどの点を始点とするか(int) point2:自身のどの点を終点とするか(int)
	bool isIntersectOtherRayToThisLine(BoundingCapsule & other, int point1, int point2);
	//自身(Box)の線分と相手(Capsule)の直線が当たっているかを調べる、pointはComponentの物を利用する other:相手の判定(BoundingCapsule) point1:自身のどの点を始点とするか(int) point2:自身のどの点を終点とするか(int)
	bool isIntersectThisRayToOtherLine(BoundingCapsule & other, int point1, int point2);



public:
	//線分を生成するための2点を保存する

	//intersectsを使うかどうか true:判定する false:判定しない


};

#endif