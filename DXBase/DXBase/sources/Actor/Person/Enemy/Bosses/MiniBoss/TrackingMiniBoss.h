#ifndef TRACKING_MINI_BOSS_H_
#define TRACKING_MINI_BOSS_H_

#include "AdhereMiniBoss.h"

class TrackingMiniBoss : public AdhereMiniBoss{
public:
	TrackingMiniBoss(
		IWorld* world,
		const Vector2&  position,
		const Vector2& direction = Vector2::One,
		const float bodyScale = 64.0f);

private:
	// ˆÚ“®ó‘Ô
	void move(float deltaTime) override;
};

#endif