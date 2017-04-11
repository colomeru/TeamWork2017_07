#pragma once

#include "../Base/Animation2D.h"

#include <unordered_map>

enum class PlayerEffectID {
	NONE,
	SEP_EXP,
	SEP_MOVE,
	SHOUGEKI,
	COUNT
};

class PlayerEffect2D : public Animation2D {
public:
	PlayerEffect2D();
	void add(const PlayerEffectID & id, const std::vector<int>& anims);
	void change(const PlayerEffectID& id, const float& speed = 1.0f);
	void update_e(float deltaTime);
	void draw_e(const Vector2& position, const Vector2& origin = Vector2::Zero, const float& scale = 1.0f)const;
private:
	float alpha_;
};
