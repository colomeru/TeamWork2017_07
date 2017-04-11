#include "EnemyDeadEffect.h"

EnemyDeadEffect::EnemyDeadEffect(
	IWorld * world,
	const Vector2 & position,
	const int animation) :
	BossEffect(world, "EnemyDeadEffect", position, Vector2(280, 800),
		animation, AnimationID::ENEMY_EFFECT_DEAD_TEX)
{
	//scale_ = 0.3f;
	animation_.setSpeed(20.0f);
}
