#include "DropClothes.h"
#include "../../../../tween/TweenManager.h"
#include "../../../../game/Random.h"
#include "../../../../graphic/Sprite.h"

DropClothes::DropClothes(IWorld * world, Vector2 pos, int laneNum, SPRITE_ID spriteID, int drawFrame, float angle)
	:Actor(world)
{
	parameter_.ID = ACTOR_ID::CLOTHES_DROPING_ACTOR;
	position_ = pos;
	laneNum_ = laneNum;
	spriteId_ = spriteID;
	angle_ = angle;

	drawFrame_ = drawFrame + 3;
	if (drawFrame_ == 4)
		turn_ = 180.0f;
	else if (drawFrame_ == 5)
		turn_ = -180.0f;

	float rand = Random::GetInstance().Range(3.0f, 4.0f);
	TweenManager::GetInstance().Add(EaseInCubic, &position_.y, WINDOW_HEIGHT, 3.0f, [=]() { Dead(); });
	TweenManager::GetInstance().Add(EaseInCubic, &angle_, turn_, rand);
}

DropClothes::~DropClothes()
{
}

void DropClothes::Update()
{
}

void DropClothes::Draw() const
{
	auto drawPos = GetDrawPosVect(position_);
	Vector2 crcOrigin = Sprite::GetInstance().GetSplitPieceSize(spriteId_) / 2;
	Sprite::GetInstance().SplitDraw(spriteId_, drawPos, drawFrame_, crcOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);
}