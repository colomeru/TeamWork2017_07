#ifndef ENTRY_SIGN_EFFECT_H_
#define ENTRY_SIGN_EFFECT_H_

#include "BossEffect.h"

class EntrySignEffect : public BossEffect {
public:
	EntrySignEffect(
		IWorld* world,
		const Vector2&  position);
};

#endif
