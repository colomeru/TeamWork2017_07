#pragma once
#include"../Actor/Base/Actor.h"
#include"../Define.h"
#include"MapChip.h"


class MoveFloorCenterUpDown :public MapChip {
public:
	//空のMapChipを生成
	MoveFloorCenterUpDown() {}
	//マップチップの生成
	MoveFloorCenterUpDown(IWorld* world, Vector2& position);
	MoveFloorCenterUpDown(int spriteID,IWorld* world, Vector2& position);
	MoveFloorCenterUpDown(std::shared_ptr<MoveFloorCenterUpDown> chip, IWorld* world, Vector2& position);
	MoveFloorCenterUpDown(MoveFloorCenterUpDown& chip, IWorld* world, Vector2& position);
	virtual void set(Vector2& pos);
	MoveFloorCenterUpDown& operator = (std::shared_ptr<MoveFloorCenterUpDown> other) {
		return MoveFloorCenterUpDown(other, world_, Vector2(0, 0));
	};

private:
	virtual void onUpdate(float deltaTime)override;
	virtual void onDraw() const override;
	virtual void onCollide(Actor& other)override;

private:
	Vector2 defaultPos_;
	float moveCount_;
	float moveVelocity;
	int spriteID_;
};



