#ifndef TRANSLESS_TURN_FLOOR_H_
#define TRANSLESS_TURN_FLOOR_H_
#include"../Actor/Base/Actor.h"
#include"../Define.h"
#include"MapChip.h"

const static float TURNSPEED = 2;

class TranslessTurnFloor :public MapChip {
public:
	//空のMapChipを生成
	TranslessTurnFloor() {}
	//マップチップの生成
	TranslessTurnFloor(IWorld* world, Vector2& position);
	TranslessTurnFloor(int spriteID, IWorld* world, Vector2& position);
	TranslessTurnFloor(std::shared_ptr<TranslessTurnFloor> chip, IWorld* world, Vector2& position);
	TranslessTurnFloor(TranslessTurnFloor& chip, IWorld* world, Vector2& position);
	virtual void set(Vector2& pos);
	TranslessTurnFloor& operator = (std::shared_ptr<TranslessTurnFloor> other) {
		return TranslessTurnFloor(other, world_, Vector2(0, 0));
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


