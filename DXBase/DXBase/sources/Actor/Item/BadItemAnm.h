#pragma once

#include "../../Animation/Base/Animation2D.h"

#include <unordered_map>


class BadItemAnm : public Animation2D {
public:
	BadItemAnm();
	//void add(const PlayerEffectID & id, const std::vector<int>& anims);
	//void change(const PlayerEffectID& id, const float& speed = 1.0f);
	void update_e(float deltaTime);
	void draw_e(const Vector2& position, const Vector2& origin = Vector2::Zero, const float& scale = 1.0f)const;
private:
	float alpha_;
};
#pragma once
