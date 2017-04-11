#ifndef SCATTER_WING_EFFECT_H_
#define SCATTER_WING_EFFECT_H_

#include "../../../../Base/Actor.h"

class ScatterWingEffect : public Actor {
public:
	ScatterWingEffect(
		IWorld* world, 
		const Vector2& position);
	void onUpdate(float deltaTime) override;
	void onDraw() const override;
	void onCollide(Actor& actor) override;
};

#endif