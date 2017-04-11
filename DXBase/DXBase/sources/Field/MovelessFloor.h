#pragma once
#include"../Actor/Base/Actor.h"
#include"MapChip.h"
#include"../Define.h"

class MovelessFloor :public MapChip {
public:
	//空のMapChipを生成
	MovelessFloor() {}
	//マップチップの生成
	MovelessFloor(IWorld* world, Vector2& position);
	MovelessFloor(int spriteID, IWorld* world, Vector2& position);
	MovelessFloor(int spriteID, IWorld* world, Vector2& position,int width,int height);
	MovelessFloor(std::shared_ptr<MovelessFloor> chip, IWorld* world, Vector2& position);
	MovelessFloor(MovelessFloor& chip, IWorld* world, Vector2& position);
	void set(Vector2& pos);
	MovelessFloor& operator = (std::shared_ptr<MovelessFloor> other) {
		return MovelessFloor(other, world_, Vector2(0, 0));
	};
private:
	void onUpdate(float deltaTime);
	void onDraw() const override;
	void onCollide(Actor& other);
private:
	int spriteID_;
};

