#pragma once
#include"../Actor/Base/Actor.h"
#include"MapChip.h"
#include"../Define.h"

class SegmentFloor :public MapChip {
public:
	//空のMapChipを生成
	SegmentFloor() {}
	//マップチップの生成
	SegmentFloor(IWorld* world, Vector2 & startPosition, Vector2& endPosition);
	SegmentFloor(int spriteID, IWorld* world, Vector2 & startPosition, Vector2& endPosition);
	SegmentFloor(std::shared_ptr<SegmentFloor> chip, IWorld* world, Vector2 & startPosition, Vector2& endPosition);
	SegmentFloor(SegmentFloor& chip, IWorld* world, Vector2 & startPosition, Vector2& endPosition);
	void set(Vector2& pos);
	SegmentFloor& operator = (std::shared_ptr<SegmentFloor> other) {
		return SegmentFloor(other, world_, Vector2(0, 0),Vector2(0,0));
	};
private:
	void onUpdate(float deltaTime);
	void onDraw() const override;
	void onCollide(Actor& other);
private:
	int spriteID_;
};

