#include "BirdsDropping.h"

BirdsDropping::BirdsDropping(IWorld * world, int laneNum, Vector2 pos):
	Enemys(world,laneNum,pos)
{
}

BirdsDropping::~BirdsDropping()
{
}

void BirdsDropping::Update()
{
	position_.y += 20;
}

void BirdsDropping::Draw() const
{
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRDS_DROPPING_SPRITE);
	Sprite::GetInstance().Draw(SPRITE_ID::BIRDS_DROPPING_SPRITE, drawPos_, origin, spriteAlpha_, Vector2::One, 0);
}

void BirdsDropping::OnUpdate()
{
}

void BirdsDropping::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void BirdsDropping::OnMessage(EventMessage message, void * param)
{
}
