#pragma once

#include "../PlayerState_Union.h"

class PlayerState_LeanBack : public PlayerState_Union {
public:
	PlayerState_LeanBack(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty);
private:
	virtual void unique_init() override;
	virtual void update(float deltaTime) override;
	virtual void collide(const Actor & other) override {}
	virtual void end() override;
	virtual void input(float deltaTime) override;
private:
	Vector2 dir_;
	float power_;
};