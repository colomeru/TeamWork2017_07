#pragma once
#include"../Effect.h"

class PlayerBiteEffect :public Effect {
public:
	PlayerBiteEffect(IWorld* world, const Vector2& position);

	void Update()override;
	void Draw()const override;

};
