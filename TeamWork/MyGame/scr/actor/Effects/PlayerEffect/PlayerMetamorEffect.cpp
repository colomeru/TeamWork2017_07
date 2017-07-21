#include "PlayerMetamorEffect.h"
#include"../../../time/Time.h"

PlayerMetamorEffect::PlayerMetamorEffect(IWorld * world,const Vector2& position,const std::shared_ptr<Actor>& target,float delay,const Vector2& addVec):
	Effect(world,SPRITE_ID::METAMOR_EFFECT_SPRITE,4),target_(target),delay_(delay), 
	addVec_(addVec)
{
	position_ = position+addVec;
	laneNum_ = world_->GetKeepDatas().playerLane_;
}

void PlayerMetamorEffect::Update()
{
	laneNum_ = world_->GetKeepDatas().playerLane_;
	delay_ -= Time::DeltaTime;
	if (delay_ >= 0.0f)return;
	if(!target_.expired())position_ = target_.lock()->GetPosition()+ addVec_;
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
	auto pos = GetDrawPosVect(position_);
	effectMgr_.DrawEffect(pos);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void PlayerMetamorEffect::OnMessage(EventMessage message, void * param)
{
	switch (message) {
	case EventMessage::CREATE_METAMOR_EFFECT: {
		effectCount_++;

		if (effectCount_ >= 4) {
			parameter_.isDead = true;
		}
		break;
	}

	default:
		break;
	}
}