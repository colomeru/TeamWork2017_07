#include "EnemyCollideEffect.h"
#include "../../../../../World/IWorld.h"

EnemyCollideEffect::EnemyCollideEffect(
	IWorld * world,
	const Vector2 & position,
	const float scale) : 
	BossEffect(world, "EnemyCollideEffect", position, Vector2::One * 256,
		EFFECT_ENEMY_COLLIDE, AnimationID::ENEMY_EFFECT_COLLIDE_TEX)
{
	scale_ = 1.5f;
}

void EnemyCollideEffect::onUpdate(float deltaTime)
{
	if (deltaTime == 0.0f && world_->isStopTime())
		deltaTime = world_->getDeltaTime();
	BossEffect::onUpdate(deltaTime);
}
