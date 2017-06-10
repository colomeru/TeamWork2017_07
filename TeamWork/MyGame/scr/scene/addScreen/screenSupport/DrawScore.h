#pragma once
#include<map>
#include"../../../math/Vector2.h"

class DrawScore {
private:
	DrawScore();
	~DrawScore();
	
public:

	static DrawScore& getInstance() {
		static DrawScore ds;
		return ds;
	}
	
	void Draw(const Vector2& position, int score,int digit);

private:
	int numberSpriteSize_;
};