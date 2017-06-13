#include "DropClothes.h"
#include "../Clothes.h"
#include "../../../../Def.h"
#include "../../../../tween/TweenManager.h"

DropClothes::DropClothes(IWorld * world, Vector2 pos, int laneNum, SPRITE_ID spriteID, int drawFrame)
	:Actor(world)
{
	parameter_.ID = ACTOR_ID::CLOTHES_DROPING_ACTOR;
	position_ = pos;
	laneNum_ = laneNum;
	spriteId_ = spriteID;

	drawFrame_ = drawFrame + 3;

	TweenManager::GetInstance().Add(EaseInQuart, &position_.y, WINDOW_HEIGHT, 3.0f, [=]() { Dead(); });
}

void DropClothes::Update()
{
}

void DropClothes::Draw() const
{
	Vector2 crcOrigin = Sprite::GetInstance().GetSplitPieceSize(spriteId_) / 2;
	Sprite::GetInstance().SplitDraw(spriteId_, drawPos_, drawFrame_, crcOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);
}