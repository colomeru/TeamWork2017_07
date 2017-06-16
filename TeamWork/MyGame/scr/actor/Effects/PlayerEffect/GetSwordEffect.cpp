#include "GetSwordEffect.h"
#include"../../player/Player_Sword.h"

GetSwordEffect::GetSwordEffect(IWorld * world, const Vector2& position,Player_Sword* target) :
	Effect(world, SPRITE_ID::GET_SWORD_EFFECT_SPRITE,3),target_(target)
{
	position_ = position;
	laneNum_ = world_->GetKeepDatas().playerLane_;
}

void GetSwordEffect::Update()
{
	laneNum_ = world_->GetKeepDatas().playerLane_;
	position_ = target_->GetSwordStartPos();
	effectMgr_.Update();
	if (effectMgr_.GetIsEnd())
	{
		parameter_.isDead = true;
	}
}

void GetSwordEffect::Draw() const
{
	if (!target_->GetUseSword())return;
	Vector2 startDP = GetDrawPosVect(target_->GetSwordStartPos());
	Vector2 endDP = GetDrawPosVect(target_->GetSwordEndPos());

	auto pos = GetDrawPosVect(position_);
	Vector2 vel = endDP - pos;
	float angle = MathAngle(vel);
	effectMgr_.DrawEffect(pos,angle+180.f);
}
