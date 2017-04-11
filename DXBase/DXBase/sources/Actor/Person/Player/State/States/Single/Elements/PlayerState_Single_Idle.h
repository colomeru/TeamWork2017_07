#pragma once

#include "../PlayerState_Single.h"

class PlayerState_Single_Idle : public PlayerState_Single {
public:
	PlayerState_Single_Idle(const Keys& keys);
private:
	virtual void unique_init() override;
	virtual void update(float deltaTime) override;
	virtual void collide(const Actor & other) override;
	virtual void draw()const override;
	virtual void end() override;
	virtual void input(float deltaTime) override;
private:
	void move(float deltaTime);

	Vector2 target_;
};

