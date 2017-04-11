#include "MapChip.h"

MapChip::MapChip(IWorld * world, Vector2 & position):rotate_(0),
	Actor(world, "MapChip", Vector2(position.x, position.y), CollisionBase(
		Vector2{ position.x,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
{
}
MapChip::MapChip(IWorld * world, Vector2 & position,CollisionBase& base) :
	Actor(world, "MapChip", Vector2(position.x, position.y),base)
{
	rotate_ = 0;
}

MapChip::MapChip(IWorld * world, Vector2 & position, std::string name, CollisionBase & base)
:rotate_(0),Actor(world,name, Vector2(position.x, position.y), base){
}


MapChip::MapChip(std::shared_ptr<MapChip> chip,IWorld* world, Vector2 & position)
	:Actor(world, "MapChip", Vector2(position.x, position.y), CollisionBase(
		Vector2{ position.x ,position.y },
		Vector2{ position.x - (CHIPSIZE ),position.y  },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
		))
{

}

MapChip::MapChip(MapChip& chip,IWorld* world, Vector2 & position)
	:Actor(world, "MapChip", Vector2(position.x, position.y), CollisionBase(
		Vector2{ position.x ,position.y  },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
		))
{
}

void MapChip::set(Vector2& pos)
{
	position_ = pos;
	body_ = CollisionBase(
		Vector2{ pos.x ,pos.y  },
		Vector2{ pos.x + (CHIPSIZE),pos.y  },
		Vector2{ pos.x ,pos.y + (CHIPSIZE) },
		Vector2{ pos.x + (CHIPSIZE ),pos.y + (CHIPSIZE) });
}

void MapChip::onUpdate(float deltaTime)
{
}

void MapChip::onDraw() const
{
	body_.draw(inv_);
}

void MapChip::onCollide(Actor & other)
{
}
