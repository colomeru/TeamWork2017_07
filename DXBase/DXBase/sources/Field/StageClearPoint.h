#pragma once
#ifndef STAGE_CLEAR_POINT
#define STAGE_CLEAR_POINT

#include"MapChip.h"

class StageClearPoint:public MapChip {
public:
	//空のMapChipを生成
	StageClearPoint() {}
	//マップチップの生成
	StageClearPoint(IWorld* world, Vector2& position);
	StageClearPoint(int spriteID,IWorld* world, Vector2& position);
	StageClearPoint(std::shared_ptr<StageClearPoint> chip, IWorld* world, Vector2& position);
	StageClearPoint(StageClearPoint& chip, IWorld* world, Vector2& position);
	void set(Vector2& pos);
	StageClearPoint& operator = (std::shared_ptr<StageClearPoint> other) {
		return StageClearPoint(other, world_, Vector2(0, 0));
	};
private:
	void onUpdate(float deltaTime);
	void onDraw() const override;
	void onCollide(Actor& other);

private:
	int spriteID_;
};

#endif // !STAGE_CLEAR_POINT
