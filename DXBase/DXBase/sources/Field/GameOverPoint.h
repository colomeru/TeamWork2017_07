#pragma once
#ifndef GAME_OVER_POINT
#define GAME_OVER_POINT

#include"MapChip.h"

class GameOverPoint :public MapChip {
public:
	//空のMapChipを生成
	GameOverPoint() {}
	//マップチップの生成
	GameOverPoint(IWorld* world, Vector2& position);
	GameOverPoint(int spriteID,IWorld* world, Vector2& position);
	GameOverPoint(std::shared_ptr<GameOverPoint> chip, IWorld* world, Vector2& position);
	GameOverPoint(GameOverPoint& chip, IWorld* world, Vector2& position);
	void set(Vector2& pos);
	GameOverPoint& operator = (std::shared_ptr<GameOverPoint> other) {
		return GameOverPoint(other, world_, Vector2(0, 0));
	};
private:
	void onUpdate(float deltaTime);
	void onDraw() const override;
	void onCollide(Actor& other);

private:
	int spriteID_;
};

#endif // !STAGE_CLEAR_POINT
