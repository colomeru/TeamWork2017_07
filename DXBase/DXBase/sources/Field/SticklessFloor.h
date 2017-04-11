#pragma once
#include"../Actor/Base/Actor.h"
#include"MapChip.h"
#include"../Define.h"

class SticklessFloor :public MapChip {
public:
	//空のMapChipを生成
	SticklessFloor() {}
	//マップチップの生成
	SticklessFloor(IWorld* world, Vector2& position);
	SticklessFloor(int spriteID, IWorld* world, Vector2& position);
	SticklessFloor(int spriteID, IWorld* world, Vector2& position, int width, int height);
	SticklessFloor(std::shared_ptr<SticklessFloor> chip, IWorld* world, Vector2& position);
	SticklessFloor(SticklessFloor& chip, IWorld* world, Vector2& position);
	void set(Vector2& pos);
	SticklessFloor& operator = (std::shared_ptr<SticklessFloor> other) {
		return SticklessFloor(other, world_, Vector2(0, 0));
	};
private:
	void onUpdate(float deltaTime);
	void onDraw() const override;
	void onCollide(Actor& other);
private:
	int spriteID_;
};

