#include "BackgroundPill.h"
#include"../../../../graphic/Sprite.h"

BackgroundPill::BackgroundPill(IWorld* world, const Vector2& position):
	BackgroundCharacters(world,position)
{
}

void BackgroundPill::Update()
{
	position_.x -= 30.f;
	//position_.y += 3.f;

	if (position_.x <= -400.f) {
		parameter_.isDead = true;
	}
}

void BackgroundPill::Draw() const
{
	//Vector2 drawPos = GetDrawPosVect(position
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::WIND_SPRITE)/2;
	Sprite::GetInstance().Draw(SPRITE_ID::WIND_SPRITE, drawPos_, origin, 1.f, Vector2::One, 0.f,true, false);

}


void BackgroundPill::Del()
{
}
