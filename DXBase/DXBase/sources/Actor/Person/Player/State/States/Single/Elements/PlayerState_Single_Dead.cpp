#include "PlayerState_Single_Dead.h"

#include "../../../../../../../Define.h"

PlayerState_Single_Dead::PlayerState_Single_Dead(const Keys & keys) : PlayerState_Single(keys) {}

void PlayerState_Single_Dead::unique_init(){
	body_->animation().change(PlayerAnimID::DEATH);
}

void PlayerState_Single_Dead::update(float deltaTime){
	if (body_->animation().end_anim())body_->dead();

	body_->position() += gravity(deltaTime);
}

void PlayerState_Single_Dead::end(){}

void PlayerState_Single_Dead::input(float deltaTime){}


