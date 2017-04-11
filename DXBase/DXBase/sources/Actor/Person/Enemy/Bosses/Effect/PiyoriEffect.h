#ifndef PIYORI_EFFECT_H_
#define PIYORI_EFFECT_H_

#include "BossEffect.h"

class PiyoriEffect : public BossEffect {
public:
	PiyoriEffect(
		IWorld* world,
		const Vector2&  position,
		const float scale = 1.0f);
	void onUpdate(float deltaTime) override;
};

#endif
