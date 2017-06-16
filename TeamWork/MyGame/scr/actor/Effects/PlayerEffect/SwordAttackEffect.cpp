#include "SwordAttackEffect.h"

SwordAttackEffect::SwordAttackEffect(IWorld * world, const Vector2& position) :
	Effect(world, SPRITE_ID::SWORD_ATTACK_EFFECT_SPRITE,1)
{
	position_ = position;
	laneNum_ = world_->GetKeepDatas().playerLane_;
}

void SwordAttackEffect::Update()
{
	effectMgr_.Update();
	if (effectMgr_.GetIsEnd())
	{
		parameter_.isDead = true;
	}
}

void SwordAttackEffect::Draw() const
{
	auto pos = GetDrawPosVect(position_);
	effectMgr_.DrawEffect(pos);
}
