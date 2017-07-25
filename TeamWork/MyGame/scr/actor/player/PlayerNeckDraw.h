#pragma once
#include"Player.h"

class PlayerNeckDraw {
public:
	PlayerNeckDraw();
	~PlayerNeckDraw();
	void Draw(const Vector2& basePos, const Vector2& lastPos)const;

private:
	std::vector<SPRITE_ID> ids_;
};