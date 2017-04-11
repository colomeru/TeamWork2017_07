#include "GameOverPoint.h"
#include"../Input/InputMgr.h"

GameOverPoint::GameOverPoint(IWorld * world, Vector2 & position) :spriteID_(-1),
	MapChip(world, Vector2(position.x, position.y), "GameOverPoint", CollisionBase(
		Vector2{ position.x,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
{
}
GameOverPoint::GameOverPoint(int spriteID,IWorld * world, Vector2 & position) :spriteID_(spriteID),
	MapChip(world, Vector2(position.x, position.y), "GameOverPoint", CollisionBase(
		Vector2{ position.x,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }))
{
}

GameOverPoint::GameOverPoint(std::shared_ptr<GameOverPoint> chip, IWorld * world, Vector2 & position) :
	MapChip(world, Vector2(position.x, position.y), "GameOverPoint", CollisionBase(
		Vector2{ position.x ,position.y - (CHIPSIZE * 5) },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
		))
{
}

GameOverPoint::GameOverPoint(GameOverPoint & chip, IWorld * world, Vector2 & position) :
	MapChip(world, Vector2(position.x, position.y), "GameOverPoint", CollisionBase(
		Vector2{ position.x ,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
		))
{
}

void GameOverPoint::set(Vector2 & pos)
{
	position_ = pos;
	body_ = CollisionBase(
		Vector2{ pos.x ,pos.y },
		Vector2{ pos.x + (CHIPSIZE),pos.y },
		Vector2{ pos.x ,pos.y + (CHIPSIZE) },
		Vector2{ pos.x + (CHIPSIZE),pos.y + (CHIPSIZE) });
}

void GameOverPoint::onUpdate(float deltaTime)
{
}

void GameOverPoint::onDraw() const
{
	body_.draw(ResourceLoader::GetInstance().getTextureID(TextureID::CHIP1_TEX), 0, inv_);
}

void GameOverPoint::onCollide(Actor & other)
{
}
