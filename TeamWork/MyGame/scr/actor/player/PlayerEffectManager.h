#pragma once
#include<vector>

enum SPRITE_ID;
struct Vector2;
#include<string>

class PlayerEffectManager {
public:
	PlayerEffectManager(SPRITE_ID id, int anmSpeed);
	~PlayerEffectManager();

	void Update();
	void DrawEffect(const Vector2& position)const;
	bool GetIsEnd()const;
private:
	int maxFrame_;
	SPRITE_ID id_;
	int anmSpeed_;
	int frameCount_;
	bool isEnd_;
};