#include "PlayerState_Single_StandBy.h"

#include "../../../PlayerState_Enum.h"

PlayerState_Single_StandBy::PlayerState_Single_StandBy(const Keys& keys) : PlayerState_Single(keys) {

}

void PlayerState_Single_StandBy::unique_init() {
	body_->reset_enemy();
}

void PlayerState_Single_StandBy::update(float deltaTime){
	request(StateElement((unsigned int)PlayerState_Enum_Single::LEAN_BACK));
}

void PlayerState_Single_StandBy::end(){
	body_->reset_enemy();
}
