#ifndef POWER_UP_EFFECT_H_
#define POWER_UP_EFFECT_H_

#include "../../Base/Actor.h"
#include "../../../ResourceLoader/ResourceLoader.h"
#include "../../../Animation/Enemy/EnemyAnimation2D.h"

class PowerUpEffect : public Actor{
protected:
	enum {
		POWER_UP_FONT_NUMBER = 0,
		STAR_POWER_UP_NUMBER = 1
	};

public:
	PowerUpEffect(
		IWorld* world,
		const std::string name,
		const Vector2& position,
		const Vector2& size,
		const int id,
		const AnimationID animeID);
	void onUpdate(float deltaTime) override;
	void onDraw() const override;

protected:
	Vector2 size_;					// 大きさ
	Vector2 scale_;					// 大きさの比率
	EnemyAnimation2D animation_;	// アニメーション
};

#endif