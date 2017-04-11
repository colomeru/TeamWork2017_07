#pragma once

#include "../../Animation/Base/Animation2D.h"

#include <unordered_map>


class SceneChangeBossAnm : public Animation2D {
public:
	SceneChangeBossAnm(float speed = 1.0f);
	//void add(const PlayerEffectID & id, const std::vector<int>& anims);
	//void change(const PlayerEffectID& id, const float& speed = 1.0f);
	void update_e(float deltaTime);
	void Turn();
	void setIdle() {
		isFirstIdle = true;
	}
	void setJump() {
		isFirstJump = true;
	}
	void draw_e(const Vector2& position, const Vector2& origin = Vector2::Zero, const float& scale = 1.0f,float rotate = 0)const;
private:
	float alpha_;
	bool isFirstIdle;
	bool isFirstJump;
	float mAnimSpeed;
};
