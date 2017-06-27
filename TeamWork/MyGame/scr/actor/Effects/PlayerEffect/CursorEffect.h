#pragma once
#include"../Effect.h"

class CursorEffect :public Effect {
public:
	enum Arrow {
		Left,
		Right
	};
public:
	CursorEffect(IWorld* world, const Vector2& position, Actor* target,Arrow arrow,float delay=0.0f);

	void Update()override;
	void Draw()const override;

private:
	Actor* target_;

	Vector2 addVec_;

	bool isturn_;
	float delay_;

};
