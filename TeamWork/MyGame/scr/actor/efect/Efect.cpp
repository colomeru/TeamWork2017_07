#include "Efect.h"

Efect::Efect(IWorld * world)
	:Actor(world)
{
}

Efect::~Efect()
{
}

void Efect::Update()
{
}

void Efect::Draw() const
{
}

void Efect::OnUpdate()
{
}

void Efect::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void Efect::OnMessage(EventMessage message, void * param)
{
}
