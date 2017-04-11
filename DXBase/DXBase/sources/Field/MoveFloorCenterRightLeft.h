#pragma once
#include"../Actor/Base/Actor.h"
#include"../Define.h"
#include"MapChip.h"


class MoveFloorCenterRightLeft :public MapChip {
public:
	//空のMapChipを生成
	MoveFloorCenterRightLeft() {}
	//マップチップの生成
	MoveFloorCenterRightLeft(IWorld* world, Vector2& position);
	MoveFloorCenterRightLeft(int spriteID,IWorld* world, Vector2& position);
	MoveFloorCenterRightLeft(std::shared_ptr<MoveFloorCenterRightLeft> chip, IWorld* world, Vector2& position);
	MoveFloorCenterRightLeft(MoveFloorCenterRightLeft& chip, IWorld* world, Vector2& position);
	virtual void set(Vector2& pos);
	MoveFloorCenterRightLeft& operator = (std::shared_ptr<MoveFloorCenterRightLeft> other) {
		return MoveFloorCenterRightLeft(other, world_, Vector2(0, 0));
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


