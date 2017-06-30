#include "DummyActor.h"

DummyActor::DummyActor(IWorld * world):
	Actor(world)
{
}

DummyActor::~DummyActor()
{
}

void DummyActor::Update()
{
}

void DummyActor::FastUpdate()
{
}

void DummyActor::Draw() const
{
}

void DummyActor::OnUpdate()
{
}

void DummyActor::OnCollide(Actor &, CollisionParameter colpara)
{
}

void DummyActor::OnMessage(EventMessage message, void * param)
{
}

bool DummyActor::CamMoveUpdate()
{
	return false;
}

void DummyActor::CamMoveUp()
{
}

void DummyActor::CamMoveDown()
{
}

void DummyActor::LaneChangeFall()
{
}
