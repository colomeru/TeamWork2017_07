#include "SceneChangeBossAnm.h"

#include "../../ResourceLoader/ResourceLoader.h"

#include <algorithm>

SceneChangeBossAnm::SceneChangeBossAnm(float speed) :
	alpha_(255), isFirstIdle(false), isFirstJump(false),
	mAnimSpeed(speed){
	add_anim(0, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::BOSS_WAIT_TEX));
	add_anim(1, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::BOSS_WALLATTACK_DASHJUMP_TEX));
	change_param(1, mAnimSpeed);
	change_dir_type(0, ActionType::Left);
	//add(PlayerEffectID::SEP_EXP, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_EFFECT_SEP_EXP));
	//add(PlayerEffectID::SEP_MOVE, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_EFFECT_SEP_MOVE));
}

void SceneChangeBossAnm::update_e(float deltaTime) {
	//alpha_ = std::max<float>(alpha_ - deltaTime * 60, 0);
	back_to_pre_motion();

	if (norm_anim_ == 1) {
		frame_ = static_cast<int>(timer_) % sprites_[curr_anim_].size();
		if (timer_ >= sprites_[curr_anim_].size())frame_ = sprites_[curr_anim_].size() - 1;
	}
	else frame_ = static_cast<int>(timer_) % sprites_[curr_anim_].size();
	
	id_ = sprites_[curr_anim_][frame_];
	//çXêVèàóù
	timer_ += deltaTime * curr_speed_ * 60.0f / sprites_[curr_anim_].size() * 10;

}
void SceneChangeBossAnm::Turn()
{
	if (isFirstIdle) {
		change_param(0, mAnimSpeed);
		isFirstIdle = false;
	}
	if (isFirstJump) {
		change_param(1, mAnimSpeed);
		isFirstJump = false;
	}

}
void SceneChangeBossAnm::draw_e(const Vector2 & position, const Vector2 & origin, const float & scale,float rotate)const {
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

	Vector2 drawPos = Vector2(position.x - ((ResourceLoader::GetInstance().GetTextureSize(AnimationID::BOSS_WAIT_TEX).x / 2)*scale), position.y);

	draw(drawPos, origin, scale,rotate);
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}
