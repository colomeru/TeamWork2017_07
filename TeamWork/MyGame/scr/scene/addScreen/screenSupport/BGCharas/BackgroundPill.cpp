#include "BackgroundPill.h"
#include"../../../../graphic/Sprite.h"
#include"../../../../tween/TweenManager.h"
#include"../../../../game/Random.h"

BackgroundPill::BackgroundPill(IWorld* world, const Vector2& position):
	BackgroundCharacters(world,position)
{
	position_ = position;
	Vector2 toPos = Vector2(world->GetKeepDatas().playerPos_.x-Random::GetInstance().Range(2000, 3000), position.y);
	float time = Random::GetInstance().Range(0.5f, 3.f);
	EaseType type = EaseType::EaseInCirc;
	TweenManager::GetInstance().Add(type, &position_, toPos,time, [this](){parameter_.isDead = true; });
}

void BackgroundPill::Update()
{
	//position_.x -= 30.f;
	//position_.y += 3.f;

	//Vector2 pPos = world_->GetKeepDatas().playerPos_;
	//if (position_.x <= pPos.x-400.f) {
	//	parameter_.isDead = true;
	//}
}

void BackgroundPill::Draw() const
{

	//Vector2 drawPos = GetDrawPosVect(position
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::WIND_SPRITE)/2;
	auto pos = GetDrawPosVect(position_);
	Sprite::GetInstance().Draw(SPRITE_ID::WIND_SPRITE, pos, origin, 1.f, Vector2::One, 0.f,true, false);

}


void BackgroundPill::Del()
{
}
