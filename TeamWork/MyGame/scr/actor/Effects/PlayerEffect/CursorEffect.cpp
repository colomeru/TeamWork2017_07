#include "CursorEffect.h"

#include"../../../math/Vector2.h"
#include"../../../time/Time.h"

CursorEffect::CursorEffect(IWorld* world, const Vector2& position, Actor* target, Arrow arrow, float delay):
	Effect(world, SPRITE_ID::ARROW_EFFECT_SPRITE), target_(target), delay_(delay)
{
	switch (arrow)
	{
	case CursorEffect::Left: {
		addVec_ = Vector2(-150.f,0.f);
		isturn_ = false;
		break; 
	}
	case CursorEffect::Right: {
		addVec_ = Vector2(150.f, 0.f);
		isturn_ = true;
		break;
	}
	default:
		break;
	}

	position_ = position;
	laneNum_ = world_->GetKeepDatas().playerLane_;
}

void CursorEffect::Update()
{
	delay_ -= Time::DeltaTime;
	if (delay_ >= 0.0f)return;
	effectMgr_.Update();
	position_ = target_->GetPosition() + addVec_;
	if (effectMgr_.GetIsEnd())
	{
		parameter_.isDead = true;
	}
}

void CursorEffect::Draw() const
{
	if (delay_ >= 0.0f)return;
	auto pos = GetDrawPosVect(position_);
	effectMgr_.DrawEffect(pos, 0, Vector2(1.f, 1.f),isturn_);
}
