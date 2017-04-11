#include "PlayerState_Single_Idle.h"

#include "../../../../Effect/PlayerEffectObj.h"
#include "../../../../../../../Input/InputMgr.h"
#include "../../../../../../../Define.h"
#include "../../../../../../../Scene/Base/SceneDataKeeper.h"
#include "../../../../../../Base/ActorGroup.h"
#include "../../../PlayerState_Enum.h"
#include <algorithm>

PlayerState_Single_Idle::PlayerState_Single_Idle(const Keys& keys) : PlayerState_Single(keys) {}

void PlayerState_Single_Idle::unique_init(){

	body_->reset_enemy();
	body_->animation().change(PlayerAnimID::DIV_IDLE);

	target_ = body_->position();
	
	if (body_->get_partner_vector().x > 0) {
		body_->animation().change_dir(PlayerAnimID::DIV_IDLE, ActionType::Right);
	}
	else {
		body_->animation().change_dir(PlayerAnimID::DIV_IDLE, ActionType::Left);
	}

	if (is_butty())return;
}

void PlayerState_Single_Idle::update(float deltaTime){
	if (timer_ >= PLAYER_DEAD_LIMIT) {
		request(StateElement((unsigned int)PlayerState_Enum_Single::DEAD));
	}
	if (static_cast<int>(timer_ * 60) % 60 == 0) {
		body_->getWorld()->addActor(ActorGroup::Effect, std::make_shared<PlayerEffectObj>(body_->getWorld(), body_->getPosition(), PlayerEffectID::SEP_MOVE, 5.0f, 0.5f));
	}
	if ((int)(timer_ * 10) % 2 < 1) body_->alpha_ -= (int)(time(deltaTime) * 20);
	else body_->alpha_ += time(deltaTime) * 20;
	body_->alpha_ = MathHelper::Clamp(body_->alpha_, 100, 255);

	move(deltaTime);
}

void PlayerState_Single_Idle::collide(const Actor & other){
	if ((body_->getName() == "PlayerBody1" && other.getName() == "PlayerBody2Collider") ||
		(body_->getName() == "PlayerBody2" && other.getName() == "PlayerBody1Collider")) {
		request(StateElement((unsigned int)PlayerState_Enum_Single::STAND_BY));
	}
}

void PlayerState_Single_Idle::draw() const {
}

void PlayerState_Single_Idle::end(){
	body_->alpha_ = 255.0f;
}

void PlayerState_Single_Idle::input(float deltaTime) {
	if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_L1) || InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_R1)) {
		target_ += body_->get_partner_vector() * std::max<float>(20.0f - body_->getWorld()->GetKeeper()->getDamageCount() * 2, 10) * time(deltaTime);
	}
}

void PlayerState_Single_Idle::move(float deltaTime){
	body_->position().x = MathHelper::Lerp(body_->position().x, target_.x, 0.2f);
	body_->position() += gravity(deltaTime);
}


