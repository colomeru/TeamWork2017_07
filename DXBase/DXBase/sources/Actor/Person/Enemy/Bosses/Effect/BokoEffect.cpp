#include "BokoEffect.h"

BokoEffect::BokoEffect(
	IWorld * world,
	const Vector2 & position,
	const int alpha,
	const float speed) : 
	BossEffect(world, "BokoEffect", position, Vector2(512, 512),
		EFFECT_BOKO, AnimationID::BOSS_EFFECT_BOKO_TEX)
{
	alpha_ = (float)alpha;
	animation_.setSpeed(speed);
}
