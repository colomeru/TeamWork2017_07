#ifndef ROCK_H_
#define ROCK_H_

#include "Bosses/MiniBoss/AdhereMiniBoss.h"

class Rock : public AdhereMiniBoss {
public:
	Rock(IWorld* world, const Vector2&  position, const float bodyScale = 64.0f);

private:
	// —‰ºó‘Ô
	void move(float deltaTime) override;
	// €–Só‘Ô
	void deadMove(float deltaTime) override;
	// °‚É“–‚½‚Á‚½‚Ìˆ—
	void floorHit() override;
};

#endif
