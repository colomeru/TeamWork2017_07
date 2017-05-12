#include "BackgroundScreen.h"
#include"../../world/World.h"
#include"../../graphic/Sprite.h"

BackgroundScreen::BackgroundScreen(IWorld * world) :world_(world), drawPos_(Vector2::Zero)
{
}

void BackgroundScreen::Init()
{
	drawPos_.y=world_->GetKeepDatas().playerPos_.y;
}

void BackgroundScreen::Update()
{
	drawPos_.y = world_->GetKeepDatas().playerPos_.y;

}

void BackgroundScreen::Draw() const
{
	//Sprite::GetInstance().Draw(SPRITE_ID::)
}
