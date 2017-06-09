#pragma once
#include"../Effect.h"

class Player_Sword;

class GetSwordEffect :public Effect {
public:
	GetSwordEffect(IWorld* world, const Vector2& position,Player_Sword* target);

	void Update()override;
	void Draw()const override;

private:
	Player_Sword* target_;
};
