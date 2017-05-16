#include "BackgroundScreen.h"
#include"../../world/World.h"
#include"../../graphic/Sprite.h"
#include"screenSupport/BGCharas/BackgroundPill.h"
#include"../../game/Random.h"

static const int defTimeCount_ = 60;
static const int defGenerateCharaCount = 10;


BackgroundScreen::BackgroundScreen(World * world) :world_(world)
{
	Vector2 cPos = Vector2(2000,-600);
	for (int i = 0; i < defGenerateCharaCount;i++) {
		charactersPosition_.push_back(cPos);
		isCharactersPositionUsed_.push_back(false);

		cPos.x += 100;
		cPos.y += 150;
	}
}

void BackgroundScreen::Init()
{
	End();

	Vector2 pPos = world_->GetKeepDatas().playerPos_;
	//characters_.push_back(new BackgroundPill(world_ , Vector2(pPos.x+2000, 500)));
	//characters_.push_back(new BackgroundPill(world_,Vector2(pPos.x+2000, 200)));
	//characters_.push_back(new BackgroundPill(world_,Vector2(pPos.x+2000,700)));

}

void BackgroundScreen::Update()
{
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
	//timeCount_--;
	//if (timeCount_ <= 0) {
	//	timeCount_ = defTimeCount_;
		
		Vector2 pPos = world_->GetKeepDatas().playerPos_;

		for (int i = 0; i < defGenerateCharaCount; i++) {

			if (isCharactersPositionUsed_[i])continue;

			int rand = Random::GetInstance().Range(0, 100);
			if (rand > 70) continue;

			//charactersPosition_[i];
			isCharactersPositionUsed_[i];

			characters_.push_back(new BackgroundPill(world_, charactersPosition_[i]+pPos));

		}

		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2000, -200)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2300, 0)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2500, 200)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2600, -400)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2900, 300)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 3200, 0)));
		//}
}
