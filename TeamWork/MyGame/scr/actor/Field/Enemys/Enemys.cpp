#include "Enemys.h"

Enemys::Enemys(IWorld * world, int laneNum,const Vector2& pos) :
	Actor(world)
{
	position_ = pos;
	laneNum_ = laneNum;
}

void Enemys::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void Enemys::OnMessage(EventMessage message, void * param)
{
}
