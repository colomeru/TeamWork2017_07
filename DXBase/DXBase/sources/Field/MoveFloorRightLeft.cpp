#include "MoveFloorRightLeft.h"

MoveFloorRightLeft::MoveFloorRightLeft(IWorld * world, Vector2 & position) :spriteID_(-1),
	MapChip(world, Vector2(position.x, position.y), "MoveFloorRightLeft", CollisionBase(
		Vector2{ position.x,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
	, defaultPos_(position), moveVelocity(1), moveCount_(CHIPSIZE*RIGHTLEFTRANGE)
{
	rotate_ = 0;
}
MoveFloorRightLeft::MoveFloorRightLeft(int spriteID,IWorld * world, Vector2 & position) :spriteID_(spriteID),
	MapChip(world, Vector2(position.x, position.y), "MoveFloorRightLeft", CollisionBase(
		Vector2{ position.x,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
	, defaultPos_(position), moveVelocity(1), moveCount_(CHIPSIZE*RIGHTLEFTRANGE)
{
}

MoveFloorRightLeft::MoveFloorRightLeft(std::shared_ptr<MoveFloorRightLeft> chip, IWorld * world, Vector2 & position) :
	MapChip(world, Vector2(position.x, position.y), "MoveFloorRightLeft", CollisionBase(
		Vector2{ position.x ,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
		))
	, defaultPos_(position), moveVelocity(1), moveCount_(CHIPSIZE*RIGHTLEFTRANGE)
{
}

MoveFloorRightLeft::MoveFloorRightLeft(MoveFloorRightLeft & chip, IWorld * world, Vector2 & position) :
	MapChip(world, Vector2(position.x, position.y), "MoveFloorRightLeft", CollisionBase(
		Vector2{ position.x ,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
		))
	, defaultPos_(position), moveVelocity(1), moveCount_(CHIPSIZE*RIGHTLEFTRANGE)
{
}

void MoveFloorRightLeft::set(Vector2 & pos)
{
	position_ = pos;
	defaultPos_ = pos;
	body_ = CollisionBase(
		Vector2{ pos.x ,pos.y },
		Vector2{ pos.x + (CHIPSIZE),pos.y },
		Vector2{ pos.x ,pos.y + (CHIPSIZE) },
		Vector2{ pos.x + (CHIPSIZE),pos.y + (CHIPSIZE) });
}

void MoveFloorRightLeft::onUpdate(float deltaTime)
{
	moveCount_ += moveVelocity*ceilf(deltaTime);
	if (moveCount_ <= 0) {
		moveVelocity = LRMoveSpeed;
	}
	if (moveCount_ >= CHIPSIZE*RIGHTLEFTRANGE) {
		moveVelocity = -LRMoveSpeed;
	}
	position_.x += moveVelocity*ceilf(deltaTime);
}

void MoveFloorRightLeft::onDraw() const
{
	spriteID_ == -1 ? body_.draw(inv_) : body_.draw(spriteID_, rotate_, inv_);
}

void MoveFloorRightLeft::onCollide(Actor & other)
{
}
