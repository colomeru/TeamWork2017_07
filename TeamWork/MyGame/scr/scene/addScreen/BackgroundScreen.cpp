#include "BackgroundScreen.h"
#include"../../world/World.h"
#include"../../graphic/Sprite.h"
#include"screenSupport/BGCharas/BackgroundPill.h"
#include"../../game/Random.h"
#include"../../Def.h"

static const int defTimeCount_ = 60;
static const int defGenerateCharaCount = 10;

static const int defDrawPointYAdd[3] = {0,30,60};

BackgroundScreen::BackgroundScreen(World * world) :world_(world)
{
	Vector2 cPos = Vector2(2000,0);
	for (int i = 0; i < defGenerateCharaCount;i++) {
		charactersPosition_.push_back(cPos);
		isCharactersPositionUsed_.push_back(false);

		cPos.x += 180;
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
	float myds=world_->GetKeepDatas().changeLaneLerpPos_;
	Vector2 addpos=Vector2::Lerp(Vector2::Zero,Vector2(0,30),myds) *world_->GetKeepDatas().nextLane_;
	Vector2 bgPos = Vector2(0,Sprite::GetInstance().GetSize(SPRITE_ID::BACKGROUND_SPRITE).y-WINDOW_HEIGHT) - addpos - Vector2(0, defDrawPointYAdd[world_->GetKeepDatas().playerLane_]);
	Sprite::GetInstance().Draw(SPRITE_ID::BACKGROUND_SPRITE, bgPos);

	Vector2 fencePos = Vector2(0, WINDOW_HEIGHT - Sprite::GetInstance().GetSize(SPRITE_ID::FENCE_SPRITE).y) - addpos - Vector2(0,defDrawPointYAdd[world_->GetKeepDatas().playerLane_]);
	Sprite::GetInstance().Draw(SPRITE_ID::FENCE_SPRITE, fencePos);


	for (auto& bgC : characters_) {
		bgC->Draw();
	}
	DrawFormatString(700, 700, GetColor(255, 255, 255), "%f", myds);
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
		
	for (auto& isC : isCharactersPositionUsed_) {
		isC = false;
	}

		Vector2 pPos = world_->GetKeepDatas().playerPos_;

		for (int i = 0; i < defGenerateCharaCount; i++) {

			if (isCharactersPositionUsed_[i])continue;

			int rand = Random::GetInstance().Range(0, 100);
			if (rand > 90) continue;

			int yPos = Random::GetInstance().Range(0, 9);


			//charactersPosition_[i];
			isCharactersPositionUsed_[i]=true;

			characters_.push_back(new BackgroundPill(world_, Vector2(2000+(i*100),charactersPosition_[yPos].y)+pPos));

		}

		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2000, -200)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2300, 0)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2500, 200)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2600, -400)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2900, 300)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 3200, 0)));
		//}
}
