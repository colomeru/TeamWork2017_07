#include "EnemyCreateBox.h"

EnemyCreateBox::EnemyCreateBox(
	IWorld * world, 
	const Vector2 & position, 
	float direction) :
	Actor(world, "EnemyCreateBox", position, CollisionBase())
{}

EnemyCreateBox::EnemyCreateBox(
	IWorld * world,
	const Vector2 & position,
	const Vector2 & direction) :
	Actor(world, "EnemyCreateBox", position, CollisionBase())
{
}

void EnemyCreateBox::onUpdate(float deltaTime)
{
}

void EnemyCreateBox::onDraw() const{}

void EnemyCreateBox::onCollide(Actor & actor){}

void EnemyCreateBox::onMessage(EventMessage event, void *){}
