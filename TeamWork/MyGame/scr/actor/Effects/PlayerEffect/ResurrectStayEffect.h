#pragma once
#include"../Effect.h"

class ResurrectStayEffect :public Effect {
public:
	ResurrectStayEffect(IWorld* world, const Vector2& position);

	void Update()override;
	void Draw()const override;

private:
};
