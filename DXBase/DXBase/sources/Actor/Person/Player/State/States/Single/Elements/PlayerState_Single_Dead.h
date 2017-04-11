#pragma once

#include "../PlayerState_Single.h"

class PlayerState_Single_Dead : public PlayerState_Single {
public:
	PlayerState_Single_Dead(const Keys& keys);
private:
	virtual void unique_init() override;
	virtual void update(float deltaTime) override;
	virtual void end() override;
	virtual void input(float deltaTime) override;
};