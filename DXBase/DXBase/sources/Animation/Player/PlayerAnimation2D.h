#pragma once

#include "../Base/Animation2D.h"

#include <unordered_map>

enum class PlayerAnimID {
	NONE,
	IDLE,
	MOVE,
	HOLD,
	SWIM,	
	DEATH,
	DAMAGE,
	DIV_IDLE,
	DIV_MOVE,
	TURN,
	SWIM_TURN,
	COUNT
};


class PlayerAnimation2D : public Animation2D{
public:
	PlayerAnimation2D(const std::string& name = "", const ActionType& type = ActionType::Right);
	void add(const PlayerAnimID & id, const std::vector<int>& anims);
	void change(const PlayerAnimID& id, const float& speed = 1.0f);
	void change_dir(const PlayerAnimID& id, const ActionType& type);
	void random_change() {}
};
