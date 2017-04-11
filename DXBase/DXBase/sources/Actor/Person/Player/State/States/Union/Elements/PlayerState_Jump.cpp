#include "PlayerState_Jump.h"

#include "../../../../../../../Define.h"
#include "../../../../../../../Input/InputMgr.h"

#include <algorithm>

PlayerState_Jump::PlayerState_Jump(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty) : 
	PlayerState_Union(butty, retty), 
	dir_(Vector2::Zero), power_(0), gra_easeing_(0) {}

void PlayerState_Jump::unique_init(){
	butty_->reset_opponent();
	retty_->reset_opponent();

	dir_ = Vector2::Down;
	power_ = PLAYER_JUMP_POWER;
	gra_easeing_ = 0;
}

void PlayerState_Jump::update(float deltaTime) {
	timer_ = std::min<float>(timer_ + deltaTime, 60);

	dir_.y = std::min<float>(dir_.y + 0.1f, 1);

	butty_->position() += dir_ * power_ * time(deltaTime);
	retty_->position() += dir_ * power_ * time(deltaTime);

	if (butty_->getOpponent() == HitOpponent::FLOOR_TOP || retty_->getOpponent() == HitOpponent::FLOOR_TOP) {
		change(PlayerState_Enum_Union::IDLE);
	}

	if (cntr_->getWorld()->isEntered()) change(PlayerState_Enum_Union::EVENT);
}

void PlayerState_Jump::end(){}

void PlayerState_Jump::input(float deltaTime){
	butty_->position() += InputMgr::GetInstance().AnalogPadVectorR().Horizontal() / 2 * speed_b(deltaTime);
	retty_->position() += InputMgr::GetInstance().AnalogPadVectorL().Horizontal() / 2 * speed_r(deltaTime);
}

