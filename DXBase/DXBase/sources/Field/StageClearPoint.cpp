#include "StageClearPoint.h"
#include"../Input/InputMgr.h"

StageClearPoint::StageClearPoint(IWorld * world, Vector2 & position) :spriteID_(-1),
	MapChip(world, Vector2(position.x, position.y), "StageClearPoint", CollisionBase(
		Vector2{ position.x,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
{
}
StageClearPoint::StageClearPoint(int spriteID,IWorld * world, Vector2 & position) :spriteID_(spriteID),
	MapChip(world, Vector2(position.x, position.y), "StageClearPoint", CollisionBase(
		Vector2{ position.x,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
{
}

StageClearPoint::StageClearPoint(std::shared_ptr<StageClearPoint> chip, IWorld * world, Vector2 & position) :
	MapChip(world, Vector2(position.x, position.y), "StageClearPoint", CollisionBase(
		Vector2{ position.x ,position.y - (CHIPSIZE * 5) },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
		))
{
}

StageClearPoint::StageClearPoint(StageClearPoint & chip, IWorld * world, Vector2 & position) :
	MapChip(world, Vector2(position.x, position.y), "StageClearPoint", CollisionBase(
		Vector2{ position.x ,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
		))
{
}

void StageClearPoint::set(Vector2 & pos)
{
	position_ = pos;
	body_ = CollisionBase(
		Vector2{ pos.x ,pos.y },
		Vector2{ pos.x + (CHIPSIZE),pos.y },
		Vector2{ pos.x ,pos.y + (CHIPSIZE) },
		Vector2{ pos.x + (CHIPSIZE),pos.y + (CHIPSIZE) });
}

void StageClearPoint::onUpdate(float deltaTime)
{
}

void StageClearPoint::onDraw() const
{
	body_.draw(ResourceLoader::GetInstance().getTextureID(TextureID::GOAL_TEX),0,inv_);
}

void StageClearPoint::onCollide(Actor & other)
{
}
