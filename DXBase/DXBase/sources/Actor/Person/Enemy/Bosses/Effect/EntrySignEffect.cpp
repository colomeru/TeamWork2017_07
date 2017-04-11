#include "EntrySignEffect.h"

EntrySignEffect::EntrySignEffect(
	IWorld * world, 
	const Vector2 & position) :
	BossEffect(
		world, "EntrySignEffect", position, Vector2(320, 321),
		EFFECT_ENTRY_SIGN, AnimationID::BOSS_EFFECT_ENTRY_SIGN_TEX,
		true)
{
	scale_ = 0.8f;
}
