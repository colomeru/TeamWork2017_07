#include "BadItemAnm.h"

#include "../../ResourceLoader/ResourceLoader.h"

#include <algorithm>

BadItemAnm::BadItemAnm() :
	alpha_(255) {
	add_anim(0, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::ITEM_MINUS_ANM));
	change_param(0, 1);
	//add(PlayerEffectID::SEP_EXP, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_EFFECT_SEP_EXP));
	//add(PlayerEffectID::SEP_MOVE, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_EFFECT_SEP_MOVE));
}

void BadItemAnm::update_e(float deltaTime) {
	//alpha_ = std::max<float>(alpha_ - deltaTime * 60, 0);
	update(deltaTime);
}

void BadItemAnm::draw_e(const Vector2 & position, const Vector2 & origin, const float & scale)const {
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	//Vector2 drawPos = Vector2(position.x - ((ResourceLoader::GetInstance().GetTextureSize(AnimationID::ITEM_ANM).x / 2)*scale), position.y);

	Vector2 origin_ = Vector2((ResourceLoader::GetInstance().GetTextureSize(AnimationID::ITEM_MINUS_ANM)) / 2);
	draw(position, origin_, scale);
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}
