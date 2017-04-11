#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_


#include "../../Math/Math.h"
#include "Body.h"


class BoundingBox : public Body {
public:
	//四角の判定を作成する(判定を行う場合) topLeft:左上の点(Vector2) topRight:右上の点(Vector2) bottomLeft:左下の点(Vector2) bottomRight:右下の点(Vector2)
	BoundingBox(const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomLeft, const Vector2& bottomRight);
	//四角の判定を作成する(判定を行うかどうかを指定する場合) (trueなら行い、falseなら行わない) topLeft:左上の点(Vector2) topRight:右上の点(Vector2) bottomLeft:左下の点(Vector2) bottomRight:右下の点(Vector2) isEnabled:判定をするかどうか(bool)
	BoundingBox(const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomLeft, const Vector2& bottomRight,bool isEnabled);
	//四角の判定を作成する(判定を行わない場合)
	BoundingBox();

	float getWidth() {
		Vector2 lengthVectWidth1 = Vector2((component_.point[0].x + component_.point[2].x) / 2, (component_.point[0].y + component_.point[2].y) / 2);
		Vector2 lengthVectWidth2 = Vector2((component_.point[1].x + component_.point[3].x) / 2, (component_.point[1].y + component_.point[3].y) / 2);
		Vector2 lengthVectMain = CreateVector(lengthVectWidth1, lengthVectWidth2);
		return lengthVectMain.Length();
	}
	float getHeight() {
		Vector2 lengthVectHeight1 = Vector2((component_.point[0].x+component_.point[1].x)/2, (component_.point[0].y + component_.point[1].y) / 2);
		Vector2 lengthVectHeight2 = Vector2((component_.point[2].x + component_.point[3].x) / 2, (component_.point[2].y + component_.point[3].y) / 2);
		Vector2 lengthVectMain = CreateVector(lengthVectHeight1,lengthVectHeight2);
		return lengthVectMain.Length();
	}
	Vector2 getSize() {
		return Vector2(getWidth(), getHeight());
	}
	virtual bool intersects( IBodyPtr & other)  override { 
		return other->intersects(*this);
		//return intersects(other);
	}//otherがBodyになっている
	//使用しない
	//virtual bool intersects(BoundingSphere& other) override{ return false; }
	////使用しない
	//virtual bool intersects(Capsule& other) override { return false; }
	////使用しない
	//virtual bool intersects(Model& other) override { return false; }
	// 自身(Box)と相手(Box)の判定 other:判定したい相手(BoundingBox) return:判定結果(bool)
	virtual bool intersects(BoundingBox & other) override;
	// 自身(Box)と相手(Capsule)の判定 other:判定したい相手(BoundingCapsule) return:判定結果(bool)
	virtual bool intersects(BoundingCapsule & other) override;
	virtual bool intersects(BoundingSegment & other) override;
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
	virtual void draw(int spriteID, Matrix inv) const override;
	virtual void draw(int spriteID, Matrix inv,int width,int height) const override;
	virtual void draw(int spriteID,int rotation, Matrix inv) const override;
	BoundingBox movePos(Vector2& position) {
		return BoundingBox();
	}
	//自身(Box)を移動する position:移動量(Vector2) return:移動した結果(BoundingBox)
	virtual IBodyPtr translate(const Vector2& position) const;
	//自身(Box)を変形する topLeft:左上の点(Vector2) topRight:右上の点(Vector2) bottomLeft:左下の点(Vector2) bottomRight:右下の点(Vector2) return:変形した結果(BoundingBox)
	BoundingBox transform(const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomLeft, const Vector2& bottomRight) const;
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

	//自身(Box)の直線と相手(Capsule)の線分が当たっているかを調べる、pointはComponentの物を利用する other:相手の判定(BoundingCapsule) point1:自身のどの点を始点とするか(int) point2:自身のどの点を終点とするか(int)
	bool isIntersectOtherRayToThisLine(BoundingCapsule & other, int point1, int point2);
	//自身(Box)の線分と相手(Capsule)の直線が当たっているかを調べる、pointはComponentの物を利用する other:相手の判定(BoundingCapsule) point1:自身のどの点を始点とするか(int) point2:自身のどの点を終点とするか(int)
	bool isIntersectThisRayToOtherLine(BoundingCapsule & other, int point1, int point2);



public:

	//intersectsを使うかどうか true:判定する false:判定しない



};

#endif