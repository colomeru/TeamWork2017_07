#include "LaneChangeWind.h"
#include"../../../../graphic/Sprite.h"
#include"../../../../game/Random.h"

LaneChangeWind::LaneChangeWind(IWorld* world, const Vector2& position, WindDir dir) :
	BackgroundCharacters(world, position),dir_(dir),speed_(Random::GetInstance().Range(1.0f, 10.0f)),alphaTime_(0),alpha_(0)

{
	windAngles_[WindDir::UP] = 90;
	windAngles_[WindDir::DOWN] = 270;
	windAngles_[WindDir::RIGHT] = 0;
	windAngles_[WindDir::LEFT] = 180;
	deadLines_[WindDir::UP] = Vector2(0.f,-1.f);
	deadLines_[WindDir::DOWN] = Vector2(0.f, 1.f);
	deadLines_[WindDir::RIGHT] = Vector2(1.f,0.f);
	deadLines_[WindDir::LEFT] = Vector2(-1.f, 0.f);
}

void LaneChangeWind::Update()
{
	position_ += deadLines_[dir_] * 30.f;
	//position_.y += 3.f;

	alphaTime_ += 0.016f * (200 + 50 * speed_);
	alpha_ = MathHelper::Sin(alphaTime_);
	alpha_ = MathHelper::Clamp(alpha_, 0.0f, 0.4f);
	if (alpha_ <= 0.f) {
		parameter_.isDead = true;
	}
}

void LaneChangeWind::CamMoveOnlyUpdate()
{
	FastUpdate();
	Update();
}

void LaneChangeWind::Draw() const
{
	//Vector2 drawPos = GetDrawPosVect(position
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::WIND_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::WIND_SPRITE, position_, origin, alpha_, Vector2::One, windAngles_.at(dir_), true, false);

	DrawFormatString(600, 600, GetColor(255, 255, 255), "da");
}


void LaneChangeWind::Del()
{
}
