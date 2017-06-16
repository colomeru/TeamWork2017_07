#include "TapperTapEffect.h"

TapperTapEffect::TapperTapEffect(IWorld * world, const Vector2& position) :
	Effect(world, SPRITE_ID::TAP_EFFECT_SPRITE,2)
{
	position_ = position;
	laneNum_ = world_->GetKeepDatas().playerLane_;
}

void TapperTapEffect::Update()
{
	effectMgr_.Update();
	if (effectMgr_.GetIsEnd())
	{
		parameter_.isDead = true;
	}
}

void TapperTapEffect::Draw() const
{
	auto pos = GetDrawPosVect(position_);
	effectMgr_.DrawEffect(pos);
}
