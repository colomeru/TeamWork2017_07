#include "SticklessFloor.h"
#include"../Input/InputMgr.h"

SticklessFloor::SticklessFloor(IWorld * world, Vector2 & position) :spriteID_(-1),
MapChip(world, Vector2(position.x, position.y), "SticklessFloor", CollisionBase(
	Vector2{ position.x,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x ,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
{
	rotate_ = 0;
}

SticklessFloor::SticklessFloor(int spriteID, IWorld * world, Vector2 & position) :spriteID_(spriteID),
MapChip(world, Vector2(position.x, position.y), "SticklessFloor", CollisionBase(
	Vector2{ position.x,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x ,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
{
	width_ = (1); height_ = (1);
	rotate_ = 0;
}

SticklessFloor::SticklessFloor(int spriteID, IWorld * world, Vector2 & position, int width, int height) :spriteID_(spriteID), 
MapChip(world, Vector2(position.x, position.y), "SticklessFloor", CollisionBase(
	Vector2{ position.x,position.y },
	Vector2{ position.x - (CHIPSIZE*width),position.y },
	Vector2{ position.x ,position.y - (CHIPSIZE*height) },
	Vector2{ position.x - (CHIPSIZE*width),position.y - (CHIPSIZE*height) }))
{
	width_ = width; height_ = height;
	rotate_ = 0;
}


SticklessFloor::SticklessFloor(std::shared_ptr<SticklessFloor> chip, IWorld * world, Vector2 & position) :spriteID_(-1),
MapChip(world, Vector2(position.x, position.y), "SticklessFloor", CollisionBase(
	Vector2{ position.x ,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x ,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
	))
{
	width_ = (1); height_ = (1);
	rotate_ = 0;
}

SticklessFloor::SticklessFloor(SticklessFloor & chip, IWorld * world, Vector2 & position) :spriteID_(-1),
MapChip(world, Vector2(position.x, position.y), "SticklessFloor", CollisionBase(
	Vector2{ position.x ,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
	))
{
	width_ = (1); height_ = (1);
	rotate_ = 0;
}

void SticklessFloor::set(Vector2 & pos)
{
	position_ = pos;
	body_ = CollisionBase(
		Vector2{ pos.x ,pos.y },
		Vector2{ pos.x + (CHIPSIZE),pos.y },
		Vector2{ pos.x ,pos.y + (CHIPSIZE) },
		Vector2{ pos.x + (CHIPSIZE),pos.y + (CHIPSIZE) });
}

void SticklessFloor::onUpdate(float deltaTime)
{

}

void SticklessFloor::onDraw() const
{
	//if (isOutCamera())return;

	spriteID_ == -1 ? body_.draw(inv_) : body_.draw(spriteID_,width_,height_, inv_);
}

void SticklessFloor::onCollide(Actor & other)
{
}
