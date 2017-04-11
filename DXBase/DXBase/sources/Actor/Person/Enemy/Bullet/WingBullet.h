#ifndef WING_BULLET_H_
#define WING_BULLET_H_

#include "../../../Base/Actor.h"

class WingBullet : public Actor {
public:
	WingBullet(
		IWorld* world,
		const Vector2& position,
		const float degree = 0.0f,
		const float speed = 4.0f,
		const float bodyScale = 16.0f);
	void onUpdate(float deltaTime) override;
	void onDraw() const override;
	void onCollide(Actor& actor) override;

private:
	float speed_;			// ë¨ìx
	float degree_;			// äpìx
	Vector2 direction_;		// ï˚å¸
	bool isFloorHit_;		// è∞Ç…ìñÇΩÇ¡ÇΩÇ©
};

#endif