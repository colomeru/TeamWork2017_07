#ifndef ATTACK_EFFECT_H_
#define ATTACK_EFFECT_H_

#include "BossEffect.h"

class AttackEffect : public BossEffect {
public:
	AttackEffect(
		IWorld* world,
		const Vector2&  position,
		const float scale = 1.0f);
};

#endif
