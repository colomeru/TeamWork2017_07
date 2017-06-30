#include "TutorialFlash.h"
#include "../../../../tween/TweenManager.h"

TutorialFlash::TutorialFlash(IWorld * world, int laneNum, Vector2 pos, Actor * clothes)
	:Actor(world, clothes)
{
	laneNum_ = laneNum;
	position_ = pos;
	parameter_.spriteAlpha_ = 0.0f;

	TweenManager::GetInstance().Loop(Linear, &parameter_.spriteAlpha_, 2.0f);
}

TutorialFlash::~TutorialFlash()
{
	TweenManager::GetInstance().Cancel(&parameter_.spriteAlpha_);
}

void TutorialFlash::Update()
{
}

void TutorialFlash::Draw() const
{
	Sprite& instance = Sprite::GetInstance();
	auto pos = GetDrawPosVect(position_ - Vector2(0, 250) + parent_->GetDrawAddPos());
	Vector2 goalSize = instance.GetSize(SPRITE_ID::GOAL_CLOTHES_SPRITE);
	Vector2 origin = instance.GetSize(SPRITE_ID::BITE_RANGE_SPRITE) / 2;

	instance.Draw(SPRITE_ID::BITE_RANGE_SPRITE, pos, origin, parameter_.spriteAlpha_, goalSize / 32);
}
