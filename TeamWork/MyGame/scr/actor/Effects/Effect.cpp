#include "Effect.h"

Effect::Effect(IWorld* world, SPRITE_ID id, int anmSpeed):
	Actor(world),effectMgr_(id, anmSpeed)
{
	
}

void Effect::Update()
{
}

void Effect::Draw() const
{
}
