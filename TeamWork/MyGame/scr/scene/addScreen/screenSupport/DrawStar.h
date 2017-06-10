#pragma once
#include"../../../math/Vector2.h"

class DrawStar {
public:
	DrawStar();
	~DrawStar();

	void Update();

	void Draw(const Vector2& position)const;

	void SetStarCount(int count) {
		starCount_ = count;
	}
private:
	int timeCount_;
	int starCount_;
};