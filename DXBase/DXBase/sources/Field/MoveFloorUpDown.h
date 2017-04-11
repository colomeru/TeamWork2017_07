#pragma once
#include"../Actor/Base/Actor.h"
#include"../Define.h"
#include"MapChip.h"


class MoveFloorUpDown :public MapChip {
public:
	//空のMapChipを生成
	MoveFloorUpDown() {}
	//マップチップの生成
	MoveFloorUpDown(IWorld* world, Vector2& position);
	MoveFloorUpDown(int spriteID,IWorld* world, Vector2& position);
	MoveFloorUpDown(std::shared_ptr<MoveFloorUpDown> chip, IWorld* world, Vector2& position);
	MoveFloorUpDown(MoveFloorUpDown& chip, IWorld* world, Vector2& position);
	virtual void set(Vector2& pos);
	MoveFloorUpDown& operator = (std::shared_ptr<MoveFloorUpDown> other) {
		return MoveFloorUpDown(other, world_, Vector2(0, 0));
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



