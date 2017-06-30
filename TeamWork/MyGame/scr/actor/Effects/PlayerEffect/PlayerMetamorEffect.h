#pragma once
#include"../Effect.h"

class PlayerMetamorEffect:public Effect{
public:
	PlayerMetamorEffect(IWorld* world, const Vector2& position,std::shared_ptr<Actor> target, float delay=0.0f, const Vector2& addVec=Vector2::Zero);

	void Update()override;
	void Draw()const override;

private:
	std::weak_ptr<Actor> target_;
	float delay_;
	Vector2 addVec_;
};
