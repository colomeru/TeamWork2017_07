#include "PiyoriEffect.h"

PiyoriEffect::PiyoriEffect(
	IWorld * world,
	const Vector2 & position,
	const float scale):
	BossEffect(world, "PiyoriEffect", position, Vector2(256, 256),
		EFFECT_PIYO, AnimationID::BOSS_EFFECT_PIYO_TEX)
{
	scale_ = scale;
}

void PiyoriEffect::onUpdate(float deltaTime){
	animation_.update(deltaTime);
}
