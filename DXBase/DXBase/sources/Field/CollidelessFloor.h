#ifndef COLLIDELESS_FLOOR_H_
#define COLLIDELESS_FLOOR_H_
#include"../Actor/Base/Actor.h"
#include"MapChip.h"
#include"../Define.h"

class CollidelessFloor :public MapChip {
public:
	//空のMapChipを生成
	CollidelessFloor() {}
	//マップチップの生成
	CollidelessFloor(IWorld* world, Vector2& position);
	CollidelessFloor(int spriteID, IWorld* world, Vector2& position);
	CollidelessFloor(std::shared_ptr<CollidelessFloor> chip, IWorld* world, Vector2& position);
	CollidelessFloor(CollidelessFloor& chip, IWorld* world, Vector2& position);
	void set(Vector2& pos);
	CollidelessFloor& operator = (std::shared_ptr<CollidelessFloor> other) {
		return CollidelessFloor(other, world_, Vector2(0, 0));
	};
private:
	void onUpdate(float deltaTime);
	void onDraw() const override;
	void onCollide(Actor& other);
private:
	int spriteID_;
};

#endif // !MAPCHIP_H_