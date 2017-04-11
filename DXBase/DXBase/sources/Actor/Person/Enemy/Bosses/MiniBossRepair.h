#pragma once
#include "../../../Base/Actor.h"
#include "../../../../Animation/Enemy/EnemyAnimation2D.h"
#include "../../../../Actor/Base/ActorPtr.h"
#include <vector>

class MiniBossRepair : public Actor {
public:
	MiniBossRepair(IWorld * world, const Vector2& position,int num);
	~MiniBossRepair();
	virtual void onUpdate(float deltaTime) override;
	virtual void onDraw() const override;
	virtual void onCollide(Actor& actor) override;
	//直すかどうか
	void Repair(bool flag);
private:
	Animation2D mAnim;
	std::vector<Vector2>mPoss;
	//最初の場所
	Vector2 mFirstPos;
	//ブロックを置く場所
	Vector2 mBlockToPos;
	//ブロックの座標
	Vector2 mBlockPos;
	//帰る場所
	Vector2 mEndPos;
	//補完関係
	float mLerpCount;
	//直すかどうか
	bool repairFlag;
	//子分の向き
	bool mChildDirection;
	//サイズ
	Vector2 mAnimSize;
	int mNum;
	bool mTes;
};