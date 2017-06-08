#pragma once
#include"../Effect.h"

class SwordAttackEffect :public Effect {
public:
	SwordAttackEffect(IWorld* world, const Vector2& position);

	void Update()override;
	void Draw()const override;

};
