#include "PlayerFatigueEffect.h"
#include"../../../time/Time.h"

PlayerFatigueEffect::PlayerFatigueEffect(IWorld * world, const Vector2& position, Actor* target, float delay) :
	Effect(world, SPRITE_ID::FATIGUE_EFFECT_SPRITE), target_(target), delay_(delay)
{
	position_ = position+Vector2(100.f,0.f);
	laneNum_ = world_->GetKeepDatas().playerLane_;
}

void PlayerFatigueEffect::Update()
{
	laneNum_ = world_->GetKeepDatas().playerLane_;
	delay_ -= Time::DeltaTime;
	if (delay_ >= 0.0f)return;
	position_ = target_->GetPosition() + Vector2(100.f, 0.f);
	effectMgr_.Update();
	if (effectMgr_.GetIsEnd())
	{
		parameter_.isDead = true;
	}
}

void PlayerFatigueEffect::Draw() const
{
	if (delay_ >= 0.0f)return;
	auto pos = GetDrawPosVect(position_);
	effectMgr_.DrawEffect(pos);
}
