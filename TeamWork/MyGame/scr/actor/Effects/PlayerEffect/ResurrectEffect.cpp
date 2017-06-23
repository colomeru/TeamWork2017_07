#include "ResurrectEffect.h"
#include"../../../math/Vector2.h"

ResurrectEffect::ResurrectEffect(IWorld * world, const Vector2& position, Actor* target) :
	Effect(world, SPRITE_ID::RESURRECT_EFFECT_SPRITE,1),target_(target),addVec_(target->GetPosition()- position)
{
	addVec_ = addVec_.Normalize()*15;

	position_ = position;
	laneNum_ = world_->GetKeepDatas().playerLane_;
}

void ResurrectEffect::Update()
{
	effectMgr_.Update();
	position_ = target_->GetPosition()+addVec_;
	if (effectMgr_.GetIsEnd())
	{
		parameter_.isDead = true;
	}
}

void ResurrectEffect::Draw() const
{
	auto pos = GetDrawPosVect(position_);
	effectMgr_.DrawEffect(pos,0,Vector2(2.f,2.f));
}
