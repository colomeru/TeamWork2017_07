#include "AttackEffect.h"

AttackEffect::AttackEffect(
	IWorld * world,
	const Vector2 & position,
	const float scale) :
	BossEffect(world, "AttackEffect", position, Vector2(256, 256),
		EFFECT_ATTACK, AnimationID::BOSS_EFFECT_ATTACK_TEX)
{
	//scale_ = scale;
}

