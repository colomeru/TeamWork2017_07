#include "FlyingMiniBoss.h"
#include "ImportAnimationNumber.h"

FlyingMiniBoss::FlyingMiniBoss(
	IWorld * world,
	const Vector2 & position,
	const Vector2 & direction,
	const float bodyScale) :
	AdhereMiniBoss(world, position, bodyScale),
	deg_(0.0f)
{
	direction_ = direction;
	speed_ = 6.0f;
}

void FlyingMiniBoss::move(float deltaTime)
{
	// ˆÚ“®
	position_ += speed_ * prevPlayerDirection_ * (deltaTime * 60.0f);

	addTexPos_ = Vector2::Up * 10.0f;
	animation_.changeDirType(prevPlayerDirection_.x);
}

// °‚É“–‚½‚Á‚½‚Ìˆ—
void FlyingMiniBoss::floorHit()
{
	if (state_ == State::Attack || state_ == State::Dead) return;
	changeDeadState(DeadState::HitDead, WAIT_NUMBER);
}

// ƒvƒŒƒCƒ„[‚ÌUŒ‚‚É“–‚½‚Á‚½‚Ìˆ—
void FlyingMiniBoss::playerAttackHit(Actor & actor)
{
	changeDeadState(DeadState::HitDead, DEAD_NUMBER);
	body_.enabled(false);
}
