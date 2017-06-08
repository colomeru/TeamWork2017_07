#pragma once
#include"../Effect.h"

class TapperTapEffect :public Effect {
public:
	TapperTapEffect(IWorld* world, const Vector2& position);

	void Update()override;
	void Draw()const override;

};
