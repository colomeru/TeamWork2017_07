#include "BackgroundPill.h"
#include"../../../../graphic/Sprite.h"

BackgroundPill::BackgroundPill(IWorld* world, const Vector2& position):
	BackgroundCharacters(world,position)
{
	isDead_ = false;
}

void BackgroundPill::Update()
{
	position_.x -= 30.f;
	//position_.y += 3.f;

	if (position_.x <= -400.f) {
		isDead_ = true;
	}
}

void BackgroundPill::Draw() const
{
	Vector2 drawPos = GetDrawPosVect(position_);
	Sprite::GetInstance().Draw(SPRITE_ID::WIND_SPRITE, drawPos);
}

void BackgroundPill::Del()
{
}
