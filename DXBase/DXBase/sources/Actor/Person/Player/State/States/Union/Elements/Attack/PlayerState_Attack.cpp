#include "PlayerState_Attack.h"

#include "../../../../../../../../Define.h"
#include "../../../../../../../../Input/InputMgr.h"
#include "../../../../../../../../Scene/Base/SceneDataKeeper.h"
#include "../../../../../../../Base/ActorGroup.h"

#include <algorithm>

PlayerState_Attack::PlayerState_Attack(const PlayerBodyPtr & butty, const PlayerBodyPtr & retty) :
	PlayerState_Union(butty, retty),
	dir_(Vector2::Zero), launch_dir_(Vector2::Zero),
	launch_power_(0), launch_dir_easeing_(0),
	chase_dir_(Vector2::Zero), chase_power_(0), chase_dir_easeing_(0),
	gravity_(Vector2::Zero), gra_easeing_(0) {

}

void PlayerState_Attack::unique_init(){
	if (element_.type_ == ActionType::Right) {
		launchBody_ = retty_;
		chaseBody_ = butty_;
	}
	if (element_.type_ == ActionType::Left) {
		launchBody_ = butty_;
		chaseBody_ = retty_;
	}

	PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_SHOT), DX_PLAYTYPE_BACK);

	butty_->reset_opponent();
	retty_->reset_opponent();

	launch_power_ = PLAYER_LAUNCH_POWER;
	chase_power_ = PLAYER_LAUNCH_POWER * 0.65f;
	launch_dir_easeing_ = 0;
	chase_dir_easeing_ = 0;
	gra_easeing_ = 0;

	// çUåÇîªíËÇÃê∂ê¨
	auto attackColl = std::make_shared<PlayerBodyCollider>(cntr_->getWorld(), std::string("PlayerAttack"));
	cntr_->getWorld()->addActor(ActorGroup::Player_Collider, attackColl);
	attackColl_ = attackColl;

	cntr_->getWorld()->GetKeeper()->addComboCount(1, 1);

	launch_dir_ = Vector2::Normalize(chaseBody_->getPosition() - launchBody_->getPosition());
	chase_dir_ = Vector2::Normalize(chaseBody_->getPosition() - launchBody_->getPosition());
}

void PlayerState_Attack::update(float deltaTime){
	timer_ += deltaTime;
	//timer_ = std::min<float>(timer_ + deltaTime, 1);

	launch_dir_easeing_ = -EasingOutExpo(timer_ * 0.7f) + 1;
	chase_dir_easeing_ = EasingSinCurve(std::max<float>(timer_ - 0.1f, 0) * 3.0f);
	gra_easeing_ = std::min<float>(EasingInExpo(timer_ * 0.8f), 1);

	if (butty_->getColliderOpponent() == HitOpponent::FLOOR_HIT && retty_->getColliderOpponent() == HitOpponent::FLOOR_HIT) {
		change(PlayerState_Enum_Union::FREEZE);
	}


	if (butty_->hit_enemy() == HitOpponent::BOSS || retty_->hit_enemy() == HitOpponent::BOSS) {
		if (launch_dir_.x > 0) {
			change(PlayerState_Enum_Union::LEAN_BACK, ActionType::Left);
		}
		else {
			change(PlayerState_Enum_Union::LEAN_BACK, ActionType::Right);
		}
	}

	launchBody_->position() += (launch_dir_ * launch_dir_easeing_ * launch_power_ + Vector2::Up * GRAVITY * gra_easeing_) * launchBody_->velocity() * time(deltaTime);
	chaseBody_->position() += (chase_dir_ * chase_dir_easeing_ * chase_power_ + Vector2::Up * GRAVITY * gra_easeing_) * chaseBody_->velocity() * time(deltaTime);

	attackColl_->position() = launchBody_->getPosition();

	if (cntr_->getWorld()->isEntered()) change(PlayerState_Enum_Union::EVENT);
}

inline void PlayerState_Attack::collide(const Actor & other) {}

void PlayerState_Attack::end(){
	// çUåÇîªíËÇÃçÌèú
	attackColl_->dead();
	attackColl_ = nullptr;

	butty_->reset_enemy();
	retty_->reset_enemy();
}

void PlayerState_Attack::input(float deltaTime){
	if (holdable_padR()) change(PlayerState_Enum_Union::HOLD, ActionType::Left);
	if (holdable_padL()) change(PlayerState_Enum_Union::HOLD, ActionType::Right);

	if (InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_R1) && element_.type_ == ActionType::Left) change(PlayerState_Enum_Union::HOLD_AIR, element_.type_);

	if (InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_L1) && element_.type_ == ActionType::Right) change(PlayerState_Enum_Union::HOLD_AIR, element_.type_);

}
