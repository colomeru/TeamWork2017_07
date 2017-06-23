#pragma once
#include"../Effect.h"

class ResurrectEffect :public Effect {
public:
	ResurrectEffect(IWorld* world, const Vector2& position, Actor* target);

	void Update()override;
	void Draw()const override;

private:
	Actor* target_;

	Vector2 addVec_;
};
