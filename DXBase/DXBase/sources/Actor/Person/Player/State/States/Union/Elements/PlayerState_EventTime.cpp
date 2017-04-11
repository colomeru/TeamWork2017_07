#include "PlayerState_EventTime.h"

PlayerState_EventTime::PlayerState_EventTime(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty) : PlayerState_Union(butty, retty) {}

void PlayerState_EventTime::unique_init(){
	butty_->animation().change(PlayerAnimID::IDLE);
	retty_->animation().change(PlayerAnimID::IDLE);
}

void PlayerState_EventTime::update(float deltaTime) {
	if (cntr_->getWorld()->isLetOuted()) {
		change(PlayerState_Enum_Union::LEAN_BACK, ActionType::Left);
	}
}

void PlayerState_EventTime::end(){
	butty_->reset_enemy();
	retty_->reset_enemy();
}

void PlayerState_EventTime::input(float deltaTime){}
