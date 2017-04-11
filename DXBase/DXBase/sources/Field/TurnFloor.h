#ifndef TURN_FLOOR_H_
#define TURN_FLOOR_H_
#include"../Actor/Base/Actor.h"
#include"../Define.h"
#include"MapChip.h"

const static float TURNRANGE = 2;

class TurnFloor :public MapChip {
public:
	//空のMapChipを生成
	TurnFloor() {}
	//マップチップの生成
	TurnFloor(IWorld* world, Vector2& position);
	TurnFloor(int spriteID,IWorld* world, Vector2& position);
	TurnFloor(std::shared_ptr<TurnFloor> chip, IWorld* world, Vector2& position);
	TurnFloor(TurnFloor& chip, IWorld* world, Vector2& position);
	virtual void set(Vector2& pos);
	TurnFloor& operator = (std::shared_ptr<TurnFloor> other) {
		return TurnFloor(other, world_, Vector2(0, 0));
	};

private:
	virtual void onUpdate(float deltaTime)override;
	virtual void onDraw() const override;
	virtual void onCollide(Actor& other)override;

private:
	Vector2 defaultPos_;
	int moveCount_;
	int spriteID_;
	Vector2 moveVelocity;
};

#endif // !MAPCHIP_H_


