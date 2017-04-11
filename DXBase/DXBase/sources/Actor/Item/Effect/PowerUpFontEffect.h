#ifndef POWER_UP_FONT_EFFECT_H_
#define POWER_UP_FONT_EFFECT_H_

#include "PowerUpEffect.h"

class PowerUpFontEffect : public PowerUpEffect {
public:
	PowerUpFontEffect(
		IWorld* world, const Vector2& position);
	void onUpdate(float deltaTime) override;

public:
	// アニメーションが終了したかを返します
	bool isEnd();
	// アニメーションの逆再生を行います
	void animaReverse();
};

#endif