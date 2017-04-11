#include "NavChip.h"

NavChip::NavChip(IWorld * world, Vector2 & position) :
	MapChip(world, Vector2(position.x, position.y), "NavChip", CollisionBase(
		Vector2{ position.x,position.y },
		Vector2{ position.x - (CHIPSIZE * 2),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE * 2),position.y - (CHIPSIZE) })){
}

NavChip::NavChip(std::shared_ptr<NavChip> chip, IWorld * world, Vector2 & position) :
	MapChip(world, Vector2(position.x, position.y), "NavChip", CollisionBase(
		Vector2{ position.x,position.y },
		Vector2{ position.x - (CHIPSIZE * 2),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE * 2),position.y - (CHIPSIZE) })) {
}

NavChip::NavChip(NavChip & chip, IWorld * world, Vector2 & position) :
	MapChip(world, Vector2(position.x, position.y), "NavChip", CollisionBase(
		Vector2{ position.x,position.y },
		Vector2{ position.x - (CHIPSIZE * 2),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE * 2),position.y - (CHIPSIZE) })) {
}

void NavChip::set(Vector2 & pos){
	body_ = CollisionBase(
		Vector2{ pos.x ,pos.y },
		Vector2{ pos.x + (CHIPSIZE),pos.y },
		Vector2{ pos.x ,pos.y + (CHIPSIZE) },
		Vector2{ pos.x + (CHIPSIZE),pos.y + (CHIPSIZE) });
}

void NavChip::onUpdate(float deltaTime)
{
}

void NavChip::onDraw() const
{
	//body_.draw();
}

void NavChip::onCollide(Actor & other)
{
}
