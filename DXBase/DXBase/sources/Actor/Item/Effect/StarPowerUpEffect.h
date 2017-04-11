#ifndef STAR_POWER_UP_EFFECT_H_
#define STAR_POWER_UP_EFFECT_H_

#include "PowerUpEffect.h"

class PowerUpFontEffect;

class StarPowerUpEffect : public PowerUpEffect {
public:
	StarPowerUpEffect(
		IWorld* world, const Vector2& position);
	void onUpdate(float deltaTime) override;

private:
	bool isReverse_;
	PowerUpFontEffect* fontEffect_;	// フォントエフェクト
};

#endif