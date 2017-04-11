#pragma once
#ifndef MOVE_FLOOR_RIGHT_LEFT_H_
#define MOVE_FLOOR_RIGHT_LEFT_H_
#include"../Actor/Base/Actor.h"
#include"../Define.h"
#include"MapChip.h"



class MoveFloorRightLeft :public MapChip {
public:
	//空のMapChipを生成
	MoveFloorRightLeft() {}
	//マップチップの生成
	MoveFloorRightLeft(IWorld* world, Vector2& position);
	MoveFloorRightLeft(int spriteID,IWorld* world, Vector2& position);
	MoveFloorRightLeft(std::shared_ptr<MoveFloorRightLeft> chip, IWorld* world, Vector2& position);
	MoveFloorRightLeft(MoveFloorRightLeft& chip, IWorld* world, Vector2& position);
	virtual void set(Vector2& pos);
	MoveFloorRightLeft& operator = (std::shared_ptr<MoveFloorRightLeft> other) {
		return MoveFloorRightLeft(other, world_, Vector2(0, 0));
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

#endif // !MAPCHIP_H_


