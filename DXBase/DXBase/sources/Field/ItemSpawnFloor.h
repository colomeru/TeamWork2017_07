#pragma once
#include"../Actor/Base/Actor.h"
#include"MapChip.h"
#include"../Define.h"

class ItemSpawnFloor :public MapChip {
public:
	//空のMapChipを生成
	ItemSpawnFloor() {}
	//マップチップの生成
	ItemSpawnFloor(IWorld* world, Vector2& position);
	ItemSpawnFloor(int spriteID, IWorld* world, Vector2& position);
	ItemSpawnFloor(int spriteID, IWorld* world, Vector2& position, int width, int height);
	ItemSpawnFloor(std::shared_ptr<ItemSpawnFloor> chip, IWorld* world, Vector2& position);
	ItemSpawnFloor(ItemSpawnFloor& chip, IWorld* world, Vector2& position);
	void set(Vector2& pos);
	ItemSpawnFloor& operator = (std::shared_ptr<ItemSpawnFloor> other) {
		return ItemSpawnFloor(other, world_, Vector2(0, 0));
	};
private:
	void onUpdate(float deltaTime);
	void onDraw() const override;
	void onCollide(Actor& other);
private:
	int spriteID_;
	int width_;
	int height_;
	bool isHit_;
};

