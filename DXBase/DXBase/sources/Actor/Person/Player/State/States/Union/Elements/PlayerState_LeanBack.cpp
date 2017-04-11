#include "PlayerState_LeanBack.h"

#include "../../../../../../../Define.h"

PlayerState_LeanBack::PlayerState_LeanBack(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty) : PlayerState_Union(butty, retty) {}

void PlayerState_LeanBack::unique_init(){
	if (element_.type_ == ActionType::Right) {
		dir_ = Vector2(1, -1);
	}
	if (element_.type_ == ActionType::Left) {
		dir_ = Vector2(-1, -1);
	}

	power_ = PLAYER_LEANBACK_POWER;

	butty_->reset_opponent();
	retty_->reset_opponent();
}

void PlayerState_LeanBack::update(float deltaTime){
	dir_.y += 0.1f;

	butty_->position() += dir_ * power_ * time(deltaTime);
	retty_->position() += dir_ * power_ * time(deltaTime);

	if (isOnFloor()) {
		change(PlayerState_Enum_Union::IDLE);
	}
}

void PlayerState_LeanBack::end(){
	butty_->reset_enemy();
	retty_->reset_enemy();
}

void PlayerState_LeanBack::input(float deltaTime){}
