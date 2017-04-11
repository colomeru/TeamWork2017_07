#include "ItemSpawnFloor.h"
#include "../Input/InputMgr.h"
#include "../Actor/Item/Items.h"
#include "../Actor/Base/ActorGroup.h"

ItemSpawnFloor::ItemSpawnFloor(IWorld * world, Vector2 & position) :spriteID_(-1), width_(1), height_(1), isHit_(false),
MapChip(world, Vector2(position.x, position.y), "MovelessFloor", CollisionBase(
	Vector2{ position.x,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x ,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
{
	rotate_ = 0;
}

ItemSpawnFloor::ItemSpawnFloor(int spriteID, IWorld * world, Vector2 & position) :spriteID_(spriteID), width_(1), height_(1), isHit_(false),
MapChip(world, Vector2(position.x, position.y), "MovelessFloor", CollisionBase(
	Vector2{ position.x,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x ,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
{
	rotate_ = 0;
}

ItemSpawnFloor::ItemSpawnFloor(int spriteID, IWorld * world, Vector2 & position, int width, int height) :spriteID_(spriteID), width_(width), height_(height),
MapChip(world, Vector2(position.x, position.y), "MovelessFloor", CollisionBase(
	Vector2{ position.x,position.y },
	Vector2{ position.x - (CHIPSIZE*width),position.y },
	Vector2{ position.x ,position.y - (CHIPSIZE*height) },
	Vector2{ position.x - (CHIPSIZE*width),position.y - (CHIPSIZE*height) }))
{
	rotate_ = 0;
}

ItemSpawnFloor::ItemSpawnFloor(std::shared_ptr<ItemSpawnFloor> chip, IWorld * world, Vector2 & position) :spriteID_(-1), width_(1), height_(1),
MapChip(world, Vector2(position.x, position.y), "MovelessFloor", CollisionBase(
	Vector2{ position.x ,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x ,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
	))
{
	rotate_ = 0;
}

ItemSpawnFloor::ItemSpawnFloor(ItemSpawnFloor & chip, IWorld * world, Vector2 & position) :spriteID_(-1), width_(1), height_(1),
MapChip(world, Vector2(position.x, position.y), "MovelessFloor", CollisionBase(
	Vector2{ position.x ,position.y },
	Vector2{ position.x - (CHIPSIZE),position.y },
	Vector2{ position.x,position.y - (CHIPSIZE) },
	Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
	))
{
	rotate_ = 0;
}

void ItemSpawnFloor::set(Vector2 & pos)
{
	position_ = pos;
	body_ = CollisionBase(
		Vector2{ pos.x ,pos.y },
		Vector2{ pos.x + (CHIPSIZE),pos.y },
		Vector2{ pos.x ,pos.y + (CHIPSIZE) },
		Vector2{ pos.x + (CHIPSIZE),pos.y + (CHIPSIZE) });
}

void ItemSpawnFloor::onUpdate(float deltaTime)
{


}

void ItemSpawnFloor::onDraw() const
{
	//if (isOutCamera())return;
	//if (isHit_)return;

	spriteID_ == -1 ? body_.draw(inv_) : body_.draw(spriteID_, width_, height_, inv_);
}

void ItemSpawnFloor::onCollide(Actor & other)
{
	if (isHit_)return;

	//if (other.getName().find("Player") != std::string::npos) {
	if(other.getName()== "BodyPoint"){
		world_->addActor(ActorGroup::Item, std::make_shared<Items>(
			world_, Vector2(position_.x + 48, position_.y - 96)));
		isHit_ = true;
		//dead();
	}
}
