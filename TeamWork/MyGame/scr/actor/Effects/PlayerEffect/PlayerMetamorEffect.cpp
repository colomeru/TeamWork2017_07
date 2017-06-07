#include "PlayerMetamorEffect.h"

PlayerMetamorEffect::PlayerMetamorEffect(IWorld * world,const Vector2& position):
	Effect(world,SPRITE_ID::METAMOR_EFFECT_SPRITE)
{
	position_ = position;
	laneNum_ = world_->GetKeepDatas().playerLane_;
}

void PlayerMetamorEffect::Update()
{
	effectMgr_.Update();
	if (effectMgr_.GetIsEnd())
	{
		parameter_.isDead = true;
	}
}

void PlayerMetamorEffect::Draw() const
{
	effectMgr_.DrawEffect(drawPos_);
}
