#include "TrackingMiniBoss.h"
#include "../../../../../World/IWorld.h"

TrackingMiniBoss::TrackingMiniBoss(
	IWorld * world, 
	const Vector2 & position, 
	const Vector2 & direction, 
	const float bodyScale) : 
	AdhereMiniBoss(world, position, bodyScale)
{
	adhereDeadTime_ = 10.0f;
	isInvincible_ = true;
}

void TrackingMiniBoss::move(float deltaTime)
{
	auto player = world_->findActor("PlayerBody1");
	auto distance = player->getPosition() - position_;
	auto dir = distance.Normalize();
	position_ += speed_ * dir * (deltaTime * 60.0f);

	addTexPos_ = Vector2::Up * 10.0f;
	animation_.changeDirType(dir.x);
}
