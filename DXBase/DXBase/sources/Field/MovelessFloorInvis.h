#pragma once
#include"../Actor/Base/Actor.h"
#include"MapChip.h"
#include"../Define.h"

class MovelessFloorInvis :public MapChip {
public:
	//空のMapChipを生成
	MovelessFloorInvis() {}
	//マップチップの生成
	MovelessFloorInvis(IWorld* world, Vector2& position);
	MovelessFloorInvis(int spriteID, IWorld* world, Vector2& position);
	MovelessFloorInvis(int spriteID, IWorld* world, Vector2& position, int width, int height);
	MovelessFloorInvis(std::shared_ptr<MovelessFloorInvis> chip, IWorld* world, Vector2& position);
	MovelessFloorInvis(MovelessFloorInvis& chip, IWorld* world, Vector2& position);
	void set(Vector2& pos);
	MovelessFloorInvis& operator = (std::shared_ptr<MovelessFloorInvis> other) {
		return MovelessFloorInvis(other, world_, Vector2(0, 0));
	};
private:
	void onUpdate(float deltaTime);
	void onDraw() const override;
	void onCollide(Actor& other);
private:
	int spriteID_;
};

