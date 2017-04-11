#pragma once

#include "../../Base/Actor.h"
#include "../../../World/IWorld.h"
#include <vector>
//ぷよテクスチャのあたり判定があるところ
class PuyoCollision : public Actor {
public:
	PuyoCollision(IWorld* world, Vector2& position,Vector2 arrayState,Vector2 &center);
	~PuyoCollision();
	virtual void onUpdate(float deltaTime) override;
	virtual void onLateUpdate(float deltaTime) override;
	virtual void onDraw() const override;
	virtual void onCollide(Actor& other) override;
public:
	//逆行列かけていないポジションを
	void SetPos(const Vector2& pos,const Vector2& center1,const Vector2& center2) {
		position_ = pos*Matrix::Invert(inv_);
		mCenter1 = center1*Matrix::Invert(inv_);
		mCenter2 = center2*Matrix::Invert(inv_);
	}
	//どの2次元配列にいるか取得
	Vector2 GetArrayState() {
		return mArrayState;
	}
	//ポジションセット
	void VecZero() {
		mVec = Vector2::Zero;
	}
	Vector2 GetVec() {
		return mVec;
	}
private:
	//線と線のあたり判定と交点を求める
	bool SegmentCol(const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& D,Vector2 & couten,bool rightLeft);
private:
	//本来の位置と交点のベクトル
	Vector2 mVec;
	//最終的な交点
	Vector2 mIntersection;
	//2次元配列のどこか
	Vector2 mArrayState;
	//ぶってぃとれってぃ
	Vector2 mCenter1;
	Vector2 mCenter2;
	//センター
	Vector2 mCenter;
	//バネ補間関係
	Vector2 mResIntersection;
	Vector2 mVelo;
	//ステージと当たっているか
	bool mIsCol;
	//交点たち
	Vector2 mIntersections[4];
	//最終的な交点たち
	std::vector<Vector2> mIntersectionsVector;
	//プレイヤーの座標
	Vector2 mPlayerPos1;
	Vector2 mPlayerPos2;
	//出現した瞬間はあたり判定無効
	float mNoColTimer;
	bool top;
	bool right;
	bool bot;
	bool left;


	bool testFlag;
};

