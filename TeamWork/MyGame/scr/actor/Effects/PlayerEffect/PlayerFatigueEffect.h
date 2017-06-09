#pragma once
#include"../Effect.h"

class PlayerFatigueEffect :public Effect {
public:
	PlayerFatigueEffect(IWorld* world, const Vector2& position, Actor* target, float delay = 0.0f);

	void Update()override;
	void Draw()const override;

private:
	Actor* target_;
	float delay_;
};
