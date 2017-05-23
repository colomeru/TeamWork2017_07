#pragma once
#include"../BackgroundCharacters.h"

enum WindDir {
	UP,
	DOWN,
	RIGHT,
	LEFT
};
class LaneChangeWind :public BackgroundCharacters {
public:
	LaneChangeWind(IWorld* world, const Vector2& position, WindDir dir);
	virtual void Update()override;
	virtual void FastUpdate() override {
		isUpdate_ = true;
		isDraw_ = true;
	}
	virtual void Draw()const override;
	virtual void Del()override;
private:
	WindDir dir_;
	std::map<WindDir, float> windAngles_;
	std::map<WindDir,Vector2> deadLines_;

	float speed_;
	float alpha_;
	float alphaTime_;
};