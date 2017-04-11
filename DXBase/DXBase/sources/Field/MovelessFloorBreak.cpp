#include "MovelessFloorBreak.h"

MovelessFloorBreak::MovelessFloorBreak(IWorld * world, Vector2 & position) :spriteID_(-1), width_(1), height_(1),
MapChip(world, Vector2(position.x, position.y), "MovelessFloorBreak", CollisionBase(
	Vector2{ position.x ,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
	))
{
}

MovelessFloorBreak::MovelessFloorBreak(int spriteID, IWorld * world, Vector2 & position) :spriteID_(-1), width_(1), height_(1),
MapChip(world, Vector2(position.x, position.y), "MovelessFloorBreak", CollisionBase(
	Vector2{ position.x ,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
	))
{
}

MovelessFloorBreak::MovelessFloorBreak(int spriteID, IWorld * world, Vector2 & position, int width, int height) :spriteID_(-1), width_(1), height_(1),
MapChip(world, Vector2(position.x, position.y), "MovelessFloorBreak", CollisionBase(
	Vector2{ position.x ,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
	))
{
}

MovelessFloorBreak::MovelessFloorBreak(std::shared_ptr<MovelessFloorBreak> chip, IWorld * world, Vector2 & position) :spriteID_(-1), width_(1), height_(1),
MapChip(world, Vector2(position.x, position.y), "MovelessFloorBreak", CollisionBase(
	Vector2{ position.x ,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
	))
{
}

MovelessFloorBreak::MovelessFloorBreak(MovelessFloorBreak & chip, IWorld * world, Vector2 & position) :spriteID_(-1), width_(1), height_(1),
MapChip(world, Vector2(position.x, position.y), "MovelessFloorBreak", CollisionBase(
	Vector2{ position.x ,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
	))
{
}

void MovelessFloorBreak::set(Vector2 & pos)
{
	position_ = pos;
	body_ = CollisionBase(
		Vector2{ pos.x ,pos.y },
		Vector2{ pos.x + (CHIPSIZE),pos.y },
		Vector2{ pos.x ,pos.y + (CHIPSIZE) },
		Vector2{ pos.x + (CHIPSIZE),pos.y + (CHIPSIZE) });
}

void MovelessFloorBreak::onUpdate(float deltaTime)
{
}

void MovelessFloorBreak::onDraw() const
{
	spriteID_ == -1 ? body_.draw(inv_) : body_.draw(spriteID_, width_, height_, inv_);
}

void MovelessFloorBreak::onCollide(Actor & other)
{
	if (other.getName() == "Tubo") {
		dead_ = true;
	}
}
