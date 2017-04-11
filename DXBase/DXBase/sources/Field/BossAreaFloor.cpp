#include "BossAreaFloor.h"
#include"../Input/InputMgr.h"

BossAreaFloor::BossAreaFloor(IWorld * world, Vector2 & position) :spriteID_(-1),
MapChip(world, Vector2(position.x, position.y), "BossAreaFloor", CollisionBase(
	Vector2{ position.x,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x ,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
{
	rotate_ = 0;
}

BossAreaFloor::BossAreaFloor(int spriteID, IWorld * world, Vector2 & position) :spriteID_(spriteID),
MapChip(world, Vector2(position.x, position.y), "BossAreaFloor", CollisionBase(
	Vector2{ position.x,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x ,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
{
	rotate_ = 0;
}
BossAreaFloor::BossAreaFloor(int spriteID, IWorld * world, Vector2 & position,int width,int height) :spriteID_(spriteID),
MapChip(world, Vector2(position.x, position.y), "BossAreaFloor", CollisionBase(
	Vector2{ position.x,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x ,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
{
	width_ = width;
	height_ = height;
	rotate_ = 0;
}

BossAreaFloor::BossAreaFloor(std::shared_ptr<BossAreaFloor> chip, IWorld * world, Vector2 & position) :spriteID_(-1),
MapChip(world, Vector2(position.x, position.y), "BossAreaFloor", CollisionBase(
	Vector2{ position.x ,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x ,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
	))
{
	rotate_ = 0;
}

BossAreaFloor::BossAreaFloor(BossAreaFloor & chip, IWorld * world, Vector2 & position) :spriteID_(-1),
MapChip(world, Vector2(position.x, position.y), "BossAreaFloor", CollisionBase(
	Vector2{ position.x ,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
	))
{
	rotate_ = 0;
}

void BossAreaFloor::set(Vector2 & pos)
{
	position_ = pos;
	body_ = CollisionBase(
		Vector2{ pos.x ,pos.y },
		Vector2{ pos.x + (CHIPSIZE),pos.y },
		Vector2{ pos.x ,pos.y + (CHIPSIZE) },
		Vector2{ pos.x + (CHIPSIZE),pos.y + (CHIPSIZE) });
}

void BossAreaFloor::onUpdate(float deltaTime)
{

}

void BossAreaFloor::onDraw() const
{
	spriteID_ == -1 ? body_.draw(inv_) : body_.draw(spriteID_, width_,height_, inv_);
}

void BossAreaFloor::onCollide(Actor & other)
{
}
