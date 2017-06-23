#pragma once
#include "../../../actor/Actor.h"


class CreditTextGenerator : public Actor {
public:
	//
	CreditTextGenerator(IWorld* world, Vector2 position);
	//
	~CreditTextGenerator();
	//
	virtual void Update() override;
	//
	virtual void Draw() const override;

private:
	float timer_;
	const float DefTime = 3.0f;
};
