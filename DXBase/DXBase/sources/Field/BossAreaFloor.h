#pragma once
#include"../Actor/Base/Actor.h"
#include"MapChip.h"
#include"../Define.h"

class BossAreaFloor :public MapChip {
public:
	//空のMapChipを生成
	BossAreaFloor() {}
	//マップチップの生成
	BossAreaFloor(IWorld* world, Vector2& position);
	BossAreaFloor(int spriteID, IWorld* world, Vector2& position);
	BossAreaFloor(int spriteID, IWorld* world, Vector2& position, int width, int height);
	BossAreaFloor(std::shared_ptr<BossAreaFloor> chip, IWorld* world, Vector2& position);
	BossAreaFloor(BossAreaFloor& chip, IWorld* world, Vector2& position);
	void set(Vector2& pos);
	BossAreaFloor& operator = (std::shared_ptr<BossAreaFloor> other) {
		return BossAreaFloor(other, world_, Vector2(0, 0));
	};
private:
	void onUpdate(float deltaTime);
	void onDraw() const override;
	void onCollide(Actor& other);
private:
	int spriteID_;
};
