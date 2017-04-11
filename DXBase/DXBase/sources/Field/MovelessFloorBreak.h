#pragma once
#include"../Actor/Base/Actor.h"
#include"MapChip.h"
#include"../Define.h"

class MovelessFloorBreak :public MapChip {
public:
	//空のMapChipを生成
	MovelessFloorBreak() {}
	//マップチップの生成
	MovelessFloorBreak(IWorld* world, Vector2& position);
	MovelessFloorBreak(int spriteID, IWorld* world, Vector2& position);
	MovelessFloorBreak(int spriteID, IWorld* world, Vector2& position, int width, int height);
	MovelessFloorBreak(std::shared_ptr<MovelessFloorBreak> chip, IWorld* world, Vector2& position);
	MovelessFloorBreak(MovelessFloorBreak& chip, IWorld* world, Vector2& position);
	void set(Vector2& pos);
	MovelessFloorBreak& operator = (std::shared_ptr<MovelessFloorBreak> other) {
		return MovelessFloorBreak(other, world_, Vector2(0, 0));
	};
private:
	void onUpdate(float deltaTime);
	void onDraw() const override;
	void onCollide(Actor& other);
private:
	int spriteID_;
	int width_;
	int height_;
};