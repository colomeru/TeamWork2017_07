#include "PlayerAnimation2D.h"

#include "../../ResourceLoader/ResourceLoader.h"

PlayerAnimation2D::PlayerAnimation2D(const std::string& name, const ActionType& type) : Animation2D(type){
	sprites_.clear();

	if (name == "") return;
	//butty
	if (name == "PlayerBody1") {
		add(PlayerAnimID::IDLE,		 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_BUTTY_IDLE));
		add(PlayerAnimID::MOVE,		 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_BUTTY_MOVE));
		add(PlayerAnimID::HOLD,		 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_BUTTY_HOLD));
		add(PlayerAnimID::SWIM,		 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_BUTTY_SWIM));
		add(PlayerAnimID::DEATH,	 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_BUTTY_DEATH));
		add(PlayerAnimID::DAMAGE,	 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_BUTTY_DAMAGE));
		add(PlayerAnimID::DIV_IDLE,	 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_BUTTY_DIV_IDLE));
		add(PlayerAnimID::DIV_MOVE,	 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_BUTTY_DIV_MOVE));
		add(PlayerAnimID::TURN,		 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_BUTTY_TURN));
		add(PlayerAnimID::SWIM_TURN, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_BUTTY_SWIM_TURN));
	}
	//retty
	if (name == "PlayerBody2") {
		add(PlayerAnimID::IDLE,		 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_RETTY_IDLE));
		add(PlayerAnimID::MOVE,		 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_RETTY_MOVE));
		add(PlayerAnimID::HOLD,		 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_RETTY_HOLD));
		add(PlayerAnimID::SWIM,		 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_RETTY_SWIM));
		add(PlayerAnimID::DEATH,	 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_RETTY_DEATH));
		add(PlayerAnimID::DAMAGE,	 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_RETTY_DAMAGE));
		add(PlayerAnimID::DIV_IDLE,  ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_RETTY_DIV_IDLE));
		add(PlayerAnimID::DIV_MOVE,  ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_RETTY_DIV_MOVE));
		add(PlayerAnimID::TURN,		 ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_RETTY_TURN));
		add(PlayerAnimID::SWIM_TURN, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_RETTY_SWIM_TURN));
	}
}

void PlayerAnimation2D::add(const PlayerAnimID & id, const std::vector<int>& anims){
	add_anim(static_cast<int>(id), anims);
}

void PlayerAnimation2D::change(const PlayerAnimID& id, const float& speed){
	change_param(static_cast<int>(id), speed);
}

void PlayerAnimation2D::change_dir(const PlayerAnimID& id, const ActionType& type){
	change_dir_type(static_cast<int>(id), type);
}

