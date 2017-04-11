#include "SceneChangeAnm.h"

#include "../../ResourceLoader/ResourceLoader.h"

#include <algorithm>
#include<map>

SceneChangeAnm::SceneChangeAnm() :
	alpha_(255) {
	//add(PlayerEffectID::SEP_EXP, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_EFFECT_SEP_EXP));
	//add(PlayerEffectID::SEP_MOVE, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::PLAYER_EFFECT_SEP_MOVE));
}
void SceneChangeAnm::set() {
	add_anim(0, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::SCENE_CHANGE_SLIME_ANM));
	change_param(0, 1);
	for (int i = 0; i < 100; i++) {
		mainc_[i] = 0;
	}
	for (int i = 0; i < 100; i++) {
		mainUc_[i] = 0;
	}
	for (int i = 0; i < 100; i++) {
		mainUcT_[i] = 2;
	}

}
void SceneChangeAnm::update_e(float deltaTime,const std::map<int,bool>& slimelist) {
	//alpha_ = std::max<float>(alpha_ - deltaTime * 60, 0);
	update(deltaTime);

	int count = 0;
	for (auto i : slimelist) {
		if (i.second) {
			mainUcT_[count]--;
			if (mainUcT_[count] <= 0)
			{
				mainc_[count]++;// static_cast<int>(timer_) % sprites_[curr_anim_].size();
				mainUc_[count]++;
				mainUcT_[count] = 2;
			}
		if (mainUc_[count] >= (int)sprites_[curr_anim_].size())mainc_[count] = 0;
		}
		else {
			mainc_[count]= 0;
		}

		count++;
	}
	//for (int i = 0; i < 8; i++) {
	//	mainc_[i] = (static_cast<int>(timer_) + i*3) % sprites_[curr_anim_].size();
	//}
}

void SceneChangeAnm::draw_e(int num,const Vector2 & position, const Vector2 & origin, const float & scale,const Vector3& color)const {
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	//draw(position, origin, scale,0,color);
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetDrawBright(static_cast<int>(color.x), static_cast<int>(color.y), static_cast<int>(color.z));
	//度数法→弧度法に変換
	float radian = MathHelper::ToRadians(0);
	

	if (type_ == ActionType::Right)DrawRotaGraph3((int)position.x, (int)position.y, (int)origin.x, (int)origin.y,1, 1, radian, sprites_.at(curr_anim_).at(((int)mainc_.at(num))), TRUE, FALSE);
	if (type_ == ActionType::Left) DrawRotaGraph3(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(origin.x), static_cast<int>(origin.y), 1,1, radian, sprites_.at(curr_anim_).at(((int)mainc_.at(num))), TRUE, TRUE);
	SetDrawBright(255, 255, 255);
}
