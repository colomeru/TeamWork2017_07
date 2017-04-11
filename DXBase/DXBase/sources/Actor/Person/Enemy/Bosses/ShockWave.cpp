#include "ShockWave.h"
#include "../../../../World/IWorld.h"

ShockWave::ShockWave(
	IWorld * world,
	const Vector2 & position,
	const Vector2 & bodyScale) : 
	Actor(world, "BaseEnemy", position,
		CollisionBase(
			Vector2(position.x + bodyScale.x / 2.0f, position.y + bodyScale.y / 2.0f),
			Vector2(position.x - bodyScale.x / 2.0f, position.y + bodyScale.y / 2.0f),
			Vector2(position.x + bodyScale.x / 2.0f, position.y - bodyScale.y / 2.0f),
			Vector2(position.x - bodyScale.x / 2.0f, position.y - bodyScale.y / 2.0f)
			)
		)
{}

void ShockWave::onUpdate(float deltaTime)
{
	if (timer_ >= 1.0f) dead();
	timer_ += deltaTime;
}

void ShockWave::onMessage(EventMessage event, void *){}
