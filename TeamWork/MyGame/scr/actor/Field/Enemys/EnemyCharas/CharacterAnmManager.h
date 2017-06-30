#pragma once
#include"../../../../game/ID.h"
#include<vector>
#include"../../../../math/Vector2.h"

class CharacterAnmManager {
public:
	CharacterAnmManager();
	~CharacterAnmManager();

	void Add(SPRITE_ID id);
	void ResetAnm();
	void ReverseAnm();
	void Update();
	void Draw(const Vector2& position, const Vector2& origin, const Vector2& size=Vector2::One, float alpha=1.0f)const;
	void Clear();
	float GetAnmEndTime()const;
	bool IsEndAnimation()const;
	void SetIsRepeat(bool isrp);
	void SetTimeCount(float time);
	void SetAnmSpeed(int anmChangeFrame);
private:
	std::vector<SPRITE_ID> anmID_;

	float timeCount_;
	int anmChangeFrame_;

	float reverse_;

	bool anmEnd_;
	bool isRepeat_;
};