#include "BackgroundScreen.h"
#include"../../world/World.h"
#include"../../graphic/Sprite.h"
#include"screenSupport\BGCharas\BackgroundPill.h"

static const int defTimeCount_=60;
BackgroundScreen::BackgroundScreen(World * world) :world_(world)
{
}

void BackgroundScreen::Init()
{
	End();

	Vector2 pPos = world_->GetKeepDatas().playerPos_;
	characters_.push_back(new BackgroundPill(world_ , Vector2(pPos.x+2000, 500)));
	characters_.push_back(new BackgroundPill(world_,Vector2(pPos.x+2000, 200)));
	characters_.push_back(new BackgroundPill(world_,Vector2(pPos.x+2000,700)));
}

void BackgroundScreen::Update()
{
	addBGCharacters();

	std::vector<BackgroundCharacters*>::iterator itr = characters_.begin();
	while (itr != characters_.end())
	{
		//size_t index = std::distance(characters_.begin(), itr);
		//characters_[index]->Update();
		(*itr)->Update();
		if ((*itr)->isDead()) {
			itr = characters_.erase(itr);
		}
		else {
			++itr;
		}
	}
	
	//for (auto& bgC : characters_) {
	//	bgC->Update();
	//	if (bgC->isDead());
	//}
}

void BackgroundScreen::Draw() const
{
	for (auto& bgC : characters_) {
		bgC->Draw();
	}
}

void BackgroundScreen::End()
{
	characters_.clear();

}

void BackgroundScreen::addBGCharacters()
{
	timeCount_--;
	if (timeCount_ <= 0) {
		timeCount_ = defTimeCount_;
		
		Vector2 pPos = world_->GetKeepDatas().playerPos_;
		characters_.push_back(new BackgroundPill(world_,Vector2(pPos.x+2000, 500)));
		characters_.push_back(new BackgroundPill(world_,Vector2(pPos.x+2000, 200)));
		characters_.push_back(new BackgroundPill(world_,Vector2(pPos.x+2000, 700)));

	}
}
