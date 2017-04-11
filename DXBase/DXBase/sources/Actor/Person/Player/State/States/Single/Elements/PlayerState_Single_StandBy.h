#pragma once

#include "../PlayerState_Single.h"

class PlayerState_Single_StandBy : public PlayerState_Single {
public:
	PlayerState_Single_StandBy(const Keys& keys);
private:
	virtual void unique_init() override;
	virtual void update(float deltaTime) override;
	virtual void end() override;
};

