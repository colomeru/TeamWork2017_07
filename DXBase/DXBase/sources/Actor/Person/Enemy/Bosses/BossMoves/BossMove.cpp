#include "BossMove.h"

BossMove::BossMove()
{
}

BossMove::BossMove(const Vector2 & position) : 
	position_(position)
{
}

void BossMove::update(float deltaTime)
{
}

Vector2 BossMove::getPosition()
{
	return position_;
}
