#ifndef ENEMY_COLLIDE_EFFECT_H_
#define ENEMY_COLLIDE_EFFECT_H_

#include "BossEffect.h"

class EnemyCollideEffect : public BossEffect {
public:
	EnemyCollideEffect(
		IWorld* world,
		const Vector2&  position,
		const float scale = 1.0f);
	void onUpdate(float deltaTime) override;
};

#endif