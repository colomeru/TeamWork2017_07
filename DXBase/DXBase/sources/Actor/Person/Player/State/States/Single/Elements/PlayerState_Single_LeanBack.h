#pragma once

#include "../PlayerState_Single.h"

class PlayerState_Single_LeanBack : public PlayerState_Single {
public:
	PlayerState_Single_LeanBack(const Keys& keys);
private:
	virtual void unique_init() override;
	virtual void update(float deltaTime) override;
	virtual void end() override;
private:
	Vector2 dir_;
	float power_;
	Vector2 firstPos_;
};