#include "ResurrectStayEffect.h"
#include"../../../math/Vector2.h"

ResurrectStayEffect::ResurrectStayEffect(IWorld * world, const Vector2& position) :
	Effect(world, SPRITE_ID::RESURRECT_EFFECT_SPRITE, 1)
{

	position_ = position;
	laneNum_ = world_->GetKeepDatas().playerLane_;
}

void ResurrectStayEffect::Update()
{
	effectMgr_.Update();
	if (effectMgr_.GetIsEnd())
	{
		parameter_.isDead = true;
	}
}

void ResurrectStayEffect::Draw() const
{
	auto pos = GetDrawPosVect(position_);
	effectMgr_.DrawEffect(pos, 0, Vector2(2.f, 2.f));
}
