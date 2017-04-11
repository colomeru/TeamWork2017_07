#include "TurnFloor.h"

TurnFloor::TurnFloor(IWorld * world, Vector2 & position) :spriteID_(-1),
	MapChip(world, Vector2(position.x, position.y), "TurnFloor", CollisionBase(
		Vector2{ position.x,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
	, defaultPos_(position), moveVelocity(1), moveCount_(static_cast<int>(CHIPSIZE*TURNRANGE))
{
}
TurnFloor::TurnFloor(int spriteID,IWorld * world, Vector2 & position) :spriteID_(spriteID),
	MapChip(world, Vector2(position.x, position.y), "TurnFloor", CollisionBase(
		Vector2{ position.x,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
	, defaultPos_(position), moveVelocity(1), moveCount_(static_cast<int>(CHIPSIZE*TURNRANGE))
{
}

TurnFloor::TurnFloor(std::shared_ptr<TurnFloor> chip, IWorld * world, Vector2 & position) :
	MapChip(world, Vector2(position.x, position.y), "TurnFloor", CollisionBase(
		Vector2{ position.x ,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
		))
	, defaultPos_(position), moveVelocity(1), moveCount_(static_cast<int>(CHIPSIZE*TURNRANGE))
{
}

TurnFloor::TurnFloor(TurnFloor & chip, IWorld * world, Vector2 & position) :
	MapChip(world, Vector2(position.x, position.y), "TurnFloor", CollisionBase(
		Vector2{ position.x ,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
		))
	, defaultPos_(position), moveVelocity(1), moveCount_(static_cast<int>(CHIPSIZE*TURNRANGE))
{
}

void TurnFloor::set(Vector2 & pos)
{
	position_ = pos;
	defaultPos_ = pos;
	body_ = CollisionBase(
		Vector2{ pos.x ,pos.y },
		Vector2{ pos.x + (CHIPSIZE),pos.y },
		Vector2{ pos.x ,pos.y + (CHIPSIZE) },
		Vector2{ pos.x + (CHIPSIZE),pos.y + (CHIPSIZE) });
}

void TurnFloor::onUpdate(float deltaTime)
{
	defaultPos_ = position_- (moveVelocity*CHIPSIZE*TURNRANGE)+Vector2(CHIPSIZE*2,CHIPSIZE*2);

	moveCount_+=1* static_cast<int>((ceilf(deltaTime)));
	if (moveCount_ % 360 == 0) {
		moveCount_ = 0;
	}
	
	moveVelocity.x = cos(moveCount_*MathHelper::Pi/180);
	moveVelocity.y = sin(moveCount_*MathHelper::Pi / 180);

	position_ = defaultPos_ + (moveVelocity*CHIPSIZE*TURNRANGE)- Vector2(CHIPSIZE * 2, CHIPSIZE * 2);

}

void TurnFloor::onDraw() const
{
	spriteID_ == -1 ? body_.draw(inv_) : body_.draw(spriteID_, rotate_, inv_);
}

void TurnFloor::onCollide(Actor & other)
{
}
