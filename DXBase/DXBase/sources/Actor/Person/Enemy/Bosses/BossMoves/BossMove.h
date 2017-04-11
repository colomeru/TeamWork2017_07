#ifndef BOSS_MOVE__H_
#define BOSS_MOVE__H_

#include "../../../../../Math/Math.h"

class BossMove {
public:
	BossMove();
	BossMove(const Vector2& position);
	virtual void update(float deltaTime);

public:
	Vector2 getPosition();

protected:
	Vector2 position_;
};

#endif