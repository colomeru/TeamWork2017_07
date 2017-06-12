#include "PlayerMetamorEffect.h"
#include"../../../time/Time.h"

PlayerMetamorEffect::PlayerMetamorEffect(IWorld * world,const Vector2& position,Actor* target,float delay,const Vector2& addVec):
	Effect(world,SPRITE_ID::METAMOR_EFFECT_SPRITE,4),target_(target),delay_(delay)
{
	position_ = position+addVec_;
	addVec_ = addVec;
	laneNum_ = world_->GetKeepDatas().playerLane_;
}

void PlayerMetamorEffect::Update()
{
	laneNum_ = world_->GetKeepDatas().playerLane_;
	delay_ -= Time::DeltaTime;
	if (delay_ >= 0.0f)return;
	position_ = target_->GetPosition()+ addVec_;
	effectMgr_.Update();
	if (effectMgr_.GetIsEnd())
	{
		parameter_.isDead = true;
	}
}

void PlayerMetamorEffect::Draw() const
{
	if (delay_ >= 0.0f)return;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	effectMgr_.DrawEffect(drawPos_);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
