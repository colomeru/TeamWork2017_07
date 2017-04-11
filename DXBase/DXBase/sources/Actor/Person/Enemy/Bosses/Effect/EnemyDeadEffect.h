#ifndef ENEMY_DEAD_EFFECT_H_
#define ENEMY_DEAD_EFFECT_H_

#include "BossEffect.h"

class EnemyDeadEffect : public BossEffect {
public:
	EnemyDeadEffect(
		IWorld* world,
		const Vector2&  position,
		const int animation);
};

#endif