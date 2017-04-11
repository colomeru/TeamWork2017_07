#ifndef PIYORI_MOVE_H_
#define PIYORI_MOVE_H_

#include "BossMove.h"

class PiyoriMove : public BossMove {
public:
	PiyoriMove(const Vector2& position);
	void update(float deltaTime) override;
};

#endif