#pragma once
#include"../../../math/Vector2.h"
#include<vector>

class DrawStar {
public:
	DrawStar(int delayTime=0);
	~DrawStar();

	void Update();

	void Draw(const Vector2& position)const;

	void SetStarCount(int count);
	void SetFullStarCount(int count);

	void SetFull();
	bool GetStarFull()const;
private:
	std::vector<int> timeCount_;
	int starCount_;
	float fstarCount_;
	bool isSetStar_;
	bool isFullStar_;

	std::vector<bool> isStarUp_;
};