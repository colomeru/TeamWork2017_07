#ifndef NEEDLE_ENEMY_H_
#define NEEDLE_ENEMY_H_

#include "../../../Base/Actor.h"

// êjÉNÉâÉX(âΩÇ‡ÇµÇ»Ç¢ÅBè·äQï®)
class NeedleEnemy : public Actor{
public:
	NeedleEnemy(IWorld * world,
		const Vector2 & position,
		const float degrees);
	void onUpdate(float deltaTime) override;
	void onDraw() const override;
	void onCollide(Actor& actor) override;
	void onMessage(EventMessage event, void*) override;

private:
	float degrees_;
};

#endif
