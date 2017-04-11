#pragma once

#include "../../Animation/Base/Animation2D.h"

#include <map>


class SceneChangeAnm : public Animation2D {
public:
	SceneChangeAnm();
	//void add(const PlayerEffectID & id, const std::vector<int>& anims);
	//void change(const PlayerEffectID& id, const float& speed = 1.0f);
	void set();
	void update_e(float deltaTime, const std::map<int, bool>& slimelist);
	void draw_e(int num,const Vector2& position, const Vector2& origin = Vector2::Zero, const float& scale = 1.0f, const Vector3& color=Vector3::Zero)const;
private:
	float alpha_;
	std::map<int, int> mainUc_;
	std::map<int, int> mainUcT_;
	std::map<int,float> mainc_;
};
