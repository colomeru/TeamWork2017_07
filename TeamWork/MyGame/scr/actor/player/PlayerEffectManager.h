#pragma once
#include<vector>

enum SPRITE_ID;
struct Vector2;
#include<string>
#include"../../math/Vector2.h"

class PlayerEffectManager {
public:
	PlayerEffectManager(SPRITE_ID id, int anmSpeed);
	~PlayerEffectManager();

	void Update();
	void DrawEffect(const Vector2& position, float angle=0.0f, const Vector2& size=Vector2::One, bool isturn=false)const;
	bool GetIsEnd()const;
private:
	int maxFrame_;
	SPRITE_ID id_;
	int anmSpeed_;
	int frameCount_;
	bool isEnd_;
};