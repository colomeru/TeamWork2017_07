#include "StageClearTextAnm.h"

#include "../../ResourceLoader/ResourceLoader.h"

#include <algorithm>

StageClearTextAnm::StageClearTextAnm() :
	alpha_(255) {
	add_anim(0, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::TEXT_STAGECLEAR_ANM_TEX));
	change_param(0, 1);
	//add(PlayerEffectID::SEP_EXP, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_EFFECT_SEP_EXP));
	//add(PlayerEffectID::SEP_MOVE, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_EFFECT_SEP_MOVE));
}

void StageClearTextAnm::update_e(float deltaTime) {
	//alpha_ = std::max<float>(alpha_ - deltaTime * 60, 0);
	update(deltaTime);
}

void StageClearTextAnm::draw_e(const Vector2 & position, const Vector2 & origin, const float & scale)const {
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

	Vector2 drawPos = Vector2(position.x - ((ResourceLoader::GetInstance().GetTextureSize(AnimationID::TEXT_STAGECLEAR_ANM_TEX).x/2)*scale), position.y);

	draw(drawPos, origin, scale);
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}
