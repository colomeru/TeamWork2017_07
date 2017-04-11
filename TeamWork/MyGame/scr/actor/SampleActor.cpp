#include "SampleActor.h"

SampleActor::SampleActor(IWorld * world) :
Actor(world)
{
}

SampleActor::~SampleActor()
{
}

void SampleActor::Update()
{
}

void SampleActor::Draw() const
{
	DrawFormatString(0, 100, GetColor(255, 0, 0), "POP_ACTOR");
}

void SampleActor::OnUpdate()
{
}

void SampleActor::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void SampleActor::OnMessage(EventMessage message, void * param)
{
}
