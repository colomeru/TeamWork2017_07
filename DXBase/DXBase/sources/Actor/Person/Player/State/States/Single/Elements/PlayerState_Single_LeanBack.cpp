#include "PlayerState_Single_LeanBack.h"

#include "../../../../../../../Define.h"
#include "../../../../Effect/PlayerEffectObj.h"
#include "../../../../../../Base/ActorGroup.h"
#include "../../../PlayerState_Enum.h"

PlayerState_Single_LeanBack::PlayerState_Single_LeanBack(const Keys& keys) :
	PlayerState_Single(keys),
	dir_(Vector2::Zero), power_(0), firstPos_(Vector2::Zero){}

void PlayerState_Single_LeanBack::unique_init(){
	body_->animation().change(PlayerAnimID::DAMAGE);

	PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_BUNRI) ,DX_PLAYTYPE_BACK);

	if(is_butty())body_->world_->addActor(ActorGroup::Effect, std::make_shared<PlayerEffectObj>(body_->world_, body_->getPosition(), PlayerEffectID::SEP_EXP, 5.0f, 3.0f));

	body_->reset_opponent();

	dir_ = Vector2(body_->get_partner_vector().x, 1) * -1;
	if (dir_.x == 0) {
		if (body_->name_ == "PlayerBody1")dir_.x = 1;
		else dir_.x = -1;
	}
	firstPos_ = body_->getPosition();
	power_ = PLAYER_LEANBACK_POWER;
	body_->position() += dir_ * body_->velocity() * power_;
}

void PlayerState_Single_LeanBack::update(float deltaTime) {
	dir_.y += 0.1f;
	body_->position() += dir_ * body_->velocity() * power_ * time(deltaTime);
	if ((body_->getPosition().y > firstPos_.y || body_->getOpponent() == HitOpponent::FLOOR_TOP) && dir_.y > 0) {
		request(StateElement((unsigned int)PlayerState_Enum_Single::IDLE));
	}
}

void PlayerState_Single_LeanBack::end(){}


