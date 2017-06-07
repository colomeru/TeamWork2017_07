#include "PlayerBiteEffect.h"

PlayerBiteEffect::PlayerBiteEffect(IWorld * world, const Vector2& position) :
	Effect(world, SPRITE_ID::BITE_EFFECT_SPRITE)
{
	position_ = position;
	laneNum_ = world_->GetKeepDatas().playerLane_;
}

void PlayerBiteEffect::Update()
{
	effectMgr_.Update();
	if (effectMgr_.GetIsEnd())
	{
		parameter_.isDead = true;
	}
}

void PlayerBiteEffect::Draw() const
{
	effectMgr_.DrawEffect(drawPos_);
}
